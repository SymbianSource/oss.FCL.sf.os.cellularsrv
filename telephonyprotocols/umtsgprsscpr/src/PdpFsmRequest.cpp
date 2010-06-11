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
// The implementation file for the PDP Context control requests.
// 
//

/**
 @file
 @internalComponent
*/


#include "PdpFsmRequest.h"


//-=========================================================
//
//CModifyActivePDPCtx
//
//-=========================================================
CModifyActivePDPCtx::CModifyActivePDPCtx(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner)
:CPdpFsmRequestBase(aProvider, aOwner)
{	
}

CModifyActivePDPCtx::~CModifyActivePDPCtx()
{
}


TInt CModifyActivePDPCtx::Start()
{
	// Store the Context Id in order to check replies later
	iContextId = iProvider.GetContextId();
	return iProvider.GetPdpFsmInterface().Input(iContextId, SpudMan::EContextModifyActive);
}

/**
@param aEvent The event Id
@param aParam Data relavent to the event. Usually a system-wide error code
@return ETrue if the event was consumed, EFalse if it was of no interest and should
be passed on to SpudMan for processing.
*/
TBool CModifyActivePDPCtx::PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam)
{
	if (aContextId == iContextId && aEvent == KContextModifyActiveEvent)
		{
		iOwner.Done(aParam, *this);
		return ETrue;
		}
	
	return EFalse;
}



#ifdef SYMBIAN_NETWORKING_UMTSR5
//-=========================================================
//
//CModifyR5QoS
//
//-=========================================================
CModifyR5QoS::CModifyR5QoS(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner, RPacketQoS::TQoSR5Requested& aParams)
:CPdpFsmRequestBase(aProvider, aOwner),
 iModifyActive(NULL),
 i5(aParams)
{
}

CModifyR5QoS::~CModifyR5QoS()
{
    ASSERT(iModifyActive == NULL || iContextId == 0); 
    //If we assert here is is becuase Start has been called (allocated iContextId), but Done has not been called (cleans iModifyActive)
	delete iModifyActive;
}

void CModifyR5QoS::GrantedSetupL()
	{
	CSubConGenEventParamsGranted* event = CSubConGenEventParamsGranted::NewL();
	CleanupStack::PushL (TCleanupItem(SubconEventCleanupFunction, event));
	event->SetFamily (KSubConQoSFamily);
	RPacketQoS::TQoSR5Negotiated negotiatedQoS;
	iProvider.GetPdpFsmInterface().Get(iProvider.GetContextId(), negotiatedQoS);
	iProvider.MapQosEtelToGrantedParamsL(&negotiatedQoS, *event);
	iProvider.NotifyClientEvent (*event);
	CleanupStack::Pop();
	delete event;
	
	}

void CModifyR5QoS::RejectedSetupL(TInt aResult)
	{
	CSubConGenEventParamsRejected* event = CSubConGenEventParamsRejected::NewL();
	CleanupStack::PushL (TCleanupItem(SubconEventCleanupFunction, event));
	event->SetFamilyId (KSubConQoSFamily);
	event->SetError (aResult);
	iProvider.NotifyClientEvent (*event);
	CleanupStack::Pop();
	delete event;
	}

void CModifyR5QoS::Done( TInt aResult, CPdpFsmRequestBase&  aThis )
{
   __ASSERT_ALWAYS(iModifyActive == &aThis, User::Invariant());

	if ( aResult == KErrNone )
   		{
		// QoS Modification complete with success.
	  	__FLOG_1(_L("CModifyR5QoS [this=%08x]:\tDone(): QoS Params Granted"), this);
	  	TRAP_IGNORE(GrantedSetupL());
   		}
	else
	  	{
	  	//QoS Modification complete with failure.
	  	__FLOG_2(_L("CModifyR5QoS [this=%08x]:\tDone(): QoS Params Rejected [aResult=%d]"), this, aResult); 
	  	TRAP_IGNORE(RejectedSetupL(aResult));
	  	}


   delete iModifyActive;
   iModifyActive = NULL;
   iOwner.Done(aResult, *this);
}


TInt CModifyR5QoS::Start()
{
	// Store the Context Id in order to check replies later
	iContextId = iProvider.GetContextId();

	TInt result;
	result = iProvider.GetPdpFsmInterface().Set(iContextId, i5);
	if (result == KErrNone)
		{
		result = iProvider.GetPdpFsmInterface().Input(iContextId, SpudMan::EContextQoSSet);
		}
	  
	return result;
}


TBool CModifyR5QoS::PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam)
{
	if (aContextId != iContextId)
		{
		return EFalse;
		}
		
	TBool handled(EFalse);
	if (aEvent == KContextQoSSetEvent)
		{
		if (aParam == KErrNone)
			{
			iModifyActive->Start();
			}
		else
			{
			//QoS Modification complete with failure.
			__FLOG_2(_L("CModifyR5QoS [this=%08x]:\tPdpFsmEvent(): QoS Params Rejected [aParam=%d]"), this, aParam);
	   
			Done(aParam, *iModifyActive);
			}
		handled = ETrue;
		}
	else
		{
		handled = iModifyActive->PdpFsmEvent(aContextId, aEvent, aParam);
		}

	return handled;
}


