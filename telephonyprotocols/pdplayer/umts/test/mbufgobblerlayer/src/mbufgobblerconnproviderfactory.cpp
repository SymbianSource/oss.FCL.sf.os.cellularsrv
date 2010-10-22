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
// Connection Provider Factory
//  (control plane)
//

/**
 @file
 @internalComponent
*/

#include "mbufgobblerconnproviderfactory.h"
#include "mbufgobblerconnprovider.h"
#include "mbufgobblerlog.h"

CMbufGobblerConnProviderFactory::~CMbufGobblerConnProviderFactory()
	{
	}

CMbufGobblerConnProviderFactory* CMbufGobblerConnProviderFactory::NewL(TAny* aParentContainer)
    {
    return new (ELeave) CMbufGobblerConnProviderFactory(TUid::Uid(CMbufGobblerConnProviderFactory::iUid), *reinterpret_cast<ESock::CConnectionFactoryContainer*>(aParentContainer));
    }

CMbufGobblerConnProviderFactory::CMbufGobblerConnProviderFactory(TUid aFactoryId, ESock::CConnectionFactoryContainer& aParentContainer)
:	CConnectionProviderFactoryBase(aFactoryId, aParentContainer)
    {
    }

ESock::ACommsFactoryNodeId* CMbufGobblerConnProviderFactory::DoCreateObjectL(ESock::TFactoryQueryBase& /* aQuery */)
    {    
	return CMbufGobblerConnProvider::NewL(*this); 
    }

void CMbufGobblerConnProviderFactory::EnumerateConnectionsL(RPointerArray<ESock::TSourcedConnectionInfo>& aConnectionInfoPtrArray)
    {
    (void)aConnectionInfoPtrArray;
    //TEnumerateConnectionsQuery query(aConnectionInfoPtrArray);
    //Find(query);
    }


