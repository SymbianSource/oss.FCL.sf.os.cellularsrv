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
// implementation of etel driver context class
// 
//

/**
 @file 
 @internalComponent
*/


#include "ceteldrivercontext.h"
#include "ceteldriverfactory.h"
#include "spudteldebuglogger.h"


using namespace EtelDriver;
	

CEtelDriverContext* CEtelDriverContext::NewL(TContextId aId, SpudMan::TPdpContextType aContextType, CEtelDriverFactory& aFactory)
	{
        CEtelDriverContext* self  = new (ELeave) CEtelDriverContext(aId, aContextType, aFactory);
        CleanupStack::PushL(self);
        self->ConstructL();
        CleanupStack::Pop(self);
        return self;
	}

void CEtelDriverContext::ConstructL()
	{
    iContextConfig.CreateL(PdpFsm::KContextConfigBufferSize);
	iContextConfig.SetMax();
	iContextConfig.FillZ();

	switch(PdpFsmInterface().UmtsRelease())
		{
		case TPacketDataConfigBase::KConfigGPRS:
			{
			RPacketContext::TContextConfigGPRS tmp;
			Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &tmp, sizeof(tmp));
			}
			break;

		case TPacketDataConfigBase::KConfigRel99Rel4:
			{
			RPacketContext::TContextConfigR99_R4 tmp;
			Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &tmp, sizeof(tmp));
			}
			break;

		case TPacketDataConfigBase::KConfigRel5:
			{
			RPacketContext::TContextConfig_R5 tmp;
			Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &tmp, sizeof(tmp));
			}
			break;
		default:
			User::Leave(KErrNotSupported);
			break;

		}	
	}

CEtelDriverContext::CEtelDriverContext (TContextId aId, SpudMan::TPdpContextType aContextType, CEtelDriverFactory& aFactory)
: CActive(CActive::EPriorityStandard),
  iId(aId),
  iContextType(aContextType),
  iCompletionStatus(KErrNone),
  iStrategyStep(MEtelDriverStrategy::EFinishStep),
  iStrategyId(ESentinelStrategy),
  iFactory(aFactory),
  iQosRequestedPckg(iQosRequested),
  iQosNegotiatedPckg(iQosNegotiated),

  iDataChannelV2(),
  iDataChannelV2Pckg(iDataChannelV2),
  iPcktMbmsSessionList(NULL),
  iContextConfigMbms(),
  iContextConfigMbmsPckg(iContextConfigMbms),
  iFilterV2(),
  iFilterV2Pckg(iFilterV2)
 	{
	SPUDTEL_FNLOG("CEtelDriverContext::CEtelDriverContext");
	CActiveScheduler::Add(this);
  	}
CEtelDriverContext::~CEtelDriverContext()
	{
	SPUDTEL_FNLOG("CEtelDriverContext::~CEtelDriverContext()");
	Cancel(); // N.B. This cancels all outstanding operations on the context, including deletion!
	iContextConfig.Close();

	// Guarantees proper release of all handles.
	// If everything is closed already, this does no harm.
	iPacketQoS.Close(); // Close QoS first, a buggy TSY may not handle it the other way.
	iPacketContext.Close(); // At this point the reference count on the context is zero.
	// TSY should cleanly dispose of the context, if it had not done so already.
	
	// This is necessary only in a situtation where Spudman is destroyed while a strategy
	// on a context is outstanding: in this case deletion of SpudTel results in cancellation
	// of all outstanding operations, which is likely to result in handle leak.
	// Under other circumstances, the handles will be closed via an appropriate strategy.
	
	//delete if MBMS sessions structures.
	iMbmsPacketContext.Close();
	if (iMbmsSession)
		{
		delete iMbmsSession;
		delete iPcktMbmsSessionList;
		}
	iSessionInfo.iSessionIds.Close();	//RArray should be closed.
	
	}

/** initiates a new request 

@param aOperation - type of an etel driver request 

@return KErrInUse if pdp context has active strategy
*/
TInt CEtelDriverContext::Input (TEtelInput aOperation)
	{
	SPUDTEL_FNLOG("CEtelDriverContext::Input()");
	SPUDTELVERBOSE_INFO_LOG1( _L("Operation %d"), aOperation );
	
	if (MEtelDriverStrategy::EFinishStep != iStrategyStep)
		{
		SPUDTEL_ERROR_LOG(_L("ERROR: Pdp context is in use, return %d"), KErrInUse);
		// I'm still doing something
		ASSERT(EFalse); // shouldn't happen
		return KErrInUse;
		}
	
	iStrategyId = iFactory.StrategyId(aOperation);
	iStrategyStep = MEtelDriverStrategy::EStartStep;
	iCompletionStatus = KErrNone;
	
	SetActive();
	Strategy(iStrategyId).Next(*this, &iStatus);

	return KErrNone;
	}


