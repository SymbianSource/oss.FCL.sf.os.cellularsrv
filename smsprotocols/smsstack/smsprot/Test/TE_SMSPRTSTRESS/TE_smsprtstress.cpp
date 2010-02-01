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

#include "TE_smsprtstressbase.h"
#include "TE_smsprtstress.h"
#include "TE_smsprtstressutils.h"

#include <e32property.h>
#include <connect/sbdefs.h>
#include <sacls.h>

using namespace conn;


TVerdict CTestInit::doTestStepL()
	{
	InitGlobalsL();
	return TestStepResult();
	}


TVerdict CTestRxAndRead::doTestStepL()
	{
	RSocketServ socketServer;
	PrepareRegTestLC(socketServer, 0);

	CTestReceiveEnumerateMessages* globalAO = CTestReceiveEnumerateMessages::NewL(KActiveReceiverPriority, socketServer, *iSmsStackTestUtils);
    CleanupStack::PushL(globalAO);

	globalAO->Start(2,5,3);

	TEST(globalAO->iStatus.Int() == KErrNone);

   	//globalAO->Cancel();

   	CleanupStack::PopAndDestroy(globalAO);
	CleanupStack::PopAndDestroy(&socketServer);

	return TestStepResult();
	}


TVerdict CTestMultipleSimultaneousRequests::doTestStepL()
	{
	INFO_PRINTF1(_L("Test multiple simultaneous requests handling"));

	RSocketServ socketServer;
	PrepareRegTestLC(socketServer, 1);

	TInt ret(KErrNone);

	RPointerArray<CTestSmsActiveBase> testSmsActives;
	CleanupResetAndDestroyPushL(testSmsActives);

	//Create instance of CTestSmsActiveStop that stops testing activity when tests are completed.
	CTestSmsActiveStop* testSmsActiveStop = new (ELeave) CTestSmsActiveStop(testSmsActives,ret);
	CleanupStack::PushL(testSmsActiveStop);

	//Create instance of CTestSmsSendActive that makes send requests to the SMS Stack.
	CTestSmsSendActive* sendActive = CTestSmsSendActive::NewL(socketServer,*testSmsActiveStop, 16, iFs, iSmsStackTestUtils, this);
	CleanupStack::PushL(sendActive);
	User::LeaveIfError(testSmsActives.Append(sendActive));
	CleanupStack::Pop(sendActive);

	//Create instance of CTestSmsParametersActive that makes retrieve and store parameters requests to the SMS Stack.
	CTestSmsParametersActive* parametersActive = CTestSmsParametersActive::NewL(socketServer,*testSmsActiveStop,70, iSmsStackTestUtils, this);
	CleanupStack::PushL(parametersActive);
	User::LeaveIfError(testSmsActives.Append(parametersActive));
	CleanupStack::Pop(parametersActive);

	//Create instance of CTestSmsStorageActive that makes enumerate, write and delete requests to the SMS Stack.
	CTestSmsStorageActive* storageActive = CTestSmsStorageActive::NewL(socketServer,*testSmsActiveStop,70, iFs,iSmsStackTestUtils, this);
	CleanupStack::PushL(storageActive);
	User::LeaveIfError(testSmsActives.Append(storageActive));
	CleanupStack::Pop(storageActive);

	//Create instance of CTestSmsReceiveActive that makes retrieve requests to the SMS Stack.
	CTestSmsReceiveActive* receiveActive = CTestSmsReceiveActive::NewL(socketServer,*testSmsActiveStop,16, iFs, iSmsStackTestUtils, this);
	CleanupStack::PushL(receiveActive);
	User::LeaveIfError(testSmsActives.Append(receiveActive));
	CleanupStack::Pop(receiveActive);

	sendActive->StartL();
	parametersActive->StartL();
	storageActive->StartL();
	receiveActive->StartL();

	//Start active objects
	CActiveScheduler::Start();

	//Checkpoint is called when test are ran through
	TEST(ret == KErrNone);

	CleanupStack::PopAndDestroy(testSmsActiveStop);	//stopper class
	CleanupStack::PopAndDestroy(&testSmsActives);	//active objects

	EndRegTest();

	return TestStepResult();
	}


TInt CTestSmsPrtBackup::SendSmsL(RSocket& aSocket)
	{

	INFO_PRINTF1(_L("Attempting to send an sms to +358405202000"));

	TBuf<32> buf;
	buf.Append(_L("TestSmsPrtBackup message"));

	CSmsMessage* message = iSmsStackTestUtils->CreateSmsMessageLC(CSmsPDU::ESmsSubmit, _L("+358405202000"), buf);

	RSmsSocketWriteStream writestream(aSocket);
	writestream << *message;
	writestream.CommitL();

 	//Send message
	TPckgBuf<TUint> sbuf;
	TRequestStatus status;
	aSocket.Ioctl(KIoctlSendSmsMessage,status,&sbuf, KSolSmsProv);

	//wait for message to be sent
	User::WaitForRequest(status);

	CleanupStack::PopAndDestroy(message);

	INFO_PRINTF2(_L("SendSmsL returning %d"), status.Int());
	return (status.Int());
	}

