// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <commsdattypesv1_1.h>
#include "metadatabase.h"

#include "TE_SMSEMSPRTBASE.h"
#include <sacls.h>

#if defined (__WINS__)
#define PDD_NAME _L("ECDRV")
#define PDD_NAME2 _L("ECDRV")
#define LDD_NAME _L("ECOMM")
#else
#define PDD_NAME _L("EUART1")
#define LDD_NAME _L("ECOMM")
#endif

using namespace CommsDat;

CSmsMessage* CSmsEmsPrtTestStep::ConfigCreateSmsMessageLC(const TDesC& aDes,
				TSmsDataCodingScheme::TSmsAlphabet aAlphabet)
	{
	_LIT(KRadiolinjaSC,"+358508771010");	// Radiolinja SC
	_LIT(KPekka,"+358408415528");			// Pekka's telephone number

	// Set destination and SC numbers
	iTelephoneNumber=KPekka;
	iServiceCenterNumber=KRadiolinjaSC;

	CSmsMessage* smsMessage = CreateSmsMessageL(aDes, aAlphabet);
	CleanupStack::PushL(smsMessage);
	return smsMessage;
	}

CSmsMessage* CSmsEmsPrtTestStep::CreateSmsMessageL(const TDesC& aDes, TSmsDataCodingScheme::TSmsAlphabet aAlphabet, CSmsPDU::TSmsPDUType aType)
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

TBool CSmsEmsPrtTestStep::SendReceiveMsgL(CSmsMessage& aMsg)
	{
	// Send SMS
	SendSmsL(&aMsg,iSocket);

	CSmsMessage* rxMsg;

	// Now receive and compare
	INFO_PRINTF1(_L("waiting for incoming SMS...") );
	WaitForRecvL(iSocket);
	rxMsg = RecvSmsL(iSocket);

	CleanupStack::PushL(rxMsg);
	TBool comp = EmsTestUtils::CompareEmsMsgL(aMsg, *rxMsg);
	TEST(comp);

	// compare OK
	INFO_PRINTF1(_L("Sent and received PDUs compare OK..."));

	// Now make sure the received message is valid - this can be
	// tested by ensuring that NumMessagePDUsL can be called without
	// leaving
	rxMsg->NumMessagePDUsL();

	CleanupStack::PopAndDestroy(rxMsg);

	// return the result of the comparison
	return comp;
	}

void CSmsEmsPrtTestStep::SendSmsL(const CSmsMessage* aSms, RSocket& aSocket)
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
		INFO_PRINTF2(_L("SendSmsL - sendSmsMessage returned %d"),status.Int());
		if (status.Int() )
			{
			tryAgain = ETrue;
			INFO_PRINTF1(_L("Try again... "));
			sendTry++;
			}
		else tryAgain = EFalse;
		}

	TEST(status.Int() == KErrNone);

    PrintMessageL(aSms);
	}

void CSmsEmsPrtTestStep::PrintMessageL(const CSmsMessage* aSms)
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
        ptr[j]=IsCharDisplayable((TText8)ptr[j]);


    INFO_PRINTF1(ptr);
	INFO_PRINTF1(_L(""));

	delete hbuf;
	}

TText8 CSmsEmsPrtTestStep::IsCharDisplayable( const TText8 aChar )
	{
	if(( aChar >= 0x20) && (aChar <= 0x80))
		return aChar;
	else
		return '.';
	}



void CSmsEmsPrtTestStep::WaitForRecvL(RSocket& aSocket)
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

CSmsMessage* CSmsEmsPrtTestStep::RecvSmsL(RSocket& aSocket, TInt aIoctl)
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
	TEST(ret == KErrNone);

	TPckgBuf<TUint> sbuf;
	TRequestStatus status;
	aSocket.Ioctl(aIoctl, status, &sbuf, KSolSmsProv);
	User::WaitForRequest(status);

	CleanupStack::Pop(smsMessage);
	return smsMessage;
	}

