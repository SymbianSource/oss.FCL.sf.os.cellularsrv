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

#include "Te_SmsTestSteps.h"

CSendSMS::CSendSMS(RSocketServ &aSocketServer)
/** 
	Each test step initialises it's own name
*/
	{
	iSocketServer = &aSocketServer;
	}

/**
  Send SMS Message
*/
TVerdict CSendSMS::doTestStepL()
	{
#ifndef _DEBUG
    INFO_PRINTF1(_L("This test can only be run when the SMS Stack is in debug mode."));
#else   	
	//Open the socket for sending messages
	RSocket socket;
	INFO_PRINTF1(_L("Opening socket and loading SMS Protocol"));
	OpenSmsSocketL(*iSocketServer, socket, ESmsAddrSendOnly);
	
	//Create SMS
	CSmsMessage* iSms=CreateSMSL();
	CleanupStack::PushL(iSms);

	if( iSms )
		{
		INFO_PRINTF1(_L("Sending SMS...") );
		PrintMessageL(iSms);
		SendSmsL(iSms,socket);				
		}
	else
		{
		INFO_PRINTF1(_L("Error creating SMS.") );
		TEST(EFalse);
		}
	
	CleanupStack::PopAndDestroy(iSms);
	socket.Close();
#endif	
	return TestStepResult();
	}
//-----------------------------------------------------------------------------







