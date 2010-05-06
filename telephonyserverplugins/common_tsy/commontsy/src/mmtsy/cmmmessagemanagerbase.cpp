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
#include "cmmmessagemanagerbase.h"
#include <ctsy/tflogger.h>
#include "ctsydelegates.h"

// ======== MEMBER FUNCTIONS ========

EXPORT_C CMmMessageManagerBase* CMmMessageManagerBase::NewL()
    {
    CMmMessageManagerBase* messageManager = NULL;
    messageManager = new ( ELeave ) CMmMessageManagerBase();
    CleanupStack::PushL( messageManager );
     messageManager->ConstructL();
    CleanupStack::Pop();
    return messageManager;
    }

CMmMessageManagerBase::~CMmMessageManagerBase()
    {
    // instance was created in this object, so we delete it here
    if ( iTsyDelgates )
        {
        delete iTsyDelgates;
        }
    iTsyDelgates = NULL;
    
    if ( iMessageRouterProxy )
        {
        delete iMessageRouterProxy;
        }
    iMessageRouterProxy = NULL;
    }

CMmMessageManagerBase::CMmMessageManagerBase()
    {
    }

void CMmMessageManagerBase::ConstructL()
    {
    //Create instances
    iTsyDelgates = CTsyDelegates::NewL();
    iMessageRouterProxy = CMmMessageRouterProxy::NewL( *iTsyDelgates );
    }
    
// ---------------------------------------------------------------------------
// CMmMessageManagerBase::SetPhoneTsy
// Set phone Tsy object
// (other items were commented in a header).
// ---------------------------------------------------------------------------    
EXPORT_C void CMmMessageManagerBase::SetPhoneTsy( CMmPhoneTsy* aMmPhone )    
    {
TFLOGSTRING2("TSY: CMmMessageManagerBase::SetPhoneTsy : Phone=0x%x", aMmPhone );   
    iMmPhone = aMmPhone;
    
    if( iMessageRouterProxy )
        {
        iMessageRouterProxy->SetPhoneTsy( iMmPhone );    
        }
    }
// ---------------------------------------------------------------------------
// CMmMessageManagerBase::HandleRequestL
// Forwards a request to DOS layer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmMessageManagerBase::HandleRequestL(    
    const TInt aIpc )        
    {
TFLOGSTRING2("TSY: CMmMessageManagerBase::HandleRequestL. IPC = %d", aIpc );
    return HandleRequestL( 
        aIpc, REINTERPRET_CAST( const CMmDataPackage*, NULL ) );
    }

// ---------------------------------------------------------------------------
// CMmMessageManagerBase::HandleRequestL
// Forwards a request to DOS layer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmMessageManagerBase::HandleRequestL(    
    const TInt aIpc,      
    const CMmDataPackage* aDataPackage )
    {
TFLOGSTRING2("TSY: CMmMessageManagerBase::HandleRequestL. IPC = %d", aIpc );
   
    return iMessageRouter->ExtFuncL( aIpc, aDataPackage );
    }

// ---------------------------------------------------------------------------
// CMmMessageManagerBase::HandleRequestL
// Forwards a request to DOS layer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmMessageManagerBase::HandleRequestL(    
    const TInt aIpc,        
    const TDataPackage* aDataPackage )
    {
TFLOGSTRING2("TSY: CMmMessageManagerBase::HandleRequestL. IPC = %d", aIpc );

    // map the TDataPackage into a CMmDataPackage
    // Non-core Methods from the SOS layer using the DOS layer will perforn
    // always this mapping.
    // TSY core methods will call directly the overloaded HandleRequest method 
    // which uses a CMmDataPackage in the method's signature.
    iDataPackage.SetPacketData ( aDataPackage );

    //call the overloaded HandleRequest of CMmMessageManagerBase
    return HandleRequestL( aIpc, &iDataPackage );
    }

// ---------------------------------------------------------------------------
// CMmMessageManagerBase::RegisterTsyObject
// Registers a Tsy object for request and notification completions
// Stored in Tsy Delegates
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmMessageManagerBase::RegisterTsyObject( 
    TTsyObjects aTsyObjectType,
    CBase* aTsyObject )
    {
    iTsyDelgates->RegisterTsyObject( aTsyObjectType, aTsyObject );
    }

// ---------------------------------------------------------------------------
// CMmMessageManagerBase::DeregisterTsyObject
// Remove registered Tsy object stored in Tsy Delegates
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CMmMessageManagerBase::DeregisterTsyObject(
    CBase* aTsyObject )
    {
    iTsyDelgates->DeregisterTsyObject( aTsyObject );
    }
    
// ---------------------------------------------------------------------------
// CMmMessageManagerBase::GetMessageManagerCallback
// Return pointer to MessageManagerCallback object
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C MmMessageManagerCallback* 
    CMmMessageManagerBase::GetMessageManagerCallback()    
    {
    return iMessageRouterProxy;
    }

    
// ======== OTHER EXPORTED FUNCTIONS ========

// ---------------------------------------------------------------------------
// CMmMessageManagerBase::SetMessageRouter
// Sets message router ptr.
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CMmMessageManagerBase::SetMessageRouter(
    MMessageRouter* aMessageRouter )
    {
TFLOGSTRING2("TSY: CMmMessageManagerBase::SetMessageRouter : MessageRouter=0x%x", aMessageRouter );
    iMessageRouter = aMessageRouter;
    }


//  End of File 

