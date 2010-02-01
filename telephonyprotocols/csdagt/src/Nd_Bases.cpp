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
// NetDial State machine base class implementations
// 
//

/**
 @file Nd_bases.cpp 
*/

#include "Nd_Bases.h"
#include "SLOGGER.H"
#include <comms-infras/eventlogger.h>
#include "ND_ETEL.H"
#include "ND_DBACC.H"
#include "ND_SCR.H"

// Netdial SM base class 

CNetdialSM::CNetdialSM(MAgentNotify& aControllerObserver, CDialogProcessor* aDlgPrc, CCommsDbAccess& aDbAccess)
	: CAgentSMBase(aControllerObserver,aDlgPrc,aDbAccess)
/**
Constructor.

@param aControllerObserver a reference to the observer.
@param aDlgPrc a reference to the dialog processor.
@param aDbAccess a reference to the CommDB accessor
*/
	{}

CNetdialSM::~CNetdialSM()
/**
Destructor.
Deletes objects used by Netdial Statemachine
*/
	{

	if(iNdScript)
		{
		delete iNdScript;
		}

	if(iLogger)
		{
		delete iLogger;
		}

	if(iTelServer)
		{
		delete iTelServer;
		}

	if(iDbAccess)
		{
		delete iDbAccess;
		}
	}

void CNetdialSM::ConstructL()
/**
2nd Phase of construction.
Instantiate Member variables.
*/
	{
	iDbAccess=CCommsDbNetDialAccess::NewL(iDb);
	iTelServer=CTelServerProcessor::NewL(iDbAccess,EPriorityStandard);
	iLogger=CEventLogger::NewL();
	}

#ifdef SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT
void CNetdialSM::GetUseScriptL()
/**
Determine if a script needs to be used.
@see CNetdialSM::UseScript.
*/
	{
	TInt len = 0;
	__FLOG_STMT(_LIT8(logString0,"Netdial:\tGetUseScriptL");)
	__FLOG_STATIC(KNetDialLogFolder(), KNetDialLogFile(), logString0);
	iDbAccess->GetScriptDetailsL(iUseScript, len);
	__FLOG_STMT(_LIT8(logString1,"Netdial:\tGetUseScriptL. iUseScript [%d]");)
	__FLOG_STATIC1(KNetDialLogFolder(), KNetDialLogFile(), logString1, iUseScript);
	}
#endif // SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT
TInt CNetdialSM::GetExcessData(TDes8& aBuffer)
/**
Get excess data buffer from script engine.

@param aBuffer a reference to the observer.
@return KErrNotFound if script is not found.
*/
	{
	__FLOG_STMT(_LIT8(logString,"NetDial:\tGetting Excess Data");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
	if (iNdScript!=NULL)
		return iNdScript->GetExcessData(aBuffer);
	else
		{	
		aBuffer.Zero();
		return KErrNotFound;
		}
	}

void CNetdialSM::DeleteScript()
/**
Deletes script.
*/
	{
	iNdScript->CleanupScript();
	iNdScript->Cancel();
	delete iNdScript;
	iNdScript=NULL;
	}

CGetLoginInfo::CGetLoginInfo(MAgentStateMachineEnv& aSMObserver, MNetdialEnv& aNdEnv)
	: CAgentStateBase(aSMObserver),iNdEnv(&aNdEnv)
/**
Get Login Info State constructor.

@param aSMObserver a reference to the state machine observer.
@param aNdEnv a reference to the Netdial which defines a set of utility functions.
*/
	{}

CGetLoginInfo::~CGetLoginInfo()
/**
Destructor.
Cancels active request.
*/
	{
	Cancel();
	}

void CGetLoginInfo::StartState()
/**
Get login info from database or from user.

Updates current progress to the state machine observer, checks if the script is used
and requests login parameters. If there is no error, sets the state active to wait for
completition for login parameters. If script is not used, calls JumpToRunl to drive the
state forward.
*/
	{
	iSMObserver->UpdateProgress(ECsdGettingLoginInfo,KErrNone);
	TInt ret=KErrNone;
	if (iNdEnv->UseScript())
		{
		TBool res=EFalse;
		TRAP(ret,res=iNdEnv->Script()->GetLoginParamsL(*this,iNdEnv->BaseEnv()->IsReconnect()));
		if(ret==KErrNone && !res)
			{
			iStatus=KRequestPending;
			SetActive();
			return;
			}
		}
	JumpToRunl(ret);
	return;
	}

void CGetLoginInfo::DoCancel()
/**
Cancel for active request.
If dialog processor is used, cancels it's requests.
If state is currently waiting for a request to be completed, calls ScriptFunctionComplete
with KErrCancel to cancel the request.
*/
	{
	if (iNdEnv->Script()!=NULL)
 		{
 		(iNdEnv->Script())->Cancel();
 		}
	if ((iNdEnv->BaseEnv())->DlgPrc()!=NULL)
		{
		(iNdEnv->BaseEnv())->DlgPrc()->Cancel();
		}
	if (iStatus == KRequestPending)
		{
		ScriptFunctionComplete(KErrCancel);
		}
	}

void CGetLoginInfo::RunL()
/**
Getting login info completed (should not complete if connection cancelled)
If there is an error then signal it. Pass login name and password from user 
to script executor, otherwise advance phase to and call ServiceStarted() 
*/
	{
	if(iStatus!=KErrNone)
		{
		__FLOG_STMT(_LIT(logString3,"Get Login Info");)
		__FLOG_STATIC2(KNetDialLogFolder(),KNetDialLogFile(),TRefByValue<const TDesC>(KCompletedPhaseLogString()), &logString3(), iStatus.Int());
		if (iNdEnv->Script()!=NULL)
			iNdEnv->Script()->CloseScript();
		iSMObserver->ConnectionComplete(ECsdGotLoginInfo,iStatus.Int());
		return;
		}
	iSMObserver->UpdateProgress(ECsdGotLoginInfo,KErrNone);
	
	if (!(iNdEnv->BaseEnv())->IsReconnect())
		{
		__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),KServiceStartedLogString());
		}
	else
		{
		__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),KServiceStartedReconnLogString());
		}
	iSMObserver->ServiceStarted();
	(iNdEnv->BaseEnv())->CompleteState(KErrNone);
	}

void CGetLoginInfo::ScriptFunctionComplete(TInt aError)
/**
Completes script function with the aError parameter.

@param aError a error passed for completition.
*/
	{
	TRequestStatus* status=&iStatus;
	User::RequestComplete(status,aError);
	}

CEventLogger* MNetdialEnv::Logger()
/**
@internalComponent
*/
	{ NetDialPanic(ELoggerNotDefined);return NULL; };

MAgentStateMachineEnv* CNetdialSM::BaseEnv()
	{ return this; };

CCommsDbNetDialAccess* CNetdialSM::NetDialDb()
	{ return iDbAccess; };

CNetDialScript* CNetdialSM::Script()
	{ return iNdScript; };

CTelServerProcessor* CNetdialSM::TelServPrc()
	{ return iTelServer; };

CEventLogger* CNetdialSM::Logger() 
	{ return iLogger; };

TBool CNetdialSM::UseScript() const 
	{ return iUseScript; }
