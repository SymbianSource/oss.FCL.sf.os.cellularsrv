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
// Name        : TSatEventList.cpp
// Part of     : Common SIM ATK TSY / commonsimatktsy
// Event list - related functionality of Sat Tsy
// Version     : 1.0
//



#include "TSatEventList.h"   // Class header
#include "TfLogger.h"        // For TFLOGSTRING

// -----------------------------------------------------------------------------
// TSatEventList::TSatEventList
// TSatEventList C++ constructor
// -----------------------------------------------------------------------------
//
TSatEventList::TSatEventList
        (
        void 
        ) : iEvents( 0 )
    {
    TFLOGSTRING( "CSAT: TSatEventList::TSatEventList" );
    }

// -----------------------------------------------------------------------------
// TSatEventList::IsEventEnabled
// Check if event is ín the set of enabled events
// -----------------------------------------------------------------------------
//
TBool TSatEventList::IsEnabled
        (
        RSat::TEventList aEvent
        ) const
    {

    if( iEvents & TUint32( aEvent ) )
        {
        TFLOGSTRING2( "CSAT: TSatEventList::IsEventEnabled, \
            Event Enabled: %d", aEvent );
        return ETrue;
        }
    else
        {
        TFLOGSTRING( "CSAT: TSatEventList::IsEventEnabled, \
            Event Not enabled" );
        return EFalse;
        }

    }

// -----------------------------------------------------------------------------
// TSatEventList::RemoveFromEventList
// Remove event from set of enabled events
// -----------------------------------------------------------------------------
//
void TSatEventList::RemoveFromEventList
        (
        RSat::TEventList aEvent
        )
    {
    TFLOGSTRING( "CSAT: TSatEventList::RemoveFromEventList" );
    iEvents &= ~TUint32( aEvent );
    }

// -----------------------------------------------------------------------------
// TSatEventList::AddToEventList
// Add event to set of enabled events
// -----------------------------------------------------------------------------
//
void TSatEventList::AddToEventList
        (
        RSat::TEventList aEvent
        )
    {
    TFLOGSTRING( "CSAT: TSatEventList::AddToEventList" );
    iEvents |= TUint32( aEvent );
    }

// -----------------------------------------------------------------------------
// TSatEventList::SetEventList
// Set the list of enabled events
// -----------------------------------------------------------------------------
//
void TSatEventList::SetEventList
        (
        TUint32 aEvent
        )
    {
    TFLOGSTRING2( "CSAT: TSatEventList::SetEventList aEvent:%d", aEvent );
    iEvents = aEvent;
    }

// End of File
