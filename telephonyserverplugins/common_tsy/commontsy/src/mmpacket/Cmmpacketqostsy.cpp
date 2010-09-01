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
#include "Cmmpacketservicetsy.h"
#include "Cmmpacketqostsy.h"
#include "Cmmpacketcontexttsy.h"
#include "cmmpacketqosgsmwcdmaext.h"
#include "MmTsy_timeoutdefs.h"
#include "MmTsy_numberOfSlots.h"
#include <ctsy/serviceapi/mmtsy_ipcdefs.h>
#include <ctsy/pluginapi/cmmdatapackage.h>

// ======== MEMBER FUNCTIONS ========

CMmPacketQoSTsy::CMmPacketQoSTsy():
    iReqHandleType(EMultimodePacketQoSReqHandleUnknown)
    {
    }

CMmPacketQoSTsy* CMmPacketQoSTsy::NewL(
        CMmPacketServiceTsy* aMmPacketService,
        CMmPacketContextTsy* aMmPacketContext ) 
    {
TFLOGSTRING("TSY: CMmPacketQoSTsy::NewL." );

    CMmPacketQoSTsy* mmPacketQoS = new ( ELeave ) CMmPacketQoSTsy();
    CleanupClosePushL( *mmPacketQoS );
    mmPacketQoS->iMmPacketService = aMmPacketService;
    mmPacketQoS->iMmPacketContext = aMmPacketContext;
    mmPacketQoS->ConstructL();
    CleanupStack::Pop( mmPacketQoS );

    return mmPacketQoS;
    }


void CMmPacketQoSTsy::ConstructL()
    {  

    InitExtensionModulesL();

#ifdef REQHANDLE_TIMER   
    // Create req handle store
    iTsyReqHandleStore = CMmTsyReqHandleStore::NewL( this,
        iMmPacketService->PhoneTsy(), EMaxNumOfPacketQoSRequests,
        iPacketQoSReqHandles );
#else
    // Create req handle store
    iTsyReqHandleStore = CMmTsyReqHandleStore::NewL( 
        EMaxNumOfPacketQoSRequests, iPacketQoSReqHandles );

#endif // REQHANDLE_TIMER

    }

CMmPacketQoSTsy::~CMmPacketQoSTsy()
    {
TFLOGSTRING("TSY: CMmPacketQoSTsy::~CMmPacketQoSTsy." );
    if( iMmPacketContext )
	    {
	    iMmPacketContext->RemoveQoS();	
	    }
    
    delete iMmPacketQoSGsmWcdmaExt;
    delete iTsyReqHandleStore;
    }



//----------------------------------------------------------------------------
// CMmPacketQoSTsy::InitExtensionModulesL
// Initialises extension module for CMmPacketQoSTsy
// create and allocate memory for iMmPacketQoSGsmWcdmaExt
//----------------------------------------------------------------------------

void CMmPacketQoSTsy::InitExtensionModulesL()
    {

    // Create Packet QoS GSM/WCDMA extension
    iMmPacketQoSGsmWcdmaExt = 
        CMmPacketQoSGsmWcdmaExt::NewL( this, iMmPacketService );

    }


//---------------------------------------------------------------------------
// CMmPacketQoSTsy::Init
// Initialisation method that is called from ETel Server.
//---------------------------------------------------------------------------

