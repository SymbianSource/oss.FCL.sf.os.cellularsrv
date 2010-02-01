
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

#include <cfshared.h>
#include <c32root.h>

#include "Te_SmsTestSteps.h"

/**
	Test for any leaks in ESOCK
	The socket server must be closed down before this step is run, or no clients must be connected to esock
	when running this test step.
*/
TVerdict CESockMemoryLeakTest::doTestStepL()
	{
	//
	// Find the current number of leaked cells in ESock...
	//
	TInt  ret, startLeakCounter;
	
	ret = RProperty::Get(KUidCommsProcess, KUidCommsModuleLeakCounter, startLeakCounter);
	if (ret == KErrNotFound)
		{
		// No variable to monitor, ESock is probably not in debug mode.
		INFO_PRINTF1(_L("<font size=2 color=00FFCC><B>ESock is probably not in debug mode.</B></font>"));		
		return EPass;
		}
	else
		{
		TESTL(ret == KErrNone);
		}

	//
	// Connect to Rootserver...
	//
	RRootServ  rootserver;

	User::LeaveIfError(rootserver.Connect());
	CleanupClosePushL(rootserver);

	//
	// Shutdown all the ESock CPMs gracefully...
	//
	TRSIter  iter;
	TCFModuleName  modName;
	TRSModuleInfo  modInfo;
	TRequestStatus  statusArray[16];
	TInt  statusCount = 0;

	while (rootserver.EnumerateModules(iter, modName) == KErrNone)
		{
		if (rootserver.GetModuleInfo(modName, modInfo) == KErrNone  &&
		    modInfo.iParams.iDll.MatchF(_L("*ESOCKSVR.DLL")) >= 0)
			{
			rootserver.UnloadCpm(statusArray[statusCount++], modInfo.iParams.iName,
			                     EGraceful);
			}
		}

	while (statusCount > 0)
		{
		statusCount--;
		User::WaitForRequest(statusArray[statusCount]);
		TEST(statusArray[statusCount] == KErrNone);
		}

	//
	// Close Rootserver session...
	//
	CleanupStack::PopAndDestroy(1, &rootserver);
	
	//
	// Get the latest number of leaked cells in ESock...
	//
	TInt  finalLeakCounter;

	ret = RProperty::Get(KUidCommsProcess, KUidCommsModuleLeakCounter, finalLeakCounter);
	TESTL(ret == KErrNone);

	if (finalLeakCounter > startLeakCounter)
		{
		INFO_PRINTF1(_L("<font size=2 color=FF0000><B>A memory leak has been detected inside ESock - Check SMS Stack!</B></font>"));
		TEST(finalLeakCounter == startLeakCounter);
		}

	//
	// Restart C32...
	//
	_LIT(KDummyCMI, "");
	WarmBootC32(KDummyCMI);
	
	return TestStepResult();
	} // CSmsPrtTestStep::DoESockMemoryLeakTestL
