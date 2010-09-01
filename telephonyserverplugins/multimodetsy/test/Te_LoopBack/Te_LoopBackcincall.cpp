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
// This file implements various tests for incoming call handling.
// See CDataCall.cpp for fully documented test scenario.
// 
//

/**
 @file
*/

#include <e32test.h>
#include <mmretrieve.h>
#include <etelmm.h>
#include <faxstd.h>
#include "Te_LoopBackcincall.h"
#include "../../hayes/TSYCONFG.H" // for KInternetAccessPoint

//
// Test-side class
// With the assistance of the base class, this class must start the emulator
// and drive the ETel API.
//
CTestDriveInCall* CTestDriveInCall::NewL(const TScriptList aScriptListEntry)
	{
	CTestDriveInCall* aA = new (ELeave) CTestDriveInCall(aScriptListEntry);
	CleanupStack::PushL(aA);
	aA->ConstructL();
	CleanupStack::Pop();
	return (aA);
	}

CTestDriveInCall::CTestDriveInCall(const TScriptList aScriptListEntry) :
	iScriptListEntry(aScriptListEntry)
	{}

TInt CTestDriveInCall::RunTestL()
	{
	iCurrentScript = iScriptListEntry;
	return (StartEmulatorL());
	}

TInt CTestDriveInCall::DriveETelApiL()
//
// This function contains the real meat of the Client-side test code
//
	{
	_LIT(KVoiceLineName, "Voice");
	_LIT(KDataLineName,  "Data");
	_LIT(KFaxLineName,   "Fax");

	RLine voiceLine;
	INFO_PRINTF1(_L("Opening Voice Line\n"));
	TESTL(voiceLine.Open(iPhone,KVoiceLineName) == KErrNone);
	RCall voiceCall;
	INFO_PRINTF1(_L("Opening New Voice Call\n"));
	TESTL(voiceCall.OpenNewCall(voiceLine) == KErrNone);

	RLine dataLine;
	INFO_PRINTF1(_L("Opening Data Line\n"));
	TESTL(dataLine.Open(iPhone,KDataLineName) == KErrNone);
	RCall dataCall;
	INFO_PRINTF1(_L("Opening New Data Call\n"));
	TESTL(dataCall.OpenNewCall(dataLine) == KErrNone);

	RLine faxLine;
	INFO_PRINTF1(_L("Opening Fax Line\n"));
	TESTL(faxLine.Open(iPhone,KFaxLineName) == KErrNone);
	RCall faxCall;
	INFO_PRINTF1(_L("Opening New Fax Call\n"));
	TESTL(faxCall.OpenNewCall(faxLine) == KErrNone);

	TRequestStatus stat1, stat2, stat3, reqStatus;

	RMobilePhone::TMMTableSettings tableSettings;
	tableSettings.iLocId=KInternetAccessPoint;
	RMobilePhone::TMMTableSettingsPckg tableSettingsPckg(tableSettings);
	iPhone.InitialiseMM(reqStatus , tableSettingsPckg); 	
	User::WaitForRequest(reqStatus);
	TESTL(reqStatus == KErrNone);

// Now wait for an incoming voice call...
	INFO_PRINTF1(_L("Answering a Voice Call...\n"));
	voiceCall.AnswerIncomingCall(stat1);
	dataCall.AnswerIncomingCall(stat2);
	faxCall.AnswerIncomingCall(stat3);
	User::WaitForRequest(stat1);
	TESTL(stat1 == KErrNone);
	dataCall.AnswerIncomingCallCancel();
	faxCall.AnswerIncomingCallCancel();
	User::WaitForRequest(stat2);
	User::WaitForRequest(stat3);
	User::After(1000000L);
	INFO_PRINTF1(_L("Hangup a Voice Call...\n"));
	TESTL(voiceCall.HangUp() == KErrNone);

// Now wait for an incoming fax call...
	INFO_PRINTF1(_L("Answering a Fax Call...\n"));
	RCall::TFaxSessionSettings faxSessionSettings;
	faxSessionSettings.iMode = RCall::EReceive;
	faxSessionSettings.iFaxRetrieveType = RCall::EFaxOnDemand;
	faxSessionSettings.iFaxClass = EClassAuto;
	faxSessionSettings.iFaxId.Zero();
	faxSessionSettings.iMaxSpeed = 14400;
	faxSessionSettings.iMinSpeed = 12000;
	faxSessionSettings.iRxResolution = EFaxNormal;
	faxSessionSettings.iRxCompression = EModifiedHuffman;
	faxCall.SetFaxSettings(faxSessionSettings);

	faxCall.AnswerIncomingCall(stat3);
	User::WaitForRequest(stat3);
	TESTL(stat3 == KFaxErrReceiveTimeout); 		// We don't actually send any data
	User::After(300000L);
	INFO_PRINTF1(_L("Hangup a Fax Call...\n"));
	TESTL(faxCall.HangUp() == KErrNone);

// Now wait for an incoming data call...
	INFO_PRINTF1(_L("Answering a Data Call\n"));
	TESTL(dataCall.AnswerIncomingCall() == KErrNone);
	User::After(300000L);
	TESTL(dataCall.HangUp() == KErrNone);

//	Get the FaxSettings before closing the line and call
	TInt ret = faxCall.GetFaxSettings(faxSessionSettings);
	TEST_CHECKL(ret, KErrNone, _L("*** Failed to get fax setttings ***"));
	
	faxCall.Close();
	faxLine.Close();
	dataCall.Close();
	dataLine.Close();
	voiceCall.Close();
	voiceLine.Close();

	return (KErrNone);
	}

//
// Emulator-side class
// With the assistance of the base class, this class must run the designated script
//
CTestInCall* CTestInCall::NewL(const TScript* aScript)
	{
	CTestInCall* aA = new (ELeave) CTestInCall(aScript);
	CleanupStack::PushL(aA);
	aA->ConstructL();
	CleanupStack::Pop();
	return (aA);
	}

CTestInCall::CTestInCall(const TScript* aScript) :
	iScript(aScript)
	{}

void CTestInCall::ConstructL()
	{
	CATScriptEng::ConstructL();
	}

TInt CTestInCall::Start()
	{
	StartScript(iScript);
	return (KErrNone);
	}

void CTestInCall::SpecificAlgorithmL(TInt /* aParam */)
	{
	}

void CTestInCall::Complete(TInt aError)
	{
	iReturnValue = aError;
	CActiveScheduler::Stop();
	}