void CSmsEmsPrtTestStep::PrepareRegTestL()
/**
 *  Run a specified test.
 *  The test number is passed via property KUidPSSimTsyCategory. This will notify the SIM tsy
 *  SIM tsy uses test number to parse correct script from config.txt
 *  @param aTestNumber The test number corresponding the test case
 */
	{
	RProperty testNumberProperty;
	User::LeaveIfError(testNumberProperty.Attach(KUidPSSimTsyCategory, KPSSimTsyTestNumber));
	CleanupClosePushL(testNumberProperty);

	TRequestStatus status;
	testNumberProperty.Subscribe(status);
	TInt testNumber = GetTestNumber();
	User::LeaveIfError(testNumberProperty.Set(KUidPSSimTsyCategory,KPSSimTsyTestNumber,testNumber));
	User::WaitForRequest(status);
	TEST(status.Int() == KErrNone);
	TInt testNumberCheck;
	User::LeaveIfError(testNumberProperty.Get(testNumberCheck));
	if (testNumber != testNumberCheck)
		User::Leave(KErrNotFound);

	CleanupStack::PopAndDestroy(&testNumberProperty);

    INFO_PRINTF1(_L("Connecting to SocketServer ..."));
	TInt ret=iSocketServ.Connect(KSocketMessageSlots);
    TEST(ret == KErrNone);

	INFO_PRINTF1(_L("Deleting segmentation and reassembly stores..."));

	// delete segmentation and reassembly store files before the test
	_LIT(KReassemblyStoreName,"C:\\Private\\101F7989\\sms\\smsreast.dat");
	_LIT(KSegmentationStoreName,"C:\\Private\\101F7989\\sms\\smssegst.dat");

	iFs.Delete(KReassemblyStoreName);
	iFs.Delete(KSegmentationStoreName);
	}

void CSmsEmsPrtTestStep::EndRegTest()
/**
 *  Closes objects used at test harness
 */
 	{
	INFO_PRINTF1(_L("Closing Reg Test"));
	User::After(2000000); //Wait a couple of seconds to avoid closing the SMS stack too early
	iSocketServ.Close();
	User::After(2000000); //Wait a couple of seconds to avoid closing the SMS stack too early
	}

TVerdict CSmsEmsPrtTestStep::doTestStepPreambleL()
	{
	__UHEAP_MARK;

	User::LeaveIfError(iFs.Connect());

    TRAPD(ret, ParseSettingsFromFileL());
	if (ret != KErrNone)
		INFO_PRINTF2(_L("ParseSettingsFromFileL [err=%d]"), ret);

	iSmsStackTestUtils = CSmsStackTestUtils::NewL(this, iFs);

	PrepareRegTestL() ;

	iSmsStackTestUtils->OpenSmsSocketL(iSocketServ, iSocket, ESmsAddrRecvAny);

#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_2);
#else
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
#endif
    CleanupStack::PushL(db);

    CMDBField<TUint32>* smsReceiveModeField = new(ELeave) CMDBField<TUint32>(KCDTIdSMSReceiveMode);
    CleanupStack::PushL(smsReceiveModeField);
	*smsReceiveModeField = RMobileSmsMessaging::EReceiveUnstoredClientAck;
	smsReceiveModeField->SetRecordId(1); //it's GlobalSettingsRecord
	smsReceiveModeField->ModifyL(*db);

	CleanupStack::PopAndDestroy(smsReceiveModeField);
	CleanupStack::PopAndDestroy(db);

	return TestStepResult();
	}

TVerdict CSmsEmsPrtTestStep::doTestStepPostambleL()
	{
	iSocket.Close();

	EndRegTest() ;

	delete iSmsStackTestUtils;
	iSmsStackTestUtils = NULL;

	iFs.Close();

	__UHEAP_MARKEND;

 	return TestStepResult();
	}

