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
// Name        : CSatIconTsy.cpp
// Part of     : Common SIM ATK TSY / commonsimatktsy
// Icon related functionality of Sat Tsy
// Version     : 1.0
// INCLUDES
//



#include <satcs.h>					// ETel Ipcs
#include "cmmmessagemanagerbase.h"  // Message manager class for forwarding req.
//#include <TSatIconInfo.h>           // For icon color coding
#include "TfLogger.h"              // For TFLOGSTRING

#include "CSatDataPackage.h"        // Parameter packing 
#include "CSatIconTsy.h"            // Class header
#include "CSatTsyReqHandleStore.h"
#include "msattsy_ipcdefs.h"		// Sat Tsy specific request types

#include <etelsat.h>


// -----------------------------------------------------------------------------
// CSatIconTsy::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatIconTsy* CSatIconTsy::NewL
        (    
        CSatTsy* aSatTsy 
        )
    {
    TFLOGSTRING("CSAT: CSatIconTsy::NewL");
    CSatIconTsy* const satIconTsy = new ( ELeave ) CSatIconTsy();
    CleanupStack::PushL( satIconTsy );
    satIconTsy->iSatTsy = aSatTsy;
    satIconTsy->ConstructL();
    CleanupStack::Pop( satIconTsy );
    TFLOGSTRING("CSAT: CSatIconTsy::NewL, end of method");
    return satIconTsy;
    }


// -----------------------------------------------------------------------------
// CSatIconTsy::~CSatIconTsy
// Destructor
// -----------------------------------------------------------------------------
//    
CSatIconTsy::~CSatIconTsy
        (    
        void   
        )
    {
    TFLOGSTRING("CSAT: CSatIconTsy::~CSatIconTsy");
    // Unregister.
    iSatTsy->MessageManager()->RegisterTsyObject(
		CMmMessageManagerBase::ESatIconTsyObjType, NULL );
    }
   
// -----------------------------------------------------------------------------
// CSatIconTsy::ConstructL
// Symbian 2nd phase constructor. Initialises internal attributes.
// -----------------------------------------------------------------------------
//
void CSatIconTsy::ConstructL
        (    
        void
        )
    {
    TFLOGSTRING("CSAT: CSatIconTsy::ConstructL\n" );
    iIconReqTypeOngoing = ERequestTypeUnknown;
    // Register.
    iSatTsy->MessageManager()->RegisterTsyObject(
		CMmMessageManagerBase::ESatIconTsyObjType, this );
		
	iIconData = NULL;
    iRecordData.Zero();
    iImageInstanceNoInRecord = 0;		
    }
     
// -----------------------------------------------------------------------------
// CSatIconTsy::CSatIconTsy
// C++ constructor
// -----------------------------------------------------------------------------
//
CSatIconTsy::CSatIconTsy
        (    
        void
        )
    {
    // None
    }

// -----------------------------------------------------------------------------
// CSatIcon::DoExtFunc
// Icon specific functionality of CSatTsy::DoExtFuncL
// -----------------------------------------------------------------------------
//
TInt CSatIconTsy::DoExtFuncL
        ( 
        const TTsyReqHandle aTsyReqHandle, // Request handle
        const TInt aIpc,                   // IPC number of request
        const TDataPackage& aPackage       // Contains parameters for request
        )
    {

    TFLOGSTRING3("CSAT: CSatIconTsy::DoExtFuncL.\n  \t\t\t IPC:%d\n  \
        \t\t\t Handle:%d", aIpc, aTsyReqHandle);

	TInt ret( KErrNone );

	TAny* dataPtr = aPackage.Ptr1();
	TAny* dataPtr2 = aPackage.Ptr2();

    switch ( aIpc )
        {
        case ESatGetIcon:
            {
            ret = GetIconL( aTsyReqHandle,
                reinterpret_cast<TUint8*>( dataPtr ), 
                reinterpret_cast<RSat::TIcon*>( dataPtr2 ) );
            break;
            }
        case ESatGetImageInstance:
            {
			ret = GetImageInstanceL( aTsyReqHandle,
                aPackage.Des1n(), aPackage.Des2n() );
            break;
            }
        case ESatGetClut:
            {
            ret = GetClutL( aTsyReqHandle,
                reinterpret_cast<RSat::TInstanceNumberAndOffset*>( dataPtr ), 
                aPackage.Des2n() );
            break;
            }
        default:
            {
            TFLOGSTRING2("CSAT: CSatIconTsy::DoExtFuncL unsupported IPC %d",
                 aIpc);
            break;
            }
        }

    return ret;

    }

