// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// A collection of etel driver strategies
// 
//

/**
 @file 
 @internalComponent
*/



#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "eteldriverstrategiesTraces.h"
#endif

#include <e32def.h>

#include "ceteldrivercontext.h"
#include "eteldriverstrategies.h"
#include "pdpfsmnmspace.h"

#include <pcktcs.h>
using namespace ConnectionServ;

//
//	TOpenStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TOpenStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TOPENSTRATEGY_NEXT_1, "TOpenStrategy::Next()");
	
	TInt err = KErrNone;
	
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			aContext.Phone().Initialise (*aStatus);
			aContext.SetStrategyStep(EInitPhoneStep);
			break;
			}
			
		case EInitPhoneStep:
			{
			aContext.SetStrategyStep(EFinishStep);
			break;
			}
			
		default:
			// incorrect step
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TOPENSTRATEGY_NEXT_2, "Incorrect step");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
				
		} // switch

	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	}
	

/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TOpenStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus)
	{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TOPENSTRATEGY_NOTIFYFSM_1, "Notifying FSM: EPhoneOpened");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPhoneOpened);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TOPENSTRATEGY_NOTIFYFSM_2, "Notifying FSM: EPhoneOpenedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPhoneOpenedFailed, aCompletionStatus.Int());
		}
	}

/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TOpenStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	switch(aContext.StrategyStep())
		{
		case EInitPhoneStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TOPENSTRATEGY_CANCELASYNCREQUEST_1, "Cancel Phone::InitialiseCancel");
			aContext.Phone().InitialiseCancel();
			break;
			}
			
		default:
			// there're NO outstanding async requests
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TOPENSTRATEGY_CANCELASYNCREQUEST_2, "ERROR: Incorrect case in TOpenStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	}


//
//	TContextDeleteStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TContextDeleteStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCONTEXTDELETESTRATEGY_NEXT_1, "TContextDeleteStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)");
	
	TInt err = KErrNone;
	
	// steps:
	//	-- cancel notifications
	//	-- deactivate context
	//	-- close QoS only for Primary & secondary, for MBMS just skip
	//	-- close context
	// 	-- delete context
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			// cancel associated notifications
			// -- 	StatusChangeNotifier
			// --	QoSChangeNotifier
			// --	ConfigChangeNotifier
			aContext.StopNotifications();
			
			if (aContext.ContextType() != SpudMan::EMbms)
				{
				if (aContext.PacketContext().SubSessionHandle())
					{
					aContext.PacketContext().Deactivate(*aStatus);
					aContext.SetStrategyStep (EDeactivateStep);
					}
				else
					{
					aContext.SetStrategyStep(EDeactivateStep);
					User::RequestComplete(aStatus, err);
					}
				}
			else
				{
				if (aContext.MbmsPacketContext().SubSessionHandle())
					{
					aContext.MbmsPacketContext().Deactivate(*aStatus);
					aContext.SetStrategyStep (EDeactivateStep);
					}
				else
					{
					aContext.SetStrategyStep(EDeleteContextStep);
					User::RequestComplete(aStatus, err);
					}	
				}	
			break;
			}
			
		case EDeactivateStep:
			{
			if (aContext.ContextType() != SpudMan::EMbms)
				{
				if (aContext.PacketQoS().SubSessionHandle())
					{
					aContext.PacketQoS().Close();
					}
				
				if (aContext.PacketContext().SubSessionHandle())
					{
					aContext.PacketContext().Delete(*aStatus);
					aContext.SetStrategyStep(EDeleteContextStep);
					}
				else
					{
					// jump to next step
					aContext.SetStrategyStep(EDeleteContextStep);
					User::RequestComplete( aStatus, err);
					}
				}
			else
				{
				if (aContext.MbmsPacketContext().SubSessionHandle())
					{
					aContext.MbmsPacketContext().Delete(*aStatus);
					aContext.SetStrategyStep(EDeleteContextStep);
					}
				else
					{
					// jump to next step
					aContext.SetStrategyStep(EDeleteContextStep);
					User::RequestComplete( aStatus, err);
					}	
				}	
			break;
			}
			
			
		case EDeleteContextStep:
			{
			if (aContext.ContextType() != SpudMan::EMbms)
				{
				aContext.PacketContext().Close();
				}
			else
				{
				aContext.MbmsPacketContext().Close();
				}	
			// clear the name
			aContext.Name().Zero();
			aContext.SetStrategyStep (EFinishStep);
			break;
			}
			
		default:
			// incorrect step
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCONTEXTDELETESTRATEGY_NEXT_2, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
				
		} // switch

	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}


/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TContextDeleteStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus)
	{
	aContext.PdpFsmInterface().Set(aContext.Id(), RPacketContext::EStatusDeleted);
	
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCONTEXTDELETESTRATEGY_NOTIFYFSM_1, "Notifying FSM: EContextDeleted");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EContextDeleted);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCONTEXTDELETESTRATEGY_NOTIFYFSM_2, "Notifying FSM: EContextDeletedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EContextDeletedFailed, aCompletionStatus.Int());
		}
	}

/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TContextDeleteStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCONTEXTDELETESTRATEGY_CANCELASYNCREQUEST_1, "TContextDeleteStrategy::CancelAsyncRequest()");
	
	// When there is no sub-session, the strategy will not call DeactivateContext, but will be in the EDeactivateStep state
	// In this case we don't want to cancel the deactivate request
	if (aContext.iStatus != KRequestPending)
		{
		return;
		}
		
	switch(aContext.StrategyStep())
		{
		case EDeactivateStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCONTEXTDELETESTRATEGY_CANCELASYNCREQUEST_2, "Cancel PacketContext::EPacketContextDeactivate");
			aContext.PacketContext().CancelAsyncRequest(EPacketContextDeactivate);
			break;
			}
			
		case EDeleteContextStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCONTEXTDELETESTRATEGY_CANCELASYNCREQUEST_3, "Cancel PacketContext::EPacketContextDelete");
			aContext.PacketContext().CancelAsyncRequest(EPacketContextDelete);
			break;
			}
			
		default:
			// there're NO outstanding async requests
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCONTEXTDELETESTRATEGY_CANCELASYNCREQUEST_4, "ERROR: Incorrect case");
			ASSERT(EFalse);
			break;
		}
	}


