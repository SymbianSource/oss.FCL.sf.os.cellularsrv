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

/**
 @file
*/

#include "TE_smsprtstressbase.h"
#include <sacls.h>
#include <cfshared.h>
#include <c32root.h>

#include <connect/sbdefs.h>
using namespace conn;

TVerdict CSmsPrtStressTestStep::doTestStepPreambleL( void )
	{
	__UHEAP_MARK;

	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

	iSecureBackupEngine = CSBEClient::NewL();
	iSecureBackupEngine->SetBURModeL(TDriveList(_L8("C")),
									 EBURNormal, ENoBackup);

	User::LeaveIfError(iFs.Connect());

	iSmsStackTestUtils = CSmsStackTestUtils::NewL(this, iFs);

    return TestStepResult() ;
	}

TVerdict CSmsPrtStressTestStep::doTestStepPostambleL( void )
	{
	delete iSmsStackTestUtils;
	iSmsStackTestUtils = NULL;

	iFs.Close();

	delete iSecureBackupEngine;
	iSecureBackupEngine = NULL;

	delete iScheduler;
	iScheduler = NULL;

	__UHEAP_MARKEND;
	
	DoESockMemoryLeakTestL();

	return TestStepResult() ;
	}


void CSmsPrtStressTestStep::InitGlobalsL()
/**
 *  Initialise global variables.
 */
    {
    TRAPD( ret, ParseSettingsFromFileL());

	if (ret != KErrNone)
		INFO_PRINTF2(_L("ParseSettingsFromFileL [err=%d]"), ret);

    GetCmdLineArguments();

    INFO_PRINTF2(_L("TelNo: [%S]"), &iTelephoneNumber);
	INFO_PRINTF2(_L("ScNo:  [%S]"), &iServiceCenterNumber);

	CommInit();

	iSmsStackTestUtils->CopyConfigFileL(KTSmsPrtStressConfigFileName);
	}


void CSmsPrtStressTestStep::PrepareRegTestLC(RSocketServ& aSocketServer, TInt aTestNumber)
/**
 *  Run a specified test.
 *  The test number is passed via property KUidPSSimTsyCategory. This will notify the SIM tsy
 *  SIM tsy uses test number to parse correct script from config.txt
 *  @param aTestNumber The test number corresponding the test case
 */
	{
	RProperty testNumberProperty;
	User::LeaveIfError(testNumberProperty.Attach(KUidPSSimTsyCategory, KPSSimTsyTestNumber));
	CleanupClosePushL(testNumberProperty);

	TRequestStatus status;
	testNumberProperty.Subscribe(status);
	User::LeaveIfError(testNumberProperty.Set(KUidPSSimTsyCategory,KPSSimTsyTestNumber,aTestNumber));
	User::WaitForRequest(status);
	TEST(status.Int() == KErrNone);
	TInt testNumberCheck;
	User::LeaveIfError(testNumberProperty.Get(testNumberCheck));
	if (aTestNumber != testNumberCheck)
		User::Leave(KErrNotFound);

	CleanupStack::PopAndDestroy(&testNumberProperty);

    INFO_PRINTF1(_L("Connecting to SocketServer ..."));
	TInt ret=aSocketServer.Connect(KSocketMessageSlots);
    TEST(ret == KErrNone);
	CleanupClosePushL(aSocketServer);

	INFO_PRINTF1(_L("Deleting segmentation and reassembly stores..."));

	// delete segmentation and reassembly store files before the test
	_LIT(KReassemblyStoreName,"C:\\Private\\101F7989\\sms\\smsreast.dat");
	_LIT(KSegmentationStoreName,"C:\\Private\\101F7989\\sms\\smssegst.dat");

	iFs.Delete(KReassemblyStoreName);
	iFs.Delete(KSegmentationStoreName);
	}


TBool CSmsPrtStressTestStep::DoGetCmdLineArguments(TDes& aCmdarg, const TDesC& aToFind, TDes& aCmd)
    {
    if(( aCmdarg.FindF(aToFind)) != KErrNotFound )
        {
        TLex myLex( aCmdarg );
        myLex.Inc(aCmdarg.FindF(_L("="))+1);
        myLex.SkipSpace();
        myLex.Mark();                         // remember where we are
        myLex.SkipCharacters();               // move to end of character token
        if (myLex.TokenLength() != 0)         // if valid potential token
            {
            aCmd.Copy(myLex.MarkedToken());    // extract token
            return ETrue;
            }
        }
    return EFalse;
    }

