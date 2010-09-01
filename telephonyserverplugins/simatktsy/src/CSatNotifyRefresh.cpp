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
// Name        : CSatNotifyRefresh.cpp
// Part of     : Common SIM ATK TSY / commonsimatktsy
// Refresh notification functionality of Sat Tsy
// Version     : 1.0
//



//INCLUDES
#include <satcs.h>                  // Etel SAT IPC definitions
#include "CSatTsy.h"                // Tsy class header
#include "CSatNotifyRefresh.h"  	// Class header
#include "CSatNotificationsTsy.h"   // Tsy class header
#include "CBerTlv.h"                // Ber Tlv data handling
#include "TTlv.h"					// TTlv class
#include "CSatDataPackage.h"        // Parameter packing 
#include "TfLogger.h"               // For TFLOGSTRING
#include "TSatUtility.h"            // Utilities
#include "CSatTsyReqHandleStore.h"  // Request handle class
#include "cmmmessagemanagerbase.h"  // Message manager class for forwarding req.
//#include "CMmCustomTsy.h"           // Custom Tsy class
#include "MSatTsy_IPCDefs.h"        // Sat Tsy internal request types


// -----------------------------------------------------------------------------
// CSatNotifyRefresh::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//  
CSatNotifyRefresh* CSatNotifyRefresh::NewL
        ( 
        CSatNotificationsTsy* aNotificationsTsy 
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::NewL");
   	CSatNotifyRefresh* const satNotifyRefresh = 
        new ( ELeave ) CSatNotifyRefresh( aNotificationsTsy );
    CleanupStack::PushL( satNotifyRefresh );
    satNotifyRefresh->ConstructL();
    CleanupStack::Pop( satNotifyRefresh );
    TFLOGSTRING("CSAT: CSatNotifyRefresh::NewL, end of method");
    return satNotifyRefresh;
    }

// -----------------------------------------------------------------------------
// CSatNotifyRefresh::~CSatNotifyRefresh
// Destructor
// -----------------------------------------------------------------------------
//  
CSatNotifyRefresh::~CSatNotifyRefresh
        ( 
		// None
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::~CSatNotifyRefresh");
    }
    
// -----------------------------------------------------------------------------
// CSatNotifyRefresh::CSatNotifyRefresh
// Default C++ constructor
// -----------------------------------------------------------------------------
//  
CSatNotifyRefresh::CSatNotifyRefresh
        ( 
        CSatNotificationsTsy* aNotificationsTsy 
        ) : iNotificationsTsy ( aNotificationsTsy )
    {
    // None
    }

// -----------------------------------------------------------------------------
// CSatNotifyRefresh::ConstructL
// Symbian 2nd phase constructor
// -----------------------------------------------------------------------------
//  
void CSatNotifyRefresh::ConstructL
        (
        // None
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::ConstructL");
    }

// -----------------------------------------------------------------------------
// CSatNotifyRefresh::Notify
// This request allows a client to be notified of a REFRESH proactive command
// -----------------------------------------------------------------------------
//
TInt CSatNotifyRefresh::Notify
        (
        const TTsyReqHandle aTsyReqHandle,
        const TDataPackage& aPackage  
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::Notify");

    // Save data pointer to client side for completion
    iRefreshV2Pckg = reinterpret_cast<RSat::TRefreshV2Pckg*>( 
        aPackage.Des1n() );
    
    // Save the request handle
    iNotificationsTsy->iSatTsy->SaveReqHandle( aTsyReqHandle, 
		CSatTsy::ESatNotifyRefreshPCmdReqType );

    if ( iNotificationsTsy->iSatReqHandleStore->TsyReqHandle(
		CSatTsy::ESatNotifyRefreshRequiredPCmdReqType ) )
    	{
    	// Check the command buffer, if both notifications are received
    	iNotificationsTsy->NotifySatReadyForNotification( KRefresh );   
    	}
    return KErrNone;   
    }