void CEtelDriverContext::RunL()
	{
	SPUDTEL_FNLOG("CEtelDriverContext::RunL()");
	ASSERT(iStrategyId < ESentinelStrategy);
	
	if(iStatus != KErrNone)
		{
		if(iCompletionStatus == KErrNone)
			{ 
			iCompletionStatus = iStatus; 
			SPUDTEL_ERROR_LOG(_L("Last async request completed with error %d"), iStatus.Int());
			}
		// Don't continue with the strategy for all cases except Delete
		// N.B.: deletion of a context has to be done till the very last step
		//	to ensure proper cleanup of resources.
		if(EContextDeleteStrategy != iStrategyId)
			{
			iStrategyStep = MEtelDriverStrategy::EFinishStep;
			}
		SPUDTELVERBOSE_INFO_LOG(_L("Strategy is completed"));
		}
	
	if(MEtelDriverStrategy::EFinishStep == iStrategyStep)
		{
		// we are done
		SPUDTELVERBOSE_INFO_LOG(_L("Strategy is completed"));
		Strategy(iStrategyId).NotifyFsm (*this, iCompletionStatus);
		}
	else
		{
		// continue with next step
		SetActive();
		Strategy(iStrategyId).Next(*this, &iStatus);
		}
	}

/** cancels last async request */  	
void CEtelDriverContext::DoCancel()
	{
	SPUDTEL_FNLOG("CEtelDriverContext::DoCancel()");

	if(IsActive())
		{
		// delegate to strategy	
		Strategy(iStrategyId).CancelAsyncRequest(*this);
		}
	iStrategyStep = MEtelDriverStrategy::EFinishStep;
	SPUDTELVERBOSE_INFO_LOG(_L("Strategy is cancelled"));
	}

/** accessor */
const TName& CEtelDriverContext::ExistingContextName() const 
	{ 
   	SPUDTEL_FNLOG("CEtelDriverContext::ExistingContextName()");
	for (TContextId i = 0; i < static_cast<TContextId>(iFactory.ContextCount()); i++)
		{
        
		if (iFactory.HasContext(i) && iFactory.Context(i).Name().Size())
			{
			return iFactory.Context(i).Name(); 
			}
		}
		
	// Unacceptable situation: we didn't create a single context yet
	SPUDTEL_ERROR_LOG(_L("CEtelDriverContext::ExistingContextName - can't find existing context. return %S"), &iFactory.Context(0).Name());
	ASSERT(EFalse);
	return iFactory.Context(0).Name(); 
	}

/** accessor 

@return reference to etel RPhone
*/
RPhone& CEtelDriverContext::Phone() const
	{ 
	return iFactory.Phone(); 
	}

/** accessor 

@return reference to etel RPacketService
*/
RPacketService& CEtelDriverContext::PacketService() const
	{ 
	return iFactory.PacketService(); 
	}

/** accessor 

@return reference to etel driver strategy
*/
MEtelDriverStrategy& CEtelDriverContext::Strategy(TEtelDriverStrategy aId) const
	{ 
	return iFactory.Strategy(aId); 
	}

/** accessor 

@return reference to pdp fsm interface
*/
CPdpFsmInterface& CEtelDriverContext::PdpFsmInterface() const
	{ 
	return iFactory.PdpFsmInterface(); 
	}

/** start pdp notifications */
void CEtelDriverContext::StartNotifications() const
	{ 
	iFactory.StartPdpNotifications(iId); 
	}

/** stops pdp notifications */
void CEtelDriverContext::StopNotifications() const
	{ 
	iFactory.CancelPdpNotifications(iId); 
	}


