// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SPUD Factory
// 
//

/**
 @file
 @internalComponent
*/

#ifndef SPUDFACTORY_H
#define SPUDFACTORY_H

#include "spudman.h"
#include "bindman.h"
#include "mux.h"
#include "rpdpfsminterface.h"

/**
Factory class for SPUD
*/
NONSHARABLE_CLASS(CSpudFactory) : public CNifIfFactory
	{
public:
	~CSpudFactory();
	
protected:
	void InstallL();
	CNifIfBase* NewInterfaceL(const TDesC& aName);
	TInt Info(TNifIfInfo& aInfo, TInt aIndex) const;
	CNifIfBase* NewInterfaceL(const TDesC& aName, MNifIfNotify* aNotify);
	};

#endif
