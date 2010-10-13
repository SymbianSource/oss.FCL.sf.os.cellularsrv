// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Panic codes for mbufgobbler layer
//

/**
 @file
 @internalComponent
*/

#include <e32std.h>

#define ASSERT_DEBUG(exp, pcode) __ASSERT_DEBUG(exp, User::Panic(KMbufGobblerLayerPanic, pcode))

_LIT(KMbufGobblerLayerPanic, "MbufGobblerLayer");

enum TMbufGobblerLayerPanic
	{
	EExpectedAccessPointAwareSystem = 1, //copied from CMbufGobblerProviderSelector not sure what this means yet.
	EUnexpectedTSelectionPrefsType,
	EInvalidNullPtr
	};
