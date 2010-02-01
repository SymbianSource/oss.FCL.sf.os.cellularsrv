// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __TE_SMSPRT_H_
#define __TE_SMSPRT_H_


#include "TE_smsprtbase.h"
#include "logcheck.h"


class CTestSimpleTxAndRx : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestBinaryTxAndRx : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestStatusReport : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestTxWithError : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestTxFailed : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMatchingToObserver : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestCommandMessages : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSimpleRx : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTest7bitMessTest : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestClassMessages : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestRxConcatenated : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestRxDuplicate : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestRxRingToneA : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestTxRxConcatenated : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestParamStorage : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSmsStore : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSmsStoreList : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestDeleteSms : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSocketBinding : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSmsEventLogger : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestBearerChange : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestRestoreBearer : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestRecvModeChange : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestTsyCaps : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestOOMSendSms : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestOOMWriteSms : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestOOMReadSms : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestOOMReadSmsList : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestOOMDeleteSms : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestOOMSmsParams : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMeStoreDupAndMiss : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSimStoreDupAndMiss : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestRxCphs : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};

class CTestInvalidPDUs : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestStress : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestCombinedStore : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestParamsInter : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestTxRxAlphanumeric : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestRxSuspend : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestEnumerateCorrupted : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestCancelling : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSendAndRecvMsgsWithDifferentTON : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSendAndReceiveIndicatorMsgs : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSendRecvMaxLengthMsg : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSendAndRecvMsgsWithDifferentPID : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CSmsParamsErrorCases : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();

private:
	void TestSmsParamsErrorCasesL(
		RMobileSmsMessaging::TMobileSmspStoreValidParams aSmspEntryValidParams,
		RMobilePhone::TMobileTON aSmspEntryScTypeOfNumber,
		RMobilePhone::TMobileNPI aSmspEntryScNumberPlan,
		const TDesC8& aSmspEntryScTelNumber);
	};


class CTestResendFailedConcatinatedMessage : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMultipartWapMessage : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMOSESMessage : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestClockBack : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestEnumerateNoMessages : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestIE : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestEmailIE : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMaxReadTimeForSmsStoreList : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSameLogIdMultiplePDU	: public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSameLogIdSinglePDU : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestConcatenatedMessageLogging : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CSmsLogChecker;

class CTestLog : public CActive
	{
	public:
		static CTestLog* NewLC(CSmsStackTestUtils& aUtils, RFs& aFs, CSmsPrtTestStep* aTest, TInt aPriority);
		~CTestLog();

		void StartL();

	private:

		void ConstructL(RFs& aFs);
		CTestLog(CSmsStackTestUtils& aUtils, CSmsPrtTestStep* aTest, TInt aPriority);
		void RunL();
		void DoCancel() {};
		TInt RunError(TInt aError) {Complete(aError); return KErrNone;}
		void Complete(TInt aError) {iStatus = aError; CActiveScheduler::Stop();}
		void StartOriginalL();
		void StartCompareL(TInt aExpectError);

	private:

		enum TState
			{
			EOriginal
			} iState;

		CSmsStackTestUtils& iUtils;
		CSmsPrtTestStep* iTest;

		CSmsLogChecker* iLogChecker;
		CSmsEventLogger* iEventLogger;
		CSmsMessage* iMessage;
		RSocketServ iSocketServer;
		RSocket iSocket;

		RArray<TLogId> iAddedIds;
		TInt iNumberToAdd;
		TLogSmsPduData iData;
	};


class CTestEnumerationOfStatusReport : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestWriteStatusReportToSIM : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestTxSmsWithDelayedCompletion : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestSmsStoreReadCancel : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestBindWhenPoweredDownWithPublishSubscribe : public CSmsPrtTestStep
	{
public :
	TVerdict doTestStepL();
	};


class CTestObserverNotifiedWhenPoweredDownWithPublishSubscribe : public CSmsPrtTestStep
	{
public :
	TVerdict doTestStepL();
	};


