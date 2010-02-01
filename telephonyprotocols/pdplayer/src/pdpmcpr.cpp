// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Pdp MCPR
// 
//

/**
 @file
 @internalComponent
*/

#include <comms-infras/ss_log.h>
#include "pdpmcpr.h"
#include "pdpmcprstates.h"
#include "PDPProvision.h"
#include "psdavailabilitylistener.h"
#include <comms-infras/agentmcpractivities.h>
#include <comms-infras/coremcpractivities.h>
#include <comms-infras/ss_msgintercept.h>
#include <elements/nm_messages_errorrecovery.h>

using namespace Messages;
using namespace MeshMachine;
using namespace ESock;
using namespace NetStateMachine;
using namespace MCprActivities;


// No Bearer Activity
namespace PdpMCPRNoBearerActivity
{
DECLARE_DEFINE_NODEACTIVITY(ECFActivityNoBearer, activity, TCFControlProvider::TNoBearer)
	FIRST_NODEACTIVITY_ENTRY(CoreNetStates::TAwaitingNoBearer, MeshMachine::TNoTag)
	LAST_NODEACTIVITY_ENTRY(KNoTag, CoreNetStates::TSendBearer)
NODEACTIVITY_END()
}

namespace PdpMCprErrorRecoveryActivity
{
DECLARE_DEFINE_CUSTOM_NODEACTIVITY(ECFActivityErrorRecovery, PdpMCprErrorRecovery, TEErrorRecovery::TErrorRecoveryRequest, CPdpErrorRecoveryActivity::NewL)
	FIRST_NODEACTIVITY_ENTRY(MCprStates::TAwaitingErrorRecoveryRequest, MeshMachine::TNoTag)
	THROUGH_NODEACTIVITY_ENTRY(KNoTag, CPdpErrorRecoveryActivity::TProcessErrorRecoveryReq, CPdpErrorRecoveryActivity::TPdpContentionNoTagOrError)
	NODEACTIVITY_ENTRY(KNoTag, MeshMachine::TDoNothing, CPdpErrorRecoveryActivity::TAwaitingContentionResult, MeshMachine::TNoTag)
	LAST_NODEACTIVITY_ENTRY(KNoTag, CPdpErrorRecoveryActivity::TProcessContentionResult)
	LAST_NODEACTIVITY_ENTRY(KErrorTag, MeshMachine::TDoNothing)
NODEACTIVITY_END()
}

// Activity Map
namespace PdpMCprStates
{
DECLARE_DEFINE_ACTIVITY_MAP(stateMap)
	ACTIVITY_MAP_ENTRY(PdpMCPRNoBearerActivity, activity)
	ACTIVITY_MAP_ENTRY(PdpMCprErrorRecoveryActivity, PdpMCprErrorRecovery)
ACTIVITY_MAP_END_BASE(AgentMCprActivities, agentMCprActivities)
} // namespace PdpMCprStates

namespace PdpMCprErrorRecoveryActivity
{
CNodeActivityBase* CPdpErrorRecoveryActivity::NewL( const MeshMachine::TNodeActivity& aActivitySig, MeshMachine::AMMNodeBase& aNode )
	{
	TUint c = GetNextActivityCountL(aActivitySig,aNode);
	return new(ELeave)CPdpErrorRecoveryActivity(aActivitySig, aNode, c);
	}

CPdpErrorRecoveryActivity::CPdpErrorRecoveryActivity(const MeshMachine::TNodeActivity& aActivitySig, MeshMachine::AMMNodeBase& aNode, TUint aActivitiesCount)
:	MeshMachine::CNodeParallelActivityBase(aActivitySig, aNode, aActivitiesCount)
	{
	}
	
CPdpErrorRecoveryActivity::~CPdpErrorRecoveryActivity()
	{	
	}
}

//-=========================================================
//
//CPdpMetaConnectionProvider implementation
//
//-=========================================================

CPdpMetaConnectionProvider* CPdpMetaConnectionProvider::NewL(ESock::CMetaConnectionProviderFactoryBase& aFactory, const TProviderInfo& aProviderInfo)
    {
    // coverity[alloc_fn] coverity[alias] coverity[assign]
    CPdpMetaConnectionProvider* provider = new (ELeave) CPdpMetaConnectionProvider(aFactory, aProviderInfo);
    // coverity[push]
    CleanupStack::PushL(provider);
    // coverity[alias] coverity[double_push]
    provider->ConstructL();
    CleanupStack::Pop(provider);
    return provider;
    }

void CPdpMetaConnectionProvider::ConstructL()
    {
    CAgentMetaConnectionProvider::ConstructL();
    SetAccessPointConfigFromDbL();
    }

CPdpMetaConnectionProvider::CPdpMetaConnectionProvider(CMetaConnectionProviderFactoryBase& aFactory, const TProviderInfo& aProviderInfo)
:	CAgentMetaConnectionProvider(aFactory, aProviderInfo, PdpMCprStates::stateMap::Self())
	{
	LOG_NODE_CREATE(KPdpMCprSubTag, CPdpMetaConnectionProvider);
	}

CPdpMetaConnectionProvider::~CPdpMetaConnectionProvider()
	{
	CPdpMetaConnectionProvider::CancelAvailabilityMonitoring(); //Don't call virtual for obvious reasons!
	LOG_NODE_DESTROY(KPdpMCprSubTag, CPdpMetaConnectionProvider);
	}

