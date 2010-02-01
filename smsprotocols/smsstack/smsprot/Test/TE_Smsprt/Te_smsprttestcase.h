/**
* Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* Header for the mainframe of the smsprot test programs
* 
*
*/



/**
 @file
*/

#ifndef __TE_SMSPRTTESTCASE_H__
#define __TE_SMSPRTTESTCASE_H__

#include <e32base.h>
#include <es_sock.h>
#include <f32file.h>
#include "e32test.h"
#include "c32comm.h"
#include <simtsy.h>
#include "Gsmumsg.h"
#include "gsmubuf.h"
#include "gsmuset.h"
#include "smsustrm.h"
#include "smsuaddr.h"


class TTestCase //: public CSmsPrtTestStep
	{
public:
	TTestCase();
	TTestCase(const TDesC& aMsg, const TDesC& aMatch);
	TTestCase(const TDesC& aMsg, CSmsInformationElement::TSmsInformationElementIdentifier aId);
	TTestCase(const TDesC& aMsg, TSmsFirstOctet::TSmsStatusReportRequest aSRR);
	TTestCase(const TDesC& aMsg, TSmsDataCodingScheme::TSmsIndicationType aIndicationType,
	TSmsDataCodingScheme::TSmsDCSBits7To4 aBits7To4, TSmsDataCodingScheme::TSmsIndicationState iIndicationState);
	TTestCase(const TDesC& aMsg);
	const TTestCase& operator=(const TTestCase& aTest);
	void SetSmsClass(TSmsDataCodingScheme::TSmsClass aClass);
	void SetValidityPeriod(const TTimeIntervalMinutes& aTimeInterval);
	void SetRecvForever();
private:
	void CommonInit();
public:
	TSmsAddrFamily iMatchType;
	TPtrC iMsg;
	TPtrC iMatch;
	CSmsInformationElement::TSmsInformationElementIdentifier iIdentifierMatch;

	TBool iTestSmsClass;
	TSmsDataCodingScheme::TSmsClass iSmsClass;
	TBool iTestValidityPeriod;
	TTimeIntervalMinutes iValidityPeriod;
	TBool iTestForever;
	TBool iTestIndicators;
	TSmsDataCodingScheme::TSmsIndicationType iIndicationType;	// for ESmsAddrMessageIndication
	TSmsDataCodingScheme::TSmsDCSBits7To4 iBits7To4;			// for ESmsAddrMessageIndication
	TSmsDataCodingScheme::TSmsIndicationState iIndicationState; // for ESmsAddrMessageIndication
	};

#endif