class CTestSmsCNumberChangeWithPublishSubscribe : public CSmsPrtTestStep
	{
public :
	TVerdict doTestStepL();
	};


class CTestStatusReportTime : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestTx8BitWapWithStatus : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestSimStoreCorruptMessage : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestCorruptPduWithNegativeResponse : public CSmsPrtTestStep
	{
public:
    TVerdict doTestStepL();
	};


class CTestBootTimer : public CSmsPrtTestStep
	{
public:
    TVerdict doTestStepL();
	};


class CTestCSmsMessageTimeStamp : public CSmsPrtTestStep
	{
public:
    TVerdict doTestStepL();
	};


class CTestCSmsMessageWithDeliverPDU : public CSmsPrtTestStep
	{
public:
    TVerdict doTestStepL();
	};


class CTestCSmsMessageWithStatusReportPDU : public CSmsPrtTestStep
	{
public:
    TVerdict doTestStepL();
	};


class CTestCSmsMessageWithSubmitPDU : public CSmsPrtTestStep
	{
public:
    TVerdict doTestStepL();
	};


class CTestHandlePID40h : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestDiskSpaceMidRangeClass0Class2 : public CSmsPrtTestStep
	{
public:	
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepL();
	TVerdict doTestStepPostambleL();
	};


class CTestDiskSpaceLowRangeClass0Class2 : public CSmsPrtTestStep
	{
public:
    TVerdict doTestStepPreambleL();
	TVerdict doTestStepL();
	TVerdict doTestStepPostambleL();
	};


class CTestCorruptPDUs : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestCongestedReceive : public CSmsPrtTestStep
	{
public:
	TVerdict doTestStepL();
	};


class CTestEncodingPDUonBoundary : public CSmsPrtTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};


class CTestReceiveInvalidOrReservedIE : public CSmsPrtTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};


class CTestEncoding7bitStandardGSM : public CSmsPrtEncodingTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};


class CTestEncoding8bitCodePage1252 : public CSmsPrtEncodingTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};


class CTestEncodingUnicode : public CSmsPrtEncodingTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};


class CTestEncoding7bitUnconvertibleGSM : public CSmsPrtEncodingTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};


class CTestEncoding7bitNonStandardGSM : public CSmsPrtEncodingTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};


class CTestEncoding7bitTurkishNationalLanguage : public CSmsPrtEncodingTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};

class CTestEncoding7bitTurkishAndUnconvertible : public CSmsPrtEncodingTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};

class CTestReceivingMessageWithDifferentEncodings : public CSmsPrtTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};

class CTestReceivingMessageAfterSocketClosure : public CSmsPrtTestStep
	{
public:
	virtual enum TVerdict doTestStepL();
	};

class CTestSimpleTxAndRxWithLoggingDisabled : public CSmsPrtTestStep
 	{
public:
 	virtual TVerdict doTestStepL();
 	};

class CTestEncoding7bitNationalLanguages : public CSmsPrtEncodingTestStep
    {
public:
    virtual enum TVerdict doTestStepL();
    };

class CTestEncodingMultiPDUwith7bitNationalLanguages : public CSmsPrtEncodingTestStep
    {
public:
    virtual enum TVerdict doTestStepL();
    };

class CTestEncodingDowngradedWith7bitNationalLanguages : public CSmsPrtEncodingTestStep
    {
public:
    virtual enum TVerdict doTestStepL();
    };

class CTestOptimumEncodingWithLockingAndShift7bitNationalLanguages : public CSmsPrtEncodingTestStep
    {
public:
    virtual enum TVerdict doTestStepL();
    };

class CTestOptimumEncodingWithLockingAndDowngrade7bitNationalLanguages : public CSmsPrtEncodingTestStep
    {
public:
    virtual enum TVerdict doTestStepL();
    };

class CTestOptimumEncodingWithLockingAndUnconvertible7bitNationalLanguages : public CSmsPrtEncodingTestStep
    {
public:
    virtual enum TVerdict doTestStepL();
    };

#endif
