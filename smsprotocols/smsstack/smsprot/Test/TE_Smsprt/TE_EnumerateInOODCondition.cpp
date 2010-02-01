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
//

/**
 @file
*/

#include "Te_SmsTestSteps.h"

CEnumerateInOODCondition::CEnumerateInOODCondition(RSocketServ &aSocketServer)
/** 
	Each test step initialises it's own name
*/
	{
	iSocketServer = &aSocketServer;
	}

/**
  Enumerates SIM messages.
		*/
TVerdict CEnumerateInOODCondition::doTestStepL()
	{
#ifndef _DEBUG
    INFO_PRINTF1(_L("This test can only be run when the SMS Stack is in debug mode."));
#else   
	
	//PrepareRegTestLC(*iSocketServer, 17);
	
	//Open a socket 
	INFO_PRINTF1(_L("Opening socket..."));
	RSocket socket;
	OpenSmsSocketL(*iSocketServer, socket, ESmsAddrRecvAny);

	TRequestStatus status;
	TPckgBuf<TUint> sbuf;
	sbuf()=0;

	//Now enumerate messages from store
	socket.Ioctl(KIoctlEnumerateSmsMessages,status,&sbuf, KSolSmsProv);
	User::WaitForRequest(status);
	
	//The Enumeration should fail due to the disk being full and returns KErrDiskFull
	if(status==KErrDiskFull)
		{
		INFO_PRINTF1(_L("Status = KErrDiskFull"));
		}
	else
		{
		TEST(EFalse);
		}
	
	socket.Close();
#endif		
	return TestStepResult();
	}
//----------------------------------------------------------------------------->


