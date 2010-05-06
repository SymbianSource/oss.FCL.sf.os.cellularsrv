// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// PDP config change notifier
// 
//

/**
 @file 
 @internalComponent
*/

#include <e32base.h>
#include <pcktcs.h>

#include "cpdpconfigchangenotifier.h"
#include "PDPFSM.h"
#include "spudteldebuglogger.h"
#include "pdpfsmnmspace.h"

/**
@param aId - pdp context id
@param aPacketContext - reference to etel packet context
@param aPdpFsmInterface - reference to pdp fsm interface
*/
CPdpConfigChangeNotifier* CPdpConfigChangeNotifier::NewL(TContextId aId, RPacketContext& aPacketContext, CPdpFsmInterface& aPdpFsmInterface)
	{
	CPdpConfigChangeNotifier* self = new (ELeave) CPdpConfigChangeNotifier(aId, aPacketContext, aPdpFsmInterface);
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop(self);
	
	return self;	
	}
/**
Initialise the scratch context buffer depending on the UMTS release obtained from 
PDP FSM interface.
*/
void CPdpConfigChangeNotifier::ConstructL()
	{
	TInt umtsRelease = iPdpFsmInterface.UmtsRelease();
	iScratchContext.CreateL(KScratchContextConfigSize);
	iScratchContext.SetMax();
	iScratchContext.FillZ();
     
    switch(umtsRelease)
		{
		case TPacketDataConfigBase::KConfigGPRS:
			{
			RPacketContext::TContextConfigGPRS tmp;
			Mem::Copy(const_cast<TUint8*>(iScratchContext.Ptr()), &tmp, sizeof(tmp));
			}
			break;

		case TPacketDataConfigBase::KConfigRel99Rel4:
			{
			RPacketContext::TContextConfigR99_R4 tmp;
			Mem::Copy(const_cast<TUint8*>(iScratchContext.Ptr()), &tmp, sizeof(tmp));
			}
			break;

		case TPacketDataConfigBase::KConfigRel5:
			{
			RPacketContext::TContextConfig_R5 tmp;
			Mem::Copy(const_cast<TUint8*>(iScratchContext.Ptr()), &tmp, sizeof(tmp));
			}
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		}
	}



/**
@param aId - pdp context id
@param aPacketContext - reference to etel packet context
@param aPdpFsmInterface - reference to pdp fsm interface
*/
CPdpConfigChangeNotifier::CPdpConfigChangeNotifier(TContextId aId,
										RPacketContext& aPacketContext,
										CPdpFsmInterface& aPdpFsmInterface)
: CEtelDriverNotifier(aPdpFsmInterface),
  iId(aId),
  iPacketContext(aPacketContext)
	{
	}

CPdpConfigChangeNotifier::~CPdpConfigChangeNotifier()
	{
	Cancel();
	iScratchContext.Close();
	}

/** initiate notification */
void CPdpConfigChangeNotifier::Start()
	{
	if (!IsActive())
		{ 
		iPacketContext.NotifyConfigChanged(iStatus, iScratchContext);
		SetActive();
		}
	}

/** stops notification */	
void CPdpConfigChangeNotifier::DoCancel()
	{
	if(IsActive())
		{ 
		SPUDTELVERBOSE_INFO_LOG(
			_L("CPdpConfigChangeNotifier::DoCancel EPacketContextNotifyConfigChanged"));
		iPacketContext.CancelAsyncRequest(EPacketContextNotifyConfigChanged); 
		}
	}

/** notifies pdp fsm 

@param aStatus - request status
*/
void CPdpConfigChangeNotifier::Notify(const TRequestStatus& aStatus)
	{
	if(aStatus == KErrNone)
		{
		SPUDTELVERBOSE_INFO_LOG(_L("FSM set ConfigGPRS"));
		iPdpFsmInterface.Set(iId, GetScratchContextAs<TPacketDataConfigBase>());
		SPUDTELVERBOSE_INFO_LOG(_L("FSM input EConfigGPRSChangeNetwork"));
		iPdpFsmInterface.Input(iId, PdpFsm::EConfigGPRSChangeNetwork);
		}
	else
		{ 
		SPUDTEL_ERROR_LOG(_L("CPdpConfigChangeNotifier::Notify(), error: %d"), 
						aStatus.Int());
		// Not all TSYs support RPacketContext::NotifyConfigChanged so we handle
		// KErrNotSupported silently (PDEF118981).
		ASSERT((aStatus == KErrCancel) || (aStatus == KErrNotSupported));
		}
	}



/**
@param aId - pdp context id
@param aPacketContext - reference to etel packet context
@param aPdpFsmInterface - reference to pdp fsm interface
*/
CMbmsPdpConfigChangeNotifier::CMbmsPdpConfigChangeNotifier(TContextId aId,
										RPacketMbmsContext& aPacketContext,
										CPdpFsmInterface& aPdpFsmInterface)
: CEtelDriverNotifier(aPdpFsmInterface),
  iId(aId),
  iMbmsPacketContext(aPacketContext),
  iConfigMbms(),
  iConfigMbmsPckg(iConfigMbms)
	{
	}

CMbmsPdpConfigChangeNotifier::~CMbmsPdpConfigChangeNotifier()
	{
	Cancel();
	}

/** initiate notification */
void CMbmsPdpConfigChangeNotifier::Start()
	{
	if (!IsActive())
		{ 
		iMbmsPacketContext.NotifyConfigChanged(iStatus, iConfigMbmsPckg); 
		SetActive();
		}
	}

/** stops notification */	
void CMbmsPdpConfigChangeNotifier::DoCancel()
	{
	if(IsActive())
		{ 
		SPUDTELVERBOSE_INFO_LOG(
			_L("CMbmsPdpConfigChangeNotifier::DoCancel EPacketContextNotifyConfigChanged"));
		iMbmsPacketContext.CancelAsyncRequest(EPacketContextNotifyConfigChanged); 
		}
	}

/** notifies pdp fsm 

@param aStatus - request status
*/
void CMbmsPdpConfigChangeNotifier::Notify(const TRequestStatus& aStatus)
	{
	if(aStatus == KErrNone)
		{
		SPUDTELVERBOSE_INFO_LOG(_L("FSM set ConfigMbms"));
		iPdpFsmInterface.Set(iId, iConfigMbms);
		SPUDTELVERBOSE_INFO_LOG(_L("FSM input EConfigMbmsChangeNetwork"));
		iPdpFsmInterface.Input(iId, PdpFsm::EConfigGPRSChangeNetwork,SpudMan::EMbms);
		//to be discussed...
		
		}
	else
		{ 
		SPUDTEL_ERROR_LOG(_L("CMbmsPdpConfigChangeNotifier::Notify(), error: %d"), 
						aStatus.Int());
		// Not all TSYs support RPacketContext::NotifyConfigChanged so we handle
		// KErrNotSupported silently (PDEF118981).
		ASSERT((aStatus == KErrCancel) || (aStatus == KErrNotSupported));
		}
	}



