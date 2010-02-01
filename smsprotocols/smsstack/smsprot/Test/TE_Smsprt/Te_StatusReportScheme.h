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


#ifndef __TE_STATUS_REPORT_SCHEME_H__
#define __TE_STATUS_REPORT_SCHEME_H__


#include "gsmuieoperations.h"
#include "gsmunonieoperations.h"
#include "TE_smsprt.h"

/**
  Base class housing common functionality used by all test steps exercising 
  status report functionality
*/
class CTestStatusReportScheme : public CSmsPrtTestStep
{
public:
	enum TCodingScheme{ESevenBit=7, EEightBit};

	CTestStatusReportScheme();
	~CTestStatusReportScheme();
	
	//Utilites
	void CreateSmsL(TCodingScheme, TPtrC);
	void SendSmsMessageL();
	void TriggerReceiveL();
	void GetNumberOfPDUs();

	//TPSRR
	void SetAllTPSRRsL();
	void SetLastTPSRRL();
	void SetSomeTPSRRL(TInt, TInt, TInt);
	
	//SMS
	void SetAllSMSCL();
	void SetLastSMSCL();
	void SetSomeSMSCL(TInt, TInt, TInt);
		
	//TEF framework	
	//virtual TVerdict doTestStepL();
 	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();

protected:
	TCodingScheme iDataCodingScheme;
	CSmsMessage* iSmsMessage;
	TInt iNumberOfPDUs;
	
private:
	void SetTestNumberL();
	void OpenSocketServerL();
	void SetCodingScheme();

protected:
	RSocketServ iSocketServer;
	RSocket iSocket;
};

#endif



