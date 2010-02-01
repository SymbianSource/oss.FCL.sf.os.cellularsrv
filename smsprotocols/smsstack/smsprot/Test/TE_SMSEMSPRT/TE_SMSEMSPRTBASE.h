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

#ifndef _TE_SMSEMSPRTBASE_H_
#define _TE_SMSEMSPRTBASE_H_


#include <test/testexecuteserverbase.h>
#include <simtsy.h>
#include <testconfigfileparser.h>


#include "gsmubuf.h"
#include "Gsmumsg.h"
#include "gsmuset.h"

#include "smsuaddr.H"
#include "smsustrm.h"
#include "smspdudb.h"
#include "smsstacktestutilities.h"
#include <mmlist.h>
#include <mmretrieve.h>

#include "attributes.h"
#include "smspproc.h"
#include "EMSFormatIE.h"
#include "EMSSoundIE.h"
#include "EMSPictureIE.h"
#include "EMSUserPromptIE.h"
#include "EMSAnimationIE.h"
#include "EmsTestUtils.h"
#include "EMSObjectDistributionIE.h"


const TUint KSocketMessageSlots = 16;   // Override the default value of 8

// The basic text we send - 26 chars long
_LIT(KBasicSmsText,"abcdefghijklmnopqrstuvwxyz");
_LIT(KTSmsEmsPrtConfigFileName, "tsmsemsprt_config.txt");

_LIT(KGmsSmsConfigFileName,"setupgsmsms.txt");

class CSmsEmsPrtTestStep : public CTestStep
{

public:
	CSmsMessage* ConfigCreateSmsMessageLC(const TDesC& aDes,
			TSmsDataCodingScheme::TSmsAlphabet aAlphabet = TSmsDataCodingScheme::ESmsAlphabet8Bit);
	CSmsMessage* CreateSmsMessageL(const TDesC& aDes, TSmsDataCodingScheme::TSmsAlphabet aAlphabet, CSmsPDU::TSmsPDUType aType  = CSmsPDU::ESmsSubmit) ;
	TBool SendReceiveMsgL(CSmsMessage& aMsg);
	void SendSmsL(const CSmsMessage* aSms, RSocket& aSocket);
	void PrintMessageL(const CSmsMessage* aSms);
	void WaitForRecvL(RSocket& aSocket);
	CSmsMessage* RecvSmsL(RSocket& aSocket, TInt aIoctl = KIoctlReadMessageSucceeded) ;
	TText8 IsCharDisplayable( const TText8 aChar ) ;
	void PrepareRegTestL() ;
	void EndRegTest();
	TInt CommInit() ;
    void CreateCommDBL();
	void ParseSettingsFromFileL();


	RFs iFs;
	TSmsServiceCenterAddress iTelephoneNumber; //Test SIM
	TSmsServiceCenterAddress iServiceCenterNumber; //Radiolinja
	RSocketServ iSocketServ;
	RSocket iSocket;

 	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TInt GetTestNumber() = 0 ;

	CSmsStackTestUtils* iSmsStackTestUtils;

};

#endif // _TE_SMSEMSPRTBASE_H_
