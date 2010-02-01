// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <commsdattypesv1_1.h>
#include <cdbcols.h>
#include "smspmain.h"
#include "smsuset.h"
#include "smspcdb.h"
#include "SmsuTimer.h"

using namespace CommsDat;


/**
 *  2 phase constructor.
 *  
 *  @param aSmsSettings a reference to the global SMS settings.
 *  @param aSmsMessaging a reference to the ETEL SMS messaging subsession.
 *  @param aPriority of the request.
 *  @leave Leaves if ContructL() leaves, or not enough memory is available.
 *  @return a new CSmspSetBearer object
 */
CSmspSetBearer* CSmspSetBearer::NewL(const TSmsSettings& aSmsSettings,RMobileSmsMessaging& aSmsMessaging, TInt aPriority)
	{
	LOGSMSPROT2("CSmspSetBearer::NewL aPriority = %d", aPriority);

	CSmspSetBearer* smsSetBearer=new(ELeave) CSmspSetBearer(aSmsSettings,aSmsMessaging, aPriority);
	CleanupStack::PushL(smsSetBearer);
	smsSetBearer->ConstructL();
	CleanupStack::Pop();

	return smsSetBearer;
	} // CSmspSetBearer::NewL


/**
 *  2-phase constructor, perform construction that can leave
 */
void CSmspSetBearer::ConstructL()
	{
	LOGSMSPROT1("CSmspSetBearer::ConstructL()");

	CSmspCommDbEvent::ConstructL();

	User::LeaveIfError(iProperty.Attach(KUidSystemCategory, KUidCommDbSMSBearerChange.iUid));
	} // CSmspSetBearer::ConstructL


/**
 *  Private constructor used in the first phase of construction.
 *  
 *  @param aSmsSettings a reference to the global SMS settings.
 *  @param aSmsMessaging a reference to the ETEL SMS messaging subsession.
 *  @param aPriority of the request.
 *  
 */
CSmspSetBearer::CSmspSetBearer(const TSmsSettings& aSmsSettings, RMobileSmsMessaging& aSmsMessaging, TInt aPriority)
	:CSmspCommDbEvent(aSmsSettings, aSmsMessaging, aPriority)
	,iState(ESmsSetBearerStateInit)
	{
	} // CSmspSetBearer::CSmspSetBearer


/**
 *  CSmspSetBearer destructor
 */
CSmspSetBearer::~CSmspSetBearer()
	{
	Cancel();
	} // CSmspSetBearer::~CSmspSetBearer


/**
 *  Notifies if SMS bearer is set to the TSY. Start setting bearer
 *  process if bearer is not set.
 */
void CSmspSetBearer::NotifyBearerSet(TRequestStatus& aStatus)
	{
	LOGSMSPROT2("CSmspSetBearer::NotifyBearerSet, aStatus = %d", aStatus.Int());
	if (!iBearerSet && IsActive())
		{
		Cancel();
		}

	Queue(aStatus);

	if (iBearerSet)
		{
		LOGSMSPROT1("CSmspSetBearer::NotifyBearerSet RequestComplete called");
		CSmsuActiveBase::Complete(KErrNone);
		}
	else
		{
		LOGSMSPROT1("CSmspSetBearer::NotifyBearerSet started");
		Start();
		}
	} // CSmspSetBearer::NotifyBearerSet


/**
 *  Starts the sequence for configuring the current SMS bearer on the phone/TSY.
 */
void CSmspSetBearer::Start()
	{
	LOGSMSPROT1("CSmspSetBearer::Start");
	// Cancel any outstanding request
	TRAPD(err, GetSmsBearerL(iBearer));

	iBearerSet = EFalse;

	if (err == KErrNone)
		{
		LOGSMSPROT2("CSmspSetBearer::GetSmsBearerL() left with %d", err);

		// Set the previous bearer to the one that has been read
		// from CommDB so that iPreviousBearer has an initial value
		// when the smsstack is first started.
		iPreviousBearer = iBearer;
		SetSmsBearer();
		}
	else
		{
		LOGSMSPROT2("CSmspSetBearer::Start failed to get SMS bearer, error = %d", err);
		Complete(err);
		}
	} // CSmspSetBearer::Start


