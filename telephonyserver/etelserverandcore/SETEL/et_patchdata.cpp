// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// All patchable constants need to be defined in a separate file.
// Care should be taken not to define the constant data in the same source file in which
// they are referred, lest the compiler inline's it. If inlined, the data has no storage
// location and cannot be patched.
// Hence, the definition of data must be put in a separate source file and they must be 
// referred in other source files by using the 'extern' qualifier.
// 
//

/**
 @file
 @internalComponent
*/

#include <e32def.h>

// This constant specifies the SID of the priority client.
// The default (unpatched) value is 0.
extern const TInt32 KUssdPriorityClientSid = 0x11111111;

// This constant specifies the SID of the default client.
// The default (unpatched) value is 0.
extern const TInt32 KUssdDefaultClientSid = 0x22222222;

// This constant specifies the timeout in microseconds that a dialogue will be
// offered to registered clients. The default (unpatched) value is 1000000.
extern const TInt32 KUssdOfferDialogueTimeout = 1000000;

// This constant specifies the percentage of the total timeout which will be
// reserved for the priority client. The default (unpatched) value is 20.
extern const TInt32 KUssdPriorityTimeoutPercentage = 40;