// 
//	TCreate1ryPdpContextStrategy 
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TCreate1ryPdpContextStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_NEXT_1, "TCreate1ryPdpContextStrategy::Next()");
	
	TInt err = KErrNone;
	
	// possible steps:
	// 	-- open new context
	//	-- open new QoS
	// 	-- initialise context
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			err = aContext.PacketContext().OpenNewContext (aContext.PacketService(), aContext.Name());
			if (err)
				{
				OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_NEXT_2, "PacketContextOpenNewContext returned %d", err);
				break;
				}
				
			aContext.PacketContext().InitialiseContext(*aStatus, aContext.DataChannelV2Pckg());						
			aContext.SetStrategyStep (EInitialiseContextStep);
			break;
			}
			
		case EInitialiseContextStep:
			{
			// get packet config from fsm
			aContext.PdpFsmInterface().Get (aContext.Id(), aContext.ContextPacketDataConfigBase());
			// async set config
			aContext.PacketContext().SetConfig (*aStatus, aContext.ContextConfig());
			// set next step
			aContext.SetStrategyStep (ESetConfigStep);
			break;
			}
			
		case ESetConfigStep:
			{
			// QoS name is not used anywhere
			TName newName;
			err = aContext.PacketQoS().OpenNewQoS (aContext.PacketContext(), newName);
			if (err)
				{ 
				OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_NEXT_3, "PacketQoS OpenNewQoS returned %d", err);
				break; 
				}
				
#ifdef SYMBIAN_NETWORKING_UMTSR5
            RPacketQoS::TQoSR5Requested r5;
            aContext.PdpFsmInterface().Get (aContext.Id(), r5);
			aContext.QosRequested() = r5;
#else
            RPacketQoS::TQoSR99_R4Requested r4;
			aContext.PdpFsmInterface().Get (aContext.Id(), r4);
			aContext.QosRequested() = r4;
#endif 
// SYMBIAN_NETWORKING_UMTSR5


#if (OST_TRACE_CATEGORY & OST_TRACE_CATEGORY_DEBUG)
            aContext.DumpReqProfileParameters ();
#endif			
			aContext.PacketQoS().SetProfileParameters (*aStatus, aContext.QosRequestedPckg());
			aContext.SetStrategyStep (ESetProfileParamsStep);
			break;
			}
			
		case ESetProfileParamsStep:
			{
			aContext.SetStrategyStep (EFinishStep);
			break;
			}
			
		default:
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_NEXT_4, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}


/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TCreate1ryPdpContextStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus)
{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_NOTIFYFSM_1, "Notifying FSM: E1ryPdpContextCreated");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::E1ryPdpContextCreated);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_NOTIFYFSM_2, "Notifying FSM: E1ryPdpContextCreatedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::E1ryPdpContextCreatedFailed, aCompletionStatus.Int());
		}
}

/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TCreate1ryPdpContextStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_1, "TCreate1ryPdpContextStrategy::CancelAsyncRequest()");
	
	switch(aContext.StrategyStep())
		{
		case ESetConfigStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_2, "Cancel PacketContext::EPacketContextSetConfig");
			aContext.PacketContext().CancelAsyncRequest(EPacketContextSetConfig);
			break;
			}
			
		case ESetProfileParamsStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_3, "Cancel PacketContext::EPacketQoSSetProfileParams");
			aContext.PacketQoS().CancelAsyncRequest(EPacketQoSSetProfileParams);
			break;
			}
			
		case EInitialiseContextStep:
		    OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_4, "Cancel PacketContext::EPacketContextInitialiseContext");
		    aContext.PacketContext().CancelAsyncRequest(EPacketContextInitialiseContext);
		    break;
			
		default:
			// there're NO outstanding async requests
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE1RYPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_5, "ERROR: Incorrect case");
			ASSERT(EFalse);
			break;
		}
	}


// 
//	TCreate2ryPdpContextStrategy 
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TCreate2ryPdpContextStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE2RYPDPCONTEXTSTRATEGY_NEXT_1, "TCreate2ryPdpContextStrategy::Next()");
	
	TInt err = KErrNone;
	
	// possible steps:
	//	-- open new secondary context
	//	-- open new QoS
	//	-- initialise context
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			const TName& existingContextName = aContext.ExistingContextName();
			aContext.PacketContext().OpenNewSecondaryContext(aContext.PacketService(), existingContextName, aContext.Name());
			aContext.PacketContext().InitialiseContext(*aStatus, aContext.DataChannelV2Pckg());						
			aContext.SetStrategyStep (EInitialiseContextStep);
			break;
			}
		case EInitialiseContextStep:
			{
			// QoS name is not used anywhere
			TName newName;
			err = aContext.PacketQoS().OpenNewQoS (aContext.PacketContext(), newName);
			if (err)
				{ 
				OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE2RYPDPCONTEXTSTRATEGY_NEXT_2, "PacketQoS OpenNewQoS returned %d", err);
				break; 
				}
			aContext.SetStrategyStep (EFinishStep);
			break;
			}
			
		default:
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE2RYPDPCONTEXTSTRATEGY_NEXT_3, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}


/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TCreate2ryPdpContextStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE2RYPDPCONTEXTSTRATEGY_NOTIFYFSM_1, "Notifying FSM: E2ryPdpContextCreated");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::E2ryPdpContextCreated);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE2RYPDPCONTEXTSTRATEGY_NOTIFYFSM_2, "Notifying FSM: E2ryPdpContextCreatedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::E2ryPdpContextCreatedFailed, aCompletionStatus.Int());
		}
}

/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TCreate2ryPdpContextStrategy::CancelAsyncRequest(CEtelDriverContext& /*aContext*/)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE2RYPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_1, "TCreate2ryPdpContextStrategy::CancelAsyncRequest()");
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATE2RYPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_2, "ERROR: No outstanding requests");
	}


