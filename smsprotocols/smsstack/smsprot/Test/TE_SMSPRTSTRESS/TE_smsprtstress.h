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


#ifndef __TE_SMSPRTSTRESS_H_
#define __TE_SMSPRTSTRESS_H_


class CTestInit : public CSmsPrtStressTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};

class CTestRxAndRead : public CSmsPrtStressTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};

class CTestMultipleSimultaneousRequests : public CSmsPrtStressTestStep
	{
public:
	virtual TVerdict doTestStepL();
	};

class CTestSmsPrtBackup : public CSmsPrtStressTestStep
	{
public:
	virtual TVerdict doTestStepL();

private:
	TInt SendSmsL(RSocket& aSocket);
	RSocketServ iSocketServer;
	};

class CTestSmsPrtBackupWhenPhoneIsOff : public CSmsPrtStressTestStep
	{
public:
	virtual TVerdict doTestStepL();

private:
	RSocketServ iSocketServer;
	};

#endif
