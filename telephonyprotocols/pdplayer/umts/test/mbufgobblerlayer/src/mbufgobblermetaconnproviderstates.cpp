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

#include "mbufgobblermetaconnproviderstates.h"
#include "mbufgobblerlog.h"

CMbufGobblerMetaConnectionProviderStates::CMbufGobblerMetaConnectionProviderStates()
	{
	// No implementation required
	}

CMbufGobblerMetaConnectionProviderStates::~CMbufGobblerMetaConnectionProviderStates()
	{
	}

CMbufGobblerMetaConnectionProviderStates* CMbufGobblerMetaConnectionProviderStates::NewLC()
	{
	CMbufGobblerMetaConnectionProviderStates* self =
			new (ELeave) CMbufGobblerMetaConnectionProviderStates();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMbufGobblerMetaConnectionProviderStates* CMbufGobblerMetaConnectionProviderStates::NewL()
	{
	CMbufGobblerMetaConnectionProviderStates* self =
			CMbufGobblerMetaConnectionProviderStates::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CMbufGobblerMetaConnectionProviderStates::ConstructL()
	{

	}
