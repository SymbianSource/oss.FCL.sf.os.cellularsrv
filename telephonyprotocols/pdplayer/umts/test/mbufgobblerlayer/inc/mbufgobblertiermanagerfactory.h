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
// Tier Manager Factory
//   (management plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERTIERMANAGERFACTORY_H
#define MBUFGOBBLERTIERMANAGERFACTORY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <ss_tiermanager.h>
#include "mbufgobbleruids.h"

// CLASS DECLARATION

/**
 *  CMbufGobblerTierManagerFactory
 * 
 */

class CMbufGobblerTierManagerFactory : public ESock::CTierManagerFactoryBase
	{
public:
    enum { iUid = EMbufGobblerTierUid};
	static CMbufGobblerTierManagerFactory* NewL(TAny* aParentContainer);

protected:
	CMbufGobblerTierManagerFactory(TUid aTierTypeId, TUid aFactoryUid, ESock::CTierManagerFactoryContainer& aParentContainer);
	virtual ESock::ACommsFactoryNodeId* DoCreateObjectL(ESock::TFactoryQueryBase& aQuery);
	};

#endif // MBUFGOBBLERTIERMANAGERFACTORY_H
