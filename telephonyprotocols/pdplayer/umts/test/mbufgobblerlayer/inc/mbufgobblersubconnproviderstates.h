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
// Connection Provider Declaration
//  (control plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERSUBCONNPROVIDERSTATES_H
#define MBUFGOBBLERSUBCONNPROVIDERSTATES_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CMbufGobblerSubconnMeshStates
 * 
 */
class CMbufGobblerSubconnMeshStates : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CMbufGobblerSubconnMeshStates();

	/**
	 * Two-phased constructor.
	 */
	static CMbufGobblerSubconnMeshStates* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CMbufGobblerSubconnMeshStates* NewLC();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMbufGobblerSubconnMeshStates();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // MBUFGOBBLERSUBCONNPROVIDERSTATES_H
