// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Defines the factory class which is used to instantiate the RAW IP NIF.
// 
//

/**
 @file
*/

#ifndef RAWIPNIFMAINFACTORY_H__
#define RAWIPNIFMAINFACTORY_H__

#include <comms-infras/nifif.h>
#include "bttlog.h"

class CRawIPNifMainFactory : public CNifIfFactory
	{
public:
	virtual CNifIfBase* NewInterfaceL(const TDesC& aName);
	virtual CNifIfBase* NewInterfaceL(const TDesC& aName,MNifIfNotify* aNotify);

protected:
	virtual void InstallL();
	virtual TInt Info(TNifIfInfo& aInfo, TInt aIndex) const;

private:
	CBttLogger* iTheLogger;
	};

#endif