void CModifyR5QoS::ConstructL()
{
	iModifyActive = new(ELeave)CModifyActivePDPCtx(iProvider, *this);
}


CModifyR5QoS* CModifyR5QoS::NewL(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner, RPacketQoS::TQoSR5Requested& aParams)
{
	CModifyR5QoS* request = new(ELeave) CModifyR5QoS(aProvider, aOwner, aParams);
	CleanupStack::PushL(request);
	request->ConstructL();
	CleanupStack::Pop(request);
	return request;
}


#else
// !SYMBIAN_NETWORKING_UMTSR5

//-=========================================================
//
//CModifyR4QoS
//
//-=========================================================
CModifyR4QoS::CModifyR4QoS(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner, RPacketQoS::TQoSR99_R4Requested aParams)
:CPdpFsmRequestBase(aProvider, aOwner),
 iModifyActive(NULL),
 i99(aParams)
{
}

CModifyR4QoS::~CModifyR4QoS()
{
    ASSERT(iModifyActive == NULL || iContextId == 0); 
    //If we assert here is is becuase Start has been called (allocated iContextId), but Done has not been called (cleans iModifyActive)
	delete iModifyActive;

}

void CModifyR4QoS::GrantedSetupL()
	{
	CSubConGenEventParamsGranted* event = CSubConGenEventParamsGranted::NewL();
	CleanupStack::PushL (TCleanupItem(SubconEventCleanupFunction, event));
	event->SetFamily (KSubConQoSFamily);
	RPacketQoS::TQoSR99_R4Negotiated negotiatedQoS;
	iProvider.GetPdpFsmInterface().Get(iProvider.GetContextId(), negotiatedQoS);
	iProvider.MapQosEtelToGrantedParamsL(&negotiatedQoS, *event);
	iProvider.NotifyClientEvent (*event);
	CleanupStack::Pop();
	delete event;
	}

void CModifyR4QoS::RejectedSetupL(TInt aResult)
	{
	CSubConGenEventParamsRejected* event = CSubConGenEventParamsRejected::NewL();
	CleanupStack::PushL (TCleanupItem(SubconEventCleanupFunction, event));
	event->SetFamilyId (KSubConQoSFamily);
	event->SetError (aResult);
	iProvider.NotifyClientEvent (*event);
	CleanupStack::Pop();
	delete event;
	}

void CModifyR4QoS::Done( TInt aResult, CPdpFsmRequestBase&  aThis )
{

	__ASSERT_ALWAYS(iModifyActive == &aThis, User::Invariant());
   	if ( aResult == KErrNone )
		{
		// QoS Modification complete with success.
		__FLOG_1(_L("CModifyR4QoS [this=%08x]:\tDone(): QoS Params Granted"), this);
		TRAP_IGNORE(GrantedSetupL());
		}
	else
	{
		//QoS Modification complete with failure.
		__FLOG_2(_L("CModifyR4QoS [this=%08x]:\tDone(): QoS Params Rejected [aResult=%d]"), this, aResult);
		TRAP_IGNORE(RejectedSetupL(aResult));
	}
	delete iModifyActive;
	iModifyActive = NULL;
	iOwner.Done(aResult, *this);
}


TInt CModifyR4QoS::Start()
{
	// Store the Context Id in order to check replies later
	iContextId = iProvider.GetContextId();

	TInt result;
	result = iProvider.GetPdpFsmInterface().Set(iContextId, i99);
	if (result == KErrNone)
		{
		result = iProvider.GetPdpFsmInterface().Input(iContextId, SpudMan::EContextQoSSet);
		}
	  
	return result;
}

TBool CModifyR4QoS::PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam)
{   
	if (aContextId != iContextId)
		{
		return EFalse;
		}

	TBool handled(EFalse);
	if (aEvent == KContextQoSSetEvent)
		{
		if (aParam == KErrNone)
			{
		   	iModifyActive->Start();
			}
		else
			{
			//QoS Modification complete with failure.
			__FLOG_2(_L("CModifyR4QoS [this=%08x]:\tPdpFsmEvent(): QoS Params Rejected [aParam=%d]"), this, aParam);
			Done(aParam, *iModifyActive);
			}
		handled = ETrue;
		}
	else
		{
		handled = iModifyActive->PdpFsmEvent(aContextId, aEvent, aParam);
		}
		
	return handled;
}


void CModifyR4QoS::ConstructL()
{
	iModifyActive = new(ELeave)CModifyActivePDPCtx(iProvider, *this);
}

CModifyR4QoS* CModifyR4QoS::NewL(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner, RPacketQoS::TQoSR99_R4Requested aParams)
{
	CModifyR4QoS* request = new(ELeave) CModifyR4QoS(aProvider, aOwner, aParams);
	CleanupStack::PushL(request);
	request->ConstructL();
	CleanupStack::Pop(request);
	return request;
}



#endif 
// SYMBIAN_NETWORKING_UMTSR5

