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
// implementation of test steps
// 
//

/**
 @file
*/


#include "WapIoctlSteps.h"
#include "WapProtSuiteDefs.h"
#include <etel.h>
#include <es_wsms.h>
#include <simtsy.h>
#include <sacls.h>
#include <s32mem.h>
#include <smspver.h>


TVerdict CIoctlStep_1::doTestStepL()
/**
 *  Test step one:
 *  Sends/receives a 7Bit business card using IOCTL
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 1: send/receive 7-Bit business card"));

	TPtrC TestData1;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber=1;
	
	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));

	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Send a business card
	//
	TRequestStatus  status;
	
	TBuf8<200> data;
	data.Copy(TestData1);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Message sent..."));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Get the size of the first incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed"));

	//
	//	Check if reported length is correct
	//
	TBuf8<256> recvBuf(length);
	
	TEST(length()==data.Length());
	
	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));
	
	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));
	TEST(recvBuf.Compare(data) == 0);
	
	//
	//	Closing the socket
	//
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_2::doTestStepL()
/** 
 *	Test step two: 
 *	Sends/receives a 7-Bit calendar entry and VCard using IOCTL(two segments each)
 *	@return - TVerdict code
 * 
 */
	{

	INFO_PRINTF1(_L("IOCTL Test step 2: send/receive a VCal and VCard"));

	TPtrC TestData1;
	TPtrC TestData2;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 2;
	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KTestData2, TestData2) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port)
		)
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}
		
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used 
	//  for test (not required for SIM Tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);
	
	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 	
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();
	
	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;
	
	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));
	
	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of the IOCTL for GetLength for first VCal entry"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	
	TBuf8<300> data;
	
	//
	//	Send a calendar entry  
	//	
	TRequestStatus  status;

	data.Copy(TestData1);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("First Message (VCal) sent..."));

	//	Send a VCard entry 	
	data.Copy(TestData2);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Second message (VCard with extended GSM alphabets) sent..."));
	
	//
	//	Get the size of the first incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed"));
	//
	//	Check if reported length is correct
	//
	TBuf8<300> recvBuf;
	TEST(length()==TestData1.Length());
	
	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the first message (VCal)..."));
	
	//	Compare the received VCal message with the original one.
	//	They should be identical
	data.Copy(TestData1);	
	if (recvBuf.Compare(data) != 0)
		{
		ERR_PRINTF1(_L("VCal message received does not match with original message."));
		TEST(recvBuf.Compare(data) == 0);
		}		
	
	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of first message (VCal)"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));
	
	
	//Issue Ioctl for getting the length of the second message	
	INFO_PRINTF1(_L("Issue of the IOCTL for GetLength for second VCard entry"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	
	//Get the size of the second incomming message
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed for second message (VCard)"));
	
	//Check if reported length is correct	
	TEST(length()==TestData2.Length());
	
	//Receiving a datagram (VCard)
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the second message (VCard)..."));
	
	//	Compare the received VCard message with the original one
	//	They should be identical
	data.Copy(TestData2);	
	if (recvBuf.Compare(data) != 0)
		{
		ERR_PRINTF1(_L("VCard message received does not match with original message."));
		TEST(recvBuf.Compare(data) == 0);
		}
		
	//Confirm the receipt of the message to the client	
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));
	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
	CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_3::doTestStepL()
/**
 *  Test step 3:
 *  send/receive one 7-Bit two segments message and one 7-Bit one segment message
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 3: send/receive two 7-Bit messages"));

	TPtrC TestData1;
	TPtrC TestData2;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 3;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KTestData2, TestData2) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
	OpenSocketLC(socketServer, sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));

	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Send a calendar entry
	//
	TRequestStatus  status;

	TBuf8<300> data2;
	data2.Copy(TestData2);
	sock.SendTo(data2, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("First message sent..."));

	//
	//	Send a business card
	//
	TBuf8<200> data1;
	data1.Copy(TestData1);
	sock.SendTo(data1, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Second message sent..."));
	//
	//	Get the size of the first incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	TBuf8<256> recvBuf2(length);

	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf2,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the first message..."));
	TEST(recvBuf2.Compare(data2) == 0);

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));
	//
	//	Issue Ioctl for getting the length of the second message
	//
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	INFO_PRINTF1(_L("Issue of the 2nd IOCTL for GetLength"));

	//
	//	Get the size of the SECOND incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed"));
	//
	//	Check if reported length is correct
	//
	TBuf8<256> recvBuf1(length);
	TEST(length()==data1.Length());

	//
	//	Receiving the second message
	//
	sock.RecvFrom(recvBuf1,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Receievd the second datagram"));
	TEST(recvBuf1.Compare(data1) == 0);

	//
	//	Confirm the receipt of the second message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of the second message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_4::doTestStepL()
/**
 *  
 *  Test step 4:
 *  simulating client crash conditions with 7bit message
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 4: client crash condition 7-Bit message "));

	TPtrC TestData;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber =4;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
  
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Send a business card
	//
	TRequestStatus  status;

	TBuf8<200> data;
	data.Copy(TestData);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Message sent..."));
	//
	//	Get the size of the incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Check if reported length is correct
	//
	TBuf8<256> recvBuf(length);
	TEST(length()==data.Length());

	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));
	//
	//	Close socket
	//
	sock.Close();
	INFO_PRINTF1(_L("Socket was closed..."));
	//
	//	Open the socket
	//
	OpenSocketL(socketServer, sock);
 	INFO_PRINTF1(_L("Socket was opened..."));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.Bind(wapAddr));
	//
	//	Issue another get length of datagram as if not aware of the previous message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	//
	//	waiting for request to be completed
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Check if reported length is correct
	//
	TBuf8<256> newRecvBuf(length);
	TEST(length()==data.Length());

	//
	//	New message retrieved from SAR store
	//
	sock.RecvFrom(newRecvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));
	TEST(newRecvBuf.Compare(data) == 0);

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_5::doTestStepL()
/**
 *  
 *  Test step 5:
 *  simulating client crash conditions with 8bit push message
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 5: client crash condition with 8Bit push message"));

	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber= 5;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//

	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));
	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Get the size of the incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));

	TBuf8<256> recvBuf(length);
	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));
	//
	//	Close socket
	//
	sock.Close();
	INFO_PRINTF1(_L("Socket was closed..."));
	//
	//	Open the socket
	//
	OpenSocketL(socketServer, sock);
 	INFO_PRINTF1(_L("Socket was opened..."));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.Bind(wapAddr));
	//
	//	Issue another get length of datagram as if not aware of the previous message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	//
	//	waiting for request to be completed
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Allocate buffer
	//
	TBuf8<256> newRecvBuf(length);

	//
	//	New message retrieved from SAR store
	//
	sock.RecvFrom(newRecvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Compare the received message with the previous one
	//	They should be identical
	//
	TEST(newRecvBuf.Compare(recvBuf) == 0);

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_6::doTestStepL()
/**
 *  Test step 6:
 *  Sends/receives a business card (Old client testing)
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 6: send/receive a VCard (Old client testing)"));

	TPtrC TestData1;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 6;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus recvStatus;

	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Send a calendar entry
	//
	TRequestStatus  status;

	TBuf8<200> data;
	data.Copy(TestData1);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Message sent..."));

	//
	//	Receiving a datagram
	//
	TBuf8<256> recvBuf;
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_7::doTestStepL()
/**
 *  Test step 7:
 *  Receives a Push Message (8-Bit)
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 7: Receive push message(8-Bit)"));

	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 7;
	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
	OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));

	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	//
	//	Get the size of the incomming push message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Allocate buffer
	//
	TBuf8<256> recvBuf2(length);

	//
	//	Receiving push datagram
	//
	sock.RecvFrom(recvBuf2,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the push message..."));

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_8::doTestStepL()
/**
 *  Test step 8:
 *  Receives two Push Messages (8-Bit) Tests store of 8-Bit messages
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 8: Receive two push message(8-Bit)"));

	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber=8;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	//
	//	Get the size of the incomming push message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Allocate buffer
	//
	TBuf8<256> recvBuf1(length);

	//
	//	Receiving push datagram
	//
	sock.RecvFrom(recvBuf1,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the push message..."));

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Issue another Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	//
	//	Get the size of the second incomming push message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Allocate buffer
	//
	TBuf8<256> recvBuf2(length);

	//
	//	Receiving push datagram
	//
	sock.RecvFrom(recvBuf2,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the second push message..."));
	//
	//	Compare the received message with the previous one
	//	They should be identical
	//
	TEST(recvBuf2.Compare(recvBuf1) == 0);

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_9::doTestStepL()
/**
 *  Test step 9:
 *  Receive a 8-Bit push message (Old client testing)
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 9: receive a push message (Testing old client)"));

	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 9;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus recvStatus;

	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Receiving a datagram
	//
	TBuf8<256> recvBuf;
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_10::doTestStepL()
/**
 *  Test step Ten:
 *  Sends/receives a 7Bit business card using IOCTL
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 10: send/receive 7-Bit business card using IOCTL"));

	TPtrC TestData1;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber=10;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Send a calendar entry
	//
	TRequestStatus  status;

	TBuf8<200> data;
	data.Copy(TestData1);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Message sent..."));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Get the size of the first incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed"));
	//
	//	Check if reported length is correct
	//
	TBuf8<256> recvBuf(length);
	TEST(length()==data.Length());

	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));
	TEST(recvBuf.Compare(data) == 0);

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
	CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}



TVerdict CIoctlStep_11::doTestStepL()
/**
 *  Test step 11:
 *  Test Cancel IOCTL
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 11: Testing CancelIOCTL"));

	TPtrC16 TelNumber;
	TInt port=226;


	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Issue Cancel Ioctl
	//
	INFO_PRINTF1(_L("Issue of cancel IOCTL for GetLength"));
	sock.CancelIoctl();

	//
	//	Check if the outstanding IOCTL has been canceled
	//
	TEST(ioctlStatus==KErrCancel);

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_12::doTestStepL()
/**
 *  Test step 12:
 *  Test issue IOCTL without SetOption NewStyleClient
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 12: Test issue IOCTL without SetOption NewStyleClient"));

	TPtrC16 TelNumber;
	TInt port=226;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;

	TPckgBuf<TInt> length;

	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message (should fail)
	//
	INFO_PRINTF1(_L("Issue of IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	User::WaitForRequest(ioctlStatus);
	TEST(ioctlStatus!=KErrNone);

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}



TVerdict CIoctlStep_13::doTestStepL()
/**
 *  Test step two:
 *  Sends/receives a 8-Bit calendar entry using IOCTL(two segments)
 *  @return - TVerdict code
 *  
 */
	{

	INFO_PRINTF1(_L("IOCTL Test step 13: send/receive a 8-Bit VCal"));

	TPtrC TestData;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 13;
	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM Tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));

	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNameDCS,KWapSmsOptionLevel,EWapSms8BitDCS));

	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of the IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Send a calendar entry
	//
	TRequestStatus  status;

	TBuf8<300> data;
	data.Copy(TestData);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Message sent..."));

	//
	//	Get the size of the incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed"));
	//
	//	Check if reported length is correct
	//
	TBuf8<300> recvBuf(length);
	TEST(length()==data.Length());

	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));
	TEST(recvBuf.Compare(data) == 0);

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
    CleanupStack::PopAndDestroy(&sock);
	CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_14::doTestStepL()
