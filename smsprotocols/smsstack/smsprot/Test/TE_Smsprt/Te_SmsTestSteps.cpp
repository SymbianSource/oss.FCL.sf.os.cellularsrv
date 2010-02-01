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
// This file contains utility functions used by the SMS test steps.  The actual SMS test steps are 
// declared in Te_SMSTestSteps.h and are defined in their own file with appropriate name.
// These utilities proved quite extensive functionality.  If a SMS test step you need is not 
// available, before creating a new one, check through these functions;  You might find something
// you can simply plug into your new test step.
// 
//

/**
 @file
*/

#include "Te_SmsTestSteps.h"
#include "TE_smsprtbase.h"
#include <sacls.h>
#include <smspver.h>


#if defined (__WINS__)
#define PDD_NAME _L("ECDRV")
#define PDD_NAME2 _L("ECDRV")
#define LDD_NAME _L("ECOMM")
#else
#define PDD_NAME _L("EUART1")
#define LDD_NAME _L("ECOMM")
#endif

// Here are the section names
_LIT8(KSetupTelNumbers,           "Defaults");

// Here are the item names
_LIT8(KServiceCenter,               "ServiceCenter");
_LIT8(KTelefoneNumber,              "TelephoneNumber");

// Location of directory for reserving disk space
_LIT(KTempDiskSpaceDirName, "C:\\sms\\temp\\");

// Location and name of the RSC file.
_LIT(KSMSUResourceDir, "C:\\private\\101f7989\\sms\\");
_LIT(KSMSUResourceFile, "C:\\private\\101f7989\\sms\\smsu.rsc");



CSMSTestSteps::~CSMSTestSteps()
{
	iFs.Close();			
}


/**
	Creates the file server session and marks the heap
*/
TVerdict CSMSTestSteps::doTestStepPreambleL()
	{
	RDebug::Print(_L("%S"), &TestStepName());
	
	User::LeaveIfError(iFs.Connect());	
	
	__UHEAP_MARK;
			
	return TestStepResult();
	}

/**
	Closes file server session and unmarks the heap	
*/
TVerdict CSMSTestSteps::doTestStepPostambleL()
	{
		
	__UHEAP_MARKEND;
	
	iFs.Close();	

	return TestStepResult();
	}
	
void CSMSTestSteps::ParseSettingsFromFileL()
	{
	CTestConfig* configFile = CTestConfig::NewLC(iFs,KGmsSmsConfigFileDir,KGmsSmsConfigFileName);
	const CTestConfigSection* cfgFile = configFile->Section(KSetupTelNumbers);
	if (cfgFile == NULL)
		{
		User::Leave(KErrNotFound);
		}

	const CTestConfigItem* item = cfgFile->Item(KServiceCenter,0);
	if (item == NULL)
		{
		User::Leave(KErrNotFound);
		}

	iServiceCenterNumber.Copy(item->Value());

	item = cfgFile->Item(KTelefoneNumber,0);
	if (item == NULL)
		{
		User::Leave(KErrNotFound);
		}

	iTelephoneNumber.Copy(item->Value());

	// beginning of the destruction
	CleanupStack::PopAndDestroy(configFile);//configFile
	}

void CSMSTestSteps::SetSimTSYTestNumberL(TInt aTestNumber)
/**
	Set the SIM TSY Test number 
	@param aTestNumber is the test number in SIM TSY config file
*/
	{	
	INFO_PRINTF2(_L("Setting SIMTSY test number to %d"), aTestNumber);

	//
	// Set the property...
	//
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory,KPSSimTsyTestNumber,aTestNumber));

	//
	// Check the property is set...
	//
	TInt  testNumberCheck;

	User::LeaveIfError(RProperty::Get(KUidPSSimTsyCategory,KPSSimTsyTestNumber,testNumberCheck));
	if (aTestNumber != testNumberCheck)
		{
	    INFO_PRINTF3(_L("Test number property set to %d, but value returned is %d"),aTestNumber,testNumberCheck);
		User::Leave(KErrNotFound);
		}
	}
//-----------------------------------------------------------------------------
void CSMSTestSteps::WaitForInitializeL()
/**
 *  Initialize the phone for the tsy. IF this is not called, message sends will complete with KErrNotReady
 */
	{
#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_2);
#else
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
#endif
	CleanupStack::PushL(db);

	TName tsy;
	TUint32 modemId = 0;

	CMDBField<TUint32>* globalSettingsField = new(ELeave) CMDBField<TUint32>(KCDTIdModemPhoneServicesSMS);
	CleanupStack::PushL(globalSettingsField);
	globalSettingsField->SetRecordId(1);
	globalSettingsField->LoadL(*db);
	modemId = *globalSettingsField;
	CleanupStack::PopAndDestroy(globalSettingsField);

	CMDBField<TDesC>* tsyField = new(ELeave) CMDBField<TDesC>(KCDTIdTsyName);
	CleanupStack::PushL(tsyField);
	tsyField->SetRecordId(modemId);
	tsyField->SetMaxLengthL(KMaxTextLength);
	tsyField->LoadL(*db);
	tsy = *tsyField;
	CleanupStack::PopAndDestroy(tsyField);

	CleanupStack::PopAndDestroy(db);

	INFO_PRINTF2(_L("Using TSY \"%S\"Loading RTelServer..."), &tsy);

	RTelServer server;
	User::LeaveIfError(server.Connect());
	CleanupClosePushL(server);
	User::LeaveIfError(server.LoadPhoneModule(tsy));

	// Find the phone corresponding to this TSY and open a number of handles on it
	TInt numPhones;
	User::LeaveIfError(server.EnumeratePhones(numPhones));
	RPhone phone;
	TBool found=EFalse;

	while (numPhones--)
		{
		TName phoneTsy;
		User::LeaveIfError(server.GetTsyName(numPhones,phoneTsy));
		if (phoneTsy.CompareF(tsy)==KErrNone)
			{
			INFO_PRINTF1(_L("Found RPhone..."));
			found = ETrue;
			RTelServer::TPhoneInfo info;
			User::LeaveIfError(server.GetPhoneInfo(numPhones,info));
			User::LeaveIfError(phone.Open(server,info.iName));
			CleanupClosePushL(phone);
			INFO_PRINTF1(_L("Initializing..."));
			const TInt err = phone.Initialise();
			TTimeIntervalMicroSeconds32 InitPause=9000000;  //Required Pause to Allow SMSStack to Complete its Async Init
			User::After(InitPause);							//call to the TSY and finish its StartUp.
			INFO_PRINTF2(_L("Completed Initialize [err=%d]"), err);
			User::LeaveIfError(err);
			CleanupStack::PopAndDestroy(&phone);
			break;
			}
		}

	TEST(found);
	CleanupStack::PopAndDestroy(&server);
	}



void CSMSTestSteps::PrepareRegTestLC(RSocketServ& aSocketServer, TInt aTestNumber)
/**
 *  Run a specified test.
 *  The test number is passed via property KUidPSSimTsyCategory. This will notify the SIM tsy
 *  SIM tsy uses test number to parse correct script from config.txt
 *  @param aTestNumber The test number corresponding the test case
 */
	{
	//
	// Set the SIM.TSY test number...
	//
	RProperty testNumberProperty;
	User::LeaveIfError(testNumberProperty.Attach(KUidPSSimTsyCategory, KPSSimTsyTestNumber));
	CleanupClosePushL(testNumberProperty);

	TRequestStatus status;
	testNumberProperty.Subscribe(status);
	INFO_PRINTF2(_L("Setting Sim.Tsy test number to %d"), aTestNumber);
	User::LeaveIfError(testNumberProperty.Set(KUidPSSimTsyCategory,KPSSimTsyTestNumber,aTestNumber));
	User::WaitForRequest(status);
	TEST(status.Int() == KErrNone);

	TInt testNumberCheck;
	User::LeaveIfError(testNumberProperty.Get(testNumberCheck));
	if (aTestNumber != testNumberCheck)
		{
	    INFO_PRINTF3(_L("Test number property set to [%d], but value returned is [%d]"),aTestNumber,testNumberCheck);
		User::Leave(KErrNotFound);
		}

	CleanupStack::PopAndDestroy(&testNumberProperty);

	//
	// Connect to the Socket Server...
	//
	TInt  ret = aSocketServer.Connect(KSocketMessageSlots);
    TESTL(ret == KErrNone);
	CleanupClosePushL(aSocketServer);

	//
	// Delete segmentation and reassembly store files before the test...
	//
	_LIT(KReassemblyStoreName,"C:\\Private\\101F7989\\sms\\smsreast.dat");
	_LIT(KSegmentationStoreName,"C:\\Private\\101F7989\\sms\\smssegst.dat");
	_LIT(KWapReassemblyStoreName,"C:\\Private\\101F7989\\sms\\wapreast.dat");

	iFs.Delete(KWapReassemblyStoreName);
	iFs.Delete(KReassemblyStoreName);
	iFs.Delete(KSegmentationStoreName);

	}
	