#ifdef _DEBUG
void CEtelDriverContext::DumpReqProfileParameters ()
    {
    SPUDTEL_INFO_LOG1( _L("Requested Profile Parameters Dump - Context Id = %d"), Id());
    SPUDTEL_INFO_LOG( _L("==========================================================="));
    SPUDTEL_INFO_LOG1( _L("ExtensionId = %d"), iQosRequested.ExtensionId());
    
    if (iQosRequested.ExtensionId() != TPacketDataConfigBase::KConfigRel99Rel4
        && iQosRequested.ExtensionId() != TPacketDataConfigBase::KConfigRel5)
        {
        SPUDTEL_INFO_LOG( _L("Invalid/Unsupported ExtensionId"));
        return;
        }

    if (iQosRequested.ExtensionId() == TPacketDataConfigBase::KConfigRel99Rel4
        || iQosRequested.ExtensionId() == TPacketDataConfigBase::KConfigRel5)
        {
        SPUDTEL_INFO_LOG1( _L("[1]iReqTrafficClass = %d"), iQosRequested.RequestedQoSR99_R4().iReqTrafficClass);
        SPUDTEL_INFO_LOG1( _L("[2]iMinTrafficClass = %d"), iQosRequested.RequestedQoSR99_R4().iMinTrafficClass);
        SPUDTEL_INFO_LOG1( _L("[3]iReqDeliveryOrderReqd = %d"), iQosRequested.RequestedQoSR99_R4().iReqDeliveryOrderReqd);
        SPUDTEL_INFO_LOG1( _L("[4]iMinDeliveryOrderReqd = %d"), iQosRequested.RequestedQoSR99_R4().iMinDeliveryOrderReqd);
        SPUDTEL_INFO_LOG1( _L("[5]iReqDeliverErroneousSDU = %d"), iQosRequested.RequestedQoSR99_R4().iReqDeliverErroneousSDU);
        SPUDTEL_INFO_LOG1( _L("[6]iMinDeliverErroneousSDU = %d"), iQosRequested.RequestedQoSR99_R4().iMinDeliverErroneousSDU);
        SPUDTEL_INFO_LOG1( _L("[7]iReqMaxSDUSize = %d"), iQosRequested.RequestedQoSR99_R4().iReqMaxSDUSize);
        SPUDTEL_INFO_LOG1( _L("[8]iMinAcceptableMaxSDUSize = %d"), iQosRequested.RequestedQoSR99_R4().iMinAcceptableMaxSDUSize);
        SPUDTEL_INFO_LOG1( _L("[9]iReqMaxRate.iUplinkRate = %d"), iQosRequested.RequestedQoSR99_R4().iReqMaxRate.iUplinkRate);
        SPUDTEL_INFO_LOG1( _L("[10]iReqMaxRate.iDownlinkRate = %d"), iQosRequested.RequestedQoSR99_R4().iReqMaxRate.iDownlinkRate);
        SPUDTEL_INFO_LOG1( _L("[11]iMinAcceptableMaxRate.iUplinkRate = %d"), iQosRequested.RequestedQoSR99_R4().iMinAcceptableMaxRate.iUplinkRate);
        SPUDTEL_INFO_LOG1( _L("[12]iMinAcceptableMaxRate.iDownlinkRate = %d"), iQosRequested.RequestedQoSR99_R4().iMinAcceptableMaxRate.iDownlinkRate);
        SPUDTEL_INFO_LOG1( _L("[13]iReqBER = %d"), iQosRequested.RequestedQoSR99_R4().iReqBER);
        SPUDTEL_INFO_LOG1( _L("[14]iMaxBER = %d"), iQosRequested.RequestedQoSR99_R4().iMaxBER);
        SPUDTEL_INFO_LOG1( _L("[15]iReqSDUErrorRatio = %d"), iQosRequested.RequestedQoSR99_R4().iReqSDUErrorRatio);
        SPUDTEL_INFO_LOG1( _L("[16]iMaxSDUErrorRatio = %d"), iQosRequested.RequestedQoSR99_R4().iMaxSDUErrorRatio);
        SPUDTEL_INFO_LOG1( _L("[17]iReqTrafficHandlingPriority = %d"), iQosRequested.RequestedQoSR99_R4().iReqTrafficHandlingPriority);
        SPUDTEL_INFO_LOG1( _L("[18]iReqTrafficHandlingPriority = %d"), iQosRequested.RequestedQoSR99_R4().iMinTrafficHandlingPriority);
        SPUDTEL_INFO_LOG1( _L("[19]iReqTransferDelay = %d"), iQosRequested.RequestedQoSR99_R4().iReqTransferDelay);
        SPUDTEL_INFO_LOG1( _L("[20]iMaxTransferDelay = %d"), iQosRequested.RequestedQoSR99_R4().iMaxTransferDelay);
        SPUDTEL_INFO_LOG1( _L("[21]iReqGuaranteedRate.iUplinkRate = %d"), iQosRequested.RequestedQoSR99_R4().iReqGuaranteedRate.iUplinkRate);
        SPUDTEL_INFO_LOG1( _L("[22]iReqGuaranteedRate.iDownlinkRate = %d"), iQosRequested.RequestedQoSR99_R4().iReqGuaranteedRate.iDownlinkRate);
        SPUDTEL_INFO_LOG1( _L("[23]iMinGuaranteedRate.iUplinkRate = %d"), iQosRequested.RequestedQoSR99_R4().iMinGuaranteedRate.iUplinkRate);
        SPUDTEL_INFO_LOG1( _L("[24]iMinGuaranteedRate.iDownlinkRate = %d"), iQosRequested.RequestedQoSR99_R4().iMinGuaranteedRate.iDownlinkRate);
        }
    
#ifdef SYMBIAN_NETWORKING_UMTSR5
    if (iQosRequested.ExtensionId() == TPacketDataConfigBase::KConfigRel5)
        {
        SPUDTEL_INFO_LOG1( _L("[25]iSignallingIndication = %d"), iQosRequested.RequestedQoSR5().iSignallingIndication);
        SPUDTEL_INFO_LOG1( _L("[26]iSourceStatisticsDescriptor = %d"), iQosRequested.RequestedQoSR5().iSourceStatisticsDescriptor);
        }
#endif        
    }