/**
 *  
 *  Test step 14:
 *  simulating client crash conditions with 8bit calendar entry
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 14: client crash condition 8-Bit calendar entry "));

	TPtrC TestData;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber =14;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//

	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));

	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNameDCS,KWapSmsOptionLevel,EWapSms8BitDCS));

	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Send a Calendar entry
	//
	TRequestStatus  status;

	TBuf8<300> data;
	data.Copy(TestData);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Message sent..."));
	//
	//	Get the size of the incomming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Check if reported length is correct
	//
	TBuf8<300> recvBuf(length);
	TEST(length()==data.Length());

	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));
	//
	//	Close socket
	//
	sock.Close();
	INFO_PRINTF1(_L("Socket was closed..."));
	//
	//	Open the socket
	//
	OpenSocketL(socketServer, sock);
	INFO_PRINTF1(_L("Socket was opened..."));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.Bind(wapAddr));
	//
	//	Issue another get length of datagram as if not aware of the previous message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	//
	//	waiting for request to be completed
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Check if reported length is correct
	//
	TBuf8<300> newRecvBuf(length);
	TEST(length()==data.Length());

	//
	//	New message retrieved from SAR store
	//
	sock.RecvFrom(newRecvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));
	TEST(newRecvBuf.Compare(data) == 0);

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TVerdict CIoctlStep_15::doTestStepL()
/**
 *  Test step fifteen:
 *  Receives a 7Bit v-card using IOCTL with concatenation for new style client
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 15: send/receive 7-Bit v-card"));

	TPtrC16 TelNumber;
	TInt port=9204;
	TInt testNumber=15;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Get the size of the first incoming message
	//
	User::WaitForRequest(ioctlStatus);
	User::LeaveIfError(ioctlStatus.Int());
	INFO_PRINTF1(_L("IOCTL completed"));

	TBuf8<360> recvBuf(length);

	//
	//	Receiving a datagram that is 7bit with a concatenated length of > 160 characters
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	User::LeaveIfError(recvStatus.Int());
	INFO_PRINTF1(_L("Received the message..."));

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Closing the socket
	//
	sock.Close();
	socketServer.Close();
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::Pop(&socketServer);

	return TestStepResult();
	}


TInt CBackupRestoreStep::SendWapL()
	{
	//modified from test 10
	INFO_PRINTF1(_L("SendWapL: send 7-Bit business card using IOCTL"));

	TInt ret = KErrNone;
	TPtrC TestData1;
	TPtrC16 TelNumber;
	TInt port=226;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port)
		)
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}


	//
	//	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);



	//
	//	Define and open the socket
	//
	RSocket sock;
	ret = sock.Open(iSocketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol);
	if (ret != KErrNone)
		{
		//
		// PREQ399 changes mean that -1 is sometimes now returned from Open() when
		// a Backup and Restore session is in progress (DEF114381).
		//
		INFO_PRINTF2(_L("Socket could not be opened (error %d)..."), ret);

		return ret;
		}
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus status;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
	//
	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Send a calendar entry
	//
	TBuf8<200> data;
	data.Copy(TestData1);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	ret = status.Int();

	if(ret == KErrNone)
		{
		INFO_PRINTF1(_L("Message sent..."));
		}
	else
		{
		INFO_PRINTF2(_L("Message NOT sent (error %d)..."), ret);
		}


	//
	//	Closing the socket
	//
	sock.Close();

	CleanupStack::PopAndDestroy(&sock);


	return ret;
	}

TBool CBackupRestoreStep::IsWapFileOpenL()
	{
	TBool ret = ETrue; //assume the file was open
	RFs fsSession;
	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL(fsSession);

	CFileMan*  fileManager = CFileMan::NewL(fsSession);
	CleanupStack::PushL(fileManager);

	_LIT(KWapFile,"C:\\Private\\101F7989\\sms\\wapreast.dat");
	_LIT(KWapFileBackup,"C:\\Private\\101f7989\\sms\\wapreast.backup");

	TEntry entry;
	if (fsSession.Entry(KWapFile,entry)==KErrNone)  //  File found
		{
		TInt moveStatus = fileManager->Move(KWapFile,KWapFileBackup);
		TESTL(moveStatus == KErrNone || moveStatus == KErrInUse);

		// If the move was successful, the file is not opened
		// If the move failed with KErrInUse the file is opened
		if (moveStatus == KErrNone)
			{
			// move it back
			TEST(fileManager->Move(KWapFileBackup,KWapFile) == KErrNone);
			ret = EFalse;
			}
		else // moveStatus == KErrInUse
			{
			ret = ETrue;
			}
		}
	else
		{
		//file not found so it couldn't be open
		ret = EFalse;
		}

	CleanupStack::PopAndDestroy(fileManager);
	CleanupStack::PopAndDestroy(&fsSession);

	if(ret)
		INFO_PRINTF1(_L("Wapfile open..."));
	else
		INFO_PRINTF1(_L("Wapfile closed..."));

	return ret;
	}

TVerdict CBackupRestoreStep::doTestStepL()
/**
 *  
 *  Test step 16:
 *  simulating backup and restore
 *  @return - TVerdict code
 *  
 */
	{
	TInt testNumber =16;
	RProperty testNumberProperty;

	User::LeaveIfError(testNumberProperty.Attach(KUidPSSimTsyCategory, KPSSimTsyTestNumber));
	CleanupClosePushL(testNumberProperty);

	TRequestStatus status;
	testNumberProperty.Subscribe(status);
	User::LeaveIfError(testNumberProperty.Set(KUidPSSimTsyCategory,KPSSimTsyTestNumber,testNumber));
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	TInt testNumberCheck;
	User::LeaveIfError(testNumberProperty.Get(testNumberCheck));
	if (testNumber != testNumberCheck)
		User::Leave(KErrNotFound);

	//
	//	Connect to socket server
	//
 	User::LeaveIfError(iSocketServer.Connect());

	INFO_PRINTF1(_L("IOCTL Test step 16: send 7-Bit business card using IOCTL to test backup and restore"));

	INFO_PRINTF1(_L("Expecting SendWapL to succeed and the wap data file to be open"));
	TEST(SendWapL() == KErrNone);
	TEST(IsWapFileOpenL() != EFalse);

	// Notify the WAPPROT server that a backup is about to take place and
	// that the server should close the wapstor
	INFO_PRINTF1(_L("Simulating a backup notification"));
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURBackupFull, EBackupBase);

	INFO_PRINTF1(_L("Expecting SendWapL to fail and the wap data file to be closed"));
	TEST(SendWapL() != KErrNone);
	TEST(IsWapFileOpenL() == EFalse);

	// Notify the WAPPROT server that a backup has completed
	// that the server should open the wapstor
	INFO_PRINTF1(_L("Simulating a backup complete notification"));
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURNormal, ENoBackup);

	INFO_PRINTF1(_L("Expecting SendWapL to succeed and the wap data file to be open"));
	TEST(SendWapL() == KErrNone);
	TEST(IsWapFileOpenL() != EFalse);

	// Notify the WAPPROT server that a restore is about to take place and
	// that the server should close the wapstor
	INFO_PRINTF1(_L("Simulating a restore notification"));
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURRestorePartial, EBackupIncrement);

	INFO_PRINTF1(_L("Expecting SendWapL to fail and the wap data file to be closed"));
	TEST(SendWapL() != KErrNone);
	TEST(IsWapFileOpenL() == EFalse);

	// Notify the WAPPROT server that a restore has completed
	// that the server should open the wapstor
	INFO_PRINTF1(_L("Simulating a restore complete notification"));
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURNormal, ENoBackup);

	INFO_PRINTF1(_L("Expecting SendWapL to succeed and the wap data file to be open"));
	TEST(SendWapL() == KErrNone);
	TEST(IsWapFileOpenL() != EFalse);

	INFO_PRINTF1(_L("Closing SocketServer ..."));
	iSocketServer.Close();
	
	CleanupStack::PopAndDestroy(&testNumberProperty);
	return TestStepResult();
	}
	
