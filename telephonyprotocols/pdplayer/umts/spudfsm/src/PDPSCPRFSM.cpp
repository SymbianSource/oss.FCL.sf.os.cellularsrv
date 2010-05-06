// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implementation file for the PDP Context Finite State Machine
// 
//

/**
 @file 
*/


#include "PDPFSM.h"
#include "cpdpfsmfactory.h"
#include "spudfsmdebuglogger.h"
#include "PDPDeftSCPR.h"


//-=========================================================
// Custom methods
//-=========================================================
CPdpFsmInterface::CPdpFsmInterface(CPDPDefaultSubConnectionProvider& aOwner)
:iPdpFsmFactory(NULL), 
 iDefaultSubConnProvd(aOwner),
 iNetworkStatus(RPacketService::EStatusUnattached),
 iRefCount(1),
 iUmtsRelease(TPacketDataConfigBase::KConfigGPRS)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::()");
    }

CPdpFsmInterface::~CPdpFsmInterface()
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::()");
	delete iPdpFsmFactory;
    }

TContextId CPdpFsmInterface::NewFsmContextL(MPdpFsmEventHandler& aPdpFsmEventHandler,SpudMan::TPdpContextType aContextType)
    {
    return iPdpFsmFactory->NewFsmContextL(aPdpFsmEventHandler,aContextType);
    }

    
TBool CPdpFsmInterface::FsmContextExists(const TContextId aPdpId)
    {
    return iPdpFsmFactory->HaveFsmContext(aPdpId);
    }
    
void CPdpFsmInterface::DeleteFsmContext(const TContextId aPdpId)
    {
    iPdpFsmFactory->DeleteFsmContext(aPdpId);
    }    


/** request to open the FSM

@param aSpudManInterface reference to SpudMan interface
*/
void CPdpFsmInterface::NewL(const TName& aTsyName, TInt aUmtsRelease)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::NewL()");
	
	iUmtsRelease = aUmtsRelease;
	iPdpFsmFactory = CPdpFsmFactory::NewL();
	iPdpFsmFactory->InitL(aTsyName, this);
    }

/** request to open the FSM

@param aSpudManInterface reference to SpudMan interface
*/
void CPdpFsmInterface::Open()
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Open()");
	iRefCount++;
    }
    
/** closes the FSM and frees underlying resources
*/
void CPdpFsmInterface::Close()
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Close()");
    ASSERT( iRefCount > 0 );
	if (--iRefCount == 0 )
	    {
	    if (iPdpFsmFactory)
    	    {
    	    iPdpFsmFactory->Close();
    	    }
    	delete this;
	    }
    }


/** 
 Performs and input action/notification for context ID aPdpId
 If aPdpId is KAllContexts, the notification is sent to every context, unless it is of type EServiceStatusChangeNetwork

@param aPdpId the PDP context ID, 0 to KMaxPdpContexts
@param aOperation the operation id to perform
@param aParam extra id for ETelDriver errors
@return error code for the synchronus patrt of the operation
*/
TInt CPdpFsmInterface::Input(TContextId aPdpId, const TInt aOperation, const TInt aParam)
    {
	TInt ret = KErrNone;

	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Input(aParam)");

	ASSERT(iPdpFsmFactory);


    // Control structure is in order of the most frequent operation first (hopefully)
    //
	if (iPdpFsmFactory->ContextIsValid(aPdpId))
	    {
	    // We have a valid PDP FSM Context, so go ahead with the operation...
	    //
	    ret = (iPdpFsmFactory->GetFsmContext(aPdpId))->Input(aOperation, aParam);

        // Recovering memory by deleting the Fsm Context
        //
        // Tricky, definitely don't try with 'SpudMan::EContextDelete' it's far too early.
        // You can get a bit further deleting on 'PdpFsm::EContextDeleted' (and 'PdpFsm::EContextDeletedFailed')
        // from TContextDeleteStrategy::NotifyFsm(), but you then find that GuQoS is calling into Spud in order to
        // transfer data from the deleted context to another...
        //
        // Additionally, the spud unit test is expecting to be able to "reuse" deleted contexts...
	    }
	else if (aPdpId == KAllContexts)
	    {
		TInt err = KErrNone;
	
		// this has to be here to avoid sending it from every context
		//
		if (aOperation == PdpFsm::EServiceStatusChangeNetwork)
		    {	
			iDefaultSubConnProvd.PdpFsmAllContextEvent(KNetworkStatusEvent, KErrNone);
		    }
		else
		    {
			for (TContextId i = 0; (i < KMaxPdpContexts) && iPdpFsmFactory->HaveFsmContext(i); i++)
			    {
		        // Process any current PDP contexts.
		        
		        err = (iPdpFsmFactory->GetFsmContext(i))->Input(aOperation, aParam);

                // See above about trials and tribulations of trying to recover the memory taken by these 
                // CPdpFsm objects.

				if (err != KErrNone)
				    {
				    // We return the last error found, ignoring any earlier ones
				    //
					ret = err;
				    }
			    }
		    }		
        }
	else
	    {
		ret = KErrBadHandle;
	    }
	
	return ret;
    }