//
// TSetQoSStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TSetQoSStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETQOSSTRATEGY_NEXT_1, "TSetQoSStrategy::Next()");
	
	TInt err = KErrNone;
	
	// possible steps:
	//	-- set QoS profile parameters
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
#ifdef SYMBIAN_NETWORKING_UMTSR5
            RPacketQoS::TQoSR5Requested req;
#else
            RPacketQoS::TQoSR99_R4Requested req;
#endif 
// SYMBIAN_NETWORKING_UMTSR5

			aContext.PdpFsmInterface().Get (aContext.Id(), req);
			aContext.QosRequested() = req;	


#if (OST_TRACE_CATEGORY & OST_TRACE_CATEGORY_DEBUG)
            aContext.DumpReqProfileParameters ();
#endif			
            aContext.PacketQoS().SetProfileParameters (*aStatus, aContext.QosRequestedPckg());
			aContext.SetStrategyStep (ESetProfileParamsStep);
			break;
			}
			
		case ESetProfileParamsStep:
			{
			aContext.SetStrategyStep (EFinishStep);
			break;
			}
			
		default:
			{
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETQOSSTRATEGY_NEXT_2, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
			}
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}
	

/** 
FSM completion notification 
and setting QoSR99_R4Negotiated in spud FSM
*/	
void TSetQoSStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
	{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETQOSSTRATEGY_NOTIFYFSM_1, "Notifying FSM: EQoSSet");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EQoSSet);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETQOSSTRATEGY_NOTIFYFSM_2, "Notifying FSM: EQoSSetFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EQoSSetFailed, aCompletionStatus.Int());
		}
	}
	
/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TSetQoSStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	switch(aContext.StrategyStep())
		{
		case ESetProfileParamsStep:
			{
			aContext.PacketQoS().CancelAsyncRequest(EPacketQoSSetProfileParams);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETQOSSTRATEGY_CANCELASYNCREQUEST_1, "Cancel PacketQoS::EPacketQoSSetProfileParams");
			break;
			}
			
		default:
			// there're NO outstanding async requests
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETQOSSTRATEGY_CANCELASYNCREQUEST_2, "ERROR: Incorrect case in TSetQoSStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	}


//
// TSetTftStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TSetTftStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NEXT_1, "TSetTftStrategy::Next()");
	
	TInt err = KErrNone;

	// possible steps:
	// 	-- add filters
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			aContext.PdpFsmInterface().Get (aContext.Id(), aContext.TftOperationCode());
			switch(aContext.TftOperationCode())
				{
				case KAddFilters:
					aContext.PdpFsmInterface().Get (aContext.Id(), aContext.TftInfo());
					OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NEXT_2, "TftOperationCode - Add Filters %d", aContext.TftInfo().FilterCount());
					// Set strategy assumes that TFT has to be created on a first place
					OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NEXT_3, "Creating TFT...");
					if (aContext.ContextType() != SpudMan::EMbms)
						{	
						aContext.PacketContext().CreateNewTFT(*aStatus, aContext.TftInfo().FilterCount());
						}
					else
						{
						aContext.MbmsPacketContext().CreateNewTFT(*aStatus, aContext.TftInfo().FilterCount());
						}	
					aContext.SetStrategyStep (ECreateNewTFTStep);
					break;

				case KRemoveFilters:
					err = KErrNotSupported;
					OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NEXT_4, "TftOperationCode - Remove is not supported in a Set strategy, return %d", err);
					break;

				case KDeleteTFT:
					err = KErrNotSupported;
					OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NEXT_5, "TftOperationCode - Delete is not supported in a Set strategy, return %d", err);
					break;

				default:
					// wrong case
					OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NEXT_6, "ERROR: Incorrect case");
					ASSERT(EFalse);
					err = KErrNotSupported;
					break;
				}
			break;
			}
	
		case ECreateNewTFTStep:
			{
			TInt err1st = aContext.FirstFilterV2();
			if(err1st == KErrNone)
				{
				if (aContext.ContextType() != SpudMan::EMbms)
					{
					aContext.PacketContext().AddPacketFilter( *aStatus, aContext.FilterV2Pckg());
					}
				else
					{
					aContext.MbmsPacketContext().AddPacketFilter( *aStatus, aContext.FilterV2Pckg());
					}	
				aContext.SetStrategyStep (EAddTftStep);
				}
			else	// done
				{ 
				// internal check
				ASSERT((aContext.TftRequested() <= aContext.TftInfo().FilterCount()));
				aContext.SetStrategyStep (EFinishStep); 
				}
				
			break;
			}

		case EAddTftStep:
			{
			TInt errV2 = aContext.NextFilterV2();
			if(errV2 == KErrNone)
				{
				if (aContext.ContextType() != SpudMan::EMbms)
					{
					aContext.PacketContext().AddPacketFilter( *aStatus, aContext.FilterV2Pckg());
					}
				else
					{
					aContext.MbmsPacketContext().AddPacketFilter( *aStatus, aContext.FilterV2Pckg());
					}
				aContext.SetStrategyStep (EAddTftStep);
				}
			else	// done
				{ 
				// internal check
				ASSERT((aContext.TftRequested() <= aContext.TftInfo().FilterCount()));
				aContext.SetStrategyStep (EFinishStep); 
				}
				
			break;
			}

		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NEXT_7, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}
	
/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TSetTftStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
	{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NOTIFYFSM_1, "Notifying FSM: ETftSet");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::ETftSet);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_NOTIFYFSM_2, "Notifying FSM: ETftSetFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::ETftSetFailed, aCompletionStatus.Int());
		}
	}
	
/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TSetTftStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	switch(aContext.StrategyStep())
		{
		case ECreateNewTFTStep:
			{
			aContext.PacketQoS().CancelAsyncRequest(EPacketContextCreateNewTFT);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_CANCELASYNCREQUEST_1, "Cancel PacketQoS::EPacketContextCreateNewTFT");
			break;
			}
			
		case EAddTftStep:
			{
			aContext.PacketQoS().CancelAsyncRequest(EPacketContextAddPacketFilter);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_CANCELASYNCREQUEST_2, "Cancel PacketQoS::EPacketContextAddPacketFilter");
			break;
			}

		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TSETTFTSTRATEGY_CANCELASYNCREQUEST_3, "ERROR: Incorrect case in TSetTftStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	}


