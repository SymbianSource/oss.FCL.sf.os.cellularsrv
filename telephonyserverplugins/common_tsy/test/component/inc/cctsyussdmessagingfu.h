// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The TEFUnit header file which tests the UssdMessaging
// functional unit of the Common TSY.
// 
//

#ifndef CCTSYUSSDMESSAGINGFU_H
#define CCTSYUSSDMESSAGINGFU_H

#include <test/tefunit.h>

#include <etelmm.h>
#include <etelmmcs.h>

#include "cctsycomponenttestbase.h"
#include "tmockltsydata.h"
#include <mmtsy_names.h>
#include <mmtsy_ipcdefs.h>


struct TTestUssdData {
    TBool iAcceptDialogue;
	TInt iAfterTimePeriod;
    };

struct TUssdClientTestData
    {
    TUssdClientTestData(TPtrC aExe, TPtrC aClientParams, TInt aReqStatus, TExitType aExitType, TInt aExitReason)
    : iExe(aExe), iClientParams(aClientParams), iReqStatus(aReqStatus), iExitType(aExitType), iExitReason(aExitReason)
    {};
    TPtrC iExe;
    TPtrC iClientParams;
    TInt iReqStatus;
    TExitType iExitType;
    TInt iExitReason;
    };

struct TTestDataWithChunk : public TTestUssdData
	{
	TTestDataWithChunk(RChunk& aChunk) : iChunk(aChunk)
	{};
	
	RChunk& iChunk;
	
	};
	
class CCTsyUssdMessagingFU : public CCtsyComponentTestBase
	{
public:
	// Create a suite of all the tests
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	TInt CreateClients(RThread& aT1, RThread& aT2, TTestDataWithChunk& aC1,  TTestDataWithChunk& aC2);
	void MultipleClientRequestsL(RThread& aT1, RThread& aT);
public:
	// Individual test steps

	void TestSendMessage0001L();
	void TestSendMessage0002L();
	void TestSendMessage0003L();
	void TestSendMessage0004L();	
	void TestSendMessage0005L();
	void TestSendMessage0006L();
	void TestSendRelease0001L();
	void TestSendRelease0001bL();
	void TestSendRelease0001cL();
	void TestSendRelease0001dL();
	void TestSendRelease0002L();
	void TestSendRelease0002bL();
	void TestSendRelease0003L();
	void TestSendRelease0004L();
	void TestSendRelease0004bL();
	void TestSendRelease0005L();
	void TestReceiveMessage0001L();
	void TestReceiveMessage0002L();
	void TestReceiveMessage0002bL();
	void TestReceiveMessage0002cL();
	void TestReceiveMessage0002dL();
	void TestReceiveMessage0002eL();
	void TestReceiveMessage0002fL();
	void TestReceiveMessage0002gL();
	void TestReceiveMessage0002hL();
	void TestReceiveMessage0002iL();
	void TestReceiveMessage0003L();
	void TestReceiveMessage0004L();
	void TestReceiveMessage0004bL();
	void TestReceiveMessage0004cL();
	void TestReceiveMessage0004dL();
	void TestReceiveMessage0004eL();
	void TestReceiveMessage0004fL();
	void TestNotifyNetworkRelease0001L();
	void TestNotifyNetworkRelease0001bL();
	void TestNotifyNetworkRelease0001cL();
	void TestNotifyNetworkRelease0001dL();
	void TestNotifyNetworkRelease0001eL();
	void TestNotifyNetworkRelease0002L();
	void TestNotifyNetworkRelease0003L();
	void TestNotifyNetworkRelease0004L();
	void TestNotifyNetworkRelease0004bL();
	void TestSendMessageNoFdnCheck0001L();
	void TestSendMessageNoFdnCheck0002L();
	void TestSendMessageNoFdnCheck0002bL();
	void TestSendMessageNoFdnCheck0003L();
	void TestSendMessageNoFdnCheck0004L();
	void TestSendMessageNoFdnCheck0005L();
	void TestGetCaps0001L();
	void TestGetCaps0003L();
	void TestReceiveMessageWithTestClient0001L();
	void TestReceiveMessageWithTestClient0002L();
	void TestReceiveMessageWithTestClient0003L();
	void TestReceiveMessageWithTestClient0004L();
	void TestReceiveMessageWithTestClient0005L();
	void TestReceiveMessageWithTestClient0006L();
	void TestReceiveMessageWithTestClient0007L();
	void TestReceiveMessageWithTestClient0008L();
	void TestReceiveMessageWithTestClient0009L();
	void TestReceiveMessageWithTestClient0010L();
	void TestReceiveMessageWithTestClient0011L();
    void TestReceiveMessageWithTestClient0013L();
    void TestReceiveMessageWithTestClient0014L();
	void TestReceiveMessageWithTestClient0016L();
	void TestReceiveMessageWithTestClient0017L();
	void TestAcceptRejectMisuse0001L();
	void TestAcceptRejectMisuse0002L();
	void TestMultipleIncomingUssdMessages0001L();	
	void TestSendMessageDefaultHandlerWithTestClient0001L();
	void TestSendMessageDefaultHandlerWithTestClient0002L();
	void TestSendMessageDefaultHandlerWithTestClient0003L();
	void TestSendMessageDefaultHandlerWithTestClient0004L();
		
private:
	static TInt TestReceiveMessage(TAny* aThreadData);
	static void CompleteMockRequestL(RBuf8& aData, TInt aMsgType, RMockLtsy& aMockLTSY);
	void TestReceiveMessageWithTestClientL(RArray<TUssdClientTestData> & aClientTestData, TBool aRandomLoopingTest=EFalse);
	
	}; // class CCTsyUssdMessagingFU



	
#endif // CCTSYUSSDMESSAGINGFU_H