// -----------------------------------------------------------------------------
// CSatIconTsy::CompleteGetIconData
// Completes GetIcon request
// -----------------------------------------------------------------------------
//
void CSatIconTsy::CompleteGetIcon
        (
        CSatDataPackage* aDataPackage,  
		TInt aResult                   
        )
    {
    TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetIcon");
    TPtrC8* icon = NULL;
    TInt iconLength = 0; 

	// Unpack parameters 
    aDataPackage->UnPackData( &icon );
    iconLength = icon->Length();

    // Reset req handle. Returns the deleted req handle.
    TTsyReqHandle reqHandle = iSatTsy->GetSatReqHandleStore()->
		ResetTsyReqHandle( CSatTsy::ESatGetIconReqType );

    // In case the request was ongoing, continue..
    if ( CSatTsy::ESatReqHandleUnknown != reqHandle )
        {
        if ( KErrNone == aResult && iconLength )
            {
            TFLOGSTRING2("CSAT: CSatIconTsy::CompleteGetIcon,\
                Icon data length: %d", iconLength);
            // Check the size...
            if ( iIconData->MaxLength() >= iconLength )
                {      
                iIconData->Copy( icon->Ptr(), iconLength );
                TFLOGSTRING2("CSAT: CSatIconTsy::CompleteGetIcon,\
                    iIconData length: %d", iIconData->Length() );             
                iRecordData.Copy( *iIconData );  
                }
            else  // Data is too big for client's buffer
                {
                TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetIcon,\
                    Icon data length exceeded");
                aResult = KErrOverflow;
                iIconData->Zero();
                }
            }
        else
            {
            TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetIcon,\
                Error occured in LSAT, or Icon length is 0");
            // Reset the length
            iIconData->Zero();
            }

        iIconData = NULL;       
        iSatTsy->ReqCompleted( reqHandle, aResult );
        }
    }

// -----------------------------------------------------------------------------
// CSatIconTsy::CompleteGetImageInstance
// Completes GetImageInstance request
// -----------------------------------------------------------------------------
//
void CSatIconTsy::CompleteGetImageInstance
        (
        CSatDataPackage* aDataPackage, 
		TInt aResult                   
        )
    {
    TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetImageInstance");
    TPtrC8* icon = NULL;
    TDes8* info;

	// Unpack parameters 
    aDataPackage->UnPackData( &icon, &info );
    // Get icon data length
    TUint8 iconLength = icon->Length();

    // Reset req handle. Returns the deleted req handle.
    TTsyReqHandle reqHandle = iSatTsy->GetSatReqHandleStore()->
		ResetTsyReqHandle( CSatTsy::ESatGetImageInstanceReqType );

	// In case the request was ongoing, continue..
    if ( CSatTsy::ESatReqHandleUnknown != reqHandle )
        {
        if ( KErrNone == aResult && iconLength > 0 )
            {
            // Check the size....
            if ( iIconData->MaxLength() >= iconLength )
                {
                TFLOGSTRING2("CSAT: CSatIconTsy::CompleteGetImageInstance,\
                    Icon length: %d", iconLength);
                // Icon body
                if( info->Length() == KIconInfoLength )
                    {
                    TInt colorCoding = iRecordData[ KImageCodingSchemeByteNr + 
                        ( iImageInstanceNoInRecord - 1 ) * 
                        KImageInstanceFileLength ];
                    // There is difference in the header length
                    // between the color and b&w icons
                    if ( RSat::KBasic == colorCoding )
                        {
                        TFLOGSTRING("CSAT: CSatIconTsy::\
                            CompleteGetImageInstance,B&W");
                        iIconData->Copy( ( *info ).Mid( 0, 2) );
                        }
                    else
                        {
                        iIconData->Copy( *info );
                        }
                        
                    iIconData->Append( *icon );
                    }
                // Clut and record
                else
                    {
                	iIconData->Copy( *icon );
                    }               
                }
            else // Data is too big for client's buffer
                {
                TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetImageInstance,\
                    Icon length exceeded");
                aResult = KErrOverflow;
                iIconData->Zero();
                }
            }
        else
            {
            TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetImageInstance,\
                Error occured in LSAT or Icon length is 0");
            // Reset the length
            iIconData->Zero();
            }

        iIconData = NULL;
        iIconReqTypeOngoing = ERequestTypeUnknown;

        iSatTsy->ReqCompleted( reqHandle, aResult );
	    }
    }