//
// TChangeTftStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TChangeTftStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_NEXT_1, "TChangeTftStrategy::Next()");
	
	TInt err = KErrNone;

	// special case. 
	if(EStartStep == aContext.StrategyStep())
		{
		aContext.PdpFsmInterface().Get (aContext.Id(), aContext.TftOperationCode());
		switch(aContext.TftOperationCode())
			{
			case KAddFilters:
				aContext.PdpFsmInterface().Get (aContext.Id(), aContext.TftInfo());
				OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_NEXT_2, "TftOperationCode - Add Filters %d", aContext.TftInfo().FilterCount());
				aContext.SetStrategyStep (EAddFirstTftStep);
				break;

			case KRemoveFilters:
				aContext.PdpFsmInterface().Get (aContext.Id(), aContext.TftInfo());
				OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_NEXT_3, "TftOperationCode - Remove %d Filters", aContext.TftInfo().FilterCount());
				aContext.SetStrategyStep (ERemoveFirstTftStep);
				break;

			case KDeleteTFT:
				OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_NEXT_4, "TftOperationCode - Delete TFT");
				// delete old TFT
				aContext.PacketContext().DeleteTFT(*aStatus);
				aContext.SetStrategyStep (EDeleteTftStep);
				// DeleteTFT() ia an async operation => return 
				return;

			default:
				// wrong case
				OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_NEXT_5, "ERROR: Incorrect case");
				ASSERT(EFalse);
				err = KErrNotSupported;
				break;
			}
		}
	
	// possible steps:
	//	-- delete TFT 
	// 	-- add filters
	// 	-- remove filters
	if(!err)
		{
		switch(aContext.StrategyStep())
			{
			case EDeleteTftStep:
				{
				aContext.SetStrategyStep (EChangeTftFinished);
				User::RequestComplete( aStatus, KErrNone);
				break;
				}
				
			case EAddFirstTftStep:
				{
				TInt errV2 = aContext.FirstFilterV2();
				if(errV2 == KErrNone)
					{
					if (aContext.ContextType() == SpudMan::EMbms)
						{
						aContext.MbmsPacketContext().AddPacketFilter( *aStatus, aContext.FilterV2Pckg());
						}
					else
						{	
						aContext.PacketContext().AddPacketFilter( *aStatus, aContext.FilterV2Pckg());
						}
					aContext.SetStrategyStep (EAddTftStep);
					}
				else	// done
					{ 
					aContext.SetStrategyStep (EChangeTftFinished);
					User::RequestComplete( aStatus, KErrNone);
					}
					
				break;
				}

			case EAddTftStep:
				{
				TInt errV2 = aContext.NextFilterV2();
				if(errV2 == KErrNone)
					{
					if (aContext.ContextType() == SpudMan::EMbms)
						{
						aContext.MbmsPacketContext().AddPacketFilter( *aStatus, aContext.FilterV2Pckg());
						}
					else
						{
						aContext.PacketContext().AddPacketFilter( *aStatus, aContext.FilterV2Pckg());
						}
					aContext.SetStrategyStep (EAddTftStep);
					}
				else	// done
					{ 
					aContext.SetStrategyStep (EChangeTftFinished);
					User::RequestComplete( aStatus, KErrNone);
					}
					
				break;
				}

			case ERemoveFirstTftStep:
				{
				TInt errV2 = aContext.FirstFilterV2();
				if(errV2 == KErrNone)
					{
					if (aContext.ContextType() == SpudMan::EMbms)
						{
						aContext.MbmsPacketContext().RemovePacketFilter( *aStatus, aContext.FilterV2().iId);
						}
					else
						{
						aContext.PacketContext().RemovePacketFilter( *aStatus, aContext.FilterV2().iId);
						}	
					aContext.SetStrategyStep (ERemoveTftStep);
					}
				else	// done
					{
					aContext.SetStrategyStep (EChangeTftFinished);
					User::RequestComplete( aStatus, KErrNone);
					}
					
				break;
				}
				
			case ERemoveTftStep:
				{
				TInt errV2 = aContext.NextFilterV2();
				if(errV2 == KErrNone)
					{
					if (aContext.ContextType() == SpudMan::EMbms)
						{
						aContext.MbmsPacketContext().RemovePacketFilter( *aStatus, aContext.FilterV2().iId);
						}
					else
						{				
						aContext.PacketContext().RemovePacketFilter( *aStatus, aContext.FilterV2().iId);
						}
					aContext.SetStrategyStep (ERemoveTftStep);
					}
				else	// done
					{
					aContext.SetStrategyStep (EChangeTftFinished);
					User::RequestComplete( aStatus, KErrNone);
					}
					
				break;
				}

			case EChangeTftFinished:
				{
				aContext.SetStrategyStep (EFinishStep);
				}
				break;

				
			default:
				// unexpected
				OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_NEXT_6, "ERROR: Incorrect case");
				ASSERT(EFalse);
				err = KErrNotSupported;
				break;
			}
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}
	

/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TChangeTftStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
	{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_NOTIFYFSM_1, "Notifying FSM: ETftChanged");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::ETftChanged);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_NOTIFYFSM_2, "Notifying FSM: ETftChangedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::ETftChangedFailed, aCompletionStatus.Int());
		}
	}
	
/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TChangeTftStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	switch(aContext.StrategyStep())
		{
		case EDeleteTftStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_CANCELASYNCREQUEST_1, "Cancel PacketQoS::EPacketContextDeleteTFTCancel");
			aContext.PacketQoS().CancelAsyncRequest(EPacketContextDeleteTFTCancel);
			break;
			}
		
		case ERemoveTftStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_CANCELASYNCREQUEST_2, "Cancel PacketQoS::EPacketContextRemovePacketFilterCancel");
			aContext.PacketQoS().CancelAsyncRequest(EPacketContextRemovePacketFilterCancel);
			break;
			}
			
		case EAddTftStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_CANCELASYNCREQUEST_3, "Cancel PacketQoS::EPacketContextAddPacketFilter");
			aContext.PacketQoS().CancelAsyncRequest(EPacketContextAddPacketFilterCancel);
			break;
			}

		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCHANGETFTSTRATEGY_CANCELASYNCREQUEST_4, "ERROR: Incorrect case in TChangeTftStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	}


