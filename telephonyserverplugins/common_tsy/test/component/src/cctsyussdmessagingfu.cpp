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
// The TEFUnit test suite for UssdMessaging in the Common TSY.
// 
//

/**
 @file
*/

#include "cctsyussdmessagingfu.h"
#include <etel.h>
#include <etelmm.h>
#include <et_clsvr.h>
#include <ctsy/mmtsy_names.h>
#include <ctsy/serviceapi/mmtsy_ipcdefs.h>
#include "tmockltsydata.h"
#include <ctsy/serviceapi/gsmerror.h>


CTestSuite* CCTsyUssdMessagingFU::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessage0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessage0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessage0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessage0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessage0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessage0006L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0001bL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0001cL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0001dL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0002bL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0004bL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendRelease0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002bL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002cL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002dL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002eL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002fL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002gL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002hL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0002iL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0004bL);	
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0004cL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0004dL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0004eL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessage0004fL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0001eL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0001bL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0001cL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0001dL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestNotifyNetworkRelease0004bL);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageNoFdnCheck0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageNoFdnCheck0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageNoFdnCheck0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageNoFdnCheck0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageNoFdnCheck0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestGetCaps0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestGetCaps0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0006L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0007L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0008L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0009L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0010L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0011L);
    ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0013L);
    ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0014L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0016L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestReceiveMessageWithTestClient0017L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestAcceptRejectMisuse0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestAcceptRejectMisuse0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestMultipleIncomingUssdMessages0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageDefaultHandlerWithTestClient0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageDefaultHandlerWithTestClient0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageDefaultHandlerWithTestClient0003L);	
	ADD_TEST_STEP_ISO_CPP(CCTsyUssdMessagingFU, TestSendMessageDefaultHandlerWithTestClient0004L);
	END_SUITE;
	
	}


//
// Actual test cases
//


