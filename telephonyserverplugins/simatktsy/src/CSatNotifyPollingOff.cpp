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
// Name        : CSatNotifyPollingOff.cpp
// Part of     : Common SIM ATK TSY / commonsimatktsy
// Polling off notification functionality of Sat Tsy
// Version     : 1.0
//



//INCLUDES
#include <satcs.h>                  // Etel SAT IPC definitions
#include "CSatTsy.h"                // Tsy class header
#include "CSatNotifyPollingOff.h"   // Tsy class header
#include "CSatNotificationsTsy.h"   // Class header
#include "CBerTlv.h"                // Ber Tlv data handling
#include "TTlv.h"					// TTlv class
#include "CSatDataPackage.h"        // Parameter packing 
#include "TfLogger.h"               // For TFLOGSTRING
#include "TSatUtility.h"            // Utilities
#include "CSatTsyReqHandleStore.h"  // Request handle class
#include "cmmmessagemanagerbase.h" 	// Message manager class for forwarding req.
#include "MSatTsy_IPCDefs.h"        // Sat Tsy internal request types

// -----------------------------------------------------------------------------
// CSatNotifyPollingOff::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//  
CSatNotifyPollingOff* CSatNotifyPollingOff::NewL
        ( 
        CSatNotificationsTsy* aNotificationsTsy 
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyPollingOff::NewL");
   	CSatNotifyPollingOff* const satNotifyPollingOff = 
        new ( ELeave ) CSatNotifyPollingOff( aNotificationsTsy );
    CleanupStack::PushL( satNotifyPollingOff );
    satNotifyPollingOff->ConstructL();
    CleanupStack::Pop( satNotifyPollingOff );
    TFLOGSTRING("CSAT: CSatNotifyPollingOff::NewL, end of method");
    return satNotifyPollingOff;
    }

// -----------------------------------------------------------------------------
// CSatNotifyPollingOff::~CSatNotifyPollingOff
// Destructor
// -----------------------------------------------------------------------------
//  
CSatNotifyPollingOff::~CSatNotifyPollingOff
        ( 
		// None
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyPollingOff::~CSatNotifyPollingOff");
    }
    
// -----------------------------------------------------------------------------
// CSatNotifyPollingOff::CSatNotifyPollingOff
// Default C++ constructor
// -----------------------------------------------------------------------------
//  
CSatNotifyPollingOff::CSatNotifyPollingOff
        ( 
        CSatNotificationsTsy* aNotificationsTsy 
        ) : iNotificationsTsy ( aNotificationsTsy )
    {
    // None
    }

// -----------------------------------------------------------------------------
// CSatNotifyPollingOff::ConstructL
// Symbian 2nd phase constructor
// -----------------------------------------------------------------------------
//  
void CSatNotifyPollingOff::ConstructL
        (
        // None
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyPollingOff::ConstructL");
    }

// -----------------------------------------------------------------------------
// CSatNotifyPollingOff::CompleteNotifyL
// This method completes an outstanding asynchronous 
// PollingOff request. 
// -----------------------------------------------------------------------------
// 
TInt CSatNotifyPollingOff::CompleteNotifyL
        ( 
        CSatDataPackage* aDataPackage,
        TInt /*aErrorCode*/  
        ) 
    {
    TFLOGSTRING("CSAT: CSatNotifyPollingOff::CompleteNotifyL");
    TPtrC8* data;
    TBuf<1> additionalInfo;
    aDataPackage->UnPackData( &data );
    // Get ber tlv 
    CBerTlv berTlv;
    berTlv.SetData( *data );
    // Get command details tlv
    CTlv commandDetails;
    berTlv.TlvByTagValue( &commandDetails, KTlvCommandDetailsTag );    
    // Store command details tlv
    iNotificationsTsy->iTerminalRespData.iCommandDetails.Copy( 
        commandDetails.Data() );	
	TUint8 pCmdNumber( commandDetails.GetShortInfo( ETLV_CommandNumber ) );
	
    if ( !( iNotificationsTsy->PollingOffStatus() ) )
        {
        iNotificationsTsy->SetPollingOffStatus( ETrue );
        iNotificationsTsy->SetPollingIntervalL( KDefaultPollInterval );
        // Terminal response is sent when the response 
        // from SIM server is received
        }
    else
        {
        // PollingOff is already off
        additionalInfo.Zero();
        additionalInfo.Append( RSat::KNoAdditionalInfo );
        CreateTerminalRespL( pCmdNumber, RSat::KSuccess, additionalInfo );
        }
    TInt returnValue( KErrNone );
    
    return returnValue;
    }

// -----------------------------------------------------------------------------
// CSatNotifyPollingOff::CreateTerminalRespL
// Constructs PollingOff specific part of terminal response and calls 
// DOS to send the actual message. 
// -----------------------------------------------------------------------------
//
TInt CSatNotifyPollingOff::CreateTerminalRespL
        ( 
        TUint8  aPCmdNumber,          
        TUint8  aGeneralResult,      
        TDesC16& aAdditionalInfo            
		)
    {
    TFLOGSTRING("CSAT: CSatNotifyPollingOff::CreateTerminalRespL");    
    
    TTlv tlvSpecificData;
    tlvSpecificData.AddTag( KTlvResultTag );
    tlvSpecificData.AddByte( aGeneralResult );

    if ( !( iNotificationsTsy->CommandPerformedSuccessfully( 
            aGeneralResult ) ) )
        {
        switch ( aGeneralResult )
            {
            // Cases in which additional info is not required
            case RSat::KPSessionTerminatedByUser:
            case RSat::KBackwardModeRequestedByUser:
            case RSat::KNoResponseFromUser:
            case RSat::KErrorRequiredValuesMissing:
            case RSat::KHelpRequestedByUser:
                {
                break;
                }
            default:
                {
                TFLOGSTRING2("CSAT: CSatNotifyPollingOff::\
                    CreateTerminalRespL, Additional Info: %d", 
                    aAdditionalInfo[0]);
                tlvSpecificData.AddByte( static_cast<TUint8>( 
                	aAdditionalInfo[0] ) );
                break;
                }
            }
        }
    // Prepare data
    iNotificationsTsy->iTerminalRespData.iPCmdNumber = aPCmdNumber;
    TPtrC8 data = tlvSpecificData.GetDataWithoutTopLevelTag();
    // Pack data
    CSatDataPackage dataPackage;
	dataPackage.PackData( &iNotificationsTsy->iTerminalRespData, &data );

    // Forward request to the DOS
    return iNotificationsTsy->iSatTsy->MessageManager()->HandleRequestL( 
		ESatTerminalRsp, &dataPackage );
    }


// End of file    
