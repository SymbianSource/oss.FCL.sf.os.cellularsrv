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

#include "FAXSERV.H"
#include "fax_reversebytes.h"
#include "FAXMDRV.H"
#include "FAXMODEM.H"
#include <et_phone.h>

#include "FAXLOG.H"


/*********************************************************************/

CFaxSession *CFaxSession::NewLC ()
	{
	CFaxSession *self = new (ELeave) CFaxSession;
	CleanupStack::PushL (self);
	return self;
	}

CFaxSession *CFaxSession::NewL ()
	{
	CFaxSession *self = NewLC ();
	CleanupStack::Pop ();
	return self;
	}
/********************************************************************/
CFaxSession::CFaxSession(void)
            :CBase()
{
}

CFaxSession::~CFaxSession ()
	{
	FxClose ();
	delete iSharedFileHandles;
	}

void CFaxSession::SetCallBack(MFaxCompletionBase* aCompletionBase)
	{
	iCompletionBase = aCompletionBase;
	}

/********************************************************************/

// this kicks off the send or receive session by launching a separate high
 // priority faxthread.  A FxOpen must be paired with a call to FxClose
 // as this is an EPOC32 requirement
 //
 // we are part of c32 here, so we cannot set a thread priority
 //
 // the heap and stack sizes set here (4K each) are pure guesswork
 // we need a handle to this parent thread so that our child thread
 // is able to signal back to us via a TRequestStatus

TInt CFaxSession::FxOpen (TFaxServerSessionSettings & aSettings,RFax::TProgress* aProgress)
{
	
    __FLOG_FAXSRV( _L8("CFaxSession::FxOpen entering"));

	ASSERT (iFaxRequest == NULL);
	iFaxServerSessionSettings = aSettings;

// Initialise the Progress Settings
	iProgress = aProgress;
	iProgress->iLastUpdateTime=0;
	iProgress->iAnswerback.Zero();
	iProgress->iPhase = ENotYetStarted;
	iProgress->iSpeed = 9600;
	iProgress->iResolution = EFaxNormal;
	iProgress->iCompression = EModifiedHuffman;
	iProgress->iECM = 0;
 	iProgress->iPage = 0;
	iProgress->iLines = 0;

	TRAPD (state, iFaxRequest = CFaxRequest::NewL (this));
	if (state == KErrNone)
		{
		iFaxRequest->iChildThread.Logon (iChildDeath);
		iFaxRequest->iChildThread.Resume ();
		User::WaitForRequest (iFaxRequest->iThreadStat);
		CActiveScheduler::Add (iFaxRequest);
		}
	return (state);
}
/********************************************************************/

TInt CFaxSession::FxClose ()
{
    __FLOG_FAXSRV( _L8("CFaxSession::FxClose entering"));

	if (iFaxRequest)
		{
		if (iFaxRequest->IsActive ())
			{
			Cancel ();
			}
		iFaxRequest->FaxRequest (EFxClose);
		User::WaitForRequest (iChildDeath);
		delete iFaxRequest;
		iFaxRequest = NULL;
		}
	if (!iAmDestructing)
		{
		iAmDestructing = ETrue;
		delete this;
		}
	return (KErrNone);
}
/********************************************************************/

MFaxCompletionBase* CFaxSession::ReturnCompletionBase ()
{
	return (iCompletionBase);
}

/*******************************************************************/

void CFaxSession::RxConnect ()
{
	iFaxRequest->FaxRequest (ERxConnect);
}
/********************************************************************/

void CFaxSession::RxFaxData (TDes8 & aData)
{
	iRxData = &aData;
	iFaxRequest->FaxRequest (ERxFaxData);
}
/********************************************************************/

void CFaxSession::RxPostPage ()
{
	iFaxRequest->FaxRequest (ERxPostPage);
}
/********************************************************************/

void CFaxSession::TxConnect ()
{
	iFaxRequest->FaxRequest (ETxConnect);
}
/********************************************************************/

