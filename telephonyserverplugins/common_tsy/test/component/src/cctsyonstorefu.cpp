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
// The TEFUnit test suite for ONStore in the Common TSY.
// 
//

/**
 @file 
*/

#include "cctsyonstorefu.h"
#include <etel.h>
#include <etelmm.h>
#include <et_clsvr.h>
#include <ctsy/mmtsy_names.h>
#include <ctsy/serviceapi/mmtsy_ipcdefs.h>
#include "tmockltsydata.h"
#include <ctsy/serviceapi/gsmerror.h>
#include "mmlist.h"
#include <ctsy/serviceapi/mmtsy_defaults.h>
#include "cctsyactiveretriever.h"
#include <mmretrieve.h>

CTestSuite* CCTsyONStoreFU::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;

	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStoreAllL0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStoreAllL0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStoreAllL0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStoreAllL0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStoreAllL0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestNotifyStoreEvent0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestNotifyStoreEvent0002L);
	//ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestNotifyStoreEvent0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestNotifyStoreEvent0004L);
	//ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestNotifyStoreEvent0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestRead0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestRead0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestRead0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestRead0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestRead0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestWrite0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestWrite0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestWrite0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestWrite0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestWrite0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDelete0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDelete0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDelete0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDelete0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDelete0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestGetInfo0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestGetInfo0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestGetInfo0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestGetInfo0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestGetInfo0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDeleteAll0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDeleteAll0002L);
	//ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDeleteAll0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDeleteAll0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestDeleteAll0005L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStart0001L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStart0002L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStart0003L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStart0004L);
	ADD_TEST_STEP_ISO_CPP(CCTsyONStoreFU, TestStart0005L);

	END_SUITE;
	}


//
// Actual test cases
//


