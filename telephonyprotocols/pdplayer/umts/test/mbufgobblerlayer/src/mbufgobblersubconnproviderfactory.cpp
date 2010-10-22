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

#include "mbufgobblersubconnproviderfactory.h"
#include "mbufgobblersubconnprovider.h"
#include "mbufgobblerlog.h"

CMbufGobblerSubconnProviderFactory* CMbufGobblerSubconnProviderFactory::NewL(TAny* aParentContainer)
    {
    return new (ELeave) CMbufGobblerSubconnProviderFactory(TUid::Uid(CMbufGobblerSubconnProviderFactory::iUid), 
                                            *reinterpret_cast<ESock::CSubConnectionFactoryContainer*>(aParentContainer));
    }
    
CMbufGobblerSubconnProviderFactory::CMbufGobblerSubconnProviderFactory(TUid aFactoryId, ESock::CSubConnectionFactoryContainer& aParentContainer)
	: CSubConnectionProviderFactoryBase(aFactoryId, aParentContainer)
    {
    }

ESock::ACommsFactoryNodeId* CMbufGobblerSubconnProviderFactory::DoCreateObjectL(ESock::TFactoryQueryBase& aQuery)
    {
    
    const ESock::TDefaultSCPRFactoryQuery& query = static_cast<const ESock::TDefaultSCPRFactoryQuery&>(aQuery);
    if (query.iSCPRType == RSubConnection::EAttachToDefault)
        {
        return CMbufGobblerSubconnProvider::NewL(*this);
        }
    else if(query.iSCPRType == RSubConnection::ECreateNew)
    	{
    	
    	}
    else if(query.iSCPRType == RSubConnection::EWaitIncoming)
    	{
    
    	}
    else //will never get here - should really assert
        {
        User::Leave(KErrNotSupported);
        }
    return NULL;
    }