TInt CSmsEmsPrtTestStep::CommInit()
    {
    RFs fs;
    TInt err=fs.Connect();  // make sure the FileServer is alive (only needed for WINS test code)
    fs.Close();

    INFO_PRINTF1(_L("CommInit: Loading PDD ") );
    INFO_PRINTF1(PDD_NAME);
    INFO_PRINTF1(_L(""));
    err=User::LoadPhysicalDevice(PDD_NAME);
    if (err!=KErrNone && err!=KErrAlreadyExists)
        return(err);

    INFO_PRINTF1(_L("CommInit: Loading LDD ") );
    INFO_PRINTF1(LDD_NAME);
    INFO_PRINTF1(_L(""));
    err=User::LoadLogicalDevice(LDD_NAME );
    if (err!=KErrNone && err!=KErrAlreadyExists)
        return(err);

    INFO_PRINTF1(_L("CommInit: Starting C32 ") );

    err = StartC32();
    return (err == KErrNone || err == KErrAlreadyExists) ? KErrNone : err;
    }

 void CSmsEmsPrtTestStep::CreateCommDBL()
/**
 *  @test Create a special CommDb Database for this test harness.
 */
    {
    INFO_PRINTF1(_L("Creating of the commdb for the TE_SmsEmsPrt") );

    
#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_2);
#else
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
#endif
    CleanupStack::PushL(db);
    db->OpenTransactionL();
    CMDBRecordSet<CCDModemBearerRecord>* modemBearerRecordSet = new(ELeave) CMDBRecordSet<CCDModemBearerRecord>(KCDTIdModemBearerRecord);
    CleanupStack::PushL(modemBearerRecordSet);
 	modemBearerRecordSet->LoadL(*db);

	_LIT(KDummy0, "DUMMY::0");
	_LIT(KDummy, "DUMMY");
	_LIT(KSim, "SIM");

    RPointerArray<CCDModemBearerRecord>& resultSet = (RPointerArray<CCDModemBearerRecord>&) modemBearerRecordSet->iRecords;

    CCDModemBearerRecord *modemRecord = static_cast<CCDModemBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
    CleanupStack::PushL(modemRecord);

    for (TInt i=0; i<resultSet.Count(); i++)
    	{
    		modemRecord = resultSet[i];
    	    modemRecord->iPortName = KDummy0;
       	    modemRecord->iCsyName = KDummy;
       	    modemRecord->iTsyName = KSim;
    		modemRecord->iMessageCentreNumber = iServiceCenterNumber;
			modemRecord->ModifyL(*db);
			modemRecord = NULL;
    	}
    db->CommitTransactionL();
    resultSet.Close();
    CleanupStack::PopAndDestroy(2);
    CleanupStack::PopAndDestroy(db);
    }


// Here are the section names
_LIT8(KSetupTelNumbers,           "Defaults");

// Here are the item names
_LIT8(KServiceCenter,               "ServiceCenter");
_LIT8(KTelefoneNumber,              "TelephoneNumber");

void CSmsEmsPrtTestStep::ParseSettingsFromFileL()
	{
	CTestConfig* configFile = CTestConfig::NewLC(iFs,KGmsSmsConfigFileDir,KGmsSmsConfigFileName);
	const CTestConfigSection* cfgFile = configFile->Section(KSetupTelNumbers);
	if (cfgFile == NULL)
		User::Leave(KErrNotFound);

	const CTestConfigItem* item = cfgFile->Item(KServiceCenter,0);
	if (item == NULL)
		User::Leave(KErrNotFound);

	iServiceCenterNumber.Copy(item->Value());

	item = cfgFile->Item(KTelefoneNumber,0);
	if (item == NULL)
		User::Leave(KErrNotFound);

	iTelephoneNumber.Copy(item->Value());

	// beginning of the destruction
	CleanupStack::PopAndDestroy(configFile);//configFile
	}