CSmsMessage* CSMSTestSteps::CreateSmsMessageL(const TDesC& aDes, TSmsDataCodingScheme::TSmsAlphabet aAlphabet, CSmsPDU::TSmsPDUType aType)
/**
 *  Create a uninitialised SMS message
 *  @param aDes contains text that will be inserted to the pdu
 *  @param aAlphabet describes the alphabet of the pdu
 *  @return CSmsMessage* :Pointer to the created CSmsMessage object.
 */
	{
	CSmsBuffer* buffer=CSmsBuffer::NewL();
	CSmsMessage* smsMessage=CSmsMessage::NewL(iFs, aType, buffer);
	CleanupStack::PushL(smsMessage);

	TSmsUserDataSettings smsSettings;
	smsSettings.SetAlphabet(aAlphabet);
	smsSettings.SetTextCompressed(EFalse);
	smsMessage->SetUserDataSettingsL(smsSettings);

	smsMessage->SetToFromAddressL(iTelephoneNumber);
	smsMessage->SmsPDU().SetServiceCenterAddressL(iServiceCenterNumber);
	buffer->InsertL(0,aDes);
	CleanupStack::Pop(smsMessage);
	return smsMessage;
	}

CSmsMessage* CSMSTestSteps::CreateSmsWithStatusReportReqL(const TDesC& aDes, TSmsDataCodingScheme::TSmsAlphabet aAlphabet)
/**
 *  Create a uninitialised SMS message with Status Report request
 *  @param aDes contains text that will be inserted to the pdu
 *  @param aAlphabet describes the alphabet of the pdu
 *  @return CSmsMessage* :Pointer to the created CSmsMessage object.
 */
	{
	CSmsMessage* smsMessage=CreateSmsMessageL(aDes, aAlphabet);

	CleanupStack::PushL(smsMessage);

	//Set Status report request
	CSmsSubmit& submitPdu=(CSmsSubmit&)smsMessage->SmsPDU();
	submitPdu.SetStatusReportRequest(ETrue);

	CleanupStack::Pop(smsMessage);
	return smsMessage;
	}

CSmsMessage* CSMSTestSteps::RecvSmsL(RSocket& aSocket, TInt aIoctl)
/**
 *  Receive an Sms
 *  @param aSocket is used to stream the sms message from the socket server
 *  @return CSmsMessage* :Sms message from Sms stack
 *  @leave Leaves if streaming the message from the socket server doesn't succeed
 */
	{
	CSmsBuffer* buffer=CSmsBuffer::NewL();
	CSmsMessage* smsMessage=CSmsMessage::NewL(iFs, CSmsPDU::ESmsSubmit,buffer);
	CleanupStack::PushL(smsMessage);

	RSmsSocketReadStream readstream(aSocket);
	TRAPD(ret,readstream >> *smsMessage);
	
	INFO_PRINTF2(_L("Return from streaming message over socket %d"),ret );
	
	TPckgBuf<TUint> sbuf;
	TRequestStatus status;

	if(ret==KErrNone)
		{
		aSocket.Ioctl(aIoctl, status, &sbuf, KSolSmsProv);
		User::WaitForRequest(status);
		CleanupStack::Pop(smsMessage);	
		}
	//An error has occured, No message has been received, so return a NULL
	else
		{
		aSocket.Ioctl(KIoctlReadMessageFailed, status, &sbuf, KSolSmsProv);
		User::WaitForRequest(status);
		TEST(status.Int() == KErrNone);
		User::Leave(ret);
		}	
	return smsMessage;	
	}
	
	
	

CSmsMessage* CSMSTestSteps::RecvSmsFailedL(RSocket& aSocket)
/**
 *  Receive an Sms, first nack the receive several times before succeeding
 *  @param aSocket is used to stream the sms message from the socket server
 *  @return CSmsMessage* :Sms message from Sms stack
 *  @leave Leaves if streaming the message from the socket server doesn't succeed
 *  @leave Leaves if nack of receiving is completed with error code
 *  @leave Leaves if ack of receiving is completed with error code
 */
	{
	CSmsBuffer* buffer=CSmsBuffer::NewL();
	CSmsMessage* smsMessage=CSmsMessage::NewL(iFs, CSmsPDU::ESmsSubmit,buffer);
	CleanupStack::PushL(smsMessage);
	RSmsSocketReadStream readstream(aSocket);

	TPckgBuf<TUint> sbuf;
	TRequestStatus status;

	for(TInt i=0; i<10; i++)
		{
		TRAPD(ret,readstream >> *smsMessage);
		TEST(ret == KErrNone);
		aSocket.Ioctl(KIoctlReadMessageFailed, status, &sbuf, KSolSmsProv);
		User::WaitForRequest(status);
		TEST(status.Int() == KErrNone);
		}

	TRAPD(ret,readstream >> *smsMessage);
	TEST(ret == KErrNone);
	aSocket.Ioctl(KIoctlReadMessageSucceeded, status, NULL, KSolSmsProv);
	User::WaitForRequest(status);
	TEST(status.Int() == KErrNone);

	CleanupStack::Pop(smsMessage);
	return smsMessage;
	}

void CSMSTestSteps::SendSmsL(const CSmsMessage* aSms, RSocket& aSocket)
/**
 *  Stream aSms out to the socket server
 *  @param aSms contains the sms tpdu that will be streamed to the sms stack
 *  @param aSocket is used to stream the aSms to the sms stack
 *  @leave Leaves if streaming the message to the socket server doesn't succeed
 *  @leave Leaves if sending is completed with error code
 */
	{
	TBool tryAgain = ETrue;
	TInt sendTry (0);
	TRequestStatus status = KErrNone;

	while (tryAgain && sendTry < 3)
		{
		RSmsSocketWriteStream writestream(aSocket);
		TRAPD(ret,writestream << *aSms);

		TRAP(ret,writestream.CommitL());

		TPckgBuf<TUint> sbuf;
		aSocket.Ioctl(KIoctlSendSmsMessage,status,&sbuf, KSolSmsProv);
		User::WaitForRequest(status);
		INFO_PRINTF2(_L("SendSmsL - sendSmsMessage returned %d"), status.Int());
		if (status.Int() != KErrNone)
			{
			tryAgain = ETrue;
			INFO_PRINTF1(_L("Try again... "));
			sendTry++;
			}
		else tryAgain = EFalse;
		}

	TEST(status.Int() == KErrNone);
	PrintMessageL(aSms);
	if (status.Int() != KErrNone)
		{
		User::Leave(status.Int());
		}
	}

void CSMSTestSteps::SendSmsCancelL(CSmsMessage* aSms, RSocket& aSocket1, RSocket& aSocket2)
/**
 *  Stream Sms out to the socket server by two RSmsSocketWriteStream object.
 *  The first request is canceled and then the second request is completed with error code.
 *  @param aSms contains the sms tpdu that will be streamed to the sms stack
 *  @param aSocket1 The socket used with message that will be canceled
 *  @param aSocket2 The socket used with message that will be completed with error code
 *  @leave Leaves if streaming the message to the socket server doesn't succeed
 *  @leave Leaves if sending is completed with KErrNone
 */
	{
	RSmsSocketWriteStream writestream(aSocket1);
	TRAPD(ret,writestream << *aSms);
	TEST(ret == KErrNone);
	TRAP(ret,writestream.CommitL());
	TEST(ret == KErrNone);

	TPckgBuf<TUint> sbuf;
	TRequestStatus status1,status2;

	//stream to socket2
	RSmsSocketWriteStream writestream2(aSocket2);
	TRAP(ret,writestream2 << *aSms);
	TEST(ret == KErrNone);
	TRAP(ret,writestream2.CommitL());
	TEST(ret == KErrNone);

	aSocket1.Ioctl(KIoctlSendSmsMessage,status1,&sbuf, KSolSmsProv);
	aSocket2.Ioctl(KIoctlSendSmsMessage,status2,&sbuf, KSolSmsProv);

	User::After(2000000);

	// Test cancel first
	aSocket1.CancelIoctl();
	User::WaitForRequest(status1);
	TEST(status1.Int()==KErrCancel);

	User::After(50000);


	User::WaitForRequest(status2);

	INFO_PRINTF2(_L("SendSmsL - sendSmsMessage returned %d"),status2.Int());
	PrintMessageL(aSms);

	//Ensure the request is completed with error code ;)
	TEST(status2.Int() != KErrNone);
	INFO_PRINTF2(_L("Sending failed! %d"), status2.Int());
	}

