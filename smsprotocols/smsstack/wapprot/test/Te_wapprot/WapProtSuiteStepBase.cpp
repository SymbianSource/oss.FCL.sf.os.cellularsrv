// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <sacls.h>
#include <smspver.h>
#include "WapProtSuiteStepBase.h"
#include "WapProtSuiteDefs.h"

/**
Utility for setting the test number used by SIM TSY
*/
void CWapProtSuiteStepBase::SetTestNumberL()
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


void CWapProtSuiteStepBase::WaitForRecvL(RSocket& aSocket)
/**
 * Wait for an Sms to be received
 * @param aSocket The status is return to this socket
 * @leave Leaves if receiving is completed with error code
 */
	{
	TPckgBuf<TUint> sbuf;
	sbuf()=KSockSelectRead;
	TRequestStatus status;
	aSocket.Ioctl(KIOctlSelect,status,&sbuf,KSOLSocket);
	User::WaitForRequest(status);
	TEST(status.Int() == KErrNone);
	}


CSmsMessage* CWapProtSuiteStepBase::RecvSmsL(RSocket& aSocket, TInt aIoctl)
/**
 * Receive an Sms
 * @param aSocket is used to stream the sms message from the socket server
 * @return CSmsMessage* :Sms message from Sms stack
 * @leave Leaves if streaming the message from the socket server doesn't succeed
 */
	{

	RFs lFs;	
	User::LeaveIfError(lFs.Connect());
	
	CSmsBuffer* buffer=CSmsBuffer::NewL();
	CSmsMessage* smsMessage=CSmsMessage::NewL(lFs, CSmsPDU::ESmsSubmit,buffer);
	CleanupStack::PushL(smsMessage);

	RSmsSocketReadStream readstream(aSocket);
	TRAPD(ret,readstream >> *smsMessage);
	TEST(ret == KErrNone);

	TPckgBuf<TUint> sbuf;
	TRequestStatus status;
	aSocket.Ioctl(aIoctl, status, &sbuf, KSolSmsProv);
	User::WaitForRequest(status);

	CleanupStack::Pop(smsMessage);
	
	lFs.Close();
	
	return smsMessage;
	}



/**
Utility for setting up the WAP address - Port number and service center address
*/
void CWapProtSuiteStepBase::SetWapAddrL()
{
	TInt port;

	//Get the port number from the ini file
	GetIntFromConfig(ConfigSection(),_L("port"), port);
	
	iWapAddr.SetWapPort(TWapPortNumber(port));

	//Get the service center number
	TPtrC telNumber;
	GetStringFromConfig(ConfigSection(),_L("telNumber"), telNumber);
	TBuf8<100> scNumber;
	scNumber.Copy(telNumber);
	TPtrC8 scAddr(scNumber);
	iWapAddr.SetWapAddress(scAddr);
	
	//	Bind
	User::LeaveIfError(iSocket.Bind(iWapAddr));
}


/**
Setup a socket for receiving status repots
*/
void CWapProtSuiteStepBase::SetupStatusReportSocketL()
{
	//	Open the socket for receiving status reports
    User::LeaveIfError(iStatusReportSocket.Open(iSocketServer,KSMSAddrFamily,KSockDatagram,KSMSDatagramProtocol));
	
	//Bind to the socket
	//TSmsAddr smsAddr;
	iSmsAddr.SetSmsAddrFamily(ESmsAddrStatusReport);
	TInt ret=iStatusReportSocket.Bind(iSmsAddr);
	INFO_PRINTF2(_L("Socket Bind Return Value : %d"),ret);
	TESTL(ret == KErrNone);
	
	//	Waiting for the phone to be initialised
	//WaitForInitializeL();
}


/**
Get the coding of the message
*/
void CWapProtSuiteStepBase::SetCodingSchemeL()
{
	TInt dataCoding;

	//Get the port number from the ini file
	GetIntFromConfig(ConfigSection(),_L("dataCoding"), dataCoding);
	
	if(dataCoding==8)
		iCodingScheme = EWapSms8BitDCS;
	else
		iCodingScheme = EWapSms7BitDCS;
	
	if(iCodingScheme==EWapSms8BitDCS)	
		User::LeaveIfError(iSocket.SetOpt(KWapSmsOptionNameDCS,KWapSmsOptionLevel,EWapSms8BitDCS));
	else
		User::LeaveIfError(iSocket.SetOpt(KWapSmsOptionNameDCS,KWapSmsOptionLevel,EWapSms7BitDCS));
}

