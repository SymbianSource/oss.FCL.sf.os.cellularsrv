// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__FAXSTPAN_H__)
#define __FAXSTPAN_H__
#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

/**
@internalComponent
*/
enum TFaxStorePanic
	{
	EFaxScanLineIndexOutOfRange,
	EFaxBandIndexOutOfRange,
	EFaxPageIndexOutOfRange,
	EFaxDecodeScanlineAlignment,
	EFaxEncodeScanlineAlignment,
	EFaxUnsupportedCompression
	};

/**
@publishedPartner
*/
GLREF_C void Panic(TFaxStorePanic aPanic);

#endif // __FAXSTPAN_H__