#ifdef SYMBIAN_NETWORKING_UMTSR5

/**
Set context parameters
@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts - 1
@param aParam - RPacketQoS::TQoSR5Requested data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const RPacketQoS::TQoSR5Requested& aParam)
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketQoS::TQoSR5Requested)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
	}


/**
Set context parameters
@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts - 1
@param aParam - RPacketQoS::TQoSR5Negotiated data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const RPacketQoS::TQoSR5Negotiated& aParam)
{
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketQoS::TQoSR5Negotiated)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
}


#else
// !SYMBIAN_NETWORKING_UMTSR5

/**
Set context parameters
@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketQoS::TQoSR99_R4Requested data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const RPacketQoS::TQoSR99_R4Requested& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketQoS::TQoSR99_R4Requested)");

	ASSERT(iPdpFsmFactory);
    ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
    iPdpFsmFactory->GetFsmContext(aPdpId,aContextType)->Set(aParam);
	return KErrNone;
    }


/**
Set context parameters
@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketQoS::TQoSR99_R4Negotiated data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const RPacketQoS::TQoSR99_R4Negotiated& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketQoS::TQoSR99_R4Negotiated)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId,aContextType)->Set(aParam);
	return KErrNone;
    }

#endif 
// SYMBIAN_NETWORKING_UMTSR5 


void CPdpFsmInterface::Set(const TContextId aPdpId,MPdpFsmEventHandler& aPdpFsmEventHandler)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketQoS::TQoSR99_R4Negotiated)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aPdpFsmEventHandler);
    }

/** Set context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - TFTInfo data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const TTFTInfo& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(TFTInfo)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
    }

/** Set context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - TFTOperationCode to set to go with the data
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const TTFTOperationCode& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(TTFTOperationCode)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
    }

/** Set context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketContext::TDataChannelV2 data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const RPacketContext::TDataChannelV2& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketContext::TDataChannelV2)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
    }

/** Set context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketContext::TContextStatus data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const RPacketContext::TContextStatus& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketContext::TContextStatus)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
    }

/** Set context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketContext::TContextConfigGPRS& data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId, const TPacketDataConfigBase& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(TPacketDataConfigBase)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
    }

/** Set network status

@param aParam - RPacketService::TStatus data to set
*/
void CPdpFsmInterface::Set(const RPacketService::TStatus aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketService::TStatus)");

	ASSERT(iPdpFsmFactory);

	iNetworkStatus = aParam;
    }


