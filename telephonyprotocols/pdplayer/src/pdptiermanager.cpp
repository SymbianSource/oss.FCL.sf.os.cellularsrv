// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 @file
 @internalComponent
*/


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "pdptiermanagerTraces.h"
#endif

#include "pdptiermanager.h"
#include "mbmsengine.h"
#include "pdptiermanagerselector.h"
#include <comms-infras/ss_log.h>
#include <comms-infras/coretiermanagerstates.h>
#include <comms-infras/coretiermanageractivities.h>
#include <pcktcs.h>

#ifdef SYMBIAN_TRACE_ENABLE
#define KPDPTierMgrTag KESockMetaConnectionTag
#endif

#include "pdpmcpr.h"

using namespace Messages;
using namespace MeshMachine;
using namespace ESock;
using namespace ConnectionServ;
using namespace NetStateMachine;
using namespace CommsDat;

namespace PDPTierManagerActivities
{
DECLARE_DEFINE_ACTIVITY_MAP(stateMap)
ACTIVITY_MAP_END_BASE(TMActivities, coreTMActivities)
}


CContextTypeChecker::CContextTypeChecker(RPacketService& aPacketService, MContextEventsObserver& aCallback):CActive(EPriorityStandard),
	iPacketService(aPacketService), iCallback(aCallback)
	{
	CActiveScheduler::Add(this);
	}

CContextTypeChecker::~CContextTypeChecker()
	{
	iContextName = NULL;
	}

void CContextTypeChecker::Start(const TName* aContextName)
	{
	iContextName = aContextName;
	iPacketService.EnumerateContextsInNif(iStatus, *iContextName, iCountInNif);
	SetActive();
	}

void CContextTypeChecker::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	if (iCountInNif == 1) // This is a primary context
		{
		iCallback.PrimaryContextAddedL(iContextName);
		}
	else
		{
		iCallback.SecondaryContextAdded(iContextName);
		}
	}

void CContextTypeChecker::DoCancel()
	{
	iPacketService.CancelAsyncRequest(EPacketEnumerateContextsInNif);
	}

TInt CContextTypeChecker::RunError(TInt aError)
	{
	// Report an error
	iCallback.ContextTypeCheckingError(iContextName, aError);
	return KErrNone; // Make ActiveScheduler happy
	}

CPrimaryContextsMonitor::CPrimaryContextsMonitor(RPacketService& aPacketService, MContentionObserver& aCallback) : CActive(EPriorityStandard),
	iPacketService(aPacketService), iCallback(aCallback)
	{
	CActiveScheduler::Add(this);
	}

CPrimaryContextsMonitor::~CPrimaryContextsMonitor()
	{
	Cancel();
	iContextMonitors.ResetAndDestroy();
	delete iContextTypeChecker;
	iAddedContextsNames.ResetAndDestroy();
	}

void CPrimaryContextsMonitor::StartL()
	{
	iContextTypeChecker = new(ELeave) CContextTypeChecker(iPacketService, *this);
	iState = EEnumeratingContexts;
	iPacketService.EnumerateNifs(iStatus, iInitialNifsCount);
	SetActive();
	}

void CPrimaryContextsMonitor::PrimaryContextAddedL(const TName* aContextName)
	{
	// Create new status monitor for this context
	StartContextStatusMonitoringL(*aContextName);
	RemoveContextNameAndCheckNext(aContextName);
	}

void CPrimaryContextsMonitor::SecondaryContextAdded(const TName* aContextName)
	{
	// This is not a primary context, just delete its name.
	RemoveContextNameAndCheckNext(aContextName);
	}

void CPrimaryContextsMonitor::RemoveContextNameAndCheckNext(const TName* aContextName)
	{
	TInt nameIndex = iAddedContextsNames.Find(aContextName);
	__ASSERT_DEBUG(nameIndex != KErrNotFound, User::Invariant());
	delete iAddedContextsNames[nameIndex];
	iAddedContextsNames.Remove(nameIndex);

	if (iAddedContextsNames.Count() > 1)
		// Should be more than one here, coz we are waiting for new context added all the time,
		// so the last one item is always empty.
		{
		iContextTypeChecker->Start(iAddedContextsNames[0]);
		}
	}

void CPrimaryContextsMonitor::PrimaryContextDeleted(const CContextStatusMonitor* aContextStatusMonitor)
	{
	if (aContextStatusMonitor->IsPassedThroughActiveState())
		{
		iCallback.ContentionResolved();
		}
	DeleteContextStatusMonitor(aContextStatusMonitor);
	}