// -----------------------------------------------------------------------------
// CSatIconTsy::CompleteGetClut
// Completes GetClut request
// -----------------------------------------------------------------------------
//
void CSatIconTsy::CompleteGetClut
        (
        CSatDataPackage* aDataPackage, 
		TInt aResult                   
        )
    {
    TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetClut");
    TPtrC8* clut = NULL;

	// Unpack parameters 
    aDataPackage->UnPackData( &clut );

    // Reset req handle. Returns the deleted req handle.
    TTsyReqHandle reqHandle = iSatTsy->GetSatReqHandleStore()->
		ResetTsyReqHandle( CSatTsy::ESatGetClutReqType );

	// In case the request was ongoing, continue..
    if ( CSatTsy::ESatReqHandleUnknown != reqHandle )
        {
        if ( KErrNone == aResult && clut->Length() > 0)
            {
            // Check the size....
            if ( iIconData->MaxLength() >= clut->Length() )
                {
                iIconData->Copy( *clut );                
                }
            else // Data is too big for client's buffer
                {
                TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetClut,\
                    Clut length exceeded");
                aResult = KErrOverflow;
                iIconData->Zero();
                }
            }
        else
            {
            TFLOGSTRING("CSAT: CSatIconTsy::CompleteGetClut,\
                Error occured in LSAT or Icon length is 0");
            // Reset the length
            iIconData->Zero();
            }

        iIconData = NULL;
        iIconReqTypeOngoing = ERequestTypeUnknown;

        iSatTsy->ReqCompleted( reqHandle, aResult );
	    }
    }