//
// TActivatePdpStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TActivatePdpStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NEXT_1, "TActivatePdpStrategy::Next()");
	
	TInt err = KErrNone;
	
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NEXT_3, "RPacketContext::Activate()");
			aContext.PacketContext().Activate(*aStatus);
			aContext.SetStrategyStep (EActivateStep);
			break;
			}
		
		case EActivateStep:
		case ENotifyStatusChange:
			{
            OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NEXT_4, "RPacketContext::GetStatus()");

            if (aContext.PacketContext().GetStatus(aContext.ContextStatus()) != KErrNone)
                {
                aContext.ContextStatus() = RPacketContext::EStatusInactive;
                }
            
            switch (aContext.ContextStatus())
            	{
            	case RPacketContext::EStatusActive:
                	// Context is now active
    	            aContext.PdpFsmInterface().Get (aContext.Id(), aContext.ContextPacketDataConfigBase());
    	            OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NEXT_5, "RPacketContext::GetConfig()");
    				aContext.PacketContext().GetConfig (*aStatus, aContext.ContextConfig());
    				aContext.SetStrategyStep (EGetConfigStep);
    				break;

            	case RPacketContext::EStatusInactive:
                    {
                    // Context is inactive so find out why
                    TInt errTsy = KErrNone;
                    TInt rc = aContext.PacketContext().GetLastErrorCause(errTsy);
                    if (rc == KErrNone)
                        {
                        if (errTsy == KErrNone)
                        	{
                        	err = KErrCompletion;
                        	}
                        else if ((errTsy & 0xFFFF0000) != 0xFFFF0000)
                            {
                            // TSY stores the basic error code to lower 16 bits and 
                            // adds the extended error code to higher 16 bits.
                            // If extended error code is present, it is prefered.
                            err = (errTsy >> 16);
                            err = (err | 0xFFFF0000);
                            }
                        else 
                        	{
                            // Basic error only - returned by TSYs not supporting extended errors
                            err = errTsy;
                        	}
                        }
                    else
                        {
                        err = KErrCompletion;   
                        }
                        
                    aContext.SetStrategyStep (EFinishStep);
                    break;
                    }
            	
            	default:
            		// Not active, not inactive. Re-request the status and try again
                    OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NEXT_6, "RPacketContext::NotifyStatusChange()");
                    aContext.PacketContext().NotifyStatusChange(*aStatus, aContext.ContextStatus());
                    aContext.SetStrategyStep (ENotifyStatusChange);
                    break;
                }
			break;
			}
			
		case EGetConfigStep:
			{
			aContext.PdpFsmInterface().Set (aContext.Id(), aContext.ContextPacketDataConfigBase());
			if (KPrimaryContextId == aContext.Id())
				{
                aContext.PacketQoS().GetProfileParameters (*aStatus, aContext.QosNegotiatedPckg());	
#if (OST_TRACE_CATEGORY & OST_TRACE_CATEGORY_DEBUG)
                aContext.DumpNegProfileParameters ();
#endif
                aContext.SetStrategyStep (EGetProfileParamsStep);
                }
			else
                {
        		// start notifications in case of normal creation of a context
        		// -- 	StatusChangeNotifier
        		// --	QoSChangeNotifier
        		// --	ConfigChangeNotifier
        		aContext.StartNotifications();
        		aContext.SetStrategyStep (EFinishStep);
                }
            break;
			}
			
		case EGetProfileParamsStep:
			{
			// start notifications in case of normal creation of a context
			// -- 	StatusChangeNotifier
			// --	QoSChangeNotifier
			// --	ConfigChangeNotifier
			aContext.StartNotifications();
			aContext.SetStrategyStep (EFinishStep);
			break;
			}
			
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NEXT_2, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}
	

/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TActivatePdpStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
	{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NOTIFYFSM_1, "FSM set: DataChannelV2");
		aContext.PdpFsmInterface().Set(aContext.Id(), aContext.DataChannelV2());

#ifdef SYMBIAN_NETWORKING_UMTSR5
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NOTIFYFSM_2, "FSM set: QoSR5Negotiated");
		aContext.PdpFsmInterface().Set(aContext.Id(), aContext.QosNegotiated().NegotiatedQoSR5());

#else
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NOTIFYFSM_3, "FSM set: QoSR99_R4Negotiated");
		aContext.PdpFsmInterface().Set(aContext.Id(), aContext.QosNegotiated().NegotiatedQoSR99_R4());
#endif 
// SYMBIAN_NETWORKING_UMTSR5

		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NOTIFYFSM_4, "Notifying FSM: EPdpActivated");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPdpActivated);

		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_NOTIFYFSM_5, "Notifying FSM: EPdpActivatedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPdpActivatedFailed, aCompletionStatus.Int());
		}
	}
	
/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TActivatePdpStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	switch(aContext.StrategyStep())
		{
		case EActivateStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_CANCELASYNCREQUEST_1, "Cancel PacketContext::EPacketContextActivate");
			aContext.PacketContext().CancelAsyncRequest(EPacketContextActivate);
			break;
			}
			
		case EGetConfigStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_CANCELASYNCREQUEST_2, "Cancel PacketContext::EPacketContextGetConfig");
			aContext.PacketContext().CancelAsyncRequest(EPacketContextGetConfig);
			break;
			}
		
		case ENotifyStatusChange:
		    {
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_CANCELASYNCREQUEST_3, "Cancel PacketContext::EPacketContextNotifyStatusChange");
			aContext.PacketContext().CancelAsyncRequest(EPacketContextNotifyStatusChange);	
			break;
			}
			
		case EGetProfileParamsStep:
			{
			if(KPrimaryContextId == aContext.Id())
				{
				OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_CANCELASYNCREQUEST_4, "Cancel PacketContext::EPacketQoSGetProfileParams");
				aContext.PacketContext().CancelAsyncRequest(EPacketQoSGetProfileParams);	
				break;
				}
			}
			
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEPDPSTRATEGY_CANCELASYNCREQUEST_5, "ERROR: Incorrect case in TActivatePdpStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	}