/**
@SYMTestCaseID BA-CTSY-PBON-OSA-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::StoreAllL
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::StoreAllL
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStoreAllL0001L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

	RBuf8 reqData;
	CleanupClosePushL(reqData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
	
        
	TRequestStatus requestStatus;
	CMobilePhoneONList* onListPtr = CMobilePhoneONList::NewL();
    CleanupStack::PushL( onListPtr );
    
    TInt listMaxSize(-1);
	
    TMockLtsyData1<TInt> tsyData(listMaxSize);           
    tsyData.SerialiseL(expData);

    listMaxSize = 1;
    
    TMockLtsyData1<TInt> tsyData2(listMaxSize);           
    tsyData2.SerialiseL(completeData);


	RMobileONStore::TMobileONEntryV1 entry;
		
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData3(entry);           
    tsyData3.SerialiseL(reqData);
    
 	//-------------------------------------------------------------------------
	// TEST A1: failure to dispatch DeleteAll request to LTSY
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNotSupported);
	
    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());		

 	//-------------------------------------------------------------------------
	// TEST A2: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------
	
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData, KErrNotSupported);
	
    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	 	

 	//-------------------------------------------------------------------------
	// TEST A3: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData, KErrNotSupported);
	
    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	 	

	//-------------------------------------------------------------------------
	// TEST B1: failure on completion of pending DeleteAll request from LTSY->CTSY
	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrGeneral);
		
	onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());

	//-------------------------------------------------------------------------
	// TEST B2: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrGeneral, completeData);
	
    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	 	

	//-------------------------------------------------------------------------
	// TEST B3: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrGeneral, KNullDesC8);

    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	 	

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileONStore::StoreAllL when result is not cached.
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrNone, KNullDesC8);

    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	 	


 	//-------------------------------------------------------------------------
 	// TEST C2
	//-------------------------------------------------------------------------  	
	listMaxSize = 1;    
   
    TMockLtsyData1<TInt> tsyData5(listMaxSize);           
    completeData.Close();
    tsyData5.SerialiseL(completeData);

    _LIT(KText, "Some text");
    _LIT(KNumber, "1122121");

	entry.iMode = RMobilePhone::ENetworkModeGsm;
	entry.iService = RMobilePhone::ETelephony;
    entry.iNumber.iTypeOfNumber = RMobilePhone::EAlphanumericNumber;
    entry.iNumber.iNumberPlan = RMobilePhone::EDataNumberPlan;
    entry.iNumber.iTelNumber.Copy(KNumber);
	entry.iText.Copy(KText);

    onListPtr->AddEntryL(entry);  

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);

    reqData.Close();
    tsyData3.SerialiseL(reqData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrNone, KNullDesC8);
	
    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	 	


 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileONStore::StoreAllL
	// from LTSY.
 	//-------------------------------------------------------------------------
	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());	 	

	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrNone, KNullDesC8);
	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());	 	

	AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(6, this); // this, etc...
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSA-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileONStore::StoreAllL
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileONStore::StoreAllL
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStoreAllL0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);


	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

	RBuf8 reqData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
	
        
	TRequestStatus requestStatus;
	CMobilePhoneONList* onListPtr = CMobilePhoneONList::NewL();
    CleanupStack::PushL( onListPtr );
    
    TInt listMaxSize(-1);
	
    TMockLtsyData1<TInt> tsyData(listMaxSize);           
    tsyData.SerialiseL(expData);

    listMaxSize = 1;
    
    TMockLtsyData1<TInt> tsyData2(listMaxSize);           
    tsyData2.SerialiseL(completeData);

	RMobileONStore::TMobileONEntryV1 entry;
		
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData3(entry);           
    tsyData3.SerialiseL(reqData);
    
 	//-------------------------------------------------------------------------
	// Test A: cancelling of RMobileONStore::StoreAllL
 	//-------------------------------------------------------------------------
	
    iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	
	onStore.StoreAllL(requestStatus, onListPtr);
	
	onStore.CancelAsyncRequest(EMobileONStoreStoreAll);
	
	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrCancel, requestStatus.Int());	
	User::WaitForRequest(mockLtsyStatus);
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

 	//-------------------------------------------------------------------------
	// Test B: cancelling of RMobileONStore::StoreAllL
 	//-------------------------------------------------------------------------

    iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);

    onStore.StoreAllL(requestStatus, onListPtr);
    
	User::WaitForRequest(mockLtsyStatus);

    onStore.CancelAsyncRequest(EMobileONStoreStoreAll);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrCancel, requestStatus.Int());	 	
 	
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrNone, KNullDesC8);
	User::WaitForRequest(mockLtsyStatus);
	
	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(6); //this, etc...
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSA-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::StoreAllL with bad parameter data
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::StoreAllL with bad parameter data
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStoreAllL0003L()
	{

// This test should test sending bad parameter data for StoreAllL
// If this API does not have any parameters, then remove this test completely.

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
	
        
	TRequestStatus requestStatus;
	CMobilePhoneONList* onListPtr = CMobilePhoneONList::NewL();
    CleanupStack::PushL( onListPtr );
    
    TInt listMaxSize(-1);
	
    TMockLtsyData1<TInt> tsyData(listMaxSize);           
    tsyData.SerialiseL(expData);
    
    //-----------------------------------------

    listMaxSize = 0;    // bad size
    
    TMockLtsyData1<TInt> tsyData2(listMaxSize);           
    tsyData2.SerialiseL(completeData);

	RMobileONStore::TMobileONEntryV1 entry;

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrTooBig, requestStatus.Int());	 	
	
	AssertMockLtsyStatusL();
	
	//-------------------------------------
	
    listMaxSize = 1;    // good size
   
    TMockLtsyData1<TInt> tsyData5(listMaxSize);           
    completeData.Close();
    tsyData5.SerialiseL(completeData);

    onListPtr->AddEntryL(entry);    // big list
    onListPtr->AddEntryL(entry);
    onListPtr->AddEntryL(entry);

    iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
 	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
    onStore.StoreAllL(requestStatus, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrTooBig, requestStatus.Int());	 	

	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(5, this); // this, etc...
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSA-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileONStore::StoreAllL
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileONStore::StoreAllL
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStoreAllL0004L()
	{

					
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);


	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

	RBuf8 reqData;
	CleanupClosePushL(reqData);

    RMobileONStore  onStore;
    ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

    RMobileONStore  onStore2;
    ret = onStore2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore2);
	
        
	TRequestStatus requestStatus;
	TRequestStatus requestStatus2;
	CMobilePhoneONList* onListPtr = CMobilePhoneONList::NewL();
    CleanupStack::PushL( onListPtr );
    
    TInt listMaxSize(-1);
	
    TMockLtsyData1<TInt> tsyData(listMaxSize);           
    tsyData.SerialiseL(expData);

    listMaxSize = 1;
    
    TMockLtsyData1<TInt> tsyData2(listMaxSize);           
    tsyData2.SerialiseL(completeData);

	RMobileONStore::TMobileONEntryV1 entry;
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData3(entry);           
    tsyData3.SerialiseL(reqData);


    iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
 	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrNone, KNullDesC8);

    onStore.StoreAllL(requestStatus, onListPtr);

    iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
 	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrNone, KNullDesC8);

    onStore2.StoreAllL(requestStatus2, onListPtr);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	 	

	User::WaitForRequest(requestStatus2);	
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	 	

	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(9, this); // this, etc...

	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSA-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::StoreAllL with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::StoreAllL and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStoreAllL0005L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

	RBuf8 reqData;
	CleanupClosePushL(reqData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
	
        
	TRequestStatus requestStatus;
	CMobilePhoneONList* onListPtr = CMobilePhoneONList::NewL();
    CleanupStack::PushL( onListPtr );
    
    TInt listMaxSize(-1);
	
    TMockLtsyData1<TInt> tsyData(listMaxSize);           
    tsyData.SerialiseL(expData);

    listMaxSize = 1;
    
    TMockLtsyData1<TInt> tsyData2(listMaxSize);           
    tsyData2.SerialiseL(completeData);

	RMobileONStore::TMobileONEntryV1 entry;
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData3(entry);           
    tsyData3.SerialiseL(reqData);

    iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
 	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);

    onStore.StoreAllL(requestStatus, onListPtr);

	ERR_PRINTF2(_L("<font color=Orange>$CTSYKnownFailure: defect id = %d</font>"), 160101);	
	ASSERT_TRUE(EFalse);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());	 	

	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(6, this); // data, this
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSNSE-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::NotifyStoreEvent
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::NotifyStoreEvent
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestNotifyStoreEvent0001L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

	TRequestStatus requestStatus;
	TUint32 event;
	TInt index(-1);

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

	RBuf8 reqData;
	CleanupClosePushL(reqData);
        
	TRequestStatus requestStatus2;
	CMobilePhoneONList* onListPtr = CMobilePhoneONList::NewL();
    CleanupStack::PushL( onListPtr );
    
    TInt listMaxSize(-1);
	
    TMockLtsyData1<TInt> tsyData(listMaxSize);           
    tsyData.SerialiseL(expData);

    listMaxSize = 1;
    
    TMockLtsyData1<TInt> tsyData2(listMaxSize);           
    tsyData2.SerialiseL(completeData);

    _LIT(KText, "Some text");
    _LIT(KNumber, "1122121");

	RMobileONStore::TMobileONEntryV1 entry;
	
	entry.iMode = RMobilePhone::ENetworkModeGsm;
	entry.iService = RMobilePhone::ETelephony;
    entry.iNumber.iTypeOfNumber = RMobilePhone::EAlphanumericNumber;
    entry.iNumber.iNumberPlan = RMobilePhone::EDataNumberPlan;
    entry.iNumber.iTelNumber.Copy(KNumber);
	entry.iText.Copy(KText);

    onListPtr->AddEntryL(entry);  
	
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData3(entry);           
    tsyData3.SerialiseL(reqData);
    
    //-------------------------------------------------------------------------
	// Test NotifyStoreEvent with StoreAll completion
 	//-------------------------------------------------------------------------
    onStore.NotifyStoreEvent( requestStatus, event, index);


    iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
 	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrNone, KNullDesC8);

    // shoot with StoreAll
    onStore.StoreAllL(requestStatus2, onListPtr);

	User::WaitForRequest(requestStatus2);	
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	 	

    // wait notification about StoreAll
	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	 	
	ASSERT_TRUE(RMobilePhoneStore::KStoreDoRefresh == event);	 
	ASSERT_TRUE(-1 == index);	 
		
	
	//-------------------------------------------------------------------------
	// Test NotifyStoreEvent with Write completion
 	//-------------------------------------------------------------------------	
    onStore.NotifyStoreEvent( requestStatus, event, index);

    RMobileONStore::TMobileONEntryV1 info;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
    
    info.iIndex = -1;
	    
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData4(info);
    expData.Close();
    tsyData4.SerialiseL(expData);

    const TInt KLocation(2);
    TInt location(KLocation);

    TMockLtsyData1<TInt> tsyData5(location);           
    completeData.Close();
    tsyData5.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrNone, completeData);
	
    onStore.Write(requestStatus2, infoPckg);
    
    User::WaitForRequest(requestStatus2);  
    
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	

    // wait notification about Write
	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	 	
	ASSERT_TRUE(RMobilePhoneStore::KStoreEntryAdded == event);	 	
	ASSERT_TRUE(KLocation == index);	 

    //-------------------------------------------------------------------------
	// Test NotifyStoreEvent with Delete completion
 	//-------------------------------------------------------------------------	
    onStore.NotifyStoreEvent( requestStatus, event, index);

    index = 1;
	    
    TMockLtsyData1<TInt> tsyData6(index);
    expData.Close();
    tsyData6.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteIPC, KErrNone);
	
    onStore.Delete(requestStatus2, index);
    
    User::WaitForRequest(requestStatus2);        
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	

    // wait notification about StoreAll
	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	 	
	ASSERT_TRUE(RMobilePhoneStore::KStoreEntryDeleted == event);	 	
	ASSERT_TRUE(1 == index);	 
	
	//-------------------------------------------------------------------------
	// Test NotifyStoreEvent with DeleteAll completion
 	//-------------------------------------------------------------------------
    onStore.NotifyStoreEvent( requestStatus, event, index);

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	onStore.DeleteAll(requestStatus2);

    User::WaitForRequest(requestStatus2);        
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	

    // wait notification about StoreAll
	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	 	
	ASSERT_TRUE(RMobilePhoneStore::KStoreEmpty == event);
	ASSERT_TRUE(-1 == index);	 
	
	AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(6, this); // this, etc...
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSNSE-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileONStore::NotifyStoreEvent
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileONStore::NotifyStoreEvent
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestNotifyStoreEvent0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

 	//-------------------------------------------------------------------------
	// Test cancelling of RMobileONStore::NotifyStoreEvent
 	//-------------------------------------------------------------------------
	TRequestStatus requestStatus;
	TUint32 event;
	TInt index(-1);

    onStore.NotifyStoreEvent( requestStatus, event, index);
    
    onStore.CancelAsyncRequest(EMobilePhoneStoreNotifyStoreEvent);

	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrCancel, requestStatus.Int());	 	

	CleanupStack::PopAndDestroy(3); // this, etc...
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSNSE-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileONStore::NotifyStoreEvent
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileONStore::NotifyStoreEvent
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestNotifyStoreEvent0004L()
	{
				
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);


    RMobileONStore  onStore;
    ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

    RMobileONStore  onStore2;
    ret = onStore2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore2);

	TRequestStatus requestStatus;
	TUint32 event;
	TInt index(-1);

	TRequestStatus requestStatus2;
	TUint32 event2;
	TInt index2(-1);

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

	RBuf8 reqData;
	CleanupClosePushL(reqData);
        
	TRequestStatus requestStatus3;
	CMobilePhoneONList* onListPtr = CMobilePhoneONList::NewL();
    CleanupStack::PushL( onListPtr );
    
    TInt listMaxSize(-1);
	
    TMockLtsyData1<TInt> tsyData(listMaxSize);           
    tsyData.SerialiseL(expData);

    listMaxSize = 1;
    
    TMockLtsyData1<TInt> tsyData2(listMaxSize);           
    tsyData2.SerialiseL(completeData);

	RMobileONStore::TMobileONEntryV1 entry;
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData3(entry);           
    tsyData3.SerialiseL(reqData);


    onStore.NotifyStoreEvent( requestStatus, event, index);
    onStore2.NotifyStoreEvent( requestStatus2, event2, index2);

    iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
 	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteSizeIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteSizeIPC, KErrNone, completeData);
	
	iMockLTSY.ExpectL(EMmTsyONStoreWriteEntryIPC, reqData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteEntryIPC, KErrNone, KNullDesC8);

    // shoot with StoreAll
    onStore.StoreAllL(requestStatus3, onListPtr);

	User::WaitForRequest(requestStatus3);	
	ASSERT_EQUALS(KErrNone, requestStatus3.Int());	 	

    // get notify about StoreAll
	User::WaitForRequest(requestStatus);	
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	 	
	ASSERT_TRUE(RMobilePhoneStore::KStoreDoRefresh == event);	 	

	User::WaitForRequest(requestStatus2);	
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	 	
	ASSERT_TRUE(RMobilePhoneStore::KStoreDoRefresh == event2);	 	


	CleanupStack::PopAndDestroy(9, this); // phone2, telServer2, expData, this
	}



/**
@SYMTestCaseID BA-CTSY-PBON-OSR-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::Read
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::Read
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestRead0001L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

		
 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//------------------------------------------------------------------------- 
	TRequestStatus requestStatus;
    RMobileONStore::TMobileONEntryV1 info;  
    info.iMode = RMobilePhone::ENetworkModeGsm;
	info.iService = RMobilePhone::ETelephony;

    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
	TInt readIndex(-1); 
	    
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreReadIPC, expData, KErrNotSupported);
	
    onStore.Read(requestStatus, infoPckg);
    
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	
	
    //-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//------------------------------------------------------------------------- 	    
    const TUint16 KLocation(2);
    _LIT(KName, "Name");
    _LIT(KNumber, "Number");
 	
    TBuf<RMobileONStore::KOwnNumberTextSize> name(KName);
    TBuf<KPBStoreNumSize> number(KNumber);
 	
    TONStoreMsg ONStoreResp = {KLocation, name, number};

    TMockLtsyData1<TONStoreMsg> tsyData2(ONStoreResp);
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreReadIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreReadIPC, KErrGeneral, completeData);
	
    onStore.Read(requestStatus, infoPckg);
    
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileONStore::Read when result is not cached.
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreReadIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreReadIPC, KErrNone, completeData);
	
    onStore.Read(requestStatus, infoPckg);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	AssertMockLtsyStatusL();
	ASSERT_TRUE(KLocation == info.iIndex);
	ASSERT_TRUE(0 == info.iText.Compare(KName));
	ASSERT_TRUE(0 == info.iNumber.iTelNumber.Compare(KNumber));
	ASSERT_TRUE(RMobilePhone::ENetworkModeUnknown == info.iMode);
	ASSERT_TRUE(RMobilePhone::EServiceUnspecified == info.iService);

 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileONStore::Read
	// from LTSY.
 	//-------------------------------------------------------------------------
	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	
	iMockLTSY.CompleteL(EMmTsyONStoreReadIPC, KErrNone, completeData);

	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());	


	AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(4, this); // this, etc...
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSR-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileONStore::Read
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileONStore::Read
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestRead0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

		
	TRequestStatus requestStatus;
    RMobileONStore::TMobileONEntryV1 info;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
	TInt readIndex(-1); 
	    
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);

    const TUint16 KLocation(2);
    _LIT(KName, "Name");
    _LIT(KNumber, "Number");

    TBuf<RMobileONStore::KOwnNumberTextSize> name(KName);
    TBuf<KPBStoreNumSize> number(KNumber);
     	
    TONStoreMsg ONStoreResp = {KLocation, name, number};

    TMockLtsyData1<TONStoreMsg> tsyData2(ONStoreResp);           
    tsyData2.SerialiseL(completeData);

 	//-------------------------------------------------------------------------
	// Test cancelling of RMobileONStore::Read
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreReadIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreReadIPC, KErrNone, completeData);
	
    onStore.Read(requestStatus, infoPckg);
    
    onStore.CancelAsyncRequest(EMobilePhoneStoreRead);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrCancel, requestStatus.Int());	
	
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(4); // this, etc...
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSR-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for bad parameters of RMobileONStore::Read
@SYMTestPriority High
@SYMTestActions Invokes bad parameters test for RMobileONStore::Read
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestRead0003L()
    {

    OpenEtelServerL(EUseExtendedError);
    CleanupStack::PushL(TCleanupItem(Cleanup,this));
    OpenPhoneL();

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
    ASSERT_EQUALS(KErrNone, ret);
    CleanupClosePushL(onStore);

    TRequestStatus requestStatus;
    RMobileNamStore::TMobileNamEntryV1 info;    // bad param 
    RMobileNamStore::TMobileNamEntryV1Pckg infoPckg(info);

    onStore.Read(requestStatus, infoPckg);

    User::WaitForRequest(requestStatus);
    ASSERT_EQUALS(KErrArgument, requestStatus.Int());

    AssertMockLtsyStatusL();

    CleanupStack::PopAndDestroy(2); // this, onStore

    }


/**
@SYMTestCaseID BA-CTSY-PBON-OSR-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileONStore::Read
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileONStore::Read
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestRead0004L()
	{

					
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

    RMobileONStore  onStore2;
    ret = onStore2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore2);

		
	TRequestStatus requestStatus;
	TRequestStatus requestStatus2;
	
    RMobileONStore::TMobileONEntryV1 info;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);

    RMobileONStore::TMobileONEntryV1 info2;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg2(info2);
	TInt readIndex(-1); 
	    
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);
 	    
    const TUint16 KLocation(2);
    _LIT(KName, "Name");
    _LIT(KNumber, "Number");
 	
    TBuf<RMobileONStore::KOwnNumberTextSize> name(KName);
    TBuf<KPBStoreNumSize> number(KNumber);
 	
    TONStoreMsg ONStoreResp = {KLocation, name, number};

    TMockLtsyData1<TONStoreMsg> tsyData2(ONStoreResp);           
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreReadIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreReadIPC, KErrNone, completeData);
	
    onStore.Read(requestStatus, infoPckg);

	iMockLTSY.ExpectL(EMmTsyONStoreReadIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreReadIPC, KErrNone, completeData);
	
    onStore2.Read(requestStatus2, infoPckg2);
    
    User::WaitForRequest(requestStatus);   
         
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	ASSERT_TRUE(KLocation == info.iIndex);
	ASSERT_TRUE(0 == info.iText.Compare(KName));
	ASSERT_TRUE(0 == info.iNumber.iTelNumber.Compare(KNumber));
	ASSERT_TRUE(RMobilePhone::ENetworkModeUnknown == info.iMode);
	ASSERT_TRUE(RMobilePhone::EServiceUnspecified == info.iService);

    User::WaitForRequest(requestStatus2);   
         
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	
	ASSERT_TRUE(KLocation == info2.iIndex);
	ASSERT_TRUE(0 == info2.iText.Compare(KName));
	ASSERT_TRUE(0 == info2.iNumber.iTelNumber.Compare(KNumber));
	ASSERT_TRUE(RMobilePhone::ENetworkModeUnknown == info2.iMode);
	ASSERT_TRUE(RMobilePhone::EServiceUnspecified == info2.iService);

	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(7, this); // this, etc...

	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSR-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::Read with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::Read and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestRead0005L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

		
	TRequestStatus requestStatus;
    RMobileONStore::TMobileONEntryV1 info;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
	TInt readIndex(-1); 
	    
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);

    const TUint16 KLocation(2);
    _LIT(KName, "Name");
    _LIT(KNumber, "Number");
 	
    TBuf<RMobileONStore::KOwnNumberTextSize> name(KName);
    TBuf<KPBStoreNumSize> number(KNumber);
 	
    TONStoreMsg ONStoreResp = {KLocation, name, number};

    TMockLtsyData1<TONStoreMsg> tsyData2(ONStoreResp);           
    tsyData2.SerialiseL(completeData);


	iMockLTSY.ExpectL(EMmTsyONStoreReadIPC, expData);	
    onStore.Read(requestStatus, infoPckg);

	ERR_PRINTF2(_L("<font color=Orange>$CTSYKnownFailure: defect id = %d</font>"), 160301);	
	ASSERT_TRUE(EFalse);
        
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());		

	AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(4, this); // this, etc...
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSW-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::Write
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::Write
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestWrite0001L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

		
 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------  
	TRequestStatus requestStatus;
    RMobileONStore::TMobileONEntryV1 info;  
    
    _LIT(KText, "Some text");
    _LIT(KNumber, "1122121");

	info.iMode = RMobilePhone::ENetworkModeGsm;
	info.iService = RMobilePhone::ETelephony;
    info.iNumber.iTypeOfNumber = RMobilePhone::EAlphanumericNumber;
    info.iNumber.iNumberPlan = RMobilePhone::EDataNumberPlan;
    info.iNumber.iTelNumber.Copy(KNumber);
	info.iText.Copy(KText);
    
    
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
    
    info.iIndex = -1;
	    
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData(info);
    tsyData.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData, KErrNotSupported);
	
    onStore.Write(requestStatus, infoPckg);
    
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	
	
    //-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//------------------------------------------------------------------------- 	    
    const TInt KLocation(2);
    TInt location(KLocation);

    TMockLtsyData1<TInt> tsyData2(location);           
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrGeneral, completeData);
	
    onStore.Write(requestStatus, infoPckg);
    
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileONStore::Read when result is not cached.
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrNone, completeData);
	
    onStore.Write(requestStatus, infoPckg);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	ASSERT_EQUALS(KLocation, info.iIndex);	
	AssertMockLtsyStatusL();

 	//-------------------------------------------------------------------------
	// TEST C2
 	//-------------------------------------------------------------------------
    const TInt KIndex(1);
    info.iIndex = KIndex;
	    
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData3(info);
    expData.Close();
    tsyData3.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrNone, completeData);
	
    onStore.Write(requestStatus, infoPckg);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	ASSERT_EQUALS(KIndex, info.iIndex);	
	AssertMockLtsyStatusL();

 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileONStore::Read
	// from LTSY.
 	//-------------------------------------------------------------------------
	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrNone, completeData);

	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());	

	AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(4, this); // this, etc...
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSW-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileONStore::Write
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileONStore::Write
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestWrite0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

	TRequestStatus requestStatus;
    RMobileONStore::TMobileONEntryV1 info;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
    
    info.iIndex = -1;
	    
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData(info);
    tsyData.SerialiseL(expData);

    TInt location(2);

    TMockLtsyData1<TInt> tsyData2(location);           
    tsyData2.SerialiseL(completeData);
    

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrNone, completeData);
	
    onStore.Write(requestStatus, infoPckg);
    
    onStore.CancelAsyncRequest(EMobilePhoneStoreWrite);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	

	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());	
	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(4); // this, etc...
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSW-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::Write with bad parameter expData
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::Write with bad parameter expData
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestWrite0003L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

	TRequestStatus requestStatus;
    RMobileONStore::TMobileONEntryV1 info;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
    
    info.iIndex = -2;   //bad index
	    
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData(info);
    tsyData.SerialiseL(expData);

    const TInt KLocation(2);
    TInt location(KLocation);

    TMockLtsyData1<TInt> tsyData2(location);           
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrNone, completeData);
	
    onStore.Write(requestStatus, infoPckg);
    
    User::WaitForRequest(requestStatus);  
    
	ERR_PRINTF2(_L("<font color=Orange>$CTSYKnownFailure: defect id = %d</font>"), 160401);	
          
	ASSERT_EQUALS(KLocation, info.iIndex);	
	ASSERT_TRUE(KErrNone != requestStatus.Int());	

	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(4); // this, etc...
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSW-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileONStore::Write
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileONStore::Write
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestWrite0004L()
	{

					
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

    RMobileONStore  onStore2;
    ret = onStore2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore2);

	TRequestStatus requestStatus;
	TRequestStatus requestStatus2;

    RMobileONStore::TMobileONEntryV1 info;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
    
    info.iIndex = -1; 

    RMobileONStore::TMobileONEntryV1 info2;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg2(info2);
    
    info2.iIndex = -1; 
	    
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData(info);
    tsyData.SerialiseL(expData);

    const TInt KLocation(2);
    TInt location(KLocation);

    TMockLtsyData1<TInt> tsyData2(location);           
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrNone, completeData);
	
    onStore.Write(requestStatus, infoPckg);

    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData3(info2);
    expData.Close();
    tsyData3.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreWriteIPC, KErrNone, completeData);
	
    onStore2.Write(requestStatus2, infoPckg2);
    
    User::WaitForRequest(requestStatus);  
	ASSERT_EQUALS(KErrNone, requestStatus.Int());
	ASSERT_EQUALS(KLocation, info.iIndex);	

    User::WaitForRequest(requestStatus2);                
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	
	ASSERT_EQUALS(KLocation, info.iIndex);

	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(7, this); 

	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSW-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::Write with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::Write and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestWrite0005L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

	TRequestStatus requestStatus;

    RMobileONStore::TMobileONEntryV1 info;  
    RMobileONStore::TMobileONEntryV1Pckg infoPckg(info);
    
    info.iIndex = 1; 
	    
    TMockLtsyData1<RMobileONStore::TMobileONEntryV1> tsyData(info);
    tsyData.SerialiseL(expData);

    const TInt KLocation(2);
    TInt location(KLocation);

    TMockLtsyData1<TInt> tsyData2(location);           
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreWriteIPC, expData);
	
	ERR_PRINTF2(_L("<font color=Orange>$CTSYKnownFailure: defect id = %d</font>"), 160402);	
	ASSERT_TRUE(EFalse);
	
    onStore.Write(requestStatus, infoPckg);

    User::WaitForRequest(requestStatus);  
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());	

	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(4, this); 

	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSD-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::Delete
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::Delete
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDelete0001L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

		
 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------    
	TRequestStatus requestStatus;
    TInt index = 1;
	    
    TMockLtsyData1<TInt> tsyData(index);
    tsyData.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteIPC, expData, KErrNotSupported);
	
    onStore.Delete(requestStatus, index);
    
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	
	
    //-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//------------------------------------------------------------------------- 	    
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteIPC, KErrGeneral);
	
    onStore.Delete(requestStatus, index);
    
    User::WaitForRequest(requestStatus);        
	AssertMockLtsyStatusL();
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileONStore::Read when result is not cached.
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteIPC, KErrNone);
	
    onStore.Delete(requestStatus, index);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	AssertMockLtsyStatusL();

 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileONStore::Delete
	// from LTSY.
 	//-------------------------------------------------------------------------
	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
	
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteIPC, KErrNone);

	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());	


	AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(3, this); // this, etc...
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSD-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileONStore::Delete
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileONStore::Delete
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDelete0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

	RBuf8 expData;
	CleanupClosePushL(expData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

		
	TRequestStatus requestStatus;
    TInt index = 1;
	    
    TMockLtsyData1<TInt> tsyData(index);
    tsyData.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteIPC, KErrNone);
	
    onStore.Delete(requestStatus, index);
    onStore.CancelAsyncRequest(EMobilePhoneStoreDelete);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
	AssertMockLtsyStatusL();

	User::WaitForRequest(mockLtsyStatus);
	CleanupStack::PopAndDestroy(3); // expData, this
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSD-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::Delete with bad parameter expData
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::Delete with bad parameter expData
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDelete0003L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
		
	TRequestStatus requestStatus;
    TInt index = -1;
	    
    onStore.Delete(requestStatus, index);  
      
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	
	AssertMockLtsyStatusL();


	CleanupStack::PopAndDestroy(2, this); 

	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSD-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileONStore::Delete
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileONStore::Delete
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDelete0004L()
	{
						
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	RBuf8 expData;
	CleanupClosePushL(expData);

    RMobileONStore  onStore;
    ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

    RMobileONStore  onStore2;
    ret = onStore2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore2);

		
	TRequestStatus requestStatus;
	TRequestStatus requestStatus2;
    TInt index = 1;
	    
    TMockLtsyData1<TInt> tsyData(index);
    tsyData.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteIPC, KErrNone);
	
    onStore.Delete(requestStatus, index);

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteIPC, KErrNone);
	
    onStore2.Delete(requestStatus2, index);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	

    User::WaitForRequest(requestStatus2);        
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	

	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(6, this); // phone2, telServer2, expData, this

	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSD-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::Delete with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::Delete and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDelete0005L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

		
	TRequestStatus requestStatus;
    TInt index = 1;
	    
    TMockLtsyData1<TInt> tsyData(index);
    tsyData.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteIPC, expData);
	
	ERR_PRINTF2(_L("<font color=Orange>$CTSYKnownFailure: defect id = %d</font>"), 160501);	
	ASSERT_TRUE(EFalse);
	
    onStore.Delete(requestStatus, index);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());	
	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(3, this); // expData, this
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSGI-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::GetInfo
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::GetInfo
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestGetInfo0001L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
		
		
	TRequestStatus requestStatus;
	RMobileONStore::TMobileONStoreInfoV1 storeInfo;
	RMobileONStore::TMobileONStoreInfoV1Pckg storePckg(storeInfo);
	
    TInt readIndex(-1);
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);

 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreGetInfoIPC, expData, KErrNotSupported);
	
	onStore.GetInfo(requestStatus, storePckg);		
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	
	
	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY	
 	//-------------------------------------------------------------------------
 	const TInt KNumOfEntries = 10;
 	const TInt KUsedEntries = 20;
 	const TInt KNameLen = 30;
 	const TInt KNumLen = 40;
 	
 	TServiceType serviceType = {KNumOfEntries, KUsedEntries, KNameLen, KNumLen};
    TMockLtsyData1<TServiceType> tsyData2(serviceType);           
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreGetInfoIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreGetInfoIPC, KErrGeneral, completeData);
	
	onStore.GetInfo(requestStatus, storePckg);		
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileONStore::GetInfo when result is not cached.
	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreGetInfoIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreGetInfoIPC, KErrNone, completeData);
	
	onStore.GetInfo(requestStatus, storePckg);		
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	
		
    ASSERT_EQUALS(RMobilePhoneStore::EOwnNumberStore, storeInfo.iType);
    ASSERT_EQUALS(KONStoreCaps, storeInfo.iCaps);
    ASSERT_TRUE(0 == storeInfo.iName.Compare(KETelOwnNumberStore));
    ASSERT_EQUALS(KUsedEntries, storeInfo.iUsedEntries);
    ASSERT_EQUALS(KNumOfEntries, storeInfo.iTotalEntries);
    ASSERT_EQUALS(KNumLen, storeInfo.iNumberLen);
    ASSERT_EQUALS(KNameLen, storeInfo.iTextLen);
		
    
 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileENStore::GetInfo
	// from LTSY.
 	//-------------------------------------------------------------------------
	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

	iMockLTSY.CompleteL(EMmTsyONStoreGetInfoIPC, KErrNone, completeData);

	User::WaitForRequest(mockLtsyStatus);

	AssertMockLtsyStatusL();
	
	//-------------------------------------------------------------------------
	// TEST F: DEF132954: GetInfo fails when another phonebook's initialisation
	// is in progress.
	//-------------------------------------------------------------------------
	
	TName name(KETelIccAdnPhoneBook);
	RMobilePhoneBookStore bookStore;
	
	RBuf8 data;
	CleanupClosePushL(data);
	    	
	RBuf8 data2;
	CleanupClosePushL(data2);
	    	
	iMockLTSY.NotifyTerminated(mockLtsyStatus);
              
	//EMmTsyPhoneBookStoreInitIPC
	TMockLtsyPhoneBookData0 storeInitData(name);
	storeInitData.SerialiseL(data);
	        
	iMockLTSY.ExpectL(EMmTsyPhoneBookStoreInitIPC, data);		
	
	//Open
	ret = bookStore.Open(iPhone, name);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(bookStore);
	
	User::WaitForRequest(mockLtsyStatus);        	
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int());	
	
	onStore.GetInfo(requestStatus, storePckg);		
	    
	User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNotReady, requestStatus.Int());

	AssertMockLtsyStatusL(); 	
		    		
	CleanupStack::PopAndDestroy(3); //data, data2, bookStore
	
	CleanupStack::PopAndDestroy(4, this); // this, etc...
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSGI-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileONStore::GetInfo
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileONStore::GetInfo
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestGetInfo0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
		
		
	TRequestStatus requestStatus;
	RMobileONStore::TMobileONStoreInfoV1 storeInfo;
	RMobileONStore::TMobileONStoreInfoV1Pckg storePckg(storeInfo);
	
    TInt readIndex(-1);
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);

 	const TInt KNumOfEntries = 10;
 	const TInt KUsedEntries = 20;
 	const TInt KNameLen = 30;
 	const TInt KNumLen = 40;
 	
 	TServiceType serviceType = {KNumOfEntries, KUsedEntries, KNameLen, KNumLen};
    TMockLtsyData1<TServiceType> tsyData2(serviceType);           
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreGetInfoIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreGetInfoIPC, KErrNone, completeData);
	
	onStore.GetInfo(requestStatus, storePckg);		
	
	onStore.CancelAsyncRequest(EMobilePhoneStoreGetInfo);
    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrCancel, requestStatus.Int());	
	
	User::WaitForRequest(mockLtsyStatus);
	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(4); // expData, this
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSGI-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Bad params test for RMobileONStore::GetInfo
@SYMTestPriority High
@SYMTestActions Invokes bad params test of RMobileONStore::GetInfo
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestGetInfo0003L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
	
    TInt8 storeInfo(-1);    // bad param 
    TPckg<TInt8> storePckg(storeInfo);

    TInt readIndex(-1);
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);

 	const TInt KNumOfEntries = 10;
 	const TInt KUsedEntries = 20;
 	const TInt KNameLen = 30;
 	const TInt KNumLen = 40;
 	
 	TServiceType serviceType = {KNumOfEntries, KUsedEntries, KNameLen, KNumLen};
    TMockLtsyData1<TServiceType> tsyData2(serviceType);           
    tsyData2.SerialiseL(completeData);

	TRequestStatus requestStatus;
	onStore.GetInfo(requestStatus, storePckg);				
	
    User::WaitForRequest(requestStatus);        
	ASSERT_TRUE(KErrNone != requestStatus.Int());	
	
	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(4); // onStore, completeData, expData, this	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSGI-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileONStore::GetInfo
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileONStore::GetInfo
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestGetInfo0004L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	RBuf8 expData;
	CleanupClosePushL(expData);

	RBuf8 completeData;
	CleanupClosePushL(completeData);

    RMobileONStore  onStore;
    ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

    RMobileONStore  onStore2;
    ret = onStore2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore2);
		
		
	TRequestStatus requestStatus;
	TRequestStatus requestStatus2;

	RMobileONStore::TMobileONStoreInfoV1 storeInfo;
	RMobileONStore::TMobileONStoreInfoV1Pckg storePckg(storeInfo);

	RMobileONStore::TMobileONStoreInfoV1 storeInfo2;
	RMobileONStore::TMobileONStoreInfoV1Pckg storePckg2(storeInfo2);
	
    TInt readIndex(-1);
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);

 	const TInt KNumOfEntries = 10;
 	const TInt KUsedEntries = 20;
 	const TInt KNameLen = 30;
 	const TInt KNumLen = 40;
 	
 	TServiceType serviceType = {KNumOfEntries, KUsedEntries, KNameLen, KNumLen};
    TMockLtsyData1<TServiceType> tsyData2(serviceType);           
    tsyData2.SerialiseL(completeData);

	iMockLTSY.ExpectL(EMmTsyONStoreGetInfoIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreGetInfoIPC, KErrNone, completeData);
	
	onStore.GetInfo(requestStatus, storePckg);		

	iMockLTSY.ExpectL(EMmTsyONStoreGetInfoIPC, expData);
	iMockLTSY.CompleteL(EMmTsyONStoreGetInfoIPC, KErrNone, completeData);
	
	onStore2.GetInfo(requestStatus2, storePckg2);		
	
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	

    ASSERT_EQUALS(RMobilePhoneStore::EOwnNumberStore, storeInfo.iType);
    ASSERT_EQUALS(KONStoreCaps, storeInfo.iCaps);
    ASSERT_TRUE(0 == storeInfo.iName.Compare(KETelOwnNumberStore));
    ASSERT_EQUALS(KUsedEntries, storeInfo.iUsedEntries);
    ASSERT_EQUALS(KNumOfEntries, storeInfo.iTotalEntries);
    ASSERT_EQUALS(KNumLen, storeInfo.iNumberLen);
    ASSERT_EQUALS(KNameLen, storeInfo.iTextLen);



    User::WaitForRequest(requestStatus2);        
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	

    ASSERT_EQUALS(RMobilePhoneStore::EOwnNumberStore, storeInfo2.iType);
    ASSERT_EQUALS(KONStoreCaps, storeInfo2.iCaps);
    ASSERT_TRUE(0 == storeInfo2.iName.Compare(KETelOwnNumberStore));
    ASSERT_EQUALS(KUsedEntries, storeInfo2.iUsedEntries);
    ASSERT_EQUALS(KNumOfEntries, storeInfo2.iTotalEntries);
    ASSERT_EQUALS(KNumLen, storeInfo2.iNumberLen);
    ASSERT_EQUALS(KNameLen, storeInfo2.iTextLen);
	
	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(7, this); 
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSGI-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::GetInfo with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::GetInfo and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestGetInfo0005L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 expData;
	CleanupClosePushL(expData);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
		
		
	TRequestStatus requestStatus;
	RMobileONStore::TMobileONStoreInfoV1 storeInfo;
	RMobileONStore::TMobileONStoreInfoV1Pckg storePckg(storeInfo);
	
    TInt readIndex(-1);
    TMockLtsyData1<TInt> tsyData(readIndex);           
    tsyData.SerialiseL(expData);

	iMockLTSY.ExpectL(EMmTsyONStoreGetInfoIPC, expData);
	
	onStore.GetInfo(requestStatus, storePckg);		
	    
	ERR_PRINTF2(_L("<font color=Orange>$CTSYKnownFailure: defect id = %d</font>"), 160602);	
	ASSERT_TRUE(EFalse);
	    
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());	
	
	CleanupStack::PopAndDestroy(3, this);

	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSDA-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::DeleteAll
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::DeleteAll
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDeleteAll0001L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
		
		
	TRequestStatus requestStatus;
	
 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNotSupported);

	onStore.DeleteAll(requestStatus);

    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNotSupported, requestStatus.Int());	

	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrGeneral);

	onStore.DeleteAll(requestStatus);

    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrGeneral, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// RMobileONStore::DeleteAll when result is not cached.
 	//-------------------------------------------------------------------------
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	onStore.DeleteAll(requestStatus);

    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	

 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of RMobileONStore::DeleteAll
	// from LTSY.
 	//-------------------------------------------------------------------------
	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	
	User::WaitForRequest(mockLtsyStatus);

	AssertMockLtsyStatusL();
	CleanupStack::PopAndDestroy(2, this);
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSDA-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of RMobileONStore::DeleteAll
@SYMTestPriority High
@SYMTestActions Invokes cancelling of RMobileONStore::DeleteAll
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDeleteAll0002L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
		
		
	TRequestStatus requestStatus;
	
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	onStore.DeleteAll(requestStatus);
	
	onStore.CancelAsyncRequest(EMobilePhoneStoreDeleteAll);

    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrCancel, requestStatus.Int());	

	User::WaitForRequest(mockLtsyStatus);

	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(2, this);	
	}




/**
@SYMTestCaseID BA-CTSY-PBON-OSDA-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to RMobileONStore::DeleteAll
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to RMobileONStore::DeleteAll
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDeleteAll0004L()
	{
				
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

    RMobileONStore  onStore;
    ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

    RMobileONStore  onStore2;
    ret = onStore2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore2);
		
		
	TRequestStatus requestStatus;
	TRequestStatus requestStatus2;
	
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	onStore.DeleteAll(requestStatus);

	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);
	iMockLTSY.CompleteL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	onStore2.DeleteAll(requestStatus2);
	
    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrNone, requestStatus.Int());	

    User::WaitForRequest(requestStatus2);        
	ASSERT_EQUALS(KErrNone, requestStatus2.Int());	

	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(5, this);		
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSDA-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileONStore::DeleteAll with timeout
@SYMTestPriority High
@SYMTestActions Invokes RMobileONStore::DeleteAll and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestDeleteAll0005L()
	{
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

    RMobileONStore  onStore;
    TInt ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);
		
		
	TRequestStatus requestStatus;
	
	iMockLTSY.ExpectL(EMmTsyONStoreDeleteAllIPC, KErrNone);

	onStore.DeleteAll(requestStatus);

	ERR_PRINTF2(_L("<font color=Orange>$CTSYKnownFailure: defect id = %d</font>"), 160701);	
	ASSERT_TRUE(EFalse);

    User::WaitForRequest(requestStatus);        
	ASSERT_EQUALS(KErrTimedOut, requestStatus.Int());	

	AssertMockLtsyStatusL();
	
	CleanupStack::PopAndDestroy(2, this);	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSS-0001
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for CRetrieveMobilePhoneONList::Start
@SYMTestPriority High
@SYMTestActions Invokes CRetrieveMobilePhoneONList::Start
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStart0001L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 reqData;
	CleanupClosePushL(reqData);

	CFilteringActiveScheduler scheduler;
	CActiveScheduler::Install(&scheduler);

	RMobileONStore store;
	TInt err = store.Open(iPhone);
	ASSERT_EQUALS(KErrNone, err);
	CleanupClosePushL(store);
	
	CRetrieveMobilePhoneONList* asyncRetrieveList = CRetrieveMobilePhoneONList::NewL(store);    
	CleanupStack::PushL(asyncRetrieveList);

	CActiveRetriever::ResetRequestsNumber();
	CActiveRetriever* activeRetriever = CActiveRetriever::NewL(*asyncRetrieveList);
	CleanupStack::PushL(activeRetriever);
	
	scheduler.AddRetrieverL(*activeRetriever);

 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------
	TInt index(-1);
    TMockLtsyData1<TInt>  expLtsyData(index);
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData, KErrNotSupported);

	asyncRetrieveList->Start(activeRetriever->iStatus);
	activeRetriever->Activate();
	scheduler.StartScheduler();

	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrNotSupported, activeRetriever->iStatus.Int());

	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------
 	
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

	TInt size(1);
    TMockLtsyData1<TInt>  complete0LtsyData(size);
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrGeneral, reqData);

	asyncRetrieveList->Start(activeRetriever->iStatus);
	activeRetriever->Activate();
	scheduler.StartScheduler();

	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrGeneral, activeRetriever->iStatus.Int());


 	//-------------------------------------------------------------------------
	// TEST A2: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------
    index = -1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrNone, reqData);

    index = 1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadEntryIPC, reqData, KErrNotSupported);
    
	asyncRetrieveList->Start(activeRetriever->iStatus);
	activeRetriever->Activate();
	scheduler.StartScheduler();

	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrNotSupported, activeRetriever->iStatus.Int());

 	//-------------------------------------------------------------------------
	// TEST B2: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------
    index = -1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrNone, reqData);

    index = 1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadEntryIPC, reqData);

	TONStoreMsg onStoreMsg;
	onStoreMsg.iLocation = 17;
	onStoreMsg.iName.Copy(_L("asta"));
	onStoreMsg.iTelNumber.Copy(_L("+441632960000"));
    TMockLtsyData1<TONStoreMsg>  complete1LtsyData(onStoreMsg);
    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);
    
    iMockLTSY.CompleteL(EMmTsyONStoreReadEntryIPC, KErrGeneral, reqData);
    
	asyncRetrieveList->Start(activeRetriever->iStatus);
	activeRetriever->Activate();
	scheduler.StartScheduler();

	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrGeneral, activeRetriever->iStatus.Int());

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// CAsyncRetrieveStoreList::Start when result is not cached.
 	//-------------------------------------------------------------------------

    index = -1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

    size = 2;
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrNone, reqData);

    index = 1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadEntryIPC, reqData);

    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);    
    iMockLTSY.CompleteL(EMmTsyONStoreReadEntryIPC, KErrNone, reqData);

    index = 2;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadEntryIPC, reqData);

    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);    
    iMockLTSY.CompleteL(EMmTsyONStoreReadEntryIPC, KErrNone, reqData);


	asyncRetrieveList->Start(activeRetriever->iStatus);
	activeRetriever->Activate();
	scheduler.StartScheduler();

	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrNone, activeRetriever->iStatus.Int());
	CMobilePhoneONList* list = asyncRetrieveList->RetrieveListL();
	CBufBase* listBuf = list->StoreLC();

	CMobilePhoneONList* completeList = CMobilePhoneONList::NewL();
	CleanupStack::PushL(completeList);
	RMobileONStore::TMobileONEntryV1 onEntry;
	onEntry.iMode                 = RMobilePhone::ENetworkModeUnknown;
	onEntry.iService              = RMobilePhone::EServiceUnspecified;
	onEntry.iIndex                = (onStoreMsg.iLocation);
	onEntry.iNumber.iTypeOfNumber = RMobilePhone::EUnknownNumber;
	onEntry.iNumber.iNumberPlan   = RMobilePhone::EUnknownNumberingPlan;
	onEntry.iNumber.iTelNumber.Copy(onStoreMsg.iTelNumber);
	onEntry.iText.Copy(onStoreMsg.iName);
	completeList->AddEntryL(onEntry);
	completeList->AddEntryL(onEntry);

	CBufBase* completeListBuf = completeList->StoreLC();

	ASSERT_EQUALS(completeListBuf->Ptr(0), listBuf->Ptr(0));

	AssertMockLtsyStatusL();


 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of CAsyncRetrieveStoreList::Start
	// from LTSY.
 	//-------------------------------------------------------------------------

    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);    
    iMockLTSY.CompleteL(EMmTsyONStoreReadEntryIPC, KErrNone, reqData);

	//-------------------------------------------------------------------------

	CArrayPtrFlat<TSmsMsg>* smsMsgArray = new ( ELeave ) CArrayPtrFlat<TSmsMsg>( 1 );
	CleanupStack::PushL(smsMsgArray);

	TSmsMsg smsMsg;
    smsMsg.iSmsMsg = _L8("Hello, World!"); 
    smsMsg.iServiceCentre = _L("ServiceSenter"); 
    smsMsg.iMobileScTON   = RMobilePhone::ENetworkSpecificNumber;  
    smsMsg.iMobileScNPI   = RMobilePhone::EServiceCentreSpecificPlan2;
    smsMsg.iMessageStatus = KSimSmsMtRead; // RMobileSmsStore::EStoredMessageRead
    smsMsg.iLocation      = 200;
    smsMsg.iDeleteAfterClientAck = EFalse;
    smsMsg.iSmsClass2            = ETrue;
	smsMsgArray->AppendL(&smsMsg, 1);
    
	TBool receivedClass2ToBeReSent(ETrue);
	TMockLtsyData2<CArrayPtrFlat< TSmsMsg >*, TBool> 
		completeSmsMsgLtsyData(smsMsgArray, receivedClass2ToBeReSent);

	//-------------------------------------------------------------------------

    reqData.Close();
    completeSmsMsgLtsyData.SerialiseL(reqData);    

    TRequestStatus requestStatus;
	iMockLTSY.NotifyTerminated(requestStatus);	
    iMockLTSY.CompleteL(EMobilePhoneStoreReadAllPhase1, KErrNone, reqData);
	User::WaitForRequest(requestStatus);
	ASSERT_EQUALS(KErrNone, requestStatus.Int());
	AssertMockLtsyStatusL();

	CleanupStack::PopAndDestroy(1); // smsMsgArray

	CActiveScheduler::Install(NULL);

	CleanupStack::PopAndDestroy(8, this);
	
	}

/**
@SYMTestCaseID BA-CTSY-PBON-OSS-0002
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for cancelling of CRetrieveMobilePhoneONList::Start
@SYMTestPriority High
@SYMTestActions Invokes cancelling of CRetrieveMobilePhoneONList::Start
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStart0002L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

	RBuf8 reqData;
	CleanupClosePushL(reqData);

	CFilteringActiveScheduler scheduler;
	CActiveScheduler::Install(&scheduler);

	RMobileONStore store;
	TInt err = store.Open(iPhone);
	ASSERT_EQUALS(KErrNone, err);
	CleanupClosePushL(store);
	
	CRetrieveMobilePhoneONList* onList = CRetrieveMobilePhoneONList::NewL(store);    
	CleanupStack::PushL(onList);

	CActiveRetriever::ResetRequestsNumber();
	CActiveRetriever* activeRetriever = CActiveRetriever::NewL(*onList);
	CleanupStack::PushL(activeRetriever);
	
	scheduler.AddRetrieverL(*activeRetriever);
	
	
	TInt index(-1);
    TMockLtsyData1<TInt>  expLtsyData(index);
    expLtsyData.SerialiseL(reqData);
	
	TInt size(1);
    TMockLtsyData1<TInt>  complete0LtsyData(size);
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
	
	TONStoreMsg onStoreMsg;
	onStoreMsg.iLocation = 17;
	onStoreMsg.iName.Copy(_L("asta"));
	onStoreMsg.iTelNumber.Copy(_L("+441632960000"));
    TMockLtsyData1<TONStoreMsg>  complete1LtsyData(onStoreMsg);
    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);
	
 	//-------------------------------------------------------------------------
	// Test cancelling of CRetrieveMobilePhoneONList::Start
 	//-------------------------------------------------------------------------
 	
    index = -1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

    size = 1;
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrNone, reqData);

    index = 1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadEntryIPC, reqData);

    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);    
    iMockLTSY.CompleteL(EMmTsyONStoreReadEntryIPC, KErrNone, reqData);


	onList->Start(activeRetriever->Status());
	activeRetriever->Activate();
	onList->Cancel();
	scheduler.StartScheduler();
	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrCancel, activeRetriever->Status().Int())
	
	User::WaitForRequest(mockLtsyStatus);
	ASSERT_EQUALS(KErrNone, mockLtsyStatus.Int())
	AssertMockLtsyStatusL();
	
	CActiveScheduler::Install(NULL);
	CleanupStack::PopAndDestroy(5); 
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSS-0003
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for CRetrieveMobilePhoneONList::Start with bad parameter expData
@SYMTestPriority High
@SYMTestActions Invokes CRetrieveMobilePhoneONList::Start with bad parameter expData
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStart0003L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 reqData;
	CleanupClosePushL(reqData);

	CFilteringActiveScheduler scheduler;
	CActiveScheduler::Install(&scheduler);

	RMobileONStore store;
	TInt err = store.Open(iPhone);
	ASSERT_EQUALS(KErrNone, err);
	CleanupClosePushL(store);
	
	CRetrieveMobilePhoneONList* onList = CRetrieveMobilePhoneONList::NewL(store);    
	CleanupStack::PushL(onList);

	CActiveRetriever::ResetRequestsNumber();
	CActiveRetriever* activeRetriever = CActiveRetriever::NewL(*onList);
	CleanupStack::PushL(activeRetriever);
	
	scheduler.AddRetrieverL(*activeRetriever);
	
	
	TInt index(-1);
    TMockLtsyData1<TInt>  expLtsyData(index);
    expLtsyData.SerialiseL(reqData);
	
	TInt size(1);
    TMockLtsyData1<TInt>  complete0LtsyData(size);
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
	
	TONStoreMsg onStoreMsg;
	onStoreMsg.iLocation = 17;
	onStoreMsg.iName.Copy(_L("asta"));
	onStoreMsg.iTelNumber.Copy(_L("+441632960000"));
    TMockLtsyData1<TONStoreMsg>  complete1LtsyData(onStoreMsg);
    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);
	
 	//-------------------------------------------------------------------------
	// Test size < 0
 	//-------------------------------------------------------------------------
 	
    index = -1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

    size = -1;
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrNone, reqData);

	onList->Start(activeRetriever->Status());
	activeRetriever->Activate();
	scheduler.StartScheduler();
	
	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrNotFound, activeRetriever->Status().Int())
	
	CActiveScheduler::Install(NULL);
	CleanupStack::PopAndDestroy(5); 
	
	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSS-0004
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for multiple client requests to CRetrieveMobilePhoneONList::Start
@SYMTestPriority High
@SYMTestActions Invokes multiple client requests to CRetrieveMobilePhoneONList::Start
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStart0004L()
	{

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup, this));
	OpenPhoneL();

	RBuf8 reqData;
	CleanupClosePushL(reqData);

	CFilteringActiveScheduler scheduler;
	CActiveScheduler::Install(&scheduler);
 	
	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(telServer2, KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

    RMobileONStore  onStore;
    ret = onStore.Open(iPhone);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore);

    RMobileONStore  onStore2;
    ret = onStore2.Open(phone2);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(onStore2);

	CRetrieveMobilePhoneONList* onList = CRetrieveMobilePhoneONList::NewL(onStore);    
	CleanupStack::PushL(onList);


	CRetrieveMobilePhoneONList* onList2 = CRetrieveMobilePhoneONList::NewL(onStore2);   
	CleanupStack::PushL(onList2);

	CActiveRetriever::ResetRequestsNumber();
	CActiveRetriever* activeRetriever = CActiveRetriever::NewL(*onList);
	CleanupStack::PushL(activeRetriever);
	scheduler.AddRetrieverL(*activeRetriever);

	CActiveRetriever* activeRetriever2 = CActiveRetriever::NewL(*onList2);
	CleanupStack::PushL(activeRetriever2);
	scheduler.AddRetrieverL(*activeRetriever2);
 	
	//-------------------------------------------------------------------------
	// Test multiple clients requesting CRetrieveMobilePhoneONList::Start
 	//-------------------------------------------------------------------------

	TInt index(-1);
    TMockLtsyData1<TInt>  expLtsyData(index);
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

	TInt size(1);
    TMockLtsyData1<TInt>  complete0LtsyData(size);
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrNone, reqData);

    index = 1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadEntryIPC, reqData);

	TONStoreMsg onStoreMsg;
	onStoreMsg.iLocation = 17;
	onStoreMsg.iName.Copy(_L("asta"));
	onStoreMsg.iTelNumber.Copy(_L("+441632960000"));
	
    TMockLtsyData1<TONStoreMsg>  complete1LtsyData(onStoreMsg);
    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);

    iMockLTSY.CompleteL(EMmTsyONStoreReadEntryIPC, KErrNone, reqData);
    

    // first
	onList->Start(activeRetriever->iStatus);
	activeRetriever->Activate();

    index = -1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

    size = 1;
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrNone, reqData);

    index = 1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadEntryIPC, reqData);

    reqData.Close();
    complete1LtsyData.SerialiseL(reqData);    
    iMockLTSY.CompleteL(EMmTsyONStoreReadEntryIPC, KErrNone, reqData);

    // setting and execute 2nd request
	onList2->Start(activeRetriever2->iStatus);
	activeRetriever2->Activate();

	// invoke scheduler
	scheduler.StartScheduler();

	// check results
	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrNone, activeRetriever->iStatus.Int());
	ASSERT_EQUALS(KErrNone, activeRetriever2->iStatus.Int());
	

	CMobilePhoneONList* list = onList->RetrieveListL();
	CBufBase* listBuf = list->StoreLC();

	CMobilePhoneONList* completeList = CMobilePhoneONList::NewL();
	CleanupStack::PushL(completeList);
	RMobileONStore::TMobileONEntryV1 onEntry;
	onEntry.iMode                 = RMobilePhone::ENetworkModeUnknown;
	onEntry.iService              = RMobilePhone::EServiceUnspecified;
	onEntry.iIndex                = (onStoreMsg.iLocation);
	onEntry.iNumber.iTypeOfNumber = RMobilePhone::EUnknownNumber;
	onEntry.iNumber.iNumberPlan   = RMobilePhone::EUnknownNumberingPlan;
	onEntry.iNumber.iTelNumber.Copy(onStoreMsg.iTelNumber);
	onEntry.iText.Copy(onStoreMsg.iName);
	completeList->AddEntryL(onEntry);

	CBufBase* completeListBuf = completeList->StoreLC();

	ASSERT_EQUALS(completeListBuf->Ptr(0), listBuf->Ptr(0));

	CMobilePhoneONList* list2 = onList2->RetrieveListL();
	CBufBase* listBuf2 = list2->StoreLC();

	ASSERT_EQUALS(completeListBuf->Ptr(0), listBuf2->Ptr(0));

	AssertMockLtsyStatusL();

	// Done !
	CActiveScheduler::Install(NULL);
	CleanupStack::PopAndDestroy(14, this);

	}


/**
@SYMTestCaseID BA-CTSY-PBON-OSS-0005
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for CRetrieveMobilePhoneONList::Start with timeout
@SYMTestPriority High
@SYMTestActions Invokes CRetrieveMobilePhoneONList::Start and tests for timeout
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
void CCTsyONStoreFU::TestStart0005L()
	{
 
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 reqData;
	CleanupClosePushL(reqData);

	CFilteringActiveScheduler scheduler;
	CActiveScheduler::Install(&scheduler);

	RMobileONStore store;
	TInt err = store.Open(iPhone);
	ASSERT_EQUALS(KErrNone, err);
	CleanupClosePushL(store);
	
	CRetrieveMobilePhoneONList* asyncRetrieveList = CRetrieveMobilePhoneONList::NewL(store);    
	CleanupStack::PushL(asyncRetrieveList);

	CActiveRetriever::ResetRequestsNumber();
	CActiveRetriever* activeRetriever = CActiveRetriever::NewL(*asyncRetrieveList);
	CleanupStack::PushL(activeRetriever);
	
	scheduler.AddRetrieverL(*activeRetriever);

	TInt index(-1);
    TMockLtsyData1<TInt>  expLtsyData(index);
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadSizeIPC, reqData);

	TInt size(1);
    TMockLtsyData1<TInt>  complete0LtsyData(size);
    reqData.Close();
    complete0LtsyData.SerialiseL(reqData);
    iMockLTSY.CompleteL(EMmTsyONStoreReadSizeIPC, KErrNone, reqData);

    index = 1;
    reqData.Close();
    expLtsyData.SerialiseL(reqData);
    iMockLTSY.ExpectL(EMmTsyONStoreReadEntryIPC, reqData);

	ERR_PRINTF2(_L("<font color=Orange>$CTSYKnownFailure: defect id = %d</font>"), 160801);	
   
	asyncRetrieveList->Start(activeRetriever->iStatus);
	activeRetriever->Activate();
	scheduler.StartScheduler();


	ASSERT_EQUALS(0, CActiveRetriever::ResetRequestsNumber());
	ASSERT_EQUALS(KErrTimedOut, activeRetriever->iStatus.Int());


	CActiveScheduler::Install(NULL);

	CleanupStack::PopAndDestroy(6, this);
	

	}

