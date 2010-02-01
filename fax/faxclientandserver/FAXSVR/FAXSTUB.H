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

/**@file
   @internalComponent */
#ifndef __FAXSTUB_H
#define __FAXSTUB_H

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#include "etel.h"
#include "FAXLOG.H"

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#include "FAXSTD.H"
/**
@internalComponent
*/
#define KMaxDialLength		50		
#define KConfigStringLength 32		

//**********************************
// TFaxServerSessionSettings
//**********************************
/**
@internalComponent
*/	   
struct TFaxServerSessionSettings
	{
	TFaxServerSessionSettings& operator=(const TFaxServerSessionSettings& aSettings);
	TBuf8<KMaxDialLength> iPhoneNumber;
	TBool iLogging;
	TBuf8<KConfigStringLength> iFaxInitString;
	TFaxMode iMode;
	TFaxClass iFaxClass;
	TFileName iPortDriverName;
	TName iCommPortName;
	TBuf<RCall::KFaxIdUserNameMaxSize> iFaxId;
	TInt iMaxSpeed;
	TInt iMinSpeed;
	TInt iPreferredECM;
	TInt iFaxOnDemandDelay;
// Tx Settings
	TFaxResolution iTxResolution;
	TFaxCompression iTxCompression;
	TInt iTxPages;
// Rx Settings
	TFaxResolution iRxResolution;
	TFaxCompression iRxCompression;
	};

class MFaxCompletionBase
/**
@internalTechnology
*/
	{
public:
	virtual void RxConnectComplete(TInt aError)=0;		
	virtual void RxFaxDataComplete(TInt aError)=0;		
	virtual void RxPostPageComplete(TInt aError)=0;
	virtual void TxConnectComplete(TInt aError)=0;
    virtual void TxFaxDataComplete(TInt aError)=0;
	virtual void TxPostPageComplete(TInt aError)=0;		

	virtual void GetCadenceAndTimeOfLastRing(TTimeIntervalMicroSeconds& aCadence,TTime& aTime)=0;
	};	


#ifdef __MARM__
/**
@internalComponent
*/
const TInt KEtelFaxTimerGranularity = 31000;
#else
const TInt KEtelFaxTimerGranularity = 80000;
#endif
/**
@internalComponent
*/
#define KDefaultLastUpdateTime  5
#define KDefaultAnswerBack  _L("01814590381")
#define KDefaultPhase  ENotYetStarted
#define KDefaultResolution  EFaxNormal;
#define KDefaultCompression  EModifiedHuffman;
#define KDefaultPageNumber  1
#define KDefaultLinesNumber  3
#define KDefaultSpeed  9600

#define ASYNC_TIMEOUT		10000
#define ASYNC_LONGTIMEOUT	500000

class TTsyTimer
/**
@internalTechnology
*/
	{
public:
	TTsyTimer();
	TBool iPending;
	TDeltaTimerEntry iEntry;
	};

class CFaxTimer;
class CFaxSession : public CBase
/**
@internalTechnology
*/
	{
public:
    static CFaxSession * NewL ();
    static CFaxSession * NewLC ();

	CFaxSession();
	void ConstructL();
	~CFaxSession();

	virtual TInt FxOpen(TFaxServerSessionSettings& aFaxSettings,RFax::TProgress* aProgress);
	virtual TInt FxClose();

// Rx Fax
	virtual void RxConnect();
	virtual void RxFaxData(TDes8& aData);
	virtual void RxPostPage();
// Tx Fax
	virtual void TxConnect();
	virtual void TxFaxData(const TDesC8& aData);
	virtual void TxPostPage();
	virtual void Cancel();
	virtual void SetCallBack(MFaxCompletionBase* aBase);

	static TInt RxConnectHandler(TAny* aPtr);
	static TInt RxFaxDataHandler(TAny* aPtr);
	static TInt RxPostPageHandler(TAny* aPtr);
	static TInt TxConnectHandler(TAny* aPtr);
	static TInt TxFaxDataHandler(TAny* aPtr);
	static TInt TxPostPageHandler(TAny* aPtr);
	static TInt ProgressChanger(TAny* aPtr);
public:
	RFax::TProgress* iTransferProgress;

	// Timer
	CFaxTimer* iFaxTimer;
	TTsyTimer iRxConnect;
	TTsyTimer iRxFaxData;
	TTsyTimer iRxPostPage;
	TTsyTimer iTxConnect;
	TTsyTimer iTxFaxData;
	TTsyTimer iTxPostPage;
	TTsyTimer iProgressChanger;	
	MFaxCompletionBase* iCompletion;
	};

class CFaxTimer: public CBase
/**
@internalTechnology
*/
	{
public:
	static CFaxTimer* NewL();
	CFaxTimer();
	void ConstructL();
	~CFaxTimer();

	void QueueTimer(TTsyTimer& aTsyTimer,
						TTimeIntervalMicroSeconds32 aTimeInMicroSeconds,
						TInt (*aFunction)(TAny *aPtr), TAny* aPtr);
	TBool RemoveTimer(TTsyTimer& aHandle);
	void ResetPending(TTsyTimer& aHandle);
private:
	CDeltaTimer* iTimer;
	};

#endif //__FAXSTUB_H