/* Test sending sms messages under backup and restore situations
	1 - Sending of an sms message under normal circumstances succeeds
	2 - Sending of an sms message fails when a backup is in progress
	3 - Sending of an sms message succeeds when a backup is complete
	4 - Sending of an sms message fails when a restore is in progress
	5 - Sending of an sms message succeeds when a restore is complete
	The test case is intended to validate the behaviour of the SMS Stack whilst a
	backup and restore is in progress, rather than testing the behaviour of the
	server during backup and restore. A test precondition is that the client
	already has a socket open prior to a backup and restore commencing,
	thus avoiding the errors that can be generated when opening the socket
	during backup and restore.*/
TVerdict CTestSmsPrtBackup::doTestStepL()
	{
	PrepareRegTestLC(iSocketServer, 2);
	RSocket socket;
	iSmsStackTestUtils->OpenSmsSocketLC(iSocketServer,socket,ESmsAddrSendOnly);

	INFO_PRINTF1(_L("Expecting SendSmsL to return KErrNone"));
	TEST(SendSmsL(socket) == KErrNone);

	// Notify the SMSPROT server that a backup is about to take place and
	// that the server should powerdown
	INFO_PRINTF1(_L("Simulating a backup notification"));
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURBackupFull, EBackupBase);

	INFO_PRINTF1(_L("Expecting SendSmsL to return KErrDisconnected"));
	TEST(SendSmsL(socket) == KErrDisconnected);


	// Notify the SMSPROT server that a backup has completed
	// that the server should powerup
	INFO_PRINTF1(_L("Simulating a backup complete notification"));
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURNormal, ENoBackup);

	// Required Pause to Allow SMSStack to hande notification and restart
	User::After(5*1000*1000);

	INFO_PRINTF1(_L("Expecting SendSmsL to return KErrNone"));
	TEST(SendSmsL(socket) == KErrNone);

	// Notify the SMSPROT server that a restore is about to take place and
	// that the server should powerdown
	INFO_PRINTF1(_L("Simulating a restore notification"));
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURRestorePartial, EBackupIncrement);

	INFO_PRINTF1(_L("Expecting SendSmsL to return KErrDisconnected"));
	TEST(SendSmsL(socket) == KErrDisconnected);

	// Notify the SMSPROT server that a restore has completed
	// that the server should powerup
	INFO_PRINTF1(_L("Simulating a restore complete notification"));
	
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURNormal, ENoBackup);

	// Required Pause to Allow SMSStack to hande notification and restart
	User::After(5*1000*1000);
	INFO_PRINTF1(_L("Expecting SendSmsL to return KErrNone"));
	TEST(SendSmsL(socket) == KErrNone);

	INFO_PRINTF1(_L("Closing SocketServer ..."));
	CleanupStack::PopAndDestroy(&socket);
	CleanupStack::PopAndDestroy(&iSocketServer);
	return TestStepResult();
	}


/**
 *  Tests the Backup and Restore operation when the phone is powered off.
 */
TVerdict CTestSmsPrtBackupWhenPhoneIsOff::doTestStepL()
	{
	PrepareRegTestLC(iSocketServer, 2);
	RSocket socket;
	iSmsStackTestUtils->OpenSmsSocketLC(iSocketServer,socket,ESmsAddrSendOnly);

	//
	// Perform backup abnd restore when the phone is powered down...
	//
	INFO_PRINTF1(_L("Simulating a backup and restore while the phone is off"));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid,
									  ESAPhoneOff));
	
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURBackupFull, EBackupBase);

	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURNormal, ENoBackup);

	User::After(5*1000*1000);

	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidPhonePwr.iUid,
									  ESAPhoneOn));

	INFO_PRINTF1(_L("Closing SocketServer..."));
	CleanupStack::PopAndDestroy(&socket);
	CleanupStack::PopAndDestroy(&iSocketServer);

	//
	// Ensure the SMS Stack has been reloaded, prior to performing the ESock
	// memory leak check.
	//
	PrepareRegTestLC(iSocketServer, 2);
	iSmsStackTestUtils->OpenSmsSocketLC(iSocketServer,socket,ESmsAddrSendOnly);

	CleanupStack::PopAndDestroy(&socket);
	CleanupStack::PopAndDestroy(&iSocketServer);

	return TestStepResult();
	} // CTestSmsPrtBackupWhenPhoneIsOff::doTestStepL
