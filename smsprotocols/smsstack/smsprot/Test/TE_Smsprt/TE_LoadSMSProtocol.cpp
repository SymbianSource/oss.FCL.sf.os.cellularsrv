// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/

#include <smspver.h>
#include "Te_SmsTestSteps.h"

CLoadSMSProtocol::CLoadSMSProtocol(RSocketServ &aSocketServer)
/** Each test step initialises it's own name
*/
	{
	iSocketServer = &aSocketServer;
	}

/** 
	This step is required before using the SMS stack.
	It connects with the socket server, opens a socket subsession AND powers-up the phones using
	interface to  Etel.
*/
TVerdict CLoadSMSProtocol::doTestStepL()
	{	
	TInt testNumber;
	GetIntFromConfig(ConfigSection(), _L("testNumber"), testNumber);
	INFO_PRINTF2(_L("Setting test number, %d"),testNumber );
	SetSimTSYTestNumberL(testNumber);
		
	WaitForInitializeL();
	
	INFO_PRINTF1(_L("Connecting to SocketServer"));
	TInt ret=iSocketServer->Connect(KSocketMessageSlots);
	TEST(ret == KErrNone);

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
	
	
	
