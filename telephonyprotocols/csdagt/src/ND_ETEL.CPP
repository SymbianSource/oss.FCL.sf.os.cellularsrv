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
// ETel Processor
// 
//

/**
 @file Nd_etel.cpp
*/

#include "ND_DBACC.H"
#include "ND_STD.H"
#include "ND_ETEL.H"
#include "SLOGGER.H"

/**
@internalComponent
*/
_LIT(KDoubleColon,"::");
_LIT(KTsyNameExtension,".tsy");

/**
@internalComponent
*/
const TInt KDefaultMaxDialAttempts=0;

// CTelServerProcessor definitions

CTelServerProcessor* CTelServerProcessor::NewL(CCommsDbNetDialAccess* aDb,TInt aPriority)
/**
2 phased constructor for CTelServerProcessor, first phase.

@param aDb a pointer to CommDB accessor.
@param aPriority is priority for this object.
@exception Leaves if ConstructL() leaves, or not enough memory is available.
@return a new CTelServerProcessor object.
*/
	{
	CTelServerProcessor* r=new(ELeave) CTelServerProcessor(aDb,aPriority);
	CleanupStack::PushL(r);
	r->ConstructL();
	CleanupStack::Pop();
	return r;
	}

CTelServerProcessor::CTelServerProcessor(CCommsDbNetDialAccess* aDb,TInt aPriority) 
	: CActive(aPriority), iDb(aDb), iCallParamsPckg(iCallParams), 
	iMmCallParamsPckg(iMmCallParams), iMmDataCallParamsPckg(iMmDataCallParams),
	iMmHscsdParamsPckg(iMmHscsdParams)
/**
Private constructor for CTelServerProcessor, used in the first phase of construction.

@param aDb a pointer to CommDB accessor.
@param aPriority is priority for this object.
*/
	{}

