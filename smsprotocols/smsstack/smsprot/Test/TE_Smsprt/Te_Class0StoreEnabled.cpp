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

CClass0StoreEnabled::CClass0StoreEnabled(RSocketServ &aSocketServer)
/** 
	Each test step initialises it's own name
*/
	{
	iSocketServer = &aSocketServer;
	}


/**
  Checks wheather the CLASS0 store has in fact been enabled.
		*/
TVerdict CClass0StoreEnabled::doTestStepL()
	{	
	TBool isClass0StoreEnabled = EFalse;
	GetBoolFromConfig(ConfigSection(), _L("isClass0StoreEnabled"), isClass0StoreEnabled);
		
	//Open a socket 
	INFO_PRINTF1(_L("Opening socket..."));
	RSocket socket;
	OpenSmsSocketL(*iSocketServer, socket, ESmsAddrRecvAny);

	TRequestStatus status;
	socket.Ioctl(KIoctlSupportOODClass0SmsMessages, status, NULL, KSolSmsProv);
	User::WaitForRequest(status);

	TEST((isClass0StoreEnabled  &&  status == KErrNone)  ||
		 (!isClass0StoreEnabled  &&  status == KErrNotSupported));
	
	socket.Close();
	
	return TestStepResult();
	}
//-----------------------------------------------------------------------------







