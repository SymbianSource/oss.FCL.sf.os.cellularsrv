// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "Te_smsprttestcase.h"
#include "TE_smsprtbase.h"

TTestCase::TTestCase()
//
// Constructor
//
	{

	CommonInit();
	}

TTestCase::TTestCase(const TDesC& aMsg, const TDesC& aMatch)
//
// Constructor
//
: iMatchType(ESmsAddrMatchText), iMsg(aMsg), iMatch(aMatch)
	{

	CommonInit();
	}

void TTestCase::CommonInit()
//
// Reset member variables
//
	{

	iTestSmsClass = EFalse;
	iTestValidityPeriod = EFalse;
	iTestForever=EFalse;
	iTestIndicators=EFalse;
	}

TTestCase::TTestCase(const TDesC& aMsg, CSmsInformationElement::TSmsInformationElementIdentifier aIdentifier)
//
// Constructor
//
: iMatchType(ESmsAddrMatchIEI), iMsg(aMsg), iIdentifierMatch(aIdentifier)
	{

	CommonInit();
	}

TTestCase::TTestCase(const TDesC& aMsg, TSmsFirstOctet::TSmsStatusReportRequest /*aSRR*/)
//
// Match on status report - ignore actual value of aSRR
//
: iMatchType(ESmsAddrStatusReport), iMsg(aMsg)
	{

	CommonInit();
	}

TTestCase::TTestCase(const TDesC& aMsg, TSmsDataCodingScheme::TSmsIndicationType aIndicationType,
					 TSmsDataCodingScheme::TSmsDCSBits7To4 aBits7To4,
					 TSmsDataCodingScheme::TSmsIndicationState aIndicationState)
//
// Match on a type of message indication
//
: iMatchType(ESmsAddrMessageIndication), iMsg(aMsg), iIndicationType(aIndicationType),
	iBits7To4(aBits7To4), iIndicationState(aIndicationState)
	{

	__ASSERT_ALWAYS(iBits7To4==TSmsDataCodingScheme::ESmsDCSMessageWaitingIndicationDiscardMessage
		|| iBits7To4==TSmsDataCodingScheme::ESmsDCSMessageWaitingIndication7Bit
		|| iBits7To4==TSmsDataCodingScheme::ESmsDCSMessageWaitingIndicationUCS2,
		User::Panic(_L("TestCase"), KErrNotSupported));

	CommonInit();
	iTestIndicators=ETrue;

	}

TTestCase::TTestCase(const TDesC& aMsg)
//
// Constructor
//
: iMatchType(ESmsAddrRecvAny), iMsg(aMsg)
	{

	CommonInit();
	}

const TTestCase& TTestCase::operator=(const TTestCase& aTest)
//
// Assignment operator
//
	{

	iMatchType = aTest.iMatchType;
	iMsg.Set(aTest.iMsg);
	iMatch.Set(aTest.iMatch);
	iIdentifierMatch = aTest.iIdentifierMatch;
	iTestForever = aTest.iTestForever;
	return *this;
	}

void TTestCase::SetSmsClass(TSmsDataCodingScheme::TSmsClass aClass)
//
// Set Sms Class
//
	{

	iTestSmsClass = ETrue;
	iSmsClass = aClass;
	}

void TTestCase::SetValidityPeriod(const TTimeIntervalMinutes& aTimeInterval)
//
// Set the validity period
//
	{

	iTestValidityPeriod = ETrue;
	iValidityPeriod = aTimeInterval;
	}

void TTestCase::SetRecvForever()
//
// Recv all messages forever
//
	{

	iTestForever = ETrue;
	iMatchType = ESmsAddrRecvAny;
	}