void CPdpMetaConnectionProvider::ReceivedL(const TRuntimeCtxId& aSender, const TNodeId& aRecipient, TSignatureBase& aMessage)
    {
	__CFLOG_VAR((KPdpMCprTag, KPdpMCprSubTag, _L8("CPdpMetaConnectionProvider [this=%08x]::ReceivedL() aMessage=%d"),
	   this, aMessage.MessageId().MessageId()));

	ESOCK_DEBUG_MESSAGE_INTERCEPT(aSender, aMessage, aRecipient);

	TNodeContext<CPdpMetaConnectionProvider> ctx(*this, aMessage, aSender, aRecipient);
    CCoreMetaConnectionProvider::Received(ctx);
    User::LeaveIfError(ctx.iReturn);
	}

void CPdpMetaConnectionProvider::SetAccessPointConfigFromDbL()
    {
    RMetaExtensionContainer mec;
    mec.Open(AccessPointConfig());
    CleanupClosePushL(mec);
    
	CCommsDatIapView* iapView = OpenIapViewLC();

	mec.AppendExtensionL(CIPConfig::NewFromGPRSOutLC(iapView));
	CleanupStack::Pop();
	mec.AppendExtensionL(CBCAProvision::NewLC(iapView));
	CleanupStack::Pop();
   	mec.AppendExtensionL(CTSYProvision::NewLC(iapView));
   	CleanupStack::Pop();
	mec.AppendExtensionL(CImsExtProvision::NewLC(iapView));
	CleanupStack::Pop();
	
	CGPRSProvision* gprsProvision = CGPRSProvision::NewLC(iapView);
	mec.AppendExtensionL(gprsProvision);
	CleanupStack::Pop(gprsProvision);
	
	//It's legal for the qos defaults to be absent.
	//in this case they're going to be supplied by
	//GuQoS.
	CDefaultPacketQoSProvision* defaultQoS = NULL;
	TRAP_IGNORE(defaultQoS = CDefaultPacketQoSProvision::NewL(iapView));
	if (defaultQoS)
    	{
    	CleanupStack::PushL(defaultQoS);
     	mec.AppendExtensionL(defaultQoS);
     	CleanupStack::Pop(defaultQoS);
    	}
	
	CRawIpAgentConfig* rawIpAgentConfig = CRawIpAgentConfig::NewLC(iapView, &gprsProvision->GetScratchContextAs<TPacketDataConfigBase>());
	mec.AppendExtensionL(rawIpAgentConfig);
	CleanupStack::Pop();

    mec.AppendExtensionL(CPppLcpConfig::NewLC(iapView));
	CleanupStack::Pop();
	mec.AppendExtensionL(CPppAuthConfig::NewLC(iapView));
	CleanupStack::Pop();

    CPppProvisionInfo* agentProvision = new (ELeave) CPppProvisionInfo;
    CleanupStack::PushL(agentProvision);
    agentProvision->SetIsDialIn(KErrNotSupported);
    agentProvision->SetNotificationData(NULL);
    mec.AppendExtensionL(agentProvision);
    CleanupStack::Pop(agentProvision);

	CleanupStack::PopAndDestroy();			// CloseIapView();

	iAccessPointConfig.Close();
	iAccessPointConfig.Open(mec);
	CleanupStack::PopAndDestroy(&mec);
	}

void CPdpMetaConnectionProvider::StartAvailabilityMonitoringL(const TNodeCtxId& aAvailabilityActivity)
	{
	ASSERT(iAvailabilityListener==NULL); //Only one start allowed from the av activity!
	const CTSYProvision& config = static_cast<const CTSYProvision&>(AccessPointConfig().FindExtensionL(STypeId::CreateSTypeId(CTSYProvision::EUid, CTSYProvision::ETypeId)));
	iAvailabilityListener = CPsdAvailabilityListener::NewL(aAvailabilityActivity, config, ProviderInfo().APId(), GetTierManager()->IsUnavailableDueToContention(this));
	AddClientL(iAvailabilityListener->Id(), TClientType(TCFClientType::ERegistrar, TCFClientType::EAvailabilityProvider));
	}

void CPdpMetaConnectionProvider::CancelAvailabilityMonitoring()
	{
	if (iAvailabilityListener)
		{
		RemoveClient(iAvailabilityListener->Id());
		iAvailabilityListener = NULL; //iAvailabilityListener will delete itself when cancelled from the availability activity
		}
	}
	
void CPdpMetaConnectionProvider::ContentionOccured()
	{
	// Send a stop request to the stop activity.
	TInt stopCode = KErrConnectionContention;
	RClientInterface::OpenPostMessageClose(NodeId(), NodeId(), TCFDataClient::TStop(stopCode).CRef());
	}

void CPdpMetaConnectionProvider::ContentionResolved(const Messages::TNodeId& aPendingCprId, TBool aResult)
	{
	TPdpContentionMessages::TPdpContentionResultMessage msg(aPendingCprId, aResult);
	TRAP_IGNORE(ReceivedL(TNodeId(Id()), TNodeId(Id()), msg));
	}

void CPdpMetaConnectionProvider::ReportContentionAvailabilityStatus(const TAvailabilityStatus& aAvailabilityStatus) const
	{
	if (iAvailabilityListener)
		{
		iAvailabilityListener->ReportContentionAvailabilityStatusIfRequired(aAvailabilityStatus);
		}
	}

