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
#include "cmmonstoretsy.h"
#include "cmmtsyreqhandlestore.h"
#include "CMmPrivateUtility.h"
#include "MmTsy_numberOfSlots.h"
#include "MmTsy_timeoutdefs.h"
#include "cmmmessagemanagerbase.h"
#include "cmmpblist.h"
#include "cmmphonebookstoretsy.h"
#include <ctsy/pluginapi/cmmdatapackage.h>
#include <ctsy/serviceapi/mmtsy_ipcdefs.h>

// ======== MEMBER FUNCTIONS ========

CMmONStoreTsy::CMmONStoreTsy()
    {
    }

void CMmONStoreTsy::ConstructL()
    {
TFLOGSTRING("TSY: CMmONStoreTsy::ConstructL");

#ifdef REQHANDLE_TIMER
    // Create req handle store
    iTsyReqHandleStore = CMmTsyReqHandleStore::NewL( this, iMmPhone, 
        EMmTsyNumOfONStoreRequests, iStorageReqHandles );
#else // REQHANDLE_TIMER
    // Create req handle store
    iTsyReqHandleStore = CMmTsyReqHandleStore::NewL( 
        EMmTsyNumOfONStoreRequests, iStorageReqHandles );
#endif // REQHANDLE_TIMER

    //get mode of current extension.
    RMobilePhone::TMobilePhoneNetworkMode currentMode;
    iMmPhone->GetStaticExtensionMode( &currentMode );

    // Reset phonebooktype and servicetype
    ResetVariables();

    // Create GSM ONStore objects
    iMmONStoreGsmExt = static_cast<CMmONStoreExtInterface*>(
        CMmONStoreGsmExt::NewL( this ) );
    // Set mode GSM
    iMmONStoreExtInterface = iMmONStoreGsmExt;


    //if current mode is GSM
    if ( RMobilePhone::ENetworkModeGsm == currentMode )
        {
        // Set phonebook type and servicetype
        iPhoneBookType = KSimMsisdn;  // MSISDN
        }

    // Construct ReadAllList
    iONStoreListArray = new (ELeave) CArrayFixFlat<TONStoreMsg>(1);

    // Construct ReadAllArray
    iONStoreReadAllArray = new (ELeave) CArrayPtrFlat<CListReadAllAttempt>(1);

    //register ON Store tsy in the message manager
    iMmPhone->MessageManager()->RegisterTsyObject( 
        CMmMessageManagerBase::EONStoreTsy, this );

    // Reset info to fill with ini values
    SetInfo();
    }

CMmONStoreTsy* CMmONStoreTsy::NewL(
    CMmPhoneTsy* aMmPhone )
    {
    CMmONStoreTsy* aMmONStoreTsy = new( ELeave ) CMmONStoreTsy();
    CleanupClosePushL( *aMmONStoreTsy );

    aMmONStoreTsy->iMmPhone = aMmPhone;

    aMmONStoreTsy->ConstructL();
    CleanupStack::Pop();

    return aMmONStoreTsy;
    }