/**
 *  Handles a completed request.
 *  
 *  Makes a state transition based on the result of the request and current state.
 *  
 *  @leave Panic if RunL is called while object is in idle state.
 */
void CSmspSetBearer::DoRunL()
	{
	LOGSMSPROT3("CSmspSetBearer::DoRunL(): iState=%d iStatus=%d", iState, iStatus.Int());

	switch (iState)
		{
		case ESmsSetBearerNotifyOnEvent:
			{
			if (iStatus.Int() == KErrNone)
				{
				TInt tempBearer;
#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
				CMDBSession *dbSession = CMDBSession::NewL(KCDVersion1_2);
#else
				CMDBSession *dbSession = CMDBSession::NewL(KCDVersion1_1);
#endif
				CleanupStack::PushL(dbSession);
				CMDBRecordSet<CCDGlobalSettingsRecord> globalSettingsRecord(KCDTIdGlobalSettingsRecord);
				TRAPD(err, globalSettingsRecord.LoadL(*dbSession));
				if (err != KErrNone)
					{
					LOGSMSPROT2("CSmspSetBearer::DoRunL, could not load global settings, leave error code = %d", err);
					User::Leave(err);
					}

				iPreviousBearer = iBearer;

				LOGSMSPROT2("CSmspSetBearer::DoRunL Storing previous bearer setting. Previous bearer now = %d", iBearer);

				tempBearer = ((CCDGlobalSettingsRecord*)globalSettingsRecord.iRecords[0])->iSMSBearer;
				iBearer = static_cast<RMobileSmsMessaging::TMobileSmsBearer>(tempBearer);

				CleanupStack::PopAndDestroy(dbSession);

				SetSmsBearer();
				}
			else
				{
				NotifyOnEvent();
				}
			}
			break;

		case ESmsSetBearerStateSettingBearer:
			{
			if (iStatus.Int() == KErrNone)
				{
				iBearerSet = ETrue;
				}
			else
				{
				// Set bearer to previous value, if status!=KErrNone
				// (e.g. KErrNotSupported)
				// Set global setting to previous value,
				// then complete.
				LOGSMSPROT3("CSmspSetBearer::DoRunL TSY failed to set MO SMS bearer. status = %d. Bearer = %d", iStatus.Int(), iBearer);

				iBearer = iPreviousBearer;
				LOGSMSPROT2("CSmspSetBearer::DoRunL Setting bearer back to previous setting. Bearer = %d", iBearer);

#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
				CMDBSession *dbSession = CMDBSession::NewL(KCDVersion1_2);
#else
				CMDBSession *dbSession = CMDBSession::NewL(KCDVersion1_1);
#endif
				CleanupStack::PushL(dbSession);
				CMDBRecordSet<CCDGlobalSettingsRecord> globalSettingsRecord(KCDTIdGlobalSettingsRecord);
				TRAPD(err, globalSettingsRecord.LoadL(*dbSession));
				if (err != KErrNone)
					{
					LOGSMSPROT2("CSmspSetBearer::DoRunL could not load global settings, error = %d", err);
					User::Leave(err);
					}

				((CCDGlobalSettingsRecord*)globalSettingsRecord.iRecords[0])->iSMSBearer = iPreviousBearer;
				TRAP(err, globalSettingsRecord.ModifyL(*dbSession));
				if (err != KErrNone)
					{
					LOGSMSPROT2("CSmspSetBearer::DoRunL could not modify global settings, error = %d", err);
					User::Leave(err);
					}

				CleanupStack::PopAndDestroy(dbSession);
				}
			}
			break;

		default:
			{
			SmspPanic(KSmspPanicUnexpectedState);
			}
			break;
		}

	//
	// DoRunL() will now return to CSmsuActiveBase which if the object
	// is not active, will call Complete().
	//
	} // CSmspSetBearer::DoRunL