void CTelServerProcessor::ConstructL()
/**
Instantiate Member variable.
Add this object into active scheduler.
Connect to ETel server.

@exception Leaves if ETel server Connect() returns error, or not enough memory is available.
*/
	{
	CActiveScheduler::Add(this);

	__FLOG_STMT(_LIT8(logString,"NetDial:\tConnecting Etel Server");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
	User::LeaveIfError(iTelServer.Connect());
	iState=EIdle;
	iCallType=EUnknown;
	}

CTelServerProcessor::~CTelServerProcessor()
/**
Destructor.
Close active call.
If TSY is loaded, call UnloadPhoneModule() to unload it.
Close ETel server connection.
*/
	{
	__FLOG_STMT(_LIT8(logString1,"NetDial:\tClosing Call");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString1());
	CloseCall();
	__FLOG_STMT(_LIT8(logString2,"NetDial:\tClosing Etel Server");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString2());
	if(iTsyLoaded)
		{
		iTelServer.UnloadPhoneModule(iTsyName);
		iTsyLoaded=EFalse;
		}
	iTelServer.Close();
	}

void CTelServerProcessor::StartDialUpL(MNetDialEtelObserver& aObserver)
/**
Open call and set call parameters, resolve number and dial.
Call OpenNewCallL() to open call object from ETel server.
Call DoDiallingResolutionL() from CommDB accessor to get correct tel number.
Call DialL() from call object to dial the call.

@param aObserver a reference to observer.
@exception Leaves if OpenNewCallL(), DoDiallingResolutionL() or DialL() leaves.
*/
	{
	Assertions();
	iCurrentObserver=&aObserver;
	OpenNewCallL();
	iDb->DoDiallingResolutionL(iTelNum);
	DialL();
	}

void CTelServerProcessor::DialL()
/**
Dial number and increment dial counter.
Call GetRedialAttempts() from CommDB accessor to get max dial attempt count.
Check call type (ECoreCallOnly, EMmDataCall or EMmHscsdCall), get corresponding call parameters
and dial the call.

@exception Leaves if GetCallParamsL() leaves.
*/
	{
	__FLOG_STMT(_LIT(logString,"NetDial:\tDialling %S");)
	__FLOG_STATIC1(KNetDialLogFolder(),KNetDialLogFile(),TRefByValue<const TDesC>(logString()),&iTelNum);

	TInt ret=iDb->GetRedialAttempts(iMaxDialAttempts);
	if (ret!=KErrNone)
		{
		iMaxDialAttempts=KDefaultMaxDialAttempts;
		}
	if (iCallType==ECoreCallOnly)
		{
		iDb->GetCallParamsL(iCallParams);
		iCall.Dial(iStatus,iCallParamsPckg,iTelNum);
		}
	else if (iCallType==EMmDataCall)
		{
		iDb->GetCallParamsL(iMmDataCallParams);
		iDb->GetMmCallParams(iMmDataCallParams); // ignore return value, contimue without if they are not there
		iMmCall.Dial(iStatus,iMmDataCallParamsPckg, iTelNum);
		}
	else if (iCallType==EMmHscsdCall)
		{
		// When the call is opened, HSCSD parameters are already read when
		// it is required and if phone supports it!
		iDb->GetCallParamsL(iMmHscsdParams);
		iDb->GetMmCallParams(iMmHscsdParams); // ignore return value, contimue without if they are not there
		iMmCall.Dial(iStatus,iMmHscsdParamsPckg, iTelNum);
		}
	else
		{
		NetDialPanic(EUnknownCallType);
		}
		
	SetActive();
	iDialCounter++;
	iState=EDialUp;
	}

void CTelServerProcessor::StartReconnectL(MNetDialEtelObserver& aObserver)
/**
Dial number and increment dial counter.
If comm port is loaned, call ReturnCommPortL() to recover comm port to ETel.
Start reconnection by calling DialL().

@param aObserver a reference to observer.
@exception Leaves if ReturnCommPortL() or DialL() leaves.
*/
	{
	if (iLoaned)
		ReturnCommPortL();

	Assertions();
	__ASSERT_ALWAYS(iCallOpen, NetDialPanic(EEtelCallNotOpen));
	iCurrentObserver=&aObserver;

	DialL();
	}

#ifdef SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT	
void CTelServerProcessor::ListenForStatusChange(MNetDialEtelObserver& aObserver)
/**
Enable listening for a baseband disconnect
@param aObserver a reference to observer.
*/
	{
	__FLOG_STMT(_LIT8(logString,"NetDial:\tListening For Call Status Change");)
	__FLOG_STATIC(KNetDialLogFolder(), KNetDialLogFile(), logString());
	
	__ASSERT_ALWAYS(EIdle == iState, NetDialPanic(EEtelServerNotIdle));
	__ASSERT_ALWAYS(iCallOpen, NetDialPanic(EEtelCallNotOpen));
	
	iCurrentObserver = &aObserver;
	ActiveCall().NotifyStatusChange(iStatus, iCallStatus);
	SetActive();
	iState = EListenForStatusChange;
	}
	
void CTelServerProcessor::CancelListenForStatusChange()
	{
	__FLOG_STMT(_LIT8(logString,"NetDial:\tCancelListenForStatusChange. iState: [%d]. iStatus: [%d]");)
	__FLOG_STATIC2(KNetDialLogFolder(), KNetDialLogFile(), logString(), iState, iStatus.Int());
	
	__ASSERT_ALWAYS(iCallOpen, NetDialPanic(EEtelCallNotOpen));	
	
	Cancel();
	}

void CTelServerProcessor::HandleStatusChange()
/**
Handle call status change.
If the call dropped, notify the observer, otherwise re-issue status
change notification
*/
	{	
	__FLOG_STMT(_LIT8(logString,"NetDial:\tCall Status Changed. CallStatus %d Error: %d");)
	__FLOG_STATIC2(KNetDialLogFolder(), KNetDialLogFile(), logString(), iCallStatus, iStatus.Int());
	
	// Ignore if subscribing to status change notification failed.
	// It can be that the TSY doesn't support the feature etc.
	if (KErrNone != iStatus.Int()) 
		{
		iState = EIdle;
		return;
		}	
	
	__ASSERT_ALWAYS(iCallOpen, NetDialPanic(EEtelCallNotOpen));
		
	switch (iCallStatus)
		{
		case RCall::EStatusHangingUp:
		case RCall::EStatusIdle:
			{
			iState = EIdle;
			iCurrentObserver->TelFunctionComplete(iStatus.Int());
			break;
			}
		default:
			{
			iState = EListenForStatusChange;
			ActiveCall().NotifyStatusChange(iStatus, iCallStatus);
			SetActive();
			}
		}
	}
#endif // SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT
void CTelServerProcessor::WaitForIncomingCallL(MNetDialEtelObserver& aObserver)
/**
If comm port is loaned, call ReturnCommPortL() to recover comm port to ETel.for callback.
Call GetPhoneInfoL().
If call is not opened, call OpenNewCallL() to open call object from ETel.
Check call type (ECoreCallOnly, EMmDataCall or EMmHscsdCall), get corresponding call parameters,
call AnswerIncomingCall() and start to wait for incoming call.

@param aObserver a reference to observer.
@exception Leaves if GetPhoneInfoL() or GetCallParamsL()leaves.
*/
	{
	if (iLoaned)
		ReturnCommPortL();

	Assertions();
	
	iCurrentObserver=&aObserver;
	RTelServer::TPhoneInfo info;
	GetPhoneInfoL(info);

	if (!iCallOpen)
		OpenNewCallL();

	__FLOG_STMT(_LIT8(logString2,"NetDial:\tWaiting For Incoming Call");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString2());

	if (iCallType==ECoreCallOnly)
		{
		iDb->GetCallParamsL(iCallParams);
		iCall.AnswerIncomingCall(iStatus,iCallParamsPckg);
		}
	else if (iCallType==EMmDataCall)
		{
		iDb->GetCallParamsL(iMmDataCallParams);
		iDb->GetMmCallParams(iMmDataCallParams); // ignore return value, contimue without if they are not there
		iMmCall.AnswerIncomingCall(iStatus,iMmDataCallParamsPckg);
		}
	else if (iCallType==EMmHscsdCall)
		{
		// When the call is opened, HSCSD parameters are already read when
		// it is required and if phone supports it!
		iDb->GetCallParamsL(iMmHscsdParams);
		iDb->GetMmCallParams(iMmHscsdParams); // ignore return value, contimue without if they are not there
		iMmCall.AnswerIncomingCall(iStatus,iMmHscsdParamsPckg);
		}

	else
		NetDialPanic(EUnknownCallType);
	
	SetActive();
	iState=EWait;
	}

void CTelServerProcessor::StartHangUpAfterDialOut(MNetDialEtelObserver& aObserver)
/**
Call StartHangUp() to end the dialled call.

@param aObserver a reference to observer.
*/
	{
	iCurrentObserver=&aObserver;
	StartHangUp();
	iState=EHangUpDialOut;
	}

void CTelServerProcessor::StartHangUpAfterDialIn(MNetDialEtelObserver& aObserver)
/**
Call StartHangUp() to end the dialled call.

@param aObserver a reference to observer.
*/
	{
	iCurrentObserver=&aObserver;
	StartHangUp();
	iState=EHangUpDialIn;
	}

void CTelServerProcessor::StartHangUp()
/**
Call HangUp() from active call object.
Set object active to wait for completition of request.
*/
	{
#ifdef SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT
	CancelListenForStatusChange();
#endif	
	Assertions();

	__FLOG_STMT(_LIT8(logString,"NetDial:\tHanging Up Call");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
	ActiveCall().HangUp(iStatus);
	SetActive();
	}

void CTelServerProcessor::GetCommPortL(RCall::TCommPort& aCommPort)
/**
Call LoanDataPort() to loan comm port from ETel server.
Call SetCommPortL() from CommDB accessor to set the comm port into CommDB.

@param aCommPort returns reference to loaned comm port.
@exception Leaves if LoanDataPort() returns error or SetCommPortL leaves.
*/
	{
	__FLOG_STMT(_LIT8(logString,"NetDial:\tLoaning Port From Etel");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
	User::LeaveIfError(ActiveCall().LoanDataPort(aCommPort));
	iDb->SetCommPortL(aCommPort);
	iLoaned=ETrue;
#ifndef SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT
	iState=EIdle;
#endif
	}	

void CTelServerProcessor::ReturnCommPortL()
/**
Call RecoverDataPort() to return comm port to ETel server.

@exception Leaves if RecoverDataPort() returns error.
*/
	{
	__FLOG_STMT(_LIT8(logString,"NetDial:\tRecovering Port To Etel");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
	User::LeaveIfError(ActiveCall().RecoverDataPort());
	iLoaned=EFalse;
#ifndef SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT
	iState=EIdle;
#endif
	}
	
void CTelServerProcessor::CloseCall()
/**
If comm port is loaned, call ReturnCommPortL() and trap error.
If call is open, call Close() from active call object.
*/
	{
	__FLOG_STMT(_LIT8(logString,"NetDial:\tClosing Call");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
	if (iLoaned)
		{
		TRAPD(ret,ReturnCommPortL());		// can't do anything with the error, so just trap
		if (KErrNone != ret)
			{
			__FLOG_STMT(_LIT8(logString2,"ReturnCommPort:\tError Occured");)
			__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString2());
			}
		}

	if (iCallOpen)
		{
		ActiveCall().Close();
		iCallOpen=EFalse;
		}

	iCallType=EUnknown;
	}

void CTelServerProcessor::DoCancel()
/**
Cancels active requests.
Ignore return value of cancels because we can't do anything with it!
*/
	{
	switch (iState)
		{
	case EDialUp:
		ActiveCall().DialCancel();
		break;
	case EWait:
		ActiveCall().AnswerIncomingCallCancel();
		break;
	case EHangUpDialOut:
	case EHangUpDialIn:
		ActiveCall().HangUpCancel();
		break;
#ifdef SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT		
	case EListenForStatusChange:
		ActiveCall().NotifyStatusChangeCancel();
		break;
#endif		
	default:
		break;
		}

	iDialCounter=0;
	iState=EIdle;
	}

void CTelServerProcessor::RunL()
/**
Request completed.
Match internal state and act accordingly.
*/
	{
	__ASSERT_ALWAYS(iCurrentObserver!=NULL, NetDialPanic(ENullCallBackContext));

	switch (iState)
		{
	case EDialUp:
			{
			TBool ret=EFalse;
			TRAPD(err,(ret=RepeatDialL()));
			if (err!=KErrNone)
				iStatus=err;
			if (err!=KErrNone || !ret)
				{
				iState=EIdle;
				iDialCounter=0;
				iCurrentObserver->TelFunctionComplete(iStatus.Int());
				}	
			}
		break;
	case EWait:
		iState=EIdle;
		iCurrentObserver->TelFunctionComplete(iStatus.Int());
		break;
	case EHangUpDialOut:
		iState=EIdle;
		iCurrentObserver->TelFunctionComplete(iStatus.Int());
		break;
	case EHangUpDialIn:
		iState=EIdle;
		iCurrentObserver->TelFunctionComplete(iStatus.Int());
		break;
#ifdef SYMBIAN_NETWORKING_CSDAGENT_BCA_SUPPORT		
	case EListenForStatusChange:
		HandleStatusChange();
		break;
#endif
	default:
		User::Leave(KErrNotFound);
		break;
		}
	}

TBool CTelServerProcessor::RepeatDialL()
/**
If iStatus.Int() is KErrEtelBusyDetected or KErrEtelNoAnswer and max dial count
is not exceeded, call DialL() to re-dial.

@exception Leaves if DialL() leaves.
@return ETrue if DialL() is called, otherwise EFalse.
*/
	{
	if ((iStatus.Int()==KErrEtelBusyDetected  || iStatus.Int()==KErrEtelNoAnswer ) && (iDialCounter<iMaxDialAttempts))
		{
		DialL();
		return ETrue;	
		}
	return EFalse;
	}

void CTelServerProcessor::Assertions()
/**
These two assertions are needed for all the functions so they are in a separate function.
*/
	{
	__ASSERT_ALWAYS(!iLoaned, NetDialPanic(EEtelPortNotRecovered));
	__ASSERT_ALWAYS(iState==EIdle, NetDialPanic(EEtelServerNotIdle));
	}

void CTelServerProcessor::OpenNewCallL()
/**
Load the TSY and open a new call from the ETEL server
*/
	{
	__ASSERT_ALWAYS(!iCallOpen, NetDialPanic(EEtelCallAlreadyOpen));
	__ASSERT_ALWAYS(iCallType==EUnknown, NetDialPanic(EEtelCallAlreadyOpen));

	TBuf<KCommsDbSvrMaxFieldLength> newTsyName;
	iDb->GetTsyNameL(newTsyName);
	// Remove unnecessary .TSY extension, if found
	if (newTsyName.Right(4).CompareF(KTsyNameExtension) == 0)
		newTsyName = newTsyName.Left(newTsyName.Length() - 4);

	TBool loaded=EFalse;
	if (iTsyName.Length()!=0)
		{
		if (iTsyName.CompareF(newTsyName)==KErrNone)	// the one we want is already loaded
			loaded=ETrue;
		else											// unload the one we were using
			{
			User::LeaveIfError(iTelServer.UnloadPhoneModule(iTsyName));
			iTsyLoaded=EFalse;
			}
		}

	if (!loaded)
		{
		User::LeaveIfError(iTelServer.LoadPhoneModule(newTsyName));
		iTsyName=newTsyName;
		iTsyLoaded=ETrue;
		}
	
	if (iTelServer.SetExtendedErrorGranularity(RTelServer::EErrorExtended)!=KErrNone)
		{
		User::LeaveIfError(iTelServer.SetExtendedErrorGranularity(RTelServer::EErrorBasic));
		}

	RTelServer::TPhoneInfo info;
	GetPhoneInfoL(info);
	
	TName callName;
	callName.Zero();
	callName.Copy(info.iName);		// phone name
	callName.Append(KDoubleColon);

	RPhone::TLineInfo lineInfo;
	GetLineInfoL(lineInfo,info.iName,RLine::KCapsData);
	callName.Append(lineInfo.iName);
	
	callName.Append(KDoubleColon);
	__FLOG_STMT(_LIT8(logString,"NetDial:\tOpening Call");)
	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());

	iDb->CopyIspInitStringToModemL();	// will not leave if the field is blank
	// Only MultimodeV1 and greater supported!
	if (info.iExtensions>=(TUint)KETelExtMultimodeV1)
		{
		User::LeaveIfError(iMmCall.OpenNewCall(iTelServer,callName));
		iCallOpen=ETrue;
		iCallType=EMmDataCall;
		SetMmParametersL();	// may change call type to EMmHscsdCall
		}
	else
		{
		User::LeaveIfError(iCall.OpenNewCall(iTelServer,callName));
		iCallOpen=ETrue;
		iCallType=ECoreCallOnly;
		}
	}

void CTelServerProcessor::GetPhoneInfoL(RTelServer::TPhoneInfo& aInfo)
	{
	TInt count;
	User::LeaveIfError(iTelServer.EnumeratePhones(count));
	if (count<=0)
		{
    	__FLOG_STMT(_LIT(logString,"NetDial:\tGetPhoneInfoL(): no phones found - leaving with -1");)
    	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
		User::Leave(KErrNotFound);
		}
	TInt i;
	TBool found=EFalse;
	for (i=0; i<count; i++)
		{
		TBuf<KCommsDbSvrMaxFieldLength> currentTsyName;
		User::LeaveIfError(iTelServer.GetTsyName(i,currentTsyName));
		// Remove unnecessary extension, if found
		if (currentTsyName.Right(4).CompareF(KTsyNameExtension) == 0)
			currentTsyName = currentTsyName.Left(currentTsyName.Length() - 4);
		// Check for match in TSY names
		if (currentTsyName.CompareF(iTsyName)==KErrNone)
			{
			User::LeaveIfError(iTelServer.GetPhoneInfo(i,aInfo));
			found=ETrue;
			break;
			}	
		}

	if (!found)
		{
		__FLOG_STMT(_LIT(logString,"NetDial:\tGetPhoneInfoL(): required phone not found - leaving with -1");)
    	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
		User::Leave(KErrNotFound);
		}
	}

void CTelServerProcessor::GetLineInfoL(RPhone::TLineInfo& aInfo,const TDesC& aPhoneName, TUint aLineType)
        {
        RPhone phone;
        User::LeaveIfError(phone.Open(iTelServer,aPhoneName));
        CleanupClosePushL(phone);
        
        TInt count = 0;
        User::LeaveIfError(phone.EnumerateLines(count));
	if (count <= 0)
		{
    	__FLOG_STMT(_LIT(logString,"NetDial:\tGetLineInfoL(): no line info available - leaving with -1");)
    	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
	        User::Leave(KErrNotFound);
		}
        
        TBool found = EFalse;
	for (TInt i=0; i<count && !found; ++i)
		{
		User::LeaveIfError(phone.GetLineInfo(i,aInfo));
		/* if (aInfo.iLineCapsFlags & aLineType) // Required line found
		        {
		        found=ETrue;
		        } */

		RLine line;
		User::LeaveIfError(line.Open(phone,aInfo.iName));
		CleanupClosePushL(line);

		RLine::TCaps caps;
		User::LeaveIfError(line.GetCaps(caps));
		if (caps.iFlags & aLineType) // Required line found
		        {
		        found=ETrue;
		        }
                CleanupStack::PopAndDestroy(&line);
	
		}

        CleanupStack::PopAndDestroy(&phone);
        if (!found)
        	{
        	__FLOG_STMT(_LIT(logString,"NetDial:\tGetLineInfoL(): required line not found - leaving with -1");)
        	__FLOG_STATIC(KNetDialLogFolder(),KNetDialLogFile(),logString());
	        User::Leave(KErrNotFound);
        	}
        }

void CTelServerProcessor::SetMmParametersL()
/**
Set the multimode call parameters.
Call HscsdSettingsAvailableL() from CommDB accessor to check if HSCSD is supported.
If available, check TSY's data call caps to find out if it supports HSCSD.
If HSCSD is supported also by TSY, call GetMmHscsdParametersL() from CommDB accessor.

@exception Leaves if HscsdSettingsAvailableL() and GetMmHscsdParametersL() leaves.
*/
	{
	__ASSERT_ALWAYS(iCallType==EMmDataCall, NetDialPanic(EAttemptHSCSDWhenNotMultimode));

	if (iDb->HscsdSettingsAvailableL())
		{
		RMobileCall::TMobileCallDataCapsV1 mmDataCaps;
		RMobileCall::TMobileCallDataCapsV1Pckg mmDataCapsPckg(mmDataCaps);
		// Get the data caps from the phone
		TInt ret = iMmCall.GetMobileDataCallCaps(mmDataCapsPckg);
		if (KErrNone == ret)
			{
			// Check if the HSCSD is supported
			if (mmDataCaps.iHscsdSupport != EFalse)
				{
				// Get the multimode HSCSD parameters and pass them in Dial.
				iDb->GetMmHscsdParametersL(iMmHscsdParams);
				iCallType=EMmHscsdCall;
				}
			}
		else if (KErrNotSupported != ret)
			User::Leave(ret);
		}
	}

RCall& CTelServerProcessor::ActiveCall()
/**
If iCallType is ECoreCallOnly, return iCall.
If iCallType is EMmDataCall or EMmHscsdCall, return iMmCall.
Otherwise panic with EUnknownCallType.

@return iCall or iMmCall.
*/
	{
	if (iCallType==ECoreCallOnly)
		return iCall;

	if (iCallType==EMmDataCall || iCallType==EMmHscsdCall)
		return iMmCall;

	NetDialPanic(EUnknownCallType);
	return iCall;		// will never happen because of panic
	}

TBool CTelServerProcessor::CommPortLoaned() const
/**
@return iLoaned.
*/
	{
	return iLoaned;
	}

TBool CTelServerProcessor::CallActive() const
/**
@return iCallType.
*/
	{
	return (iCallType != EUnknown);
	}