// -----------------------------------------------------------------------------
// CSatIconTsy::GetIconL
// This method gets the content of the EF_IMG record specified 
// by aRecordnumber
// -----------------------------------------------------------------------------
//
TInt CSatIconTsy::GetIconL
        (
        const TTsyReqHandle aTsyReqHandle, 
        TUint8* aRecordNumber,             
        RSat::TIcon* aIconEf              
        )   
    {
    TFLOGSTRING("CSAT: CSatIconTsy::GetIconL");
	TInt ret = KErrNone;
	
	iImageInstanceNoInRecord = 0;
	
	// Get possibly existing request handle
    TTsyReqHandle reqHandle = iSatTsy->GetSatReqHandleStore()->
		TsyReqHandle( CSatTsy::ESatGetIconReqType );

    // In case the request was ongoing, complete right away..
    if ( CSatTsy::ESatReqHandleUnknown != reqHandle )
    	{
        TFLOGSTRING("CSAT: CSatIcon::GetIconL, KErrServerBusy");
    	iSatTsy->ReqCompleted( aTsyReqHandle, KErrServerBusy);
        }
    else
        {
	    // Save the request handle
	    iSatTsy->SaveReqHandle( aTsyReqHandle, CSatTsy::ESatGetIconReqType );

	    iIconData = aIconEf;
	    iIconReqTypeOngoing = EGetIconReq;
        iRecordNumber = *aRecordNumber;

	    // Length is zero -> NAA will return all the contents of a given file
	    TUint16 length( 0 ); 

	    // If the Elementary File pointed by File ID is linear fixed or cyclic,
	    // then the least significant OffSet byte denotes the record number, 
	    // while the most significant OffSet byte denotes the start byte within
	    // that record. 
	    TUint8 highByte = *aRecordNumber;
	    TUint8 lowByte = 0x00; // Start byte
	    TUint16 offset( static_cast<TUint16>( ( highByte << 8 ) + lowByte ) );
	    
	    // File ID
	    TBuf8<KFilePathLength> simFilePath;

	    // The 16bit values are appended in little endian
	    // this is the correct way to map them into 8bit values
		simFilePath.Append( TUint8 ( KMasterFile >> 8) );
	    // lint -e{778} Constant expression evaluates to 0 in operation  
	    simFilePath.Append( TUint8 ( KMasterFile & 0xff ) ); 
	    simFilePath.Append( TUint8 ( KDedicatedFile >> 8) ); 
	    simFilePath.Append( TUint8 ( KDedicatedFile & 0xff ) ); 
	    simFilePath.Append( TUint8 ( KDedicatedFileGraphics >> 8) ); 
	    simFilePath.Append( TUint8 ( KDedicatedFileGraphics & 0xff ) ); 
	    simFilePath.Append( TUint8 ( KElementaryFileImg >> 8) ); 
	    simFilePath.Append( TUint8 ( KElementaryFileImg & 0xff ) ); 

		TIconInfo iconInfoPkg;
	    iconInfoPkg.iOffset = offset;
	    iconInfoPkg.iLength = length;
	    iconInfoPkg.iSimFilePath.Copy( simFilePath );

		// Pack data
	    CSatDataPackage dataPackage;
		dataPackage.PackData( &iconInfoPkg );
	    ret = iSatTsy->MessageManager()->HandleRequestL( 
	        ESatTsyGetIconData, &dataPackage );
        }

    return ret;
    }