void CFaxSession::TxFaxData (const TDesC8 & aData)
{
	iTxData = &aData;
	iFaxRequest->FaxRequest (ETxFaxData);
}
/********************************************************************/

void CFaxSession::TxPostPage ()
{
	iFaxRequest->FaxRequest (ETxPostPage);
}

void CFaxSession::Cancel ()
{
	iFaxRequest->Cancel ();
}

void CFaxSession::StartModemL ()
{
    __FLOG_FAXSRV( _L8("CFaxSession::StartModemL entering"));

	if (iModemDriver != NULL)
		return;

	if (iFaxServerSessionSettings.iFaxClass == EClass1)
		iModemDriver = CFaxClass1::NewL (&iFaxServerSessionSettings,*iProgress);
	else if (iFaxServerSessionSettings.iFaxClass == EClass2)
		iModemDriver = CFaxClass2::NewL (&iFaxServerSessionSettings,*iProgress);
	else if (iFaxServerSessionSettings.iFaxClass == EClass2point0)
		iModemDriver = CFaxClass20::NewL (&iFaxServerSessionSettings,*iProgress);
	else
		User::Leave (KFaxCannotAutodetect);

	iModemDriver->iFaxServerSessionSettings = &iFaxServerSessionSettings;
   iCompletionBase->GetCadenceAndTimeOfLastRing(iModemDriver->iCadence, iModemDriver->iTimeOfLastRing);
}

void CFaxSession::SetFaxHeaderFile(CFaxSharedFileHandles* aSharedFileHandles)
	{
	//if we already have an object then delete it and use this one instead
	if(iSharedFileHandles)
	{
		delete iSharedFileHandles;
		iSharedFileHandles=NULL;
	}
	//we are now owners of this object and are responsible for its deletion.
	iSharedFileHandles = aSharedFileHandles;
	}

/********************************************************************/

// the CFaxRequest class is our Fax Server Active Object

/*********************************************************************/

CFaxSession::CFaxRequest::CFaxRequest ()
:CActive (1)
{
}
/********************************************************************/

CFaxSession::CFaxRequest *CFaxSession::CFaxRequest::NewLC (CFaxSession * aFaxSession)
{
	CFaxSession::CFaxRequest *self = new (ELeave) CFaxSession::CFaxRequest ();
	CleanupStack::PushL (self);
	self->ConstructL (aFaxSession);
	return self;
}
/********************************************************************/

CFaxSession::CFaxRequest *CFaxSession::CFaxRequest::NewL (CFaxSession * aFaxSession)
{
	CFaxSession::CFaxRequest *self = NewLC (aFaxSession);
	CleanupStack::Pop ();
	return self;
}
/********************************************************************/

void CFaxSession::CFaxRequest::ConstructL (CFaxSession * aFaxSession)
{
	TInt stackSize = 0x1400;
	_LIT(KFaxThread,"FaxServerThread");

	iFaxSession = aFaxSession;
	TInt res = iFaxSession->iParentThread.Duplicate (RThread ());
	if (res == KErrNone)
		res = iChildThread.Create (KFaxThread,
			FaxServerThread,
			stackSize,
			NULL,
			iFaxSession,
			EOwnerProcess);
	if (res)
		User::Leave (KFaxThreadError);
}
/********************************************************************/

CFaxSession::CFaxRequest::~CFaxRequest ()
{
	iFaxSession->iParentThread.Close ();
	iChildThread.Close ();
}
/********************************************************************/

// once we have our active object, we simply call its FaxRequest
 // it re-activates the faxserver thread to process the request
 // and sets the FaxRequest object active before returning

void CFaxSession::CFaxRequest::FaxRequest (CFaxSession::TFaxThreadRequest aFaxThreadRequest)
{
	TRequestStatus *threadStatus = &iThreadStat;
	iFaxThreadRequest = aFaxThreadRequest;
	if (iFaxThreadRequest != EFxClose)
		{
		iStatus = KRequestPending;
		SetActive ();
		}
	iChildThread.RequestComplete (threadStatus, aFaxThreadRequest);
}
/********************************************************************/

// here we request a cancel of a fax call