void CMmPacketQoSTsy::Init()
    {
    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::ExtFunc 
// Handles the ETel server requests.
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::ExtFunc( 
        const TTsyReqHandle aTsyReqHandle, 
        const TInt aIpc,     
        const TDataPackage& aPackage )      
    {
TFLOGSTRING2("TSY: CMmPacketQoSTsy::ExtFunc. aIpc: %d", aIpc );
    TInt ret( KErrNone );
    TInt trapError( KErrNone );

    // Ensure the ReqHandleType is unset.
    // This will detect cases where this method indirectly calls itself
    // (e.g. servicing a client call that causes a self-reposting notification to complete and thus repost).
    // Such cases are not supported because iReqHandleType is in the context of this class instance,
    // not this request, and we don't want the values set by the inner request and the outer request
    // interfering with each other.
    __ASSERT_DEBUG(iReqHandleType==EMultimodePacketQoSReqHandleUnknown, User::Invariant());

    TRAP( trapError, ret = DoExtFuncL( aTsyReqHandle, aIpc, aPackage ); );

    if ( KErrNone != trapError )
        {
        CMmPacketQoSTsy::ReqCompleted( aTsyReqHandle, trapError );
        }
    else if ( KErrNone != ret )
        {
        CMmPacketQoSTsy::ReqCompleted( aTsyReqHandle, ret );
        }

    // Save request handle
    if ( EMultimodePacketQoSReqHandleUnknown != iReqHandleType )
        {
#ifdef REQHANDLE_TIMER
        SetTypeOfResponse( iReqHandleType, aTsyReqHandle );
#else
        iTsyReqHandleStore->SetTsyReqHandle( iReqHandleType, aTsyReqHandle );
#endif //REQHANDLE_TIMER
        // We've finished with this value now. Clear it so it doesn't leak
        //  up to any other instances of this method down the call stack
        iReqHandleType = EMultimodePacketQoSReqHandleUnknown;
        }

    return KErrNone;

    }

//----------------------------------------------------------------------------
// CMmPacketQoSTsy::DoExtFuncL    
// DoExtFuncL calls Packet QoS related methods 
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::DoExtFuncL( 
        const TTsyReqHandle aTsyReqHandle, 
        const TInt aIpc,        
        const TDataPackage& aPackage )  
    {
TFLOGSTRING3("TSY: CMmPacketQoSTsy::DoExtFuncL. IPC: %d  Handle:%d", aIpc, aTsyReqHandle);
    TInt ret( KErrNotSupported );

    switch ( aIpc )
        {
        case EPacketQoSSetProfileParams:
            ret = SetProfileParametersL( aTsyReqHandle, aPackage.Des1n() );
            break;
        case EPacketQoSGetProfileParams:
            ret = GetProfileParameters( aTsyReqHandle, aPackage.Des1n() );
            break;
        case EPacketQoSGetProfileCaps:
            ret = GetProfileCapabilities( aTsyReqHandle, aPackage.Des1n() );
            break;
        case EPacketQoSNotifyProfileChanged:
            ret = NotifyProfileChanged( aPackage.Des1n() );
            break;
        default:
            break;
        }
    return ret;

    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::ReqModeL
// Returns request mode for given IPC.
//----------------------------------------------------------------------------

CTelObject::TReqMode CMmPacketQoSTsy::ReqModeL(    
        const TInt aIpc ) 
    {
TFLOGSTRING2("TSY: CMmPacketQoSTsy::ReqModeL. IPC: %d", aIpc );
    CTelObject::TReqMode reqMode( 0 );

    switch ( aIpc )
        {
        case EPacketQoSSetProfileParams:
        case EPacketQoSGetProfileParams:
        case EPacketQoSGetProfileCaps:
            reqMode = KReqModeFlowControlObeyed;
            break;

        case EPacketQoSNotifyProfileChanged:
            reqMode = KReqModeMultipleCompletionEnabled |
                      KReqModeRePostImmediately;
            break;

        // Leave with KErrNotSupported
        default:
            User::Leave( KErrNotSupported );
            break;
        }
    return reqMode;
    }


//----------------------------------------------------------------------------
//CMmPacketQoSTsy::NumberOfSlotsL
// Returns number of slots to be used for given IPC.
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::NumberOfSlotsL(    
        const TInt aIpc ) 
    {

TFLOGSTRING2("TSY: CMmPacketQoSTsy::NumberOfSlotsL. IPC: %d", aIpc );

    TInt numberOfSlots( 0 );

    if ( EPacketQoSNotifyProfileChanged == aIpc )
        {
        numberOfSlots = KMmPacketQoSNotifyParameterChangedSlots;
        }
    else
        {
        // Unknown or invalid IPC
        User::Leave( KErrNotSupported );
        }

    return numberOfSlots;

    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::CancelService
// Cancels request defined by aIpc.
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::CancelService(    
        const TInt aIpc,   
        const TTsyReqHandle aTsyReqHandle ) 
    {
TFLOGSTRING3("TSY: CMmPacketQoSTsy::CancelService. IPC: %d  Handle:%d", aIpc, aTsyReqHandle);
     TInt ret( KErrNone );

     switch ( aIpc )
        {
        case EPacketQoSSetProfileParams:
        case EPacketQoSGetProfileParams:
        case EPacketQoSGetProfileCaps:
            break;

        // Notification cancels
        case EPacketQoSNotifyProfileChanged:
            // Reset the GSM/WCDMA extension from previous notify request
            iMmPacketQoSGsmWcdmaExt->NotifyProfileChangedCancel();
            
            // Reset tsy request handle
            iTsyReqHandleStore->ResetTsyReqHandle( 
                EMultimodePacketQoSNotifyParameterChanged );

            // Complete request with KErrCancel
            CMmPacketQoSTsy::ReqCompleted( aTsyReqHandle, KErrCancel );

            break;
        // Default case
        default:
            ret = KErrNotSupported;
            break;
        }

    return ret;
    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::RegisterNotification
// Registers Notification
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::RegisterNotification(    
        const TInt aIpc ) 
    {
TFLOGSTRING2("TSY: CMmPacketQoSTsy::RegisterNotification. IPC: %d", aIpc );

    if ( EPacketQoSNotifyProfileChanged == aIpc )
        {
        return KErrNone;
        }
    return KErrNotSupported;
    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::DeregisterNotification
// This function unsubscribe given notification from Manufacturer specific 
// part.Currently does not really do anything but returns KErrNone to 
// ETel server in case of known notification request type.
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::DeregisterNotification(
        const TInt aIpc )    
    {
TFLOGSTRING2("TSY: CMmPacketQoSTsy::DeregisterNotification. IPC: %d", aIpc );

    if ( EPacketQoSNotifyProfileChanged == aIpc )
        {
        return KErrNone;
        }
    return KErrNotSupported;
    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::OpenNewObjectL
// Creates new object and returns a pointer to it.
// leave with KErrNotSupported
// return NULL and control to calling object
//----------------------------------------------------------------------------

CTelObject* CMmPacketQoSTsy::OpenNewObjectL(    
        TDes& /* aNewName*/ ) 
    {
TFLOGSTRING("TSY: CMmPacketQoSTsy::OpenNewObjectL. Leaves with:KErrNotSupported" );
    User::Leave( KErrNotSupported );
    return NULL;
    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::OpenNewObjectByNameL
// Creates new object and returns a pointer to it.   
// The ETel Server calls this method. TSY classes must not use this method.
// leave with KErrNotSupported
// return NULL and control to calling object
//----------------------------------------------------------------------------

CTelObject* CMmPacketQoSTsy::OpenNewObjectByNameL(    
        const TDesC& /* aName */ )  
    {
TFLOGSTRING("TSY: CMmPacketQoSTsy::OpenNewObjectByNameL. Leaves with:KErrNotSupported" );
    // Otherwise we can leave...
    User::Leave( KErrNotSupported );
    return NULL;
    }

//----------------------------------------------------------------------------
// CMmPacketQoSTsy::PacketQoSGsmWcdmaExt
// Return pointer to the PacketQoSGsmWcdmaExt.
//----------------------------------------------------------------------------

CMmPacketQoSGsmWcdmaExt* CMmPacketQoSTsy::PacketQoSGsmWcdmaExt()
    {
    return iMmPacketQoSGsmWcdmaExt;
    }

//----------------------------------------------------------------------------
// CMmPacketQoSTsy::GetProfileCapabilities
// Returns the QoS capabilities for the current packet service.
// The TSY will assign a bitmask for each parameter in the relevant TQoSCaps
// class. This bitmask contains all values which can be assigned to the 
// particular parameter for the current packet service & phone.  The bitmask 
// values will conform to values stated in the QoS-related enum data types 
// defined in ETelQoS.h
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::GetProfileCapabilities(
        const TTsyReqHandle aTsyReqHandle,   
        TDes8* aProfileCaps )   
    {
TFLOGSTRING3("TSY: CMmPacketQoSTsy::GetProfileCapabilities. TsyReqHandle: %d  aProfileCaps: %d", aTsyReqHandle, aProfileCaps );
     
    // Call GetProfileCapabilities from GSM/WCDMA extension
    TInt ret = iMmPacketQoSGsmWcdmaExt->GetProfileCapabilities( 
        aProfileCaps );
   
    // Complete the request
    CMmPacketQoSTsy::ReqCompleted( aTsyReqHandle, ret );
    return KErrNone;
    }
        

//----------------------------------------------------------------------------
//CMmPacketQoSTsy::GetProfileParameters
// Retrieves all parameters associated with a negotiated profile.
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::GetProfileParameters(
        const TTsyReqHandle aTsyReqHandle,
        TDes8* aProfile )      
    {
TFLOGSTRING3("TSY: CMmPacketQoSTsy::GetProfileParameters. TsyReqHandle: %d   aProfile: %d", aTsyReqHandle, aProfile );
  
    // Call GetProfileParameters method from GSM/WCDMA extension
    TInt ret = iMmPacketQoSGsmWcdmaExt->GetProfileParameters( aProfile );

    // Complete the request
    CMmPacketQoSTsy::ReqCompleted( aTsyReqHandle, ret );

    return KErrNone;

    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::NotifyProfileChanged
// Notifies of the change in the negotiated QoS profile.
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::NotifyProfileChanged(
        TDes8* aProfile )  
    {
TFLOGSTRING2("TSY: CMmPacketQoSTsy::NotifyProfileChanged. aProfile: %d", aProfile );

    // Set Req Handle
    iReqHandleType = EMultimodePacketQoSNotifyParameterChanged;

    // Call NotifyProfileChanged-method from GSM/WCDMA extension
    return iMmPacketQoSGsmWcdmaExt->NotifyProfileChanged( aProfile );

    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::CompleteNotifyProfileChanged
// Completes the notification to client.
//----------------------------------------------------------------------------

void CMmPacketQoSTsy::CompleteNotifyProfileChanged()
    {
TFLOGSTRING("TSY: CMmPacketQoSTsy::CompleteNotifyProfileChanged." );
   
    // Reset tsy request handle. 
    TTsyReqHandle reqHandle( iTsyReqHandleStore->ResetTsyReqHandle( 
        EMultimodePacketQoSNotifyParameterChanged ) );

    if ( EMultimodePacketQoSReqHandleUnknown != reqHandle )
        {

        // Complete the client request
        CMmPacketQoSTsy::ReqCompleted( reqHandle, KErrNone );

        }
    }

//----------------------------------------------------------------------------
// CMmPacketQoSTsy::SetProfileParametersL
// Allows client pass to the TSY required parameters
//----------------------------------------------------------------------------

TInt CMmPacketQoSTsy::SetProfileParametersL(
        const TTsyReqHandle aTsyReqHandle, 
        TDes8* aProfile )                
    {
TFLOGSTRING3("TSY: CMmPacketQoSTsy::SetProfileParametersL. TsyReqHandle: %d  aProfile: %d", aTsyReqHandle, &aProfile );

    // Call SetProfileParametersL from GSM/WCDMA extension
    TInt ret = iMmPacketQoSGsmWcdmaExt->SetProfileParametersL( aProfile );

    RPacketContext::TContextStatus contextStatus =
        CMmPacketQoSTsy::PacketContextTsy()->ContextStatus();

    if ( KErrNone == ret )
        {

        if ( RPacketContext::EStatusActivating != contextStatus &&
             RPacketContext::EStatusActive != contextStatus)
            {

            // Store the request handle type 
            iReqHandleType = EMultimodePacketQoSSetProfileParameters;

            }

        else
            {

            CMmPacketQoSTsy::ReqCompleted( aTsyReqHandle, KErrNone );

            }
        }

    return ret;

    }

//----------------------------------------------------------------------------
// CMmPacketQoSTsy::CompleteSetProfileParameters
// Complete Set Profile Parameters
//----------------------------------------------------------------------------

void CMmPacketQoSTsy::CompleteSetProfileParameters(
        const TInt aError )     
    {
TFLOGSTRING2("TSY: CMmPacketQoSTsy::CompleteSetProfileParameters. Error: %d", aError );

    // Reset the request handle. 
    TTsyReqHandle reqHandle( iTsyReqHandleStore->ResetTsyReqHandle(
        EMultimodePacketQoSSetProfileParameters ) );

    if ( EMultimodePacketQoSReqHandleUnknown != reqHandle )
        {

        // Complete the client request
        CMmPacketQoSTsy::ReqCompleted( reqHandle, aError );

        }

    }


//----------------------------------------------------------------------------
// CMmPacketQoSTsy::PacketContextTsy
// Return pointer to the CMmPacketContextTsy.
//----------------------------------------------------------------------------

CMmPacketContextTsy* CMmPacketQoSTsy::PacketContextTsy()
    {
    return iMmPacketContext;
    }

#ifdef REQHANDLE_TIMER

//----------------------------------------------------------------------------
// CMmPacketQoSTsy::SetTypeOfResponse
// Sets the type of response for a given Handle.    
// Automatic mode includes an automatic response in case of non response from
// the manufacturer specific layer in a specified time.
//----------------------------------------------------------------------------

void CMmPacketQoSTsy::SetTypeOfResponse(
        const TInt aReqHandleType,    
        const TTsyReqHandle aTsyReqHandle )    
    {

    if ( EMultimodePacketQoSSetProfileParameters == aReqHandleType )
        {
        // The timeout parameter is given in seconds.
        iTsyReqHandleStore->SetTsyReqHandle( aReqHandleType, aTsyReqHandle,
			KMmPacketQoSSetProfileParametersTimeOut );
        }
    else 
        {
        // Does not use timer
        iTsyReqHandleStore->SetTsyReqHandle( aReqHandleType, aTsyReqHandle );
        }

    }

//----------------------------------------------------------------------------
// CMmPacketQoSTsy::Complete
// Completes the request due timer expiration.
//----------------------------------------------------------------------------

void CMmPacketQoSTsy::Complete(
        const TInt aReqHandleType,    
        const TInt aError )        
    {

    if ( EMultimodePacketQoSSetProfileParameters == aReqHandleType )
        {
        CMmPacketQoSTsy::CompleteSetProfileParameters( aError );
        }
    else
        {
        CMmPacketQoSTsy::ReqCompleted( iTsyReqHandleStore->ResetTsyReqHandle( 
                            aReqHandleType ), aError );
        }

    }
#endif //REQHANDLE_TIMER

#ifdef TF_LOGGING_ENABLED

//----------------------------------------------------------------------------
// CMmPacketQoSTsy::ReqCompleted
// Description: Overloads CTelObject::ReqCompleted for logging purposes.
// calls CTelObject::ReqCompleted.
//----------------------------------------------------------------------------

void CMmPacketQoSTsy::ReqCompleted(
        const TTsyReqHandle aTsyReqHandle,
        const TInt aError )     
       
    {
TFLOGSTRING3("TSY: CMmPacketQoSTsy::Request Completed.  Handle: %d Error:%d", aTsyReqHandle, aError ); 

    CTelObject::ReqCompleted( aTsyReqHandle, aError );  
    }

#endif //TF_LOGGING_ENABLED


//  End of File 
