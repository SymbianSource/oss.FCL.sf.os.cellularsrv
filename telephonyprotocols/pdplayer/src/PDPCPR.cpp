// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// PDP Connection Provider implementation
// 
//

/**
 @file
 @internalComponent
*/

#include "PDPCPR.h"
#include <comms-infras/ss_log.h>
#include <comms-infras/ss_msgintercept.h>
#include <comms-infras/corecpractivities.h>
#include <elements/nm_address.h>
#include <comms-infras/ss_nodemessages_parameters.h>
#include <comms-infras/corescprstates.h>
#include <elements/mm_context.h>
#include <etelmm.h>
#include <etelpckt.h>
#include <comms-infras/ss_metaconnprov.h>
#include "pdpprovision.h"
#include <etel.h>
#include <networking/cfbearers.h>
#include "pdpcpravailabilitylistener.h"
#include <comms-infras/ss_nodemessages.h>
#include <networking/ipcpr_states.h>
#include <comms-infras/linkmessages.h>
#include <elements/nm_interfaces.h>
#include <cs_genevent.h>
#include <networking/etelbearers.h>

using namespace ESock;


//-=========================================================
//
// CPDPConnectionProvider methods
//
//-=========================================================	

//We reserve space for two preallocated activities that may start concurrently on the CPR
//node: destroy and data client stop.
static const TUint KDefaultMaxPreallocatedActivityCount = 2;
static const TUint KMaxPreallocatedActivitySize = sizeof(MeshMachine::CNodeRetryParallelActivity) + sizeof(MeshMachine::APreallocatedOriginators<4>);
static const TUint KPDPCPRPreallocatedActivityBufferSize = KDefaultMaxPreallocatedActivityCount * KMaxPreallocatedActivitySize;

namespace PDPCprLinkCharacteristicActivity
{
DECLARE_DEFINE_NODEACTIVITY(ECFActivityParamRequest, PDPCprLinkCharacteristic, TCFScpr::TGetParamsRequest)
    NODEACTIVITY_ENTRY(MeshMachine::KNoTag, PDPCprStates::TUpdateBundleAndRespondWithRetrievedParams, PRStates::TAwaitingParamRequest, MeshMachine::TNoTag)
NODEACTIVITY_END()
}

// Agent SCPR Going Down Activity
namespace PDPDataClientGoneDownActivity
{
DECLARE_DEFINE_NODEACTIVITY(ECFActivityDataClientGoneDown, PDPScprGoneDown, TCFControlProvider::TDataClientGoneDown)
    // AwaitingDataClientGoneDown used rather than AlwaysAccept to mark data client EStopped
    FIRST_NODEACTIVITY_ENTRY(CoreNetStates::TAwaitingDataClientGoneDown, CoreNetStates::TNoTagOrNonDefault)
    LAST_NODEACTIVITY_ENTRY(MeshMachine::KNoTag, PRStates::TSendGoneDown)
    LAST_NODEACTIVITY_ENTRY(CoreNetStates::KNonDefault, MeshMachine::TDoNothing)    
NODEACTIVITY_END()
}

namespace PDPCprActivities
{
DEFINE_ACTIVITY_MAP(activityMap)
    ACTIVITY_MAP_ENTRY(PDPDataClientGoneDownActivity, PDPScprGoneDown)
	ACTIVITY_MAP_ENTRY(PDPCprLinkCharacteristicActivity, PDPCprLinkCharacteristic)
ACTIVITY_MAP_END_BASE(CprActivities, coreCprActivities)
}


CPDPConnectionProvider* CPDPConnectionProvider::NewL(ESock::CConnectionProviderFactoryBase& aFactory)
    {
    CPDPConnectionProvider* provider = new (ELeave) CPDPConnectionProvider(aFactory);
    CleanupStack::PushL(provider);
    provider->ConstructL();
    CleanupStack::Pop(provider);
	return provider;
    }        


