// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "NOTIFY.H"
#include "ETELFAX.H"
#include "FAX.H"
#include "mSLOGGER.H"

//
// CFaxHayes - Fax functionality
//

void CFaxHayes::CloseFax(TAny* aObj)
//
// Utility func for cleanup stack
//
	{
	((CObject*)aObj)->Close();
	}

CFaxHayes* CFaxHayes::NewL(CCallMobileFax* aCallFaxHayes,CPhoneGlobals* aPhoneGlobals)
//
//	Completes successfully if fax call has already been dialled creating a CETelFaxBase 
//  instance, and is not called if that is not true.
//
	{
	CFaxHayes* fax=new(ELeave) CFaxHayes(aCallFaxHayes,aPhoneGlobals);
	TCleanupItem newFaxHayesClose(CloseFax,fax);
	CleanupStack::PushL(newFaxHayesClose);
	fax->ConstructL();
    CleanupStack::Pop();
	return fax;
	}
	
RHandleBase* CFaxHayes::GlobalKernelObjectHandle()
	{	
	return iOwner->GlobalKernelObjectHandle();
	}

CFaxHayes::CFaxHayes(CCallMobileFax* aCallFaxHayes,CPhoneGlobals* aPhoneGlobals)
								 : iPhoneGlobals(aPhoneGlobals), iOwner(aCallFaxHayes)
	{}

void CFaxHayes::ConstructL()
//
//	Used to be that CFaxHayes could not be constructed before a call had been initiated
//  ie the fax server was started and the call had a pointer to CETelFaxBase.
//	Now, to solve the problem of the fax progress being only available after phase C, 
//  RFax can be opened before a call has begun to connect. So in the event of this happening,
//  each function of CFaxHayes must check that the fax server has been started.
//	
//	
	{
	LOGTEXT(_L8("CFaxHayes::ConstructL()"));
	iDataDirection = EUnknown;
	}

CFaxHayes::~CFaxHayes()
	{
	iOwner->RemoveFax(this);
	iPhoneGlobals->iNotificationStore->RemoveClientFromLastEvents(this);
	}

CTelObject::TReqMode CFaxHayes::ReqModeL(const TInt aIpc)
	{
	TReqMode reqMode = CFaxBase::ReqModeL(aIpc);
	if ((reqMode & KReqModeFlowControlObeyed) && iPhoneGlobals->iPhoneStatus.iDataPortLoaned)
		{
		LOGTEXT2(_L8("ReqModeL Leaving with KErrInUse as data port is loaned (aIpc=%d)"),aIpc);
		User::Leave(KErrInUse);
		}
	return reqMode;
	}

TInt CFaxHayes::RegisterNotification(const TInt /*aIpc*/)
	{
	return KErrNone;
	}
TInt CFaxHayes::DeregisterNotification(const TInt /*aIpc*/)
	{
	return KErrNone;
	}

void CFaxHayes::Init()
	{}

TInt CFaxHayes::CheckAndSetRegistrationParams(const TInt /*aIpc*/,const TDes8* /*aDes1*/,const TDes8* /*aDes2*/)
	{
	return KErrNone;
	}

TInt CFaxHayes::Read(const TTsyReqHandle aTsyReqHandle, TDes8* aDes)
//
//	Read fax data into descriptor, asynchronously. Use NotificationStore to notify client 
//	when completed
//
    {
	if (iOwner->iFaxSession==NULL)
		{
		ReqCompleted(aTsyReqHandle,KErrNotReady);
		return KErrNone;
		}	
	LOGTEXT(_L8("Fax:\tCalling RxFaxData"));
	iDataDirection = EReceiveData;
	CFaxSession* session = iOwner->iFaxSession;

	iPhoneGlobals->iNotificationStore->RegisterNotification(EReadOrWriteFax,aTsyReqHandle,this);
	session->RxFaxData(*aDes);
	return KErrNone;
	}

TInt CFaxHayes::Write(const TTsyReqHandle aTsyReqHandle, TDesC8* aDes)
//
//	Send fax data in descriptor to fax module, asynchronously
//
    {
	if (iOwner->iFaxSession==NULL)
		{
		ReqCompleted(aTsyReqHandle,KErrNotReady);
		return KErrNone;
		}	
	LOGTEXT(_L8("Fax:\tCalling TxFaxData"));
	iDataDirection = ESendData;
	CFaxSession* session = iOwner->iFaxSession;
	iPhoneGlobals->iNotificationStore->RegisterNotification(EReadOrWriteFax,aTsyReqHandle,this);
	session->TxFaxData(*aDes);
	return KErrNone;
	}

TInt CFaxHayes::WaitForEndOfPage(const TTsyReqHandle aTsyReqHandle)
//
//	Send request for notification of end of page to fax module
//
    {
	if (iOwner->iFaxSession==NULL)
		{
		ReqCompleted(aTsyReqHandle,KErrNotReady);
		return KErrNone;
		}	
	LOGTEXT(_L8("Fax:\tCalling Wait for end of page"));
	CFaxSession* session = iOwner->iFaxSession;
	if (iDataDirection==EUnknown)
		{
		ReqCompleted(aTsyReqHandle,KErrUnknown); // client hasn't read or written data yet
		return KErrNone;
		}
	iPhoneGlobals->iNotificationStore->RegisterNotification(EEndOfFaxPage,aTsyReqHandle,this);
	LOGTEXT(_L8("Fax:\tEnd Of Page Notification lodged"));	
	if (iDataDirection==EReceiveData)
		session->RxPostPage();
	else 
		session->TxPostPage();
	return KErrNone;
	}

TInt CFaxHayes::TerminateFaxSession(const TTsyReqHandle aTsyReqHandle)
//
//	Cancel the entire fax session. Server completes fax notification requests that it knows
//	as notifications. TSY treats ReadOrWrite and EndOfPage as notifications although they 
//  have no Cancel function, so it must remove them from its notification list here 
//	explicitly
//
    {
	if (iOwner->iFaxSession==NULL)
		{
		ReqCompleted(aTsyReqHandle,KErrNotReady);
		return KErrNone;
		}	
	LOGTEXT(_L8("Fax:\tCancelling fax session"));
	CFaxSession* session= iOwner->iFaxSession;
	session->Cancel();
	iPhoneGlobals->iNotificationStore->CheckNotification(session,EFaxSessionTerminated,KErrCancel);
	ReqCompleted(aTsyReqHandle,KErrNone);	
	return KErrNone;
	}
