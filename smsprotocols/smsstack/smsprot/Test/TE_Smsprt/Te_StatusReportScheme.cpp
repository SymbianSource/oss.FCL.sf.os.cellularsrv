// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "Te_StatusReportScheme.h"
#include <gsmuelem.h>

CTestStatusReportScheme::CTestStatusReportScheme()
{

}

CTestStatusReportScheme::~CTestStatusReportScheme()
{

}

TVerdict CTestStatusReportScheme::doTestStepPreambleL()
{
	//Call base class preamble
	CSmsPrtTestStep::doTestStepPreambleL();

	// delete segmentation and reassembly store files before the test
	_LIT(KReassemblyStoreName,"C:\\Private\\101F7989\\sms\\smsreast.dat");
	_LIT(KSegmentationStoreName,"C:\\Private\\101F7989\\sms\\smssegst.dat");
	_LIT(KWapReassemblyStoreName,"C:\\Private\\101F7989\\sms\\wapreast.dat");

	iFs.Delete(KWapReassemblyStoreName);
	iFs.Delete(KReassemblyStoreName);
	iFs.Delete(KSegmentationStoreName);

	SetTestNumberL();
	OpenSocketServerL();
	SetCodingScheme();
	GetNumberOfPDUs();
	
	return TestStepResult() ;
}

TVerdict CTestStatusReportScheme::doTestStepPostambleL()
{
	iSocket.Close();
	iSocketServer.Close();
	
	//Call base class postamble
	CSmsPrtTestStep::doTestStepPostambleL();
	
	return TestStepResult() ;
}

void CTestStatusReportScheme::SetTestNumberL()
{
	TInt testNumber;
	GetIntFromConfig(ConfigSection(), _L("testNumber"), testNumber);

	RProperty testNumberProperty;
	User::LeaveIfError(testNumberProperty.Attach(KUidPSSimTsyCategory, KPSSimTsyTestNumber));
	CleanupClosePushL(testNumberProperty);

	TRequestStatus status;
	testNumberProperty.Subscribe(status);
	INFO_PRINTF1(_L("Setting Sim.Tsy test number P&S property"));
	User::LeaveIfError(testNumberProperty.Set(KUidPSSimTsyCategory,KPSSimTsyTestNumber,testNumber));
	User::WaitForRequest(status);
	TEST(status.Int() == KErrNone);
	TInt testNumberCheck;
	User::LeaveIfError(testNumberProperty.Get(testNumberCheck));
	if (testNumber != testNumberCheck)
		{
	    INFO_PRINTF3(_L("Test number property set to [%d], but value returned is [%d]"),testNumber,testNumberCheck);
		User::Leave(KErrNotFound);
		}

	CleanupStack::PopAndDestroy(&testNumberProperty);
}

void CTestStatusReportScheme::OpenSocketServerL()
	{
	INFO_PRINTF1(_L("Connecting to SocketServer ..."));
	TInt ret=iSocketServer.Connect(KSocketMessageSlots);
	TEST(ret == KErrNone);

	iSmsStackTestUtils->OpenSmsSocketLC(iSocketServer,iSocket,ESmsAddrRecvAny);
	CleanupStack::Pop(); // Pop off iSocket which is a member variable!
	}


void CTestStatusReportScheme::CreateSmsL(TCodingScheme aCodingScheme ,TPtrC aConfigSectionEntry)
	{
	TSmsDataCodingScheme::TSmsAlphabet alphabet;

	if (aCodingScheme == ESevenBit)
		{	
		//8 bit coding scheme
		alphabet = TSmsDataCodingScheme::ESmsAlphabet7Bit;
		}

	else 
		{
		alphabet = TSmsDataCodingScheme::ESmsAlphabet8Bit;
		}		
	
	TPtrC messageText;
	GetStringFromConfig(ConfigSection(), aConfigSectionEntry, messageText);

	iSmsMessage=CreateSmsMessageL(messageText,alphabet);
	} // CTestStatusReportScheme::CreateSmsL


void CTestStatusReportScheme::TriggerReceiveL()
{
	CreateSmsL(EEightBit,_L("trigger"));
	SendSmsMessageL();
}

void CTestStatusReportScheme::SendSmsMessageL()
{
	SendSmsL(iSmsMessage,iSocket);
}

void CTestStatusReportScheme::SetCodingScheme()
{
	TInt codingScheme;
	GetIntFromConfig(ConfigSection(), _L("messageCoding"), codingScheme);
	iDataCodingScheme = (TCodingScheme)codingScheme;	

}

