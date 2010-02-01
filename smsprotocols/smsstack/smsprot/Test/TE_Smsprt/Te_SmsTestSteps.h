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
 
#ifndef __TE_SMS_TEST_STEPS_H__
#define __TE_SMS_TEST_STEPS_H__
 
#include "TE_smsprtbase.h"

/**
  Test Step to Load SMS Protocol
*/
class CLoadSMSProtocol : public CSMSTestSteps
{
public:
	CLoadSMSProtocol(RSocketServ &aSocketServer);
	
	//From CTestStep
	virtual enum TVerdict doTestStepL();

};

/**
  Test Step to receive a SMS message
*/
class CReceiveSMS : public CSMSTestSteps
	{
public:
	CReceiveSMS(RSocketServ &aSocketServer);

	//From CTestStep
	virtual enum TVerdict doTestStepL();
	};

/**
  Test Step to send a SMS message
*/
class CSendSMS : public CSMSTestSteps
{
public:
	CSendSMS(RSocketServ &aSocketServer);

	//From CTestStep
	virtual enum TVerdict doTestStepL();
};

/**
  Close the socket and the socket server session
*/
class CCloseSMSProtocol : public CSMSTestSteps
{
public:
	CCloseSMSProtocol(RSocketServ &aSocketServer);

	//From CTestStep
	virtual enum TVerdict doTestStepL();
};


/**
  Delete segmentation and reasembly stores
*/
class CDeleteSARStores : public CSMSTestSteps
{
public:
	//From CTestStep
	virtual enum TVerdict doTestStepL();
};

/**
  Forwards system time by x number of minutes
*/
class CForwardSystemTime : public CSMSTestSteps
{
public:
	//From CTestStep
	virtual enum TVerdict doTestStepL();
};

/**
  Tests receive of SMS message while SMSStack Out of Memory (OOM)
*/
class CSendReceiveSMSWhileServerInOOM : public CSMSTestSteps
{
public:
	CSendReceiveSMSWhileServerInOOM(RSocketServ &aSocketServer);

	//From CTestStep
	virtual enum TVerdict doTestStepL();
};

/**
  Compares the text contents of a SMS message with some text
*/
class CCompareSMS : public CSMSTestSteps
{
public:
	CCompareSMS(CSmsMessage &aSmsMessage);
	
	//From CTestStep
	virtual enum TVerdict doTestStepL();
};


/**
  Checks if the Class0 Stores have been enabled
*/
class CClass0StoreEnabled : public CSMSTestSteps
{
public:
	CClass0StoreEnabled(RSocketServ &aSocketServer);

	//From CTestStep
	virtual enum TVerdict doTestStepL();
};

/**
  Test for Enumeration in an OOD Condition 
*/
class CEnumerateInOODCondition : public CSMSTestSteps
{
public:
	CEnumerateInOODCondition(RSocketServ &aSocketServer);

	//From CTestStep
	virtual enum TVerdict doTestStepL();
};

/**
  Test Esock for any memory leaks
*/
class CESockMemoryLeakTest : public CSMSTestSteps
{
public:
	//From CTestStep
	virtual enum TVerdict doTestStepL();
};



#endif



