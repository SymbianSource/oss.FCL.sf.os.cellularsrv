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

#include "FAXSTUB.H"

TTsyTimer::TTsyTimer()
	{
	iPending=EFalse;
	}

CFaxSession *CFaxSession::NewLC ()
	{
	return NULL;
	}

CFaxSession *CFaxSession::NewL ()
	{
	CFaxSession* faxstub=new(ELeave) CFaxSession();
	CleanupStack::PushL(faxstub);
	faxstub->ConstructL();
	CleanupStack::Pop();
	return faxstub;
	}

CFaxSession::CFaxSession()
	: CBase()
	{}

void CFaxSession::SetCallBack(MFaxCompletionBase* aCompletionBase)
	{
	iCompletion = aCompletionBase;
	}

void CFaxSession::ConstructL()
	{
	iFaxTimer = CFaxTimer::NewL();
	}

CFaxSession::~CFaxSession()
	{
	
		if(iFaxTimer) //-- LYN-585JX9, code improvements by Dmitry Lyokhin. 11.03.02
		{
			iFaxTimer->RemoveTimer(iRxConnect);
			iFaxTimer->RemoveTimer(iRxFaxData);
			iFaxTimer->RemoveTimer(iRxPostPage);
			iFaxTimer->RemoveTimer(iTxConnect);
			iFaxTimer->RemoveTimer(iTxFaxData);
			iFaxTimer->RemoveTimer(iTxPostPage);

			delete iFaxTimer;
		}


	}


TInt CFaxSession::FxOpen(TFaxServerSessionSettings& aFaxSettings,RFax::TProgress* aProgress)
	{
	iTransferProgress = aProgress;
	iTransferProgress->iLastUpdateTime = KDefaultLastUpdateTime;
	iTransferProgress->iAnswerback = KDefaultAnswerBack;
	iTransferProgress->iPhase = KDefaultPhase;
	iTransferProgress->iResolution = KDefaultResolution;
	iTransferProgress->iCompression = KDefaultCompression;
	iTransferProgress->iPage = KDefaultPageNumber;
	iTransferProgress->iLines = KDefaultLinesNumber;
	iTransferProgress->iSpeed = KDefaultSpeed;
	TFaxServerSessionSettings copy;
	copy = aFaxSettings;
    TBuf8<KMaxFileName> tmpName;    
            
	__FLOG_FAXSTUB (_L8("FaxStub:\tFxOpen called."));
	__FLOG_FAXSTUB (_L8("FaxStub:\tFax settings received: "));
	__FLOG_FAXSTUB1(_L8("Phone Number : %S"),&copy.iPhoneNumber);
	__FLOG_FAXSTUB1(_L8("Fax Init String : %S"),&copy.iFaxInitString);
	__FLOG_FAXSTUB1(_L8("Fax Mode: %d"),copy.iMode);
	__FLOG_FAXSTUB1(_L8("Fax Class: %d"),copy.iFaxClass);
	
     
    tmpName.Copy(_L8("Port Driver Name: "));
    tmpName.Append(copy.iPortDriverName);
    __FLOG_FAXSTUB(tmpName);

	tmpName.Copy(_L8("Comm Port Name: "));
    tmpName.Append(copy.iCommPortName);
    __FLOG_FAXSTUB(tmpName);
	
    tmpName.Copy(_L8("Fax ID: "));
    tmpName.Append(copy.iFaxId);
    __FLOG_FAXSTUB(tmpName);


	__FLOG_FAXSTUB1(_L8("Max speed: %d"),copy.iMaxSpeed);
	__FLOG_FAXSTUB1(_L8("Min speed: %d"),copy.iMinSpeed);
	__FLOG_FAXSTUB1(_L8("Tx compression: %d"),copy.iTxCompression);
	__FLOG_FAXSTUB1(_L8("Rx compression: %d"),copy.iRxCompression);
	__FLOG_FAXSTUB1(_L8("Fax on demand delay: %d"),copy.iFaxOnDemandDelay);
	__FLOG_FAXSTUB1(_L8("Tx Fax Res: %d"),copy.iTxResolution);
	__FLOG_FAXSTUB1(_L8("Tx pages: %d"),copy.iTxPages);
	__FLOG_FAXSTUB1(_L8("Rx Fax Res: %d"),copy.iRxResolution);
	return KErrNone;
	}