TVerdict CIoctlStep_17::doTestStepL()
/**
 *  Test step seventeen:
 *  Sends a 7_Bit business card. Using IOCTL query SMS transport parameters
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 17: send 7-Bit v-card and query SMS transport parameters"));

	TInt result = KErrNone;
	TPtrC TestData1;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber=1;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result = KErrNotFound;
		}
	
	TESTL(result==KErrNone);
	
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);
	
	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);
	
	//
	//	Send a business card
	//
	TRequestStatus status;
	TBuf8<200> data;
	data.Copy(TestData1);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	TESTL(status.Int()==KErrNone);
	INFO_PRINTF1(_L("Message sent..."));
	
	//
	// Create a descriptor with correct size for obtaining the message parameter
	//
	HBufC8* parameterStore = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr = parameterStore->Des();
	
	//
	// Get the message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr);
	
	//
	// Internalise the message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore);
	
	//
	//	Closing the socket
	//
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}
	
TVerdict CIoctlStep_18::doTestStepL()
/**
 *  Test step eighteen:
 *  Sends a 7-Bit calendar entry (two segments). Using IOCTL query SMS transport parameters.
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 18: Sends a 7-Bit calendar entry (two segments). Using IOCTL query SMS transport parameters."));

	TInt result = KErrNone;
	TPtrC TestData;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 2;
	
	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result = KErrNotFound;
		}

	TESTL(result==KErrNone);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM Tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();
	
	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);
	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);
		
	//
	//	Send a calendar entry
	//
	TRequestStatus status;
	TBuf8<300> data;
	data.Copy(TestData);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	TESTL(status.Int()==KErrNone);
	INFO_PRINTF1(_L("Message sent..."));

	//
	// Create a descriptor with correct size for obtaining the message parameter
	//
	HBufC8* parameterStore = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr = parameterStore->Des();
	
	//
	// Get the message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr);
	
	//
	// Internalise the message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore);
	
	//
	//	Closing the socket

    CleanupStack::PopAndDestroy(&sock);
	CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}
	
TVerdict CIoctlStep_19::doTestStepL()
/**
 *  Test step 19:
 *  send/receive one 7-Bit two segments message and one 7-Bit one segment message
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 19: send/receive two 7-Bit messages"));

	TInt result = KErrNone;
	TPtrC TestData1;
	TPtrC TestData2;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 3;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KTestData2, TestData2) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result = KErrNotFound;
		}

	TESTL(result==KErrNone);
	
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);
	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Send a calendar entry
	//
	TRequestStatus  status;

	TBuf8<300> data2;
	data2.Copy(TestData2);
	sock.SendTo(data2, wapAddr, 0, status);
	User::WaitForRequest(status);
	TESTL(status.Int()==KErrNone);
	INFO_PRINTF1(_L("First message sent..."));
	//
	//	Send a business card
	//
	TBuf8<200> data1;
	data1.Copy(TestData1);
	sock.SendTo(data1, wapAddr, 0, status);
	User::WaitForRequest(status);
	TESTL(status.Int()==KErrNone);
	INFO_PRINTF1(_L("Second message sent..."));
	//
	//	Get the size of the first incoming message
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("IOCTL for GetLength completed..."));
	TBuf8<256> recvBuf2(length);
	
	INFO_PRINTF1(_L("Getting parameter value of first message..."));
	
	//
	// Create a descriptor with correct size for obtaining the first message parameter
	//
	HBufC8* parameterStore = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr = parameterStore->Des();
	
	//
	// Get the first message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr);
	
	//
	// Internalise the first message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore);
	
	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf2,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("Received the first message..."));

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	TESTL(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0)==KErrNone);
	
	//
	//	Issue Ioctl for getting the length of the second message
	//
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	INFO_PRINTF1(_L("Issue of the 2nd IOCTL for GetLength"));

	//
	//	Get the size of the SECOND incoming message
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("IOCTL completed"));
	
	//
	//	Check if reported length is correct
	//
	TBuf8<256> recvBuf1(length);
	TESTL(length()==data1.Length());
		
	INFO_PRINTF1(_L("Getting parameter value of second message..."));
	
	//
	// Create a descriptor with correct size for obtaining the second message parameter
	//
	HBufC8* parameterStore2 = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr2 = parameterStore2->Des();
	
	//
	// Get the second message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr2);
	
	//
	// Internalise the second message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr2, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore2);
	
	//
	//	Receiving the second message
	//
	sock.RecvFrom(recvBuf1,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("Receievd the second datagram"));
	TEST(recvBuf1.Compare(data1) == 0);

	//
	//	Confirm the receipt of the second message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of the second message"));
	TESTL(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Closing the socket
	//
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}
	
TVerdict CIoctlStep_20::doTestStepL()
/**
 *  
 *  Test step 20:
 *  simulating client crash conditions with 7bit message
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 20: client crash condition 7-Bit message "));

	TInt result = KErrNone;
	TPtrC TestData;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber =4;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result=KErrNotFound;
		}
		
	TESTL(result==KErrNone);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);
	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Send a business card
	//
	TRequestStatus status;
	TBuf8<200> data;
	data.Copy(TestData);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	TESTL(status.Int()==KErrNone);
	INFO_PRINTF1(_L("Message sent..."));
	//
	//	Get the size of the incoming message
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("IOCTL completed..."));
	//
	//	Check if reported length is correct
	//
	TBuf8<256> recvBuf(length);
	TESTL(length()==data.Length());
	
	//
	// Create a descriptor with correct size for obtaining the message parameter
	//
	HBufC8* parameterStore = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr = parameterStore->Des();
	
	//
	// Get the message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr);
	
	//
	// Internalise the message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore);

	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("Received the message..."));
	
	//
	//	Close socket
	//
	sock.Close();
	INFO_PRINTF1(_L("Socket was closed..."));
	
	//
	//	Open the socket
	//
	TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	INFO_PRINTF1(_L("Socket was opened..."));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.Bind(wapAddr)==KErrNone);
	
	//
	//	Issue another get length of datagram as if not aware of the previous message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	
	//
	//	waiting for request to be completed
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("IOCTL completed..."));
	
	//
	//	Check if reported length is correct
	//
	TBuf8<256> newRecvBuf(length);
	TESTL(length()==data.Length());
	
	//
	// Recreate a descriptor with correct size for obtaining the message parameter
	//
	HBufC8* parameterStore2 = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr2 = parameterStore2->Des();
	
	//
	// Get the message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr2);
	
	//
	// Internalise the message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr2, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore2);

	//
	//	New message retrieved from SAR store
	//
	sock.RecvFrom(newRecvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("Received the message..."));
	TEST(newRecvBuf.Compare(data) == 0);
	
	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	TESTL(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Closing the socket
	//
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}
	
TVerdict CIoctlStep_21::doTestStepL()
/**
 *  
 *  Test step 21:
 *  simulating client crash conditions with 8bit push message
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 21: client crash condition with 8Bit push message"));

	TInt result = KErrNone;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber= 5;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result=KErrNotFound;
		}
		
	TESTL(result==KErrNone);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);
	
	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);
	
	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Get the size of the incoming message
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("IOCTL completed..."));

	TBuf8<256> recvBuf(length);
	
	//
	// Create a descriptor with correct size for obtaining the message parameter
	//
	HBufC8* parameterStore = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr = parameterStore->Des();
	
	//
	// Get the message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr);
	
	//
	// Internalise the message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore);
	
	//
	//	Receiving a datagram
	//
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("Received the message..."));
	
	//
	//	Close socket
	//
	sock.Close();
	INFO_PRINTF1(_L("Socket was closed..."));
	
	//
	//	Open the socket
	//
	TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	INFO_PRINTF1(_L("Socket was opened..."));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.Bind(wapAddr)==KErrNone);
	
	//
	//	Issue another get length of datagram as if not aware of the previous message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	
	//
	//	waiting for request to be completed
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("IOCTL completed..."));
	
	//
	//	Allocate buffer
	//
	TBuf8<256> newRecvBuf(length);
	
	//
	// Recreate a descriptor with correct size for obtaining the message parameter
	//
	HBufC8* parameterStore2 = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr2 = parameterStore2->Des();
	
	//
	// Get the message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr2);
	
	//
	// Internalise the message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr2, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore2);
	
	//
	//	New message retrieved from SAR store
	//
	sock.RecvFrom(newRecvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("Received the message..."));
	
	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	TESTL(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0)==KErrNone);
	
	//
	//	Compare the received message with the previous one
	//	They should be identical
	//
	TESTL(newRecvBuf.Compare(recvBuf)==0);
	
	//
	//	Closing the socket
	//
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}

TVerdict CIoctlStep_22::doTestStepL()
/**
 *  Test step 22:
 *  Push Message (8-Bit). Using IOCTL query SMS transport parameters
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 22: Push Message (8-Bit) and query SMS transport parameters"));

	TInt result = KErrNone;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 7;
	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result=KErrNotFound;
		}
		
	TESTL(result==KErrNone);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);
	
	//
	// Create a descriptor with correct size for obtaining the message parameter
	//
	HBufC8* parameterStore = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr = parameterStore->Des();
	
	//
	// Get the message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr);
	
	//
	// Internalise the message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore);
	
	//
	//	Closing the socket
	//
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}
	
TVerdict CIoctlStep_23::doTestStepL()
/**
 *  Test step 23:
 *  Two Push Messages (8-Bit). Using IOCTL query SMS transport parameters
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 23: Push two push message(8-Bit) query transport parameters"));

	TInt result=KErrNone;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber=8;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result=KErrNotFound;
		}
		
	TESTL(result==KErrNone);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);

	//
	//	Issue Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Get the size of the incoming push message
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("IOCTL completed..."));
	
	INFO_PRINTF1(_L("Getting parameter value of first message..."));
	
	//
	// Create a descriptor with correct size for obtaining the first message parameter
	//
	HBufC8* parameterStore = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr = parameterStore->Des();
	
	//
	// Get the first message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr);
	
	//
	// Internalise the first message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore);

	//
	//	Allocate buffer
	//
	TBuf8<256> recvBuf1(length);

	//
	//	Receiving push datagram
	//
	sock.RecvFrom(recvBuf1,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("Received the push message..."));

	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	TESTL(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Issue another Ioctl for getting the length of the message
	//
	INFO_PRINTF1(_L("Issue of first IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Get the size of the second incoming push message
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("IOCTL completed..."));
	
	INFO_PRINTF1(_L("Getting parameter value of second message..."));

	//
	// Create a descriptor with correct size for obtaining the second message parameter
	//
	HBufC8* parameterStore2 = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr2 = parameterStore2->Des();
	
	//
	// Get the second message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr2);
	
	//
	// Internalise the second message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr2, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore2);
	
	//
	//	Allocate buffer
	//
	TBuf8<256> recvBuf2(length);

	//
	//	Receiving push datagram
	//
	sock.RecvFrom(recvBuf2,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	INFO_PRINTF1(_L("Received the second push message..."));

	//
	//	Compare the received message with the previous one
	//	They should be identical
	//
	TESTL(recvBuf2.Compare(recvBuf1)==0);
		
	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
	TESTL(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Closing the socket
	//
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}

TVerdict CIoctlStep_24::doTestStepL()
/**
 *  Test step 24:
 *  Test Cancel IOCTL
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 24: Testing Cancel IOCTL"));

	TInt result=KErrNone;
	TPtrC16 TelNumber;
	TInt port=226;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result=KErrNotFound;
		}
		
	TESTL(result==KErrNone);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TBuf8<200> smsdata;
	TPckgBuf<TInt> paramLength;

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);
	
	//
	//	Issue Ioctl for getting the length of the message parameter
	//
	INFO_PRINTF1(_L("Issue of IOCTL for KSOGetMessageParametersLength"));
	sock.Ioctl(KSOGetMessageParametersLength, ioctlStatus, &smsdata, KSolWapProv);
	
	//
	//	Issue Cancel Ioctl
	//
	INFO_PRINTF1(_L("Issue of cancel IOCTL for KSOGetMessageParametersLength"));
	sock.CancelIoctl();
	
	//
	//	Check if the outstanding IOCTL has been cancelled
	//
	TESTL(ioctlStatus.Int()==KErrCancel);

	//
	//	Issue Ioctl for getting the message parameter
	//
	INFO_PRINTF1(_L("Issue of IOCTL for KSOGetMessageParameters"));
	sock.Ioctl(KSOGetMessageParameters, ioctlStatus, &smsdata, KSolWapProv);

	//
	//	Issue Cancel Ioctl
	//
	INFO_PRINTF1(_L("Issue of cancel IOCTL for KSOGetMessageParameters"));
	sock.CancelIoctl();

	//
	//	Check if the outstanding IOCTL has been cancelled
	//
	TESTL(ioctlStatus==KErrCancel);

	//
	//	Closing the socket
	//
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}

TVerdict CIoctlStep_25::doTestStepL()
/**
 *  Test step 25:
 *  Test issue IOCTL without SetOption NewStyleClient
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 25: Test issue IOCTL without SetOption NewStyleClient"));

	TInt result=KErrNone;
	TPtrC16 TelNumber;
	TInt port=226;

	if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result=KErrNotFound;
		}
		
	TESTL(result==KErrNone);

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	TRequestStatus ioctlStatus;
	TBuf8<200> smsdata;
	TPckgBuf<TInt> paramLength;

	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);
	
	//
	//	Issue Ioctl for getting the length of the message parameter (should fail)
	//
	INFO_PRINTF1(_L("Issue of IOCTL for KSOGetMessageParametersLength"));
	sock.Ioctl(KSOGetMessageParametersLength, ioctlStatus, &paramLength, KSolWapProv);
	TESTL(ioctlStatus.Int()!=KErrNone);
				
	//
	//	Issue Ioctl for getting the message parameter (should fail)
	//
	INFO_PRINTF1(_L("Issue of IOCTL for KSOGetMessageParameters"));
	sock.Ioctl(KSOGetMessageParameters, ioctlStatus, &smsdata, KSolWapProv);
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()!=KErrNone);

	//
	//	Closing the socket
	//
    CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}
	
TVerdict CIoctlStep_26::doTestStepL()
/**
 *  Test step 26:
 *  Sends a 8-Bit calendar entry using IOCTL(two segments)
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("IOCTL Test step 26: send a 8-Bit VCal (two segments)"));

	TInt result=KErrNone;
	TPtrC TestData;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber = 13;
	
	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result=KErrNotFound;
		}
	
	TESTL(result==KErrNone);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM Tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));
	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);
	TESTL(sock.SetOpt(KWapSmsOptionNameDCS,KWapSmsOptionLevel,EWapSms8BitDCS)==KErrNone);

	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);

	//
	//	Send a calendar entry
	//
	TRequestStatus  status;

	TBuf8<300> data;
	data.Copy(TestData);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	TESTL(status.Int()==KErrNone);
	INFO_PRINTF1(_L("Message sent..."));
	
	//
	// Create a descriptor with correct size for obtaining the message parameter
	//
	HBufC8* parameterStore = HBufC8::NewLC(GetMessageParameterLengthL(sock));
	TPtr8 parameterStorePtr = parameterStore->Des();
	
	//
	// Get the message parameter value
	//
	GetMessageParameterL(sock, parameterStorePtr);
	
	//
	// Internalise the message parameter and check
	//
	InternaliseSmsDataAndCheckL(parameterStorePtr, scAddr);
	
	CleanupStack::PopAndDestroy(parameterStore);		
	
	//
	//	Closing the socket
	//
    CleanupStack::PopAndDestroy(&sock);
	CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}

struct SOneOpTestThreadInfo
	{
	CIoctlStep_27* iTest;
	TPtrC* iTestData1;
	TPtrC16* iTelNumber;
	TInt iPort;
	};

TInt CIoctlStep_27::DoPanicTestL(TPtrC* aTestData1, TPtrC16* aTelNumber, TInt aPort)
	{
	TInt testNumber = 1;
	TInt port = aPort;
	TPtrC TestData1 = *aTestData1;
	TPtrC16 TelNumber = *aTelNumber;

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	//
	//	Indicating to the protocol that it's a new client
	//
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);

	//
	//	Send a business card
	//
	TRequestStatus  status;

	TBuf8<200> data;
	data.Copy(TestData1);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	TESTL(status.Int()==KErrNone);

	TRequestStatus ioctlStatus;
	TRequestStatus recvStatus;
	TPckgBuf<TInt> length;
	
	TRequestStatus ioctlmessageStatus;
	TBuf8<200> smsdata;

	//
	//	Issue Ioctl for getting the length of the message
	//
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//
	//	Issue Ioctl for getting the message parameters (should panic!)
	//
	sock.Ioctl(KSOGetMessageParameters, ioctlmessageStatus, &smsdata, KSolWapProv);
	
	//
	//	Get the size of the first incoming message
	//
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int()==KErrNone);

	//
	//	Receiving a datagram
	//
	TBuf8<256> recvBuf(length);
	sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int()==KErrNone);
	TEST(recvBuf.Compare(data) == 0);
	
	//
	//	Confirm the receipt of the message to the client
	//
	TESTL(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0)==KErrNone);
	
	//
	//	Closing the socket
	//
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return KErrNone;
	}

TInt testPanicsL(TAny* aPtr)
	{
	SOneOpTestThreadInfo *pI=(SOneOpTestThreadInfo *)aPtr;
	
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAPD(err, pI->iTest->DoPanicTestL(pI->iTestData1, pI->iTelNumber, pI->iPort));
	
	delete cleanup;
	__UHEAP_MARKEND;
	return err;
	}

TVerdict CIoctlStep_27::doTestStepL()
/**
 *  Test step 27:
 *  Send a 7_Bit business card. Issue both a get length and get message parameters to IOCTL  at sametime
 *  @return - TVerdict code
 *  
 */
	{
	const TUint KHeapMinSize=0x01000;
	const TUint KHeapMaxSize=0x10000;

	TPtrC TestData1;
	TPtrC16 TelNumber;
	TInt port=226;
	
	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		User::Leave(KErrNotFound);
		}
	
	INFO_PRINTF1(_L("Send a 7_Bit business card. Issue both a get length and get message parameters to IOCTL"));

	TBool jitEnabled = User::JustInTime();
	
	User::SetJustInTime(EFalse);
	
	SOneOpTestThreadInfo info;

	info.iTest=this;
	info.iTestData1=&TestData1;
	info.iTelNumber=&TelNumber;
	info.iPort=226;
	
	RThread thread;
	TInt rc = thread.Create(_L("PanicThread"),testPanicsL,KDefaultStackSize,KHeapMinSize,KHeapMaxSize,&info);
	
	TESTL(rc==KErrNone);
		
	TRequestStatus s;
    thread.Logon(s);
	thread.Resume();
	User::WaitForRequest(s);
	TESTL(thread.ExitType()==EExitPanic);
			
	INFO_PRINTF2(_L("Exit type is: %d"), thread.ExitType());
	
	thread.Close();
	
	User::SetJustInTime(jitEnabled);
	
	return TestStepResult();
	}

