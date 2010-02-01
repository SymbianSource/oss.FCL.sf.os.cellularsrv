// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TE_SMSPRT_R6_H_
#define __TE_SMSPRT_R6_H_


#include "TE_smsprt.h"
#include "Gsmuelem.h"
#include "gsmuieoperations.h"


class CTestSinglePDUHyperLinks : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMultiplePDUHyperLinks : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSinglePDUReplyAddress : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMultiplePDUReplyAddress : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSpecialMessageOperations : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSinglePDUSpecialMessageWaiting : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMultiplePDUSpecialMessageWaiting : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestEnhancedVoiceMail : public CSmsPrtTestStep
{
protected:
	CEnhancedVoiceMailNotification* CreateEnhancedVoiceMailNotificationL(TGsmSmsTelNumber aAccessAddress);
	CVoiceMailNotification*  CreateVoiceMailNotificationL(TGsmSmsTelNumber aCallingLineIdentity);
};


class CTestEnhancedVoiceMailOperations : public CTestEnhancedVoiceMail
	{
public:
	enum TBoundaries {EInside=0,EOn,EOutside};
	enum TAddressType {EInternational=0,EParsed};

	virtual TVerdict doTestStepL();

protected:
	void AddAddressToNotifcationL( 	CEnhancedVoiceMailNotification* aEnhancedVoiceMailNotification,
		CVoiceMailNotification* aVoiceMailNotification,
		TPtrC aCallingLineIdentity,TUint aAddressType);
	};


class CTestSinglePDUEnhancedVoiceMailInformation : public CTestEnhancedVoiceMail
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMultiplePDUEnhancedVoiceMailInformation : public CTestEnhancedVoiceMail
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestEnhancedVoiceMailDeleteOperations : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestSinglePDUEnhancedVoiceMailDeleteConfirmation : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestMultiplePDUEnhancedVoiceMailDeleteConfirmation : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTest7BitDefaultAlphabet : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestZeroLengthIE : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestAutomaticDeletionInDCS : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestAutomaticDeletionInDCSAndInESK : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestAutomaticDeletionTypeZeroSMS : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestAutomaticDeletionTypeZeroAndSetInESK : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CTestOOMSendR6Sms : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CEnhancedVoiceMessageBoundaryTest : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CEnhancedVoiceMessageBoundaryTest1 : public CSmsPrtTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};


class CSubmitReportDefaultsTo7BitDecoding : public CSmsPrtTestStep
    {
public:
    virtual TVerdict doTestStepL();
    };


class CDeliverReportDefaultsTo7BitDecoding : public CSmsPrtTestStep
    {
public:
    virtual TVerdict doTestStepL();
    };


class CStatusReportDefaultsTo7BitDecoding : public CSmsPrtTestStep
    {
public:
    virtual TVerdict doTestStepL();
    };

#endif