void CSmsPrtStressTestStep::GetCmdLineArguments()
    {
    CCommandLineArguments *cmdLine = NULL;
    TRAPD(err, cmdLine = CCommandLineArguments::NewL());
    if (err==KErrNone)
        {
        TInt numCmds = cmdLine->Count();
        TInt i=0;
        while (++i < numCmds)
            {
            TBuf<256> arg = cmdLine->Arg(i);

            // Reading the Service Centre Number
            _LIT( KSCno, "sc=");
            _LIT( KSCNO, "SC=");
            if(    DoGetCmdLineArguments( arg, KSCno, iServiceCenterNumber )
                || DoGetCmdLineArguments( arg, KSCNO, iServiceCenterNumber ))
                continue;

            // Reading the Recipient Telephone Number
            _LIT( KTELno, "tel=");
            _LIT( KTELNO, "TEL=");
            if(    DoGetCmdLineArguments( arg, KTELno, iTelephoneNumber )
                || DoGetCmdLineArguments( arg, KTELNO, iTelephoneNumber ))
                continue;
            }
        }
    delete cmdLine;
    cmdLine = NULL;
    }

void CSmsPrtStressTestStep::ParseSettingsFromFileL()
	{
	CTestConfig* configFile = CTestConfig::NewLC(iFs,KGmsSmsConfigFileDir,KGmsSmsConfigFileName);
	const CTestConfigSection* cfgFile = configFile->Section(KSetupTelNumbers);
	if (cfgFile == NULL)
		User::Leave(KErrNotFound);

	const CTestConfigItem* item = cfgFile->Item(KServiceCenter,0);
	if (item == NULL)
		User::Leave(KErrNotFound);

	iServiceCenterNumber.Copy(item->Value());

	item = cfgFile->Item(KTelefoneNumber,0);
	if (item == NULL)
		User::Leave(KErrNotFound);

	iTelephoneNumber.Copy(item->Value());

	// beginning of the destruction
	CleanupStack::PopAndDestroy(configFile);//configFile
	}

void CSmsPrtStressTestStep::EndRegTest()
/**
 *  Closes objects used at test harness
 */
 	{
	INFO_PRINTF1(_L("Closing Reg Test"));
	User::After(2000000); //Wait a couple of seconds to avoid closing the SMS stack too early
	CleanupStack::PopAndDestroy(); //socketServer
	User::After(2000000); //Wait a couple of seconds to avoid closing the SMS stack too early
	}


#if defined (__WINS__)
#define PDD_NAME _L("ECDRV")
#define PDD_NAME2 _L("ECDRV")
#define LDD_NAME _L("ECOMM")
#else
#define PDD_NAME _L("EUART1")
#define LDD_NAME _L("ECOMM")
#endif


TInt CSmsPrtStressTestStep::CommInit()
    {
    RFs fs;
    TInt err=fs.Connect();  // make sure the FileServer is alive (only needed for WINS test code)
    fs.Close();

    INFO_PRINTF1(_L("CommInit: Loading PDD ") );
    INFO_PRINTF1(PDD_NAME);
    INFO_PRINTF1(_L(""));
    err=User::LoadPhysicalDevice(PDD_NAME);
    if (err!=KErrNone && err!=KErrAlreadyExists)
        return(err);

    INFO_PRINTF1(_L("CommInit: Loading LDD ") );
    INFO_PRINTF1(LDD_NAME);
    INFO_PRINTF1(_L(""));
    err=User::LoadLogicalDevice(LDD_NAME );
    if (err!=KErrNone && err!=KErrAlreadyExists)
        return(err);

    INFO_PRINTF1(_L("CommInit: Starting C32 ") );

    err = StartC32();
    return (err == KErrNone || err == KErrAlreadyExists) ? KErrNone : err;
    }


void CSmsPrtStressTestStep::DoESockMemoryLeakTestL()
	{
	//
	// Find the current number of leaked cells in ESock...
	//
	TInt  ret, startLeakCounter;
	
	ret = RProperty::Get(KUidCommsProcess, KUidCommsModuleLeakCounter, startLeakCounter);
	if (ret == KErrNotFound)
		{
		// No variable to monitor, ESock is probably not in debug mode.
		return;
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
	} // CSmsPrtStressTestStep::DoESockMemoryLeakTestL