/**
@SYMTestCaseID BA-CTSY-USSDM-USM-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendMessage
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessage0001L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);
	mockData2.SerialiseL(data);
	
 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------

    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessage, data, KErrNotSupported);

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	

	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------

    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessage, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessage, KErrGeneral);

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileUssdMessaging::SendMessage when result is not cached.
 	//-------------------------------------------------------------------------

    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessage, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessage, KErrNone);

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileUssdMessaging::SendMessage
	// from LTSY.
 	//-------------------------------------------------------------------------

    TRequestStatus mockLtsyStatus;

	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessage, KErrNone);
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

 	//-------------------------------------------------------------------------
	// TEST : to increase CMmUssdTsy::ExtFunc coverage
	// covers "if ( ERfsStateInfoInactive == iMmPhone->GetRfStateInfo() && 
	// ( !IsRequestPossibleInOffline( aIpc ) ) )" condition
 	//-------------------------------------------------------------------------

	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	TRfStateInfo rfInfo = ERfsStateInfoInactive;
	TMockLtsyData1<TRfStateInfo> mockData1(rfInfo);
	
	data.Close();
	mockData1.SerialiseL(data);

	iMockLTSY.CompleteL(EMmTsyBootGetRFStatusIPC, KErrNone, data);
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes);

	User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGsmOfflineOpNotAllowed, requestStatus.Int());	

	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging

	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USM-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileUssdMessaging::SendMessage
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileUssdMessaging::SendMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessage0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	TRequestStatus mockLtsyStatus;
	TRequestStatus requestStatus;

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);

 	//-------------------------------------------------------------------------
	// Test cancelling of RMobileUssdMessaging::SendMessage
 	//-------------------------------------------------------------------------
 	
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
 	
	mockData2.SerialiseL(data);
    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessage, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessage, KErrNone, 10);
	
	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes);
    ussdMessaging.CancelAsyncRequest(EMobileUssdMessagingSendMessage);
		
	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

    User::WaitForRequest(requestStatus);
    ASSERT_EQUALS(KErrCancel, requestStatus.Int());

    AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(3); // data, this, ussdMessaging
	
	}


/**
@SYMTestCaseID BA-CTSY-USSDM-USM-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendMessage with bad parameter data
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendMessage with bad parameter data
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessage0003L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

 	//-------------------------------------------------------------------------
	// function parameter

	TName name = _L("Name1");
	TPckg<TName> msgData(name);

	//-------------------------------------------------------------------------
	// Test B: Test passing wrong descriptor size to parameter in
	// RMobileUssdMessaging::SendMessage
 	//-------------------------------------------------------------------------

	TBuf8<1> badSizeDescriptor;

	ussdMessaging.SendMessage(requestStatus, msgData, badSizeDescriptor);

	User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrArgument, requestStatus.Int());	
	
	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging, ussdMessaging

	}


/**
@SYMTestCaseID BA-CTSY-USSDM-USM-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::SendMessage
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::SendMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessage0004L()
	{

					
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	RMobileUssdMessaging ussdMessaging1;
	ret = ussdMessaging1.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging1);

	RMobileUssdMessaging ussdMessaging2;
	ret = ussdMessaging2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// parameters for first function

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes1;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes1(attributes1);
	
	attributes1.iFlags  = flags;
	attributes1.iFormat = format;
	attributes1.iType   = type;
	attributes1.iDcs    = dcs;

	TName name1 = _L("Name1");
	TPckg<TName> msgData1(name1);

 	//-------------------------------------------------------------------------
	// parameters for second function

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes2;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes2(attributes2);

	flags  = 10;
	format = RMobileUssdMessaging::EFormatPackedString;
	type   = RMobileUssdMessaging::EUssdMOAcknowledgement;
	dcs    = 20;
	
	attributes2.iFlags  = flags;
	attributes2.iFormat = format;
	attributes2.iType   = type;
	attributes2.iDcs    = dcs;
	
	TName name2 = _L("Name2");
	TPckg<TName> msgData2(name2);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name1, attributes1);
	mockData2.SerialiseL(data);

	RBuf8 data3;
	CleanupClosePushL(data3);

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	    
	RMobileUssdMessaging::TGsmUssdMessageData name;

	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData3(completeName, completeAttributes);
	mockData3.SerialiseL(data3);

	//-------------------------------------------------------------------------
	// Test A: Test multiple clients requesting RMobileUssdMessaging::SendMessage
 	//-------------------------------------------------------------------------
	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data3);

	ussdMessaging1.ReceiveMessage(requestStatus1, name, msgAttributes);   
	User::WaitForRequest(requestStatus1);        
	ret = requestStatus1.Int();
	ASSERT_EQUALS(KErrNone, ret);
	ussdMessaging1.AcceptIncomingDialogue();
	
    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessage, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessage, KErrNone, 10);
	ussdMessaging1.SendMessage(requestStatus1, msgData1, msgAttributes1);

	ussdMessaging2.SendMessage(requestStatus2, msgData2, msgAttributes2);
	
    User::WaitForRequest(requestStatus1);        
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());	

    User::WaitForRequest(requestStatus2);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrServerBusy, requestStatus2.Int());	

	CleanupStack::PopAndDestroy(7, this); // phone2, telServer2, data, this, 
	                                      // ussdMessaging1, ussdMessaging2, data3

	}


/**
@SYMTestCaseID BA-CTSY-USSDM-USM-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendMessage with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendMessage and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessage0005L()
	{


	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);

	//-------------------------------------------------------------------------
	// Test A: Test timeout of RMobileUssdMessaging::SendMessage
 	//-------------------------------------------------------------------------

	mockData2.SerialiseL(data);
    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessage, data);

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());	

	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging

	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USM-0006
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendMessage DefaultHandler
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendMessage DefaultHandler
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessage0006L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);
	mockData2.SerialiseL(data);
	
 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------
	
	
    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageDefaultHandler, data, KErrNotSupported);

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes, RMobileUssdMessaging::ETransferToDefaultHandler);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	
	
	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------

    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageDefaultHandler, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageDefaultHandler, KErrGeneral);

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes, RMobileUssdMessaging::ETransferToDefaultHandler);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	
	
 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileUssdMessaging::SendMessage when result is not cached.
 	//-------------------------------------------------------------------------

    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageDefaultHandler, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageDefaultHandler, KErrNone);

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes, RMobileUssdMessaging::ETransferToDefaultHandler);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	
 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileUssdMessaging::SendMessage
	// from LTSY.
 	//-------------------------------------------------------------------------

    TRequestStatus mockLtsyStatus;

	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageDefaultHandler, KErrNone);
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

 	//-------------------------------------------------------------------------
	// TEST : to increase CMmUssdTsy::ExtFunc coverage
	// covers "if ( ERfsStateInfoInactive == iMmPhone->GetRfStateInfo() && 
	// ( !IsRequestPossibleInOffline( aIpc ) ) )" condition
 	//-------------------------------------------------------------------------

	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	TRfStateInfo rfInfo = ERfsStateInfoInactive;
	TMockLtsyData1<TRfStateInfo> mockData1(rfInfo);
	
	data.Close();
	mockData1.SerialiseL(data);

	iMockLTSY.CompleteL(EMmTsyBootGetRFStatusIPC, KErrNone, data);
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes);

	User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGsmOfflineOpNotAllowed, requestStatus.Int());	
	
	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging

	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendRelease
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0001L()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Release the dialogue
    //------------------------------------------------------------------------- 
    
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
		
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
	
	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
		
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;
	
	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);	
	mockData1.SerialiseL(data);
	
 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileUssdMessaging::SendRelease when result is not cached.
 	//-------------------------------------------------------------------------
	
	iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendRelease, KErrNone, data);
	ussdMessaging.SendRelease(requestStatus, pckgSendV3);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	
	ASSERT_EQUALS(opCode, sendV3.iOpCode);	
	ASSERT_EQUALS(0, completeSendV3.iAdditionalInfo.Compare(sendV3.iAdditionalInfo));
	
	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileUssdMessaging::SendRelease
	// from LTSY.
 	//-------------------------------------------------------------------------

	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendRelease, KErrNone, data);
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());
	
    CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging, dataTest
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0001b
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendRelease
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0001bL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Release the dialogue
    //------------------------------------------------------------------------- 
	
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;

	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);

	mockData1.SerialiseL(data);

 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------

    iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease, KErrNotSupported);
	ussdMessaging.SendRelease(requestStatus, pckgSendV3);
	
    User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());

 	//-------------------------------------------------------------------------

	iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendRelease, KErrNone, data);
	ussdMessaging.SendRelease(requestStatus, pckgSendV3);
	
    User::WaitForRequest(requestStatus);        

	ASSERT_EQUALS(KErrNone, requestStatus.Int());	

	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(3); // data, this, ussdMessaging
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0006
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendRelease without USSD session in progress
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0001cL()
	{
		
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
		
    TRequestStatus mockLtsyStatus;
	TRequestStatus requestStatus;
		
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
		
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;

	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
		
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;
	
	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);	
	mockData1.SerialiseL(data);
		
	//-------------------------------------------------------------------------
	// TEST A: failure because there is no USSD session in progress
 	//-------------------------------------------------------------------------
	
	ussdMessaging.SendRelease(requestStatus, pckgSendV3);
		
    User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrDisconnected, requestStatus.Int());
		
	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0001d
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for failure on completion of pending comRMobileUssdMessaging::SendRelease request from LTSY->CTSY
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0001dL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Release the dialogue
    //------------------------------------------------------------------------- 
    
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
		
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
	
	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
		
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;
	
	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);	
	mockData1.SerialiseL(data);
	
	//-------------------------------------------------------------------------
	// Failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------
	
    iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendRelease, KErrGeneral, data);
	ussdMessaging.SendRelease(requestStatus, pckgSendV3);
		
    User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());
	
    CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging, dataTest
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileUssdMessaging::SendRelease
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileUssdMessaging::SendRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0002L()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Release the dialogue
    //------------------------------------------------------------------------- 
	
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;

	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);

	mockData1.SerialiseL(data);

 	//-------------------------------------------------------------------------
	// Test cancelling of RMobileUssdMessaging::SendRelease
 	//-------------------------------------------------------------------------
 	
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
 	
	iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendRelease, KErrNone, data, 10);

	ussdMessaging.SendRelease(requestStatus, pckgSendV3);
    ussdMessaging.CancelAsyncRequest(EMobileUssdMessagingSendRelease);
		
	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

    User::WaitForRequest(requestStatus);
    ASSERT_EQUALS(KErrCancel, requestStatus.Int());

    AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(3); // data, this, ussdMessaging
	
	}


/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0002b
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileUssdMessaging::SendRelease
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileUssdMessaging::SendRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0002bL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	//-------------------------------------------------------------------------
	// Setup: This block is taken from TestReceiveMessage0001L. All we want to
	// do is to initiate a dialogue and then end it. Once the dialogue has ended
	// then we can call Accept and Reject.
	//------------------------------------------------------------------------- 
	
	TRequestStatus requestStatus;
	TRequestStatus mockLtsyStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMTRequest;
	TUint8 dcs = 200;

	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);
	mockData2.SerialiseL(data);
	
	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

	ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
	User::WaitForRequest(requestStatus);        
	ret = requestStatus.Int();
	ASSERT_EQUALS(KErrNone, ret);
			
	//-------------------------------------------------------------------------
	// Release the dialogue
	//------------------------------------------------------------------------- 
	
	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;

	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);

	mockData1.SerialiseL(data);
	
	//-------------------------------------------------------------------------

	ussdMessaging.SendRelease(requestStatus, pckgSendV3);
	
	ussdMessaging.AcceptIncomingDialogue();		    
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrDisconnected, requestStatus.Int());

	CleanupStack::PopAndDestroy(3); // data, this, ussdMessaging
	
	}



/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendRelease with bad parameter data
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendRelease with bad parameter data
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0003L()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Release the dialogue
    //------------------------------------------------------------------------- 
    
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;

	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);

	mockData1.SerialiseL(data);

	//-------------------------------------------------------------------------
	// Test B: Test passing wrong descriptor size to parameter in
	// RMobileUssdMessaging::SendRelease
 	//-------------------------------------------------------------------------

	TBuf8<1> badSizeDescriptor;
	ussdMessaging.SendRelease(requestStatus, badSizeDescriptor);
	User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrArgument, requestStatus.Int());
	
	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging
	
	}


/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::SendRelease
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::SendRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0004L()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Release the dialogue
    //------------------------------------------------------------------------- 

    // Open second client
    RTelServer telServer2;
	ret = telServer2.Connect();
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(telServer2);

    RMobilePhone phone2;
    ret = phone2.Open(telServer2,KMmTsyPhoneName);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(phone2);
    
	RMobileUssdMessaging ussdMessaging2;
	ret = ussdMessaging2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

 	//-------------------------------------------------------------------------
	// parameters for first function
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV31;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV31(sendV31);
	
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV31;

	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV31.iOpCode         = opCode;
	completeSendV31.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData11(completeSendV31);

 	//-------------------------------------------------------------------------
	// parameters for second function
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV32;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV32(sendV32);
	
	//-------------------------------------------------------------------------
	// Test A: Test multiple clients requesting RMobileUssdMessaging::SendRelease
 	//-------------------------------------------------------------------------
	
	mockData11.SerialiseL(data);
	iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendRelease, KErrNone, data, 10);
	ussdMessaging.SendRelease(requestStatus, pckgSendV31);

	ussdMessaging2.SendRelease(requestStatus2, pckgSendV32);
	
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	

    User::WaitForRequest(requestStatus2);  
	ASSERT_EQUALS(KErrAccessDenied, requestStatus2.Int());	
	ASSERT_EQUALS(opCode, sendV31.iOpCode);	
	ASSERT_EQUALS(0, sendV31.iAdditionalInfo.Compare(KAdditionalInfo1));	
	 
	AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(6, this); // data, ussdMessaging2, ussdMessaging,
										  // phone2, telServer2, this
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0004a
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::SendRelease
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::SendRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0004bL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();
	
	RBuf8 data;
	CleanupClosePushL(data);
	
	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	//-------------------------------------------------------------------------
	// Setup: This block is taken from TestReceiveMessage0001L. All we want to
	// do is to initiate a dialogue and then end it. Once the dialogue has ended
	// then we can call Accept and Reject.
	//------------------------------------------------------------------------- 
	
	TRequestStatus requestStatus;
	TRequestStatus mockLtsyStatus;
	
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMTRequest;
	TUint8 dcs = 200;
	
	//-------------------------------------------------------------------------
	// function parameters
	
	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;
	
	//-------------------------------------------------------------------------
	
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;
	
	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");
	
	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);
	mockData2.SerialiseL(data);
	
	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);
	
	ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
	User::WaitForRequest(requestStatus);        
	ret = requestStatus.Int();
	ASSERT_EQUALS(KErrNone, ret);
	ussdMessaging.AcceptIncomingDialogue();
	
	AssertMockLtsyStatusL();
	
	//-------------------------------------------------------------------------
	// Try to release the dialogue
	//------------------------------------------------------------------------- 
	
	// Open second client
	RTelServer telServer2;
	ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);
	
	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);
	
	RMobileUssdMessaging ussdMessaging2;
	ret = ussdMessaging2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging2);
	
	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
		
	//-------------------------------------------------------------------------
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
	
	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
		
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;
	
	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);	
	mockData1.SerialiseL(data);
	
	//-------------------------------------------------------------------------
	// Test: The second client requests RMobileUssdMessaging::SendRelease
	//-------------------------------------------------------------------------
	
	iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendRelease, KErrDisconnected, data, 10);
	
	TRequestStatus requestStatus2;
	ussdMessaging2.SendRelease(requestStatus2, pckgSendV3);
	
	User::WaitForRequest(requestStatus2);  
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrDisconnected, requestStatus2.Int());	
		 
	CleanupStack::PopAndDestroy(6, this); // data, ussdMessaging2, ussdMessaging,
										  // phone2, telServer2, this
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USR-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendRelease with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendRelease and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendRelease0005L()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Release the dialogue
    //------------------------------------------------------------------------- 

 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
	//-------------------------------------------------------------------------
	// Test A: Test timeout of RMobileUssdMessaging::SendRelease
 	//-------------------------------------------------------------------------

	iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease);
	ussdMessaging.SendRelease(requestStatus, pckgSendV3);
	
    User::WaitForRequest(requestStatus); 
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());

	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging
	}


/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::ReceiveMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0001L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;
	    
    TRequestStatus mockLtsyStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);
	
 	//-------------------------------------------------------------------------
	// TEST A: Successful completion request of
	// RMobileUssdMessaging::ReceiveMessage when result is not cached.
 	//------------------------------------------------------------------------- 
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

	ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);	
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());
	
	ASSERT_EQUALS(flags , attributes.iFlags );
	ASSERT_EQUALS(format, attributes.iFormat);
	ASSERT_EQUALS(type  , attributes.iType  );
	ASSERT_EQUALS(dcs   , attributes.iDcs   );

	ASSERT_EQUALS(0, name.Compare(completeName));

 
	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging
	}


/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileUssdMessaging::ReceiveMessage (including via calling Close)
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;
	TRequestStatus mockLtsyStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);

 	//-------------------------------------------------------------------------
	// Test cancelling of RMobileUssdMessaging::ReceiveMessage
 	//-------------------------------------------------------------------------
 	
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
 	
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data, 10);
	
	ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);
    ussdMessaging.CancelAsyncRequest(EMobileUssdMessagingReceiveMessage);
		
	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

    User::WaitForRequest(requestStatus);
    ASSERT_EQUALS(KErrCancel, requestStatus.Int());

 	//-------------------------------------------------------------------------
	// Test Close after calling RMobileUssdMessaging::ReceiveMessage
 	//-------------------------------------------------------------------------
 
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
 
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data, 10);

	ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);
    ussdMessaging.Close();

    User::After(2 * 1000000);
    
    AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(3); // data, this, ussdMessaging
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002b
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test cancelling of RMobileUssdMessaging::ReceiveMessage with multiple interested clients.
@SYMTestPriority High
@SYMTestActions Two clients present, first client calls Cancel before message received.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002bL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussd1;
	RMobileUssdMessaging ussd2;
	TInt ret1 = ussd1.Open(iPhone);
	TInt ret2 = ussd2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);
	ASSERT_EQUALS(KErrNone, ret2);
	CleanupClosePushL(ussd1);
	CleanupClosePushL(ussd2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

 	//-------------------------------------------------------------------------
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);	

 	//-------------------------------------------------------------------------
	// TEST: Two clients have called ReceiveMessage. No session in progress.
	// First client calls CancelAsyncRequest with value EMobileUssdMessagingReceiveMessage.
	// Request completes with TRequestStatus set to KErrCancel. 
	// At same time a message received from network. This is offered to second client.
 	//-------------------------------------------------------------------------
	ussd1.ReceiveMessage(requestStatus1 ,name, msgAttributes);
	ussd2.ReceiveMessage(requestStatus2 ,name, msgAttributes);	
    ussd1.CancelAsyncRequest(EMobileUssdMessagingReceiveMessage);
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);
	User::WaitForAnyRequest();
	ASSERT_EQUALS(KErrCancel, requestStatus1.Int());

	User::WaitForAnyRequest();
	// Second client receives offer of dialogue.
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());
	
	CleanupStack::PopAndDestroy(4, this); //  this, data, ussd1, ussd2
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002c
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test cancelling of RMobileUssdMessaging::ReceiveMessage with multiple interested clients.
@SYMTestPriority High
@SYMTestActions Two clients present, first client calls Cancel AFTER accepting dialogue.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002cL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussd1;
	RMobileUssdMessaging ussd2;
	TInt ret1 = ussd1.Open(iPhone);
	TInt ret2 = ussd2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);
	ASSERT_EQUALS(KErrNone, ret2);
	CleanupClosePushL(ussd1);
	CleanupClosePushL(ussd2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

 	//-------------------------------------------------------------------------
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);	

	//-------------------------------------------------------------------------
	// TEST : Two clients have called ReceiveMessage. 
	// First client accepts a dialogue THEN calls cancel.
	// Request completes with TRequestStatus set to KErrCancel.
	//-------------------------------------------------------------------------
	ussd1.ReceiveMessage(requestStatus1 ,name, msgAttributes);
	ussd2.ReceiveMessage(requestStatus2 ,name, msgAttributes);	
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);
    User::WaitForRequest(requestStatus1);
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());
	ussd1.AcceptIncomingDialogue();
	User::After(1000000);	
	ussd1.CancelAsyncRequest(EMobileUssdMessagingReceiveMessage);	
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());
	
	CleanupStack::PopAndDestroy(4, this); //  this, data, ussd1, ussd2
	}
	
/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002d
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test cancelling of RMobileUssdMessaging::ReceiveMessage with multiple interested clients.
@SYMTestPriority High
@SYMTestActions Two clients present, second client calls Cancel.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002dL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussd1;
	RMobileUssdMessaging ussd2;
	TInt ret1 = ussd1.Open(iPhone);
	TInt ret2 = ussd2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);
	ASSERT_EQUALS(KErrNone, ret2);
	CleanupClosePushL(ussd1);
	CleanupClosePushL(ussd2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

 	//-------------------------------------------------------------------------
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);	

	//-------------------------------------------------------------------------
	// TEST: Two clients have called ReceiveMessage. First accepts a dialogue.
	// SECOND client calls CancelAsyncRequest with value EMobileUssdMessagingReceiveMessage.
	// Request completes with TRequestStatus set to KErrCancel.
	//-------------------------------------------------------------------------
	ussd1.ReceiveMessage(requestStatus1 ,name, msgAttributes);
	ussd2.ReceiveMessage(requestStatus2 ,name, msgAttributes);	
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);
    User::WaitForRequest(requestStatus1);
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());
	ussd1.AcceptIncomingDialogue();
	User::After(1000000);	
	ussd2.CancelAsyncRequest(EMobileUssdMessagingReceiveMessage);
 	
    User::WaitForRequest(requestStatus2);
	// Second client receives offer of dialogue.
	ASSERT_EQUALS(KErrCancel, requestStatus2.Int());	
	ussd2.AcceptIncomingDialogue();
	
	CleanupStack::PopAndDestroy(4, this); //  this, data, ussd1, ussd2
	}

	
/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002e
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test cancelling of RMobileUssdMessaging::ReceiveMessage with multiple interested clients.
@SYMTestPriority High
@SYMTestActions Two clients present, second client calls Close().
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002eL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussd1;
	RMobileUssdMessaging ussd2;
	TInt ret1 = ussd1.Open(iPhone);
	TInt ret2 = ussd2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);
	ASSERT_EQUALS(KErrNone, ret2);
	CleanupClosePushL(ussd1);
	CleanupClosePushL(ussd2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

 	//-------------------------------------------------------------------------
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);	

	//-------------------------------------------------------------------------
	// TEST: Two clients have called ReceiveMessage. No session in progress.
	// Second client calls Close(). First client is offered a dialogue and accepts as normal.
	//-------------------------------------------------------------------------
	ussd1.ReceiveMessage(requestStatus1 ,name, msgAttributes);
	ussd2.ReceiveMessage(requestStatus2 ,name, msgAttributes);	
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);
	ussd1.Close();
    User::WaitForAnyRequest();
    ASSERT_EQUALS(KErrNone, requestStatus2.Int());
	ussd2.RejectIncomingDialogue();
	
	
	CleanupStack::PopAndDestroy(4, this); //  this, data, ussd1, ussd2
	}	

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002f
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test cancelling of RMobileUssdMessaging::ReceiveMessage with multiple interested clients.
@SYMTestPriority High
@SYMTestActions Cancel one of the clients in the middle of a delivery. 
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002fL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussd1;
	RMobileUssdMessaging ussd2;
	RMobileUssdMessaging ussd3;
	TInt ret1 = ussd1.Open(iPhone);
	TInt ret2 = ussd2.Open(iPhone);
	TInt ret3 = ussd3.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);
	ASSERT_EQUALS(KErrNone, ret2);
	ASSERT_EQUALS(KErrNone, ret3);
	CleanupClosePushL(ussd1);
	CleanupClosePushL(ussd2);
	CleanupClosePushL(ussd3);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;
	TRequestStatus requestStatus3;

	TRequestStatus mockLtsyStatus;

 	//-------------------------------------------------------------------------
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);	


	//-------------------------------------------------------------------------
	// TEST : Test to cancel one of the clients in the middle of a delivery. 
	// 1) Have three clients. 2) Request offered to 1st. 3) 2nd client then cancels.
	// 4) 1st client rejects request. 5) 3rd client should now be offered request.
	//-------------------------------------------------------------------------
	ret1 = ussd1.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);

	ussd1.ReceiveMessage(requestStatus1 ,name, msgAttributes);
	ussd2.ReceiveMessage(requestStatus2 ,name, msgAttributes);	
	ussd3.ReceiveMessage(requestStatus3 ,name, msgAttributes);
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    User::WaitForAnyRequest();
	AssertMockLtsyStatusL();

	// Offered to first client
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());	
	
	// Cancel second client and First client rejects dialogue
	ussd2.CancelAsyncRequest(EMobileUssdMessagingReceiveMessage);
	User::After(1000000);
	ussd1.RejectIncomingDialogue();
	User::WaitForRequest(requestStatus2);
	ASSERT_EQUALS(KErrCancel, requestStatus2.Int());

	// dialogue offered to third client
	ussd3.AcceptIncomingDialogue();
	User::WaitForRequest(requestStatus3);
	ASSERT_EQUALS(KErrNone, requestStatus3.Int());

	CleanupStack::PopAndDestroy(5, this); //  this, data, ussd1, ussd2, ussd3
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002g
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test cancelling of RMobileUssdMessaging::ReceiveMessage with multiple interested clients.
@SYMTestPriority High
@SYMTestActions Have two clients. Request offered to 1st. 1st client cancels. 2nd client should now be offered request.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002gL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussd1;
	RMobileUssdMessaging ussd2;
	TInt ret1 = ussd1.Open(iPhone);
	TInt ret2 = ussd2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);
	ASSERT_EQUALS(KErrNone, ret2);
	CleanupClosePushL(ussd1);
	CleanupClosePushL(ussd2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

	TRequestStatus mockLtsyStatus;

 	//-------------------------------------------------------------------------
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);	


	//-------------------------------------------------------------------------
	// TEST : Have two clients. 1) Request offered to 1st. 2) 1st client cancels.
	// 3) 2nd client should now be offered request.
	//-------------------------------------------------------------------------	
	ussd1.ReceiveMessage(requestStatus1 ,name, msgAttributes);
	ussd2.ReceiveMessage(requestStatus2 ,name, msgAttributes);	
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    User::WaitForAnyRequest();
	AssertMockLtsyStatusL();

	// Offered to first client
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());	
	
	// Cancel first client	
	ussd2.CancelAsyncRequest(EMobileUssdMessagingReceiveMessage);
	
	User::WaitForRequest(requestStatus2);
	ASSERT_EQUALS(KErrCancel, requestStatus2.Int());
		
	// dialogue offered to second client
	ussd1.RejectIncomingDialogue();	
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());
	
	CleanupStack::PopAndDestroy(4, this); //  this, data, ussd1, ussd2
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002h
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test cancelling of RMobileUssdMessaging::ReceiveMessage with multiple interested clients.
@SYMTestPriority High
@SYMTestActions Cancel one of the clients in the middle of a delivery. 
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002hL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussd1;
	RMobileUssdMessaging ussd2;
	RMobileUssdMessaging ussd3;
	TInt ret1 = ussd1.Open(iPhone);
	TInt ret2 = ussd2.Open(iPhone);
	TInt ret3 = ussd3.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);
	ASSERT_EQUALS(KErrNone, ret2);
	ASSERT_EQUALS(KErrNone, ret3);
	CleanupClosePushL(ussd1);
	CleanupClosePushL(ussd2);
	CleanupClosePushL(ussd3);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;
	TRequestStatus requestStatus3;

	TRequestStatus mockLtsyStatus;

 	//-------------------------------------------------------------------------
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);	

	//-------------------------------------------------------------------------
	// TEST : Test to cancel one of the clients in the middle of a delivery. 
	// 1) Have three clients. 2) Request offered to 1st. 3) 1nd client then cancels.
	// 4) 2st client accepts request. The cancel is processed prior to 2nd client accepting.
	//-------------------------------------------------------------------------
	ret1 = ussd1.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);

	ussd1.ReceiveMessage(requestStatus1 ,name, msgAttributes);
	ussd2.ReceiveMessage(requestStatus2 ,name, msgAttributes);	
	ussd3.ReceiveMessage(requestStatus3 ,name, msgAttributes);

	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);
  
	User::WaitForAnyRequest();	
	// Offered to first client... 
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());	

	// Client cancels.
    ussd1.CancelAsyncRequest(EMobileUssdMessagingReceiveMessage);
	
	// Offered to second client. Client accepts.
    ussd2.AcceptIncomingDialogue();
	
    AssertMockLtsyStatusL();
	
    User::WaitForAnyRequest();
    ASSERT_EQUALS(KErrNone, requestStatus2.Int());  
 
	CleanupStack::PopAndDestroy(5, this); //  this, data, ussd1, ussd2, ussd3
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0002i
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support of RMobileUssdMessaging::ReceiveMessage for consecutive reception of two USSD dialogues/messages by two different clients. 
@SYMTestPriority High
@SYMTestActions Two clients present, second client calls Cancel.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0002iL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussd1;
	RMobileUssdMessaging ussd2;
	TInt ret1 = ussd1.Open(iPhone);
	TInt ret2 = ussd2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret1);
	ASSERT_EQUALS(KErrNone, ret2);
	CleanupClosePushL(ussd1);
	CleanupClosePushL(ussd2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

 	//-------------------------------------------------------------------------
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMTNotify;
	TUint8 dcs = 200;

	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);	

	//-------------------------------------------------------------------------
	// TEST: The first client accepts the first USSD notification.
	// The SECOND client accepts the second USSD notification.
	//-------------------------------------------------------------------------
	
	//first notification
	ussd1.ReceiveMessage(requestStatus1, name, msgAttributes);
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    User::WaitForRequest(requestStatus1);
	AssertMockLtsyStatusL();

	ASSERT_EQUALS(KErrNone, requestStatus1.Int());	
	ussd1.AcceptIncomingDialogue();
	
	//second notification
	ussd2.ReceiveMessage(requestStatus2, name, msgAttributes);	
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    User::WaitForRequest(requestStatus2);
	AssertMockLtsyStatusL();

	ASSERT_EQUALS(KErrNone, requestStatus2.Int());
	ussd2.AcceptIncomingDialogue();
	
	CleanupStack::PopAndDestroy(4, this); //  this, data, ussd1, ussd2
	}


/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::ReceiveMessage with bad parameter data
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::ReceiveMessage with bad parameter data
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0003L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);

	//-------------------------------------------------------------------------
	// Test C: Test passing out of bounds parameters to
	// RPacketQoS::SetProfileParameters
 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);

	TBuf8<1> badName;
	
	ussdMessaging.ReceiveMessage(requestStatus, badName, msgAttributes);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrArgument, requestStatus.Int());

	//-------------------------------------------------------------------------
	// Test B: Test passing wrong descriptor size to parameter in
	// RMobileUssdMessaging::ReceiveMessage
 	//-------------------------------------------------------------------------

	TBuf8<KMaxName> name;
	TBuf8<1> badSizeDescriptor;

	ussdMessaging.ReceiveMessage(requestStatus, name, badSizeDescriptor);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();	
	ASSERT_EQUALS(KErrArgument, requestStatus.Int());
	
	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging

	}


/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0004L()
	{

    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();
	
    RThread thread1;
	RThread thread2;
		
	// Populate the test data with a handle to a memory chunk
	RChunk chunk1;
    RChunk chunk2;
	TInt *chunkValue1; 
	TInt *chunkValue2;
	
	chunk1.CreateLocal((TBool)4, (TInt) 4);
    chunk2.CreateLocal((TBool)4, (TInt) 4);
	
	chunkValue1 = (TInt *)chunk1.Base();   
    chunkValue2 = (TInt *)chunk2.Base(); 
    
	*chunkValue1 = 0;  
    *chunkValue2 = 0;  
	
	TTestDataWithChunk testData1(chunk1);
    TTestDataWithChunk testData2(chunk2);

	TInt err = CreateClients( thread1, thread2, testData1, testData2); 
	ASSERT_EQUALS(KErrNone, err);
	
	//-------------------------------------------------------------------------
	// Test : Test multiple clients requesting RMobileUssdMessaging::ReceiveMessage
	//			First client accepts, not offered to second client.
 	//-------------------------------------------------------------------------

    // Create the threads
    testData1.iAcceptDialogue = ETrue;
    testData1.iAfterTimePeriod = 0;

    testData2.iAcceptDialogue = ETrue;
    testData2.iAfterTimePeriod = 0;
	
	MultipleClientRequestsL(thread1, thread2);

	thread1.Close();
	thread2.Close();
	CleanupStack::PopAndDestroy(this); // this
	}	

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0004b
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0004bL()
	{

    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();
	
    RThread thread1;
	RThread thread2;
		
	// Populate the test data with a handle to a memory chunk
	RChunk chunk1;
    RChunk chunk2;
	TInt *chunkValue1; 
	TInt *chunkValue2;
	
	chunk1.CreateLocal((TBool)4, (TInt) 4);
    chunk2.CreateLocal((TBool)4, (TInt) 4);
	
	chunkValue1 = (TInt *)chunk1.Base();   
    chunkValue2 = (TInt *)chunk2.Base(); 
    
	*chunkValue1 = 0;  
    *chunkValue2 = 0;  
	
	TTestDataWithChunk testData1(chunk1);
    TTestDataWithChunk testData2(chunk2);

	TInt err = CreateClients( thread1, thread2, testData1, testData2); 
	ASSERT_EQUALS(KErrNone, err);

	//-------------------------------------------------------------------------
	// Test : Test multiple clients requesting RMobileUssdMessaging::ReceiveMessage
	//		  First client rejects, second accepts.
 	//-------------------------------------------------------------------------

    // Create the threads
    testData1.iAcceptDialogue = EFalse;
    testData1.iAfterTimePeriod = 0;

    testData2.iAcceptDialogue = ETrue;
    testData2.iAfterTimePeriod = 0;
	
	MultipleClientRequestsL(thread1, thread2);

	thread1.Close();
	thread2.Close();
	CleanupStack::PopAndDestroy(this); // this
	}	

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0004c
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0004cL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();
	
    RThread thread1;
	RThread thread2;
		
	// Populate the test data with a handle to a memory chunk
	RChunk chunk1;
    RChunk chunk2;
	TInt *chunkValue1; 
	TInt *chunkValue2;
	
	chunk1.CreateLocal((TBool)4, (TInt) 4);
    chunk2.CreateLocal((TBool)4, (TInt) 4);
	
	chunkValue1 = (TInt *)chunk1.Base();   
    chunkValue2 = (TInt *)chunk2.Base(); 
    
	*chunkValue1 = 0;  
    *chunkValue2 = 0;  
	
	TTestDataWithChunk testData1(chunk1);
    TTestDataWithChunk testData2(chunk2);

	
	TInt err = CreateClients( thread1, thread2, testData1, testData2); 
	ASSERT_EQUALS(KErrNone, err);

	//-------------------------------------------------------------------------
	// Test : Test multiple clients requesting ReceiveMessage, 
	// first client fails to accept in time. Second client accepts before time-out.
 	//-------------------------------------------------------------------------
	
	testData1.iAcceptDialogue = ETrue;
	testData1.iAfterTimePeriod = 6; //Time-out

	testData2.iAcceptDialogue = ETrue;
	testData2.iAfterTimePeriod = 2; //Time-in
	
    MultipleClientRequestsL(thread1, thread2);

	thread1.Close();
	thread2.Close();
	CleanupStack::PopAndDestroy(this); // this
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0004d
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0004dL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();
	
    RThread thread1;
	RThread thread2;
		
	// Populate the test data with a handle to a memory chunk
	RChunk chunk1;
    RChunk chunk2;
	TInt *chunkValue1; 
	TInt *chunkValue2;
	
	chunk1.CreateLocal((TBool)4, (TInt) 4);
    chunk2.CreateLocal((TBool)4, (TInt) 4);
	
	chunkValue1 = (TInt *)chunk1.Base();   
    chunkValue2 = (TInt *)chunk2.Base(); 
    
	*chunkValue1 = 0;  
    *chunkValue2 = 0;  
	
	TTestDataWithChunk testData1(chunk1);
    TTestDataWithChunk testData2(chunk2);

	TInt err = CreateClients( thread1, thread2, testData1, testData2); 
	ASSERT_EQUALS(KErrNone, err);

	//-------------------------------------------------------------------------
	// Test: Test multiple clients requesting ReceiveMessage, 
	// Both clients reject, no default/catch-all
 	//-------------------------------------------------------------------------
	
	testData1.iAcceptDialogue = EFalse;
	testData1.iAfterTimePeriod = 0;

	testData1.iAcceptDialogue = EFalse;
	testData2.iAfterTimePeriod = 0;

    MultipleClientRequestsL(thread1, thread2);

	thread1.Close();
	thread2.Close();
	CleanupStack::PopAndDestroy(this); // this
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0004e
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0004eL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();
	
    RThread thread1;
	RThread thread2;
		
	// Populate the test data with a handle to a memory chunk
	RChunk chunk1;
    RChunk chunk2;
	TInt *chunkValue1; 
	TInt *chunkValue2;
	
	chunk1.CreateLocal((TBool)4, (TInt) 4);
    chunk2.CreateLocal((TBool)4, (TInt) 4);
	
	chunkValue1 = (TInt *)chunk1.Base();   
    chunkValue2 = (TInt *)chunk2.Base(); 
    
	*chunkValue1 = 0;  
    *chunkValue2 = 0;  
	
	TTestDataWithChunk testData1(chunk1);
    TTestDataWithChunk testData2(chunk2);

	
	TInt err = CreateClients( thread1, thread2, testData1, testData2); 
	ASSERT_EQUALS(KErrNone, err);

	//-------------------------------------------------------------------------
	// Test: Test multiple clients requesting ReceiveMessage, 
	// First client times out before accepting (too late), Second client rejects message.
 	//-------------------------------------------------------------------------
	
	testData1.iAcceptDialogue = ETrue;
	testData1.iAfterTimePeriod = 0;
	testData2.iAcceptDialogue = EFalse;
	testData2.iAfterTimePeriod = 2;
	
	MultipleClientRequestsL(thread1, thread2);
	thread1.Close();
	thread2.Close();
	CleanupStack::PopAndDestroy(this); // this
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0004f
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessage0004fL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();
	
    RThread thread1;
	RThread thread2;
		
	// Populate the test data with a handle to a memory chunk
	RChunk chunk1;
    RChunk chunk2;
	TInt *chunkValue1; 
	TInt *chunkValue2;
	
	chunk1.CreateLocal((TBool)4, (TInt) 4);
    chunk2.CreateLocal((TBool)4, (TInt) 4);
	
	chunkValue1 = (TInt *)chunk1.Base();   
    chunkValue2 = (TInt *)chunk2.Base(); 
    
	*chunkValue1 = 0;  
    *chunkValue2 = 0;  
	
	TTestDataWithChunk testData1(chunk1);
    TTestDataWithChunk testData2(chunk2);

    testData1.iAcceptDialogue = EFalse;
    testData1.iAfterTimePeriod = 0;
    testData2.iAcceptDialogue = ETrue;
    testData2.iAfterTimePeriod = 0;
        
	TInt err = CreateClients( thread1, thread2, testData1, testData2); 
	ASSERT_EQUALS(KErrNone, err);

	//-------------------------------------------------------------------------
	// Test : Test multiple clients requesting RMobileUssdMessaging::ReceiveMessage
	//		  Message completes with an error. First client rejects, second accepts.
 	//-------------------------------------------------------------------------
	RBuf8 data;
	
	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;
    
    thread1.Logon(requestStatus1);
    thread2.Logon(requestStatus2);
    thread1.Resume();
    thread2.Resume();
    	
   //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrCorrupt, data);

	User::WaitForAnyRequest();
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());
			
	data.Close();
	thread1.Close();
	thread2.Close();
	CleanupStack::PopAndDestroy(this); // this
	}	

	
/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0001L()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Terminate the dialogue
    //------------------------------------------------------------------------- 
    
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 attrV1;
	RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pckgAttrV1(attrV1);
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
	
	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);
	mockData1.SerialiseL(data);

	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttrV1;
	completeAttrV1.iFlags = RMobileUssdMessaging::KUssdDataFormat | RMobileUssdMessaging::KUssdMessageType;
	completeAttrV1.iFormat = RMobileUssdMessaging::EFormatPackedString;
	completeAttrV1.iType = RMobileUssdMessaging::EUssdMOAcknowledgement;
	
	TMockLtsyData2< RMobilePhone::TMobilePhoneSendSSRequestV3, 
					RMobileUssdMessaging::TMobileUssdAttributesV1 > 
					mockData2Release(completeSendV3,completeAttrV1);
	
	RBuf8 data2;
	CleanupClosePushL(data2);
	mockData2Release.SerialiseL(data2);
		
 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileUssdMessaging::NotifyNetworkRelease when result is not cached.
 	//-------------------------------------------------------------------------
	iMockLTSY.CompleteL(EMobileUssdMessagingNotifyNetworkRelease, KErrNone, data);
	ussdMessaging.NotifyNetworkRelease(requestStatus, pckgSendV3);

	
    User::WaitForRequest(requestStatus); 
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());
	
	ASSERT_EQUALS(opCode, sendV3.iOpCode);	
	ASSERT_EQUALS(0, sendV3.iAdditionalInfo.Compare(KAdditionalInfo1));	

	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileUssdMessaging::NotifyNetworkRelease
	// from LTSY.
 	//-------------------------------------------------------------------------
	
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	iMockLTSY.CompleteL(EMobileUssdMessagingNotifyNetworkRelease, KErrNone, data);
	
	// other version of API (with optional TMobileUssdAttributesV1 parameter)
	
	iMockLTSY.CompleteL(EMobileUssdMessagingNotifyNetworkRelease, KErrNone, data2);
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());
	
	CleanupStack::PopAndDestroy(4, this); // data2, ussdMessaging, data, this,
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0001a
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for failure on completion of pending comRMobileUssdMessaging::SendRelease request from LTSY->CTSY
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0001eL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Terminate the dialogue
    //------------------------------------------------------------------------- 
    
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 attrV1;
	RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pckgAttrV1(attrV1);
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
	
	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);
	mockData1.SerialiseL(data);

	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttrV1;
	completeAttrV1.iFlags = RMobileUssdMessaging::KUssdDataFormat | RMobileUssdMessaging::KUssdMessageType;
	completeAttrV1.iFormat = RMobileUssdMessaging::EFormatPackedString;
	completeAttrV1.iType = RMobileUssdMessaging::EUssdMOAcknowledgement;
	
	TMockLtsyData2< RMobilePhone::TMobilePhoneSendSSRequestV3, 
					RMobileUssdMessaging::TMobileUssdAttributesV1 > 
					mockData2Release(completeSendV3,completeAttrV1);
	
	RBuf8 data2;
	CleanupClosePushL(data2);
	mockData2Release.SerialiseL(data2);
	
	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------
		
	iMockLTSY.CompleteL(EMobileUssdMessagingNotifyNetworkRelease, KErrGeneral, data);
	ussdMessaging.NotifyNetworkRelease(requestStatus, pckgSendV3);

	User::WaitForRequest(requestStatus);   
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	
	
	CleanupStack::PopAndDestroy(4, this); // data2, ussdMessaging, data, this
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0001b
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::NotifyNetworkRelease  with optional paramter
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0001bL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Terminate the dialogue
    //------------------------------------------------------------------------- 
    
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 attrV1;
	RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pckgAttrV1(attrV1);
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
	
	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);
	mockData1.SerialiseL(data);

	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttrV1;
	completeAttrV1.iFlags = RMobileUssdMessaging::KUssdDataFormat | RMobileUssdMessaging::KUssdMessageType;
	completeAttrV1.iFormat = RMobileUssdMessaging::EFormatPackedString;
	completeAttrV1.iType = RMobileUssdMessaging::EUssdMOAcknowledgement;
	
	TMockLtsyData2< RMobilePhone::TMobilePhoneSendSSRequestV3, 
					RMobileUssdMessaging::TMobileUssdAttributesV1 > 
					mockData2Release(completeSendV3,completeAttrV1);
	
	RBuf8 data2;
	CleanupClosePushL(data2);
	mockData2Release.SerialiseL(data2);
	
 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileUssdMessaging::NotifyNetworkRelease when result is not cached.
	// other version of API (with optional TMobileUssdAttributesV1 parameter)
 	//-------------------------------------------------------------------------
	
	iMockLTSY.CompleteL(EMobileUssdMessagingNotifyNetworkRelease, KErrNone, data2);
	ussdMessaging.NotifyNetworkRelease(requestStatus, pckgSendV3, pckgAttrV1);

    User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	
	ASSERT_EQUALS(opCode, sendV3.iOpCode);	
	ASSERT_EQUALS(0, sendV3.iAdditionalInfo.Compare(KAdditionalInfo1));
	
	ASSERT_EQUALS(completeAttrV1.iFlags ,attrV1.iFlags);
	ASSERT_EQUALS(completeAttrV1.iFormat ,attrV1.iFormat);
	ASSERT_EQUALS(completeAttrV1.iType ,attrV1.iType);
		
	CleanupStack::PopAndDestroy(4, this); // data2, ussdMessaging, data, this,
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0001c
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0001cL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Terminate the dialogue
    //------------------------------------------------------------------------- 
    
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 attrV1;
	RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pckgAttrV1(attrV1);
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
	
	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);
	mockData1.SerialiseL(data);

	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttrV1;
	completeAttrV1.iFlags = RMobileUssdMessaging::KUssdDataFormat | RMobileUssdMessaging::KUssdMessageType;
	completeAttrV1.iFormat = RMobileUssdMessaging::EFormatPackedString;
	completeAttrV1.iType = RMobileUssdMessaging::EUssdMOAcknowledgement;
	
	TMockLtsyData2< RMobilePhone::TMobilePhoneSendSSRequestV3, 
					RMobileUssdMessaging::TMobileUssdAttributesV1 > 
					mockData2Release(completeSendV3,completeAttrV1);
	
	RBuf8 data2;
	CleanupClosePushL(data2);
	mockData2Release.SerialiseL(data2);
	
	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------
		
	// other version of API (with optional TMobileUssdAttributesV1 parameter)	
	iMockLTSY.CompleteL(EMobileUssdMessagingNotifyNetworkRelease, KErrNotReady, data2);
	ussdMessaging.NotifyNetworkRelease(requestStatus, pckgSendV3, pckgAttrV1);

    User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNotReady, requestStatus.Int());
	
	CleanupStack::PopAndDestroy(4, this); // data2, ussdMessaging, data, this,
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0001d
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::NotifyNetworkRelease before accepting a dialogue
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0001dL()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Terminate the dialogue
    //------------------------------------------------------------------------- 
    
 	//-------------------------------------------------------------------------
	// function parameters
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 attrV1;
	RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pckgAttrV1(attrV1);
 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
	
	TUint8 opCode = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV3.iOpCode         = opCode;
	completeSendV3.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);
	mockData1.SerialiseL(data);

	// optional parameter
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttrV1;
	completeAttrV1.iFlags = RMobileUssdMessaging::KUssdDataFormat | RMobileUssdMessaging::KUssdMessageType;
	completeAttrV1.iFormat = RMobileUssdMessaging::EFormatPackedString;
	completeAttrV1.iType = RMobileUssdMessaging::EUssdMOAcknowledgement;
	
	TMockLtsyData2< RMobilePhone::TMobilePhoneSendSSRequestV3, 
					RMobileUssdMessaging::TMobileUssdAttributesV1 > 
					mockData2Release(completeSendV3,completeAttrV1);
	
	RBuf8 data2;
	CleanupClosePushL(data2);
	mockData2Release.SerialiseL(data2);
	
	//Testing NotifyNetworkRelease() before accepting the dialogue, 
	//so that the requests are not always very sequential
	ussdMessaging.NotifyNetworkRelease(requestStatus, pckgSendV3, pckgAttrV1);
	ussdMessaging.AcceptIncomingDialogue();

    User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrDisconnected, requestStatus.Int());
	
	CleanupStack::PopAndDestroy(4, this); // data2, ussdMessaging, data, this
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0002L()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Terminate the dialogue
    //------------------------------------------------------------------------- 

	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
 	//-------------------------------------------------------------------------
	// Test cancelling of RMobileUssdMessaging::NotifyNetworkRelease
 	//-------------------------------------------------------------------------
	
	ussdMessaging.NotifyNetworkRelease(requestStatus, pckgSendV3);
	ussdMessaging.CancelAsyncRequest(EMobileUssdMessagingNotifyNetworkRelease);

	User::WaitForRequest(requestStatus);
	ASSERT_EQUALS(KErrCancel, requestStatus.Int());

	AssertMockLtsyStatusL();
	
	// again with the optional parameter
	
	RMobileUssdMessaging::TMobileUssdAttributesV1 attrV1;
	RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pckgAttrV1(attrV1);
	
	ussdMessaging.NotifyNetworkRelease(requestStatus, pckgSendV3, pckgAttrV1);
	ussdMessaging.CancelAsyncRequest(EMobileUssdMessagingNotifyNetworkRelease);
	
	User::WaitForRequest(requestStatus);
	ASSERT_EQUALS(KErrCancel, requestStatus.Int());

	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(3); // this, ussdMessaging, data
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::NotifyNetworkRelease with bad parameter data
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::NotifyNetworkRelease with bad parameter data
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0003L()
	{
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Terminate the dialogue
    //------------------------------------------------------------------------- 

	//-------------------------------------------------------------------------
	// Test B: Test passing wrong descriptor size to parameter in
	// RMobileUssdMessaging::NotifyNetworkRelease
 	//-------------------------------------------------------------------------

	TBuf8<1> badSizeDescriptor;
	ussdMessaging.NotifyNetworkRelease(requestStatus, badSizeDescriptor);

	User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrArgument, requestStatus.Int());
	
	// again with the optional parameter
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
	
	ussdMessaging.NotifyNetworkRelease(requestStatus, pckgSendV3, badSizeDescriptor);

	User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrArgument, requestStatus.Int());
	
	CleanupStack::PopAndDestroy(3); // this, ussdMessaging, data
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0004L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

	RBuf8 data;
	CleanupClosePushL(data);

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	RMobileUssdMessaging ussdMessaging1;
	ret = ussdMessaging1.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging1);

	RMobileUssdMessaging ussdMessaging2;
	ret = ussdMessaging2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging2);

	//-------------------------------------------------------------------------
	// The first client needs to send a message. This will cause a session to be opened
	// which will be owned by the first client.
	TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    attributes.iFlags  = flags;
    attributes.iFormat = format;
    attributes.iType   = type;
    attributes.iDcs   = dcs;
    
    TName name = _L("Name1");
    TPckg<TName> msgData(name);

    TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockSendData2(name, attributes);
    mockSendData2.SerialiseL(data);

    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessage, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessage, KErrNone);

    ussdMessaging1.SendMessage(requestStatus1, msgData, msgAttributes);
    
    User::WaitForRequest(requestStatus1);        
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus1.Int());
    	
 	//-------------------------------------------------------------------------
	// parameters for first function
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV31;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV31(sendV31);

 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV31;

	TUint8 opCode1 = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV31.iOpCode         = opCode1;
	completeSendV31.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData11(completeSendV31);

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttrV1;
	completeAttrV1.iFlags = RMobileUssdMessaging::KUssdDataFormat | RMobileUssdMessaging::KUssdMessageType;
	completeAttrV1.iFormat = RMobileUssdMessaging::EFormatPackedString;
	completeAttrV1.iType = RMobileUssdMessaging::EUssdMTRequest;
	
	TMockLtsyData2< RMobilePhone::TMobilePhoneSendSSRequestV3, 
					RMobileUssdMessaging::TMobileUssdAttributesV1 > 
					mockData2(completeSendV31,completeAttrV1);
	
	
	RBuf8 data2;
	CleanupClosePushL(data2);
	mockData2.SerialiseL(data2);
		
	//-------------------------------------------------------------------------
	// parameters for second function
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV32;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV32(sendV32);

	//-------------------------------------------------------------------------
	// Test A: Test multiple clients requesting RMobileUssdMessaging::NotifyNetworkRelease
 	//-------------------------------------------------------------------------

	mockData11.SerialiseL(data2);
	iMockLTSY.CompleteL(EMobileUssdMessagingNotifyNetworkRelease, KErrNone, data2, 10);
	ussdMessaging1.NotifyNetworkRelease(requestStatus1, pckgSendV31);

	ussdMessaging2.NotifyNetworkRelease(requestStatus2, pckgSendV32);

    User::WaitForRequest(requestStatus1);        
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());	

    User::WaitForRequest(requestStatus2);        
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	

	ASSERT_EQUALS(opCode1, sendV31.iOpCode);	
	ASSERT_EQUALS(0, sendV31.iAdditionalInfo.Compare(KAdditionalInfo1));	

	ASSERT_EQUALS(opCode1, sendV32.iOpCode);	
	ASSERT_EQUALS(0, sendV32.iAdditionalInfo.Compare(KAdditionalInfo1));	
	
	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(7, this); // phone2, telServer2, data, this
	                                      // ussdMessaging1, ussdMessaging2, data2 (inv. order)
	}

////
/**
@SYMTestCaseID BA-CTSY-USSDM-UNNR-0004b
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::NotifyNetworkRelease
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestNotifyNetworkRelease0004bL()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

	RBuf8 data;
	CleanupClosePushL(data);

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	RMobileUssdMessaging ussdMessaging1;
	ret = ussdMessaging1.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging1);

	RMobileUssdMessaging ussdMessaging2;
	ret = ussdMessaging2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging2);

	//-------------------------------------------------------------------------
	// The first client needs to send a message. This will cause a session to be opened
	// which will be owned by the first client.
	TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    attributes.iFlags  = flags;
    attributes.iFormat = format;
    attributes.iType   = type;
    attributes.iDcs   = dcs;
    
    TName name = _L("Name1");
    TPckg<TName> msgData(name);

    TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockSendData2(name, attributes);
    mockSendData2.SerialiseL(data);

    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessage, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessage, KErrNone);

    ussdMessaging1.SendMessage(requestStatus1, msgData, msgAttributes);
    
    User::WaitForRequest(requestStatus1);        
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus1.Int());
    	
 	//-------------------------------------------------------------------------
	// parameters for first function
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV31;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV31(sendV31);

 	//-------------------------------------------------------------------------

	RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV31;

	TUint8 opCode1 = 100;
	_LIT(KAdditionalInfo1, "additionalInfo1");
	
	completeSendV31.iOpCode         = opCode1;
	completeSendV31.iAdditionalInfo = KAdditionalInfo1;

	TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData11(completeSendV31);

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttrV1;
	completeAttrV1.iFlags = RMobileUssdMessaging::KUssdDataFormat | RMobileUssdMessaging::KUssdMessageType;
	completeAttrV1.iFormat = RMobileUssdMessaging::EFormatPackedString;
	completeAttrV1.iType = RMobileUssdMessaging::EUssdMTRequest;
	
	TMockLtsyData2< RMobilePhone::TMobilePhoneSendSSRequestV3, 
					RMobileUssdMessaging::TMobileUssdAttributesV1 > 
					mockData2(completeSendV31,completeAttrV1);
	
	
	RBuf8 data2;
	CleanupClosePushL(data2);
	mockData2.SerialiseL(data2);
		
	//-------------------------------------------------------------------------
	// parameters for second function
	
	RMobilePhone::TMobilePhoneSendSSRequestV3 sendV32;
	TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV32(sendV32);

	//-------------------------------------------------------------------------
	// Test B: Test multiple clients requesting RMobileUssdMessaging::NotifyNetworkRelease
	// with optional second parameter
 	//-------------------------------------------------------------------------
	 
	ussdMessaging1.Close();
	ret = ussdMessaging1.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	ussdMessaging2.Close();
	ret = ussdMessaging2.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	
	RMobileUssdMessaging::TMobileUssdAttributesV1 attrV1;
	RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pckgAttrV1(attrV1);

	RMobileUssdMessaging::TMobileUssdAttributesV1 attrV12;
	RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pckgAttrV12(attrV12);

	iMockLTSY.CompleteL(EMobileUssdMessagingNotifyNetworkRelease, KErrNone, data2, 5);
	ussdMessaging1.NotifyNetworkRelease(requestStatus1, pckgSendV31, pckgAttrV1);
	ussdMessaging2.NotifyNetworkRelease(requestStatus2, pckgSendV32, pckgAttrV12);	
	
    User::WaitForRequest(requestStatus1);  
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());	

    User::WaitForRequest(requestStatus2);  
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	
	
	ASSERT_EQUALS(opCode1, sendV31.iOpCode);	
	ASSERT_EQUALS(0, sendV31.iAdditionalInfo.Compare(KAdditionalInfo1));	

	ASSERT_EQUALS(opCode1, sendV32.iOpCode);	
	ASSERT_EQUALS(0, sendV32.iAdditionalInfo.Compare(KAdditionalInfo1));	
	
	ASSERT_EQUALS(completeAttrV1.iFlags ,attrV1.iFlags);
	ASSERT_EQUALS(completeAttrV1.iFormat ,attrV1.iFormat);
	ASSERT_EQUALS(completeAttrV1.iType ,attrV1.iType);	
	
	ASSERT_EQUALS(completeAttrV1.iFlags ,attrV12.iFlags);
	ASSERT_EQUALS(completeAttrV1.iFormat ,attrV12.iFormat);
	ASSERT_EQUALS(completeAttrV1.iType ,attrV12.iType);		
		
	CleanupStack::PopAndDestroy(7, this); // phone2, telServer2, data, this
	                                      // ussdMessaging1, ussdMessaging2, data2 (inv. order)
	}
////

/**
@SYMTestCaseID BA-CTSY-USSDM-USMNFC-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendMessageNoFdnCheck
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendMessageNoFdnCheck
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageNoFdnCheck0001L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	TRequestStatus requestStatus;

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);

 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------

	mockData2.SerialiseL(data);
	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageNoFdnCheck, data, KErrNotSupported);

	ussdMessaging.SendMessageNoFdnCheck(requestStatus, msgData, msgAttributes);

	User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	

	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------

	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageNoFdnCheck, data);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageNoFdnCheck, KErrGeneral);

	ussdMessaging.SendMessageNoFdnCheck(requestStatus, msgData, msgAttributes);

	//Wrong IPC is passed to ResetTsyReqHandle() EMultimodeUssdSendMessage 
	//instead of EMultimodeUssdSendMessageNoFdnCheck by call of 
	//CMmUssdTsy::CompleteSendMessageNoFdnCheck(). This reduces to panic 
	//with EEtelFaultNotRecognisedTsyHandle.

	User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileUssdMessaging::SendMessageNoFdnCheck when result is not cached.
 	//-------------------------------------------------------------------------

	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageNoFdnCheck, data);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageNoFdnCheck, KErrNone);

	ussdMessaging.SendMessageNoFdnCheck(requestStatus, msgData, msgAttributes);

	User::WaitForRequest(requestStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());

 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileUssdMessaging::SendMessageNoFdnCheck
	// from LTSY.
 	//-------------------------------------------------------------------------

	TRequestStatus mockLtsyStatus;

	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageNoFdnCheck, KErrNone);
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

	CleanupStack::PopAndDestroy(3, this); // data, ussdMessaging, this
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USMNFC-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileUssdMessaging::SendMessageNoFdnCheck
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileUssdMessaging::SendMessageNoFdnCheck
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageNoFdnCheck0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	TRequestStatus mockLtsyStatus;
	TRequestStatus requestStatus;

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);

 	//-------------------------------------------------------------------------
	// Test cancelling of RMobileUssdMessaging::SendMessageNoFdnCheck
 	//-------------------------------------------------------------------------
 	
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
 	
	mockData2.SerialiseL(data);
    iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageNoFdnCheck, data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageNoFdnCheck, KErrNone, 10);
	
	ussdMessaging.SendMessageNoFdnCheck(requestStatus, msgData, msgAttributes);
    ussdMessaging.CancelAsyncRequest(EMobileUssdMessagingSendMessageNoFdnCheck);
		
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());

    User::WaitForRequest(requestStatus);
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrCancel, requestStatus.Int());

	CleanupStack::PopAndDestroy(3); // data, this, ussdMessaging
	
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USMNFC-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendMessageNoFdnCheck with bad parameter data
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendMessageNoFdnCheck with bad parameter data
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageNoFdnCheck0003L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

 	//-------------------------------------------------------------------------
	// function parameter

	TName name = _L("Name1");
	TPckg<TName> msgData(name);

	//-------------------------------------------------------------------------
	// Test B: Test passing wrong descriptor size to parameter in
	// RMobileUssdMessaging::SendMessage
 	//-------------------------------------------------------------------------

	TBuf8<1> badSizeDescriptor;

	ussdMessaging.SendMessageNoFdnCheck(requestStatus, msgData, badSizeDescriptor);
	
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrArgument, requestStatus.Int());	
	
	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging, ussdMessaging
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USMNFC-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::SendMessageNoFdnCheck
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::SendMessageNoFdnCheck
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageNoFdnCheck0004L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	RMobileUssdMessaging ussdMessaging1;
	ret = ussdMessaging1.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging1);

	RMobileUssdMessaging ussdMessaging2;
	ret = ussdMessaging2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging2);

	TRequestStatus requestStatus1;
	TRequestStatus requestStatus2;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// parameters for first function

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes1;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes1(attributes1);
	
	attributes1.iFlags  = flags;
	attributes1.iFormat = format;
	attributes1.iType   = type;
	attributes1.iDcs    = dcs;

	TName name1 = _L("Name1");
	TPckg<TName> msgData1(name1);

 	//-------------------------------------------------------------------------
	// parameters for second function

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes2;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes2(attributes2);

	flags  = 10;
	format = RMobileUssdMessaging::EFormatPackedString;
	type   = RMobileUssdMessaging::EUssdMOAcknowledgement;
	dcs    = 20;
	
	attributes2.iFlags  = flags;
	attributes2.iFormat = format;
	attributes2.iType   = type;
	attributes2.iDcs    = dcs;
	
	TName name2 = _L("Name2");
	TPckg<TName> msgData2(name2);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name1, attributes1);

	//-------------------------------------------------------------------------
	// Test A: Test multiple clients requesting RMobileUssdMessaging::SendMessageNoFdnCheck
 	//-------------------------------------------------------------------------

	mockData2.SerialiseL(data);
	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageNoFdnCheck, data);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageNoFdnCheck, KErrNone, 10);
	ussdMessaging1.SendMessageNoFdnCheck(requestStatus1, msgData1, msgAttributes1);

	ussdMessaging2.SendMessageNoFdnCheck(requestStatus2, msgData2, msgAttributes2);

	User::WaitForRequest(requestStatus1);
	ASSERT_EQUALS(KErrNone, requestStatus1.Int());	

	User::WaitForRequest(requestStatus2);
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrServerBusy, requestStatus2.Int());	

	CleanupStack::PopAndDestroy(6, this); // phone2, telServer2, data, this, 
	                                      // ussdMessaging1, ussdMessaging2
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-USMNFC-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::SendMessageNoFdnCheck with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::SendMessageNoFdnCheck and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageNoFdnCheck0005L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

 	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);

	//-------------------------------------------------------------------------
	// Test A: Test timeout of RMobileUssdMessaging::SendMessageNoFdnCheck
 	//-------------------------------------------------------------------------

	mockData2.SerialiseL(data);
	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageNoFdnCheck, data);

	ussdMessaging.SendMessageNoFdnCheck(requestStatus, msgData, msgAttributes);

	User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());	

	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UGC-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::GetCaps
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::GetCaps
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestGetCaps0001L()
	{	
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);

	TUint32 format = RMobileUssdMessaging::KCapsPackedString;
	TUint32 types  = KUssdTypeCaps;

	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileUssdMessaging::GetCaps when result is not cached.
 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdCapsV1 capsV1;
	TPckg<RMobileUssdMessaging::TMobileUssdCapsV1> pckg(capsV1);
	ret = ussdMessaging.GetCaps(pckg);
	ASSERT_EQUALS(KErrNone, ret);
	ASSERT_EQUALS(format, capsV1.iUssdFormat); // hard coded value
	ASSERT_EQUALS(types , capsV1.iUssdTypes ); // hard coded value

	CleanupStack::PopAndDestroy(2, this); // this, ussdMessaging
	}

/**
@SYMTestCaseID BA-CTSY-USSDM-UGC-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::GetCaps with bad parameter data
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::GetCaps with bad parameter data
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestGetCaps0003L()
	{
	
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);

	//-------------------------------------------------------------------------
	// Test B: Test passing wrong descriptor size to parameter in
	// RMobileUssdMessaging::GetCaps
 	//-------------------------------------------------------------------------

	TBuf8<1> badSizeDescriptor;
	
	ret = ussdMessaging.GetCaps(badSizeDescriptor);
	ASSERT_EQUALS(KErrArgument, ret);

	CleanupStack::PopAndDestroy(2, this); // this, ussdMessaging
	
	}


TInt CCTsyUssdMessagingFU::CreateClients(RThread& aT1, RThread& aT2, TTestDataWithChunk& aC1,  TTestDataWithChunk& aC2) 
	{
    TInt KStackSize=0x8000;
    TInt KHeapSize=0x8000;
    TInt KMaxHeapSize=0x80000;

	TInt ret = KErrNone;
	ret = aT1.Create(_L("UssdClient1"), TestReceiveMessage, 
	        KStackSize, KHeapSize, KMaxHeapSize, &aC1);
			
	if (ret !=KErrNone)
		{
		return ret;
		}

	ret = aT2.Create(_L("UssdClient2"), TestReceiveMessage, 
	        KStackSize, KHeapSize, KMaxHeapSize, &aC2);
			
	return ret;	
	}

void CCTsyUssdMessagingFU::MultipleClientRequestsL(RThread& aT1, RThread& aT2)
	{
	RBuf8 data;
	
	TRequestStatus reqStatus1;
	TRequestStatus reqStatus2;
    
    TBool req1Complete=EFalse;
	TBool req2Complete=EFalse;

    aT1.Logon(reqStatus1);
    aT2.Logon(reqStatus2);
    aT1.Resume();
    aT2.Resume();
    
    CompleteMockRequestL(data, EMobileUssdMessagingReceiveMessage, iMockLTSY);
    while ( req1Complete==EFalse || req2Complete==EFalse )
        {
        User::WaitForAnyRequest();
        if (reqStatus1 != KRequestPending && req1Complete==EFalse)
            {
            ASSERT_EQUALS(reqStatus1.Int(), KErrNone);
            req1Complete=ETrue;
            }
        if (reqStatus2 != KRequestPending && req2Complete==EFalse)
            {
            ASSERT_EQUALS(reqStatus2.Int(), KErrNone);
            req2Complete=ETrue;
            }
        } // while statement
	
	data.Close();
	}
	
	
// 
TInt CCTsyUssdMessagingFU::TestReceiveMessage(TAny* aThreadData )
	{
	// Create a cleanup stack object
	CTrapCleanup* cleanup=CTrapCleanup::New();
	if (cleanup==NULL)
		return KErrNoMemory;	

	RMockLtsy mockLtsy;
	TInt err=mockLtsy.Connect();
	// ASSERT_EQUALS(KErrNone, err);

	RBuf8 data;
	
	TTestUssdData* testData = static_cast<TTestUssdData*>(aThreadData);
	
    RTelServer server;
    RMobilePhone phone;

    TInt ret = server.Connect();
    if (ret != KErrNone)
        {
        server.Close();
        return ret; 
        }

	ret = phone.Open(server,KMmTsyPhoneName);
	if (ret != KErrNone)
		{
		server.Close();
		phone.Close();
		return ret;	
		}
	
    TRequestStatus requestStatus;	
    RMobileUssdMessaging::TGsmUssdMessageData name;  
    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging ussdMessaging;
	ret = ussdMessaging.Open(phone);
    if (ret != KErrNone)
        {
        server.Close();
        phone.Close();
        return ret; 
        }
    
    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);	
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
	
	TBool waiting = EFalse;
	if (testData->iAfterTimePeriod > 0)
		{
		// Wait, mimic a bad client taking too long...
		User::After(testData->iAfterTimePeriod * 100000);
		}
		
	if (testData->iAcceptDialogue)
		{
		// Client wants the dialogue
		if (ussdMessaging.AcceptIncomingDialogue() != KErrTimedOut) 
			{
			// This client should have the dialogue. 
			// To check, wait for next message (in the session/dialogue)
			ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);		
			CompleteMockRequestL(data, EMobileUssdMessagingReceiveMessage, mockLtsy);		
			waiting = ETrue;
			}
		}
	else
		{
		// Client not interested
		ussdMessaging.RejectIncomingDialogue();   
		}

	if (waiting)
		{
		User::WaitForRequest(requestStatus);        
		ret = requestStatus.Int();		
		}

	User::After(2 * 1000000);

    ussdMessaging.Close();
	phone.Close();
	server.Close();
	data.Close();
	
	delete cleanup;
	return KErrNone;
	}

void CCTsyUssdMessagingFU::CompleteMockRequestL(RBuf8& aData, TInt aMsgType, RMockLtsy& aMockLTSY)
	{
    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;

    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
                                            mockData(completeName, completeAttributes);
    TRAPD(ret, mockData.SerialiseL(aData));
	
	aMockLTSY.CompleteL(aMsgType, KErrNone, aData, 15); 
	}

void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClientL(RArray<TUssdClientTestData> & aClientTestData, TBool aRandomLoopingTest)
    {
	
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();
    
    RBuf8 data;
    CleanupClosePushL(data);
     //-------------------------------------------------------------------------
    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;

    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
                                            mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    const TInt numClients = aClientTestData.Count();
    RProcess* proc = new RProcess[numClients];
    TRequestStatus* req = new TRequestStatus[numClients];
    
    for (TInt i=0; i<numClients; ++i)
        {
        TInt err = proc[i].Create(aClientTestData[i].iExe, aClientTestData[i].iClientParams);
        // If a client isn't created properly, we need to clean up previously created ones here
        if (err != KErrNone)
            {
            for (TInt c=0; c<i-1; ++c)
                {
                proc[c].Kill(KErrNone);
                proc[c].Close();
                }
            }
        // Fail the test if any client has not been created properly
        ASSERT_EQUALS(err, KErrNone);
        }
    
    // All processes created successfully, start their execution
    for (TInt i = 0; i < numClients; i++)
        {
        proc[i].Logon(req[i]);
        proc[i].Resume();
        // Pause here so that clients register in the order specified (i.e. As ordered in the array)
        // Resolves timing issues in WDP/SMP.
        User::After(500000);
        }
    
    if (aRandomLoopingTest)
        {
        // Special case just for TestReceiveMessageWithTestClient0017L
        for (TInt i=0; i<20; ++i)
            {
            iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data, 1);
            User::After(750000);
            }
        for (TInt i = 0; i < numClients; i++)
            {
            proc[i].Kill(KErrNone);
            proc[i].Close();
            }
        AssertMockLtsyStatusL(); 
        CleanupStack::PopAndDestroy(2, this);
        // We have no idea what state the clients will be in so there is no point in
        // checking. This test passes if we don't crash the ETel server!
        return;
        }
    
   iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data, 15);
    User::WaitForAnyRequest();
    
    // Wait for any clients given a timeout
    User::After(5 * 1000000);
    
    for (TInt i = 0; i < numClients; i++)
        {
        TRequestStatus status = req[i];
        TExitType exit = proc[i].ExitType();
        TInt reason = proc[i].ExitReason();
        
        // Kill/Close the clients before doing assert
        proc[i].Kill(KErrNone);
        proc[i].Close();

        const TBool checkRequestStatus = status.Int() == aClientTestData[i].iReqStatus;
        const TBool checkExitType = exit == aClientTestData[i].iExitType;
        TBool checkExitReason = ETrue;
        if (aClientTestData[i].iExitType == EExitKill)
            {
            checkExitReason = reason == aClientTestData[i].iExitReason;
            }

        if (!checkRequestStatus || !checkExitType || !checkExitReason)
            {
            // Some useful output in case of a failed test
            INFO_PRINTF2(_L("Client[%d] returned unexpected results:"),i);
            if (!checkRequestStatus)
                INFO_PRINTF3(_L("  [RequestStatus] Expected: %d , Actual: %d"),aClientTestData[i].iReqStatus,status.Int());
            if (!checkExitType)
                INFO_PRINTF3(_L("  [ExitType]      Expected: %d , Actual: %d"),aClientTestData[i].iExitType,exit);
            if (!checkExitReason)
                INFO_PRINTF3(_L("  [ExitReason]    Expected: %d , Actual: %d"),aClientTestData[i].iExitReason,reason);
            for (TInt c=i+1; c<numClients; ++c)
                {
                proc[c].Kill(KErrNone);
                proc[c].Close();
                }
            ASSERT_TRUE(checkRequestStatus);
            ASSERT_TRUE(checkExitType);
            ASSERT_TRUE(checkExitReason);
            }
        }
    AssertMockLtsyStatusL();
    
    CleanupStack::PopAndDestroy(2, this);
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0005
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority client accepts dialogue
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0001L()
    {
    RArray<TUssdClientTestData> data1;
    data1.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"), _L("-R -A"),        KErrNone,    EExitKill, KErrNone)); //Priority
    data1.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"),  _L("-R -A"), KRequestPending, EExitPending, KErrNone)); //Default
    data1.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),   _L("-R -A"), KRequestPending, EExitPending, KErrNone));
    TestReceiveMessageWithTestClientL(data1);
    data1.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0006
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority client takes too long before accepting/rejecting dialogue. Next standard client to accept will get the dialogue
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0002L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -A -T 2"),    KErrTimedOut, EExitKill, KErrTimedOut)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),      KRequestPending, EExitPending, KErrNone )); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A"),             KErrNone,    EExitKill, KErrNone ));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0007
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority client rejects the dialogue. Next standard client to accept will get the dialogue.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0003L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R"),            KErrNone,    EExitKill, KErrNone)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),  KRequestPending, EExitPending, KErrNone)); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A"),         KErrNone,    EExitKill, KErrNone));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0008
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority client rejects the dialogue. Standard client takes too long before accepting/rejecting the dialogue. Default client accepts the dialogue.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0004L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R"),             KErrNone, EExitKill, KErrNone    )); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),          KErrNone, EExitKill, KErrNone    )); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 3"), KErrTimedOut, EExitKill, KErrTimedOut));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0009
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority client rejects the dialogue. Standard client rejects the dialogue. Default client accepts the dialogue.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0005L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R"),    KErrNone, EExitKill, KErrNone)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"), KErrNone, EExitKill, KErrNone)); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R"),    KErrNone, EExitKill, KErrNone));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0010
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority doesn't time out where others would.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0006L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -A -T 0.2"),        KErrNone,    EExitKill, KErrNone)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),        KRequestPending, EExitPending, KErrNone)); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A"),        KRequestPending, EExitPending, KErrNone));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0011
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority times out. Normal clients time out with same length priority won in previous test case. Default client accepts the dialogue.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0007L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -A -T 0.5"), KErrTimedOut, EExitKill, KErrTimedOut)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),            KErrNone, EExitKill,     KErrNone)); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0012
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority times out. Normal client with small timeout should get the dialogue.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0008L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -A -T 0.5"),     KErrTimedOut,    EExitKill, KErrTimedOut)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),         KRequestPending, EExitPending,     KErrNone)); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.05"),        KErrNone,    EExitKill,     KErrNone));

    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0013
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Priority and all normal clients reject or timeout the incoming dialogue. Default client accepts the dialogue.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0009L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -T 0.5"),    KErrTimedOut, EExitKill, KErrTimedOut)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),            KErrNone, EExitKill,     KErrNone)); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -T 0.2"),    KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R"),               KErrNone, EExitKill,     KErrNone));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -T 0.2"),        KErrNone, EExitKill,     KErrNone)); //Rejecting the request after the session is over will lead to KErrNone
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R"),               KErrNone, EExitKill,     KErrNone));
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A -T 0.2"), KErrTimedOut, EExitKill, KErrTimedOut));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0014
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. No normal clients, only priority and default. Dialogue accepted by priority client.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0010L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -A"),         KErrNone,       EExitKill,   KErrNone)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),  KRequestPending,    EExitPending,   KErrNone)); //Default
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-00015
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileUssdMessaging::ReceiveMessage with varying interest categories
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. No normal clients, only priority and default. Priority times out. Dialogue accepted by default client.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0011L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -A -T 1.5"), KErrTimedOut, EExitKill, KErrTimedOut)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),            KErrNone, EExitKill,     KErrNone)); //Default
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0015
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Testing multiple priority clients. The second Priority client should not be treated as such, should be added to the list of standard clients.
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Four clients: 2 priority, 1 default and 1 standard client. The first Priority client should be offered the dialogue, not the second.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0013L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -A"),            KErrNone,    EExitKill, KErrNone)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -A"),     KRequestPending, EExitPending, KErrNone)); //Priority. Gets added to standard client list as already have a priority client
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),     KRequestPending, EExitPending, KErrNone)); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A"),     KRequestPending, EExitPending, KErrNone));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0015
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Testing multiple default clients. The second default client should not be treated as such, should be added to the list of standard clients.
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileUssdMessaging::ReceiveMessage. Four clients: 1 priority, 2 default and 1 standard client. The second default client added should act like a standard client and be offered the dialogue before the first default client.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0014L()
    {
    RArray<TUssdClientTestData> data;
    data.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R"),               KErrNone,    EExitKill, KErrNone)); //Priority
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),     KRequestPending, EExitPending, KErrNone)); //Default
    data.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -A"),            KErrNone,    EExitKill, KErrNone)); //Default. Gets added to standard client list as already have a default client
    data.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),  _L("-R -A"),     KRequestPending, EExitPending, KErrNone));
    TestReceiveMessageWithTestClientL(data);
    data.Close();
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-URM-0020
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test ReceiveMessage with multiple clients.
@SYMTestPriority High
@SYMTestActions Two clients, both call ReceiveMessage. A message is received. First client offered message. Both clients call Close.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0016L()
    {
    RArray<TUssdClientTestData> data1;
    data1.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"), _L("-R -A"), KErrNone, EExitKill, KErrNone));
    data1.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"), _L("-R -RTimeout"), KRequestPending, EExitKill, KRequestPending));
    TestReceiveMessageWithTestClientL(data1);
    data1.Close();
    }

/*
@SYMTestCaseID BA-CTSY-USSDM-URM-0021
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test ReceiveMessage with multiple clients.
@SYMTestPriority High
@SYMTestActions Two clients, both call ReceiveMessage. A message is received. First client offered message. Both clients call Close.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestReceiveMessageWithTestClient0017L()
    {
    RArray<TUssdClientTestData> data1;
    data1.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"), _L("-Random"), KErrNone, EExitKill, KErrNone));
    data1.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"), _L("-Random"), KErrNone, EExitKill, KErrNone));
    data1.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"), _L("-Random"), KErrNone, EExitKill, KErrNone));
    data1.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"), _L("-Random"), KErrNone, EExitKill, KErrNone));
    data1.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-Random"), KErrNone, EExitKill, KErrNone));
    TestReceiveMessageWithTestClientL(data1, ETrue);
    data1.Close();
    }


/**
@SYMTestCaseID BA-CTSY-USSDM-UARM-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test calling accept and reject before a session has been created 
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::AcceptIncomingDialogue and RMobileUssdMessaging::RejectIncomingDialogue
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestAcceptRejectMisuse0001L()
    {	
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    TInt status = ussdMessaging.AcceptIncomingDialogue();
    
    ASSERT_EQUALS(KErrNotReady , status);
    
    status = ussdMessaging.RejectIncomingDialogue();
    
    ASSERT_EQUALS(KErrNotReady , status);
    
    AssertMockLtsyStatusL();
    
    CleanupStack::PopAndDestroy(2, this); // this, ussdMessaging
    
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-UARM-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test calling accept and reject after a session has ended 
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::AcceptIncomingDialogue and RMobileUssdMessaging::RejectIncomingDialogue
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestAcceptRejectMisuse0002L()
    {
	
    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RBuf8 data;
    CleanupClosePushL(data);

    RMobileUssdMessaging ussdMessaging;
    TInt ret = ussdMessaging.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(ussdMessaging);
    
    //-------------------------------------------------------------------------
    // Setup: This block is taken from TestReceiveMessage0001L. All we want to
    // do is to initiate a dialogue and then end it. Once the dialogue has ended
    // then we can call Accept and Reject.
    //------------------------------------------------------------------------- 
    
    TRequestStatus requestStatus;
        
    TRequestStatus mockLtsyStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    //-------------------------------------------------------------------------
    // function parameters

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging::TGsmUssdMessageData name;

    //-------------------------------------------------------------------------

    RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

    completeAttributes.iFlags  = flags;
    completeAttributes.iFormat = format;
    completeAttributes.iType   = type;
    completeAttributes.iDcs    = dcs;
    
    TBuf8<KMaxName> completeName = _L8("Name1");

    TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
        mockData2(completeName, completeAttributes);
    mockData2.SerialiseL(data);
    
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);   
    User::WaitForRequest(requestStatus);        
    ret = requestStatus.Int();
    ASSERT_EQUALS(KErrNone, ret);
    ussdMessaging.AcceptIncomingDialogue();
    
    AssertMockLtsyStatusL();
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // Release the dialogue
    //------------------------------------------------------------------------- 
    
    RMobilePhone::TMobilePhoneSendSSRequestV3 sendV3;
    TPckg<RMobilePhone::TMobilePhoneSendSSRequestV3> pckgSendV3(sendV3);
    
    iMockLTSY.ExpectL(EMobileUssdMessagingSendRelease);
    ussdMessaging.SendRelease(requestStatus, pckgSendV3);
    
    RMobilePhone::TMobilePhoneSendSSRequestV3 completeSendV3;
    TUint8 opCode = 100;
    _LIT(KAdditionalInfo1, "additionalInfo1");
    completeSendV3.iOpCode         = opCode;
    completeSendV3.iAdditionalInfo = KAdditionalInfo1;
    TMockLtsyData1< RMobilePhone::TMobilePhoneSendSSRequestV3 > mockData1(completeSendV3);
    mockData1.SerialiseL(data);
    iMockLTSY.CompleteL(EMobileUssdMessagingSendRelease, KErrNone, data, 1);
    
    User::WaitForRequest(requestStatus);    
    ASSERT_EQUALS(KErrNone, requestStatus.Int());
    
    //-------------------------------------------------------------------------
    // TEST A: Call Accept and Reject once the dialogue is over.
    //------------------------------------------------------------------------- 
    
    TInt status = ussdMessaging.AcceptIncomingDialogue();
    
    ASSERT_EQUALS(KErrTimedOut, status);
    
    status = ussdMessaging.RejectIncomingDialogue();
    
    ASSERT_EQUALS(KErrNone, status);
    
    AssertMockLtsyStatusL();
    
    CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging
    
    }

/**
@SYMTestCaseID BA-CTSY-USSDM-MIUM-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc BA-CTSY-USSDM-MIUM-0001
@SYMTestPriority High
@SYMTestActions Client calls ReceiveMessage. First USSD message arrives and is offered to client.
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestMultipleIncomingUssdMessages0001L()
    {
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;
	    
    TRequestStatus mockLtsyStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

 	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	RMobileUssdMessaging::TGsmUssdMessageData name;

 	//-------------------------------------------------------------------------

	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;

	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	
	TBuf8<KMaxName> completeName = _L8("Name1");

	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(completeName, completeAttributes);

	mockData2.SerialiseL(data);
	
 	//-------------------------------------------------------------------------
	// TEST : Multiple incoming "network" USSD messages are received.
	// This is handled without problem in Etel.
 	//------------------------------------------------------------------------- 
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);

	ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);	
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());

	// Second incoming USSD message
    ussdMessaging.AcceptIncomingDialogue();
    iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data);	
    User::After(5 * 1000000);
    
    AssertMockLtsyStatusL();
 
	CleanupStack::PopAndDestroy(3, this); // data, this, ussdMessaging
	
	}
	

/**
@SYMTestCaseID BA-CTSY-USSDM-USMDH-0001
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY client requests to RMobileUssdMessaging::SendMessage with default handler
@SYMTestPriority High
@SYMTestActions Invokes request to RMobileUssdMessaging::SendMessage with EMobileUssdMessagingSendMessageDefaultHandler option
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageDefaultHandlerWithTestClient0001L()
    {	
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RArray<TUssdClientTestData> data1;
	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);
	mockData2.SerialiseL(data);	
	//-------------------------------------------------------------------------
	RBuf8 data2;
	CleanupClosePushL(data2);
	//-------------------------------------------------------------------------	
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;
	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	TBuf8<KMaxName> completeName = _L8("Name2");
	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
											mockData3(completeName, completeAttributes);
	mockData3.SerialiseL(data2);
	//-------------------------------------------------------------------------	
	data1.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -M"), KErrNone, EExitKill, KErrNone)); //Default
	const TInt numClients = data1.Count();
	RProcess* proc = new RProcess[numClients];
	TRequestStatus* req = new TRequestStatus[numClients];
	
	for (TInt i=0; i<numClients; ++i)
		{
		TInt err = proc[i].Create(data1[i].iExe, data1[i].iClientParams);
		// If a client isn't created properly, we need to clean up previously created ones here
		if (err != KErrNone)
			{
			for (TInt c=0; c<i-1; ++c)
				{
				proc[c].Kill(KErrNone);
				proc[c].Close();
				}
			}
		// Fail the test if any client has not been created properly
		ASSERT_EQUALS(err, KErrNone);
		}
	// All processes created successfully, start their execution
	for (TInt i = 0; i < numClients; i++)
		{
		proc[i].Logon(req[i]);
		proc[i].Resume();
		// Pause here so that clients register in the order specified (i.e. As ordered in the array)
		// Resolves timing issues in WDP/SMP.
		User::After(500000);
		}
	//-------------------------------------------------------------------------
	
	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageDefaultHandler, data);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageDefaultHandler, KErrNone);
		
	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes, RMobileUssdMessaging::ETransferToDefaultHandler);	
	User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	//---------------------------------------------------------------------------
		
	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data2, 15);
	User::After(5 * 1000000);
	
	for (TInt i = 0; i < numClients; i++)
		{
		TRequestStatus status = req[i];
		TExitType exit = proc[i].ExitType();
		TInt reason = proc[i].ExitReason();
		
		// Kill/Close the clients before doing assert
		proc[i].Kill(KErrNone);
		proc[i].Close();

		const TBool checkRequestStatus = status.Int() == data1[i].iReqStatus;
		const TBool checkExitType = exit == data1[i].iExitType;
		TBool checkExitReason = ETrue;
		if (data1[i].iExitType == EExitKill)
			{
			checkExitReason = reason == data1[i].iExitReason;
			}

		if (!checkRequestStatus || !checkExitType || !checkExitReason)
			{
			// Some useful output in case of a failed test
			INFO_PRINTF2(_L("Client[%d] returned unexpected results:"),i);
			if (!checkRequestStatus)
				INFO_PRINTF3(_L("  [RequestStatus] Expected: %d , Actual: %d"),data1[i].iReqStatus,status.Int());
			if (!checkExitType)
				INFO_PRINTF3(_L("  [ExitType]      Expected: %d , Actual: %d"),data1[i].iExitType,exit);
			if (!checkExitReason)
				INFO_PRINTF3(_L("  [ExitReason]    Expected: %d , Actual: %d"),data1[i].iExitReason,reason);
			for (TInt c=i+1; c<numClients; ++c)
				{
				proc[c].Kill(KErrNone);
				proc[c].Close();
				}
			ASSERT_TRUE(checkRequestStatus);
			ASSERT_TRUE(checkExitType);
			ASSERT_TRUE(checkExitReason);
			}
		}	
	//---------------------------------------------------------------------------
	data1.Close();
	CleanupStack::PopAndDestroy(4, this); // data, data2, this, ussdMessaging
    }



/**
@SYMTestCaseID BA-CTSY-USSDM-USMDH-0002
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY client requests to RMobileUssdMessaging::SendMessage with two default clients
@SYMTestPriority High
@SYMTestActions Invokes request to RMobileUssdMessaging::SendMessage with EMobileUssdMessagingSendMessageDefaultHandler option
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageDefaultHandlerWithTestClient0002L()
    {	
	
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RArray<TUssdClientTestData> data1;
	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);
	mockData2.SerialiseL(data);	
	//-------------------------------------------------------------------------
	RBuf8 data2;
	CleanupClosePushL(data2);
	//-------------------------------------------------------------------------	
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;
	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	TBuf8<KMaxName> completeName = _L8("Name2");
	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
											mockData3(completeName, completeAttributes);
	mockData3.SerialiseL(data2);
	//-------------------------------------------------------------------------	
	data1.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -M"), KErrNone, EExitKill, KErrNone)); //Default
	data1.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"),_L("-R -M"), KRequestPending, EExitPending, KErrNone)); //Second default client
	const TInt numClients = data1.Count();
	RProcess* proc = new RProcess[numClients];
	TRequestStatus* req = new TRequestStatus[numClients];
	
	for (TInt i=0; i<numClients; ++i)
		{
		TInt err = proc[i].Create(data1[i].iExe, data1[i].iClientParams);
		// If a client isn't created properly, we need to clean up previously created ones here
		if (err != KErrNone)
			{
			for (TInt c=0; c<i-1; ++c)
				{
				proc[c].Kill(KErrNone);
				proc[c].Close();
				}
			}
		// Fail the test if any client has not been created properly
		ASSERT_EQUALS(err, KErrNone);
		}
	// All processes created successfully, start their execution
	for (TInt i = 0; i < numClients; i++)
		{
		proc[i].Logon(req[i]);
		proc[i].Resume();
		// Pause here so that clients register in the order specified (i.e. As ordered in the array)
		// Resolves timing issues in WDP/SMP.
		User::After(500000);
		}
	//-------------------------------------------------------------------------
	
	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageDefaultHandler, data);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageDefaultHandler, KErrNone);
		
	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes, RMobileUssdMessaging::ETransferToDefaultHandler);	
	User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	//---------------------------------------------------------------------------
		
	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data2, 15);
	User::After(5 * 1000000);
	
	for (TInt i = 0; i < numClients; i++)
		{
		TRequestStatus status = req[i];
		TExitType exit = proc[i].ExitType();
		TInt reason = proc[i].ExitReason();
		
		// Kill/Close the clients before doing assert
		proc[i].Kill(KErrNone);
		proc[i].Close();

		const TBool checkRequestStatus = status.Int() == data1[i].iReqStatus;
		const TBool checkExitType = exit == data1[i].iExitType;
		TBool checkExitReason = ETrue;
		if (data1[i].iExitType == EExitKill)
			{
			checkExitReason = reason == data1[i].iExitReason;
			}

		if (!checkRequestStatus || !checkExitType || !checkExitReason)
			{
			// Some useful output in case of a failed test
			INFO_PRINTF2(_L("Client[%d] returned unexpected results:"),i);
			if (!checkRequestStatus)
				INFO_PRINTF3(_L("  [RequestStatus] Expected: %d , Actual: %d"),data1[i].iReqStatus,status.Int());
			if (!checkExitType)
				INFO_PRINTF3(_L("  [ExitType]      Expected: %d , Actual: %d"),data1[i].iExitType,exit);
			if (!checkExitReason)
				INFO_PRINTF3(_L("  [ExitReason]    Expected: %d , Actual: %d"),data1[i].iExitReason,reason);
			for (TInt c=i+1; c<numClients; ++c)
				{
				proc[c].Kill(KErrNone);
				proc[c].Close();
				}
			ASSERT_TRUE(checkRequestStatus);
			ASSERT_TRUE(checkExitType);
			ASSERT_TRUE(checkExitReason);
			}
		}	
	//---------------------------------------------------------------------------
	data1.Close();
	CleanupStack::PopAndDestroy(4, this); // data, data2, this, ussdMessaging
    }



/**
@SYMTestCaseID BA-CTSY-USSDM-USMDH-0003
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY client requests to RMobileUssdMessaging::SendMessage with default and priority clients
@SYMTestPriority High
@SYMTestActions Invokes request to RMobileUssdMessaging::SendMessage with EMobileUssdMessagingSendMessageDefaultHandler option
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageDefaultHandlerWithTestClient0003L()
    {	
	
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RArray<TUssdClientTestData> data1;
	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);
	mockData2.SerialiseL(data);	
	//-------------------------------------------------------------------------
	RBuf8 data2;
	CleanupClosePushL(data2);
	//-------------------------------------------------------------------------	
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;
	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	TBuf8<KMaxName> completeName = _L8("Name2");
	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
											mockData3(completeName, completeAttributes);
	mockData3.SerialiseL(data2);
	//-------------------------------------------------------------------------
	data1.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -M"), KRequestPending, EExitPending, KErrNone)); //Priority
	data1.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -M"), KErrNone, EExitKill, KErrNone)); //Default
	const TInt numClients = data1.Count();
	RProcess* proc = new RProcess[numClients];
	TRequestStatus* req = new TRequestStatus[numClients];
	
	for (TInt i=0; i<numClients; ++i)
		{
		TInt err = proc[i].Create(data1[i].iExe, data1[i].iClientParams);
		// If a client isn't created properly, we need to clean up previously created ones here
		if (err != KErrNone)
			{
			for (TInt c=0; c<i-1; ++c)
				{
				proc[c].Kill(KErrNone);
				proc[c].Close();
				}
			}
		// Fail the test if any client has not been created properly
		ASSERT_EQUALS(err, KErrNone);
		}
	// All processes created successfully, start their execution
	for (TInt i = 0; i < numClients; i++)
		{
		proc[i].Logon(req[i]);
		proc[i].Resume();
		// Pause here so that clients register in the order specified (i.e. As ordered in the array)
		// Resolves timing issues in WDP/SMP.
		User::After(500000);
		}
	//-------------------------------------------------------------------------
	
	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageDefaultHandler, data);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageDefaultHandler, KErrNone);
		
	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes, RMobileUssdMessaging::ETransferToDefaultHandler);	
	User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	//---------------------------------------------------------------------------
		
	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data2, 15);
	User::After(5 * 1000000);
	
	for (TInt i = 0; i < numClients; i++)
		{
		TRequestStatus status = req[i];
		TExitType exit = proc[i].ExitType();
		TInt reason = proc[i].ExitReason();
		
		// Kill/Close the clients before doing assert
		proc[i].Kill(KErrNone);
		proc[i].Close();

		const TBool checkRequestStatus = status.Int() == data1[i].iReqStatus;
		const TBool checkExitType = exit == data1[i].iExitType;
		TBool checkExitReason = ETrue;
		if (data1[i].iExitType == EExitKill)
			{
			checkExitReason = reason == data1[i].iExitReason;
			}

		if (!checkRequestStatus || !checkExitType || !checkExitReason)
			{
			// Some useful output in case of a failed test
			INFO_PRINTF2(_L("Client[%d] returned unexpected results:"),i);
			if (!checkRequestStatus)
				INFO_PRINTF3(_L("  [RequestStatus] Expected: %d , Actual: %d"),data1[i].iReqStatus,status.Int());
			if (!checkExitType)
				INFO_PRINTF3(_L("  [ExitType]      Expected: %d , Actual: %d"),data1[i].iExitType,exit);
			if (!checkExitReason)
				INFO_PRINTF3(_L("  [ExitReason]    Expected: %d , Actual: %d"),data1[i].iExitReason,reason);
			for (TInt c=i+1; c<numClients; ++c)
				{
				proc[c].Kill(KErrNone);
				proc[c].Close();
				}
			ASSERT_TRUE(checkRequestStatus);
			ASSERT_TRUE(checkExitType);
			ASSERT_TRUE(checkExitReason);
			}
		}	
	//---------------------------------------------------------------------------
	data1.Close();
	CleanupStack::PopAndDestroy(4, this); // data, data2, this, ussdMessaging
    }


/**
@SYMTestCaseID BA-CTSY-USSDM-USMDH-0004
@SYMComponent telephony_ctsy
@SYMTestCaseDesc Test support in CTSY client requests to RMobileUssdMessaging::SendMessage with default, normal and priority clients
@SYMTestPriority High
@SYMTestActions Invokes request to RMobileUssdMessaging::SendMessage with EMobileUssdMessagingSendMessageDefaultHandler option
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyUssdMessagingFU::TestSendMessageDefaultHandlerWithTestClient0004L()
    {	
	
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	RArray<TUssdClientTestData> data1;
	RMobileUssdMessaging ussdMessaging;
	TInt ret = ussdMessaging.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(ussdMessaging);
	
	TRequestStatus requestStatus;

	TUint32 flags = 100;
	RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
	RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
	TUint8 dcs = 200;

	//-------------------------------------------------------------------------
	// function parameters

	RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
	TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
	
	TName name = _L("Name1");
	TPckg<TName> msgData(name);

	//-------------------------------------------------------------------------

	TMockLtsyData2<TName, RMobileUssdMessaging::TMobileUssdAttributesV1> 
		mockData2(name, attributes);
	mockData2.SerialiseL(data);	
	//-------------------------------------------------------------------------
	RBuf8 data2;
	CleanupClosePushL(data2);
	//-------------------------------------------------------------------------	
	RMobileUssdMessaging::TMobileUssdAttributesV1 completeAttributes;
	completeAttributes.iFlags  = flags;
	completeAttributes.iFormat = format;
	completeAttributes.iType   = type;
	completeAttributes.iDcs    = dcs;
	TBuf8<KMaxName> completeName = _L8("Name2");
	TMockLtsyData2<TBuf8<KMaxName>, RMobileUssdMessaging::TMobileUssdAttributesV1> 
											mockData3(completeName, completeAttributes);
	mockData3.SerialiseL(data2);
	//-------------------------------------------------------------------------
	data1.Append(TUssdClientTestData(_L("ussdtestclient_priority.exe"),_L("-R -M"), KRequestPending, EExitPending, KErrNone)); //Priority
	data1.Append(TUssdClientTestData(_L("ussdtestclient_normal.exe"),_L("-R -M"), KRequestPending, EExitPending, KErrNone)); //Normal
	data1.Append(TUssdClientTestData(_L("ussdtestclient_default.exe"), _L("-R -M"), KErrNone, EExitKill, KErrNone)); //Default
	const TInt numClients = data1.Count();
	RProcess* proc = new RProcess[numClients];
	TRequestStatus* req = new TRequestStatus[numClients];
	
	for (TInt i=0; i<numClients; ++i)
		{
		TInt err = proc[i].Create(data1[i].iExe, data1[i].iClientParams);
		// If a client isn't created properly, we need to clean up previously created ones here
		if (err != KErrNone)
			{
			for (TInt c=0; c<i-1; ++c)
				{
				proc[c].Kill(KErrNone);
				proc[c].Close();
				}
			}
		// Fail the test if any client has not been created properly
		ASSERT_EQUALS(err, KErrNone);
		}
	// All processes created successfully, start their execution
	for (TInt i = 0; i < numClients; i++)
		{
		proc[i].Logon(req[i]);
		proc[i].Resume();
		// Pause here so that clients register in the order specified (i.e. As ordered in the array)
		// Resolves timing issues in WDP/SMP.
		User::After(500000);
		}
	//-------------------------------------------------------------------------
	
	iMockLTSY.ExpectL(EMobileUssdMessagingSendMessageDefaultHandler, data);
	iMockLTSY.CompleteL(EMobileUssdMessagingSendMessageDefaultHandler, KErrNone);
		
	ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes, RMobileUssdMessaging::ETransferToDefaultHandler);	
	User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	//---------------------------------------------------------------------------
		
	iMockLTSY.CompleteL(EMobileUssdMessagingReceiveMessage, KErrNone, data2, 15);
	User::After(5 * 1000000);
	
	for (TInt i = 0; i < numClients; i++)
		{
		TRequestStatus status = req[i];
		TExitType exit = proc[i].ExitType();
		TInt reason = proc[i].ExitReason();
		
		// Kill/Close the clients before doing assert
		proc[i].Kill(KErrNone);
		proc[i].Close();

		const TBool checkRequestStatus = status.Int() == data1[i].iReqStatus;
		const TBool checkExitType = exit == data1[i].iExitType;
		TBool checkExitReason = ETrue;
		if (data1[i].iExitType == EExitKill)
			{
			checkExitReason = reason == data1[i].iExitReason;
			}

		if (!checkRequestStatus || !checkExitType || !checkExitReason)
			{
			// Some useful output in case of a failed test
			INFO_PRINTF2(_L("Client[%d] returned unexpected results:"),i);
			if (!checkRequestStatus)
				INFO_PRINTF3(_L("  [RequestStatus] Expected: %d , Actual: %d"),data1[i].iReqStatus,status.Int());
			if (!checkExitType)
				INFO_PRINTF3(_L("  [ExitType]      Expected: %d , Actual: %d"),data1[i].iExitType,exit);
			if (!checkExitReason)
				INFO_PRINTF3(_L("  [ExitReason]    Expected: %d , Actual: %d"),data1[i].iExitReason,reason);
			for (TInt c=i+1; c<numClients; ++c)
				{
				proc[c].Kill(KErrNone);
				proc[c].Close();
				}
			ASSERT_TRUE(checkRequestStatus);
			ASSERT_TRUE(checkExitType);
			ASSERT_TRUE(checkExitReason);
			}
		}	
	//---------------------------------------------------------------------------
	data1.Close();
	CleanupStack::PopAndDestroy(4, this); // data, data2, this, ussdMessaging
    }

