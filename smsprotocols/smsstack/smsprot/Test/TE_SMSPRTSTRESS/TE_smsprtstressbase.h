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

#ifndef _TE_SMSPRTBASE_H_
#define _TE_SMSPRTBASE_H_

#include <testconfigfileparser.h>
#include <test/testexecuteserverbase.h>

#include <e32test.h>
#include <es_sock.h>
#include <logwrap.h>
#include "smsuact.h"
#include "gsmubuf.h"
#include "Gsmumsg.h"
#include "gsmuset.h"
#include "smsuaddr.H"
#include "smsustrm.h"
#include <mmretrieve.h>
#include <smsustrm.h>
#include <mmlist.h>
#include <simtsy.h>
#include "smspdudb.h"
#include "attributes.h"
#include "smspproc.h"
#include "smsstacktestutilities.h"
#include <bacline.h>        // CCommandLineArguments
#include <connect/sbeclient.h>

using namespace conn;

_LIT(KTSmsPrtStressConfigFileName, "tsmsprtStress_config.txt");
_LIT(KSmsMsgStressTestReport, "tsmsprtStress_testreport.txt");
_LIT8(KSmsCompInfo, "SMSPROT.PRT");
_LIT(KGmsSmsConfigFileName,"setupgsmsms.txt");

// Here are the section names
_LIT8(KSetupTelNumbers,           "Defaults");

// Here are the item names
_LIT8(KServiceCenter,               "ServiceCenter");
_LIT8(KTelefoneNumber,              "TelephoneNumber");

const TInt KActiveReceiverPriority = CActive::EPriorityStandard;
const TUint KSocketMessageSlots = 16;

class CSmsPrtStressTestStep : public CTestStep
{
public:
	void PrepareRegTestLC(RSocketServ& aSocketServer, TInt aTestNumber) ;
	void EndRegTest() ;
	void InitGlobalsL();
	void ParseSettingsFromFileL();
	TBool DoGetCmdLineArguments(TDes& aCmdarg, const TDesC& aToFind, TDes& aCmd);
	void GetCmdLineArguments();
	TInt CommInit();
	TVerdict doTestStepPreambleL();
	TVerdict doTestStepPostambleL();
	void DoESockMemoryLeakTestL();
public:
	RFs iFs;
	CSmsStackTestUtils* iSmsStackTestUtils;
	TSmsServiceCenterAddress iTelephoneNumber;
	TSmsServiceCenterAddress iServiceCenterNumber;
	CActiveScheduler*  iScheduler;
	CSBEClient*  iSecureBackupEngine;
};

#endif
