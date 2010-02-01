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
// TS_SMSCAPSSERVER.CPP
// 
//

/**
 @file
*/

#include <commsdattypesv1_1.h>

#include <rsshared.h>		// StartC32
#include "te_smscapsstep.h"
#include "te_smscapsserver.h"
#include <smsuaddr.h>
#include <e32property.h>
#include <simtsy.h>

using namespace CommsDat;


//

_LIT(KSmsCapsServerName,"TE_SMSCAPS_SC");

CSmsCapsTestServer::~CSmsCapsTestServer()
	{
	iSocketServer.Close();
    iFs.Close();
    }

CSmsCapsTestServer* CSmsCapsTestServer::NewL()
/**
 *  @return - Instance of the test server
 *  Called inside the MainL() function to create and start the
 *  CTestServer derived server.
 */
	{
	CSmsCapsTestServer * server = new (ELeave) CSmsCapsTestServer();
	CleanupStack::PushL(server);
	server->ConstructL(KNullDesC);
	// CServer base class call
	server->StartL(KSmsCapsServerName);
	CleanupStack::Pop(server);
	return server;
	}


LOCAL_C void MainL()
/**
 *  Uses the new Rendezvous() call to sync with the client
 */
	{
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CSmsCapsTestServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CSmsCapsTestServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}

#if defined (__WINS__)
#define PDD_NAME _L("ECDRV")
#define PDD_NAME2 _L("ECDRV")
#define LDD_NAME _L("ECOMM")
#else
#define PDD_NAME _L("EUART1")
#define LDD_NAME _L("ECOMM")
#endif


GLDEF_C TInt E32Main()
/**
 *  @return - Standard Epoc error code on exit
 */
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	delete cleanup;

	return err;
    }

void CSmsCapsTestServer::ConstructL(const TDesC& /*aName*/)
	{
	TSmsServiceCenterAddress serviceCenterNumber=_L("+358508771010"); //Radiolinja

	// File Server
	User::LeaveIfError(iFs.Connect());

	//
	// Create CommDB
	//

#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_2);
#else
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
#endif
	
    CleanupStack::PushL(db);
    db->OpenTransactionL();
    CMDBRecordSet<CCDModemBearerRecord>* modemBearerRecordSet = new(ELeave) CMDBRecordSet<CCDModemBearerRecord>;
    CleanupStack::PushL(modemBearerRecordSet);
    modemBearerRecordSet->LoadL(*db);

	_LIT(KDummy0, "DUMMY::0");
	_LIT(KDummy, "DUMMY");
	_LIT(KSim, "SIM");

    RPointerArray<CCDModemBearerRecord>& resultSet = (RPointerArray<CCDModemBearerRecord>&) modemBearerRecordSet->iRecords;
    CCDModemBearerRecord* modemRecord;
    for (TInt i=0; i<resultSet.Count(); i++)
    	{
    		modemRecord = resultSet[i];
    	    modemRecord->iPortName = KDummy0;
       	    modemRecord->iCsyName = KDummy;
       	    modemRecord->iTsyName = KSim;
    		modemRecord->iMessageCentreNumber = serviceCenterNumber;
    		modemRecord->ModifyL(*db);
    		modemRecord = NULL;
    	}
    db->CommitTransactionL();
    CleanupStack::PopAndDestroy(); //modemBearerRecordSet
    CleanupStack::PopAndDestroy(); //db

	//
	// initialise ser-comms - is this really needed ????
	//

    TInt err=User::LoadPhysicalDevice(PDD_NAME);
    if (err!=KErrNone && err!=KErrAlreadyExists)
      User::Leave(err);
    err=User::LoadLogicalDevice(LDD_NAME );
    if (err!=KErrNone && err!=KErrAlreadyExists)
        User::Leave(err);

    err = StartC32();
    if (err!=KErrNone)
    	User::Leave(err);

	//
	// turn phone on
	//
//    User::LeaveIfError(RProperty::Set(KUidSystemCategory,KUidPhonePwr.iUid,ESAPhoneOn));
//    User::After(1000000);

    //
    // define test number property
    //
//	TInt val = RProperty::Define(KUidPSSimTsyCategory, KPSSimTsyTestNumber, RProperty::EInt);
//	if ((val != KErrNone) && (val != KErrAlreadyExists))
//		{
//		User::Leave(val);
//		}
	}

CTestStep* CSmsCapsTestServer::CreateTestStep(const TDesC& aStepName)
/**
 *  @return - A CTestStep derived instance
 *  Implementation of CTestServer pure virtual
 */
	{

	CTestStep*  testStep=NULL;

	//
	// Here the test step is created when it is needed. Note that this
	// function is non-leaving so we cannot use new(ELeave). Therefore
	// the new could return NULL, but that is not a problem as it implies
	// the test step is missing and this will be marked in the log file.
	//
	if(aStepName == _L("CSmsCapsSmsIoctlDeleteSms"))
		{
		testStep = new CSmsCapsSmsIoctlDeleteSms(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsSmsIoctlEnumerateSms"))
		{
		testStep = new CSmsCapsSmsIoctlEnumerateSms(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsSmsIoctlReadSucceededSms"))
		{
		testStep = new CSmsCapsSmsIoctlReadSucceededSms(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsSmsIoctlReadFailedSms"))
		{
		testStep = new CSmsCapsSmsIoctlReadFailedSms(iSocketServer,iFs);
		}
	else if(aStepName == _L("CSmsCapsSmsIoctlWriteSms"))
		{
		testStep = new CSmsCapsSmsIoctlWriteSms(iSocketServer,iFs);
		}
	else if(aStepName == _L("CSmsCapsSmsIoctlSendSms"))
		{
		testStep = new CSmsCapsSmsIoctlSendSms(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsSmsIoctlReadParamsSms"))
		{
		testStep = new CSmsCapsSmsIoctlReadParamsSms(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsSmsIoctlCompleteReadParamsSms"))
		{
		testStep = new CSmsCapsSmsIoctlCompleteReadParamsSms(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsSmsIoctlWriteParamsSms"))
		{
		testStep = new CSmsCapsSmsIoctlWriteParamsSms(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsSmsSetLocalName"))
		{
		testStep = new CSmsCapsSmsSetLocalName(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsSmsWrite"))
		{
		testStep = new CSmsCapsSmsWrite(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsWapSetLocalName"))
		{
		testStep = new CSmsCapsWapSetLocalName(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsWapWrite"))
		{
		testStep = new CSmsCapsWapWrite(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsWapIoctl"))
		{
		testStep = new CSmsCapsWapIoctl(iSocketServer,iFs) ;
		}
	else if(aStepName == _L("CSmsCapsWapSetOption"))
		{
		testStep = new CSmsCapsWapSetOption(iSocketServer,iFs) ;
		}

	//
	// Set the test step name here to save code!!!
	//
	if (testStep != NULL)
		{
		testStep->SetTestStepName(aStepName);
		}

	return testStep;
	}
