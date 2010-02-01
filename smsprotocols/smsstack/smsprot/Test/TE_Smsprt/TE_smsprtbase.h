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

/**
 @file
*/

#ifndef _TE_SMSPRTBASE_H_
#define _TE_SMSPRTBASE_H_

#include <commsdattypesv1_1.h>
#include "Te_smsprttestcase.h"
#include "TE_smsprt_alphabet.h"
#include "smsstacktestutilities.h"
#include <e32base.h>
#include <es_sock.h>
#include <f32file.h>
#include <e32test.h>
#include <c32comm.h>
#include <simtsy.h>
#include <gsmumsg.h>
#include <gsmubuf.h>
#include <gsmuset.h>
#include <smsustrm.h>
#include "smspdudb.h"
#include "smspproc.h"
#include "attributes.h"
#include <es_wsms.h>
#include <testconfigfileparser.h>
#include <test/testexecuteserverbase.h>

#define DSTDNC_NEWCALL_FULLNAME				_L("DummyNc::PotsNc::")

_LIT(KGmsSmsConfigFileName,"setupgsmsms.txt");
_LIT(KTSmsPrtConfigFileName, "tsmsprt_config.txt");
_LIT(KTSmsPrtConfigExtraFileName, "tsmsprt_config_extra.txt");
_LIT(KTSmsPrtConfigTxRxMaxMsgFileName, "tsmsprt_config_tx_rx_maxmsg.txt");

_LIT8(KServiceCentreAddress,   "ServiceCentreAddress");
_LIT8(KSmspEntryValidParams,  "SmspEntryValidParams");
_LIT8(KSmspEntryScTypeOfNumber,  "SmspEntryScTypeOfNumber");
_LIT8(KSmspEntryScNumberPlan,  "SmspEntryScNumberPlan");

_LIT(KVodafoneSC,"+447785016005"); //Vodafone SC
_LIT(KRegTestNumber,"+447747065548"); //Number used with regtest
_LIT(KTestNumber,"+447747065825"); //Number used with regtest

_LIT(KSoneraSC,"+358405202000"); //Sonera SC
_LIT(KRadiolinjaSC,"+358508771010"); //Radiolinja SC

_LIT(KPekka,"+358408415528");  //Pekka's telephone number
_LIT(KOther,"+358408415582"); // Other test number
_LIT(KLocalNumber, "08408415528"); //Local test number to verify local/international switching
_LIT(KInternationalTestNumber, "+819021710979"); // International number to be used with a status
                                                 // report containing local version of this number

_LIT8(KTestSendAndRecvMsgsWithDifferentTON,	"SendAndRecvMsgsWithDifferentTON");
_LIT8(KTestSendAndReceiveIndicatorMsgs,	"SendAndReceiveIndicatorMsgs");
_LIT(KConfigFileDir,"sms");
_LIT8(KAlphabet,	"Alphabet");
_LIT8(KTypeOfNumber,	"TypeOfNumber");
_LIT8(KIndicationType,	"IndicationType");
_LIT8(KDCSBits7To4,	"DCSBits7To4");
_LIT8(KIndicationState,	"IndicationState");