TVerdict CIoctlStep_28::doTestStepPreambleL()
	{
	__UHEAP_MARK;
	
	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

	TInt err;
    err=User::LoadPhysicalDevice(PDD_NAME);
    TESTL(err==KErrNone || err==KErrAlreadyExists);

    err=User::LoadLogicalDevice(LDD_NAME);
    TESTL(err==KErrNone || err==KErrAlreadyExists);

    err = StartC32();
	ERR_PRINTF2(TRefByValue<const TDesC>(_L("Start Comms Process Status = %d")), err);
    TESTL(err==KErrNone || err==KErrAlreadyExists);

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
	
TVerdict CIoctlStep_28::doTestStepPostambleL()
	{
	delete iScheduler;
	iScheduler = NULL;

	__UHEAP_MARKEND;
	
	return TestStepResult();
	}
	
TVerdict CIoctlStep_28::doTestStepL()

/**
 *  Test step seventeen:
 *  Sends a 7_Bit business card. Issue a get message parameters to IOCTL without correct platform security
 *  @return - TVerdict code
 *  
 */
	{
	INFO_PRINTF1(_L("Sends a 7_Bit business card. Issue a get message parameters to IOCTL without correct platform security"));

	TInt result=KErrNone;
	TPtrC TestData1;
	TPtrC16 TelNumber;
	TInt port=226;
	TInt testNumber=1;

	if(!GetStringFromConfig(ConfigSection(),KTestData1, TestData1) ||
	   !GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber) ||
	   !GetIntFromConfig(ConfigSection(),KWapPort,port))
		{
		result=KErrNotFound;
		}
		
	TESTL(result==KErrNone);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr wapAddr;
	TWapAddr recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100> scNumber;
	scNumber.Copy(TelNumber);
	TPtrC8 scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket sock;
    TESTL(sock.Open(socketServer,KWAPSMSAddrFamily,KSockDatagram,KWAPSMSDatagramProtocol)==KErrNone);
	CleanupClosePushL(sock);

	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	TESTL(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0)==KErrNone);

	//
	//	Bind
	//
	TESTL(sock.Bind(wapAddr)==KErrNone);

	//
	//	Send a business card
	//
	TRequestStatus status;
	TBuf8<200> data;
	data.Copy(TestData1);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	TESTL(status.Int()==KErrNone);
	INFO_PRINTF1(_L("Message sent..."));
	
	//
	//	Issue Ioctl for getting the message parameters
	//
	TRequestStatus testStatus;
	TBuf8<200> smsdata;
	
	INFO_PRINTF1(_L("Issue of IOCTL for KSOGetMessageParameters"));
	sock.Ioctl(KSOGetMessageParameters, testStatus, &smsdata, KSolWapProv);
	
	User::WaitForRequest(testStatus);
	TESTL(testStatus.Int()==KErrPermissionDenied);
	INFO_PRINTF2(_L("KSOGetMessageParameters on Ioctl failed to complete with: %d"), testStatus.Int());
			
	//
	//	Closing the socket
	//
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}