void CTestStatusReportScheme::GetNumberOfPDUs()
{
	GetIntFromConfig(ConfigSection(), _L("numberOfPDUs"), iNumberOfPDUs);
}

/**
Sets the TP-SRR bit in every PDU
*/
void CTestStatusReportScheme::SetAllTPSRRsL()
{	
	CSmsTPSRROperations& TPSRROperations = static_cast<CSmsTPSRROperations&>(iSmsMessage->GetOperationsForNonIEL(ESmsTPSRRParameter));
	TPSRROperations.SetSchemeL();

	//Set TPSRR for all PDUs
	for(TInt i=0; i< iNumberOfPDUs; i++)
	{
		TPSRROperations.SetSegmentStatusReportL(i,ETrue);
	}
	
}

/**
Set TPSRR bit for the last segment of message
*/
void CTestStatusReportScheme::SetLastTPSRRL()
{
	CSmsTPSRROperations& TPSRROperations = static_cast<CSmsTPSRROperations&>(iSmsMessage->GetOperationsForNonIEL(ESmsTPSRRParameter));
	TPSRROperations.SetSchemeL();
	TPSRROperations.SetLastSegmentStatusReportL(ETrue);	
}

/**
Set TPSRR bit for some segments
*/
void CTestStatusReportScheme::SetSomeTPSRRL(TInt aFirstIndex, TInt aSecondIndex, TInt aThirdIndex)
{
	CSmsTPSRROperations& TPSRROperations = static_cast<CSmsTPSRROperations&>(iSmsMessage->GetOperationsForNonIEL(ESmsTPSRRParameter));
	TPSRROperations.SetSchemeL();
	
	if(aFirstIndex)
	{
		TPSRROperations.SetSegmentStatusReportL(0,ETrue);		
	}
	else
	{
		TPSRROperations.SetSegmentStatusReportL(0,EFalse);	
	}
		
	if(aSecondIndex)
	{
		TPSRROperations.SetSegmentStatusReportL(1,ETrue);	
	}
	else
	{
		TPSRROperations.SetSegmentStatusReportL(1,EFalse);	
	}
	if(aThirdIndex)
	{
		TPSRROperations.SetSegmentStatusReportL(2,ETrue);	
	}
	else
	{
		TPSRROperations.SetSegmentStatusReportL(2,EFalse);	
	}
	
}

/**
Set the last segment SMSC value to 0x01
*/
void CTestStatusReportScheme::SetLastSMSCL()
{
	CSmsSMSCCtrlParameterOperations& SMSCOperations = static_cast<CSmsSMSCCtrlParameterOperations&>(iSmsMessage->GetOperationsForIEL(CSmsInformationElement::ESmsIEISMSCControlParameters));
	SMSCOperations.SetSchemeL();
	SMSCOperations.SetStatusReportL(iNumberOfPDUs-1, 0x01);	
}

/**
Set all segments SMSC value to 0x01
*/
void CTestStatusReportScheme::SetAllSMSCL()
{
	CSmsSMSCCtrlParameterOperations& SMSCOperations = static_cast<CSmsSMSCCtrlParameterOperations&>(iSmsMessage->GetOperationsForIEL(CSmsInformationElement::ESmsIEISMSCControlParameters));
	SMSCOperations.SetSchemeL();
	
	for(TInt i = 0; i < iNumberOfPDUs; i++)
	{
		SMSCOperations.SetStatusReportL(i, 0x01);		
	}
	
}


/**
Set SMSC bit for some segments
*/
void CTestStatusReportScheme::SetSomeSMSCL(TInt aFirstIndex, TInt aSecondIndex, TInt aThirdIndex)
{
	CSmsSMSCCtrlParameterOperations& SMSCOperations = static_cast<CSmsSMSCCtrlParameterOperations&>(iSmsMessage->GetOperationsForIEL(CSmsInformationElement::ESmsIEISMSCControlParameters));
	SMSCOperations.SetSchemeL();
	
	if(aFirstIndex)
	{
		SMSCOperations.SetStatusReportL(0,0x01);		
	}
	else
	{
		SMSCOperations.SetStatusReportL(0,0x00);	
	}
		
	if(aSecondIndex)
	{
		SMSCOperations.SetStatusReportL(1,0x01);	
	}
	else
	{
		SMSCOperations.SetStatusReportL(1,0x00);	
	}
	if(aThirdIndex)
	{
		SMSCOperations.SetStatusReportL(2,0x01);	
	}
	else
	{
		SMSCOperations.SetStatusReportL(2,0x00);	
	}
	
}

