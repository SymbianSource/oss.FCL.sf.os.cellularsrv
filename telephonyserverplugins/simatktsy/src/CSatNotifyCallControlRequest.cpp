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
// Name        : CSatNotifyCallControlRequest.cpp
// Part of     : Common SIM ATK TSY / commonsimatktsy
// Get inkey notification functionality of Sat Tsy
// Version     : 1.0
//



//INCLUDES
#include <satcs.h>							// Etel SAT IPC definitions
#include "CSatTsy.h"						// Tsy class header
#include "CSatNotifyCallControlRequest.h"   // Tsy class header
#include "CSatNotificationsTsy.h"			// Class header
#include "CBerTlv.h"						// Ber Tlv data handling
#include "CSatDataPackage.h"				// Parameter packing 
#include "TfLogger.h"						// For TFLOGSTRING
#include "TSatUtility.h"					// Utilities
#include "CSatTsyReqHandleStore.h"			// Request handle class
#include "cmmmessagemanagerbase.h"			// Message manager class for forwarding req.

// -----------------------------------------------------------------------------
// CSatNotifyCallControlRequest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//  
CSatNotifyCallControlRequest* CSatNotifyCallControlRequest::NewL
        ( 
        CSatNotificationsTsy* aNotificationsTsy 
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyCallControlRequest::NewL");
   	CSatNotifyCallControlRequest* const satNotifyCallControlRequest = 
        new ( ELeave ) CSatNotifyCallControlRequest( aNotificationsTsy );
    CleanupStack::PushL( satNotifyCallControlRequest );
    satNotifyCallControlRequest->ConstructL();
    CleanupStack::Pop( satNotifyCallControlRequest );
    TFLOGSTRING("CSAT: CSatNotifyCallControlRequest::NewL, end of method");
    return satNotifyCallControlRequest;
    }

// -----------------------------------------------------------------------------
// CSatNotifyCallControlRequest::~CSatNotifyCallControlRequest
// Destructor
// -----------------------------------------------------------------------------
//  
CSatNotifyCallControlRequest::~CSatNotifyCallControlRequest
        ( 
		// None
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyCallControlRequest::\
        ~CSatNotifyCallControlRequest");
    }
    
// -----------------------------------------------------------------------------
// CSatNotifyCallControlRequest::CSatNotifyCallControlRequest
// Default C++ constructor
// -----------------------------------------------------------------------------
//  
CSatNotifyCallControlRequest::CSatNotifyCallControlRequest
        ( 
        CSatNotificationsTsy* aNotificationsTsy 
        ) : iNotificationsTsy ( aNotificationsTsy )
    {
    // None
    }

// -----------------------------------------------------------------------------
// CSatNotifyCallControlRequest::ConstructL
// Symbian 2nd phase constructor
// -----------------------------------------------------------------------------
//  
void CSatNotifyCallControlRequest::ConstructL
        (
        // None
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyCallControlRequest::ConstructL");
    }

// -----------------------------------------------------------------------------
// CSatNotifyCallControlRequest::Notify
// This request allows a client to be notified of a Call Control Request proactive 
// command
// -----------------------------------------------------------------------------
//
TInt CSatNotifyCallControlRequest::Notify
        (
        const TTsyReqHandle aTsyReqHandle,
        const TDataPackage& aPackage 
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyCallControlRequest::Notify");

    // Save data pointer to client side for completion
    iCallControlV2Pckg = reinterpret_cast<RSat::TCallControlV2Pckg*>( 
        aPackage.Des1n() ); 
    // Save the request handle
    iNotificationsTsy->iSatTsy->SaveReqHandle( aTsyReqHandle, 
		CSatTsy::ESatNotifyCallControlRequestPCmdReqType );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatNotifyCallControlRequest::CancelNotification
// This method cancels an outstanding asynchronous 
// NotifyDisplayText request.
// -----------------------------------------------------------------------------
//
TInt CSatNotifyCallControlRequest::CancelNotification
        (
        const TTsyReqHandle aTsyReqHandle // Tsy request handle
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyCallControlRequest::CancelNotification");   
    // Reset the request handle
    iNotificationsTsy->iSatReqHandleStore->
        ResetTsyReqHandle( CSatTsy::ESatNotifyCallControlRequestPCmdReqType );
    iCallControlV2Pckg = NULL;
    // Complete the request with KErrCancel
    iNotificationsTsy->iSatTsy->ReqCompleted( aTsyReqHandle, KErrCancel );
    return KErrNone;    
    }
    
// -----------------------------------------------------------------------------
// CSatNotifyCallControlRequest::CompleteNotifyL
// This method unpacks the parameters and requests for completion
// -----------------------------------------------------------------------------
//
TInt CSatNotifyCallControlRequest::CompleteNotifyL 
        (
        CSatDataPackage* aDataPackage,   
        TInt aErrorCode 
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyCallControlRequest::CompleteNotifyL");   
    RSat::TAlphaIdBuf alphaId;
    RSat::TControlResult result;
	// Unpack data
	aDataPackage->UnPackData( alphaId, result ); 
	CompleteAlphaId( alphaId, result, aErrorCode );
	return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatNotifyCallControlRequest::CompleteAlphaId
// This method completes an outstanding asynchronous 
// NotifyCallControlRequest request. 
// -----------------------------------------------------------------------------
//
void CSatNotifyCallControlRequest::CompleteAlphaId
        (
        RSat::TAlphaIdBuf& aAlphaId, 
        RSat::TControlResult aResult,
        TInt aErrorCode                
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyCallControlRequest::CompleteAlphaId");	

	// Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iNotificationsTsy->iSatReqHandleStore->
		ResetTsyReqHandle( CSatTsy::ESatNotifyCallControlRequestPCmdReqType );
		
    // Check that someone has requested this notification
    if ( CSatTsy::ESatReqHandleUnknown != reqHandle )
        {
        if ( KErrNone == aErrorCode )
            {
            // Temporary storage for Alpha Id 
            RSat::TAlphaId alphaIdTemp;
            // Alpha ID validity
            RSat::TAlphaIdValidity validity( RSat::EValidAlpaId );
            alphaIdTemp.iStatus = RSat::EAlphaIdProvided;
            alphaIdTemp.iAlphaId.Copy( aAlphaId );
            // Fill the call control structure
            RSat::TCallControlV2& callControlV2 = ( *iCallControlV2Pckg )();
            // Set default control result
            callControlV2.SetCcGeneralResult( aResult );          
            callControlV2.SetAlphaId( validity, alphaIdTemp );  
            }
        TFLOGSTRING( "CSAT: CSatNotifyCallControlRequest::CompleteAlphaId\
            completing" );
        // Complete notification
        iNotificationsTsy->iSatTsy->ReqCompleted( reqHandle, aErrorCode );
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatNotifyCallControlRequest::CompleteAlphaId\
            Request not ongoing" );
        }
    }

// End of file
