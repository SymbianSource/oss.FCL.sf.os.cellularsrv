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
// Name        : CSatTsyReqHandleStore.cpp
// Part of     : Common SIM ATK TSY / commonsimatktsy
// Implementation of CSatTsyReqHandleStore class member functions.
// Version     : 1.0
//



//  INCLUDE FILES
#include "CSatTsyReqHandleStore.h"  // Class header
#include "CSatTsy.h"                // Main tsy class header
#include "TfLogger.h"               // For TFLOGSTRING

// -----------------------------------------------------------------------------
// CSatTsyReqHandleStore::NewL
// This method is used to create a new instance of CSatTsyReqHandleStore. 
// Because the constructor of CSatTsyReqHandleStore class is private, calling 
// this method is the only possible way to make new CSatTsyReqHandleStore object.
// -----------------------------------------------------------------------------
//
CSatTsyReqHandleStore* CSatTsyReqHandleStore::NewL
        (
        TInt aNumberOfRequests,      // Number of requests
        TTsyReqHandle* aFirstElement // Pointer to the first element
        )
    {
    TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::NewL" );
    CSatTsyReqHandleStore* tsyReqHandleStore = new ( ELeave ) 
        CSatTsyReqHandleStore();
 
    CleanupStack::PushL( tsyReqHandleStore );
    tsyReqHandleStore->iNumOfRequests = aNumberOfRequests;   
    tsyReqHandleStore->iReqHandles = aFirstElement;

    // Check that array is really initialised and reset it at the same time
    for ( TInt i = 0; i < aNumberOfRequests; i++ )
        {
        *( tsyReqHandleStore->iReqHandles + i ) = 0;
        }

    CleanupStack::Pop( tsyReqHandleStore );
    TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::NewL, end of method" );
    return tsyReqHandleStore;
    }

// -----------------------------------------------------------------------------
// CSatTsyReqHandleStore::~CSatTsyReqHandleStore
// Destructor. Deletes all entries from the CResponseTimerStore related with 
// this handle store
// -----------------------------------------------------------------------------
//
CSatTsyReqHandleStore::~CSatTsyReqHandleStore
        (
        // None
        )
    {
    TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::~CSatTsyReqHandleStore" );
    }

// -----------------------------------------------------------------------------
// CSatTsyReqHandleStore::CSatTsyReqHandleStore
// Constructor
// -----------------------------------------------------------------------------
//
CSatTsyReqHandleStore::CSatTsyReqHandleStore
        (
        // None
        )
    {
    TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::CSatTsyReqHandleStore" );
    }

// -----------------------------------------------------------------------------
// CSatTsyReqHandleStore::ConstructL
// Symbian 2nd phase constructor. 
// -----------------------------------------------------------------------------
//
void CSatTsyReqHandleStore::ConstructL
        ( 
        // None
        )
    {
    TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::ConstructL" );
    }

// -----------------------------------------------------------------------------
// CSatTsyReqHandleStore::TsyReqHandle
// Returns TSY request handle for given index.
// -----------------------------------------------------------------------------
//
TTsyReqHandle CSatTsyReqHandleStore::TsyReqHandle
        (
        const TInt aIndex  // TSY req handle index
        )
    {
    TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::TsyReqHandle" );
    TTsyReqHandle ret( NULL );

    // Check the bounds
    if ( ( aIndex >= 0 ) && ( aIndex < iNumOfRequests ) )
        {
        ret = *( iReqHandles + aIndex );
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::TsyReqHandle, \
            Out of bounds" );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSatTsyReqHandleStore::SetTsyReqHandle
// Sets TSY request handle for given index.
// -----------------------------------------------------------------------------
//
void CSatTsyReqHandleStore::SetTsyReqHandle
        (    
        const TInt            aIndex,         // Index
        const TTsyReqHandle   aTsyReqHandle   // Request handle
        )
    {
    TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::SetTsyReqHandle" );
    if ( ( aIndex >= 0 ) && ( aIndex < iNumOfRequests ) )
        {
        *( iReqHandles + aIndex ) = aTsyReqHandle;
        TFLOGSTRING2( "CSAT: CSatTsyReqHandleStore::SetTsyReqHandle, %d saved", 
            aTsyReqHandle );
        }
    else
        {
        TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::SetTsyReqHandle,\
            Request handle not saved" );
        }
    }

// -----------------------------------------------------------------------------
// CSatTsyReqHandleStore::ResetTsyReqHandle
// Resets TSY req handle of given index and also returns it. If automatic 
// response is in use it deletes the entry for the given index in the 
// CResponseTimerStore.
// -----------------------------------------------------------------------------
//
TTsyReqHandle CSatTsyReqHandleStore::ResetTsyReqHandle
        (    
        const TInt aIndex   // Index
        )
    {

    TFLOGSTRING( "CSAT: CSatTsyReqHandleStore::ResetTsyReqHandle" );
    // Req handle was not used.
    TInt ret = 0;

    if ( ( aIndex >= 0 ) && ( aIndex < iNumOfRequests ) )
        {
        // If req handle is currently used
        if( *( iReqHandles + aIndex ) != 0 )
            {
            // Return deleted req handle
            ret = *( iReqHandles + aIndex );

            // Reset req handle
            *( iReqHandles + aIndex ) = 0;
            TFLOGSTRING2( "CSAT: CSatTsyReqHandleStore::ResetTsyReqHandle, index \
            	%d removed", aIndex );
            }
        else
            {
            TFLOGSTRING("CSAT: CSatTsyReqHandleStore::ResetTsyReqHandle,\
                Not in use");
            }
        }
    else
        {
        // Do nothing    
        }
            
	TFLOGSTRING2("CSAT: CSatTsyReqHandleStore::ResetTsyReqHandle \
	    reqHandle is :%d", ret ); 
		
    return ret;
    }

//  End of File 




