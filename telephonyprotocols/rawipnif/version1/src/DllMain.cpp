// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Defines the global functions which provide DLL entry points.
// 
//

/**
 @file
*/

#include "RawIPNifMainFactory.h"


extern "C"
    {
    IMPORT_C CNifFactory* NewInterfaceFactoryL();	// Force export
    }

EXPORT_C CNifFactory* NewInterfaceFactoryL()
/**
 * Creates a new RAW IP NIF main factory. This must be the first exported function
 * in the NIF's DLL.
 *
 * @return A newly created RAW IP NIF main factory
 */
	{
	return new (ELeave) CRawIPNifMainFactory;
	}