void CWapProtSuiteStepBase::SetMessageTypeL()
{
	//Get the type of message
	TPtrC messageType;
	GetStringFromConfig(ConfigSection(),_L("type"), messageType);

	//Set the message type to WapDatagram
	if(messageType.Compare(_L("Datagram")) ==0)
		User::LeaveIfError(iSocket.SetOpt(KWapSmsOptionWapDatagram,KWapSmsOptionLevel));	
//	else
//		User::LeaveIfError(iSocket.SetOpt(KWapSmsOptionSmartMessage,KWapSmsOptionLevel));	
			
	//The creation of the message will set the type to SMART by default
}
	


TVerdict CWapProtSuiteStepBase::doTestStepPreambleL()
/**
 *  @return - TVerdict
 *  Implementation of CTestStep base class virtual
 *  Load serial drivers
 *  Do all initialisation common to derived classes in here.
 */
	{
	__UHEAP_MARK;
	
	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

	iSecureBackupEngine = CSBEClient::NewL();
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURNormal, ENoBackup);

	TInt err;
    err=User::LoadPhysicalDevice(PDD_NAME);
    TESTL(err == KErrNone  ||  err == KErrAlreadyExists);

    err=User::LoadLogicalDevice(LDD_NAME );
    TESTL(err == KErrNone  ||  err == KErrAlreadyExists);

    err = StartC32();
    if(err != KErrNone && err != KErrAlreadyExists)
        {
        ERR_PRINTF2(TRefByValue<const TDesC>(_L("Start Comms Process Status = %d")), err);
        SetTestStepResult(EFail);
        }

	INFO_PRINTF1(_L("Deleting segmentation and reassembly stores..."));

	RFs fileServer;
	User::LeaveIfError(fileServer.Connect());

	// delete segmentation and reassembly store files before the test
	_LIT(KReassemblyStoreName,"C:\\Private\\101F7989\\sms\\smsreast.dat");
	_LIT(KSegmentationStoreName,"C:\\Private\\101F7989\\sms\\smssegst.dat");
	_LIT(KWapReassemblyStoreName,"C:\\Private\\101F7989\\sms\\wapreast.dat");

	fileServer.Delete(KWapReassemblyStoreName);
	fileServer.Delete(KReassemblyStoreName);
	fileServer.Delete(KSegmentationStoreName);

	fileServer.Close();

	return TestStepResult();
	}

TVerdict CWapProtSuiteStepBase::doTestStepPostambleL()
	{
	delete iSecureBackupEngine;
	iSecureBackupEngine = NULL;

	delete iScheduler;
	iScheduler = NULL;

	__UHEAP_MARKEND;
	
	return TestStepResult();
	}

void CWapProtSuiteStepBase::WaitForInitializeL()
	{
	TName tsy(KTSY);
	RTelServer serverT;
	User::LeaveIfError(serverT.Connect());
	CleanupClosePushL(serverT);
	User::LeaveIfError(serverT.LoadPhoneModule(tsy));

	// Find the phone corresponding to this TSY and open a number of handles on it
	TInt numPhones;
	User::LeaveIfError(serverT.EnumeratePhones(numPhones));
	RPhone phone;

	while (numPhones--)
		{
		TName phoneTsy;
		User::LeaveIfError(serverT.GetTsyName(numPhones,phoneTsy));
		if (phoneTsy.CompareF(tsy)==KErrNone)
			{
			RTelServer::TPhoneInfo info;
			User::LeaveIfError(serverT.GetPhoneInfo(numPhones,info));
			User::LeaveIfError(phone.Open(serverT,info.iName));
			CleanupClosePushL(phone);
			const TInt err = phone.Initialise();
			User::LeaveIfError(err);
			CleanupStack::PopAndDestroy(&phone);
			break;
			}
		}

	CleanupStack::PopAndDestroy(&serverT);
	}



/**
  *  Set high and low limits in .RSC file. When the SMS Stack starts the limits
  *  will be loaded as if set by the licensee.
  *
  *  @param aLowLimit   Low limit value.
  *  @param aHighLimit  High limit value.
  *
  *  @note Only works in debug mode for security reasons.
  */ 