TInt CFaxSession::FxClose()
	{
	__FLOG_FAXSTUB(_L8("FaxStub:\tFxClose called."));
	delete this;
	return KErrNone;
	}

// Rx Fax
void CFaxSession::RxConnect()
	{
	__FLOG_FAXSTUB(_L8("FaxStub:\tRxConnect called."));	
	iFaxTimer->QueueTimer(iRxConnect,ASYNC_TIMEOUT,CFaxSession::RxConnectHandler,this);
	iFaxTimer->QueueTimer(iProgressChanger,ASYNC_LONGTIMEOUT,CFaxSession::ProgressChanger,this);
	}

TInt CFaxSession::RxConnectHandler(TAny* aPtr)
	{
	CFaxSession* faxHayes = REINTERPRET_CAST(CFaxSession*,aPtr);
	TTime timeAtLastRing;
	TTimeIntervalMicroSeconds cadence;
	faxHayes->iCompletion->GetCadenceAndTimeOfLastRing(cadence,timeAtLastRing);
	
	__FLOG_FAXSTUB2(_L8("FaxStub:\tTime at last RING is %d:%d"),timeAtLastRing.DateTime().Second(),timeAtLastRing.DateTime().MicroSecond());
	__FLOG_FAXSTUB1(_L8("FaxStub:\tCadence is %d microseconds"),cadence.Int64());

	faxHayes->iCompletion->RxConnectComplete(KErrNone);
	faxHayes->iFaxTimer->ResetPending(((CFaxSession*)aPtr)->iRxConnect);
	return KErrNone;
	}

void CFaxSession::RxFaxData(TDes8& aData)
	{
	__FLOG_FAXSTUB(_L8("FaxStub:\tRxFaxData called."));	
	aData = _L8("INCOMING FAX DATA!!");
	iFaxTimer->QueueTimer(iRxFaxData,ASYNC_TIMEOUT,CFaxSession::RxFaxDataHandler,this);
	}

TInt CFaxSession::RxFaxDataHandler(TAny* aPtr)
	{
	CFaxSession* faxHayes = REINTERPRET_CAST(CFaxSession*,aPtr);
	faxHayes->iCompletion->RxFaxDataComplete(KErrNone);
	faxHayes->iFaxTimer->ResetPending(((CFaxSession*)aPtr)->iRxFaxData);
	return KErrNone;
	}

void CFaxSession::RxPostPage()
	{
	__FLOG_FAXSTUB(_L8("FaxStub:\tRxPostPage called."));	
	iFaxTimer->QueueTimer(iRxPostPage,ASYNC_TIMEOUT,CFaxSession::RxPostPageHandler,this);
	}

TInt CFaxSession::RxPostPageHandler(TAny* aPtr)
	{
	CFaxSession* faxHayes = REINTERPRET_CAST(CFaxSession*,aPtr);
	faxHayes->iCompletion->RxPostPageComplete(KErrNone);
	faxHayes->iFaxTimer->ResetPending(((CFaxSession*)aPtr)->iRxPostPage);
	return KErrNone;
	}

// Tx Fax

void CFaxSession::TxConnect()
	{
	__FLOG_FAXSTUB(_L8("FaxStub:\tTxConnect called."));
	iFaxTimer->QueueTimer(iTxConnect,ASYNC_TIMEOUT,CFaxSession::TxConnectHandler,this);
	}

TInt CFaxSession::TxConnectHandler(TAny* aPtr)
	{
	CFaxSession* faxHayes = REINTERPRET_CAST(CFaxSession*,aPtr);
	faxHayes->iCompletion->TxConnectComplete(KErrNone);
	faxHayes->iFaxTimer->ResetPending(((CFaxSession*)aPtr)->iTxConnect);
	return KErrNone;
	}

void CFaxSession::TxFaxData(const TDesC8& /*aData*/)
	{
	__FLOG_FAXSTUB(_L8("FaxStub:\tTxFaxData called."));	
	iFaxTimer->QueueTimer(iTxFaxData,ASYNC_TIMEOUT,CFaxSession::TxFaxDataHandler,this);
	}

TInt CFaxSession::TxFaxDataHandler(TAny* aPtr)
	{
	CFaxSession* faxHayes = REINTERPRET_CAST(CFaxSession*,aPtr);
	faxHayes->iCompletion->TxFaxDataComplete(KErrNone);
	faxHayes->iFaxTimer->ResetPending(((CFaxSession*)aPtr)->iTxFaxData);
	return KErrNone;
	}