void CPrimaryContextsMonitor::ContextTypeCheckingError(const TName* aContextName, TInt aError)
	{
	RemoveContextNameAndCheckNext(aContextName);
	ProcessError(aError);
	}


void CPrimaryContextsMonitor::ContextMonitoringError(const CContextStatusMonitor* aContextStatusMonitor, TInt aError)
	{
	DeleteContextStatusMonitor(aContextStatusMonitor);
	ProcessError(aError);
	}

void CPrimaryContextsMonitor::DeleteContextStatusMonitor(const CContextStatusMonitor* aContextStatusMonitor)
	{
	TInt monitorIndex = iContextMonitors.Find(aContextStatusMonitor);
	__ASSERT_DEBUG(monitorIndex != KErrNotFound, User::Invariant());
	delete iContextMonitors[monitorIndex];
	iContextMonitors.Remove(monitorIndex);
	}

void CPrimaryContextsMonitor::ProcessError(TInt aError)
	{
	__ASSERT_DEBUG(aError != KErrNone, User::Invariant());
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPRIMARYCONTEXTSMONITOR_PROCESSERROR_1, ("PDP context monitoring error: %d"), aError);
	(void)aError;  //needed for debug builds 
	}

void CPrimaryContextsMonitor::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	SwitchStateL();
	RPacketService::TNifInfoV2Pckg nifInfoV2Pckg(iCurrentNifInfo);
	switch(iState)
		{
		case EGettingInfo:
			StartContextStatusMonitoringL(iCurrentNifInfo.iContextName);
			iPacketService.GetNifInfo(iStatus, iCurrentNifIndex, nifInfoV2Pckg);
			++iCurrentNifIndex;
			SetActive();
			break;
		case EListening:
			// All functions in RPacketService and RPacketContext,
			// that can be used to check if context is primary, are asynchronous.
			// We could not call them here, coz can miss some events from NotifyContextAdded
			// So subscribe to NotifyContextAdded as soon as possible and check context
			// type using CContextTypeChecker active object
			TName *contextName = new (ELeave) TName;
			CleanupStack::PushL(contextName);
			iAddedContextsNames.AppendL(contextName);
			CleanupStack::Pop(contextName);
			// subscribe to NotifyContextAdded
			iPacketService.NotifyContextAdded(iStatus, *contextName);
			SetActive();
			// if there are any items in iAddedContextsNames except that one that has been added above, i.e count > 1
			// starting asynchronous context type checking
			if (!iContextTypeChecker->IsActive() && iAddedContextsNames.Count()>1)
				{
				iContextTypeChecker->Start(iAddedContextsNames[0]);
				}
			break;
		}
	}
void CPrimaryContextsMonitor::DoCancel()
	{
	switch(iState)
		{
		case EEnumeratingContexts:
			iPacketService.CancelAsyncRequest(EPacketEnumerateNifs);
			break;
		case EGettingInfo:
			iPacketService.CancelAsyncRequest(EPacketGetNifInfo);
			break;
		case EListening:
			iPacketService.CancelAsyncRequest(EPacketNotifyContextAdded);
			break;
		}

	}

TInt CPrimaryContextsMonitor::RunError(TInt aError)
	{
	// Process an error
	ProcessError(aError);
	return KErrNone; // Make ActiveScheduler happy
	}


void CPrimaryContextsMonitor::SwitchStateL()
	{
	if (iState == EEnumeratingContexts)
		{
		if (iInitialNifsCount > 0)
			{
			iState = EGettingInfo;
			}
		else
			{
			iState = EListening;
			}
		}
	else if (iState == EGettingInfo && iCurrentNifIndex == iInitialNifsCount)
		{
		StartContextStatusMonitoringL(iCurrentNifInfo.iContextName);
		iState = EListening;
		}
	}

void CPrimaryContextsMonitor::StartContextStatusMonitoringL(const TName& aContextName)
	{
	CContextStatusMonitor* newStatusMonitor = new (ELeave) CContextStatusMonitor(iPacketService, *this);
	CleanupStack::PushL(newStatusMonitor);
	iContextMonitors.AppendL(newStatusMonitor);
	CleanupStack::Pop(newStatusMonitor);
	newStatusMonitor->StartL(aContextName);
	}

CContextStatusMonitor::CContextStatusMonitor(RPacketService& aPacketService, MContextEventsObserver& aCallback):CActive(EPriorityStandard),
	iPacketService(aPacketService), iCallback(aCallback)
	{
	CActiveScheduler::Add(this);
	}

CContextStatusMonitor::~CContextStatusMonitor()
	{
	Cancel();
	iPacketContext.Close();
	}

