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

#include "mbufgobblermetaconnproviderfactory.h"
#include "mbufgobblermetaconnprovider.h"
#include "mbufgobblerlog.h"


CMbufGobblerMetaConnectionProviderFactory* CMbufGobblerMetaConnectionProviderFactory::NewL(TAny* aParentContainer)
	{
 	return new (ELeave) CMbufGobblerMetaConnectionProviderFactory(TUid::Uid(CMbufGobblerMetaConnectionProviderFactory::iUid), 
 			                         *(reinterpret_cast<ESock::CMetaConnectionFactoryContainer*>(aParentContainer)));
	}

CMbufGobblerMetaConnectionProviderFactory::CMbufGobblerMetaConnectionProviderFactory(TUid aFactoryUid, ESock::CMetaConnectionFactoryContainer& aParentContainer)
	: CMetaConnectionProviderFactoryBase(aFactoryUid,aParentContainer)
	{
 	//LOG_NODE_CREATE(KVccMCprFactoryTag, CVccMetaConnectionProviderFactory);
	}

ESock::ACommsFactoryNodeId* CMbufGobblerMetaConnectionProviderFactory::DoCreateObjectL(ESock::TFactoryQueryBase& aQuery)
	{ 
	//LOG1(_L8("CMbufGobblerMetaConnectionProviderFactory %08x:\tDoCreateObjectL()"), this);
	const ESock::TMetaConnectionFactoryQuery& query = static_cast<const ESock::TMetaConnectionFactoryQuery&>(aQuery);
	return CMbufGobblerMetaConnectionProvider::NewL (*this, query.iProviderInfo);
	}

void CMbufGobblerMetaConnectionProviderFactory::DoEnumerateConnectionsL (RPointerArray<TConnectionInfo>& /*aConnectionInfoPtrArray*/)
	{
	// Not supported
	//User::Leave(KErrNotSupported);
	}






