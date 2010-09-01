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
// Name        : CSatEventDownloadTsy.cpp
// Part of     : Common SIM ATK TSY / commonsimatktsy
// EventDownload-related functionality of Sat Tsy
// Version     : 1.0
//



#include <satcs.h>                  // Etel SAT IPC definitions
#include "CSatDataPackage.h"        // Parameter packing 
#include "CSatEventDownloadTsy.h"   // Class header
#include "CSatTsy.h"                // Sat Tsy class
#include "cmmmessagemanagerbase.h"  // Message manager class for forwarding req.
#include "TfLogger.h"               // For TFLOGSTRING
#include "CBerTlv.h"                // Ber Tlv
#include "TTlv.h"					// TTlv class
#include "MSatTsy_IPCDefs.h"        // Sat Tsy internal request types
#include "CSatNotificationsTsy.h"  	// KTonNpiBit8

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatEventDownloadTsy* CSatEventDownloadTsy::NewL
        (    
        CSatTsy* aSatTsy  
        )
    { 
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::NewL" );
    CSatEventDownloadTsy* const satEventDownloadTsy = 
        new ( ELeave ) CSatEventDownloadTsy();
    CleanupStack::PushL( satEventDownloadTsy );
    satEventDownloadTsy->iSatTsy = aSatTsy;
    satEventDownloadTsy->ConstructL();
    CleanupStack::Pop( satEventDownloadTsy );
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::NewL, end of method" );
    return satEventDownloadTsy;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::~CSatEventDownloadTsy
// Destructor
// -----------------------------------------------------------------------------
//    
CSatEventDownloadTsy::~CSatEventDownloadTsy
        (    
        void   
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::~CSatEventDownloadTsy" );
    // Unregister.
    iSatTsy->MessageManager()->RegisterTsyObject(
		CMmMessageManagerBase::ESatEventDownloadTsyObjType, NULL );
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CSatEventDownloadTsy
// C++ constructor
// -----------------------------------------------------------------------------
//
CSatEventDownloadTsy::CSatEventDownloadTsy
        (
        void
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CSatEventDownloadTsy" );
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::ConstructL
// Symbian 2nd phase constructor. Initialises internal attributes.
// -----------------------------------------------------------------------------
//    
void CSatEventDownloadTsy::ConstructL
        (    
        void
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::ConstructL" );
    // Register .
    iSatTsy->MessageManager()->RegisterTsyObject(
		CMmMessageManagerBase::ESatEventDownloadTsyObjType, this );
	iSetUpCallOngoing = EFalse; 
	iForceLocationStatusEnvelope = EFalse;
    }
    
// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::DoExtFuncL
// Handles event download IPC requests originating from ETel
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::DoExtFuncL
        (
        const TTsyReqHandle aTsyReqHandle, 
        const TInt /*aIpc*/, 
        const TDataPackage& aPackage 
        )
    {
    TFLOGSTRING2( "CSAT: CSatEventDownloadTsy::DoExtFuncL\
        . Handle:%d", aTsyReqHandle );

    TInt ret ( KErrNotSupported );

    // Get parameters (eventList, eventInfo) from package
    RSat::TEventDownloadBaseV2Pckg* eventInfoPckg =
        (RSat::TEventDownloadBaseV2Pckg*)aPackage.Des2n();
    RSat::TEventDownloadBaseV2& eventInfo = (*eventInfoPckg)();
    RSat::TEventList& singleEvent = *(RSat::TEventList*)aPackage.Ptr1();

    if ( RSat::KSatV2 == eventInfo.ExtensionId() )
        {
        // Branch and do whatever is necessary according to the event type..
        switch ( singleEvent )
            {
            case RSat::KUserActivity:
                {
                ret = CreateUserActivityEnvelopeL();
                break;
                }
            case RSat::KIdleScreenAvailable:
                {
                ret = CreateIdleScreenAvailableEnvelopeL();
                break;
                }
            case RSat::KBrowserTermination:
                {
                ret = CreateBrowserTerminationEnvelopeL(
                    ( ( RSat::TBrowserTerminationEventV2* )&eventInfo )->iCause );
                break;
                }
            case RSat::KLanguageSelection:
                {
                ret = CreateLanguageSelectionEnvelopeL(
                    ( ( RSat::TLanguageSelectionEventV2* )&eventInfo )->iLanguage );
                break;
                }
            case RSat::KDataAvailable:
            	{            	
                // DataAvailable will trigger a ReceiveData PCmd
                ret = CreateDataAvailableEnvelopeL(
                    ( ( RSat::TDataAvailableEventV2* )&eventInfo )->iStatus,
                    ( ( RSat::TDataAvailableEventV2* )&eventInfo )->iLength );
                break;
            	}
            case RSat::KChannelStatus:
            	{
                // When the ME detects one of the following changes:
                // -	a link is error, or
                // -	any other error,
                // which is not resulting from the execution of a proactive 
                // command, the ME shall inform the SIM that this has occurred.
                ret = CreateChannelStatusEnvelopeL(
                    ( ( RSat::TChannelStatusEventV2* )&eventInfo )->iStatus );
                break;
            	}
            // TSY is responsible for these events
            // but they are not reported to ETel
            //case RSat::KMTCall:
            //case RSat::KCallConnected:
            //case RSat::KCallDisconnected:
            //case RSat::KLocationStatus:
            //case RSat::KAccessTechnologyChange:

            // Events that can be reported by client, but are not supported by TSY
            //case RSat::KCardReaderStatus:
            //case RSat::KDisplayParamsChanges:
            //case RSat::KLocalConnection:
            default:
                {
                ret = KErrNotSupported;
                TFLOGSTRING( "CSAT: CSatEventDownloadTsy::DoExtFuncL, not supported" );
                break;
                }
            } // End switch  

        } // End if extensionid.

    // Complete the request
    iSatTsy->ReqCompleted( aTsyReqHandle, ret );

    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::SetUpEventList
// Sets up event list 
// -----------------------------------------------------------------------------
//  
void CSatEventDownloadTsy::SetUpEventList
        (
        TUint32 aEvents        // Bit mask of enabled events
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::SetUpEventList" );
    iEventList.SetEventList( aEvents );
        
    // Force sending of envelope at least once, if location status event set
    if ( iEventList.IsEnabled( RSat::KLocationStatus ) )
        {
        iForceLocationStatusEnvelope = ETrue;
        }
    // Reset flag if removal of event list or not location status event
    else
        {
        iForceLocationStatusEnvelope = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::SetSetUpCallStatus
// Set SetUpCall status
// -----------------------------------------------------------------------------
//
void CSatEventDownloadTsy::SetSetUpCallStatus
        (
        const TBool aStatus // SetUpCall status
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::SetSetUpCallStatus" );
    iSetUpCallOngoing = aStatus;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CancelService
// Since event download is served at once 
// we don't have to do anything here
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CancelService
        (
        const TInt /*aIpc*/,
        const TTsyReqHandle /*aTsyReqHandle*/
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CancelService" );
    // Since event download is served at once, there's anything to do here
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CompleteMTCallL
// Creates MT call envelope to NAA
// Not supported yet
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CompleteMTCallL
        (
        const CSatDataPackage* aDataPackage
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteMTCallL" );    
    TInt ret( KErrNone );

    if ( iEventList.IsEnabled( RSat::KMTCall ) )
        {
		TMTCallEnvelope* mTCallEnvelope;
    	aDataPackage->UnPackData( &mTCallEnvelope );        
        // Create envelope
        TTlv envelope;
        envelope.Begin  ( KBerTlvEventDownloadTag );

        // Event list
        envelope.AddTag ( KTlvEventListTag );
        envelope.AddByte( KEventMTCall );

        // Device identities
        envelope.AddTag ( KTlvDeviceIdentityTag );
        envelope.AddByte( KNetwork );
        envelope.AddByte( KSim );

        // Transaction identifier
        envelope.AddTag ( KTlvTransactionIdentifierTag );
        envelope.AddByte( mTCallEnvelope->iCmdNumber );

        if ( NULL != mTCallEnvelope->iTONandNPI )
            {
            // Bits 1 - 4 are NPI, bits 5 - 7 TON
            // Bit 8 always 1 -> OR'd to be sure of that
            // See 3GPP TS 11.11 10.5.1
            // and 3GPP 04.08 figure 10.5.91
            mTCallEnvelope->iTONandNPI |= KTonNpiBit8;

            envelope.AddTag ( KTlvAddressTag );
            envelope.AddByte( mTCallEnvelope->iTONandNPI );

            if ( mTCallEnvelope->iDiallingNumber.Length() )
                {
                // Optional dialling number
                envelope.AddData( mTCallEnvelope->iDiallingNumber );
                }
            else
            	{
            	TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteMTCallL,\
            	    Dialling number missing" );
            	}
            }            
		else
        	{	
        	TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteMTCallL,\
            	TON and NPI missing" );
        	}
        	
        // If subaddress present
        if ( mTCallEnvelope->iCalledPartySubAddress.Length() )
            {
            // Optional called party subaddress
            envelope.AddTag( KTlvSubaddressTag );
            envelope.AddData( mTCallEnvelope->iCalledPartySubAddress );
            }
        else
        	{
        	TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteMTCallL,\
            	Subaddress missing" );
        	}

        // Prepare data
        TPtrC8 data = envelope.End();
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
		    ESatEventDownload, &dataPackage );
        }
	else
    	{
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteMTCallL\
            Event not enabled" );
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CompleteCallConnectedL
// Creates Call connected envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CompleteCallConnectedL
        (
        const CSatDataPackage* aDataPackage
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteCallConnectedL" );    
    TInt ret = KErrNone;
    
    if ( iEventList.IsEnabled( RSat::KCallConnected ) )
        {
	    TUint8 cmdNumber;
	    TBool nearEnd;
	    aDataPackage->UnPackData( cmdNumber, nearEnd );
	    
	    TFLOGSTRING2( "CSAT: CSatEventDownloadTsy::CompleteCallConnectedL \
	        ( cmdNumber=%d )", ( TInt )cmdNumber ); 
	               
        // Create envelope
        TTlv envelope;
        envelope.Begin( KBerTlvEventDownloadTag );

        // Event list
        envelope.AddTag( KTlvEventListTag );
        envelope.AddByte( KEventCallConnected );
        // Device identities
        envelope.AddTag( KTlvDeviceIdentityTag );
        envelope.AddByte( nearEnd ? KMe : KNetwork );
        envelope.AddByte( KSim );
        // Transaction identifier
        envelope.AddTag( KTlvTransactionIdentifierTag );
        envelope.AddByte( cmdNumber );
        // Prepare data
        TPtrC8 data = envelope.End();
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );
        // Check if the call is SetUpCall originated and it's an MO call
        if ( iSetUpCallOngoing && !nearEnd )
            {
            // Forward data to CNotifySetUpCall it will be send after a 
            // terminal resp of set up call.  
            iSatTsy->StoreCallConnectedEvent( envelope.End() );
            }
        else
            {
            // Forward request to the DOS
             ret = iSatTsy->MessageManager()->HandleRequestL( 
		        ESatEventDownload, &dataPackage );
            }
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteCallConnectedL,\
            Event not enabled" );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CompleteCallDisconnectedL
// Creates Call disconnected envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CompleteCallDisconnectedL
        (
        const CSatDataPackage* aDataPackage
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteCallDisconnectedL" );
    TInt ret = KErrNone;

    if ( iEventList.IsEnabled( RSat::KCallDisconnected ) )
        {
	    TCallDisconnectedEnvelope* callDisconnectedEnvelope;
    	// Unpack data
    	aDataPackage->UnPackData( &callDisconnectedEnvelope );
    	// Unpack structure 
    	TUint8 cmdNumber = callDisconnectedEnvelope->iCmdNumber;
    	TBool nearEnd = callDisconnectedEnvelope->iNearEnd;
    	TBool causeGiven = callDisconnectedEnvelope->iCauseGiven;
    	const TDesC8& cause = callDisconnectedEnvelope->iCause; 
    	TFLOGSTRING2( "CSAT: CSatEventDownloadTsy::CompleteCallDisconnectedL \
        	( cmdNumber=%d )", ( TInt )cmdNumber );    	       
        // Create envelope
        TTlv envelope;
        envelope.Begin  ( KBerTlvEventDownloadTag );
        // Event list
        envelope.AddTag ( KTlvEventListTag );
        envelope.AddByte( KEventCallDisconnected );
        // Device identities
        envelope.AddTag ( KTlvDeviceIdentityTag );
        envelope.AddByte( nearEnd ? KMe : KNetwork );
        envelope.AddByte( KSim );
        // Transaction identifier
        envelope.AddTag ( KTlvTransactionIdentifierTag );
        envelope.AddByte( cmdNumber );
        // Cause
        if ( causeGiven )
            {
            envelope.AddTag ( KTlvCauseTag );
            envelope.AddData( cause );
            }
        else
            {
            TFLOGSTRING( "CSAT: CSatEventDownloadTsy::\
                CompleteCallDisconnectedL, cause not given" );
            }
        // Prepare data
        TPtrC8 data = envelope.End();
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
		    ESatEventDownload, &dataPackage );
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteCallDisconnectedL,\
            Event not enabled" );
        }
    return ret;
    }


// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CompleteLocationStatusL
// Creates Location status envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CompleteLocationStatusL
        (
        const CSatDataPackage* aDataPackage
        )
	{
	TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CompleteLocationStatusL" );	       	
    TInt ret( KErrNone );

    if ( iEventList.IsEnabled( RSat::KLocationStatus ) )
        {
	    CSatTsy::TLocationStatusEnvelope* locationStatusEnvelope;
	    // Unpack data
	    aDataPackage->UnPackData( &locationStatusEnvelope );
	    // Unpack structure 
	    TUint8 locationStatus = locationStatusEnvelope->iLocationStatus;
	    TBuf8<KCountryCodeLength> operatorCode = 
	        locationStatusEnvelope->iCountryCode;
	    TUint16 locationAreaCode = locationStatusEnvelope->iLocationAreaCode;
	    TUint16 cellId = locationStatusEnvelope->iCellId;

	    TFLOGSTRING2( "CSAT: CSatEventDownloadTsy::CompleteLocationStatusL \
	        ( locationStatus=%d )", ( TInt )locationStatus );
                
        // Send location status envelope only if there
        // have been changes or we're sending first
        // envelope after setup event list pcmd
        if ( ( iLocationStatus != locationStatus ) ||
             ( iOperatorCode   != operatorCode ) ||
             ( iLocationaAreaCode != locationAreaCode )||
             ( iCellId != cellId ) || 
             iForceLocationStatusEnvelope ) 
            {
            TFLOGSTRING( "CSAT: CSatEventDownload::\
                CompleteLocationStatusEnvelopeL, -- saving" );

            // Cache values
            iLocationStatus = locationStatus;
            iOperatorCode = operatorCode;
            iLocationaAreaCode  = locationAreaCode;
            iCellId = cellId;

            // Create envelope
            TTlv envelope;
            envelope.Begin  ( KBerTlvEventDownloadTag );
            // Event list
            envelope.AddTag ( KTlvEventListTag );
            envelope.AddByte( KEventLocationStatus );
            // Device identities
            envelope.AddTag ( KTlvDeviceIdentityTag );
            envelope.AddByte( KMe );
            envelope.AddByte( KSim );
            // Locationstatus
            envelope.AddTag ( KTlvLocationStatusTag );
            envelope.AddByte( TUint8(locationStatus) );

            // Location information, this is included only if status indicates 
            // normal service
            if ( KLocationStatusNormalService == locationStatus )
                {
                TFLOGSTRING( "CSAT: CSatEventDownload::CompleteLocationStatusEnvelopeL\
                    -- sending" );
                envelope.AddTag ( KTlvLocationInformationTag );
                envelope.AddData( operatorCode );        
                // Mobile country & network codes
                envelope.AddByte( TUint8( locationAreaCode >> 8 ) );// Hi byte
                envelope.AddByte( TUint8( locationAreaCode >> 0 ) );// Lo byte
                envelope.AddByte( TUint8( cellId >> 8 ) );          // Hi byte
                envelope.AddByte( TUint8( cellId >> 0 ) );          // Lo byte
                }
            else
                {
                // Do nothing
                }
            
            // Reset the flag
            iForceLocationStatusEnvelope = EFalse;

            // Prepare data
            TPtrC8 data = envelope.End();
            // Pack data
            CSatDataPackage dataPackage;
	        dataPackage.PackData( &data );
            // Forward request to the DOS
            ret = iSatTsy->MessageManager()->HandleRequestL( 
		        ESatEventDownload, &dataPackage );
            }
        else
            {
            // Do nothing
            }
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::\
            CompleteLocationStatusEnvelopeL, Event not enabled" );
        }             

    return ret;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CompleteAccessTechnologyChangeL
// Creates access technology change envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CompleteAccessTechnologyChangeL
        (
		const CSatDataPackage* aDataPackage
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::\
    		CompleteAccessTechnologyChangeEnvelopeL" );
    TInt ret( KErrNone );

    if ( iEventList.IsEnabled( RSat::KAccessTechnologyChange ) )
        {
	    TUint8 accTechChange; 
		aDataPackage->UnPackData( accTechChange );        
        // create envelope
        TTlv envelope;
        envelope.Begin  ( KBerTlvEventDownloadTag );

        // event list
        envelope.AddTag ( KTlvEventListTag );
        envelope.AddByte( KEventAccessTechnologyChange );

        // device identities
        envelope.AddTag ( KTlvDeviceIdentityTag );
        envelope.AddByte( KMe );
        envelope.AddByte( KSim );

        // channel status
        envelope.AddTag( KTlvAccessTechnologyTag );
        envelope.AddByte( accTechChange );
        
        // Prepare data
        TPtrC8 data = envelope.End();
        // Pack data
        CSatDataPackage dataPackage;
        dataPackage.PackData( &data );
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
	        ESatEventDownload, &dataPackage );
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::\
            CompleteAccessTechnologyChangeL, Event not enabled" );
        }        

    return ret;
    }
// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CreateUserActivityEnvelopeL
// Send user activity envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CreateUserActivityEnvelopeL
        (
        // None
        )
    {
    TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CreateUserActivityEnvelopeL" );
    TInt ret = KErrNone;
    if ( iEventList.IsEnabled( RSat::KUserActivity ) )
        {
        // Create envelope
        TTlv envelope;
        envelope.Begin  ( KBerTlvEventDownloadTag );
        // Event list
        envelope.AddTag ( KTlvEventListTag );
        envelope.AddByte( KEventUserActivity );
        // Device identities
        envelope.AddTag ( KTlvDeviceIdentityTag );
        envelope.AddByte( KMe );
        envelope.AddByte( KSim );
        // Prepare data
        TPtrC8 data = envelope.End();
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
		    ESatEventDownload, &dataPackage );
        
        // Remove from event list
        iEventList.RemoveFromEventList( RSat::KUserActivity );
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CreateUserActivityEnvelopeL,\
            Event not enabled" );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CreateIdleScreenAvailableEnvelopeL
// Send idle screen available envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CreateIdleScreenAvailableEnvelopeL
        (
        // None
        )
    {
    TFLOGSTRING("CSAT: CSatEventDownloadTsy::\
        CreateIdleScreenAvailableEnvelopeL");
    TInt ret = KErrNone;

    if ( iEventList.IsEnabled( RSat::KIdleScreenAvailable ) )
        {
        // Create envelope
        TTlv envelope;
        envelope.Begin  ( KBerTlvEventDownloadTag );
        // Event list
        envelope.AddTag ( KTlvEventListTag );
        envelope.AddByte( KEventIdleScreenAvailable );
        // Device identities
        envelope.AddTag ( KTlvDeviceIdentityTag );
        envelope.AddByte( KDisplay );
        envelope.AddByte( KSim );       
        // Prepare data
        TPtrC8 data = envelope.End();        
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
		    ESatEventDownload, &dataPackage );
        
        if ( KErrNone == ret )
            {
            // Remove from event list
            iEventList.RemoveFromEventList( RSat::KIdleScreenAvailable );
            }
        else
            {
            TFLOGSTRING("CSAT: CSatEventDownloadTsy::\
                CreateIdleScreenAvailableEnvelopeL, Error in forwarding req.");
            }
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::\
            CreateIdleScreenAvailableEnvelopeL, Event not enabled" );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CreateBrowserTerminationEnvelopeL
// Send browser termination envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CreateBrowserTerminationEnvelopeL
        (
        const RSat::TBrowserTerminationCause aCause
        )
    {
    TFLOGSTRING2( "CSAT: CSatEventDownloadTsy::\
        CreateBrowserTerminationEnvelopeL, ( cause=%d )",( TInt )aCause );

    TInt ret = KErrNone;

    if ( iEventList.IsEnabled( RSat::KBrowserTermination ) )
        {
        // Create envelope
        TTlv envelope;
        envelope.Begin  ( KBerTlvEventDownloadTag );
        // Event list
        envelope.AddTag ( KTlvEventListTag );
        envelope.AddByte( KEventBrowserTermination );
        // Device identities
        envelope.AddTag ( KTlvDeviceIdentityTag );
        envelope.AddByte( KMe );
        envelope.AddByte( KSim );
        // Cause
        envelope.AddTag( KTlvBrowserTerminationCauseTag );
        envelope.AddByte(
            TUint8( ( RSat::EUserTermination == aCause ) ?
            KBrowserTerminationUser : KBrowserTerminationError ) );
        // Prepare data
        TPtrC8 data = envelope.End();        
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );        
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
		    ESatEventDownload, &dataPackage );
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::\
            CreateBrowserTerminationEnvelopeL, Event not enabled" );
        }

    return ret;
    }


// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CreateLanguageSelectionEnvelopeL
// Send Language selection envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CreateLanguageSelectionEnvelopeL
        (
        const TUint16 aLanguage
        )
    {
    TFLOGSTRING2( "CSAT: CSatEventDownloadTsy::\
        CreateLanguageSelectionEnvelopeL, (language=%d)", ( TInt )aLanguage );
    
    TInt ret = KErrNone;

    if ( iEventList.IsEnabled(RSat::KLanguageSelection ) )
        {
        // Create envelope
        TTlv envelope;
        envelope.Begin  ( KBerTlvEventDownloadTag );
        // Event list
        envelope.AddTag ( KTlvEventListTag );
        envelope.AddByte( KEventLanguageSelection );
        // Device identities
        envelope.AddTag ( KTlvDeviceIdentityTag );
        envelope.AddByte( KMe );
        envelope.AddByte( KSim );
        // Language
        envelope.AddTag( KTlvLanguageTag );
        envelope.AddByte( TUint8( ( aLanguage >> 8 ) & 0x7f ) );// MSB
        envelope.AddByte( TUint8( ( aLanguage >> 0 ) & 0x7f ) );// LSB    
        // Prepare data
        TPtrC8 data = envelope.End();        
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );        
        
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
		    ESatEventDownload, &dataPackage );
        }
    else
    	{
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::\
            CreateLanguageSelectionEnvelopeL, Event not enabled" );	
    	}
    return ret;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CreateDataAvailableEnvelopeL
// Create data available envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CreateDataAvailableEnvelopeL
        (
        const RSat::TChannelStatus aStatus,
		const TInt8 aLength    
        )
    {
    TFLOGSTRING("CSAT: CSatEventDownload::CreateDataAvailableEnvelopeL");

    TInt ret = KErrNone;

    if ( iEventList.IsEnabled( RSat::KDataAvailable ) )
        {
        // Create envelope
        TTlv envelope;
        envelope.Begin( KBerTlvEventDownloadTag );
        // Event list
        envelope.AddTag( KTlvEventListTag );
        envelope.AddByte( KEventDataAvailable );
        // Device identities
        envelope.AddTag( KTlvDeviceIdentityTag );
        envelope.AddByte( KMe );
        envelope.AddByte( KSim );
        // Channel status
        envelope.AddTag( KTlvChannelStatusTag );
        envelope.AddData( aStatus );
        // Channel data length
        envelope.AddTag( KTlvChannelDataLengthTag );
        envelope.AddByte( aLength );
        // Prepare data
        TPtrC8 data = envelope.End();
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );        
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
		    ESatEventDownload, &dataPackage );
        }
    else
    	{
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CreateDataAvailableEnvelopeL\
            Event not enabled" );	
    	}
    return ret;
    }

// -----------------------------------------------------------------------------
// CSatEventDownloadTsy::CreateChannelStatusEnvelopeL
// Create channel status envelope to NAA
// -----------------------------------------------------------------------------
//
TInt CSatEventDownloadTsy::CreateChannelStatusEnvelopeL
        (
        const RSat::TChannelStatus aStatus
        )
    {
    TFLOGSTRING("CSAT: CSatEventDownloadTsy::CreateChannelStatusEnvelopeL");
    
    TInt ret( KErrNone );

    if ( iEventList.IsEnabled( RSat::KChannelStatus ) )
        {
        // Create envelope
        TTlv envelope;
        envelope.Begin  ( KBerTlvEventDownloadTag );
        // Event list
        envelope.AddTag ( KTlvEventListTag );
        envelope.AddByte( KEventChannelStatus );
        // Device identities
        envelope.AddTag ( KTlvDeviceIdentityTag );
        envelope.AddByte( KMe );
        envelope.AddByte( KSim );
        // Channel status
        envelope.AddTag( KTlvChannelStatusTag );
        envelope.AddData( aStatus );   
        // Prepare data
        TPtrC8 data = envelope.End();        
        // Pack data
        CSatDataPackage dataPackage;
	    dataPackage.PackData( &data );                
        // Forward request to the DOS
        ret = iSatTsy->MessageManager()->HandleRequestL( 
		    ESatEventDownload, &dataPackage );
        }
    else
    	{
        TFLOGSTRING( "CSAT: CSatEventDownloadTsy::CreateChannelStatusEnvelopeL\
            Event not enabled" );	
    	}        

    return ret;
    }
  
//  End of File 

