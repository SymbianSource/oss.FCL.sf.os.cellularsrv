/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#ifndef __USSD__TEST__CLIENT__
#define __USSD__TEST__CLIENT__

//  INCLUDES
#include <etelmm.h>
#include <e32std.h>
#include <etel.h>

enum
    {
    EOptionReceive = 1,
    EOptionSend = 2,
    EOptionAccept = 4,
    EOptionReceiveTimeout = 8,
    EOptionRandom = 16,
    EOptionDefault = 32,
    EOptionMO = 64 // this is a mobile originated(MO) request
    };

class CCTsyUssdMessagingTestClient : public CBase
	{
public:
CCTsyUssdMessagingTestClient();
TInt ReceiveMessageL(TBool aAcceptDialogue, TBool aReceiveTimeout, TReal aAfterTimeperiod, TBool aMO);
TInt SendMessageL();
TInt SendMessageDefaultHandlerL();
TInt RandomLoopL();

private:
TBool ProbabilityPercent(TInt aPercentTrue);
TInt RandomNumber(TInt aMax);
	}; // class CCTsyUssdMessagingTestClient

#endif // __USSD__TEST__CLIENT__

//  End of file