//
// TGetNegQoSStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TGetNegQoSStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TGETNEGQOSSTRATEGY_NEXT_1, "TGetNegQoSStrategy::Next()");
	
	TInt err = KErrNone;
	
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			aContext.PacketQoS().GetProfileParameters (*aStatus, aContext.QosNegotiatedPckg());
#if (OST_TRACE_CATEGORY & OST_TRACE_CATEGORY_DEBUG)
            aContext.DumpNegProfileParameters ();
#endif
			aContext.SetStrategyStep (EGetProfileParamsStep);
			break;
			}

		case EGetProfileParamsStep:
			{
			aContext.SetStrategyStep (EFinishStep);
			break;
			}
			
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TGETNEGQOSSTRATEGY_NEXT_2, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}
	

/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TGetNegQoSStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
	{
	if(aCompletionStatus == KErrNone)
		{
#ifdef SYMBIAN_NETWORKING_UMTSR5
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TGETNEGQOSSTRATEGY_NOTIFYFSM_1, "FSM set: QoSR5Negotiated");
		aContext.PdpFsmInterface().Set(aContext.Id(), aContext.QosNegotiated().NegotiatedQoSR5());

#else
// !SYMBIAN_NETWORKING_UMTSR5

		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TGETNEGQOSSTRATEGY_NOTIFYFSM_2, "FSM set: QoSR99_R4Negotiated");
		aContext.PdpFsmInterface().Set(aContext.Id(), aContext.QosNegotiated().NegotiatedQoSR99_R4());
#endif
// SYMBIAN_NETWORKING_UMTSR5
		
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TGETNEGQOSSTRATEGY_NOTIFYFSM_3, "Notifying FSM: EPdpNegQoSRetrieved");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPdpNegQoSRetrieved);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TGETNEGQOSSTRATEGY_NOTIFYFSM_4, "Notifying FSM: EPdpNegQoSRetrievedFailed");
	 	aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPdpNegQoSRetrievedFailed, aCompletionStatus.Int());
		}
	}
	
/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TGetNegQoSStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	switch(aContext.StrategyStep())
		{	
		case EGetProfileParamsStep:
			{
			aContext.PacketContext().CancelAsyncRequest(EPacketQoSGetProfileParams);	
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TGETNEGQOSSTRATEGY_CANCELASYNCREQUEST_1, "Cancel PacketContext::EPacketQoSGetProfileParams");
			break;
			}
			
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TGETNEGQOSSTRATEGY_CANCELASYNCREQUEST_2, "ERROR: Incorrect case in TGetNegQoSStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	}


//
// TModifyActiveStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TModifyActiveStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_NEXT_1, "TModifyActiveStrategy::Next()");
	
	TInt err = KErrNone;
	
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			if (aContext.ContextType() != SpudMan::EMbms)
				{
				aContext.PacketContext().ModifyActiveContext(*aStatus);
				aContext.SetStrategyStep (EModifyActiveStep);
				}
			else
				{
				aContext.MbmsPacketContext().ModifyActiveContext(*aStatus);
				aContext.SetStrategyStep (EGetProfileParamsStep);
				}	
			//aContext.SetStrategyStep (EModifyActiveStep);
			break;
			}
		
		case EModifyActiveStep:
			{
			aContext.PacketQoS().GetProfileParameters (*aStatus, aContext.QosNegotiatedPckg());
#if (OST_TRACE_CATEGORY & OST_TRACE_CATEGORY_DEBUG)
            aContext.DumpNegProfileParameters ();
#endif
			aContext.SetStrategyStep (EGetProfileParamsStep);
			break;
			}
			
		case EGetProfileParamsStep:
			{
			aContext.SetStrategyStep (EFinishStep);
			break;
			}
			
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_NEXT_2, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}
	

/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TModifyActiveStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
	{
	if(aCompletionStatus == KErrNone)
		{
		if(aContext.ContextType() != SpudMan::EMbms)
			{
#ifdef SYMBIAN_NETWORKING_UMTSR5
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_NOTIFYFSM_1, "FSM set: QoSR5Negotiated");
			aContext.PdpFsmInterface().Set(aContext.Id(), aContext.QosNegotiated().NegotiatedQoSR5());

#else
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_NOTIFYFSM_2, "FSM set: QoSR99_R4Negotiated");
 			aContext.PdpFsmInterface().Set(aContext.Id(), aContext.QosNegotiated().NegotiatedQoSR99_R4());
#endif 
// SYMBIAN_NETWORKING_UMTSR5
			}
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_NOTIFYFSM_3, "Notifying FSM: EPdpContextModified");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPdpContextModified);

		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_NOTIFYFSM_4, "Notifying FSM: EPdpContextModifiedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPdpContextModifiedFailed, aCompletionStatus.Int());
		}
	}
	
/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TModifyActiveStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	switch(aContext.StrategyStep())
		{
		case EModifyActiveStep:
			{
			aContext.PacketContext().CancelAsyncRequest(EPacketContextModifyActiveContext);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_CANCELASYNCREQUEST_1, "Cancel PacketContext::EPacketContextModifyActiveContext");
			break;
			}
			
		case EGetProfileParamsStep:
			{
			aContext.PacketContext().CancelAsyncRequest(EPacketQoSGetProfileParams);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_CANCELASYNCREQUEST_2, "Cancel PacketContext::EPacketQoSGetProfileParams");
			break;
			}
			
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMODIFYACTIVESTRATEGY_CANCELASYNCREQUEST_3, "ERROR: Incorrect case in TModifyActiveStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	}








