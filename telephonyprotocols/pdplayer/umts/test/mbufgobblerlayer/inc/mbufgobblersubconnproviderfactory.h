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
// Sub Connection Provider Factory
//  (control plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERSUBCONNPROVIDERFACTORY_H
#define MBUFGOBBLERSUBCONNPROVIDERFACTORY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <comms-infras/ss_subconnprov.h>
#include "mbufgobbleruids.h"

// CLASS DECLARATION

/**
 *  CMbufGobblerSubconnProviderFactory
 * 
 */
class CMbufGobblerSubconnProviderFactory : public ESock::CSubConnectionProviderFactoryBase
	{
public:
    enum { iUid = EMbufGobblerSCPrUid };
	static CMbufGobblerSubconnProviderFactory* NewL(TAny* aParentContainer);

protected:
	CMbufGobblerSubconnProviderFactory(TUid aFactoryId, ESock::CSubConnectionFactoryContainer& aParentContainer);
	virtual ESock::ACommsFactoryNodeId* DoCreateObjectL(ESock::TFactoryQueryBase& aQuery);
	};

#endif // MBUFGOBBLERSUBCONNPROVIDERFACTORY_H