TInt CSMSTestSteps::SendSmsErrorL(CSmsMessage* aSms, RSocket& aSocket)
/**
 *  Stream aSms out to the socket server. Sending is completed with error code.
 *  @param aSms contains the sms tpdu that will be streamed to the sms stack
 *  @param aSocket is used to stream the aSms to the sms stack
 *  @return error code
 *  @leave Leaves if streaming the message to the socket server doesn't succeed
 *  @leave Leaves if sending is completed with KErrNone
 */
	{
	RSmsSocketWriteStream writestream(aSocket);
	TRAPD(ret,writestream << *aSms);
	TEST(ret == KErrNone);
	TRAP(ret,writestream.CommitL());
	TEST(ret == KErrNone);

	TPckgBuf<TUint> sbuf;
	TRequestStatus status;

  	User::After(50000);
	// test cancel first
	aSocket.Ioctl(KIoctlSendSmsMessage,status,&sbuf, KSolSmsProv);
	aSocket.CancelIoctl();
	User::WaitForRequest(status);
	TEST(status.Int()==KErrCancel);

	//Now send again, completed with error
	TRAP(ret,writestream << *aSms);
	TEST(ret == KErrNone);
	TRAP(ret,writestream.CommitL());
	TEST(ret == KErrNone);

	aSocket.Ioctl(KIoctlSendSmsMessage,status,&sbuf, KSolSmsProv);
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("SendSmsL - sendSmsMessage returned %d"), status.Int());
	PrintMessageL(aSms);
	INFO_PRINTF2(_L("Sending failed! %d"), status.Int());
	TEST(status.Int() != KErrNone);
	return status.Int();
	}

void CSMSTestSteps::SendCommandSmsL(CSmsMessage* aSms, RSocket& aSocket)
/**
 *  Stream command message out to the socket server and wait for the return status
 *  @param aSms contains the sms tpdu that will be streamed to the sms stack
 *  @param aSocket is used to stream the aSms to the sms stack
 *  @leave Leaves if streaming the message to the socket server doesn't succeed
 *  @leave Leaves if sending is completed with error code
 */
	{
	RSmsSocketWriteStream writestream(aSocket);
	TRAPD(ret,writestream << *aSms);
	TEST(ret == KErrNone);
	TRAP(ret,writestream.CommitL());
	TEST(ret == KErrNone);

	TRequestStatus status;
	TPckgBuf<TUint> sbuf;
	aSocket.Ioctl(KIoctlSendSmsMessage,status,&sbuf,KSolSmsProv);

	User::WaitForRequest(status);

	INFO_PRINTF2(_L("SendCommandSmsL, sendSms returned %d"), status.Int());
	User::After(1000000);
	TEST(status.Int() == KErrNone);

	}

void CSMSTestSteps::SendAndRecvTestMessageL(const TTestCase& aTestCase, RSocket& aSocket)
/**
 *  Send a test message. This method is used to send and receive different DCS type messages
 *  @param aTestCase has information about the used test message, e.g. message data and DCS
 *  @leave Leaves if any of the leaving functions used at this function leaves
 */
	{
	SendTestMessageL(aTestCase, aSocket);

	WaitForRecvL(aSocket);
	CSmsMessage* smsMessage = RecvSmsL(aSocket);
	CleanupStack::PushL(smsMessage);
	TestMessageContentsL(smsMessage,aTestCase);
	CleanupStack::PopAndDestroy(smsMessage);
	}

void CSMSTestSteps::SendTestMessageL(const TTestCase& aTestCase, RSocket& aSocket)
/**
 *  Send a test message
 *  Assumes recv is already done.
 *  @param aTestCase has information about the used test message, e.g. message data and DCS
 *  @leave Leaves if any of the leaving functions used at this function leaves
 */
	{
	CSmsMessage* smsMessage = CreateSmsMessageL(aTestCase.iMsg, TSmsDataCodingScheme::ESmsAlphabet7Bit);
	CleanupStack::PushL(smsMessage);

	CSmsPDU& pdu = smsMessage->SmsPDU();
	CSmsUserData& userData = pdu.UserData();

	if (aTestCase.iMatchType == ESmsAddrMatchIEI)
		userData.AddInformationElementL(aTestCase.iIdentifierMatch,_L8("98"));

	if (aTestCase.iTestSmsClass)
		{
		pdu.SetBits7To4(TSmsDataCodingScheme::ESmsDCSTextUncompressedWithClassInfo);
		pdu.SetClass(ETrue,aTestCase.iSmsClass);
		}

	if (aTestCase.iTestValidityPeriod && pdu.Type()==CSmsPDU::ESmsSubmit)
		{
		CSmsSubmit* submitPdu = static_cast<CSmsSubmit*>(&pdu);
		submitPdu->SetValidityPeriod(aTestCase.iValidityPeriod);
		}

	if (aTestCase.iTestIndicators && pdu.Type()==CSmsPDU::ESmsSubmit)
		{
		SetIndicatorL(aTestCase, smsMessage);
		}

	SendSmsL(smsMessage, aSocket);
	CleanupStack::PopAndDestroy(smsMessage);
	}

void CSMSTestSteps::SendSmsDontCheckReturnValueL(CSmsMessage* aSms, RSocket& aSocket)
/**
 *  Stream aSms out to the socket server and don't check return value.
 *  @param aSms contains the sms tpdu that will be streamed to the sms stack
 *  @param aSocket is used to stream the aSms to the sms stack
 *  @leave Leaves if streaming the message to the socket server doesn't succeed
 *  @leave Leaves if sending is completed with KErrNone
 */
	{
	RSmsSocketWriteStream writestream(aSocket);
	writestream << *aSms;
	writestream.CommitL();

	TPckgBuf<TUint> sbuf;
	TRequestStatus status;
	aSocket.Ioctl(KIoctlSendSmsMessage,status,&sbuf, KSolSmsProv);
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("Send SMS message returned %d"), status.Int());
	if(status.Int() != KErrNone)
		{
		User::Leave(status.Int());
		}
	}

void CSMSTestSteps::SendAndRecvSms7BitL(const TDesC& aDes, RSocket& aSocket)
/**
 *  Send and receive one 7bit sms
 *  @param aDes contains the text to be send
 *  @leave Leaves if DoSendAndRecvSmsL leaves
 */
	{
	DoSendAndRecvSmsL(aDes,TSmsDataCodingScheme::ESmsAlphabet7Bit, aSocket);
	}

void CSMSTestSteps::DoSendAndRecvSmsL(const TDesC& aDes, TSmsDataCodingScheme::TSmsAlphabet aAlphabet, RSocket& aSocket)
/**
 *  Send and recv one sms,
 *  then check that the sent message corresponds with the received message
 *  @param aDes contains the text that will be inserted to the pdu at CreateSmsMessageL
 *  @param aAlphabet describes the alphabet of the pdu that will be created at CreateSmsMessageL
 *  @leave Leaves if any of the leaving functions used at this function leaves
 */
	{
	CSmsMessage* smsMessage=CreateSmsMessageL(aDes, aAlphabet);
	CleanupStack::PushL(smsMessage);
	SendSmsL(smsMessage, aSocket);
	CleanupStack::PopAndDestroy(smsMessage); //destroyed because created again in RecvSmsL

	INFO_PRINTF1(_L("waiting for incoming SMS...") );
	WaitForRecvL( aSocket);
	smsMessage = RecvSmsL( aSocket);

	INFO_PRINTF1(_L("incoming SMS") );

	CleanupStack::PushL(smsMessage);
	TestSmsContentsL(smsMessage,aDes);

	User::After(1000000);

	CleanupStack::PopAndDestroy(smsMessage);
	}