TVerdict CSetDiskMonitorLimits::doTestStepL()
/**
	Creates smsu.rsc file which defines the upper and lower limits for the disk space monitor
*/

   {
#ifndef _DEBUG
    INFO_PRINTF1(_L("This test can only be run when the SMS Stack is in debug mode."));
#else       
    TInt highDrop = 4;
    TInt lowDrop = 10;
    TInt freeDrop = 0;
  
    GetIntFromConfig(_L("DiskMonitorLimits"), _L("highDrop"), highDrop);
    GetIntFromConfig(_L("DiskMonitorLimits"), _L("lowDrop"), lowDrop);
  
    SetHighLowLimitsAndDiskSpaceLevelL(highDrop, lowDrop, freeDrop); 
#endif    
	return TestStepResult();
    }
    
/**
 *  Set free disk space to the required limit
 */
TVerdict CSetDiskSpace::doTestStepL()
    {
#ifndef _DEBUG
    INFO_PRINTF1(_L("This test can only be run when the SMS Stack is in debug mode."));
#else    
    ReleaseDiskSpaceL();
     
    TInt  freeDrop = 0;
    GetIntFromConfig(ConfigSection(), _L("freeDrop"), freeDrop);         
 
    SetFreeDiskSpaceFromDropLevelL(freeDrop);
#endif        
	return TestStepResult();
    }
    