// 
//	TCreateMbmsPdpContextStrategy 
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TCreateMbmsPdpContextStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATEMBMSPDPCONTEXTSTRATEGY_NEXT_1, "TCreateMbmsPdpContextStrategy::Next()");
	
	TInt err = KErrNone;
	
	// possible steps:
	// 	-- open new context
	// -- Set session information
	// 	-- initialise context
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			err = aContext.MbmsPacketContext().OpenNewContext (aContext.PacketService(), aContext.Name());
			if (err)
				{
				OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATEMBMSPDPCONTEXTSTRATEGY_NEXT_2, "PacketContextOpenNewContext returned for MBMS %d", err);
				break;
				}
				
			aContext.MbmsPacketContext().InitialiseContext(*aStatus, aContext.DataChannelV2Pckg());						
			aContext.SetStrategyStep (EInitialiseContextStep);
			break;
			}
			
		case EInitialiseContextStep:
			{
			// get mbms packet config from fsm
			aContext.PdpFsmInterface().Get (aContext.Id(), aContext.ContextConfigMbms());
			// async set config
			aContext.MbmsPacketContext().SetConfig (*aStatus, aContext.ContextConfigMbmsPckg());
			// set next step
			aContext.SetStrategyStep (ESetConfigStep);
			break;
			}
		case ESetConfigStep:
			aContext.SetStrategyStep (EFinishStep);
			break;	
	
		default:
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATEMBMSPDPCONTEXTSTRATEGY_NEXT_3, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}


/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TCreateMbmsPdpContextStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus)
{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATEMBMSPDPCONTEXTSTRATEGY_NOTIFYFSM_1, "Notifying FSM: EMbmsPdpContextCreated");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EMbmsPdpContextCreated);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATEMBMSPDPCONTEXTSTRATEGY_NOTIFYFSM_2, "Notifying FSM: E1ryPdpContextCreatedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EMbmsPdpContextCreatedFailed, aCompletionStatus.Int());
		}
}

/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TCreateMbmsPdpContextStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATEMBMSPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_1, "TCreate1ryPdpContextStrategy::CancelAsyncRequest()");
	
	switch(aContext.StrategyStep())
		{
		case ESetConfigStep:
			{
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATEMBMSPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_2, "Cancel PacketContext::EPacketContextSetConfig");
			aContext.PacketContext().CancelAsyncRequest(EPacketContextSetConfig);
			break;
			}
			
		default:
			// there're NO outstanding async requests
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TCREATEMBMSPDPCONTEXTSTRATEGY_CANCELASYNCREQUEST_3, "ERROR: Incorrect case");
			ASSERT(EFalse);
			break;
		}
	}


//
// TActivatePdpStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/
void TActivateMbmsPdpStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_NEXT_1, "TActivatePdpStrategy::Next()");
	
	TInt err = KErrNone;
	
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			aContext.MbmsPacketContext().Activate(*aStatus);
			aContext.SetStrategyStep (EActivateStep);
			break;
			}
		
		case EActivateStep:
			{
			aContext.PdpFsmInterface().Get (aContext.Id(), aContext.ContextConfigMbms());
			aContext.MbmsPacketContext().GetConfig (*aStatus, aContext.ContextConfigMbmsPckg());
			aContext.SetStrategyStep (EGetConfigStep);
			break;
			}
			
		case EGetConfigStep:
			{
			aContext.PdpFsmInterface().Set (aContext.Id(), aContext.ContextConfigMbms());
			aContext.MbmsPacketContext().InitialiseContext(*aStatus, aContext.DataChannelV2Pckg());
			aContext.SetStrategyStep (EGetProfileParamsStep);
			break;
			}

				
		case EGetProfileParamsStep:
			{
			// start notifications in case of normal creation of a context
			// -- 	StatusChangeNotifier
			// --	QoSChangeNotifier
			// --	ConfigChangeNotifier
			aContext.StartNotifications();
			aContext.SetStrategyStep (EFinishStep);
			break;
			}
			
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_NEXT_2, "ERROR: Incorrect case");
			ASSERT(EFalse);
			err = KErrNotSupported;
			break;
		}
		
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
	
	}
	

/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/	
void TActivateMbmsPdpStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
	{
	if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_NOTIFYFSM_1, "FSM set: DataChannelV2");
		aContext.PdpFsmInterface().Set(aContext.Id(), aContext.DataChannelV2());


		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_NOTIFYFSM_2, "Notifying FSM: EPdpActivated");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPdpActivated);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_NOTIFYFSM_3, "Notifying FSM: EPdpActivatedFailed");
		aContext.PdpFsmInterface().Input(aContext.Id(), PdpFsm::EPdpActivatedFailed, aCompletionStatus.Int());
		}
	}
	
/** Cancels last asynchronous request to eTel

@param  aContext - pdp context
*/
void TActivateMbmsPdpStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	switch(aContext.StrategyStep())
		{
		case EActivateStep:
			{
			aContext.MbmsPacketContext().CancelAsyncRequest(EPacketContextActivate);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_CANCELASYNCREQUEST_1, "Cancel MbmsPacketContext::EPacketContextActivate");
			break;
			}
		
		case EGetConfigStep:
			{
			aContext.MbmsPacketContext().CancelAsyncRequest(EPacketContextGetConfig);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_CANCELASYNCREQUEST_2, "Cancel MbmsPacketContext::EGetConfigStep");
			break;
			}
		
		case EInitialiseContextStep:
			{
			aContext.MbmsPacketContext().CancelAsyncRequest(EPacketContextInitialiseContext);	
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_CANCELASYNCREQUEST_3, "Cancel MbmsPacketContext::EPacketContextInitialiseContext");
			break;
			}
			
					
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TACTIVATEMBMSPDPSTRATEGY_CANCELASYNCREQUEST_4, "ERROR: Incorrect case in TActivatePdpStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	}
	
