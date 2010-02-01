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
//



//  INCLUDE FILES
#include <e32std.h>
#include "cmmtsyreqhandlestore.h"
#include "cmmphonetsy.h"
#include "CResponseTimerStore.h"

// ======== MEMBER FUNCTIONS ========

CMmTsyReqHandleStore::~CMmTsyReqHandleStore()
    {
#ifdef REQHANDLE_TIMER

    //Check if this handle store is using the timer
    if ( NULL != iPhone )    
        {
        //delete the possible entries in the time stamp table
        iPhone->GetTimeStampStore()->DeleteByHandleStoreAllEntries( this );
        }

#endif
    }

CMmTsyReqHandleStore* CMmTsyReqHandleStore::NewL(
    TInt aNumberOfRequests, //number of requests
    TTsyReqHandle* aFirstElement ) //pointer to the first element
    {
    TFLOGSTRING3("TSY:CMmTsyReqHandleStore::NewL aNumberOfRequests=%d aFirstElement=%x",aNumberOfRequests,aFirstElement);
    
    CMmTsyReqHandleStore* tsyReqHandleStore 
            = new ( ELeave ) CMmTsyReqHandleStore();
 
    CleanupStack::PushL( tsyReqHandleStore );

    tsyReqHandleStore->iNumOfRequests = aNumberOfRequests;   
    tsyReqHandleStore->iReqHandles = aFirstElement;

    //check that array is really initialised and reset it at the same time
    for( TInt i = 0; i < aNumberOfRequests; i++ )
        *( tsyReqHandleStore->iReqHandles + i ) = 0;

    CleanupStack::Pop();
    TFLOGSTRING2("TSY:CMmTsyReqHandleStore::NewL returns tsyReqHandleStore=%x",tsyReqHandleStore);
    return tsyReqHandleStore;
    }

#ifdef REQHANDLE_TIMER

CMmTsyReqHandleStore* CMmTsyReqHandleStore::NewL(
    CTelObject * aCTelObject, 
    CMmPhoneTsy *aPhone, 
    TInt aNumberOfRequests, 
    TTsyReqHandle* aFirstElement ) 
    {
    TFLOGSTRING3("TSY:CMmTsyReqHandleStore::NewL aCTelObject=%x aPhone=%x",aCTelObject,aPhone);
    TFLOGSTRING3("TSY:CMmTsyReqHandleStore::NewL aNumberOfRequests=%d aFirstElement=%x",aNumberOfRequests,aFirstElement);
    CMmTsyReqHandleStore* tsyReqHandleStore 
            = new ( ELeave ) CMmTsyReqHandleStore();
    
    CleanupStack::PushL( tsyReqHandleStore );

    tsyReqHandleStore->iNumOfRequests = aNumberOfRequests;   
    tsyReqHandleStore->iReqHandles = aFirstElement;
    tsyReqHandleStore->iCTelObject = aCTelObject;
    tsyReqHandleStore->iPhone = aPhone;
 
    for( TInt i = 0; i < aNumberOfRequests; i++ )
        *( tsyReqHandleStore->iReqHandles + i ) = 0;
 
    CleanupStack::Pop();
 
    return tsyReqHandleStore;
    }

#endif

// ---------------------------------------------------------------------------
// CMmTsyReqHandleStore::GetTsyReqHandle
// Returns TSY request handle for given index.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TTsyReqHandle CMmTsyReqHandleStore::GetTsyReqHandle(
    const TInt aIndex )
    {
    TFLOGSTRING2("TSY:CMmTsyReqHandleStore::GetTsyReqHandle aIndex=%d", aIndex);
    TTsyReqHandle ret( NULL );

    //check the bounds
    if ( aIndex >= 0 && aIndex < iNumOfRequests )
        {
        ret = *( iReqHandles + aIndex );
        }

    return ret;
    }
    
// ---------------------------------------------------------------------------
// CMmTsyReqHandleStore::SetTsyReqHandle
// Sets TSY request handle for given index.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmTsyReqHandleStore::SetTsyReqHandle(    
    const TInt aIndex, 
    const TTsyReqHandle aTsyReqHandle ) 
    {
    TFLOGSTRING3("TSY:CMmTsyReqHandleStore::SetTsyReqHandle aIndex=%d aTsyReqHandle=%d ", aIndex, aTsyReqHandle);
    if ( aIndex >= 0 && aIndex < iNumOfRequests )
        {
        *( iReqHandles + aIndex ) = aTsyReqHandle;
        }

    }

