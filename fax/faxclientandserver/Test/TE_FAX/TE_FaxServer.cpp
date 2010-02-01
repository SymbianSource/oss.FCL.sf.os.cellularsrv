// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Telephony Fax Test server test code.
// 
//

/**
 @file
 @internalComponent
*/

#include "faxdefn.h"
#include "FAXSET.H"
#include "CFAX32.H"

#include "TE_FaxServer.h"
#include "TE_FaxTest.h"


_LIT(KServerName,"TE_Fax");


/**
 *  Called inside the MainL() function to create and start the CTestServer
 *  derived server.
 *
 *  @return Instance of the test server
 */
CFaxTestServer* CFaxTestServer::NewL()
	{
	CFaxTestServer*  server = new(ELeave) CFaxTestServer();
	CleanupStack::PushL(server);
	// CServer base class call
	server->StartL(KServerName);
	CleanupStack::Pop(server);
	return server;
	} // CFaxTestIntServer::NewL


/**
 *  Much simpler, uses the new Rendezvous() call to sync with the client.
 */
LOCAL_C void MainL()
	{
	//
	// Start an active scheduler...
	//
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	//
	// Create the CTestServer derived server...
	//
	CFaxTestServer* server = NULL;

	TRAPD(err, server = CFaxTestServer::NewL());
	if (err == KErrNone)
		{
		//
		// Sync with the client and enter the active scheduler
		//
		RProcess::Rendezvous(KErrNone);
		scheduler->Start();
		}

	//
	// Clean up...
	//
	CleanupStack::Pop(scheduler);
	delete server;
	delete scheduler;
	} // MainL
/**
 *  @return Standard Epoc error code on exit.
 */
GLDEF_C TInt E32Main()
	{
	CTrapCleanup*  cleanup = CTrapCleanup::New();

	if (cleanup == NULL)
		{
		return KErrNoMemory;
		}

	TRAPD(err, MainL());

	delete cleanup;

	return err;
	} // E32Main


/**
 *  Implementation of CTestServer pure virtual.
 *
 *  @return A CTestStep derived instance.
 */
CTestStep* CFaxTestServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep*  testStep = NULL;

    //
	// Create the required test step...
	//
	if (aStepName == _L("TestFaxDefinition"))
		{
		testStep = new CFaxTestFaxDefinition();
		}

	if (aStepName == _L("TestFaxStorage"))
		{
		testStep = new CFaxTestFaxStorage();
		}

	if (aStepName == _L("TestFaxCoding"))
		{
		testStep = new CFaxTestFaxCoding();
		}

	if (aStepName == _L("TestFaxHeaderDefinition"))
		{
		testStep = new CFaxTestFaxHeaderDefinition();
		}

	if (aStepName == _L("TestTransmitFax01"))
		{
		testStep = new CFaxTestTransmitFax01();
		}

	if (aStepName == _L("TestTransmitFax02"))
		{
		testStep = new CFaxTestTransmitFax02();
		}

	if (aStepName == _L("TestTransmitFax03"))
		{
		testStep = new CFaxTestTransmitFax03();
		}

	if (aStepName == _L("TestTransmitFax04"))
		{
		testStep = new CFaxTestTransmitFax04();
		}

	if (aStepName == _L("TestTransmitFax05"))
		{
		testStep = new CFaxTestTransmitFax05();
		}

	if (aStepName == _L("TestTransmitFax06"))
		{
		testStep = new CFaxTestTransmitFax06();
		}

	if (aStepName == _L("TestReceiveFax01"))
		{
		testStep = new CFaxTestReceiveFax01();
		}

	if (aStepName == _L("TestReceiveFax02"))
		{
		testStep = new CFaxTestReceiveFax02();
		}

	if (aStepName == _L("TestReceiveFax03"))
		{
		testStep = new CFaxTestReceiveFax03();
		}

	if (aStepName == _L("TestReceiveFax04"))
		{
		testStep = new CFaxTestReceiveFax04();
		}

	if (aStepName == _L("TestReceiveFax05"))
		{
		testStep = new CFaxTestReceiveFax05();
		}

	if (aStepName == _L("TestReceiveFax06"))
		{
		testStep = new CFaxTestReceiveFax06();
		}

	return testStep;
	} // CFaxTestServer::CreateTestStep
