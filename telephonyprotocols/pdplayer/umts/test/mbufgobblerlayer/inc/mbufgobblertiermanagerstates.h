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
// Tier Manager Mesh States
//   (management plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERTIERMANAGERSTATES_H
#define MBUFGOBBLERTIERMANAGERSTATES_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CMbufGobblerTierManagerMeshStates
 * 
 */
class CMbufGobblerTierManagerMeshStates : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CMbufGobblerTierManagerMeshStates();

	/**
	 * Two-phased constructor.
	 */
	static CMbufGobblerTierManagerMeshStates* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CMbufGobblerTierManagerMeshStates* NewLC();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMbufGobblerTierManagerMeshStates();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // MBUFGOBBLERTIERMANAGERSTATES_H