// -----------------------------------------------------------------------------
// CSatNotifyRefresh::Notify
// This request allows a client to be notified of a REFRESH proactive 
// command
// -----------------------------------------------------------------------------
//
TInt CSatNotifyRefresh::NotifyRefreshRequired
        (
        const TTsyReqHandle aTsyReqHandle,
        const TDataPackage& aPackage 
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::NotifyRefreshRequired");

    // Save data pointer to client side for completion
    iRefreshRequiredV2Pckg = reinterpret_cast<RSat::TRefreshV2Pckg*>( 
        aPackage.Des1n() );
    
    // Save the request handle
    iNotificationsTsy->iSatTsy->SaveReqHandle( aTsyReqHandle, 
		CSatTsy::ESatNotifyRefreshRequiredPCmdReqType );
	
	if ( iNotificationsTsy->iSatReqHandleStore->TsyReqHandle(
		CSatTsy::ESatNotifyRefreshPCmdReqType ) )
		{
    	// Check the command buffer, if both notifications are received
    	iNotificationsTsy->NotifySatReadyForNotification( KRefresh );   
		}
	return KErrNone;  
    }
    
// -----------------------------------------------------------------------------
// CSatNotifyRefresh::CancelNotification
// This method cancels an outstanding asynchronous 
// NotifyRefresh request.
// -----------------------------------------------------------------------------
//
TInt CSatNotifyRefresh::CancelNotification
        (
        const TTsyReqHandle aTsyReqHandle
        )
    {
    TFLOGSTRING( "CSAT: CSatNotifyRefresh::CancelNotification" );
    
    // Reset the request handle
    iNotificationsTsy->iSatReqHandleStore->ResetTsyReqHandle( 
        CSatTsy::ESatNotifyRefreshPCmdReqType );

    // Reset the data pointer
    iRefreshV2Pckg = NULL;

    // Complete the request with KErrCancel
    iNotificationsTsy->iSatTsy->ReqCompleted( aTsyReqHandle, KErrCancel );

    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CSatNotifyRefresh::CancelRefreshRequiredNotification
// Cancels the current notification request set the request handle
// -----------------------------------------------------------------------------
//    
TInt CSatNotifyRefresh::CancelRefreshRequiredNotification
		( 
    	const TTsyReqHandle aTsyReqHandle
     	)
	{
    TFLOGSTRING( "CSAT: CSatNotifyRefresh::CancelRefreshRequiredNotification" );
        
    // Reset the request handle
    iNotificationsTsy->iSatReqHandleStore->ResetTsyReqHandle( 
        CSatTsy::ESatNotifyRefreshRequiredPCmdReqType );

    // Reset the data pointer
    iRefreshRequiredV2Pckg = NULL;

    // Complete the request with KErrCancel
    iNotificationsTsy->iSatTsy->ReqCompleted( aTsyReqHandle, KErrCancel );

    return KErrNone; 
    }

// -----------------------------------------------------------------------------
// CSatNotifyRefresh::CompleteNotifyL
// This method completes an outstanding asynchronous 
// NotifyRefresh request. 
// -----------------------------------------------------------------------------
//
TInt CSatNotifyRefresh::CompleteNotifyL
        (
        CSatDataPackage* aDataPackage,   
        TInt aErrorCode                 
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL");

    TInt ret( KErrNone );
	TBuf<1> additionalInfo;	
    // Unpack parameters
    TPtrC8* data;
    aDataPackage->UnPackData( &data );
    TFLOGSTRING2("CSAT: CSatNotifyRefresh::CompleteNotifyL, dataLength: %d", 
        data->Length());
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iNotificationsTsy->iSatReqHandleStore->
        TsyReqHandle( CSatTsy::ESatNotifyRefreshPCmdReqType );
    TTsyReqHandle reqHandleRefreshRequired = iNotificationsTsy->
    	iSatReqHandleStore->ResetTsyReqHandle( 
    		CSatTsy::ESatNotifyRefreshRequiredPCmdReqType );
		
    // Get ber tlv 
    CBerTlv berTlv;
    berTlv.SetData( *data );
    // Get command details tlv
    CTlv commandDetails;
    berTlv.TlvByTagValue( &commandDetails, KTlvCommandDetailsTag );
    // Store command details tlv
    iNotificationsTsy->iTerminalRespData.iCommandDetails.Copy( 
        commandDetails.Data() );
    // Get command qualifier
    TUint8 cmdQualifier( commandDetails.GetShortInfo( 
        ETLV_CommandQualifier ) );
        
    TUint8 pCmdNumber( commandDetails.GetShortInfo( ETLV_CommandNumber ) );    
    
    // In case the request was ongoing, continue..
    if ( ( CSatTsy::ESatReqHandleUnknown != reqHandle ) &&
    	 ( CSatTsy::ESatReqHandleUnknown != reqHandleRefreshRequired ) )
        {
        // Complete right away if error has occured, otherwise continue..
        if ( KErrNone == aErrorCode )
            {
	        // Fill the refresh structure
	        RSat::TRefreshV2& refreshV2 = ( *iRefreshV2Pckg )();

	        TInt returnValue( KErrNone );
	       
	        refreshV2.iAid.Zero();
	        
	         // File list
	        refreshV2.iFileList.Zero();
	        iFileList.Zero();
	        iInternalCache = 0;
	        
	        // Used in case of SimInit refresh + Imsi change 
            iImsiState = ENotSet;

	        CTlv fileListTlv;
	        returnValue = berTlv.TlvByTagValue( &fileListTlv, 
	                                            KTlvFileListTag );
	        if ( KErrNone == returnValue )
	            {
	            // Filelist found
	            // File list length, -1 for number of files 
	            TUint8 fileLength( (TUint8)( fileListTlv.GetLength() - 1 ) );
	           
	            // Save file list with full path.
	            iFileList = fileListTlv.GetData( ETLV_Files );

	            // Going through all files
	            // Received as 8-bit, append to 16-bit
	            TInt i( 0 );

	            for ( i = 0; i < fileLength / 2; i++ )
	                {                           
	                TUint16 oneItem( 0 );
	                // Read 16-bit
	                TSatUtility::CopyTwo8toOne16LE( iFileList, oneItem, 2 * i );
	                // File header 8-bit
	                TUint8 header( TUint8( oneItem >> 8 ) );

	                // The TSY needs to strip the Refresh File List 
	                // provided by the ICC to remove the paths to the files.
	                // '3FXX': Master File;
	                // '7FXX': 1st level Dedicated File;
	                // '5FXX': 2nd level Dedicated File;
	                if ( ( KRefreshMasterFileHeader != header )
	                    && ( KRefresh1StLevelDedicatedFileHeader != header ) 
	                    && ( KRefresh2StLevelDedicatedFileHeader != header ) )
	                    {
	                    // Append file to the file list, without path
	                    refreshV2.iFileList.Append( oneItem );
	                    }
	                }
	            // Check if the file list contains the EF-SST file which is cached 
	            // in COMMON SIM ATK TSY
	            if ( KErrNotFound != refreshV2.iFileList.Locate( RSat::KSstEf ) )   
	                {
	                iInternalCache |= CSatNotificationsTsy::ECacheEFSST;
	                }
	            // Check EF-CBMID
	            if ( KErrNotFound != refreshV2.iFileList.Locate( RSat::KCbmidEf ) )   
	                {
	                iInternalCache |= CSatNotificationsTsy::ECacheEFCBMID;
	                }
	            }
	        else if ( KFileChangeNotification == iNotificationsTsy->
	        	iTerminalRespData.iCommandDetails[KCommandQualifier] )
	            {
	            TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL,\
	                File list missing");
	            // File list object is missing, return terminal resp immediately.
	            ret = KErrCorrupt;
	            additionalInfo.Zero();
	            additionalInfo.Append( RSat::KNoSpecificMeProblem ); 
	            CreateTerminalRespL( pCmdNumber, 
	                RSat::KErrorRequiredValuesMissing, additionalInfo );                                                                
	            }
	        if ( KErrNone == ret )
	            {
	            // Set refresh mode
	            switch ( iNotificationsTsy->iTerminalRespData.
	            		iCommandDetails[KCommandQualifier] )
	                {
	                case KSimInitFullFileChangeNotification:
	                    {
	                    TFLOGSTRING("CSAT: CSatNotifyRefresh::\
	                    	CompleteNotifyL, Refresh mode: Sim init \
	                    	and full file change notification");
	                    refreshV2.iType = 
	                        RSat::ESimInitFullFileChangeNotification;
	                    iInternalCache = CSatNotificationsTsy::ECacheEFSST + 
	                    	CSatNotificationsTsy::ECacheEFCBMID;
	                    break;
	                    }
	                case KFileChangeNotification:
	                    {
	                    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL\
	                    	Refresh mode: File Change Notification");
	                    TFLOGSTRING2("CSAT: CSatNotifyRefresh::CompleteNotifyL\
	                    	Number of files: %d", 
	                    	refreshV2.iFileList.Length() );
	                    refreshV2.iType = RSat::EFileChangeNotification;
	                    
	                    if ( ( KErrNotFound != refreshV2.iFileList.Locate( 
	                         RSat::KImsiEf ) ) && 
	                         ( !fileListTlv.GetComprehensionRequired() ) )
	                        {
	                        // IMSI was part of file list, CR bit was cleared
	                        // This is done here because client does not have 
	                        // the CR bit information available,
	                        // Other error values comes from Client
	                        ret = KErrCorrupt;
	                        additionalInfo.Zero();
	                        additionalInfo.Append( 
	                            RSat::KNoSpecificMeProblem );
	                        CreateTerminalRespL( pCmdNumber, 
	                            RSat::KErrorRequiredValuesMissing,
	                            additionalInfo ); 
	                        }
	                    break;
	                    }
	                case KSimInitFileChangeNotification:
	                    {
	                    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL,\
	                    	Refresh mode: Sim init and file \
	                    	change notification");
	                    TFLOGSTRING2("CSAT: CSatNotifyRefresh::CompleteNotifyL\
	                    	Number of files: %d", 
	                    	refreshV2.iFileList.Length() );
	                    refreshV2.iType = RSat::ESimInitFileChangeNotification;
	                    break;
	                    }
	                case KSimInit:
	                    {
	                    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL\
	                    	Refresh mode: Sim init ");
	                    refreshV2.iType = RSat::ESimInit;
	                    iInternalCache = CSatNotificationsTsy::ECacheEFSST 
	                    	+ CSatNotificationsTsy::ECacheEFCBMID;
	                    // Used in case of SimInit refresh + Imsi change
                        iImsiState = ERefreshSimInitReceived;
	                    break;
	                    }
	                case KSimReset:
	                    {
	                    refreshV2.iType = RSat::ESimReset;
	                    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL\
	                    	Refresh mode: Reset");
	                    iInternalCache = CSatNotificationsTsy::ECacheEFSST + 
	                    	CSatNotificationsTsy::ECacheEFCBMID;
	                    break;
	                    }
	                case KUSIMApplicationReset:
	                    {
	                    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL\
	                    	Refresh mode: USIM Application Reset");
	                    refreshV2.iType = RSat::EUsimApplicationReset;
	                    iInternalCache = CSatNotificationsTsy::ECacheEFSST + 
	                    	CSatNotificationsTsy::ECacheEFCBMID;
	                    break;
	                    }
	                case K3GSessionReset:
	                    {
	                    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL\
	                    	Refresh mode: 3G Session Reset");
	                    refreshV2.iType = RSat::E3GSessionReset;
	                    break;
	                    }
	                default:
	                    {
	                    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL\
	                    	Refresh type not set!");
	                    // This migth be an error case
	                    refreshV2.iType = RSat::ERefreshTypeNotSet;
	                    break;
	                    }
	                }
	            
	            // Application identifies(optional)
	            // If an AID TLV is present, it indicates the USIM application which
	            // needs to be refreshed. If it is not present, the ME shall assume 
	            // the current USIM application needs to be refreshed.
	            CTlv applicationId;     
	            returnValue = berTlv.TlvByTagValue( &applicationId, KTlvAIDTag ); 
	            iAid.Zero();
	            if ( KErrNone == returnValue )
	                {
	                // Set application identifies
	                refreshV2.iAid = applicationId.GetData( ETLV_AID );
	                // Store aplication Id to member variable.
	                iAid = refreshV2.iAid;
	                }
	            }
	        // Copy parameters
	        (*iRefreshRequiredV2Pckg)() = (*iRefreshV2Pckg)();
	        
	        }
        else
        	{
        	ret = aErrorCode;
        	}
	        
        iNotificationsTsy->iSatTsy->ReqCompleted( reqHandleRefreshRequired, ret );
        }
    else
    	{        
    	TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteNotifyL,\
    	    One or more of the requests not ongoing");
        // Some of the request were not on, returning response immediately
        additionalInfo.Zero();
        additionalInfo.Append( RSat::KNoSpecificMeProblem );
        CreateTerminalRespL( pCmdNumber, RSat::KMeUnableToProcessCmd,
            additionalInfo );
        	
        ret = KErrCorrupt;    
    	}    
    return ret;
    }


// -----------------------------------------------------------------------------
// CSatNotifyRefresh::TerminalResponseL
// Response for Refresh query
// Called by ETel server, passes terminal response to DOS
// -----------------------------------------------------------------------------
//
TInt CSatNotifyRefresh::TerminalResponseL
        ( 
        TDes8* aRsp
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::TerminalResponseL");

    RSat::TRefreshRspV1Pckg* aRspPckg = 
            reinterpret_cast<RSat::TRefreshRspV1Pckg*>( aRsp );
    RSat::TRefreshRspV1& rspV1 = ( *aRspPckg ) ();
    TBuf<1> additionalInfo;
    additionalInfo.Append( RSat::KNoSpecificMeProblem );
    // Get Proactive command number
    TUint8 pCmdNumber( rspV1.PCmdNumber() );
    
    if ( RSat::KNoAdditionalInfo != rspV1.iInfoType )
        {
        additionalInfo.Zero();
        additionalInfo.Append( rspV1.iAdditionalInfo[0] );
        }
        
    // At this point the result should be always Success 
    if ( ( ( KSimReset != iNotificationsTsy->iTerminalRespData.
    		iCommandDetails[KCommandQualifier] ) || 
    		( RSat::KSuccess != rspV1.iGeneralResult ) ) && 
    		( EImsiNotEqual != iImsiState ) )
        {
        TFLOGSTRING("CSAT: CSatNotifyRefresh::TerminalResponseL, \
        	S60 has done the Refresh" );
        // Send terminal response
        CreateTerminalRespL( pCmdNumber, (TUint8) rspV1.iGeneralResult, 
        	additionalInfo );
        // Case was not SimInit refresh + Imsi change, set back to default 
        iImsiState = ENotSet;
        }
    else
        {
        // Set flag off indicating that proactive command is not ongoing
        // Normaly this is cleared after a terminal response.
    	iNotificationsTsy->GetSatTimer()->SetProactiveCommandOnGoingStatus( 
    	    EFalse );   
    	// Case was not SimInit refresh + Imsi change, set back to default 
        iImsiState = ENotSet;
        }
        
    return KErrNone;
    }   
    
// -----------------------------------------------------------------------------
// CSatNotifyRefresh::CompleteImsiChange
// Compeletes refresh request
// -----------------------------------------------------------------------------
//
void CSatNotifyRefresh::CompleteImsiChange 
		(
        // None
    	)
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteImsiChange");
    if ( ERefreshSimInitReceived == iImsiState )
        {
        TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteImsiChange, \
            IMSI not equal");
        iImsiState = EImsiNotEqual;
        }        
    }    
   