void CFaxSession::TxPostPage()
	{
	__FLOG_FAXSTUB(_L8("FaxStub:\tTxPostPage called."));	
	iFaxTimer->QueueTimer(iTxPostPage,ASYNC_TIMEOUT,CFaxSession::TxPostPageHandler,this);
	}

TInt CFaxSession::TxPostPageHandler(TAny* aPtr)
	{
	CFaxSession* faxHayes = REINTERPRET_CAST(CFaxSession*,aPtr);
	faxHayes->iCompletion->TxPostPageComplete(KErrNone);
	faxHayes->iFaxTimer->ResetPending(((CFaxSession*)aPtr)->iTxPostPage);
	return KErrNone;
	}

void CFaxSession::Cancel()
	{
	__FLOG_FAXSTUB(_L8("FaxStub:\tCancel called - set iTransferProgress.iPhase to EDisconnection"));
	iTransferProgress->iPhase = EDisconnection;
	}

TInt CFaxSession::ProgressChanger(TAny* aPtr)
	{
	CFaxSession* faxHayes = REINTERPRET_CAST(CFaxSession*,aPtr);	
	faxHayes->iTransferProgress->iLines++;
	return KErrNone;
	}

// CFaxTimer

CFaxTimer* CFaxTimer::NewL()
	{
	CFaxTimer* faxTimer = new(ELeave)CFaxTimer();
	CleanupStack::PushL(faxTimer);
	faxTimer->ConstructL();
	CleanupStack::Pop();
	return faxTimer;
	}

CFaxTimer::CFaxTimer()
	{}

void CFaxTimer::ConstructL()
	{
	iTimer=CDeltaTimer::NewL(CActive::EPriorityHigh,KEtelFaxTimerGranularity);
	}

CFaxTimer::~CFaxTimer()
	{
	delete iTimer;
	}

void CFaxTimer::QueueTimer(TTsyTimer& aTsyTimer,
						TTimeIntervalMicroSeconds32 aTimeInMicroSeconds,
						TInt (*aFunction)(TAny *aPtr),TAny* aPtr)
	{
	TCallBack callBackFn(aFunction,aPtr);
	aTsyTimer.iEntry.Set(callBackFn);

	aTsyTimer.iPending=ETrue;
	aTimeInMicroSeconds=aTimeInMicroSeconds.Int()+(KEtelFaxTimerGranularity>>2);
	if(aTimeInMicroSeconds.Int()<100000)
		aTimeInMicroSeconds=aTimeInMicroSeconds.Int()+KEtelFaxTimerGranularity;
	iTimer->Queue(aTimeInMicroSeconds,aTsyTimer.iEntry);
	}

TBool CFaxTimer::RemoveTimer(TTsyTimer& aTsyTimer)
	{
	if (aTsyTimer.iPending)
		{
		aTsyTimer.iPending=EFalse;
		iTimer->Remove(aTsyTimer.iEntry);
		return ETrue;
		}
	return EFalse;
	}

void CFaxTimer::ResetPending(TTsyTimer& aTsyTimer)
	{
	aTsyTimer.iPending=EFalse;
	}

TFaxServerSessionSettings& 
	TFaxServerSessionSettings::operator=(const TFaxServerSessionSettings& aSettings)
	{
	iPhoneNumber = aSettings.iPhoneNumber; iLogging = aSettings.iLogging;
	iFaxInitString = aSettings.iFaxInitString; iMode = aSettings.iMode;;
	iFaxClass = aSettings.iFaxClass;
	iPortDriverName = aSettings.iPortDriverName; iCommPortName = aSettings.iCommPortName; iFaxId = aSettings.iFaxId;
	iMaxSpeed = aSettings.iMaxSpeed;
	iMinSpeed = aSettings.iMinSpeed;
	iPreferredECM = aSettings.iPreferredECM; iFaxOnDemandDelay = aSettings.iFaxOnDemandDelay; iTxResolution = aSettings.iTxResolution; iTxCompression = aSettings.iTxCompression; iTxPages = aSettings.iTxPages;
	iRxResolution = aSettings.iRxResolution; iRxCompression = aSettings.iRxCompression; return(*this);
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