/**
 *  Handles a request to cancel the state machine.
 *  Cancels any outstanding request and calls Complete().
 */
void CSmspSetBearer::DoCancel()
	{
	LOGSMSPROT1("CSmspSetBearer::DoCancel");

	TimedSetActiveCancel();

	// Explicitly set iBearerSet to false to ensure new bearer is set on each execution
	iBearerSet = EFalse;

	switch (iState)
		{
		case ESmsSetBearerStateInit:
			{
			// NOP
			}
			break;
			
		case ESmsSetBearerStateSettingBearer:
			{
			iSmsMessaging.CancelAsyncRequest(EMobileSmsMessagingSetMoSmsBearer);
			}
			break;

		case ESmsSetBearerNotifyOnEvent:
			{
			iProperty.Cancel();
			}
			break;

		default:
			{
			SmspPanic(KSmspPanicUnexpectedState);
			}
			break;
		}

	//
	// Handle completion of this Active Object. Note that the object
	// may well still be active at this point...
	//
	if (TimedOut())
		{
		Complete(KErrTimedOut);
		}
	else
		{
		Complete(KErrCancel);
		}
	} // CSmspSetBearer::DoCancel


/**
 *  Performs the state transition to CSmspSetBearer::ESmsSetBearerStateQueryingCommDb by
 *  querying CommDB for the current value.
 *  
 *  @param aBearer a reference to the TMobileSmsBearer.
 */
void CSmspSetBearer::GetSmsBearerL(RMobileSmsMessaging::TMobileSmsBearer& aBearer)
	{
	LOGSMSPROT1("CSmspSetBearer::GetSmsBearerL()");
	
#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CMDBSession* sess = CMDBSession::NewL(KCDVersion1_2);
#else
	CMDBSession* sess = CMDBSession::NewL(KCDVersion1_1);
#endif
	CleanupStack::PushL(sess);

	CMDBField<TUint32>* smsBearerField = NULL;

	smsBearerField = new(ELeave) CMDBField<TUint32>(KCDTIdSMSBearer);
	CleanupStack::PushL(smsBearerField);

	smsBearerField->SetRecordId(1);
	smsBearerField->LoadL(*sess);

	aBearer = static_cast<RMobileSmsMessaging::TMobileSmsBearer>(static_cast<TUint32>(*smsBearerField));
	LOGSMSPROT2("CSmspSetBearer::GetSmsBearerL(): aBearer=%d", aBearer);

	CleanupStack::PopAndDestroy(smsBearerField);
	CleanupStack::PopAndDestroy(sess);
	} // CSmspSetBearer::GetSmsBearerL


/**
 *  Performs the state transition to CSmspSetBearer::ESmsSetBearerStateSettingBearer
 *  by trying to set the sms bearer on the phone/TSY.
 */
void CSmspSetBearer::SetSmsBearer()
	{
	LOGSMSPROT2("CSmspSetBearer::SetSmsBearer, iBearer = %d", iBearer);

	iBearerSet = EFalse;
	iState = ESmsSetBearerStateSettingBearer;

	iSmsMessaging.SetMoSmsBearer(iStatus, iBearer);
	TimedSetActive(iSmsSettings.Timeout());
	} // CSmspSetBearer::SetSmsBearer


/**
 *  Wait for notification from property for when the CommDB global setting SMS_BEARER changes
 */
void CSmspSetBearer::NotifyOnEvent()
	{
	LOGSMSPROT1("CSmspSetBearer::NotifyOnEvent");

	iState = ESmsSetBearerNotifyOnEvent;
	CSmspCommDbEvent::NotifyOnEvent();
	} // CSmspSetBearer::NotifyOnEvent
