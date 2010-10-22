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

#include "mbufgobblertiermanagerstates.h"
#include "mbufgobblerlog.h"

CMbufGobblerTierManagerMeshStates::CMbufGobblerTierManagerMeshStates()
	{
	// No implementation required
	}

CMbufGobblerTierManagerMeshStates::~CMbufGobblerTierManagerMeshStates()
	{
	}

CMbufGobblerTierManagerMeshStates* CMbufGobblerTierManagerMeshStates::NewLC()
	{
	CMbufGobblerTierManagerMeshStates* self =
			new (ELeave) CMbufGobblerTierManagerMeshStates();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMbufGobblerTierManagerMeshStates* CMbufGobblerTierManagerMeshStates::NewL()
	{
	CMbufGobblerTierManagerMeshStates* self =
			CMbufGobblerTierManagerMeshStates::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CMbufGobblerTierManagerMeshStates::ConstructL()
	{

	}