void CEtelDriverContext::DumpNegProfileParameters ()
    {
    SPUDTEL_INFO_LOG1( _L("Negotiated Profile Parameters Dump - Context Id = %d"), Id());
    SPUDTEL_INFO_LOG( _L("==========================================================="));
    SPUDTEL_INFO_LOG1( _L("ExtensionId = %d"), iQosNegotiated.ExtensionId());
    
    if (iQosNegotiated.ExtensionId() != TPacketDataConfigBase::KConfigRel99Rel4
        && iQosNegotiated.ExtensionId() != TPacketDataConfigBase::KConfigRel5)
        {
        SPUDTEL_INFO_LOG( _L("Invalid/Unsupported ExtensionId"));
        return;
        }

    if (iQosNegotiated.ExtensionId() == TPacketDataConfigBase::KConfigRel99Rel4
        || iQosNegotiated.ExtensionId() == TPacketDataConfigBase::KConfigRel5)
        {
        SPUDTEL_INFO_LOG1( _L("[2]iTrafficClass = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iTrafficClass);
        SPUDTEL_INFO_LOG1( _L("[3]iDeliveryOrderReqd = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iDeliveryOrderReqd);
        SPUDTEL_INFO_LOG1( _L("[4]iDeliverErroneousSDU = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iDeliverErroneousSDU);
        SPUDTEL_INFO_LOG1( _L("[5]iMaxSDUSize = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iMaxSDUSize);
        SPUDTEL_INFO_LOG1( _L("[6]iBER = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iBER);
        SPUDTEL_INFO_LOG1( _L("[7]iSDUErrorRatio = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iSDUErrorRatio);
        SPUDTEL_INFO_LOG1( _L("[8]iTrafficHandlingPriority = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iTrafficHandlingPriority);
        SPUDTEL_INFO_LOG1( _L("[9]iTransferDelay = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iTransferDelay);
        SPUDTEL_INFO_LOG1( _L("[10]iGuaranteedRate.iUplinkRate = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iGuaranteedRate.iUplinkRate);
        SPUDTEL_INFO_LOG1( _L("[11]iGuaranteedRate.iDownlinkRate = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iGuaranteedRate.iDownlinkRate);
        SPUDTEL_INFO_LOG1( _L("[12]iMaxRate.iUplinkRate = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iMaxRate.iUplinkRate);
        SPUDTEL_INFO_LOG1( _L("[13]iMaxRate.iDownlinkRate = %d"), iQosNegotiated.NegotiatedQoSR99_R4().iMaxRate.iDownlinkRate);
        }

    
#ifdef SYMBIAN_NETWORKING_UMTSR5
    if (iQosNegotiated.ExtensionId() == TPacketDataConfigBase::KConfigRel5)
        {
        SPUDTEL_INFO_LOG1( _L("[14]iSignallingIndication = %d"), iQosNegotiated.NegotiatedQoSR5().iSignallingIndication);
        SPUDTEL_INFO_LOG1( _L("[15]iSourceStatisticsDescriptor = %d"), iQosNegotiated.NegotiatedQoSR5().iSourceStatisticsDescriptor);
        }
#endif
    }
    
#endif
// _DEBUG