#ifdef REQHANDLE_TIMER

// ---------------------------------------------------------------------------
// CMmTsyReqHandleStore::SetTsyReqHandle
// Sets TSY request handle for given index. Adds an entry
// in the time table for automatic response when timeout.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmTsyReqHandleStore::SetTsyReqHandle(    
    const TInt aIndex, 
    const TTsyReqHandle aTsyReqHandle, 
    TInt aTimeout, 
    TInt aIPC ) 
    {
    TFLOGSTRING3("TSY:CMmTsyReqHandleStore::SetTsyReqHandle aIndex=%d, aTsyReqHandle=%d",aIndex,aTsyReqHandle);
    TFLOGSTRING3("TSY:CMmTsyReqHandleStore::SetTsyReqHandle aTimeout=%d  IPC=%d", aTimeout, aIPC);
    
    if ( aIndex >= 0 && aIndex < iNumOfRequests )
        {
        *( iReqHandles + aIndex ) = aTsyReqHandle;

#ifdef __WINS__
        //increase the timeout to help wins debugging
        aTimeout = aTimeout + KMultimodeTsyWinsTimeoutIncreasing;
#endif
        //add a entry for triggering the automatic response
        iPhone->GetTimeStampStore()->AddTimeEntry( aIndex, 
                                                   this,
                                                   iCTelObject,
                                                   aTimeout,
                                                   aIPC );
        }
    }

#endif

// ---------------------------------------------------------------------------
// CMmTsyReqHandleStore::ResetTsyReqHandle
// Resets TSY req handle of given index and also returns it.
// If automatic response is in use it deletes the entry for the given index
// in the CResponseTimerStore.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TTsyReqHandle CMmTsyReqHandleStore::ResetTsyReqHandle(    
    const TInt aIndex ) 
{
TFLOGSTRING2("TSY:CMmTsyReqHandleStore::ResetTsyReqHandle aIndex=%d", aIndex);
    //req handle was not used.
    TInt ret = 0;

#ifdef REQHANDLE_TIMER

    // check if this HandleStore is using the automatic response
    // in such case the iPhone internal attribute will be not NULL
    if( NULL != iPhone )
        {
        //check if a time Entry is set for this index 
        if (iPhone->GetTimeStampStore()->CheckEntry( aIndex, this ) )
            {
            //delete the entry cause the response is completed
            iPhone->GetTimeStampStore()->DeleteEntryByHandle( aIndex, this );
            }
        }
#endif 

    if ( aIndex >= 0 && aIndex < iNumOfRequests )
    {
    //if req handle is currently used
    if( *( iReqHandles + aIndex ) != 0 )
        {
        //return deleted req handle
        ret = *( iReqHandles + aIndex );

        //reset req handle
        *( iReqHandles + aIndex ) = 0;
    
        }
    }

    return ret;
}

#ifdef REQHANDLE_TIMER

// ---------------------------------------------------------------------------
// CMmTsyReqHandleStore::PostponeTimeout
// Sets a new Timeout for the given handle
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmTsyReqHandleStore::PostponeTimeout(
    const TInt aHandle, 
    const TInt aTimeout ) 
    {
    TFLOGSTRING3("TSY:CMmTsyReqHandleStore::PostponeTimeout aHandle=%d, aTimeout=%d",aHandle,aTimeout);
    // call ResetEntryTimeout 
    iPhone->GetTimeStampStore()->ResetEntryTimeout( aHandle,
                                                    this,
                                                    iCTelObject,
                                                    aTimeout );
    }

// ---------------------------------------------------------------------------
// CMmTsyReqHandleStore::StopTimeout
// Stops the timeout for the given handle. The entry for the
// given handle is deleted from the ResponseTimerStore.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmTsyReqHandleStore::StopTimeout(
    const TInt aHandle ) 
    {
    TFLOGSTRING2("TSY:CMmTsyReqHandleStore::StopTimeout aHandle=%d",aHandle);
    iPhone->GetTimeStampStore()->DeleteEntryByHandle( aHandle, this );
    }

#endif

//  End of File 




