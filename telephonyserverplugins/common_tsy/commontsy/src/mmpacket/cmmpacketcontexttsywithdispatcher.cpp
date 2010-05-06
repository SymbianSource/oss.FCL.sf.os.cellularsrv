// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifdef USING_CTSY_DISPATCHER

/**
 * This file contains additional function definitions for the CMmPacketContextTsy class
 * in CTSY for use when the CTSY is used with the CTSY Dispatcher. 
 */

// INCLUDE FILES

#include "cmmpacketservicegsmwcdmaext.h"

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// CMmPacketContextTsy::RemovePacketFilter
// This method removes a packet filter from the TFT belonging to this context.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmPacketContextTsy::RemovePacketFilter(
        TInt *aID )    
    {
TFLOGSTRING2( "TSY: CMmPacketContextTsy::RemovePacketFilter. RemovedFilter:%d", *aID );

    // id must be value from 1 to 8
    if ( 0 < *aID && 8 >= *aID )
        {
        iMmPacketContextGsmWcdmaExt->RemovePacketFilter( *aID );
        }

    // Operation is context configuration and complete method handles request complete
    iReqHandleType = EMultimodePacketContextRemovePacketFilter;


    return KErrNone;
    }

#endif //USING_CTSY_DISPATCHER	
	
//  End of File 
