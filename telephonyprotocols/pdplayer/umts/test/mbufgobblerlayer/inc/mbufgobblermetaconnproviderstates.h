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
// Meta Connection Provider Mesh States
//  (management plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERMETACONNPROVIDERMESHSTATES_H
#define MBUFGOBBLERMETACONNPROVIDERMESHSTATES_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CMbufGobblerMetaConnectionProviderStates
 * 
 */
class CMbufGobblerMetaConnectionProviderStates : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CMbufGobblerMetaConnectionProviderStates();

	/**
	 * Two-phased constructor.
	 */
	static CMbufGobblerMetaConnectionProviderStates* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CMbufGobblerMetaConnectionProviderStates* NewLC();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMbufGobblerMetaConnectionProviderStates();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // MBUFGOBBLERMETACONNPROVIDERMESHSTATES_H
