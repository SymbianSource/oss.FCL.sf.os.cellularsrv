// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// functional unit of the SAT.
//



/**
 @file The TEFUnit header file which tests the SendSS
*/

#ifndef CCSATSENDSSFU_H
#define CCSATSENDSSFU_H

#include <test/tefunit.h>

#include <etelmm.h>
#include <etelmmcs.h>

#include "ccsatcomponenttestbase.h"

class CCSatSendSSFU : public CCSatComponentTestBase
	{
public:
	// Create a suite of all the tests
	static CTestSuite* CreateSuiteL(const TDesC& aName);

public:
	// Individual test steps

	void TestNotifySendSsPCmd0001L();
	void TestNotifySendSsPCmd0002L();
	void TestNotifySendSsPCmd0004L();

	static void PrepareTlv(TTlv& aTlv, const TDesC8& aSsString, const TDesC8& aAlpha = KNullDesC8);

private:	

	void PrepareDispatchL();
	void CompleteDispatchL(const TDesC8& aTlvDsc, TInt aRes = KErrNone);
	void DoTestTerminalResponseL();	
	}; // class CCSatSendSSFU

#endif // CCSATSENDSSFU_H