TBool CContextStatusMonitor::StartL(const TName& aContextName)
	{
	iContextName.Copy(aContextName);
	User::LeaveIfError(iPacketContext.OpenExistingContext(iPacketService, iContextName));
	User::LeaveIfError(iPacketContext.GetStatus(iContextStatus));

	if (iContextStatus == RPacketContext::EStatusDeleted)
		{
		return EFalse;
		}

	if (iContextStatus == RPacketContext::EStatusActive ||
		iContextStatus == RPacketContext::EStatusSuspended ||
		iContextStatus == RPacketContext::EStatusDeactivating)
		{
		iWasActive = ETrue;
		}

	iPacketContext.NotifyStatusChange(iStatus, iContextStatus);
	SetActive();
	return ETrue;
	}

void CContextStatusMonitor::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	if (iContextStatus == RPacketContext::EStatusActive)
		{
		iWasActive = ETrue;
		}

	if (iContextStatus == RPacketContext::EStatusDeleted)
		{
		iPacketContext.CancelAsyncRequest(EPacketContextNotifyStatusChange);
		iCallback.PrimaryContextDeleted(this);
		}
	else
		{
		iPacketContext.NotifyStatusChange(iStatus, iContextStatus);
		SetActive();
		}
	}

void CContextStatusMonitor::DoCancel()
	{
	iPacketContext.CancelAsyncRequest(EPacketContextNotifyStatusChange);
	}

TInt CContextStatusMonitor::RunError(TInt aError)
	{
	iCallback.ContextMonitoringError(this, aError);
	return KErrNone;
	}

CPdpContentionManager* CPdpContentionManager::NewL(const ESock::CTierManagerBase& aTierManager, RPacketService& aPacketService)
	{
	CPdpContentionManager* self = new (ELeave) CPdpContentionManager(aTierManager);
	CleanupStack::PushL(self);
	self->ConstructL(aPacketService);
	CleanupStack::Pop(self);
	return self;
	}

CPdpContentionManager::CPdpContentionManager(const ESock::CTierManagerBase& aTierManager):
CContentionManager(aTierManager)
	{
	}

CPdpContentionManager::~CPdpContentionManager()
	{
	delete iPrimaryContextsMonitor;
	}

void CPdpContentionManager::ConstructL(RPacketService& aPacketService)
	{
	iPrimaryContextsMonitor = new (ELeave) CPrimaryContextsMonitor(aPacketService, *this);
	}

void CPdpContentionManager::StartMonitoringL()
	{
	if (iPrimaryContextsMonitor)
		{
		iPrimaryContextsMonitor->StartL();
		}
	}

void CPdpContentionManager::ContentionResolved(const TContentionRequestItem& aContentionRequest, TBool aResult)
	{
	CPdpMetaConnectionProvider* pdpMcpr = static_cast<CPdpMetaConnectionProvider*>(aContentionRequest.iMcpr);
	
	pdpMcpr->ContentionResolved(aContentionRequest.iPendingCprId, aResult);
	}

void CPdpContentionManager::ContentionOccured(ESock::CMetaConnectionProviderBase& aMcpr)
	{
	CPdpMetaConnectionProvider& pdpMcpr = static_cast<CPdpMetaConnectionProvider&>(aMcpr);
	
	pdpMcpr.ContentionOccured();
	}

void CPdpContentionManager::ReportContentionAvailabilityStatus(ESock::CMetaConnectionProviderBase& aMcpr, const ESock::TAvailabilityStatus& aStatus) const
	{
	CPdpMetaConnectionProvider& pdpMcpr = static_cast<CPdpMetaConnectionProvider&>(aMcpr);
	
	pdpMcpr.ReportContentionAvailabilityStatus(aStatus);
	}

/**
The NewL factory function for PDPTierManager.
This function also acts as the single ECom entry point into this object.
@param aFactory The  reference to CTierManagerFactoryBase
@return CPDPTierManager*
*/
CPDPTierManager* CPDPTierManager::NewL(ESock::CTierManagerFactoryBase& aFactory)
	{
 	CPDPTierManager* self = new(ELeave)CPDPTierManager(aFactory, PDPTierManagerActivities::stateMap::Self());
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Constructor for CPDPTierManager
*/
CPDPTierManager::CPDPTierManager(ESock::CTierManagerFactoryBase& aFactory,
								const MeshMachine::TNodeActivityMap& aActivityMap)
	:CCoreTierManager(aFactory,aActivityMap)
	{
	LOG_NODE_CREATE(KPDPTierMgrTag, CPDPTierManager);
	}

/**
Descructor for CPDPTierManager
*/
CPDPTierManager::~CPDPTierManager()
	{
	LOG_NODE_DESTROY(KPDPTierMgrTag, CPDPTierManager);
	delete iPdpContentionManager;
	iPdpContentionManager = NULL;
	if (iMBMSEngine)
		{
		iMBMSEngine->Cancel();
		delete iMBMSEngine;
		iMBMSEngine = NULL;
		}
	}

/**
Create selector for this Tier.
@return MProviderSelector*
*/
MProviderSelector* CPDPTierManager::DoCreateProviderSelectorL(const Meta::SMetaData& aSelectionPreferences)
	{
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPTIERMANAGER_DOCREATEPROVIDERSELECTORL_1, "CPdpTierManager[%08x]::DoSelectProvider()", (TUint)this);
	return TPdpSelectorFactory::NewSelectorL(aSelectionPreferences);
	}