void CSMSTestSteps::PrintMessageL(const CSmsMessage* aSms)
/**
 *  Print the content of SMS to the console
 */
	{
	CSmsBuffer& smsbuffer = (CSmsBuffer&)aSms->Buffer();
	const TInt len = smsbuffer.Length();
	HBufC* hbuf = HBufC::NewL(len);
	TPtr ptr = hbuf->Des();
	smsbuffer.Extract(ptr,0,len);

	INFO_PRINTF1(_L("SMS contains..."));

	for (TInt j = 0; j < len; j++)
		{
		if (ptr[j] < 0x20  ||  ptr[j] > 0xFF)
			{
			ptr[j] = 0x007F;
			}
		}
   	INFO_PRINTF2(_L("%S"), &ptr);

	delete hbuf;
	}

void CSMSTestSteps::PrintSmsMessage(const CSmsMessage& aMessage)
	{
	TBuf<255> buf;
	aMessage.Buffer().Extract(buf, 0, Min(aMessage.Buffer().Length(), 255));
	INFO_PRINTF2(_L("%S"), &buf);
	}

TSmsStatus::TSmsStatusValue CSMSTestSteps::RecvStatusReportL(TSmsServiceCenterAddress& aRecipientNumber, RSocket& aSocket)
/**
 * Receive a Status report
 * @param aRecipientNumber The supposed recipient number
 * @param aSocket is used to stream the sms message from the socket server
 */
	{
	//Receive SMS
	INFO_PRINTF1(_L("waiting for incoming status report...") );
	WaitForRecvL(aSocket);
	CSmsMessage* smsMessage = RecvSmsL(aSocket);

	INFO_PRINTF1(_L("incoming SMS") );

	//Check the status report
	CleanupStack::PushL(smsMessage);
	TBool isSR = (smsMessage->Type()==CSmsPDU::ESmsStatusReport);

	if (isSR)
		{
		INFO_PRINTF1(_L("Received status report"));
		TSmsServiceCenterAddress telephoneNumber=smsMessage->ToFromAddress();
		TEST(telephoneNumber==aRecipientNumber);
		INFO_PRINTF2(_L("Message delivered to %S"), &telephoneNumber);
		}
	else
		{
		INFO_PRINTF1(_L("Received SMS is NOT a Status report!"));
		}

	TEST(isSR==1);
	
	//Get the status report
	CSmsStatusReport& statusReport = static_cast<CSmsStatusReport&>(smsMessage->SmsPDU()); 
	TSmsStatus::TSmsStatusValue status = statusReport.Status();
	
	CleanupStack::PopAndDestroy(smsMessage);
	
	return status;
	}

void CSMSTestSteps::WaitForRecvL(RSocket& aSocket)
/**
 *  Wait for an Sms to be received
 *  @param aSocket The status is return to this socket
 *  @leave Leaves if receiving is completed with error code
 */
	{
	TPckgBuf<TUint> sbuf;
	sbuf()=KSockSelectRead;
	TRequestStatus status;
	aSocket.Ioctl(KIOctlSelect,status,&sbuf,KSOLSocket);
	User::WaitForRequest(status);
	TEST(status.Int() == KErrNone);
	}

void CSMSTestSteps::TestSmsContentsL(CSmsMessage* aSms, const TDesC& aDes, TBool aIgnorePrintOutput)
/**
 *  Check that aSms contains text that matches to aDes
 *  @param aSms SMS message that has been come from SMS stack
 *  @param aDes SMS message's text that is defined at client side
 *  @leave Leaves if aSms doesn't match to aDes
 */
	{
	CSmsBufferBase& smsBuffer=aSms->Buffer();
	TInt bufLen=smsBuffer.Length();
	
	INFO_PRINTF2(_L("Length of buffer is : %d"),bufLen);
		
	HBufC* textBuf=HBufC::NewL(bufLen);
	CleanupStack::PushL(textBuf);
	TPtr textPtr(textBuf->Des());
	smsBuffer.Extract(textPtr,0,bufLen);

	INFO_PRINTF1(_L("Comparing messages..."));
	TInt  compareResult = textPtr.Compare(aDes);

	if (!aIgnorePrintOutput)
		{
		TInt  bufLen2 = aDes.Length();
		HBufC*  textBuf2 = HBufC::NewL(aDes.Length());
		CleanupStack::PushL(textBuf2);
		TPtr textPtr2(textBuf2->Des());
		TInt  index;
		
		for (index = 0;  index < bufLen;  index++)
			{
			if (textPtr[index] < 0x20  ||  textPtr[index] > 0xff)
				{
				textPtr[index] = 0x007f;
				}
			}
		INFO_PRINTF2(_L("%S"), &textPtr);

		INFO_PRINTF1(_L("with expected"));

		textPtr2.Copy(aDes);
		for (index = 0;  index < bufLen2;  index++)
			{
			if (textPtr2[index] < 0x20  ||  textPtr2[index] > 0xff)
				{
				textPtr2[index] = 0x007f;
				}
			}
		INFO_PRINTF2(_L("%S"), &textPtr2);

		CleanupStack::PopAndDestroy(textBuf2);
		}
		
	TEST(compareResult == 0);
	if (compareResult != 0)
		{
		INFO_PRINTF1(_L("No Match!"));	
		}
	else
		{
		INFO_PRINTF1(_L("Incoming matches outgoing!"));	
		}
	
	CleanupStack::PopAndDestroy(textBuf);
	}

void CSMSTestSteps::TestMessageContentsL(CSmsMessage* aSms, const TTestCase& aTestCase)
/**
 *  Check the sms matches the expected test result
 *  @param aSms has the message to be tested with aTestCase.iMsg
 *  @param aTestCase has information about the used test message, e.g. message data and DCS
 *  @leave Leaves if TSmsClass isn't defined at the pdu
 *  @leave Leaves if class of pdu doesn't match to supposed class (aTestCase.iSmsClass)
 */
	{
	TestSmsContentsL(aSms,aTestCase.iMsg);
	CSmsPDU& pdu = aSms->SmsPDU();

	if (aTestCase.iTestSmsClass)
		{
		TSmsDataCodingScheme::TSmsClass smsClass;
		TBool isDefined = pdu.Class(smsClass);
		TEST(isDefined);
		TEST(smsClass == aTestCase.iSmsClass);
		}
	if (aTestCase.iTestIndicators)
		{
		TEST(pdu.Bits7To4() == aTestCase.iBits7To4);
		TEST(pdu.IndicationType() == aTestCase.iIndicationType);
		TEST(pdu.IndicationState() == aTestCase.iIndicationState);
		}

	}

void CSMSTestSteps::WriteSmsToSimL(CSmsMessage& aSms, RSocket& aSocket)
/**
 *  This method stores SMS messages to the SMS storage.
 *  @param aSms SMS message that will be stored
 *  @param aSocket Used to stream SMS message to the sms stack
 *  @leave Leaves if streaming the message to the socket server doesn't succeed
 *  @leave Leaves if store request is completed with error code
 */
	{
	INFO_PRINTF1(_L("Write message"));

	TRequestStatus status;

	RSmsSocketWriteStream writestream(aSocket);
	TRAPD(ret,writestream << aSms);
	TEST(ret == KErrNone);
	TRAP(ret,writestream.CommitL());
	TEST(ret == KErrNone);

	aSocket.Ioctl(KIoctlWriteSmsMessage,status,NULL,KSolSmsProv);
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("WriteSmsToSimL - returned %d"), status.Int());
	}

void CSMSTestSteps::WriteSmsLeaveIfErrorL(const CSmsMessage& aSms, RSocket& aSocket)
/**
 *  This method stores SMS messages to the SMS storage.
 *  @param aSms SMS message that will be stored
 *  @param aSocket Used to stream SMS message to the sms stack
 */
	{
	TRequestStatus status;
	RSmsSocketWriteStream writestream(aSocket);
	writestream << aSms;
	writestream.CommitL();
	aSocket.Ioctl(KIoctlWriteSmsMessage,status,NULL, KSolSmsProv);
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("Write SMS message returned %d"), status.Int());
	if(status.Int() != KErrNone)
		{
		User::Leave(status.Int());
		}
	}

void CSMSTestSteps::ReadSmsStoreL(RSocket& aSocket, RPointerArray<CSmsMessage>& aMessages)
/**
 *  This method retrieves SMS messages from SMS storage and print them out.
 *  @param aSocket Used to stream SMS messages from the socket server
 *  @param aMessages Sms messages will be streamed to this array
 *  @leave Leaves if first request is NOT completed with KErrCancel
 *  @leave Leaves if second request is completed with error code
 *  @leave Leaves if streaming the message from the socket server doesn't succeed
 *  @leave Leaves if nack of reading is completed with error code
 *  @leave Leaves if ack of reading is completed with error code
 */
	{
	TRequestStatus status;
	ReadSmsStoreL(aSocket, aMessages, status);
	}

