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
// Implements panic and e32dll functions for SMSU
// 
//

/**
 @file
*/

#include "smsumain.h"

#include "smsstacklog.h"

/**
 *  panic in SMSU
 */
GLDEF_C void SmsuPanic(TSmsuPanic aPanic)
    {
    // Ignore in code coverage  - panic method
    BULLSEYE_OFF
    LOGSMSU2("SMSUPANIC %d", aPanic);
    _LIT(KSmsuPanic, "SMSU");
    User::Panic(KSmsuPanic,aPanic);
    BULLSEYE_RESTORE
    }