void CPDPConnectionProvider::StartListener()
/**
 * Start listening for dynamic caps or network mode changes.
 * @param None
 * @return void
 */
	{
	iDynamicCapsEventListener->NotifyDynamicCapsChange(this);
	iNetworkModeEventListener->NotifyNetworkModeChange(this);
	}

void CPDPConnectionProvider::ConstructL()
/**
 * PDP Connection Provider Second Phase Constructor
 * @param None
 * @return void
 */
	{
	CCoreConnectionProvider::ConstructL(KPDPCPRPreallocatedActivityBufferSize);
	}

void CPDPConnectionProvider::StopListener()
/**
 * Start listening for dynamic caps or network mode changes.
 */
	{
	if(iDynamicCapsEventListener)
		{
		iDynamicCapsEventListener->Cancel();
		}
	if(iNetworkModeEventListener)
		{
		iNetworkModeEventListener->Cancel();
		}
	}

CPDPConnectionProvider::CPDPConnectionProvider(ESock::CConnectionProviderFactoryBase& aFactory)
	: CCoreConnectionProvider(aFactory, PDPCprActivities::activityMap::Self())
/**
 * Construct PDP connection provider.
 */	
    {        
    LOG_NODE_CREATE(KESockConnectionTag, CPDPConnectionProvider);
    }

CPDPConnectionProvider::~CPDPConnectionProvider()
/**
 * Destroy PDP connection provider.
 */
    {    
    LOG_NODE_DESTROY(KESockConnectionTag, CPDPConnectionProvider);
    StopListener();
	delete iDynamicCapsEventListener;
	delete iNetworkModeEventListener;
    }
    
void CPDPConnectionProvider::ReceivedL(const Messages::TRuntimeCtxId& aSender, const Messages::TNodeId& aRecipient, Messages::TSignatureBase& aMessage)
	{
	//ESOCK_DEBUG_MESSAGE_INTERCEPT(aSender, aMessage, aRecipient);
	MeshMachine::TNodeContext<CPDPConnectionProvider> ctx(*this, aMessage, aSender, aRecipient);
	CCoreConnectionProvider::Received(ctx);
	User::LeaveIfError(ctx.iReturn);
	}
	

void CPDPConnectionProvider::BearerChangeDetectedL()
/**
 * Update the parmeter set in case of modulation change and send TPlaneNotification message to
 * all the control client.
 */
	{
	//Update bearers.
	UpdateBearer();
	
	//Send msg to all control clients
	CEventParamsChanged* eventChanged = CEventParamsChanged::NewL(KBearerInfo);
	
	CleanupStack::PushL(eventChanged);
	//Create the family and set it to bearer type 
	XBearerInfo* bearerInfo = XBearerInfo::NewL();
	CleanupStack::PushL(bearerInfo);
	
	bearerInfo->SetBearerType(iBearerType);
	
	//Add bearer family to parameter set.
	eventChanged->AddParameterSetL(KBearerInfo, bearerInfo);
	CleanupStack::Pop(2,eventChanged);
	
	//Create the event node for bearer change notification.
	CRefCountOwnedNotification* eventNode = new (ELeave) CRefCountOwnedNotification(eventChanged);
	
	//Build the CPR plane notification message, which will be retrieved by IPCPR.
	TCFSubConnControlClient::TPlaneNotification msg(eventNode);
	
   	Messages::TClientIter<Messages::TDefaultClientMatchPolicy> iter = this->GetClientIter<Messages::TDefaultClientMatchPolicy>(Messages::TClientType(TCFClientType::ECtrl));
    Messages::RNodeInterface* ctl;
	while ((ctl = iter++) != NULL)
		{
		msg.iRefCountOwnedNotification->Open();
		ctl->PostMessage(this->NodeId(), msg);
		}

	}