// -----------------------------------------------------------------------------
// CSatIconTsy::GetImageInstanceL
// This method gets the image instance specified by the 
// InstanceNumber, Offset and Length
// -----------------------------------------------------------------------------
//
TInt CSatIconTsy::GetImageInstanceL
        ( 
        const TTsyReqHandle aTsyReqHandle, 
        TDesC8* aInstanceInfo,            
        TDes8* aInstance                 
        )
    {
    TFLOGSTRING("CSAT: CSatIconTsy::GetImageInstanceL");
    TInt ret = KErrNone;
	
    // Get possibly existing request handle
    TTsyReqHandle reqHandle = iSatTsy->GetSatReqHandleStore()->
		TsyReqHandle( CSatTsy::ESatGetImageInstanceReqType );

    // In case the request was ongoing, complete right away..
    if ( CSatTsy::ESatReqHandleUnknown != reqHandle )
    	{
        TFLOGSTRING("CSAT: CSatIcon::GetImageInstanceL, KErrServerBusy");
    	iSatTsy->ReqCompleted( aTsyReqHandle, KErrServerBusy);
        }
    else
        {
        TInt iconNumber = iRecordData[0];
        TBool iconFound( EFalse );
        TInt pos( KImageInstanceIdOffset );
        
	    // Save the request handle
	    iSatTsy->SaveReqHandle( aTsyReqHandle, 
	    	CSatTsy::ESatGetImageInstanceReqType );

	    iIconData = aInstance;
	    iIconReqTypeOngoing = EGetImageInstanceReq;

	    RSat::TInstanceInfoV2Pckg* aPckg = REINTERPRET_CAST( 
	        RSat::TInstanceInfoV2Pckg*, aInstanceInfo );
	    RSat::TInstanceInfoV2& instanceInfo = ( *aPckg ) ();
        
        // Calculate position for icon imageinstance selected from record.
        while( iconNumber && !iconFound )
            {
            // If the instance number, offset and length of the previously
            // received record data match with the one received from the 
            // client, it means that the icon is found and request can be made
            if( ( static_cast<TUint16>( instanceInfo.iInstanceNumber ) == 
                  static_cast<TUint16>( 
                  	iRecordData[ pos ] << 8 | iRecordData[ pos + 1 ] ) ) && 
                ( static_cast<TUint16>( instanceInfo.iOffset ) == 
                  static_cast<TUint16>( 
                  	iRecordData[ pos + 2 ] << 8 | iRecordData[ pos + 3 ] ) ) &&
                ( static_cast<TUint16>( instanceInfo.iLength ) == 
                  static_cast<TUint16>(
                  	iRecordData[ pos + 4 ] << 8 | iRecordData[ pos + 5 ] ) ) ) 
                {
                iconFound = ETrue;
                }
                
            pos += KImageInstanceFileLength;
            iconNumber--;
            }
            
        iImageInstanceNoInRecord = iRecordData[0] - iconNumber; 
        
        // Check icon data length
        if( ( KIconDataMaxSize > instanceInfo.iLength ) && iconFound )
            {
            TBuf8<KGetIconDataPckgLength> data( 0 );
            data.Append( iRecordNumber );
            data.Append( iImageInstanceNoInRecord );
            // Pack data
		    CSatDataPackage dataPackage;
			dataPackage.PackData( &data );
		    ret = iSatTsy->MessageManager()->HandleRequestL(
			    ESatTsyGetImageInstance, &dataPackage );
            }
        }

    return ret;
    }


// -----------------------------------------------------------------------------
// CSatIconTsy::GetClutL
// This method gets the CLUT (Color look-up table) specified 
// by aInstanceNumber and Offset.
// -----------------------------------------------------------------------------
//
TInt CSatIconTsy::GetClutL
        (
        const TTsyReqHandle aTsyReqHandle,                                            
        RSat::TInstanceNumberAndOffset* /*aNumberAndOffset*/,
        TDes8* aClut                        
        )   
    {
    TFLOGSTRING("CSAT: CSatIconTsy::GetClutL");
    TInt ret = KErrNone;
	
    // Get possibly existing request handle
    TTsyReqHandle reqHandle = iSatTsy->GetSatReqHandleStore()->
		TsyReqHandle( CSatTsy::ESatGetImageInstanceReqType );

    // In case the request was ongoing, complete right away..
    if ( CSatTsy::ESatReqHandleUnknown != reqHandle )
    	{
        TFLOGSTRING("CSAT: CSatIcon::GetClutL : KErrServerBusy");
    	iSatTsy->ReqCompleted( aTsyReqHandle, KErrServerBusy);
        }
    else
    	{
	    // Save the request handle
	    iSatTsy->SaveReqHandle( aTsyReqHandle, CSatTsy::ESatGetClutReqType );

	    iIconData = aClut;
	    iIconReqTypeOngoing = EGetClutReq;

	    // Icon sequence number counted
        // Check icon data length
        if( KIconDataMaxSize > ( *aClut ).MaxLength() )
            {
            TBuf8<KGetIconDataPckgLength> data( 0 );
            data.Append( iRecordNumber );
            data.Append( iImageInstanceNoInRecord );                        
            // Pack data
		    CSatDataPackage dataPackage;
			dataPackage.PackData( &data );
		    ret = iSatTsy->MessageManager()->HandleRequestL( ESatTsyGetClut, 
		    	&dataPackage );
            }
        }

    iImageInstanceNoInRecord = 0;
    return ret;
    }

//  End of File


