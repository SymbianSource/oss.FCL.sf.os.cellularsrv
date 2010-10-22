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
// Factory which creates the Meta Connection Provider
//  (management plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERMETACONNPROVIDERFACTORY_H
#define MBUFGOBBLERMETACONNPROVIDERFACTORY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <comms-infras/ss_metaconnprov.h>
#include "mbufgobbleruids.h"

// CLASS DECLARATION

/**
 *  CMbufGobblerMetaConnectionProviderFactory
 * 
 */


class CMbufGobblerMetaConnectionProviderFactory : public ESock::CMetaConnectionProviderFactoryBase
	{
public:
    enum { iUid = EMbufGobblerMCPrUid };
	static CMbufGobblerMetaConnectionProviderFactory* NewL(TAny* aParentContainer);
	
protected: //from CMetaConnectionProviderFactoryBase
	virtual ESock::ACommsFactoryNodeId* DoCreateObjectL(ESock::TFactoryQueryBase& aQuery);
	virtual void DoEnumerateConnectionsL (RPointerArray<TConnectionInfo>& /*aConnectionInfoPtrArray*/);

private: //not intended for derivation
	CMbufGobblerMetaConnectionProviderFactory(TUid aFactoryUid, ESock::CMetaConnectionFactoryContainer& aParentContainer);	
	};

#endif // MBUFGOBBLERMETACONNPROVIDERFACTORY_H
