// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implements the GSMU panic function and E32Dll
// 
//

/**
 @file
*/

#include <e32std.h>
#include "Gsmumain.h"
#include "Gsmuelem.h"


/**
 *  panic in the GSMU
 */
GLDEF_C void Panic(TGsmuPanic aPanic)
    {
    // Ignore in code coverage - panic method
    BULLSEYE_OFF    
    LOGGSMU2("GSMUPANIC %d", aPanic);
    
    _LIT(KGsmuPanic, "GSMU");
    User::Panic(KGsmuPanic, aPanic);
    BULLSEYE_RESTORE
    }