/**
	Free up disk space by deleting the temp files created
*/
TVerdict CFreeDiskSpace::doTestStepL()
   {
#ifndef _DEBUG
    INFO_PRINTF1(_L("This test can only be run when the SMS Stack is in debug mode."));
#else   
    ReleaseDiskSpaceL();
      
    TInt err = RProperty::Delete(KUidPSSMSStackCategory, KUidPSSMSStackFreeDiskSpaceKey);
    if (err != KErrNone && err != KErrNotFound)
        {
        ERR_PRINTF2(_L("RProperty::Delete() failure [err=%d]"), err);
        }           
#endif  
   	  	
	return TestStepResult();
   }    

/**
	Free up disk space by deleting the temp files created
*/
TVerdict CInitializePhone::doTestStepL()
   {
	SetTestNumberL();
	WaitForInitializeL();
	
#ifdef _DEBUG   
    TInt err = RProperty::Define(KUidPSSMSStackCategory, KUidPSSMSStackFreeDiskSpaceKey, RProperty::EInt);
    if ((err != KErrNone) && (err != KErrAlreadyExists))    
        {
        ERR_PRINTF2(_L("RProperty::Define() failure [err=%d]"), err);
        User::Leave(err);
        }
#endif  	

	return TestStepResult();
   }    
   
   
/**
	Free up disk space by deleting the temp files created
*/
TVerdict CReceiveWapMessage::doTestStepL()
   {
#ifndef _DEBUG
    INFO_PRINTF1(_L("This test can only be run when the SMS Stack is in debug mode."));
#else   
    
	//	Connect to socket server
	RSocketServ socketServer;
	User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);
	//	Define and open the socket
	RSocket sock;	
	OpenSocketLC(socketServer, sock);
	WaitForInitializeL();
	
	//Read port and SC number from ini file
	TWapAddr wapAddr;
	ReadWapPortSettingsL(wapAddr);
	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client"));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));

	//	Bind
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	TRequestStatus recvStatus;
	TRequestStatus ioctlStatus;
	TPckgBuf<TInt> length;	  	

	//
	//	Send a business card
	//
	TRequestStatus status;
	TPtrC testData;
	GetStringFromConfig(ConfigSection(),KTestData1, testData);
	TBuf8<200> data;
	data.Copy(testData);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Message sent..."));

	//	Issue Ioctl for getting the length of the message
	TBool messageIsExpected;
	GetBoolFromConfig(ConfigSection(), _L("messageIsExpected"), messageIsExpected);

	//Setup delay
	TRequestStatus timerStatus;
	RTimer timer;
	//Read from the INI file.  
	TInt timeout;
	TBool found = GetIntFromConfig(_L("ReceiveTimeout"), _L("timeout"), timeout);
		//Timeout must be specified
	if(!found)
		{
		timeout=10000000;		// else a default of 10 seconds will be used
		}
	
	timer.CreateLocal();
	timer.After(timerStatus, TTimeIntervalMicroSeconds32(timeout));

	//	Get the size of the first incomming message
	INFO_PRINTF1(_L("Issue of the IOCTL for GetLength"));
	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);

	//Check if there is in fact a message
	TBool received=EFalse;
	User::WaitForRequest(timerStatus, ioctlStatus);
	
	if(ioctlStatus == KRequestPending)
		{
		// if timer elapsed but ESock request is still pending
		sock.CancelIoctl();
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
			received=ETrue;		     
			}
		}
	timer.Close();

	if(received && messageIsExpected)
		{
		//	Receiving push datagram
		TWapAddr recvWapAddr;			
		TBuf8<256> recvBuf1(length);
		sock.RecvFrom(recvBuf1,recvWapAddr,0,recvStatus);
		User::WaitForRequest(recvStatus);
		User::LeaveIfError(recvStatus.Int());
		INFO_PRINTF1(_L("Received the push message..."));
		TEST(recvBuf1.Compare(data) == 0);

		//
		//	Confirm the receipt of the message to the client
		//
		INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
		User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));
		
		}
	else if (received  ||  messageIsExpected)
		{
		//Message is not expected
		TEST(EFalse);
		}

    CleanupStack::Pop(&sock);
    CleanupStack::Pop(&socketServer);
    sock.Close();
    socketServer.Close();
	INFO_PRINTF1(_L("End of ReceiveWapMessage test step"));
#endif	

	return TestStepResult();
   } 


/**
 *  Tests enumerating of a VCard which needs to go via the observer and not returned to the client.
 *
 *  @return A TVerdict code.
 */