void CFaxSession::CFaxRequest::DoCancel ()
{
	iCancel = 1;
	if (iFaxSession->iModemDriver)
		{
		if (iFaxSession->iModemDriver->iModem->iCancel == 0)
			iFaxSession->iModemDriver->iModem->iCancel++;
		}
}
/********************************************************************/

void CFaxSession::CFaxRequest::RunL ()
{
	switch (iFaxThreadRequest)
		{
		case ERxConnect:
				{
			iFaxSession->ReturnCompletionBase()->RxConnectComplete (iStatus.Int ());
			break;
				}

		case ERxFaxData:
				{
				iFaxSession->ReturnCompletionBase()->RxFaxDataComplete (iStatus.Int ());
			break;
				}

		case ERxPostPage:
				{
				iFaxSession->ReturnCompletionBase()->RxPostPageComplete (iStatus.Int ());
				break;
				}

		case ETxConnect:
				{
				iFaxSession->ReturnCompletionBase()->TxConnectComplete (iStatus.Int ());
				break;
				}

		case ETxFaxData:
				{
				iFaxSession->ReturnCompletionBase()->TxFaxDataComplete (iStatus.Int ());
				break;
				}

		case ETxPostPage:
				{
				iFaxSession->ReturnCompletionBase()->TxPostPageComplete (iStatus.Int ());
				break;
				}
		default:;
		}
}
/********************************************************************/
// this is a utility function which is the entry to our thread
 // it isn't part of any class, but we pass the address
 // of our CFaxModemDriver in so that we can check the
 // session parameter and get back to the required function
 //
 // it also has the thread service request dispatcher