void CSMSTestSteps::ReadSmsStoreL(RSocket& aSocket, RPointerArray<CSmsMessage>& aMessages, TRequestStatus& aStatus)
/**
 *  This method retrieves SMS messages from SMS storage and print them out.
 *  @param aSocket Used to stream SMS messages from the socket server
 *  @param aMessages Sms messages will be streamed to this array
 *  @param aStatus Status of request to enumerate messages from store
 *  @leave Leaves if first request is NOT completed with KErrCancel
 *  @leave Leaves if second request is completed with error code
 *  @leave Leaves if streaming the message from the socket server doesn't succeed
 *  @leave Leaves if nack of reading is completed with error code
 *  @leave Leaves if ack of reading is completed with error code
 */
	{
	INFO_PRINTF1(_L("Enumerating messages"));

	TPckgBuf<TUint> sbuf;
	sbuf()=0;

	// test cancel first - due to Lubbock problems canceled out
	aSocket.Ioctl(KIoctlEnumerateSmsMessages,aStatus,&sbuf, KSolSmsProv);
	aSocket.CancelIoctl();
	User::WaitForRequest(aStatus);
	TEST(aStatus.Int()==KErrCancel);

	User::After(2000000);
	//Now enumerate messages from store
	aSocket.Ioctl(KIoctlEnumerateSmsMessages,aStatus,&sbuf, KSolSmsProv);
	User::WaitForRequest(aStatus);
	TEST(aStatus.Int() == KErrNone);
	User::After(2000000);
	
	//sbuf() includes the count of messages on Store
	TInt count = sbuf();
	INFO_PRINTF2(_L("%d enumerated messages"), count);

	RSmsSocketReadStream readstream(aSocket);

	//Read each message from the stream
	for(TInt i=0; i< count; i++)
		{
		CSmsBuffer* buffer=CSmsBuffer::NewL();
		CSmsMessage* smsmessage=CSmsMessage::NewL(iFs, CSmsPDU::ESmsDeliver,buffer);
		CleanupStack::PushL(smsmessage);

		TRAPD(ret,readstream >> *smsmessage);
		TEST(ret == KErrNone);
		aSocket.Ioctl(KIoctlReadMessageFailed, aStatus, NULL, KSolSmsProv);
		User::WaitForRequest(aStatus);
		TEST(aStatus.Int() == KErrNone);

		TRAP(ret,readstream >> *smsmessage);
		TEST(ret == KErrNone);
		aSocket.Ioctl(KIoctlReadMessageSucceeded, aStatus, NULL, KSolSmsProv);
		User::WaitForRequest(aStatus);
		TEST(aStatus.Int() == KErrNone);

		Logger().Write(smsmessage->ToFromAddress());
		INFO_PRINTF1(_L("     "));
		Logger().Write(smsmessage->ServiceCenterAddress());
		INFO_PRINTF1(_L(""));

		if(smsmessage->Storage() == CSmsMessage::ESmsSIMStorage)
			{
			INFO_PRINTF1(_L("Store: SIM"));
			}
		else if (smsmessage->Storage() == CSmsMessage::ESmsPhoneStorage)
			{
			INFO_PRINTF1(_L("Store: Phone"));
			}
		else if (smsmessage->Storage() == CSmsMessage::ESmsCombinedStorage)
			{
			INFO_PRINTF1(_L("Store: Combined"));
			}
		else
			{
			INFO_PRINTF1(_L("Store: Unknown"));
			}

		switch (smsmessage->Status())
			{
			case RMobileSmsStore::EStoredMessageUnread:
				INFO_PRINTF1(_L("Status: Unread"));
				break;

			case RMobileSmsStore::EStoredMessageRead:
				INFO_PRINTF1(_L("Status: Read"));
				break;

			case RMobileSmsStore::EStoredMessageUnsent:
				INFO_PRINTF1(_L("Status: Unsent"));
				break;

			case RMobileSmsStore::EStoredMessageSent:
				INFO_PRINTF1(_L("Status: Sent"));
				break;

			case RMobileSmsStore::EStoredMessageDelivered:
				INFO_PRINTF1(_L("Status: Delivered"));
				break;

			case RMobileSmsStore::EStoredMessageUnknownStatus:
			default:
				INFO_PRINTF1(_L("Status: Unknown"));
				break;
			}

        PrintMessageL(smsmessage);
		User::LeaveIfError(aMessages.Append(smsmessage));
		CleanupStack::Pop(smsmessage);
		}

	}


TInt CSMSTestSteps::DeleteSmsL(const CSmsMessage& aSms, RSocket& aSocket)
/**
 *  This method deletes SMS message from the SMS storage.
 *  @param aSms SMS message that will be deleted
 *  @param aSocket Used to stream SMS message to the sms stack
 *  @leave Leaves if delete request is completed with error code (except KErrNotFound)
 *  @return TInt :error code
 */
	{
	INFO_PRINTF1(_L("Delete message"));

	TRequestStatus status;

	RSmsSocketWriteStream writestream(aSocket);
	TRAPD(ret,writestream << aSms);
	TEST(ret == KErrNone);
	TRAP(ret,writestream.CommitL());
	TEST(ret == KErrNone);

	aSocket.Ioctl(KIoctlDeleteSmsMessage, status, NULL, KSolSmsProv);
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("DeleteSmsL - returned %d"), status.Int());

	if (status.Int()!=KErrArgument)
		{
		TEST(status.Int() == KErrNone);
		}

	return status.Int();
	}

void CSMSTestSteps::DeleteSmsLeaveIfErrorL(const CSmsMessage& aSms, RSocket& aSocket)
/**
 *  This method deletes SMS message from the SMS storage.
 *  @param aSms SMS message that will be deleted
 *  @param aSocket Used to stream SMS message to the sms stack
 */
	{
	TRequestStatus status;
	RSmsSocketWriteStream writestream(aSocket);
	writestream << aSms;
	writestream.CommitL();
	aSocket.Ioctl(KIoctlDeleteSmsMessage, status, NULL, KSolSmsProv);
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("Delete SMS returned %d"), status.Int());
	if(status.Int() != KErrNone)
		{
		User::Leave(status.Int());
		}
	}

void CSMSTestSteps::SetIndicatorL(const TTestCase& aTestCase, CSmsMessage* aSms)
/**
 *  
 */
	{
	TSmsDataCodingScheme::TSmsAlphabet alphabet;
	if (aTestCase.iBits7To4 == TSmsDataCodingScheme::ESmsDCSMessageWaitingIndicationUCS2)
		alphabet = TSmsDataCodingScheme::ESmsAlphabetUCS2;
	else
		alphabet = TSmsDataCodingScheme::ESmsAlphabet7Bit;

	TSmsUserDataSettings smsSettings;
	smsSettings.SetAlphabet(alphabet);
	smsSettings.SetTextCompressed(EFalse);
	aSms->SetUserDataSettingsL(smsSettings);

	CSmsPDU& pdu = aSms->SmsPDU();
	pdu.SetBits7To4(aTestCase.iBits7To4);
	pdu.SetIndicationType(aTestCase.iIndicationType);
	pdu.SetIndicationState(aTestCase.iIndicationState);

	}

void CSMSTestSteps::FillDes(TDes& aDes,TInt aLength)
/**
 *  Fill aDes with randomly generated 7bit data
 *  @param aDes will be filled with random data
 *  @param aLength has the length to be set to aDes
 */
	{
	TText baseChar='A';

	aDes.SetLength(aLength);
	for (TInt i=0; i<aLength; i++)
		{
		aDes[i]=TText(baseChar + i%26);
		}
	}

TInt CSMSTestSteps::MakeReadSmsStoreRequestL(RSocket& aSocket)
/**
 *  This method retrieves SMS messages from SMS storage.
 *  Main purpose is to test out of memory conditions.
 *  @param aSocket Used to stream SMS messages from the sms stack
 *  @param aMessages reference to CSmsMessage pointer array.
 */
	{
	TRequestStatus status;
	TPckgBuf<TUint> sbuf;
	sbuf()=0;

	//Now enumerate messages from SIM
	aSocket.Ioctl(KIoctlEnumerateSmsMessages,status,&sbuf, KSolSmsProv);
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("Read SMS returned %d"), status.Int());
	if(status.Int() != KErrNone)
		{
		User::Leave(status.Int());
		}

	//sbuf() includes the count of messages on SIM
	return sbuf();
	}