TVerdict CTestEnumeratingVCard::doTestStepL()
	{
	//
	// Setup the test...
	//
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, 34));

	//
	// Connect to socket server
	//
	RSocketServ  socketServer;

    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	// Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	//
	// Define and open the socket to receive the datagram on...
	//
	RSocket  sock;
	TWapAddr  wapAddr;
	OpenSocketLC(socketServer, sock);

	wapAddr.SetWapPort(TWapPortNumber(226));
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Indicate to the protocol that it's a new client
	//
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));

	//
	// Create a socket for the enumeration...
	//
	RSocket   enumerateSock;
	TSmsAddr  smsAddr;

	OpenSocketLC(socketServer, enumerateSock, KSMSAddrFamily,KSMSDatagramProtocol);
 
	smsAddr.SetSmsAddrFamily(ESmsAddrLocalOperation);
	User::LeaveIfError(enumerateSock.Bind(smsAddr));

	//
	// Trigger the enumeration...
	//
	TPckgBuf<TUint>  messageCount;
	TRequestStatus  enumStatus;
	
	User::After(1000000); // Gives SMS Prot and WAP Prot time to initialise after loading!
	enumerateSock.Ioctl(KIoctlEnumerateSmsMessages, enumStatus, &messageCount, KSolSmsProv);
	User::WaitForRequest(enumStatus);
	TESTL(enumStatus.Int() == KErrNone);
	TEST(messageCount() == 0);

	//
	// Issue an Ioctl for getting the length of the enumerated datagram...
	//
	TRequestStatus  ioctlStatus;
	TPckgBuf<TInt>  length;

	sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
	User::WaitForRequest(ioctlStatus);
	TESTL(ioctlStatus.Int() == KErrNone);
	TEST(length() == 118);

	//
	// Receive the VCard...
	//
	TRequestStatus  recvStatus;
	TBuf8<256>  recvBuf;
	TBuf<256>  vcardBuf16;
	TWapAddr  recvWapAddr;

	sock.RecvFrom(recvBuf, recvWapAddr, 0, recvStatus);
	User::WaitForRequest(recvStatus);
	TESTL(recvStatus.Int() == KErrNone);
	
	vcardBuf16.Copy(recvBuf);
	INFO_PRINTF2(_L("Received the VCard: \"%S\""), &vcardBuf16);

	//
	// Confirm the receipt of the message...
	//
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));

	//
	//	Close all the sockets
	//
	CleanupStack::PopAndDestroy(&enumerateSock);
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	} // CTestEnumeratingVCard::doTestStepL


/**
 *  Sends a 7-bit VCard with an email address in it, to ensure that '@'
 *  characters are sent correctly.
 * 
 *  @return EPass or EFail.
 */
TVerdict CTest7BitBusinessCardWithEmail::doTestStepL()
	{
	TPtrC    testData;
	TPtrC16  telNumber;
	TInt     port;
	TInt     testNumber = 35;
	
	if(!GetStringFromConfig(ConfigSection(), KTestData1, testData) ||
	   !GetStringFromConfig(ConfigSection(), KSCNumber, telNumber) ||
	   !GetIntFromConfig(ConfigSection(), KWapPort, port))
		{
		// Leave if there's any error.
		User::Leave(KErrNotFound);
		}

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber));

	//
    //	Setting the port number and service center number of the wap address
	//	The service center number should be the same as the sim phone number used
	//  for test (not required for SIM tsy)
	//
	TWapAddr  wapAddr;
	TWapAddr  recvWapAddr;
	wapAddr.SetWapPort(TWapPortNumber(port));

	TBuf8<100>  scNumber;
	scNumber.Copy(telNumber);
	TPtrC8  scAddr(scNumber);
	wapAddr.SetWapAddress(scAddr);

	//
	//	Connect to socket server
	//
	RSocketServ  socketServer;
    User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	//	Define and open the socket
	//
	RSocket  sock;
    OpenSocketLC(socketServer, sock);
 
	//
	//	Waiting for the phone to be initialised
	//
	WaitForInitializeL();

	//
	//	Indicating to the protocol that it's a new client
	//
	INFO_PRINTF1(_L("Socket set option for indicating new client..."));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient, KWapSmsOptionLevel, 0));

	//
	//	Bind to the port...
	//
	User::LeaveIfError(sock.Bind(wapAddr));

	//
	//	Send a business card with the email address.
	//
	TRequestStatus  status;
	
	TBuf8<200>  data;
	data.Copy(testData);
	sock.SendTo(data, wapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Message sent..."));

	//
	//	Issue Ioctl for getting the length of the message
	//
	TPckgBuf<TInt>  length;

	INFO_PRINTF1(_L("Issue of IOCTL for GetLength..."));
	sock.Ioctl(KSOGetLength, status, &length, KSolWapProv);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());

	//
	//	Check if reported length is correct
	//
	TEST(length() == data.Length());
	
	//
	//	Receiving a datagram
	//
	TBuf8<256>  recvBuf(length);
	
	sock.RecvFrom(recvBuf, recvWapAddr, 0, status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());
	INFO_PRINTF1(_L("Received the message..."));
	
	//
	//	Confirm the receipt of the message to the client
	//
	INFO_PRINTF1(_L("Socket set option for indicating receipt of message..."));
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));
	TEST(recvBuf.Compare(data) == 0);
	
	//
	//	Closing the socket
	//
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	} // CTest7BitBusinessCardWithEmail::doTestStepL


/**
 *  Attempts to send datagrams bigger than the maximum message and bigger
 *  than the maximum size of an MBuf.
 * 
 *  @return EPass or EFail.
 */
TVerdict CTestOversizedDatagram::doTestStepL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid, ESAPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, 1));

	TWapAddr wapAddr;
	wapAddr.SetWapPort(TWapPortNumber(226));

	//
	// Connect to socket server...
	//
	RSocketServ  socketServer;

	User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	//
	// Define and open the socket...
	//
	RSocket  sock;
    OpenSocketLC(socketServer, sock);
 
	//
	// Wait for the phone to be initialised...
	//
	WaitForInitializeL();

	//
	// Indicating to the protocol that it's a new client...
	//
	User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));

	//
	// Send a datagram of the maximum datagram size (this will fail because
	// of the WAP headers)...
	//
	TRequestStatus  status;
	
	HBufC8*  textBuf = HBufC8::NewL(65536+1);
	CleanupStack::PushL(textBuf);
	TPtr8  textPtr = textBuf->Des();

	textPtr.Fill('A', KWAPSMSMaxDatagramSize);
	sock.SendTo(textPtr, wapAddr, 0, status);
	User::WaitForRequest(status);
	TEST(status.Int() == KErrOverflow);

	//
	// Send a datagram bigger than the maximum datagram...
	//
	textPtr.Fill('B', KWAPSMSMaxDatagramSize+1);
	sock.SendTo(textPtr, wapAddr, 0, status);
	User::WaitForRequest(status);
	TEST(status.Int() == KErrTooBig);

	//
	// Send a datagram bigger than the maximum MBuf...
	//
	textPtr.Fill('C', 65536+1);
	sock.SendTo(textPtr, wapAddr, 0, status);
	User::WaitForRequest(status);
	TEST(status.Int() == KErrTooBig);

	CleanupStack::PopAndDestroy(textBuf);

	//
	//	Close the socket...
	//
	CleanupStack::PopAndDestroy(&sock);
    CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	} // CTestOversizedDatagram::doTestStepL