//
// TMbmsSessionUpdatePdpStrategy
//
/** Executes next asynchronous step in a strategy. 

@param aContext - pdp context
@param aStatus - request status of this step
*/	
void TMbmsSessionUpdateStrategy::Next(CEtelDriverContext& aContext, TRequestStatus* aStatus)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMBMSSESSIONUPDATESTRATEGY_NEXT_1, "TMbmsSessionUpdateStrategy::Next()");
	
	TInt err = KErrNone;
	switch(aContext.StrategyStep())
		{
		case EStartStep:
			{
			TInt sessionId=KErrNotFound;
			aContext.PdpFsmInterface().Get (aContext.Id(), aContext.GetSessionInfo());
			TSessionOperatioInfo sessionInfo = aContext.GetSessionInfo();
			TInt count=sessionInfo.iSessionIds.Count();
			TInt curSessionIndex = aContext.GetSessionIndex();
			if (curSessionIndex < count )
				{
				sessionId = sessionInfo.iSessionIds[curSessionIndex];
				aContext.SetSessionIndex(++curSessionIndex);
				}
			if (curSessionIndex>=count)
				{
				aContext.SetStrategyStep (EUpdateMbmsSessionList);//move to next strategy otherwise service next entry in session list
				aContext.SetSessionIndex(0);	//reset index
				}
			if ((sessionInfo.iOperation == CSubConMBMSExtensionParamSet::EAddSession) 
				  && (sessionId != KErrNotFound))
				{
				aContext.MbmsPacketContext().UpdateMbmsSessionList(*aStatus,EAddEntries,sessionId);
				break;
				}
			else if ((sessionInfo.iOperation == CSubConMBMSExtensionParamSet::ERemoveSession) 
					&& (sessionId != KErrNotFound))
				{
				aContext.MbmsPacketContext().UpdateMbmsSessionList(*aStatus,ERemoveEntries,sessionId);
				break;
				}
			else if ((sessionInfo.iOperation == CSubConMBMSExtensionParamSet::ERemoveAll) 
					&& (sessionId != KErrNotFound))
				{
				aContext.MbmsPacketContext().UpdateMbmsSessionList(*aStatus,ERemoveAllEntries,sessionId);
				aContext.SetStrategyStep(EUpdateMbmsSessionList);	
				break;
				}
			//proper session operation is not supplied or the list is empty..
			// ..so return parameter rejected  event
			else 
				{
				aContext.SetStrategyStep (EFinishStep);	
				err=KErrNotSupported;
				break;
				}
					
			} 

		case EUpdateMbmsSessionList:
			{
			TRAP_IGNORE(aContext.SetMbmsSession(RPacketMbmsContext::CMbmsSession::NewL()));
			TRAP_IGNORE(aContext.SetRetrievePcktMbmsSessionList(CRetrievePcktMbmsSessionList::NewL(aContext.MbmsPacketContext(),*aContext.MbmsSession())));
			aContext.RetrievePcktMbmsSessionList()->Start(*aStatus);
			aContext.SetStrategyStep(EPrepareSessionList);	
			break;
			}
			
		case EPrepareSessionList:
			{
			RPacketMbmsContext::CMbmsSession *mbmsSession =	aContext.MbmsSession();
			if (mbmsSession)
				{
				TSessionOperatioInfo sessionInfo;
				sessionInfo.iSessionIds = mbmsSession->iSessionIdList;
				sessionInfo.iOperation = aContext.GetSessionInfo().iOperation;
				aContext.PdpFsmInterface().Set (aContext.Id(), sessionInfo);
				CRetrievePcktMbmsSessionList *retrievePktMbmsSessionList=aContext.RetrievePcktMbmsSessionList();
				delete retrievePktMbmsSessionList;
				delete mbmsSession; //no need to close sessionInfo.iSessionIds..it will be delted by this delete
				aContext.SetMbmsSession(NULL);
				aContext.SetRetrievePcktMbmsSessionList(NULL);
				}
			aContext.SetStrategyStep(EFinishStep);	
			break;
			}
		
		default: 
			CRetrievePcktMbmsSessionList *retrievePktMbmsSessionList=aContext.RetrievePcktMbmsSessionList();
			if (retrievePktMbmsSessionList)
				delete retrievePktMbmsSessionList;
			RPacketMbmsContext::CMbmsSession *mbmsSession =	aContext.MbmsSession();	
			if (mbmsSession)
				delete mbmsSession;
			aContext.SetMbmsSession(NULL);
			aContext.SetRetrievePcktMbmsSessionList(NULL);
			break;
		}
	if(err || (EFinishStep == aContext.StrategyStep()))
		{
		User::RequestComplete( aStatus, err);
		}
		
	}

/** 
Notifies SPUD FSM about execution results.

@param aContext - pdp context
@param aCompletionStatus - completion request status
*/		
void TMbmsSessionUpdateStrategy::NotifyFsm(CEtelDriverContext& aContext, TRequestStatus& aCompletionStatus )
	{
		if(aCompletionStatus == KErrNone)
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMBMSSESSIONUPDATESTRATEGY_NOTIFYFSM_1, "Notifying FSM: TMbmsSessionUpdateStrategy");
		aContext.PdpFsmInterface().Input(aContext.Id(), SpudMan::EMbmsParameterUpdate);
		}
	else
		{
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMBMSSESSIONUPDATESTRATEGY_NOTIFYFSM_2, "Notifying FSM: TMbmsSessionUpdateStrategy");
		aContext.PdpFsmInterface().Input(aContext.Id(), SpudMan::EMbmsParameterUpdate, aCompletionStatus.Int());
		}
	}
/** Cancels last asynchronous request to eTel

@param  aContext - mbms context
*/
void TMbmsSessionUpdateStrategy::CancelAsyncRequest(CEtelDriverContext& aContext)
	{
	
	switch(aContext.StrategyStep())
		{
		case EUpdateMbmsSessionList:
			{
			aContext.MbmsPacketContext().CancelAsyncRequest(EPacketContextSetConfig);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMBMSSESSIONUPDATESTRATEGY_CANCELASYNCREQUEST_1, "Cancel MbmsPacketContext::EUpdateMbmsSessionList");
			break;
			}
		
		case EPrepareSessionList:
			{
			aContext.MbmsPacketContext().CancelAsyncRequest(EPacketContextSetConfig);
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMBMSSESSIONUPDATESTRATEGY_CANCELASYNCREQUEST_2, "Cancel MbmsPacketContext::EPrepareSessionList");
			break;
			}
								
		default:
			// unexpected
			OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TMBMSSESSIONUPDATESTRATEGY_CANCELASYNCREQUEST_3, "ERROR: Incorrect case in TMbmsSessionUpdateStrategy::CancelAsyncRequest");
			ASSERT(EFalse);
			break;
		}
	
	}