TInt FaxServerThread (TAny * session)
	{
	TInt state=0;
	TInt ret;
	TBool terminateThread=EFalse;
	CTrapCleanup *cleanup = CTrapCleanup::New ();
	CFaxSession *faxsession = (CFaxSession *) session;
	TRequestStatus *openStatus = &faxsession->iFaxRequest->iThreadStat;
	faxsession->iParentThread.RequestComplete (openStatus, state);

	FOREVER
		{
		User::WaitForRequest (faxsession->iFaxRequest->iThreadStat);
		state = ret = KErrNone;
		
        __FLOG_FAXSRV1( _L8("FaxServerThread: iThreadStat=%d"), faxsession->iFaxRequest->iThreadStat.Int ());
		
		switch (faxsession->iFaxRequest->iThreadStat.Int ())
			{
			case CFaxSession::ERxConnect:
				TRAP (state, faxsession->StartModemL ());
							
				if (state == KErrNone)
					{
					faxsession->iModemDriver->iModem->iCancel = faxsession->iFaxRequest->iCancel;
					TRAP (state, ret = faxsession->iModemDriver->RxConnectL ());
                    __FLOG_FAXSRV2(_L8("FaxServerThread state: ERxConnect returned ret=%d, state=%d"), ret, state);
					}
				break;
			
			case CFaxSession::ERxFaxData:
				if(faxsession->iModemDriver)
					{
					TRAP (state, faxsession->iModemDriver->GetFaxDataL (faxsession->iRxData));
					__FLOG_FAXSRV2( _L8("FaxServerThread state: ERxFaxData returned ret=%d, state=%d"), ret, state);
					}
				else
					__FLOG_FAXSRV( _L8("FaxServerThread state: ERxFaxData - faxsession->iModemDriver=NULL"));
				break;
			
			case CFaxSession::ERxPostPage:
				if(faxsession->iModemDriver)
					{			
					TRAP (state, ret = faxsession->iModemDriver->RxPostPageL ());
					__FLOG_FAXSRV2( _L8("FaxServerThread: iModemDriver->RxPostPageL returned ret=%d, state=%d"), ret, state);
					}
				else
					__FLOG_FAXSRV( _L8("FaxServerThread: iModemDriver->RxPostPageL - faxsession->iModemDriver = NULL"));
				break;
			
			case CFaxSession::ETxConnect:
				__FLOG_FAXSRV(_L8("FaxServerThread state: ETxConnect"));
				TRAP (state, faxsession->StartModemL ());		//creates an instance of the appropriate 
																//modem driver (CFax1, CFax2, CFax2.0)
				if (state == KErrNone)
					{
					faxsession->iModemDriver->iModem->iCancel = faxsession->iFaxRequest->iCancel; // added now
					faxsession->iModemDriver->SetSharedFileHandles(faxsession->iSharedFileHandles);
					TRAP (state, ret = faxsession->iModemDriver->TxConnectL ());
					}
				break;
			
			case CFaxSession::ETxFaxData:
				if(faxsession->iModemDriver)
					{			
					TRAP (state, ret = faxsession->iModemDriver->SendFaxDataL (faxsession->iTxData));
					__FLOG_FAXSRV2(_L8("FaxServerThread state: ETxFaxData, state =%d, ret=%d"),state, ret);
					}
				else
					__FLOG_FAXSRV(_L8("FaxServerThread state: ETxFaxData, faxsession->iModemDriver = NULL"));
				break;
			
			case CFaxSession::ETxPostPage:
                __FLOG_FAXSRV(_L8("FaxServerThread state: ETxPostPage"));
				if(faxsession->iModemDriver)
					{			
					TRAP (state, ret = faxsession->iModemDriver->TxPostPageL ());
					}
				else
					{
					__FLOG_FAXSRV(_L8("FaxServerThread state: ETxPostPage, faxsession->iModemDriver = NULL"));
					}
				break;
			
			case CFaxSession::EFxClose:
                __FLOG_FAXSRV(_L8("FaxServerThread state: EFxClose:"));

				delete faxsession->iModemDriver;
				faxsession->iModemDriver = NULL;
				terminateThread=ETrue;
				state = KErrNone;
				ret   = KErrNone;
				break;
			
			default:
				state = KErrNone;
				ret   = KErrNone;
                break;
			}
		if (state == KErrNone)
			state = ret;
		if (faxsession->iFaxRequest->iCancel)
			{
			
			__FLOG_FAXSRV1(_L8("FaxServerThread: iCancel=%d"), faxsession->iFaxRequest->iCancel);
			
			state = KFaxCancelRequested;
			delete faxsession->iModemDriver;
			faxsession->iModemDriver = NULL;
			}
		if (faxsession->iFaxRequest->IsActive ())
			{
			TRequestStatus *returnStatus = &faxsession->iFaxRequest->iStatus;
			faxsession->iParentThread.RequestComplete (returnStatus, state);
			}
		if(terminateThread)
			break;
//		if (faxsession->iModemDriver == NULL)
//			break;
		}
	delete cleanup;
	return (state);
}
/*********************************************************************/

TFaxServerSessionSettings& TFaxServerSessionSettings::operator=(const TFaxServerSessionSettings& aSettings)
	{
	iPhoneNumber = aSettings.iPhoneNumber;
	iLogging = aSettings.iLogging;
	iFaxInitString = aSettings.iFaxInitString;
	iMode = aSettings.iMode;;
	iFaxClass = aSettings.iFaxClass;
	iPortDriverName = aSettings.iPortDriverName;
	iCommPortName = aSettings.iCommPortName;
	iFaxId = aSettings.iFaxId;
	iMaxSpeed = aSettings.iMaxSpeed;
	iMinSpeed = aSettings.iMinSpeed;
	iPreferredECM = aSettings.iPreferredECM;
	iFaxOnDemandDelay = aSettings.iFaxOnDemandDelay;
	iTxResolution = aSettings.iTxResolution;
	iTxCompression = aSettings.iTxCompression;
	iTxPages = aSettings.iTxPages;
	iRxResolution = aSettings.iRxResolution;
	iRxCompression = aSettings.iRxCompression;
	return(*this);
	}
 
//
// First Ordinal Functions
//
extern "C"
	{
	IMPORT_C CFaxSession* LibEntry(void);	// Force "Proper Name" export
	}

EXPORT_C CFaxSession* LibEntry()
	{
	return CFaxSession::NewL();
	}