// -----------------------------------------------------------------------------
// CSatNotifyRefresh::CreateTerminalRespL
// Constructs Refresh specific part of terminal response and calls 
// DOS to send the actual message.
// -----------------------------------------------------------------------------
//
TInt CSatNotifyRefresh::CreateTerminalRespL
        ( 
        TUint8 aPCmdNumber,         
        TUint8 aGeneralResult,      
        TDesC16& aAdditionalInfo		   
		)
    {
	TFLOGSTRING("CSAT: CSatNotifyRefresh::CreateTerminalRespL");
    // Create and append response data
    TTlv tlvSpecificData;
    // append result tag
    tlvSpecificData.AddTag( KTlvResultTag );

    // General result
    tlvSpecificData.AddByte( aGeneralResult );
    
    if ( RSat::KMeUnableToProcessCmd == aGeneralResult ) 
        {
        if ( aAdditionalInfo.Length() )
        	{
        	tlvSpecificData.AddByte( ( TUint8 ) aAdditionalInfo[0] );
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

// -----------------------------------------------------------------------------
// CSatNotifyRefresh::RefreshAllowedL
// Response to RefreshRequired notify. Tells if the refresh is 
// allowed by the Client.
// -----------------------------------------------------------------------------
//
TInt CSatNotifyRefresh::RefreshAllowedL
		(
		const TDataPackage& aPackage
    	)
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::RefreshAllowed ");       
    RSat::TRefreshRspV1Pckg* aRspPckg = 
            reinterpret_cast<RSat::TRefreshRspV1Pckg*>( aPackage.Des1n() );
    RSat::TRefreshRspV1& rspV1 = ( *aRspPckg ) ();
    TUint8 pCmdNumber( rspV1.PCmdNumber() );
    CSatNotificationsTsy::TRefreshInfo refreshInfo;
	
    if ( ( RSat::KSuccess == rspV1.iGeneralResult ) || 
         ( RSat::KRefreshAdditionEFRead == rspV1.iGeneralResult ) )
        {
        // Refresh allowed
		refreshInfo.iAid.Copy( iAid );
		refreshInfo.iFileList.Copy( iFileList );
		refreshInfo.iInternalCache = iInternalCache;
		// Pack data
    	CSatDataPackage dataPackage;
		dataPackage.PackData( &iNotificationsTsy->iTerminalRespData, &refreshInfo );	
        // Send refresh request
        iNotificationsTsy->iSatTsy->MessageManager()->HandleRequestL( 
			ESatTsyServiceRequest, &dataPackage );			                              
        }
    else
        {
        // Refresh not allowed by the client
        TFLOGSTRING("CSAT: CSatNotifyRefresh::RefreshAllowed, refresh was not \
            allowed by the client");
		TBuf<RSat::KAdditionalInfoMaxSize> additionalInfo;
		additionalInfo.Append(RSat::KNoSpecificMeProblem);
        if ( RSat::KNoAdditionalInfo != rspV1.iInfoType )
            {
            additionalInfo.Zero();
            additionalInfo.Append( rspV1.iAdditionalInfo[0] );
            }
        // Send terminal response
        CreateTerminalRespL( pCmdNumber, (TUint8) rspV1.iGeneralResult, 
            additionalInfo );
        }
        
    return KErrNone;
    }
        
// -----------------------------------------------------------------------------
// CSatNotifyRefresh::CompleteTerminalRespDataL
// Unpacks Refresh specific part of terminal response and calls 
// CreateTerminalRespL to send the actual message.
// -----------------------------------------------------------------------------
//
TInt CSatNotifyRefresh::CompleteTerminalRespDataL
        (
        CSatDataPackage* aDataPackage,   
        TInt /*aErrorCode*/              
        )
    {
    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteTerminalRespDataL");
    TInt ret( KErrNone );
    CSatNotificationsTsy::TTerminalRespData* terminalRespData;
    // Send terminal response
    aDataPackage->UnPackData( &terminalRespData );
    CreateTerminalRespL( iNotificationsTsy->iTerminalRespData.iPCmdNumber,
                         terminalRespData->iGeneralResult,
                         terminalRespData->iAdditionalInfo );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CSatNotifyRefresh::CompleteRequest
// Compeletes refresh request
// -----------------------------------------------------------------------------
//
#ifdef USING_CTSY_DISPATCHER
void CSatNotifyRefresh::CompleteRequest
		(
        CSatDataPackage* /*aDataPackage*/,  
        TInt aErrorCode             
    	)
    {
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iNotificationsTsy->iSatReqHandleStore->
        ResetTsyReqHandle( CSatTsy::ESatNotifyRefreshPCmdReqType );
		
    TFLOGSTRING3("CSAT: CSatNotifyRefresh::CompleteRequest.\
    	 \n\t\t\t Handle:%d\n\t\t\t Error:%d", reqHandle, aErrorCode);
    	 		
    iNotificationsTsy->iSatTsy->ReqCompleted( reqHandle, aErrorCode);

    TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteRequest. Request is \
    	now completed");    
    }     
#else //USING_CTSY_DISPATCHER
void CSatNotifyRefresh::CompleteRequest
		(
        CSatDataPackage* aDataPackage,  
        TInt /*aErrorCode*/              
    	)
    {
    TInt retValue;
    aDataPackage->UnPackData( retValue );
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iNotificationsTsy->iSatReqHandleStore->
        ResetTsyReqHandle( CSatTsy::ESatNotifyRefreshPCmdReqType );

    TFLOGSTRING3("CSAT: CSatNotifyRefresh::CompleteRequest.\
    	 \n\t\t\t Handle:%d\n\t\t\t Error:%d", reqHandle, retValue);
    	 		   
    if(CSatTsy::ESatReqHandleUnknown != reqHandle) 
    	{
        iNotificationsTsy->iSatTsy->ReqCompleted( reqHandle, retValue );

        TFLOGSTRING("CSAT: CSatNotifyRefresh::CompleteRequest. Request is \
        	now completed");
    	} 
    
    }
#endif //USING_CTSY_DISPATCHER

// End of file