void CWapProtSuiteStepBase::SetLowHighLimitsInSmsuRscL(TInt64 aLowLimit, TInt64 aHighLimit)
	{
	INFO_PRINTF3(_L("Setting high and low .RSC limits to %ld and %ld."),
				 aHighLimit, aLowLimit);

	__ASSERT_ALWAYS(aLowLimit  < 0x7fffffff, User::Leave(KErrArgument));
	__ASSERT_ALWAYS(aHighLimit < 0x7fffffff, User::Leave(KErrArgument));
	__ASSERT_ALWAYS(aLowLimit  < aHighLimit, User::Leave(KErrArgument));

	RFs lFs;	
	User::LeaveIfError(lFs.Connect());

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
	
	ret = lFs.MkDir(KSMSUResourceDir);
	if (ret != KErrNone  &&  ret != KErrAlreadyExists)
		{
		User::Leave(ret);
		}

	//
	// Write the RSC file to the private C:\ directory...
	//
	RFile  file;

	User::LeaveIfError(file.Replace(lFs, KSMSUResourceFile, EFileWrite));
	CleanupClosePushL(file);
	User::LeaveIfError(file.Write(smsuRscSize, smsuRscBuffer));
	CleanupStack::PopAndDestroy(&file);
	
	lFs.Close();
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
TUint64 CWapProtSuiteStepBase::SetHighLowLimitsAndDiskSpaceLevelL(TUint aHighDrop, TUint aLowDrop, TUint aFreeDrop, TUint64 aMax/*=0x7fffffff*/)
    {
    INFO_PRINTF5(_L("Setting High-Low limits and Diskspace levels [aHighDrop=%u, aLowDrop=%u, aFreeDrop=%u, aMax=%ld]"), 
            aHighDrop, aLowDrop, aFreeDrop, aMax);
    
    __ASSERT_ALWAYS( (aMax > (aLowDrop*1024*1024)), User::Leave(KErrArgument));
    __ASSERT_ALWAYS( (aLowDrop > aHighDrop), User::Leave(KErrArgument));
    
    ReleaseDiskSpaceL();
   
    TVolumeInfo  volumeInfo;
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    User::LeaveIfError(fs.Volume(volumeInfo, EDriveC));
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
    
    CleanupStack::PopAndDestroy(&fs);   
    return current;
    }

void CWapProtSuiteStepBase::SetFreeDiskSpaceFromDropLevelL(TUint aFreeDrop)
    {
    if( aFreeDrop == 0)
        {
        return;
        }
    
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);    
    
    TVolumeInfo  volumeInfo;
    User::LeaveIfError(fs.Volume(volumeInfo, EDriveC));
    TUint64 current = volumeInfo.iFree;
    if( current > 0x7fffffff )
        {
        current = 0x7fffffff;
        }    
    TUint64 free = current - (aFreeDrop*1024*1024);
    SetFreeDiskSpaceL(free);
    CleanupStack::PopAndDestroy(&fs); 
    }
/**
 *  Reserves disk space so that a specified amount of free disk space is
 *  available.
 *
 *  @param aNewFreeValue  Amount of free space required.
 */
void CWapProtSuiteStepBase::SetFreeDiskSpaceL(TInt64 aNewFreeValue)
    {
    
#ifndef _DEBUG
    ERR_PRINTF1(_L("Unexpected call: CWapProtSuiteStepBase::SetFreeDiskSpaceL() is expected to be called only in DEBUG mode."));
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
void CWapProtSuiteStepBase::ReleaseDiskSpaceL()
	{
	
#ifndef _DEBUG
    ERR_PRINTF1(_L("Unexpected call: CWapProtSuiteStepBase::ReleaseDiskSpaceL() is expected to be called only in DEBUG mode."));
    User::Leave(KErrNotSupported);
#else

    INFO_PRINTF1(_L("CWapProtSuiteStepBase::ReleaseDiskSpaceL()"));
    
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    TVolumeInfo  volumeInfo;
    User::LeaveIfError(fs.Volume(volumeInfo, EDriveC));
    TUint64 current = volumeInfo.iFree;
    if( current > 0x7fffffff )
        {
        current = 0x7fffffff;
        }         
    
    SetFreeDiskSpaceL(current);
    CleanupStack::PopAndDestroy(&fs);
#endif       
	} // CSMSTestSteps::ReleaseDiskSpaceL


/**
	Get WAP Address from INI file
*/
void CWapProtSuiteStepBase::ReadWapPortSettingsL(TWapAddr &aWapAddr)
	{
	//Local vars	
	TPtrC16 telNumber;
	TInt port;
	
	//Read Port and SC number from INI file
	if(!GetStringFromConfig(ConfigSection(),KSCNumber,telNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port)
		)
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}
	  	

	aWapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(telNumber);
	TPtrC8 scAddr(scNumber);
	aWapAddr.SetWapAddress(scAddr);	  	
	}