void CPDPConnectionProvider::UpdateBearer()
/**
 * Update bearer type based on the change of the dynamic caps or network mode.
 */
	{
	//Get dynamic caps
	TUint dynamicCaps = iDynamicCapsEventListener->DynamicCaps();		
	//Get network mode
	RMobilePhone::TMobilePhoneNetworkMode networkMode = iNetworkModeEventListener->NetworkMode();
	
	//Resolve the bearer based on both dynamic caps and network mode
	iBearerType = Bearer(dynamicCaps, networkMode);
	iBearerSet = ETrue;
	}


TUint32 CPDPConnectionProvider::Bearer(TUint aDynamicCaps, RMobilePhone::TMobilePhoneNetworkMode& aNetworkMode)
/**
 * Determine bearer based on dynamic caps and network mode.
 * @param aDynamicCaps a value of dynamic caps
 * @param aNetworkMode a value of network mode
 * @return Type of Bearer
 *
 */
	{
	
	if ((aDynamicCaps & RPacketService::KCapsHSUPA) || (aDynamicCaps & RPacketService::KCapsHSDPA))
		{
		return KHsdpaBearer;
		}
	else if (aDynamicCaps & RPacketService::KCapsEGPRS)
		{
		return KEGprsBearer;
		}
	else
		{
		if(aNetworkMode == RMobilePhone::ENetworkModeGsm)
			{
		    return KGprsBearer;
			}
		else if(aNetworkMode == RMobilePhone::ENetworkModeWcdma)
			{
		    return KUmtsBearer;
			}
		else
			{
		   	return KDefaultBearer;
			}
		}
	}


DEFINE_SMELEMENT(PDPCprStates::TUpdateBundle, NetStateMachine::MStateTransition, PDPCprStates::TContext)
void PDPCprStates::TUpdateBundle::DoL()
/**
 * Process TGetParamsRequest
 */
	{
	// Node receives TGetParamsRequest msg containing the bundle.
	// Determine current network type - GPRS/EDGE/UMTS/HSDPA and update the bundle.
	const CTSYProvision& tsyProvision = static_cast<const CTSYProvision&>(iContext.Node().AccessPointConfig().FindExtensionL(
	        STypeId::CreateSTypeId(CTSYProvision::EUid, CTSYProvision::ETypeId)));
	if(!iContext.Node().iDynamicCapsEventListener)
		{
		iContext.Node().iDynamicCapsEventListener = CPDPCPRDynamicCapsListener::NewL(&tsyProvision);
		}
	if (!iContext.Node().iNetworkModeEventListener)
		{
		iContext.Node().iNetworkModeEventListener = CPDPCPRNetworkModeListener::NewL(&tsyProvision);
		}

	if(iContext.Node().iBearerSet == EFalse)
		{
		iContext.Node().UpdateBearer();
		}
	
	TCFScpr::TGetParamsRequest& paramRequest = Messages::message_cast<TCFScpr::TGetParamsRequest>(iContext.iMessage);
	if( (! paramRequest.iFamilyBundle.IsNull()) && (iContext.Node().GetParameterBundle() != paramRequest.iFamilyBundle))
		{
		iContext.Node().GetParameterBundle().Open(paramRequest.iFamilyBundle);
		RParameterFamily family = iContext.Node().GetParameterBundle().FindFamily(KBearerInfo);	
 
		if(!family.IsNull())
			{
			XBearerInfo *bearerType = static_cast<XBearerInfo*>(family.FindParameterSet(STypeId::CreateSTypeId(KIpBearerInfoUid, KIpBearerInfoParameterType), RParameterFamily::ERequested));
		
			if(bearerType)
				{
				bearerType->SetBearerType(iContext.Node().iBearerType);
				}
			}
		}

	if(iContext.Node().iDynamicCapsEventListener->IsDynamicCapsSupported())
		{
		// if Dynamic Caps supported, start listener, which would Notify of Dynamic Caps change
		// no check required for network mode, since atleast one network mode is supported.
		iContext.Node().StartListener();
		}
	}