/**
ConstructL function of CPDPTierManager.
This function is used to retrieve tsy name.
@return void
*/
void CPDPTierManager::ConstructL()
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPTIERMANAGER_CONSTRUCTL_1, "ConstructL::In CPDPTierManager");
	iMBMSEngine = CMBMSEngine::NewL(NodeId(), *this);
	}

/**
 The PacketServiceAttachedCallbackL is a callback function.
 It's called by MBMSEngine when RPacketService has been attached.
*/
void CPDPTierManager::PacketServiceAttachedCallbackL()
	{
	if (!iPdpContentionManager)
		{
		iPdpContentionManager = CPdpContentionManager::NewL(*this, iMBMSEngine->GetRPacketService());
		}
	static_cast<CPdpContentionManager*>(iPdpContentionManager)->StartMonitoringL();
	}

/**
The ReceivedL function for PDPTierManager.
The messages from RConnectionServ are received in ReceivedL function.
@param aCFMessage The  reference to Messages::TSignatureBase
@return TInt*
*/
void CPDPTierManager::ReceivedL(const TRuntimeCtxId& aSender, const TNodeId& aRecipient, TSignatureBase& aMessage)
	{
	//TODO I think that the generic TCancel handling will do this - so perhaps remove this and CancelAndRemoveFromRequestList!
	if (aMessage.IsMessage<TEBase::TCancel>())
		{
		TRAPD(err, iMBMSEngine->CancelAndRemoveFromRequestListL(aSender));
		if (err == KErrNone)
			{
			aMessage.ClearMessageId();
			}
		else if (err != KErrNotFound)
			{
			User::Leave(err);
			}
		// err == KErrNotFound
		// Probably this is not cancel for MBMS request but cancel for PDP availability notification request
		// It will be processed by core TM then.
		}

	if (aMessage.IsMessage<TCFTierStatusProvider::TTierNotificationRegistration>())
		{
		TCFTierStatusProvider::TTierNotificationRegistration* msgTNR = message_cast<TCFTierStatusProvider::TTierNotificationRegistration>(&aMessage);

		const Messages::TNodeSignal::TMessageId requestType = TCFTierStatusProvider::TTierNotificationRegistration::Id();
		CRefCountOwnedParameterBundle* requestBundleOwner = msgTNR->iBundle;

		TRAPD(err, iMBMSEngine->AddToRequestListL(FindClient(aSender), aSender, requestType, requestBundleOwner));
		if (err == KErrNone)
			{
			aMessage.ClearMessageId();
			}
		else if (err != KErrArgument)
			{
			User::Leave(err);
			}
		// err == KErrArgument
		// Probably this is not MBMS notification request but PDP availability notification request
		// It will be processed by core TM then.
		}

	TNodeContext<CPDPTierManager> ctx(*this, aMessage, aSender, aRecipient);
	CCoreTierManager::Received(ctx);
	User::LeaveIfError(ctx.iReturn);
	}

TBool CPDPTierManager::HandleContentionL(ESock::CMetaConnectionProviderBase* aMcpr, Messages::TNodeId& aPendingCprId, TUint aPriority)
	{
	User::LeaveIfNull(iMBMSEngine);
	if (!iPdpContentionManager)
		{
		iPdpContentionManager = CPdpContentionManager::NewL(*this, iMBMSEngine->GetRPacketService());
		}
	return iPdpContentionManager->HandleContentionL(aMcpr, aPendingCprId, aPriority);
	}

TBool CPDPTierManager::IsUnavailableDueToContention(const ESock::CMetaConnectionProviderBase* aMetaConnectionProvider) const
	{
	if (!iPdpContentionManager)
		return EFalse;
	return iPdpContentionManager->IsUnavailableDueToContention(aMetaConnectionProvider);
	}