_LIT(KText, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123");


const TUint KSocketMessageSlots = 16;   // Override the default value of 8

using namespace CommsDat; 


//
// Test macros
//
#define TESTCHECKL(a, b) \
    { \
    if((a)!=(b)) \
	    { \
	    INFO_PRINTF3(_L("<font color=FF0000>Failed: Got %d while expecting %d.</font>"),a,b); \
        SetTestStepResult(EFail); \
        User::Leave(EFail); \
        } \
    }

#define TESTCHECKCONDITIONL(a) \
    { \
    if (!(a)) \
	    { \
	    INFO_PRINTF1(_L("<font color=FF0000>Failed: Condition was false.</font>")); \
        SetTestStepResult(EFail); \
        User::Leave(EFail); \
        } \
    }
    
#define TESTCHECKSTRL(a, b) \
    { \
    if((a)!=(b)) \
	    { \
	    TBuf<255> temp = a ; \
	    INFO_PRINTF3(_L("<font color=FF0000>Failed: Got \"%S\" while expecting \"%S\".</font>"), &temp,&(b)); \
        SetTestStepResult(EFail); \
        User::Leave(EFail); \
        } \
    }
    
#define TESTCHECK(a, b) \
    { \
    if((a)!=(b)) \
	    { \
	    INFO_PRINTF3(_L("<font color=FF0000>Failed: Got %d while expecting %d.</font>"),a,b); \
        SetTestStepResult(EFail); \
        } \
    }

#define TESTCHECKCONDITION(a) \
    { \
    if (!(a)) \
	    { \
	    INFO_PRINTF1(_L("<font color=FF0000>Failed: Condition was false.</font>")); \
        SetTestStepResult(EFail); \
        } \
    }
    
#define TESTCHECKSTR(a, b) \
    { \
    if((a)!=(b)) \
	    { \
	    TBuf<255> temp = a ; \
	    INFO_PRINTF3(_L("<font color=FF0000>Failed: Got \"%S\" while expecting \"%S\".</font>"), &temp,&(b)); \
        SetTestStepResult(EFail); \
        } \
    }


/**
  Base class housing SMS test Steps 
*/
class CSMSTestSteps : public CTestStep
{
public:

//	static CSMSTestSteps* NewL();
	~CSMSTestSteps();


	enum TCodingScheme{ESevenBit=7, EEightBit};

	/* Functions grabbed from SMS Stack Test Utils */
	//Socket utils
	void OpenSmsSocketL(RSocketServ& aSocketServer, RSocket& aSocket, TSmsAddr& aSmsAddr);
	void OpenSmsSocketL(RSocketServ& aSocketServer, RSocket& aSocket, TSmsAddrFamily aFamily);
	void WaitForInitializeL();
	void SetSimTSYTestNumberL(TInt aTestNumber);	
	void ParseSettingsFromFileL();
	void PrepareRegTestLC(RSocketServ& aSocketServer, TInt aTestNumber);
	CSmsMessage* CreateSmsMessageL(const TDesC& aDes, TSmsDataCodingScheme::TSmsAlphabet aAlphabet, CSmsPDU::TSmsPDUType aType = CSmsPDU::ESmsSubmit);
	CSmsMessage* RecvSmsL(RSocket& aSocket, TInt aIoctl = KIoctlReadMessageSucceeded);
	CSmsMessage* CreateSmsWithStatusReportReqL(const TDesC& aDes, TSmsDataCodingScheme::TSmsAlphabet aAlphabet);
	CSmsMessage* RecvSmsFailedL(RSocket& aSocket);
	CSmsMessage* CreateSmsMessageLC(CSmsPDU::TSmsPDUType aType, CSmsBuffer* aBuffer, const TDesC& aAddress);
	//This method is used to create SMS message from test data
	CSmsMessage* CreateSMSL();
	void SetFreeDiskSpaceL(TInt64 aNewFreeValue);
	void ReleaseDiskSpaceL();
	TUint64 SetHighLowLimitsAndDiskSpaceLevelL(TUint aHighDrop, TUint aLowDrop, TUint aFreeDrop, TUint64 aMax=0x7fffffff);
	void SetFreeDiskSpaceFromDropLevelL(TUint aFreeDrop);
	void SetLowHighLimitsInSmsuRscL(TInt64 aLowLimit, TInt64 aHighLimit);
	void RemoveLowHighLimitsFromSmsuRscL();
	void SendSmsL(const CSmsMessage* aSms, RSocket& aSocket);
	void SendSmsCancelL(CSmsMessage* aSms, RSocket& aSocket1, RSocket& aSocket2);
	TInt SendSmsErrorL(CSmsMessage* aSms, RSocket& aSocket);
	void SendCommandSmsL(CSmsMessage* aSms, RSocket& aSocket);
	void SendAndRecvTestMessageL(const TTestCase& aTestCase, RSocket& aSocket);
	void SendTestMessageL(const TTestCase& aTestCase, RSocket& aSocket);
	void SendSmsDontCheckReturnValueL(CSmsMessage* aSms, RSocket& aSocket);
	void SendAndRecvSms7BitL(const TDesC& aDes, RSocket& aSocket);
	void DoSendAndRecvSmsL(const TDesC& aDes, TSmsDataCodingScheme::TSmsAlphabet aAlphabet, RSocket& aSocket);
	void PrintMessageL(const CSmsMessage* aSms);
	void PrintSmsMessage(const CSmsMessage& aMessage);
	TSmsStatus::TSmsStatusValue RecvStatusReportL(TSmsServiceCenterAddress& aRecipientNumber, RSocket& aSocket);
	void WaitForRecvL(RSocket& aSocket);
	void TestSmsContentsL(CSmsMessage* aSms, const TDesC& aDes, TBool aIgnorePrintOutput = EFalse);	
	void TestMessageContentsL(CSmsMessage* aSms, const TTestCase& aTestCase);
	void WriteSmsToSimL(CSmsMessage& aSms, RSocket& aSocket);
	void WriteSmsLeaveIfErrorL(const CSmsMessage& aSms, RSocket& aSocket);
	void ReadSmsStoreL(RSocket& aSocket, RPointerArray<CSmsMessage>& aMessages);
	void ReadSmsStoreL(RSocket& aSocket, RPointerArray<CSmsMessage>& aMessages, TRequestStatus &aStatus);
	TInt DeleteSmsL(const CSmsMessage& aSms, RSocket& aSocket);
	void DeleteSmsLeaveIfErrorL(const CSmsMessage& aSms, RSocket& aSocket);
	void SetIndicatorL(const TTestCase& aTestCase, CSmsMessage* aSms);
	void FillDes(TDes& aDes,TInt aLength);
	TInt MakeReadSmsStoreRequestL(RSocket& aSocket);
	TBool TimedWaitForRecvL(RSocket& aSocket, TUint aDelay);
	TInt CancelWriteSmsToSimL(CSmsMessage& aSms, RSocket& aSocket, TInt aDelay);
	TBool DoSingleTestCaseL( const TDesC8& aSection, TInt aCount );
	void ReadPduL( TBuf8<64> aSectionName, RPointerArray<CSmsPduDbMessage>& aArray );
	void TestSendAndRecvMsgsWithDifferentTONL( const RPointerArray<CSmsPduDbMessage>& aArray,
												   const CArrayFixFlat<TInt>* aAlphabetArray,
												   const CArrayFixFlat<TInt>* aTypeOfNumberArray );
	void TestSendAndReceiveIndicatorMsgsL( const RPointerArray<CSmsPduDbMessage>& aArray,
											   const CArrayFixFlat<TInt>* aIndicationStateArray,
											   const CArrayFixFlat<TInt>* aDcsBits7To4Array,
											   const CArrayFixFlat<TInt>* aIndicationTypeArray );
	RFs iFs;
	RSocketServ *iSocketServer;
	
	TSmsServiceCenterAddress iTelephoneNumber; //Test SIM
	TSmsServiceCenterAddress iServiceCenterNumber; //Radiolinja

	void WriteToSIML(RSocket& aSocket, const CSmsMessage& aMsg);

	CSmsStackTestUtils* iSmsStackTestUtils;

	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();	
	
protected:
	CSmsMessage *iSms;
};


/**
	Legacy base class.  This was the original base class.  It has a dependency with SmsStackTestUtils.
	Its preamble and postamble steps create and destroy the socket server session and socket 
	subsession - This is limiting as it only allows one test step to execute within a session.
	
	The CSmsTestSteps was abstracted from this class. This has effectively removed this constraint.  
	The CSmsTestSteps share the socket session and subsession, which establishes re-usable test steps.  
*/
class CSmsPrtTestStep : public CSMSTestSteps
{

public:
 	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	void DoESockMemoryLeakTestL();

public:
	CActiveScheduler* iScheduler;
};


/**
 *  Test Step base case for all encoding tests. 
 */
class CSmsPrtEncodingTestStep : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();

protected:
	void GetExpectedPDUCountAndUDUnitsFreeL(TInt aOctetsCount, 
			                                TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
			                                TSmsEncoding aEncodingExpected,
				                            TInt& aExpectedPDUs,
				                            TInt& aExpectedFreeUDUnitsInLastPDU) const;
	void GetExpectedEncodingInfoL(TDesC& aDes,
								  TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
				                  TInt& aExpectedUnconvertibleChars,
								  TInt& aExpectedFirstUnconvertibleChar) const;
	void GetExpectedEncodingInfoL(TDesC& aDes,
								  TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
								  TSmsEncoding aEncoding,
				                  TInt& aExpectedUnconvertibleChars,
				                  TInt& aExpectedDowngradedChars,
				                  TInt& aExpectedAlternativeEncodingChars,
								  TInt& aExpectedFirstUnconvertibleChar) const;
	
	void DoEncodingTestL(RSocket aSocket,
						 const TDesC& aTestName,
		                 TInt aTestSequences,
						 RArray<TPtrC>& aTestCharSetNames,
						 TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
						 TSmsEncoding aEncodingToUse,
						 TSmsEncoding aEncodingExpected);

	void DoEncodingTestL(RSocket aSocket,
	                     const TDesC& aTestName,
	                     TInt aTestCharacters,
	                     RArray<TPtrC>& aTestCharSetNames,
	                     TInt aAdditionalTestCharacters,
	                     RArray<TPtrC>& aAdditionalTestCharSetNames,
	                     TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
	                     TSmsEncoding aEncodingToUse,
	                     TSmsEncoding aEncodingExpected);
	
	TInt PerformCharacterTestsL(CSmsMessage* aSmsMessage, 
	        TDesC16& aOriginalPtr,
	        TSmsDataCodingScheme::TSmsAlphabet& aAlphabet,
            TSmsEncoding aEncodingToUse,
            TSmsEncoding aEncodingExpected,
            TInt aOctetsCount,
            TBool aExtendedCharAdded
            );
	
protected:
	CSmsPrtTestAlphabet* iTestAlphabet;
    RArray<TPtrC> iCharSets;
    RArray<TPtrC> iAdditionalCharSets;
};

#endif // _TE_SMSPRTBASE_H_