TBool CSMSTestSteps::TimedWaitForRecvL(RSocket& aSocket, TUint aDelay)
/**
 *  Wait for up to the delay for an Sms to be received
 *  @param aSocket The status is return to this socket
 *  @param aDelay Maximum time to wait for receipt
 *  @return ETrue if data received
 *  @leave Leaves if receiving is completed with error code
 */
	{
	TRequestStatus timerStatus;
	RTimer timer;
	timer.CreateLocal();
	timer.After(timerStatus, TTimeIntervalMicroSeconds32(aDelay));

	TPckgBuf<TUint> sbuf;
	sbuf()=KSockSelectRead;
	TRequestStatus ioctlStatus;
	aSocket.Ioctl(KIOctlSelect, ioctlStatus, &sbuf, KSOLSocket);
	User::WaitForRequest(timerStatus, ioctlStatus);
	TBool retval = EFalse;
	if(ioctlStatus == KRequestPending)
		{
		// if timer elapsed but ESock request is still pending
		aSocket.CancelIoctl();
		User::WaitForRequest(ioctlStatus);
		}
	else
		{
		// ESock request is done. Cancel timer
		timer.Cancel();
		User::WaitForRequest(timerStatus);
		// check ESock error status
		if(ioctlStatus.Int() == KErrNone)
			{
			retval=ETrue;             
			}
		}
	timer.Close();
	return retval;
	}

TInt CSMSTestSteps::CancelWriteSmsToSimL(CSmsMessage& aSms, RSocket& aSocket, TInt aDelay)
/**
 *  This method stores SMS messages to the SMS storage.
 *  @param aSms SMS message that will be stored
 *  @param aSocket Used to stream SMS message to the sms stack
 *  @leave Leaves if streaming the message to the socket server doesn't succeed
 *  @leave Leaves if store request is completed with error code
 */
	{
	INFO_PRINTF1(_L("Write message"));

	TRequestStatus status;

	RSmsSocketWriteStream writestream(aSocket);
	TRAPD(ret,writestream << aSms);
	TEST(ret == KErrNone);
	TRAP(ret,writestream.CommitL());
	TEST(ret == KErrNone);

	aSocket.Ioctl(KIoctlWriteSmsMessage,status,NULL, KSolSmsProv);
	INFO_PRINTF1(_L("Cancel"));
	User::After(aDelay);
	aSocket.CancelIoctl();
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("WriteSmsToSimL - returned %d"), status.Int());

	return status.Int();
	}

TBool CSMSTestSteps::DoSingleTestCaseL( const TDesC8& aSection, TInt aCount )
	{
	CTestConfig* configFile = CTestConfig::NewLC(iFs, KGmsSmsConfigFileDir, KConfigFilename);

	TBuf8<64> sectionName(aSection);
	sectionName.AppendNum(aCount);
	const CTestConfigSection* section = NULL;
	TBool found( ETrue );

	if ( ( section = configFile->Section( sectionName ) ) != NULL )
		{
		if ( aSection == KTestSendAndRecvMsgsWithDifferentTON )
			{
			TInt num( 1 );
			CArrayFixFlat<TInt>* alphabetArray = new ( ELeave ) CArrayFixFlat<TInt>( 1 );
			CleanupStack::PushL(alphabetArray);
			CArrayFixFlat<TInt>* typeOfNumberArray = new ( ELeave ) CArrayFixFlat<TInt>( 1 );
			CleanupStack::PushL(typeOfNumberArray);

			TBuf8<32> itemName( KAlphabet );
			itemName.AppendNum( num );

			TInt param  = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
			while ( param != KErrNotFound )
				{
				alphabetArray->AppendL(param);
				itemName = KAlphabet;
				itemName.AppendNum( ++num );
				param = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
				}
			num = 1;
			itemName = KTypeOfNumber;
			itemName.AppendNum( num );

			param  = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
			while ( param != KErrNotFound )
				{
				typeOfNumberArray->AppendL(param);
				itemName = KTypeOfNumber;
				itemName.AppendNum( ++num );
				param = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
				}

			RPointerArray<CSmsPduDbMessage> array;
			CleanupResetAndDestroyPushL(array);
			ReadPduL( sectionName, array );
			TestSendAndRecvMsgsWithDifferentTONL( array, alphabetArray, typeOfNumberArray );
			CleanupStack::PopAndDestroy(&array);
			CleanupStack::PopAndDestroy(typeOfNumberArray);
			CleanupStack::PopAndDestroy(alphabetArray);
			}
		else if ( aSection == KTestSendAndReceiveIndicatorMsgs )
			{
			TInt num( 1 );
			CArrayFixFlat<TInt>* indicationTypeArray = new ( ELeave ) CArrayFixFlat<TInt>( 1 );
			CleanupStack::PushL(indicationTypeArray);
			CArrayFixFlat<TInt>* dcsBits7To4Array = new ( ELeave ) CArrayFixFlat<TInt>( 1 );
			CleanupStack::PushL(dcsBits7To4Array);
			CArrayFixFlat<TInt>* indicationStateArray = new ( ELeave ) CArrayFixFlat<TInt>( 1 );
			CleanupStack::PushL(indicationStateArray);

			TBuf8<32> itemName( KIndicationType );
			itemName.AppendNum( num );

			TInt param  = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
			while ( param != KErrNotFound )
				{
				indicationTypeArray->AppendL(param);
				itemName = KIndicationType;
				itemName.AppendNum( ++num );
				param = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
				}
			num = 1;
			itemName = KDCSBits7To4;
			itemName.AppendNum( num );

			param  = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
			while ( param != KErrNotFound )
				{
				dcsBits7To4Array->AppendL(param);
				itemName = KDCSBits7To4;
				itemName.AppendNum( ++num );
				param = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
				}
			num = 1;
			itemName = KIndicationState;
			itemName.AppendNum( num );

			param  = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
			while ( param != KErrNotFound )
				{
				indicationStateArray->AppendL(param);
				itemName = KIndicationState;
				itemName.AppendNum( ++num );
				param = section->ItemValue( (TPtrC8)itemName, KErrNotFound );
				}

			RPointerArray<CSmsPduDbMessage> array;
			CleanupResetAndDestroyPushL(array);
			ReadPduL( sectionName, array );
			TestSendAndReceiveIndicatorMsgsL( array, indicationStateArray, dcsBits7To4Array, indicationTypeArray );
			CleanupStack::PopAndDestroy(&array);
			CleanupStack::PopAndDestroy(indicationStateArray);
			CleanupStack::PopAndDestroy(dcsBits7To4Array);
			CleanupStack::PopAndDestroy(indicationTypeArray);
			}
		else
			found = EFalse;
		}
	else
		found = EFalse;

    CleanupStack::PopAndDestroy(configFile);//configFile
	return found;
	}

