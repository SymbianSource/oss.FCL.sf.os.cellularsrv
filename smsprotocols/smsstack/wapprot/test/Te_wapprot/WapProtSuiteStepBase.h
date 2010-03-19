/**
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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



/**
 @file
*/

#if (!defined __WAPPROT_SUITE_STEP_BASE__)
#define __WAPPROT_SUITE_STEP_BASE__
#include <test/testexecutestepbase.h>
#include <connect/sbeclient.h>

#include <es_sock.h>
#include <es_wsms.h>
#include <smsuaddr.h>
#include <gsmumsg.h>
#include <gsmubuf.h>

#include <c32comm.h>
#include <etel.h>
#include <e32property.h>
#include <simtsy.h>
#include <smsuaddr.h>
#include <etelmm.h>
#include "WapProtSuiteDefs.h"
#include "wap_sock.h"
#include "smsustrm.h"

#if defined (__WINS__)
#define PDD_NAME _L("ECDRV")
#define PDD_NAME2 _L("ECDRV")
#define LDD_NAME _L("ECOMM")
#else
#define PDD_NAME _L("EUART1")
#define LDD_NAME _L("ECOMM")
#endif

using namespace conn;


// Location and name of the RSC file.
_LIT(KSMSUResourceDir, "C:\\private\\101f7989\\sms\\");
_LIT(KSMSUResourceFile, "C:\\private\\101f7989\\sms\\smsu.rsc");
// Location of directory for reserving disk space
_LIT(KTempDiskSpaceDirName, "C:\\sms\\temp\\");

class CWapProtSuiteStepBase : public CTestStep
	{
public:
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	
	
	
	//Utitilies
	void WaitForRecvL(RSocket& aSocket);
	CSmsMessage* RecvSmsL(RSocket& aSocket, TInt aIoctl = KIoctlReadMessageSucceeded);
	TUint64 SetHighLowLimitsAndDiskSpaceLevelL(TUint aHighDrop, TUint aLowDrop, TUint aFreeDrop, TUint64 aMax=0x7fffffff);
	void SetFreeDiskSpaceFromDropLevelL(TUint aFreeDrop);
	void SetLowHighLimitsInSmsuRscL(TInt64 aLowLimit, TInt64 aHighLimit);
	void SetFreeDiskSpaceL(TInt64 aNewFreeValue);
	void ReleaseDiskSpaceL();
	void ReadWapPortSettingsL(TWapAddr &aWapAddr);
	void SetTestNumberL();
	void SetWapAddrL();
	void SetCodingSchemeL();
	void SetMessageTypeL();
	
	//Socket for receiving status reports
	void SetupStatusReportSocketL();

private:
	void CheckSmsMessageL(CSmsMessage& aSmsmessagebuf, TPtrC8& aScnumber);

protected:
	void WaitForInitializeL();
	void InternaliseSmsDataAndCheckL(TDes8& aBuffer , TPtrC8& aScnumber);
	TInt GetMessageParameterLengthL(RSocket& aSock);
	void GetMessageParameterL(RSocket& aSock, TPtr8& aParameterStorePtr);
	void OpenSocketLC(RSocketServ& aSocketServer, RSocket& aSocket, TUint aAddrFamily = KWAPSMSAddrFamily, TUint aProtocol =KWAPSMSDatagramProtocol);
    void OpenSocketL(RSocketServ& aSocketServer, RSocket& aSocket, TUint aAddrFamily = KWAPSMSAddrFamily, TUint aProtocol =KWAPSMSDatagramProtocol);

protected:
	CActiveScheduler*  iScheduler;
	CSBEClient*  iSecureBackupEngine;
	RSocketServ iSocketServer;
	RSocket iSocket;
	RSocket iStatusReportSocket;
	TWapAddr iWapAddr;
	TWapSmsDataCodingScheme iCodingScheme;
	TSmsAddr iSmsAddr;
	};

#endif