/**
Used to check the CSmsMessage, will also print details of values in CSmsMessage
*/
void CWapProtSuiteStepBase::CheckSmsMessageL(CSmsMessage& aSmsmessagebuf, TPtrC8& aScnumber)
	{
	TPtrC toFromAddress = aSmsmessagebuf.ToFromAddress();
	TBuf<0x10> date;
	TBuf<0x10> timestring;
	TTime time = aSmsmessagebuf.Time();
	time.FormatL(date,(_L("%D%M%Y%/0%1%/1%2%/2%3%/3")));
	TDateTime dateTime(time.DateTime());
	INFO_PRINTF2(_L("Value for toFromAddress: %S"), &toFromAddress);
	INFO_PRINTF2(_L("Value for date: %S"), &date);
	INFO_PRINTF5(_L("TimeStamp=%02d:%02d:%02d.%06d"), dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond());
	HBufC16* scnumber16 = HBufC16::NewLC(aScnumber.Length());
	TPtr16 scnumber16mod = scnumber16->Des();
	scnumber16mod.Copy(aScnumber);
	INFO_PRINTF2(_L("Value for Service Centre number: %S"), &scnumber16mod);
	TESTL(scnumber16mod.Compare(aSmsmessagebuf.ServiceCenterAddress())==0);
	CleanupStack::PopAndDestroy(scnumber16);
	}

/**
Used to make call to get message parameters length via Ioctl
*/
TInt CWapProtSuiteStepBase::GetMessageParameterLengthL(RSocket& aSock)
	{
	TRequestStatus getParamStatus;
	TPckgBuf<TInt> paramLength;
	
	// Get the length of the message parameter value
	INFO_PRINTF1(_L("Issue of IOCTL for KSOGetMessageParametersLength"));
	aSock.Ioctl(KSOGetMessageParametersLength, getParamStatus, &paramLength, KSolWapProv);
	User::WaitForRequest(getParamStatus);	
	INFO_PRINTF1(_L("KSOGetMessageParametersLength on Ioctl completed"));
	TESTL(getParamStatus.Int()==KErrNone);

	// Return the message parameter length
	return paramLength();
	}
	
/**
Used to make call to get message parameters via Ioctl
*/
void CWapProtSuiteStepBase::GetMessageParameterL(RSocket& aSock, TPtr8& aParameterStorePtr)
	{
	TRequestStatus getParamStatus;
	
	// Get the message parameter and assign to TPtr8
	INFO_PRINTF1(_L("Issue of IOCTL for KSOGetMessageParameters"));
	aSock.Ioctl(KSOGetMessageParameters, getParamStatus, &aParameterStorePtr, KSolWapProv);
	User::WaitForRequest(getParamStatus);
	INFO_PRINTF1(_L("KSOGetMessageParameters on Ioctl completed"));
	TESTL(getParamStatus.Int()==KErrNone);
	}

/**
Used to internalise the TDes8 and check the resultant CSmsMessage
*/
void CWapProtSuiteStepBase::InternaliseSmsDataAndCheckL(TDes8& aBuffer, TPtrC8& aScnumber)
	{
	CSmsMessage* smsMessageBuf = NULL;
	
	RDesReadStream reader(aBuffer);
	reader.PushL();
	
	RFs rFs;
	User::LeaveIfError(rFs.Connect());
	CleanupClosePushL(rFs);
	
	smsMessageBuf = CSmsMessage::NewL(rFs, CSmsPDU::ESmsStatusReport, CSmsBuffer::NewL());
	smsMessageBuf->InternalizeWithoutBufferL(reader);
	
	INFO_PRINTF1(_L("Check the contents of the internalized sms message"));
	CheckSmsMessageL(*smsMessageBuf, aScnumber);
	
	rFs.Close();
	delete smsMessageBuf;
	CleanupStack::PopAndDestroy(); // rFs.Close()
	CleanupStack::PopAndDestroy(&reader);
	}

void CWapProtSuiteStepBase::OpenSocketL(RSocketServ& aSocketServer, RSocket& aSocket, TUint aAddrFamily,TUint aProtocol)
    {
    OpenSocketLC(aSocketServer, aSocket, aAddrFamily, aProtocol);   
    CleanupStack::Pop(&aSocket);
    }

void CWapProtSuiteStepBase::OpenSocketLC(RSocketServ& aSocketServer, RSocket& aSocket, TUint aAddrFamily, TUint aProtocol)
    {
    TInt error = aSocket.Open(aSocketServer,aAddrFamily,KSockDatagram,aProtocol);
    if(error != KErrNone)
         {
         //If error == KErrNone, do nothing.
         //If error == KErrServerBusy, change the leave code to "84" to imply the failure otherwise the failed step will be started again.
         //Any other errors will leave with the error code.

         
         if(error == KErrServerBusy)
             {
             error = TEST_ERROR_CODE;
             ERR_PRINTF2(_L("Open socket failed - error is KErrServerBusy, changing to TEST_ERROR_CODE (=%d)"), error);            
             }
         else
             {
             ERR_PRINTF2(_L("Open socket failed - error=%d"), error);
             }
          User::Leave(error);
          }   
  
    CleanupClosePushL(aSocket);
    }