void CSMSTestSteps::TestSendAndRecvMsgsWithDifferentTONL( const RPointerArray<CSmsPduDbMessage>& aArray,
												   const CArrayFixFlat<TInt>* aAlphabetArray,
												   const CArrayFixFlat<TInt>* aTypeOfNumberArray )
    {
    INFO_PRINTF1(_L("Send and receive messages with different type of number"));

	RSocketServ socketServer;
	PrepareRegTestLC(socketServer, 39);

	RSocket socket;
	iSmsStackTestUtils->OpenSmsSocketLC(socketServer,socket,ESmsAddrRecvAny);

	TSmsDataCodingScheme::TSmsAlphabet alphabet = ( TSmsDataCodingScheme::TSmsAlphabet )aAlphabetArray->At(0);

	TInt bufLen=aArray[0]->iSmsMessage->Buffer().Length();
	HBufC* textBuf=HBufC::NewL(bufLen);
	CleanupStack::PushL(textBuf);
	TPtr testText(textBuf->Des());
	aArray[0]->iSmsMessage->Buffer().Extract(testText,0,bufLen);

	CSmsMessage* sendMessage=CreateSmsMessageL(testText,alphabet);
	CleanupStack::PushL(sendMessage);
    CSmsSubmit& submitPdu=(CSmsSubmit&)sendMessage->SmsPDU();
	TGsmSmsTelNumber gsmSmsTelNumber;
	gsmSmsTelNumber.iTelNumber.Append(iTelephoneNumber);
	if(gsmSmsTelNumber.iTelNumber.Length() > 0 && gsmSmsTelNumber.iTelNumber[0] == '+')
	{
		// Remove "+" sign from telephony number
		gsmSmsTelNumber.iTelNumber.Delete(0,1);
		gsmSmsTelNumber.iTelNumber.Trim();
	}

	for ( TInt i = 0; i < aTypeOfNumberArray->Count(); i++ )
		{

		gsmSmsTelNumber.iTypeOfAddress.SetTON( ( TGsmSmsTypeOfNumber )aTypeOfNumberArray->At(i) );
		sendMessage->SetParsedToFromAddressL(gsmSmsTelNumber);

		if ( ( TGsmSmsTypeOfNumber )aTypeOfNumberArray->At(i) == EGsmSmsTONSubscriberNumber )
			{
			submitPdu.SetPIDType(TSmsProtocolIdentifier::ESmsPIDShortMessageType);
			submitPdu.SetShortMessageType(TSmsProtocolIdentifier::ESmsReturnCallMesage);
			}
		INFO_PRINTF2(_L("Send SMS message. Type of number is %d"), aTypeOfNumberArray->At(i) );
		SendSmsL(sendMessage,socket);

		INFO_PRINTF1(_L("Waiting for incoming SMS...") );
		WaitForRecvL(socket);
		CSmsMessage* recvMessage = RecvSmsL(socket);
		delete recvMessage;

		}

	CleanupStack::PopAndDestroy(sendMessage);
	CleanupStack::PopAndDestroy(textBuf);
    CleanupStack::PopAndDestroy(&socket);
    CleanupStack::PopAndDestroy(&socketServer);


	}

void CSMSTestSteps::TestSendAndReceiveIndicatorMsgsL( const RPointerArray<CSmsPduDbMessage>& aArray,
											   const CArrayFixFlat<TInt>* aIndicationStateArray,
											   const CArrayFixFlat<TInt>* aDcsBits7To4Array,
											   const CArrayFixFlat<TInt>* aIndicationTypeArray )
/**
 *  @test Send and receive different indicator messages
 *  TODO ESmsDCSMessageWaitingIndicationDiscardMessage test doesn't work with Nokia 6210 because phone
 *  doesn't route incoming message to the MM TSY.
 */
    {

    INFO_PRINTF1(_L("Send and receive different indicator messages"));

	RSocketServ socketServer;
	PrepareRegTestLC(socketServer, 40);

	RSocket socket;
	iSmsStackTestUtils->OpenSmsSocketLC(socketServer,socket,ESmsAddrRecvAny);

	for ( TInt i = 0; i < aArray.Count(); i++ )
		{

		TSmsDataCodingScheme::TSmsIndicationState indicationState =
			( TSmsDataCodingScheme::TSmsIndicationState )aIndicationStateArray->At(i);
		TSmsDataCodingScheme::TSmsDCSBits7To4 dcsBits7To4Array =
			( TSmsDataCodingScheme::TSmsDCSBits7To4 )aDcsBits7To4Array->At(i);
		TSmsDataCodingScheme::TSmsIndicationType indicationType =
			( TSmsDataCodingScheme::TSmsIndicationType )aIndicationTypeArray->At(i);

		TInt bufLen=aArray[i]->iSmsMessage->Buffer().Length();
		HBufC* textBuf=HBufC::NewL(bufLen);
		CleanupStack::PushL(textBuf);
		TPtr testText(textBuf->Des());
		aArray[i]->iSmsMessage->Buffer().Extract(testText,0,bufLen);

		TTestCase msg(testText, indicationType, dcsBits7To4Array, indicationState );

		SendAndRecvTestMessageL(msg,socket);

		CleanupStack::PopAndDestroy(textBuf);
		}

    CleanupStack::PopAndDestroy(&socket);
    CleanupStack::PopAndDestroy(&socketServer);

    }

void CSMSTestSteps::ReadPduL( TBuf8<64> aSectionName, RPointerArray<CSmsPduDbMessage>& aArray )
	{
	CSmsPduDatabase* db = CSmsPduDatabase::NewL(iFs, aSectionName, KConfigFilename, KConfigFileDir);
	CleanupStack::PushL(db);

	db->GetMessageL(aArray, CSmsPDU::ESmsSubmit);

	CleanupStack::PopAndDestroy(db);
	}


CSmsMessage* CSMSTestSteps::CreateSmsMessageLC(CSmsPDU::TSmsPDUType aType, CSmsBuffer* aBuffer, const TDesC& aAddress)
	{
	CSmsMessage* msg = CSmsMessage::NewL(iFs, aType, aBuffer);
	CleanupStack::PushL(msg);

	if (aAddress.Length() != 0)
		{
		msg->SetToFromAddressL(aAddress);
		}

	return msg;
	}


void CSMSTestSteps::WriteToSIML(RSocket& aSocket, const CSmsMessage& aMsg)
	{
	TRequestStatus status;

	RSmsSocketWriteStream writestream(aSocket);
	writestream << aMsg;
	writestream.CommitL();

	aSocket.Ioctl(KIoctlWriteSmsMessage,status,NULL, KSolSmsProv);
	User::WaitForRequest(status);
	INFO_PRINTF2(_L("Write Sms - returned %d"), status.Int());
	if (status.Int() != KErrNone)
		{
		INFO_PRINTF1(_L("Test has not written message to SIM successfully"));
		}
	TEST(status.Int() == KErrNone);
	}


/*
These functions were grabbed from SMSStackTestUtils.dll
*/
void CSMSTestSteps::OpenSmsSocketL(RSocketServ& aSocketServer, RSocket& aSocket, TSmsAddrFamily aFamily)
/**
 *  Initialise an RSocket for SMS, aSocket is NOT pushed to CleanupStack.
 *  @param aSocketServer reference to the socket server object
 *  @param aSocket The socket that will be opened
 *  @param aFamily The sms address family
 */
	{
	TSmsAddr smsaddr;
	smsaddr.SetSmsAddrFamily(aFamily);
	OpenSmsSocketL(aSocketServer, aSocket, smsaddr);
	}

void CSMSTestSteps::OpenSmsSocketL(RSocketServ& aSocketServer, RSocket& aSocket, TSmsAddr& aSmsAddr)
/*
 *  Initialise an RSocket for SMS, aSocket is NOT pushed to CleanupStack.
 *  @param aSocketServer reference to the socket server object
 *  @param aSocket The socket that will be opened
 *  @param aSmsAddr The sms address to bind to
 */
	{
	TInt ret=aSocket.Open(aSocketServer,KSMSAddrFamily,KSockDatagram,KSMSDatagramProtocol);
	TEST(ret==KErrNone);
	CleanupClosePushL(aSocket);
	ret=aSocket.Bind(aSmsAddr);
	TEST(ret==KErrNone);
	CleanupStack::Pop(&aSocket);
	}

/*
	Utility for creating a SMS message from test data (ini) file
*/
CSmsMessage* CSMSTestSteps::CreateSMSL()
{
	INFO_PRINTF1(_L("Creating SMS...") );
	
	TInt codingScheme;
	GetIntFromConfig(ConfigSection(), _L("messageCoding"), codingScheme);
	
	TCodingScheme dataCodingScheme = (TCodingScheme)codingScheme;	
	
	TSmsDataCodingScheme::TSmsAlphabet alphabet;
	if (dataCodingScheme == ESevenBit)
		{	
		//8 bit coding scheme
		alphabet = TSmsDataCodingScheme::ESmsAlphabet7Bit;
		}

	else 
		{
		alphabet = TSmsDataCodingScheme::ESmsAlphabet8Bit;
		}		
	
	TPtrC messageText;
	GetStringFromConfig(ConfigSection(), _L("message"), messageText);
	CSmsMessage *iSms = CreateSmsMessageL(messageText,alphabet);
	
	INFO_PRINTF1(_L("SMS created") );
	PrintMessageL(iSms);
	
	return iSms;

}

