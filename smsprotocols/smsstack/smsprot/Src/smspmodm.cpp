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
// Implements CSmsPhoneInitialization and CSmsModemNotification
// 
//

/**
 @file
*/

#include "smspmodm.h"
#include "smsuset.h"
#include "smspmain.h"
#include "SmsuTimer.h"
#include "smspread.h"


//
// implementation of CSmsPhoneInitialization
//


/**
 *  D'tor
 */
CSmsPhoneInitialization::~CSmsPhoneInitialization()
	{
	Cancel();
	} // CSmsPhoneInitialization::~CSmsPhoneInitialization


/**
 *  start initializing the phone
 */
void CSmsPhoneInitialization::Start()
	{
	LOGSMSPROT1("CSmsPhoneInitialization::Start  Start initializing the phone");

	Cancel();

	iInitialized = KErrNotReady;
	iNetworkInfoAvailable =EFalse;

	iState=ESmsPhoneInitializationPhoneInit;
	iGsmPhone.Initialise(iStatus);
	SetActive();
	} // CSmsPhoneInitialization::Start


void CSmsPhoneInitialization::DoRunL()
	{
	LOGSMSPROT3("CSmsPhoneInitialization::DoRunL [iStatus=%d, iState=%d]", iStatus.Int(), iState);

	switch (iState)
		{
		case ESmsPhoneInitializationPhoneInit:
			{
			AfterPhoneInitialize(iStatus.Int());
			}
			break;

		case ESmsPhoneInitializationSettingMoBearer:
			{
			if (iStatus.Int() != KErrNone)
				{
				LOGSMSPROT1("ERROR - TSY failed to set bearer but initialisation will continue; bearer setting may be successful during send");
				}

			iState=ESmsPhoneInitializationGettingRegistrationStatus;
			iGsmPhone.GetNetworkRegistrationStatus(iStatus, iRegistrationStatus);
			SetActive();
			}
			break;

		case ESmsPhoneInitializationGettingRegistrationStatus:
			{
			if(iStatus == KErrNone)
				{
				LOGSMSPROT2("CSmsPhoneInitialization::DoRunL registration status  : >%d<",iRegistrationStatus);
				if (iRegistrationStatus==RMobilePhone::ERegisteredOnHomeNetwork  ||
					iRegistrationStatus == RMobilePhone::ERegisteredRoaming)
					{
					iState=ESmsPhoneInitializationGettingCurrentNetworkInfo;
					iGsmPhone.GetCurrentNetwork(iStatus, iNetworkInfoPckg);
					SetActive();
					}
				else
					{
					iState=ESmsPhoneInitializationWaitingForCurrentNetwork;
					iGsmPhone.NotifyCurrentNetworkChange(iStatus,iNetworkInfoPckg);
					SetActive();
					}
				}
			else
				{
				iState=ESmsPhoneInitializationCompleted;
				iNetworkInfoAvailable =EFalse;
				LOGSMSPROT2("CSmsPhoneInitialization::DoRunL NETWORK INFO NOT available due %d",iStatus.Int());
				iSmsPDURead.Start();
				}
			}
			break;

		case ESmsPhoneInitializationWaitingForCurrentNetwork:
			{
			if(iStatus == KErrNone)
				{
				iState=ESmsPhoneInitializationGettingCurrentNetworkInfo;
				iGsmPhone.GetCurrentNetwork(iStatus, iNetworkInfoPckg);
				SetActive();
				}
			else
				{
				iState=ESmsPhoneInitializationCompleted;
				iNetworkInfoAvailable =EFalse;
				LOGSMSPROT2("CSmsPhoneInitialization::DoRunL NETWORK INFO NOT available due %d",iStatus.Int());
				iSmsPDURead.Start();
				}
			}
			break;

		case ESmsPhoneInitializationGettingCurrentNetworkInfo:
			{
			iState=ESmsPhoneInitializationCompleted;
			if(iStatus == KErrNone)
				{
				iNetworkInfoAvailable =ETrue;
				LOGSMSPROT2("CSmsPhoneInitialization::DoRunL network longname : >%S<",&iNetworkInfo.iLongName);
				LOGSMSPROT2("CSmsPhoneInitialization::DoRunL network shortname : >%S<",&iNetworkInfo.iShortName);
				LOGSMSPROT2("CSmsPhoneInitialization::DoRunL network shortname : >%S<",&iNetworkInfo.iDisplayTag);
				}
			else
				{
				iNetworkInfoAvailable =EFalse;
				LOGSMSPROT2("CSmsPhoneInitialization::DoRunL NETWORK INFO NOT available due %d",iStatus.Int());
				}
			iSmsPDURead.Start();
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
	} // CSmsPhoneInitialization::DoRunL


/**
 *  cancel the initialization
 */
void CSmsPhoneInitialization::DoCancel()
	{
	LOGSMSPROT1("CSmsPhoneInitialization::DoCancel()");

	switch (iState)
		{
		case ESmsPhoneInitializationPhoneInit:
			{
			iGsmPhone.InitialiseCancel();
			}
			break;

		case ESmsPhoneInitializationSettingMoBearer:
			{
			iInitialisationSetBearer->Cancel();
			}
			break;

		case ESmsPhoneInitializationGettingRegistrationStatus:
			{
			iGsmPhone.CancelAsyncRequest(EMobilePhoneGetNetworkRegistrationStatus);
			}
			break;

		case ESmsPhoneInitializationWaitingForCurrentNetwork:
			{
			iGsmPhone.CancelAsyncRequest(EMobilePhoneNotifyCurrentNetworkNoLocationChange);
			}
			break;

		case ESmsPhoneInitializationGettingCurrentNetworkInfo:
			{
			iGsmPhone.CancelAsyncRequest(EMobilePhoneGetCurrentNetwork);
			}
			break;

		case ESmsPhoneInitializationCompleted:
		case ESmsPhoneInitializationIdle:
			{
			// NOP
			}
			break;

		default:
 			{
			SmspPanic(KSmspPanicUnexpectedState);
			}
			break;
       }

	iInitialized = KErrNotReady;
	} // CSmsPhoneInitialization::DoCancel


/**
 *  Completes the phone initialization request
 *  
 *  @param aStatus a status value
 */
void CSmsPhoneInitialization::Complete(TInt aStatus)
	{
	LOGSMSPROT1("CSmsPhoneInitialization::Complete()");

	//
	// Call the base function to perform the actual complete...
	//
	CSmsuActiveBase::Complete(aStatus);
	} // CSmsPhoneInitialization::Complete


/**
 *  Read the TSY caps and start CSmsPDURead if aStatus == KErrNone
 *  Otherwise, issue another asynchronous Initialize request.
 *  
 *  @param aStatus An error code indicating the status returned on the
 *  previous call to RPhone::Initialise().
 */
void CSmsPhoneInitialization::AfterPhoneInitialize(TInt aStatus)
	{
    LOGSMSPROT2("CSmsPhoneInitialization::AfterPhoneIntialize [status=%d]", aStatus);

	if (aStatus == KErrNone)
		{
		RMobileSmsMessaging::TMobileSmsCapsV1Pckg smsCapsPckg(iMobileSmsCaps);
		aStatus = iSmsMessaging.GetCaps(smsCapsPckg);
		}

	if (aStatus == KErrNone)
		{
		if (iMobileSmsCaps.iSmsMode != RMobileSmsMessaging::KCapsGsmSms)
			{
		    LOGSMSPROT1("TSY does not support GsmSms");
			aStatus = KErrNotSupported;
			}
		else
			{
			// Initialisation of phone is complete and phone supports GSM SMS.
			// Therefore, set the MO SMS bearer.
			iState=ESmsPhoneInitializationSettingMoBearer;
			iInitialisationSetBearer->NotifyBearerSet(iStatus);
			SetActive();
			}
		}
	else
		{
	    if (++iNumberOfAttempts < iMaxNumberOfAttempts)
			{
			LOGSMSPROT2("CSmsPhoneInitialization Restarting [aStatus=%d]", aStatus);
			iGsmPhone.Initialise(iStatus);
			SetActive();
			}
		else
			{
			LOGSMSPROT3("CSmsPhoneInitialization Failed after %d attempts [aStatus=%d]", iNumberOfAttempts, aStatus);
			}
		}

	iInitialized = aStatus;
	} // CSmsPhoneInitialization::AfterPhoneInitialize


/**
 *  Default constructor.
 */
CSmsPhoneInitialization::CSmsPhoneInitialization(RMobileSmsMessaging& aSmsMessaging, RMobilePhone& aGsmPhone, CSmsPDURead& aSmsPDURead,
                                                 RMobileSmsMessaging::TMobileSmsCapsV1& aMobileSmsCaps, TInt aPriority, CSmspSetBearer* aInitialisationSetBearer)
	:CSmsuActiveBase(aPriority)
	,iState(ESmsPhoneInitializationPhoneInit)
	,iSmsMessaging(aSmsMessaging)
	,iGsmPhone(aGsmPhone)
	,iSmsPDURead(aSmsPDURead)
	,iMobileSmsCaps(aMobileSmsCaps)
	,iMaxNumberOfAttempts(KMaxNumberOfInitiaisationAttempts)
	,iNetworkInfoPckg(iNetworkInfo)
	,iInitialisationSetBearer(aInitialisationSetBearer)
	{
	} // RMobileSmsMessaging::TMobileSmsCapsV1

//
// implementation of CSmsModemNotification
//


/**
 *  2 phase constructor - safely create a CSmsModemNotification object
 */
CSmsModemNotification* CSmsModemNotification::NewL(MSmsComm& aSmsComm)
	{
	LOGSMSPROT1("CSmsModemNotification::NewL");

	CSmsModemNotification*	self=new(ELeave) CSmsModemNotification(aSmsComm);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
	} // CSmsModemNotification::NewL


void CSmsModemNotification::ConstructL()
	{
	LOGSMSPROT1("CSmsModemNotification::ConstructL()");

	User::LeaveIfError(iPhonePowerProperty.Attach(KUidSystemCategory, KUidPhonePwr.iUid));
	
	ConstructTimeoutL();
	} // CSmsModemNotification::ConstructL


/**
 *  D'tor
 */
CSmsModemNotification::~CSmsModemNotification()
	{
	Cancel();
	iPhonePowerProperty.Close();
	} // CSmsModemNotification::~CSmsModemNotification


RPhone::TModemDetection CSmsModemNotification::ModemState()
	{
	LOGSMSPROT1("CSmsModemNotification::ModemState()");

	//
	// Get the phone power state.
	//
	TInt phonePowerState;

	if (iPhonePowerProperty.Get(phonePowerState) != KErrNone)
		{
		phonePowerState = ESAPhoneOff;
		}

	if (phonePowerState == ESAPhoneOn)
		{
		return RPhone::EDetectedPresent;
		}
	else
		{
		return RPhone::EDetectedNotPresent;
		}
	} // CSmsModemNotification::ModemState


/**
 *  start the modem notification
 */
void CSmsModemNotification::Start()
	{
	LOGSMSPROT1("CSmsModemNotification::Start");

	__ASSERT_DEBUG(iState==ESmsModemNotificationIdle,SmspPanic(KSmspPanicUnexpectedState));

	//
	// Subscribe to the power property...
	//
	iState = ESmsModemNotificationWaitingForNotification;
	iPhonePowerProperty.Subscribe(iStatus);

	SetActive();
	} // CSmsModemNotification::Start


/**
 *  asyncronous completion - called by ActiveScheduler when modem notification completed
 */
void CSmsModemNotification::DoRunL()
	{
    LOGSMSPROT2("CSmsModemNotification::RunL [iStatus=%d]", iStatus.Int() );

	__ASSERT_DEBUG(iState==ESmsModemNotificationWaitingForNotification,SmspPanic(KSmspPanicUnexpectedState));

	//
	// DoRunL() will now return to CSmsuActiveBase which if the object
	// is not active, will call Complete().
	//
	} // CSmsModemNotification::DoRunL


/**
 *  cancel the modem notification
 */
void CSmsModemNotification::DoCancel()
	{
	LOGSMSPROT1("CSmsModemNotification::DoCancel()");

	__ASSERT_DEBUG(iState==ESmsModemNotificationWaitingForNotification,SmspPanic(KSmspPanicUnexpectedState));
	
	//
	// Cancel the request...
	//
	iPhonePowerProperty.Cancel();

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
	} // CSmsModemNotification::DoCancel


/**
 *  C'tor
 */
CSmsModemNotification::CSmsModemNotification(MSmsComm& aSmsComm)
  : CSmsuActiveBase(KSmsSessionPriority),
	iState(ESmsModemNotificationIdle),
	iSmsComm(aSmsComm)
	{
	} // CSmsModemNotification::CSmsModemNotification


/**
 *  complete the modem notification - inform the observer
 */
void CSmsModemNotification::Complete(TInt aStatus)
	{
	LOGSMSPROT1("CSmsModemNotification::Complete()");

	//
	// Call the base function to perform the actual complete...
	//
	CSmsuActiveBase::Complete(aStatus);

	iState = ESmsModemNotificationIdle;

	//
	// If the request was not cancelled, then restart ourselves ready
	// for the next event...
	//
	if (aStatus != KErrCancel  &&  aStatus != KErrTimedOut)
		{
		Start(); // starting here to ensure no states are missed
		}

	//
	// Notify our client that the event occured...
	//
	RPhone::TModemDetection  modemDetection = ModemState();

	iSmsComm.ModemNotificationCompleted(aStatus, modemDetection);
	} // CSmsModemNotification::Complete
