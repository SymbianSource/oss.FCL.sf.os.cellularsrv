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

#include "mbufgobblersubconnproviderstates.h"
#include "mbufgobblerlog.h"

CMbufGobblerSubconnMeshStates::CMbufGobblerSubconnMeshStates()
	{
	// No implementation required
	}

CMbufGobblerSubconnMeshStates::~CMbufGobblerSubconnMeshStates()
	{
	}

CMbufGobblerSubconnMeshStates* CMbufGobblerSubconnMeshStates::NewLC()
	{
	CMbufGobblerSubconnMeshStates* self =
			new (ELeave) CMbufGobblerSubconnMeshStates();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMbufGobblerSubconnMeshStates* CMbufGobblerSubconnMeshStates::NewL()
	{
	CMbufGobblerSubconnMeshStates* self =
			CMbufGobblerSubconnMeshStates::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CMbufGobblerSubconnMeshStates::ConstructL()
	{

	}