/**
 * Sets high and low limits in .RSC file and then reserves disk space to match requested levels.
 * 
 * Checks the current free space and then sets the high and low marks
 * to be aHighDrop MB and aLowDrop MB below the current free space 
 * level.
 * 
 * Then diskspace is reserved to aFreeDrop MB below the current free
 * space level.
 * 
 * If the current free space level is greater then aMax then the 
 * current free space level is set to aMax
 * 
 * If the current free space level is less than aLowDrop MB then this
 * method leaves with KErrArgument.
 * 
 * @param   aHighDrop   The number of MB below the current free space level for the high level mark (in the .RSC file)
 * @param   aLowDrop    The number of MB below the current free space level for the low level mark (in the .RSC file)  
 * @param   aFreeDrop   The number of MB below the current free space level to set the free space to ;
 *                      if aFreeDrop == 0, then SetFreeDiskSpaceL() is not called
 * @param   aMax        The maximum level for the high limit allowed
 * 
 * @return  The max current free space level used.
 * 
 * @leave   KErrArgument if the current free diskspace level is less than aLowDrop MB
 * @leave   KErrArgument if aMax is not greater than aLowDrop MB
 * @leave   KErrArgument if aHighDrop >= aLowDrop  
 */
TUint64 CSMSTestSteps::SetHighLowLimitsAndDiskSpaceLevelL(TUint aHighDrop, TUint aLowDrop, TUint aFreeDrop, TUint64 aMax/*=0x7fffffff*/)
    {
    INFO_PRINTF5(_L("Setting High-Low limits and Diskspace levels [aHighDrop=%u, aLowDrop=%u, aFreeDrop=%u, aMax=%ld]"), 
            aHighDrop, aLowDrop, aFreeDrop, aMax);
    
    __ASSERT_ALWAYS( (aMax > (aLowDrop*1024*1024)), User::Leave(KErrArgument));
    __ASSERT_ALWAYS( (aLowDrop > aHighDrop), User::Leave(KErrArgument));
    
    ReleaseDiskSpaceL();
   
    TVolumeInfo  volumeInfo;
    User::LeaveIfError(iFs.Volume(volumeInfo, EDriveC));
    INFO_PRINTF2(_L("  Drive C currently has %ld bytes free."), volumeInfo.iFree);
    
    TUint64 current = volumeInfo.iFree;
    if( current < (aLowDrop*1024*1024) )
        {
        INFO_PRINTF1(_L("  Drive C already has too little free space!"));
        User::Leave(KErrArgument);
        }
    if( current > aMax )
        {
        current = aMax;
        }
    TUint64 high = current - (aHighDrop*1024*1024);
    TUint64 low  = current - (aLowDrop*1024*1024);    
        
    SetLowHighLimitsInSmsuRscL(low, high);

    if( aFreeDrop > 0 )
        {       
        TUint64 free = current - (aFreeDrop*1024*1024);    
        SetFreeDiskSpaceL(free);       
        }
    
    return current;
    }

void CSMSTestSteps::SetFreeDiskSpaceFromDropLevelL(TUint aFreeDrop)
    {
    if( aFreeDrop == 0)
        {
        return;
        }
    
    TVolumeInfo  volumeInfo;
    User::LeaveIfError(iFs.Volume(volumeInfo, EDriveC));
    TUint64 current = volumeInfo.iFree;
    if( current > 0x7fffffff )
        {
        current = 0x7fffffff;
        }         
    TUint64 free = current - (aFreeDrop*1024*1024);
    SetFreeDiskSpaceL(free);   
    }

/**
 *  Reserves disk space so that a specified amount of free disk space is
 *  available.
 *
 *  @param aNewFreeValue  Amount of free space required.
 */
void CSMSTestSteps::SetFreeDiskSpaceL(TInt64 aNewFreeValue)
    {
    
#ifndef _DEBUG
    ERR_PRINTF1(_L("Unexpected call: CSMSTestSteps::SetFreeDiskSpaceL() is expected to be called only in DEBUG mode."));
    User::Leave(KErrNotSupported);
#else
 
    INFO_PRINTF2(_L("Setting Drive C free disk space to %ld bytes."), aNewFreeValue);    
    
    __ASSERT_DEBUG( (aNewFreeValue <= 0x7fffffff), User::Leave(KErrArgument));

    TInt err = RProperty::Set(KUidPSSMSStackCategory, KUidPSSMSStackFreeDiskSpaceKey, (TInt)aNewFreeValue);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("RProperty::Set() failure [err=%d]"), err);
        User::Leave(err);
        }
#endif           
    } // CSMSTestSteps::SetFreeDiskSpaceL

/**
 *  Release all reserved disk space.
 */
void CSMSTestSteps::ReleaseDiskSpaceL()
	{
	
#ifndef _DEBUG
    ERR_PRINTF1(_L("Unexpected call: CSMSTestSteps::ReleaseDiskSpaceL() is expected to be called only in DEBUG mode."));
    User::Leave(KErrNotSupported);
#else

    INFO_PRINTF1(_L("CSMSTestSteps::ReleaseDiskSpaceL()"));
    
    TVolumeInfo  volumeInfo;
    User::LeaveIfError(iFs.Volume(volumeInfo, EDriveC));
    TUint64 current = volumeInfo.iFree;
    if( current > 0x7fffffff )
        {
        current = 0x7fffffff;
        }         
    
    SetFreeDiskSpaceL(current);
#endif    
	} // CSMSTestSteps::ReleaseDiskSpaceL


 /**
  *  Set high and low limits in .RSC file. When the SMS Stack starts the limits
  *  will be loaded as if set by the licensee.
  *
  *  @param aLowLimit   Low limit value.
  *  @param aHighLimit  High limit value.
  *
  *  @note Only works in debug mode for security reasons.
  */ 
void CSMSTestSteps::SetLowHighLimitsInSmsuRscL(TInt64 aLowLimit, TInt64 aHighLimit)
	{
	INFO_PRINTF3(_L("Setting high and low .RSC limits to %ld and %ld."),
				 aHighLimit, aLowLimit);

	__ASSERT_ALWAYS(aLowLimit  < 0x7fffffff, User::Leave(KErrArgument));
	__ASSERT_ALWAYS(aHighLimit < 0x7fffffff, User::Leave(KErrArgument));
	__ASSERT_ALWAYS(aLowLimit  < aHighLimit, User::Leave(KErrArgument));

	//
	// Data for the SMSU resource file. The low limit is written at position
	// 20 and the high limit at position 24.
	//
	const TInt  smsuRscSize = 34;
	TChar  smsuRscData[smsuRscSize] =
				{0x6b, 0x4a, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x00,
   	 			 0x00, 0x00, 0x00, 0x00, 0x19, 0xfd, 0x48, 0xe8,
				 0x01, 0x04, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12,
				 0x87, 0x65, 0x43, 0x21, 0x14, 0x00, 0x18, 0x00, 
   		 		 0x1c, 0x00};
    						  
	smsuRscData[20] = (aLowLimit  >>  0) & 0xff;
	smsuRscData[21] = (aLowLimit  >>  8) & 0xff;
	smsuRscData[22] = (aLowLimit  >> 16) & 0xff;
	smsuRscData[23] = (aLowLimit  >> 24) & 0xff;
	smsuRscData[24] = (aHighLimit >>  0) & 0xff;
	smsuRscData[25] = (aHighLimit >>  8) & 0xff;
	smsuRscData[26] = (aHighLimit >> 16) & 0xff;
	smsuRscData[27] = (aHighLimit >> 24) & 0xff;

	TBuf8<smsuRscSize>  smsuRscBuffer;

	for (TInt index = 0;  index < smsuRscSize;  index++)
		{
		smsuRscBuffer.Append(smsuRscData[index]);
		}

	//
	// Ensure the target directory exists...
	//
	TInt  ret;
	
	ret = iFs.MkDir(KSMSUResourceDir);
	if (ret != KErrNone  &&  ret != KErrAlreadyExists)
		{
		User::Leave(ret);
		}

	//
	// Write the RSC file to the private C:\ directory...
	//
	RFile  file;

	User::LeaveIfError(file.Replace(iFs, KSMSUResourceFile, EFileWrite));
	CleanupClosePushL(file);
	User::LeaveIfError(file.Write(smsuRscSize, smsuRscBuffer));
	CleanupStack::PopAndDestroy(&file);
	} // CSMSTestSteps::SetLowHighLimitsInSmsuRscL


/**
 *  Removes the low and high limits and returns the SMSU.RSC to defauls.
 */
void CSMSTestSteps::RemoveLowHighLimitsFromSmsuRscL()
	{
	INFO_PRINTF1(_L("Removing .RSC limits."));

	//
	// Remove the RSC file...
	//
	TInt  ret;
	
	ret = iFs.Delete(KSMSUResourceFile);
	if (ret != KErrNone  &&  ret != KErrNotFound)
		{
		User::Leave(ret);
		}
	} // CSMSTestSteps::RemoveLowHighLimitsFromSmsuRscL