TVerdict CTestWapDatagramSegmentContainingNoData::doTestStepL()
/** 
  *  Test step: Receive 3 Wap Datagrams, each WAP message contains
  *  one or more segments which comprise of a wap header but no data. 
  *  
  *  @return - TVerdict code
  * 
  */
     {
     INFO_PRINTF1(_L("CTestWapDatagramSegmentContainingNoData:"));
 
     TVerdict verdict = EPass;
 
     // Create an empty string which will encode into a single segment wap message which contains a wap header, but no wap data.
     _LIT8(KLocalTestData1, "");
     TBuf8<300> data1(KLocalTestData1);      
     // Create a VCard which will encode into 2 segment wapdatagram, the 2nd segment contains a wap header, but no wap data.
     // Use a local literal string rather than reading from a text file. This prevents extra unwanted '\' being added to the string, for example \\r\\n
 	_LIT8(KLocalTestData2,"BEGIN:VCARD\r\nVERSION:2.1\r\nREV:20090403T094807Z\r\nUID:83702f931a905a6e-00e14456815a8324-33\r\nN:;3SIM;;;\r\nTEL;WORK;CELL:07878931672\r\nEND:VCARD\r");
      TBuf8<300> data2(KLocalTestData2);
 
     
     TPtrC16 TelNumber;
     TInt port=9204;
     TInt testNumber = 36;
     if(!GetStringFromConfig(ConfigSection(),KSCNumber,TelNumber))
         {
         // Leave if there's any error.
         User::Leave(KErrNotFound);
         }
         
     RProperty phonePowerProperty;
     User::LeaveIfError(phonePowerProperty.Attach(KUidSystemCategory, KUidPhonePwr.iUid));
     CleanupClosePushL(phonePowerProperty);
 
     RProperty testNumberProperty;
     User::LeaveIfError(testNumberProperty.Attach(KUidPSSimTsyCategory, KPSSimTsyTestNumber));
     CleanupClosePushL(testNumberProperty);  
 
     TRequestStatus status;
     phonePowerProperty.Subscribe(status);
     User::LeaveIfError(phonePowerProperty.Set(KUidSystemCategory,KUidPhonePwr.iUid,ESAPhoneOn));
     User::WaitForRequest(status);
     User::LeaveIfError(status.Int());
     
     testNumberProperty.Subscribe(status);
     User::LeaveIfError(testNumberProperty.Set(KUidPSSimTsyCategory,KPSSimTsyTestNumber,testNumber));
     User::WaitForRequest(status);
     User::LeaveIfError(status.Int());
     TInt testNumberCheck;
     User::LeaveIfError(testNumberProperty.Get(testNumberCheck));
     if (testNumber != testNumberCheck)
         {
         User::Leave(KErrNotFound);  
         }
     //
     //  Setting the port number and service center number of the wap address
     //  The service center number should be the same as the sim phone number used 
     //  for test (not required for SIM Tsy)
     //
     TWapAddr wapAddr;
     TWapAddr recvWapAddr;
     wapAddr.SetWapPort(TWapPortNumber(port));
     TBuf8<100> scNumber;
     scNumber.Copy(TelNumber);
     TPtrC8 scAddr(scNumber);
     wapAddr.SetWapAddress(scAddr);
     
     //
     //  Connect to socket server
     //
     RSocketServ socketServer;
     User::LeaveIfError(socketServer.Connect());
     CleanupClosePushL(socketServer);
 
     //
     //  Define and open the socket
     //
     RSocket sock;
     OpenSocketLC(socketServer, sock);
     

     //
     //  Waiting for the phone to be initialised
     //
     WaitForInitializeL();
     
     TRequestStatus ioctlStatus;
     TRequestStatus recvStatus;
     TPckgBuf<TInt> length;
     
     //
     //  Indicating to the protocol that it's a new client
     //
     INFO_PRINTF1(_L("Socket set option for indicating new client"));
     User::LeaveIfError(sock.SetOpt(KWapSmsOptionNewStyleClient,KWapSmsOptionLevel, 0));
     //
     //  Bind
     //
     User::LeaveIfError(sock.Bind(wapAddr));
     
     //
     //  Issue Ioctl for getting the length of the message
     //
     INFO_PRINTF1(_L("Issue of the IOCTL for GetLength for first VCal entry"));
     sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
 
     
     TBuf8<300> data;
     
     //
     //  Send a SMS in order to trigger receiving the SMS messages.
     //  This is to make sure the SMS messages are not received before the 
     //  wap address is bound to the socket.
     //  
     sock.SendTo(data1, wapAddr, 0, status);
     User::WaitForRequest(status);
     User::LeaveIfError(status.Int());
     INFO_PRINTF1(_L("First Message (VCal) sent..."));
     
     //
     //  Get the size of the first incoming message
     //  The text string should be empty.
     //
     TBuf8<300> recvBuf;    
     User::WaitForRequest(ioctlStatus);
     User::LeaveIfError(ioctlStatus.Int());
     INFO_PRINTF1(_L("IOCTL completed"));
     //
     //  Check if reported length is correct
     //
     if(length()!=data1.Length())    
         {
         verdict = EFail;
         }
     //>     //  Receiving a datagram
     //
     sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
     User::WaitForRequest(recvStatus);
     User::LeaveIfError(recvStatus.Int());
     INFO_PRINTF1(_L("Received the first message (VCal)..."));
     
     //  The receive buffer should contain an empty string.
     //  This is as a consequence of the wapprot receiving
     //  a single segment wap message which contains no data.
     if(recvBuf.Compare(data1))
         {
         ERR_PRINTF1(_L("Non empty string returned"));
         verdict = EFail;
         }       
     
     //
     //  Confirm the receipt of the message to the client
     //
     INFO_PRINTF1(_L("Socket set option for indicating receipt of first message (VCal)"));
     User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));
     
     
     //Issue Ioctl for getting the length of the second message  
     INFO_PRINTF1(_L("Issue of the IOCTL for GetLength for second VCard entry"));
     sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
     
     //Get the size of the second incoming message
     User::WaitForRequest(ioctlStatus);
     User::LeaveIfError(ioctlStatus.Int());
     INFO_PRINTF1(_L("IOCTL completed for second message (VCard)"));
     
     //Check if reported length is correct
     if(length()!=data2.Length())
         {
         verdict = EFail;
         }
         
     //Receiving a datagram (VCard)
     sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
     User::WaitForRequest(recvStatus);
     User::LeaveIfError(recvStatus.Int());
     INFO_PRINTF1(_L("Received the second message (VCard)..."));
     
     //  Compare the received VCard message with the original one.
     //  They should be identical. This checks that the WAPPROT
     //  can receive a 2 segment wap datagram, with the 2nd segment containing no data.
     //
     if(recvBuf.Compare(data2))
         {
         ERR_PRINTF1(_L("VCard message received does not match with original message."));
         verdict = EFail;
         }
         
     //Confirm the receipt of the message to the client  
     INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
     User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));
     //Issue Ioctl for getting the length of the third message  

     INFO_PRINTF1(_L("Issue of the IOCTL for GetLength for second VCard entry"));
     sock.Ioctl(KSOGetLength, ioctlStatus, &length, KSolWapProv);
     
     //Get the size of the third incoming message
     User::WaitForRequest(ioctlStatus);
     User::LeaveIfError(ioctlStatus.Int());
     INFO_PRINTF1(_L("IOCTL completed for third message (VCard)"));
     
     //Check if reported length is correct   
     if(length()!=data1.Length())
         {
         verdict = EFail;
         }
     
     //Receiving a datagram (VCard)
     sock.RecvFrom(recvBuf,recvWapAddr,0,recvStatus);
     User::WaitForRequest(recvStatus);
     User::LeaveIfError(recvStatus.Int());
     INFO_PRINTF1(_L("Received the third message (VCard)..."));
     
     //  The receive buffer should contain an empty string.
     //  This is as a consequence of the WAPPROT containing 2 concatenated WAP Datagrams
     //  each datagram segement containing no data.
      if(recvBuf.Compare(data1))
         {
         ERR_PRINTF1(_L("VCard message received does not match with original message."));
         verdict = EFail;
         }
         
     //Confirm the receipt of the message to the client  
     INFO_PRINTF1(_L("Socket set option for indicating receipt of message"));
     User::LeaveIfError(sock.SetOpt(KWapSmsOptionOKToDeleteMessage,KWapSmsOptionLevel, 0));  
     //
     //  Closing the socket
     //
     sock.Close();
     socketServer.Close();
     CleanupStack::PopAndDestroy(&sock);
     CleanupStack::Pop(&socketServer);
     CleanupStack::PopAndDestroy(&testNumberProperty);
     CleanupStack::PopAndDestroy(&phonePowerProperty);
     INFO_PRINTF1(_L("End of CTestWapDatagramSegmentContainingNoData"));
     SetTestStepResult(verdict);
     return TestStepResult();
     }