#ifdef SYMBIAN_NETWORKING_UMTSR5
/**
Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketQoS::TQoSR5Requested data to get
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,RPacketQoS::TQoSR5Requested& aParam) const
{
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(RPacketQos::TQoSR5Requested)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
}


/**
Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketQoS::TQoSR5Negotiated data to get
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,RPacketQoS::TQoSR5Negotiated& aParam) const
{
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(RPacketQoS::TQoSR5Negotiated)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
}

#else
// !SYMBIAN_NETWORKING_UMTSR5

/**
Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketQoS::TQoSR99_R4Requested data to get
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,RPacketQoS::TQoSR99_R4Requested& aParam) const
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(RPacketQos::TQoSRequestedR99_R4)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
    }


/**
Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketQoS::TQoSR99_R4Negotiated data to get
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,RPacketQoS::TQoSR99_R4Negotiated& aParam) const
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(RPacketQoS::TQoSR99_R4Negotiated)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
    }

#endif 
// SYMBIAN_NETWORKING_UMTSR5 



/** Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - TTFTInfo data to get
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,TTFTInfo& aParam) const
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(TFTInfo)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
    }

/** Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - TTFTOperationCode for the TFT data
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,TTFTOperationCode& aParam) const
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(TFTInfo)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
    }



/** Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketContext::TDataChannelV2 data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,RPacketContext::TDataChannelV2& aParam) const
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(RPacketContext::TDataChannelV2)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
    }

/** Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketContext::TContextConfigGPRS data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId, TPacketDataConfigBase& aParam) const
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(TPacketDataConfigBase)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
    }

/** Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketContext::TContextStatus data to get
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,RPacketContext::TContextStatus& aParam) const
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(RPacketContext::TContextStatus)");

	ASSERT(iPdpFsmFactory);
	ASSERT(iPdpFsmFactory->ContextIdIsValid(aPdpId));
	ASSERT(iPdpFsmFactory->HaveFsmContext(aPdpId));
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
    }

/** Get context parameters

@param aPdpId - the PDP context ID, 0 to KMaxPdpContexts
@param aParam - RPacketContext::TContextConfigGPRS data to set
@return - KErrBadHandle or KErrNone
*/
void CPdpFsmInterface::Get(RPacketService::TStatus& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Get(RPacketService::TStatus)");

	ASSERT(iPdpFsmFactory);

	aParam = iNetworkStatus;
    }


/** Get the TsyName

@return - TsyName in a TName
*/
const TName& CPdpFsmInterface::TsyName(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::TsyName");

	ASSERT(iPdpFsmFactory);
	
	return iPdpFsmFactory->TsyName();
}





/**
Set Mbms context parameters
@param aPdpId - the Mbms PDP context ID
@param aParam - RPacketMbmsContext::TContextConfigMbmsV1 data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId,const RPacketMbmsContext::TContextConfigMbmsV1& aParam)
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketMbmsContext::TContextConfigMbmsV1)");

	ASSERT(iPdpFsmFactory);
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
	}

/**
Get Mbms context parameters
@param aPdpId - the Mbms PDP context ID
@param aParam - RPacketMbmsContext::TContextConfigMbmsV1 data to get
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId,RPacketMbmsContext::TContextConfigMbmsV1& aParam) const 
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketMbmsContext::TContextConfigMbmsV1)");

	ASSERT(iPdpFsmFactory);
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
	}
	
/**
Get Mbms Session  parameters
@param aPdpId - the Mbms PDP context ID
@param aParam - TSessionOperatioInfo data to set
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Set(TContextId aPdpId, const TSessionOperatioInfo& aParam)
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketMbmsContext::TContextConfigMbmsV1)");

	ASSERT(iPdpFsmFactory);
	iPdpFsmFactory->GetFsmContext(aPdpId)->Set(aParam);
	return KErrNone;
	}


/**
Get Mbms Session parameters
@param aPdpId - the Mbms PDP context ID
@param aParam - TSessionOperatioInfo data to get
@return - KErrBadHandle or KErrNone
*/
TInt CPdpFsmInterface::Get(TContextId aPdpId, TSessionOperatioInfo& aParam) const 
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsmInterface::Set(RPacketMbmsContext::TContextConfigMbmsV1)");

	ASSERT(iPdpFsmFactory);
	iPdpFsmFactory->GetFsmContext(aPdpId)->Get(aParam);
	return KErrNone;
	}

