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
#include "ctsydelegates.h"
#include "cmmmessagemanagerbase.h"
#include <ctsy/tflogger.h> 

// ======== MEMBER FUNCTIONS ========

CTsyDelegates* CTsyDelegates::NewL()
    {
    CTsyDelegates* tsyDelegates = new( ELeave ) CTsyDelegates();
    return tsyDelegates;
    }
    
CTsyDelegates::CTsyDelegates()
    {	
    }
    
 CTsyDelegates::~CTsyDelegates()
    {
    }
// ---------------------------------------------------------------------------
// CTsyDelegates::GetTsyObject
// Returns pointer requested TSY object
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CBase* CTsyDelegates::GetTsyObject( 
    CMmMessageManagerBase::TTsyObjects aObject )
	{
TFLOGSTRING2("TSY: CTsyDelegates::GetTsyObject object=%d", aObject );
	
	CBase* object = NULL;	
 	object = iTsyObjectArray.At( aObject );	
 	return object ; 	
	}

// ---------------------------------------------------------------------------
// CTsyDelegates::RegisterTsyObject
// Registers a Tsy object for request and notification completions
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CTsyDelegates::RegisterTsyObject(
    CMmMessageManagerBase::TTsyObjects aTsyObjectType,
    CBase* aTsyObject )
    {
TFLOGSTRING3("TSY: CTsyDelegates::RegisterTsyObject type=%d address=0x%x", TInt( aTsyObjectType ), aTsyObject );

    iTsyObjectArray[ aTsyObjectType ] = aTsyObject;
    }

// ---------------------------------------------------------------------------
// CTsyDelegates::DeregisterTsyObject
// Remove previously registered Tsy object from the array. 
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CTsyDelegates::DeregisterTsyObject( CBase* aTsyObject )
    {
TFLOGSTRING2("TSY: CTsyDelegates::DeregisterTsyObject address=0x%x", aTsyObject );

    for( TInt i = 0; i < TInt( 
        CMmMessageManagerBase::EMaxNumOfTsyObjects ); i++ )
        {
        if( iTsyObjectArray[ i ] == aTsyObject )
            {
            iTsyObjectArray[ i ] = 0;
            break;
            }
        }
    }


//  End of File