CMmONStoreTsy::~CMmONStoreTsy()
    {
TFLOGSTRING("TSY: CMmONStoreTsy::~CMmONStoreTsy");

    if (iMmPhone )
        {
        // deregister tsy object from message manager
        iMmPhone->MessageManager()->DeregisterTsyObject(this);
        }

    if ( NULL != iTsyReqHandleStore )
        {
        delete iTsyReqHandleStore;
        iTsyReqHandleStore = NULL;
        }

    // Delete ONStoreGsmExt
    delete iMmONStoreGsmExt;
    iMmONStoreGsmExt = NULL;

    // Delete TSY's internal ONStore list
    delete iONStoreListArray;
    iONStoreListArray = NULL;

    // Delete ONStoreReadAll array
    iONStoreReadAllArray->ResetAndDestroy();
    delete iONStoreReadAllArray;
    iONStoreReadAllArray = NULL;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::Init
// Initialisation method that is called from ETel Server.
// This is an initialization method for the class functionality.
// Generally initization method inits e.g. class attributes.
// In this case, this method is not needed.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::Init()
    {
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::OpenNewObjectByNameL
// This method opens extendable sub-sessions
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CTelObject* CMmONStoreTsy::OpenNewObjectByNameL(
    const TDesC& )
    {
    User::Leave( KErrNotSupported );
    //lint -e{527} "unreachable code"
    
    return NULL;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::OpenNewObjectL
// This method opens extendable sub-sessions
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CTelObject* CMmONStoreTsy::OpenNewObjectL(
    TDes& )
    {
    User::Leave( KErrNotSupported );
    //lint -e{527} "unreachable code"
    
    return NULL;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::ReqModeL
// To ask what type of flow control is used for the IPC number.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CTelObject::TReqMode CMmONStoreTsy::ReqModeL(
    const TInt aIpc )
    {
TFLOGSTRING2("TSY: CMmONStoreTsy::ReqModeL IPC %d", aIpc);
    CTelObject::TReqMode ret=0;
    switch ( aIpc )
        {
        // Flow Controlled Services
        case EMobilePhoneStoreRead:
        case EMobilePhoneStoreWrite:
        case EMobilePhoneStoreDelete:
        case EMobilePhoneStoreDeleteAll:
        case EMobilePhoneStoreReadAllPhase1:
        case EMobilePhoneStoreReadAllPhase2:
        case EMobilePhoneStoreGetInfo:
        case EMobileONStoreStoreAll:
            ret=KReqModeFlowControlObeyed;
            break;
        // Multiple Completion Services with Immediate Server Repost
        // (Usually Notifications)
        case EMobilePhoneStoreNotifyStoreEvent:
            ret=KReqModeMultipleCompletionEnabled | KReqModeRePostImmediately;
            break;
        default:
            User::Leave(KErrNotSupported);
            break;
        }
        
    return ret;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::RegisterNotification
// Enables the TSY to "turn on" any regular notification
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::RegisterNotification(
    const TInt aIpc )
    {
    TInt ret( KErrNone );

    switch ( aIpc )
        {
        case EMobilePhoneStoreNotifyStoreEvent:
            // KErrNone
            break;
        default:
            // Unknown or invalid IPC
            ret = KErrNotSupported;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::DeregisterNotification
// Enables the TSY to "turn off" any regular notification
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::DeregisterNotification(
    const TInt aIpc )
    {
    TInt ret( KErrNone );

    switch ( aIpc )
        {
        case EMobilePhoneStoreNotifyStoreEvent:
            // KErrNone
            break;
        default:
            // Unknown or invalid IPC
            ret = KErrNotSupported;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::NumberOfSlotsL
// To ask the TSY how big buffer it wants
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::NumberOfSlotsL(
    const TInt aIpc )
    {
    TInt numberOfSlots = 1;

    switch ( aIpc )
        {
        case EMobilePhoneStoreNotifyStoreEvent:
            numberOfSlots = KMmONStoreNotifyStoreEventSlots;
            break;
        default:
            // Unknown or invalid Phone IPC
            User::Leave( KErrNotSupported );
            break;
        }

    return numberOfSlots;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::ExtFunc
// Handling of extended requests
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::ExtFunc(
    const TTsyReqHandle aTsyReqHandle,
    const TInt aIpc,
    const TDataPackage& aPackage )
    {
TFLOGSTRING3("TSY: CMmONStoreTsy::ExtFunc - IPC:%d Handle:%d", aIpc, aTsyReqHandle);   
               
    TInt ret = KErrNone;
    TInt trapError = KErrNone;

    // Reset last tsy request type
    iReqHandleType = EMultimodeONStoreReqHandleUnknown;

    TRAP( trapError, ret = DoExtFuncL( aTsyReqHandle, aIpc, aPackage ); );

    if ( KErrNone != trapError )
        {
        ReqCompleted( aTsyReqHandle, trapError );
        }
    else if ( KErrNone != ret )
        {
        ReqCompleted( aTsyReqHandle, ret );
        }

    // Save request handle
    if ( EMultimodeONStoreReqHandleUnknown != iReqHandleType )
        {
#ifdef REQHANDLE_TIMER
        SetTypeOfResponse( iReqHandleType, aTsyReqHandle );
#else // REQHANDLE_TIMER
        iTsyReqHandleStore->SetTsyReqHandle( iReqHandleType, aTsyReqHandle );
#endif // REQHANDLE_TIMER
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::DoExtFuncL
// Dispatches extension function requests
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::DoExtFuncL(
    const TTsyReqHandle aTsyReqHandle,
    const TInt aIpc,
    const TDataPackage& aPackage )
    {
TFLOGSTRING3("TSY: CMmONStoreTsy::DoExtFuncL - IPC:%d Handle:%d", aIpc, aTsyReqHandle);

    TAny* dataPtr=aPackage.Ptr1();
    TAny* dataPtr2=aPackage.Ptr2();

    // Create unique transaction id

    switch (aIpc)
        {
        case EMobilePhoneStoreRead:
            return ReadL( aPackage.Des1n() );

        case EMobilePhoneStoreWrite:
            return WriteL( aTsyReqHandle, aPackage.Des1n() );

        case EMobilePhoneStoreDelete:
            return DeleteL( aTsyReqHandle,
                REINTERPRET_CAST( TInt*, dataPtr ) );

        case EMobilePhoneStoreDeleteAll:
            return DeleteAllL( aTsyReqHandle );

        case EMobilePhoneStoreGetInfo:
            return GetInfoL( aTsyReqHandle,
                REINTERPRET_CAST( RMobileONStore::TMobileONStoreInfoV1*, 
                    dataPtr ) );

        case EMobilePhoneStoreReadAllPhase1:
            return ReadAllPhase1L( aTsyReqHandle,
                REINTERPRET_CAST( RMobilePhone::TClientId*, dataPtr ),
                REINTERPRET_CAST( TInt*, dataPtr2 ) );

        case EMobilePhoneStoreReadAllPhase2:
            return ReadAllPhase2( aTsyReqHandle,
                REINTERPRET_CAST( RMobilePhone::TClientId*, dataPtr ), 
                aPackage.Des2n() );

        case EMobilePhoneStoreNotifyStoreEvent:
            return NotifyStoreEvent(
                REINTERPRET_CAST( RMobilePhoneStore::TMobileStoreEvent*, 
                dataPtr ), REINTERPRET_CAST( TInt*, dataPtr2 ) );

        case EMobileONStoreStoreAll:
            return StoreAllL( aTsyReqHandle,aPackage.Des1n() );

        default:
            return KErrNotSupported;
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CancelService
// To cancel any extension requests
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::CancelService(
    const TInt aIpc,
    const TTsyReqHandle aTsyReqHandle )
    {
    switch ( aIpc )
        {
        case EMobilePhoneStoreGetInfo:
            return GetInfoCancel( aTsyReqHandle );
        case EMobilePhoneStoreRead:
            return ReadCancel( aTsyReqHandle );
        case EMobilePhoneStoreWrite:
            return WriteCancel( aTsyReqHandle );
        case EMobilePhoneStoreDelete:
            // There is no change to cancel this request, so only thing to do,
            // is let the request continue and return KErrNone.
            return KErrNone;
        case EMobilePhoneStoreDeleteAll:
            return DeleteAllCancel( aTsyReqHandle );
        case EMobilePhoneStoreReadAllPhase1:
        case EMobilePhoneStoreReadAllPhase2:
            return ReadAllCancel( aTsyReqHandle );
        case EMobilePhoneStoreNotifyStoreEvent:
            return NotifyStoreEventCancel( aTsyReqHandle );
        case EMobileONStoreStoreAll:
            return StoreAllCancel( aTsyReqHandle );
        default:
            return KErrGeneral;
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::SetActiveONStoreExtension
// Sets pointer to active extension
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::SetActiveONStoreExtension(
    CMmONStoreExtInterface* aMmONStoreExt )
    {
    TInt ret = KErrGeneral;

    if ( aMmONStoreExt )
        {
        iMmONStoreExtInterface = aMmONStoreExt;
        ret = KErrNone;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::Returns a pointer to the active extension
// Sets pointer to active extension
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CMmONStoreExtInterface* CMmONStoreTsy::ActiveONExtension()
    {
    return iMmONStoreExtInterface;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::ReadL
// Read one ON store entry using index
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::ReadL( TDes8* aEntry )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::ReadL");
    // Initialize ret value
    TInt ret( KErrArgument );

    if( sizeof( RMobileONStore::TMobileONEntryV1 ) <= aEntry->MaxLength() )
	  	{

        // Unpack entry parameter
        RMobileONStore::TMobileONEntryV1Pckg* entryPckg =
            REINTERPRET_CAST
            ( RMobileONStore::TMobileONEntryV1Pckg*, aEntry );
        RMobileONStore::TMobileONEntryV1& entry = ( *entryPckg )();

        // Save entry pointer
        iReadONStoreEntryPtr = aEntry;

        // Forward request to GSM Extension
        ret = iMmONStoreExtInterface->ReadL( EMmTsyONStoreReadIPC, entry.iIndex );

        if ( KErrNone == ret )
            {
            // Save tsy req handle type
            iReqHandleType = EMultimodeONStoreRead;
            }
	  	}

    return ret;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteRead
// Completes reading a entry from the ONStore
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteRead(
    TInt aResult,
    CMmDataPackage* aDataPackage )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteRead");
    // Response information
    TONStoreMsg* ONStoreResp = NULL;
    // unpack data if exists
    if ( aDataPackage )
        {
        //unpack data
        aDataPackage->UnPackData( ONStoreResp );
        }
    else if ( KErrNone == aResult )
        {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteRead result is KErrNone and aDataPackage is NULL!");
        aResult = KErrArgument;
        }

    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreRead );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {

        if ( KErrNone == aResult )
            {

            // Get entry
            RMobileONStore::TMobileONEntryV1Pckg* entryPckg =
                REINTERPRET_CAST( RMobileONStore::TMobileONEntryV1Pckg*,
                iReadONStoreEntryPtr );
            RMobileONStore::TMobileONEntryV1& entry = ( *entryPckg )();

            // Fill entry
            // Map iIndex to client world (increase value by one)
            entry.iIndex = ONStoreResp->iLocation;
            entry.iText.Copy( ONStoreResp->iName );
            entry.iNumber.iTelNumber.Copy( ONStoreResp->iTelNumber );

            // SIM_PN_READ_RESP response
            entry.iMode = RMobilePhone::ENetworkModeUnknown;
            /*ENetworkModeUnknown,
            ENetworkModeUnregistered,
            ENetworkModeGsm,
            ENetworkModeAmps,
            ENetworkModeCdma,
            ENetworkModeWcdma
            */
            entry.iService = RMobilePhone::EServiceUnspecified;
            /*EServiceUnspecified,
            EVoiceService,
            EAuxVoiceService,
            ECircuitDataService,
            EPacketDataService,
            EFaxService,
            EShortMessageService,
            EAllServices
            */

            // Complete request to success
            ReqCompleted( reqHandle, KErrNone );
            }
        else
            {
            // Failure
            ReqCompleted( reqHandle, aResult );
            }
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::ReadCancel
// Cancel read one ON store entry using index
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::ReadCancel(
    const TTsyReqHandle aTsyReqHandle )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::ReadCancel");
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreRead );

    // Check that request handle was valid
    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        // Complete with KErrCancel
        ReqCompleted( aTsyReqHandle, KErrCancel );
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::WriteL
// Writes the entry supplied within the entry parameter into the store
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::WriteL(
    const TTsyReqHandle aTsyReqHandle,
    TDesC8* aEntry )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::WriteL");
    // Save tsy req handle type
    iReqHandleType = EMultimodeONStoreWrite;

    // Unpack entry param to get the location
    RMobileONStore::TMobileONEntryV1Pckg* entryPckg =
        REINTERPRET_CAST
        ( RMobileONStore::TMobileONEntryV1Pckg*, aEntry );
    RMobileONStore::TMobileONEntryV1& entry = ( *entryPckg )();

    iWriteONStoreEntryPtr = aEntry;
    iWriteIndexToReturn = entry.iIndex;

    // It is not possible to cancel write request anymore...
    iIsWriteCancellingPossible = EFalse;

    // Return value
    TInt ret( KErrNone );

    ret = iMmONStoreExtInterface->
        WriteL(
        EMmTsyONStoreWriteIPC,
        aEntry );

    // Check error while processing write
    if ( KErrNone != ret )
        {
        iWriteIndexToReturn = 0;
        iWriteONStoreEntryPtr = NULL;
        ReqCompleted( aTsyReqHandle, ret );
        }
    else
        {
        // Save tsy req handle type
        iReqHandleType = EMultimodeONStoreWrite;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteWrite
// Completes writing a entry to the phonebookstore
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteWrite(
    TInt aResult,
    CMmDataPackage* aDataPackage )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteWrite");   
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreWrite );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        if ( KErrNone == aResult )
            {
            TInt location = 0;
            
            if ( aDataPackage )
                {
                aDataPackage->UnPackData( location );
                }
                
            // Complete notify store event
            iMobileONStoreEvent = ( RMobilePhoneStore::KStoreEntryAdded );

            // Return index within entry.iIndex, if needed
            if ( -1 == iWriteIndexToReturn )
                {
                // Unpack entry param
                RMobileONStore::TMobileONEntryV1Pckg* entryPckg =
                    ( RMobileONStore::TMobileONEntryV1Pckg*
                    )iWriteONStoreEntryPtr;
                RMobileONStore::TMobileONEntryV1& entry = ( *entryPckg )();

                // Set the correct entry index
                entry.iIndex = location;
                }
            
            CompleteNotifyStoreEvent( location );
            }

        // Complete with error
        ReqCompleted( reqHandle, aResult );
        }

    iWriteIndexToReturn = 0;
    iWriteONStoreEntryPtr = NULL;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::WriteCancel
// Cancels an outstanding Write method
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::WriteCancel(
    const TTsyReqHandle aTsyReqHandle )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::WriteCancel");
    // Check if cancelling is possible. If not, return KErrNone
    if ( iIsWriteCancellingPossible )
        {
        // Reset req handle. Returns the deleted req handle
        TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
            EMultimodeONStoreWrite );

        if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
            {
            iWriteIndexToReturn = 0;
            iWriteONStoreEntryPtr = NULL;
            ReqCompleted( aTsyReqHandle, KErrCancel );
            }
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::DeleteL
// Deletes one ON entry by index supplied as a parameter
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::DeleteL(
    const TTsyReqHandle aTsyReqHandle,
    TInt* aIndex )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::DeleteL");
    // Initialize ret value
    TInt ret( KErrGeneral );

    // Get index and decrease value (CS uses indexes '0' ->, SOS '1' ->)
    iDeleteONStoreIndex = *aIndex;

    // Check that entry index value is greater than zero
    if ( 0 < iDeleteONStoreIndex )
        {
        // Forward Delete request to gsm extension. Use Write function
        // to "erase" ON store entry
        ret = iMmONStoreExtInterface->DeleteL( EMmTsyONStoreDeleteIPC, 
            iDeleteONStoreIndex );
        }

    // Message construction failed or phonet sender returned error
    if ( KErrNone != ret )
        {
        ReqCompleted( aTsyReqHandle, ret );
        }
    else
        {
        // Save tsy request handle type
        iReqHandleType = EMultimodeONStoreDelete;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteDelete
// Complete delete one ON entry by index
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteDelete(
    TInt aResult )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteDelete");
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreDelete );

    // Check that request was valid
    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        // Complete with CS return value
        ReqCompleted( reqHandle, aResult );

        // If deletetion done succesfully, complete notify store event
        if ( KErrNone == aResult )
            {
            // Complete notify store event
            iMobileONStoreEvent = ( RMobilePhoneStore::KStoreEntryDeleted );
            CompleteNotifyStoreEvent( iDeleteONStoreIndex );
            }
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::DeleteAllL
// Delete all ON store entries
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::DeleteAllL (
    const TTsyReqHandle aTsyReqHandle )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::DeleteAllL");
    // Init variable for DeleteAllPhase1
    iDeleteAllContinue = ETrue;

    // Forwarded extension call
    TInt ret = iMmONStoreExtInterface->DeleteAllL( 
        EMmTsyONStoreDeleteAllIPC );

    // Check is return value valid
    if ( KErrNone != ret )
        {
        // Complete with error
        ReqCompleted( aTsyReqHandle, ret );
        }
    else
        {
        // Save TSY request handle type
        iReqHandleType = EMultimodeONStoreDeleteAll;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteDeleteAllPhase1L
// Complete delete all ON store entries phase 1
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteDeleteAllPhase1L(
    TInt aResult )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteDeleteAllPhase1L");
    // Check that DeleteAll was called
    if ( iDeleteAllContinue )
        {
        if ( KErrNone == aResult )
            {
            // Empty variables in order to call Write-function
            TBuf<1> name;
            TBuf<1> telNumber;

#ifdef REQHANDLE_TIMER
            // Set the new terminate time for deleting new entries from
            // ONStore. About 300ms/entry => about (15s)/(50entries) ->
            // x2 -> 30sec.
            iTsyReqHandleStore->PostponeTimeout( EMultimodeONStoreDeleteAll,
                30 );
#endif // REQHANDLE_TIMER

            // Forward Delete request to gsm extension. Use Write function
            // to start process to erase all ON store entries.
            // '0' means that first location will be deleted first
              }
        // If error from SIM Server
        else
            {
            CompleteDeleteAll( aResult );
            }
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteDeleteAll
// Complete delete all ON store entries
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteDeleteAll(
    TInt aResult )
    {
    // Reset request handle. Returns the deleted request handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreDeleteAll );

    // Check that deleted request handle was valid
    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteDeleteAll");        
        // Complete with error
        ReqCompleted( reqHandle, aResult );

        // If deletion done succesfully, complete notify store event
        // Notify will complete if it was requested
        if ( KErrNone == aResult )
            {
            // Set notify reason, whole ON store deleted. Store empty.
            iMobileONStoreEvent = ( RMobilePhoneStore::KStoreEmpty );
            // Complete notify store event, with '-1' due that more than one
            // entries deleted.
            CompleteNotifyStoreEvent( -1 );
            }
        }
    else if(EMultimodeONStoreReqHandleUnknown !=
				 iTsyReqHandleStore->GetTsyReqHandle(EMultimodeONStoreStoreAll ))
    	{
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteDeleteAll requested by StoreAllL.");
    	TInt ret (aResult);
		    	
    	if(KErrNone == ret)
    		{
    		// ON store is now empty. Let's begin with reading store size to actually store all entries.
    		TInt writeAllIndex = -1; // Store index when writing all entries
    		TRAPD(err, ret = iMmONStoreExtInterface->ReadL(
    				EMmTsyONStoreWriteSizeIPC, writeAllIndex ));
			if(err != KErrNone)
				{
				ret = err;
				}
    		}
        if(KErrNone != ret)
        	{
        	CompleteStoreAll( ret );
        	}
    	}
    else
    	{
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteDeleteAll - the request has been cancelled or bad req handle");
    	}
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::DeleteAllCancel
// This method cancels an outstanding DeleteAll -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::DeleteAllCancel(
    const TTsyReqHandle aTsyReqHandle )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::DeleteAllCancel");
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreDeleteAll );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        // Prevent DeleteAllPhase1
        iDeleteAllContinue = EFalse;
        // Complete with KErrCancel
        ReqCompleted( aTsyReqHandle, KErrCancel );
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::GetInfoL
// This method returns information about the ONStore
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::GetInfoL(
    const TTsyReqHandle aTsyReqHandle,
    RMobileONStore::TMobileONStoreInfoV1* aInfo )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::GetInfoL");

    TInt ret( KErrNone );

    // if aInfo is not correct type, send KErrArgument to client and return
    if( RMobilePhoneStore::KETelMobileONStoreV1 != aInfo->ExtensionId() )
        {
        ReqCompleted( aTsyReqHandle, KErrArgument );
        return ret;
        }

   	// Don't send request if pb init is still ongoing
	// instead send KErrNotReady to client
	TBool done ( ETrue );
TFLOGSTRING2( "TSY: CMmONStoreTsy::GetInfoL iPBList->GetNumberOfObjects() = %d", iMmPhone->PBList()->GetNumberOfObjects());

	for( TInt i = 0; i < iMmPhone->PBList()->GetNumberOfObjects(); i++ )
	    {
	    CMmPhoneBookStoreTsy* pbStore = iMmPhone->PBList()->
	        GetMmPBByIndex( i );
TFLOGSTRING2( "TSY: CMmONStoreTsy::GetInfoL index = %d", i);    

		if ( !pbStore->IsPBInitDone() 
			  && ( pbStore->GetPhonebookType() == KADNPhoneBook
		      || pbStore->GetPhonebookType() == KFDNPhoneBook )  )
		    {
TFLOGSTRING("TSY: CMmONStoreTsy::GetInfoL - ADN or FDN init not done");
TFLOGSTRING("TSY: CMmONStoreTsy::GetInfoL - in !pbStore->IsPBInitDone()");
			// init still ongoing
			done = EFalse;
			}
   		}
   				
    // Check if some PB is not initialized, if so send KErrNotReady to client   				
   	if ( !done )
		{
TFLOGSTRING("TSY: CMmONStoreTsy::GetInfoL - PBInit not done, return KErrNotReady");
		ReqCompleted( aTsyReqHandle, KErrNotReady );
		}
	            
	else
		{
TFLOGSTRING("TSY: CMmONStoreTsy::GetInfoL - PBInit done, complete");
        // Set get info internal pointer
        iONStoreInfoChanged = aInfo;
        
        // Request to GSM extension, get used entries,
        // -1 as a index param-> specified location param not needed
        ret = iMmONStoreExtInterface->ReadL( EMmTsyONStoreGetInfoIPC, -1 );

        if ( KErrNone != ret )   // Message sending failed
            {
            ReqCompleted( aTsyReqHandle, ret );
            }
        else
            {
            iIsGetInfoActive = ETrue;
            // Save tsy req handle type
            iReqHandleType = EMultimodeONStoreGetInfo;
            }
		}

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteGetInfo
// Completes GetInfo -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteGetInfo( 
    TInt aResult,
    CMmDataPackage* aDataPackage )
    {
TFLOGSTRING2("TSY: CMmONStoreTsy::CompleteGetInfo - Result: %d", aResult );

    // The number of used entries
    TServiceType serviceType;   
    //unpack data
    aDataPackage->UnPackData( serviceType );

    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreGetInfo );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {

        // If request was not cancelled or no error from SIM
        // give info to client
        if ( KErrNone == aResult )
            {
            iONStoreInfo.iTotalEntries = serviceType.iNumOfEntries;
            iONStoreInfo.iUsedEntries = serviceType.iUsedEntries;
            iONStoreInfo.iNumberLen = serviceType.iNumLen;
            iONStoreInfo.iTextLen = serviceType.iNameLen;

            *iONStoreInfoChanged = iONStoreInfo;

TFLOGSTRING3("TSY: CMmONStoreTsy::CompleteGetInfo - Name: %S, Type: %d", &iONStoreInfoChanged->iName, iONStoreInfoChanged->iType );
TFLOGSTRING3("TSY: CMmONStoreTsy::CompleteGetInfo - Total entries: %d, Used entries: %d", iONStoreInfoChanged->iTotalEntries, iONStoreInfoChanged->iUsedEntries );
TFLOGSTRING3("TSY: CMmONStoreTsy::CompleteGetInfo - Max Number length: %d, Max Name length: %d", iONStoreInfoChanged->iNumberLen, iONStoreInfoChanged->iTextLen );
            }

        // NULL the pointer to info supplied to TSY from the client
        iONStoreInfoChanged = NULL;
        iIsGetInfoActive = EFalse;
    
        // Complete with error
        ReqCompleted( reqHandle, aResult );
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::SetInfo
// This method initializes ONStore internal info parameters
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::SetInfo()
    {
TFLOGSTRING("TSY: CMmONStoreTsy::SetInfo");
    // The name of the store
    iONStoreInfo.iName = KETelOwnNumberStore;

    // The maximum number of characters for the number in a phonebook entry
    iONStoreInfo.iNumberLen = KNoInfoAvail;

    // The maximum number of characters for the text tag in a phonebook entry
    iONStoreInfo.iTextLen = KMaxTextLength;

    // The type of entry in the phone-side store
    iONStoreInfo.iType = RMobilePhoneStore::EOwnNumberStore;

    // Indicates the capabilities of the store
    iONStoreInfo.iCaps = KONStoreCaps;

    // Indicates the current number of entries that may be held in this store
    iONStoreInfo.iUsedEntries = KNoInfoAvail;

    // Indicates the total numbetr of entries
    iONStoreInfo.iTotalEntries = KNoInfoAvail;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::GetInfoCancel
// This method cancels an outstanding GetInfo -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::GetInfoCancel(
    const TTsyReqHandle aTsyReqHandle )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::GetInfoCancel");

    iTsyReqHandleStore->ResetTsyReqHandle( EMultimodeONStoreGetInfo );

    // Complete with KErrCancel
    ReqCompleted( aTsyReqHandle, KErrCancel );

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::NotifyStoreEvent
// This method activates notifying of ONStore events
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::NotifyStoreEvent(
    RMobilePhoneStore::TMobileStoreEvent* aEvent,
    TInt* aIndex )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::NotifyStoreEvent");
    TTsyReqHandle requestHandle = iTsyReqHandleStore->GetTsyReqHandle( 
        EMultimodeONStoreNotifyStoreEvent );

    // If request is not running
    if ( 0 == requestHandle )
        {
        // Set variables to notify
        iNotifyONStoreEventPtr = aEvent;
        iNotifyONStoreEventIndexPtr = aIndex;

        // Save tsy req handle type
        iReqHandleType = EMultimodeONStoreNotifyStoreEvent;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteNotifyStoreEvent
// This method is called when some of ONStore event happens
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteNotifyStoreEvent(
    TInt aLocation )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteNotifyStoreEvent");
    // Reset request handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreNotifyStoreEvent );

    // If request handle is valid
    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        RMobilePhoneStore::TMobileStoreEvent* ONStoreEvent =
            REINTERPRET_CAST( RMobilePhoneStore::TMobileStoreEvent*,
            iNotifyONStoreEventPtr );

        // Fill notify information
        *ONStoreEvent = iMobileONStoreEvent;
        *iNotifyONStoreEventIndexPtr = aLocation;

        ReqCompleted( reqHandle, KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::NotifyStoreEventCancel
// This method cancels an outstanding NotifyStoreEvent -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::NotifyStoreEventCancel(
    const TTsyReqHandle aTsyReqHandle )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::NotifyStoreEventCancel");
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreNotifyStoreEvent );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        // Reset variables and complete with KErrCancel
        iNotifyONStoreEventPtr = NULL;
        iNotifyONStoreEventIndexPtr = NULL;
        ReqCompleted( aTsyReqHandle, KErrCancel );
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::ReadAllPhase1L
// This method starts the two-phased ReadAll -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::ReadAllPhase1L(
    const TTsyReqHandle aReqHandle,
    const RMobilePhone::TClientId* aId, 
    TInt* aBufSize )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::ReadAllPhase1L");
    // Init return value
    TInt ret( KErrNone );
    
    // Store index when reading all entries
    TInt readAllIndex = -1; 
    
    iReadAllBufSizePtr = aBufSize;
    iReadAllId = *aId;
    
    iReadAllONStoreIndex = 0;
    ret = iMmONStoreExtInterface->
        ReadL( EMmTsyONStoreReadSizeIPC, readAllIndex );

    // Increase PhoneBookStoreIndex by 1.
    iReadAllONStoreIndex++;

    // Unsuccess request for read entry
    if ( KErrNone != ret )
        {
        // Complete with error from Phonet Sender
        ReqCompleted( aReqHandle, ret );
        }
    else
        {
        // Save tsy req handle type
        iReqHandleType = EMultimodeONStoreReadAll;
        }

    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CMmONStoreTsy::InternalRetrieveONListReadSizeL
// This method is part of ReadAll -request, purposed to get
// the ONStore size and continue request with reading the whole ONStore
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::InternalRetrieveONListReadSizeL(
    TInt aError,
    CMmDataPackage* aDataPackage )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::InternalRetrieveONListReadSizeL");
    if ( KErrNone == aError )
        {
        TInt size;
        aDataPackage->UnPackData( size );
TFLOGSTRING2("TSY: CMmONStoreTsy::InternalRetrieveONListReadSizeL size=%d",size);
        if( 0 < size )
            {
            *iReadAllBufSizePtr = size;
            iReadAllONStoreSize = size;
            MakeInternalRetrieveONListReadReqL();
            }
        else
            {
TFLOGSTRING2("TSY: CMmONStoreTsy::InternalRetrieveONListReadSizeL size is %d", size);
            CompleteReadAllPhase1Req( KErrNotFound );
            }
        }
    else
        {
        CompleteReadAllPhase1Req( aError );
        }

    } 

// ---------------------------------------------------------------------------
// CMmONStoreTsy::MakeInternalRetrieveONListReadReqL
// This method is part of ReadAll -request, purposed to read
// entries from ONStore and handle the ONStoreTSY's internal read all -index.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::MakeInternalRetrieveONListReadReqL()
    {
TFLOGSTRING("TSY: CMmONStoreTsy::MakeInternalRetrieveONListReadReqL");
    // Read one entry from the PhoneBook.
    TInt ret = iMmONStoreExtInterface->
        ReadL( EMmTsyONStoreReadEntryIPC,iReadAllONStoreIndex );

	// Increase PhoneBookStoreIndex by 1.
    iReadAllONStoreIndex++;

    // Unsuccess request for read entry
    if ( KErrNone != ret )
        {
        // Complete with error from Phonet Sender
        CompleteReadAllPhase1Req( ret );
        }
    }
    
// ---------------------------------------------------------------------------
// CMmONStoreTsy::InternalRetrieveONListReadEntryL
// This method is part of ReadAll -request, purposed to get the read response 
// and complete ReadAllPhase1 -request, if all the entries have been read.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::InternalRetrieveONListReadEntryL(
    TInt aError,
    CMmDataPackage* aDataPackage)
    {
TFLOGSTRING("TSY: CMmONStoreTsy::InternalRetrieveONListReadEntryL");
    if ( KErrNone == aError )
        {
        // Check if there is data to save
        if ( aDataPackage )
            {
            TONStoreMsg onStoreMsg;
            aDataPackage->UnPackData( onStoreMsg );
            // Add entry to the TSY's internal storage array
            iONStoreListArray->AppendL( onStoreMsg );
            }
        // Check if all entries read, complete if so
        if ( iReadAllONStoreIndex > iReadAllONStoreSize )
            {
            TInt leaveCode( KErrNone );

            TRAP( leaveCode, CompleteReadAllPhase1L(); );
            // Memory allocation failed
            // TRAP is used here, because if memory allocation
            // failed, list array elements must be destroyed
            if ( leaveCode != KErrNone )
                {
                DeleteONStoreListArrayElements();
                CompleteReadAllPhase1Req( leaveCode );
                }
            }
        // There is more entries to read
        else
            {
            MakeInternalRetrieveONListReadReqL();
            }
        }
    else
        {
        CompleteReadAllPhase1Req( aError );
        }
    }
  
// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteReadAllPhase1L
// Completes internal retrieving entries from the ONStore
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::CompleteReadAllPhase1L()
    {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteReadAllPhase1L");
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->GetTsyReqHandle(
        EMultimodeONStoreReadAll );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        // Check that ONStoreList array contains ONStore entries
        if( 0 < iONStoreListArray->Count() )
            {
            // read the list, store its content and then return size of this
            // buffer to client
            CMobilePhoneONList* list = CMobilePhoneONList::NewL();
            CleanupStack::PushL( list );

            RMobileONStore::TMobileONEntryV1 entry;

            // Loop until TSY's internal ONStorage is empty 
            while( !iONStoreListArray->Count() == 0 )     
                {
                TONStoreMsg& ONStoreMsg = iONStoreListArray->At(0);
 
                // Fill TMobileONEntryV1 information
                entry.iMode = RMobilePhone::ENetworkModeUnknown;
                entry.iService = RMobilePhone::EServiceUnspecified;
                entry.iIndex = ( ONStoreMsg.iLocation );
                                                             // from SIM
                entry.iNumber.iTypeOfNumber = RMobilePhone::EUnknownNumber;
                entry.iNumber.iNumberPlan = 
                    RMobilePhone::EUnknownNumberingPlan;
                entry.iNumber.iTelNumber.Copy( ONStoreMsg.iTelNumber );
                entry.iText.Copy( ONStoreMsg.iName );
         
                // Add to PhoneBook list
                list->AddEntryL( entry );

                // Delete list member
                iONStoreListArray->Delete(0);
                iONStoreListArray->Compress();
                }

            // Store the streamed list and the client ID
            CListReadAllAttempt* read = CListReadAllAttempt::NewL(
                &iReadAllId );
            CleanupStack::PushL( read );

            read->iListBuf = list->StoreLC();
            CleanupStack::Pop(); // Pop the CBufFlat allocated by StoreLC

            iONStoreReadAllArray->AppendL( read );
            CleanupStack::Pop(); // Pop the CListReadAllAttempt
            
            // Return the CBufFlat’s size to client
            *iReadAllBufSizePtr = ( read->iListBuf )->Size();
 
            // Complete first phase of list retrieval
            CompleteReadAllPhase1Req( KErrNone );
            CleanupStack::PopAndDestroy(); // Pop&destroy list

            return KErrNone;
            }
        else    // Store was empty
            {
            CompleteReadAllPhase1Req( KErrNotFound );
            }
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteReadAllPhase1Req
// Completes ReadAll phase1 -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteReadAllPhase1Req(
    TInt aResult )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::CompleteReadAllPhase1Req");
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreReadAll );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        // If this request is completed with error,
        // delete ReadAll -arrays
        if ( KErrNone != aResult )
            {
            // Delete ONStore array
            iONStoreReadAllArray->ResetAndDestroy();
            // Delete ONStoreList array
            DeleteONStoreListArrayElements();
            }

        ReqCompleted( reqHandle, aResult );
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::DeleteONStoreListArrayElements
// Deletes ONStore elements that are stored to an array
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::DeleteONStoreListArrayElements()
    {
TFLOGSTRING("TSY: CMmONStoreTsy::DeleteONStoreListArrayElements");
    while( 0 == !iONStoreListArray->Count() )
        {
        // Delete list member
        iONStoreListArray->Delete(0);
        iONStoreListArray->Compress();
        }

    // Delete pointers to elements from array
    iONStoreListArray->Reset();
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::ReadAllPhase2
// Read all ONStore entries, second phase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::ReadAllPhase2(
    TTsyReqHandle aTsyReqHandle,
    const RMobilePhone::TClientId* aId,
    TDes8* aBuffer )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::ReadAllPhase2");
    // Initialize ret value
    TInt ret( KErrCorrupt );

    // Set read pointer to NULL
    CListReadAllAttempt* read = NULL;

    // Find the read attempt from this client
    for ( TInt i=0; ( i<iONStoreReadAllArray->Count() ) &&
        ( KErrCorrupt == ret ); ++i )
        {
        // Get and set item from array
        // This is not deleted later on within this function if session
        // handle and subsession handle does not match.
        // Someone else 'owns' this data.
        read = iONStoreReadAllArray->At( i );

        // Check that session and client are rigth ones
        if ( ( read->iClient.iSessionHandle == aId->iSessionHandle ) &&
            ( read->iClient.iSubSessionHandle == aId->iSubSessionHandle ) )
            {
            // Set pointer to list buffer
            TPtr8 bufPtr( ( read->iListBuf )->Ptr( 0 ) );
            // Copy the streamed list to the client
            aBuffer->Copy( bufPtr );

            // Delete read and item from internal array
            delete read;
            iONStoreReadAllArray->Delete( i );

            // Complete request
            ReqCompleted( aTsyReqHandle, KErrNone );
            // Set ret to KErrNone so that loop can exit
            ret = KErrNone;
            }
        }

    // Check ret value
    if ( KErrCorrupt == ret )
        {
        // Doesn't found the matching client from read all phase 1
        iONStoreReadAllArray->ResetAndDestroy();
        ReqCompleted( aTsyReqHandle, KErrCorrupt);
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::ReadAllCancel
// Cancels an outstanding ReadAll -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::ReadAllCancel(
    const TTsyReqHandle aTsyReqHandle )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::ReadAllCancel");
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreReadAll );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        // Delete ONStore array
        iONStoreReadAllArray->ResetAndDestroy();
        // Delete ONStoreList elements
        DeleteONStoreListArrayElements();
        // Complete with KErrCancel
        ReqCompleted( aTsyReqHandle, KErrCancel );
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::StoreAllL
// This method stores the whole ONStore in one go
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::StoreAllL(
    const TTsyReqHandle aTsyReqHandle,
    TDes8* aBuffer )
    {
TFLOGSTRING("TSY: CMmONStoreTsy::StoreAllL");
    // Init return value
    TInt ret( KErrNone );
    
    // Set pointer to descrptor
    iStoreAllBuffer = aBuffer;

    // Create ONList and push it to cleanup stack
    CMobilePhoneONList* aONList=CMobilePhoneONList::NewL();
    CleanupStack::PushL(aONList);
    aONList->RestoreL(*aBuffer);
    
    // Get the size of ONList
    // Note: If this list does not contain any data, all the entries
    // will be deleted from ONStore!!
    iStoreAllONListSize = aONList->Enumerate();
    
    // StoreAll overwrites ON phone store. So, first delete all store.
    ret = iMmONStoreExtInterface->DeleteAllL( 
        EMmTsyONStoreDeleteAllIPC );
TFLOGSTRING2("CMmONStoreTsy::StoreAllL - DeleteAllL() ret: %d", ret);
    
    // Message construction failed or phonet sender returned error
    if ( KErrNone != ret )
        {
        ReqCompleted( aTsyReqHandle, ret );
        }
    else
        {
        // Save tsy req handle type
        iReqHandleType = EMultimodeONStoreStoreAll;
        }

    CleanupStack::PopAndDestroy(); // aONList
   
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::InternalStoreAllGetSizeL
// This method is part of StoreAll -request, purposed to get
// the ONStore size and continue request with writing the whole ONStore
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::InternalStoreAllGetSizeL(
    TInt aError,
    CMmDataPackage* aDataPackage )
    {
TFLOGSTRING2("CMmONStoreTsy::InternalStoreAllGetSizeL %d",aError);
    if ( KErrNone == aError )
        {
        TInt size;
        aDataPackage->UnPackData( size );

TFLOGSTRING2("CMmONStoreTsy::InternalStoreAllGetSizeL size %d", size);
        // Check the size of ONStore and the ONList size from Client
        // Client's ONList size should not be greater than ONStore size
        if ( size > 0 && size >= iStoreAllONListSize )
            {
            TTsyReqHandle storeAllRequestHandle = 
                iTsyReqHandleStore->GetTsyReqHandle(
                EMultimodeONStoreStoreAll );
            // Check if request has been cancelled or bad req. handle
            if ( EMultimodeONStoreReqHandleUnknown != storeAllRequestHandle )
                {
                // Ok, set the class attribute iReadAllPhoneBookStoreIndex 
                // with the size of the ONStore
                iStoreAllONStoreSize = 
                size;
                iStoreAllONStoreIndex = 0;
                MakeInternalStoreAllWriteReqL();
                }
            }
        // Error occurs, when the size of the ONStore is not > 0
        // or ONList size is greater that ONStore
        else
            {
            CompleteStoreAll( KErrTooBig );
            }
        }
    // Error from SIM Server
    else
        {
        CompleteStoreAll( aError );
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::MakeInternalStoreAllWriteReqL
// This method is part of StoreAll -request, purposed to make
// a write request by storeall index and an entry from ONList
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::MakeInternalStoreAllWriteReqL()
    {
TFLOGSTRING("CMmONStoreTsy::MakeInternalStoreAllWriteReqL ");
    // Create ONList and push it to cleanup stack
    CMobilePhoneONList* aONList=CMobilePhoneONList::NewL();
    CleanupStack::PushL(aONList);

    // Check the existance of buffered ONlist
    if ( iStoreAllBuffer )
        {
        aONList->RestoreL(*iStoreAllBuffer);

        // Initialize ONStore entry
        RMobileONStore::TMobileONEntryV1 entry;
        TInt ret( KErrNone );

        // Loop until all entriest written
        if( iStoreAllONStoreIndex < iStoreAllONStoreSize )
            {
            // Get entry from ONList from clinet, if list contains
            // entries
            if ( iStoreAllONStoreIndex < iStoreAllONListSize )
                {
                // Get entry from ONStore list
                entry = aONList->GetEntryL( iStoreAllONStoreIndex );
                }
            // List does not contain entries anymore!!
            // Destroy rest of the entries by overwriting Nam&Num with zero
            else
                {
                entry.iText.Zero();
                entry.iNumber.iTelNumber.Zero();
                }

#ifdef REQHANDLE_TIMER
            // Set 10 sec. terminate time for writing a new entry
            iTsyReqHandleStore->PostponeTimeout( 
                EMultimodeONStoreStoreAll, 10 );
#endif // REQHANDLE_TIMER
            CMmDataPackage dataPckg;
            dataPckg.PackData( &entry );
            // Forward request to GSM Extension
            ret = iMmONStoreExtInterface->WriteAllL(    
                EMmTsyONStoreWriteEntryIPC, &dataPckg );

            if ( KErrNone != ret )
                {
                CompleteStoreAll( ret );
                }
            iStoreAllONStoreIndex++;
            }
        // All entries written; complete with KErrNone
        else
            {
            CompleteStoreAll( KErrNone );
            }
        }
    // List not found
    else
        {
        CompleteStoreAll( KErrCorrupt );
        }

    CleanupStack::PopAndDestroy(); // aONList
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::InternalStoreAllWriteEntryL
// This function is part of StoreAll -request purposed to get
// the error value from write one ONStore entry -request. This method 
// continues StoreAll -request if the error is KErrNone supplied as 
// a parameter.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::InternalStoreAllWriteEntryL(
    TInt aResult )
    {
TFLOGSTRING2("CMmONStoreTsy::InternalStoreAllWriteEntryL %d",aResult);

	TTsyReqHandle storeAllRequestHandle = 
		iTsyReqHandleStore->GetTsyReqHandle(
				EMultimodeONStoreStoreAll );
	
	if ( EMultimodeONStoreReqHandleUnknown == storeAllRequestHandle )
		{
		// The request has been cancelled or bad req handle
TFLOGSTRING("The ON store request has been cancelled or bad req handle");
		return;
		}

    if ( KErrNone == aResult )
        {
        MakeInternalStoreAllWriteReqL();
        }
    // Error from SIM Server, complete with an error
    else
        {
        CompleteStoreAll( aResult );
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::CompleteStoreAll
// This method completes StoreAll -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::CompleteStoreAll(
    TInt aResult )
    {
TFLOGSTRING2("CMmONStoreTsy::CompleteStoreAll %d",aResult);
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreStoreAll );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {

        if ( KErrNone == aResult )
            {
            // Set notify reason, whole ON store stored. Do refresh.
            iMobileONStoreEvent = ( RMobilePhoneStore::KStoreDoRefresh );

            // Complete notify store event, with '-1' due that whole
            // store has been changed
            CompleteNotifyStoreEvent( -1 );
            }

        // Reset class attributes
        iStoreAllBuffer = NULL;
        iStoreAllONStoreSize = 0;
        iStoreAllONStoreIndex = 0;
        iStoreAllONListSize = 0;

        // Complete request
        ReqCompleted( reqHandle, aResult );
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::StoreAllCancel
// This method cancels an outstanding StoreAll -request
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmONStoreTsy::StoreAllCancel(
    TTsyReqHandle aTsyReqHandle )
    {
TFLOGSTRING("CMmONStoreTsy::StoreAllCancel");
    // Reset req handle. Returns the deleted req handle
    TTsyReqHandle reqHandle = iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodeONStoreStoreAll );

    if ( EMultimodeONStoreReqHandleUnknown != reqHandle )
        {
        // Reset class attributes
        iStoreAllBuffer = NULL;
        iStoreAllONStoreSize = 0;
        iStoreAllONStoreIndex = 0;
        iStoreAllONListSize = 0;

        // Complete with KErrCancel
        ReqCompleted( aTsyReqHandle, KErrCancel );
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::ResetVariables
// Reset all used variables
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::ResetVariables()
    {
    iPhoneBookType = KSimNoServiceType;
    iServiceType.iNumOfEntries = KNoInfoAvail;
    iServiceType.iNameLen = KNoInfoAvail;
    iServiceType.iNumLen = KNoInfoAvail;
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::Phone
// Returns CMmPhoneTsy object
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CMmPhoneTsy* CMmONStoreTsy::Phone()
    {
    return iMmPhone;
    }

#ifdef TF_LOGGING_ENABLED
// ---------------------------------------------------------------------------
// CMmONStoreTsy::ReqCompleted
// Overrides CTelObject::ReqCompleted for test logging purposes.
// It prints the aTsyReqHandle and aError variable in the log file and then
// calls CTelObject::ReqCompleted.
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::ReqCompleted(
    const TTsyReqHandle aTsyReqHandle, 
    const TInt aError )
    {
TFLOGSTRING3("TSY: CMmONStoreTsy::ReqCompleted Completed - Handle:%d Error:%d", aTsyReqHandle, aError);

    CTelObject::ReqCompleted( aTsyReqHandle, aError );
    }

#endif // TF_LOGGING_ENABLED

#ifdef REQHANDLE_TIMER
// ---------------------------------------------------------------------------
// CMmONStoreTsy::SetTypeOfResponse
// Sets the type of response for a given Handle
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::SetTypeOfResponse(
    const TInt aReqHandleType,
    const TTsyReqHandle aTsyReqHandle )
    {
    TInt timeOut( 0 );

    //example switch
    switch ( aReqHandleType )
        {

        case EMultimodeONStoreRead:
            timeOut = KMmONStoreReadTimeOut;
            break;
        case EMultimodeONStoreWrite:
            // Set time for writing first empty location
            if ( iIsWriteCancellingPossible )
                {
                timeOut = KMmONStoreWriteToFirstEmptyLocTimeOut;
                }
            // Set time for normal write request
            else
                {
                timeOut = KMmONStoreWriteTimeOut;
                }
            break;
        case EMultimodeONStoreDelete:
            timeOut = KMmONDeleteTimeOut;
            break;
        case EMultimodeONStoreDeleteAll:
            timeOut = KMmONDeleteAllTimeOut;
            break;
        case EMultimodeONStoreGetInfo:
            timeOut = KMmONGetInfoTimeOut;
            break;
           case EMultimodeONStoreReadAll:
        case EMultimodeONStoreStoreAll:
            timeOut = KMmONStoreAllTimeOut;
            break;

        //Must not use timer:
        // - all notifications
            //case EMultimodePhoneStoreNotifyStoreEvent:
        // - 2nd phase methods (when two phase lists are used)
            //case EMultimodeONStoreReadAll:

        default:
            //does not use timer
            iTsyReqHandleStore->SetTsyReqHandle(
                aReqHandleType,
                aTsyReqHandle );
            break;
        }

    if ( 0 < timeOut )
        {
        //the timeout parameter is given in seconds.
        iTsyReqHandleStore->SetTsyReqHandle(
            aReqHandleType,
            aTsyReqHandle,
            timeOut );
        }
    }

// ---------------------------------------------------------------------------
// CMmONStoreTsy::Complete
// Completes the request due timer expiration
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmONStoreTsy::Complete(
    TInt aReqHandleType,
    TInt aError )
    {
TFLOGSTRING3( "TSY: CMmONStoreTsy::Complete - ReqHandleType: %d Error: %d", aReqHandleType, aError );    
    //All possible TSY req handle types are listed in the
    //switch case below.
    switch( aReqHandleType )
        {
        case EMultimodeONStoreRead:
            CompleteRead( aError, NULL );
            break;
        case EMultimodeONStoreWrite:
            CompleteWrite( aError, NULL );
            break;
        case EMultimodeONStoreDelete:
            CompleteDelete( aError );
            break;
        case EMultimodeONStoreDeleteAll:
            CompleteDeleteAll( aError );
            break;
        case EMultimodeONStoreGetInfo:
            CompleteGetInfo( aError, 0 );
            break;
        case EMultimodeONStoreStoreAll:
            CompleteStoreAll( aError );
            break;

        //Can't use timer:
        // - all notifications
            //case EMultimodeONStoreNotifyStoreEvent:
        // - 2nd phase methods (when two phase lists are used)
            //case EMultimodeONStoreReadAll:

        default:
            ReqCompleted( iTsyReqHandleStore->ResetTsyReqHandle(
                aReqHandleType ), aError );
            break;
        }
    }

#endif

//  End of File
