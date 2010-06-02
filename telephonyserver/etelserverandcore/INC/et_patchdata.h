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
//

/**
 @file
 @internalComponent
*/

#ifndef __ETEL_PATCHDATA_H__
#define __ETEL_PATCHDATA_H__

#include <e32def.h>


/** Priority USSD client application SID
@SYMPatchable
@publishedPartner
@released
*/
extern const TInt32 KUssdPriorityClientSid;

/** Default USSD client application SID
@SYMPatchable
@publishedPartner
@released
*/
extern const TInt32 KUssdDefaultClientSid;

/** Total timeout value for offering USSD dialogue to interested clients.
@SYMPatchable
@publishedPartner
@released
*/
extern const TInt32 KUssdOfferDialogueTimeout;

/** Priority Timeout Percentage Value
@SYMPatchable
@publishedPartner
@released
*/
extern const TInt32 KUssdPriorityTimeoutPercentage;

#endif
