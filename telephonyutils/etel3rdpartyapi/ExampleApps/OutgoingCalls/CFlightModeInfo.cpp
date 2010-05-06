// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "CFlightModeInfo.h"

/**
Factory constructor.

@param  aController Pointer to MExecAsync object passed to constructor of 
                    CISVAPIBase
@return             Instance of CFlightModeInfo class
*/
CFlightModeInfo* CFlightModeInfo::NewL(MExecAsync* aController)
	{
	CFlightModeInfo* self = new(ELeave) CFlightModeInfo(aController);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Destructor.
Calls Cancel() to cancel outstanding requests.
*/
CFlightModeInfo::~CFlightModeInfo()
	{
	Cancel();
	}

/**
Gets the flight mode status and stores it in the iFlightModeV1Pckg.
*/
void CFlightModeInfo::DoStartRequestL()
	{
	// Retrieves the current flight mode status.
	iTelephony->GetFlightMode(iStatus, iFlightModeV1Pckg);
	SetActive();
	}

/**
Constructor called by the CFlightModeInfo::NewL() factory constructor.

@param aController Pointer to an MExecAsync object passed to constructor of 
                   CISVAPIBase
*/
CFlightModeInfo::CFlightModeInfo(MExecAsync* aController)
	: CISVAPIAsync(aController, KFlightModeInfo),
	  iFlightModeV1Pckg(iFlightModeV1)
	{
	// Empty method
	}

/**
Second phase constructor.
*/
void CFlightModeInfo::ConstructL()
	{
	// Empty method
	}

/**
Displays the flight mode status if the active object completed with no error.
*/
void CFlightModeInfo::RunL()
	{
	if(iStatus != KErrNone)
		{
		iConsole->Printf(KError);
		
		// Print the error status code
		iConsole->Printf(_L("%d\n"), iStatus.Int());
		}
	else
		{
		switch (iFlightModeV1.iFlightModeStatus)
			{
		case CTelephony::EFlightModeOff:
			iConsole->Printf(_L("Flight Status is Off, you can make a call!\n"));
			ExampleComplete();
			break;
		case CTelephony::EFlightModeOn:
			iConsole->Printf(_L("Flight Status is On, you can't make a call!\n"));
			User::After(5000000);
			AppTerminate();
			break;
			}
		}
	}

/**
Requests to receive notifications of change in the flight mode.
*/
void CFlightModeInfo::DoRequestNotificationL()
	{
	// Panic if this object is already performing an asynchronous operation. 
	// Application will panic if you call SetActive() on an already active object.
	_LIT( KNotifyPanic, "CFlightModeInfo Notify Method" );
	__ASSERT_ALWAYS( !IsActive(), User::Panic( KNotifyPanic, 1 ));
	iRequestNotify = ETrue;
	
	// Registers interest in receiving change notifications for events
	iTelephony->NotifyChange(	iStatus,
								CTelephony::EFlightModeChange,
								iFlightModeV1Pckg );
	SetActive();
	}

/**
Cancels asynchronous request to CTelephony::GetFlightMode()
*/
void CFlightModeInfo::DoCancel()
	{
	// Cancels an outstanding asynchronous request.
	iTelephony->CancelAsync(CTelephony::EFlightModeChangeCancel);
	}
