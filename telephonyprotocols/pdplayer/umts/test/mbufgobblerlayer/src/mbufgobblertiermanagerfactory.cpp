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

#include "mbufgobblertiermanagerfactory.h"
#include "mbufgobblertiermanager.h"
#include "mbufgobblerlog.h"



CMbufGobblerTierManagerFactory* CMbufGobblerTierManagerFactory::NewL(TAny* aParentContainer)
	{
	//__CFLOG_VAR((KIPProtoTierMgrTag, KIPProtoTierMgrSubTag, _L8("CIPProtoTierManagerFactory::\tNewL(%08x)"), aParentContainer));
 	return new (ELeave) CMbufGobblerTierManagerFactory(TUid::Uid(CMbufGobblerTierManagerFactory::iUid),TUid::Uid(CMbufGobblerTierManagerFactory::iUid),*(reinterpret_cast<ESock::CTierManagerFactoryContainer*>(aParentContainer)));
	}

CMbufGobblerTierManagerFactory::CMbufGobblerTierManagerFactory(TUid aTierTypeId, TUid aFactoryUid, ESock::CTierManagerFactoryContainer& aParentContainer)
:	CTierManagerFactoryBase(aTierTypeId,aFactoryUid,aParentContainer)
	{
	//MBUFGOBBLER_LOG_NODE_CREATE(KMbufGobblerTierFactorySubTag,CMbufGobblerTierManagerFactory, this);
	
	//__CFLOG_VAR((KIPProtoTierMgrTag, KIPProtoTierMgrSubTag, _L8("CIPProtoTierManagerFactory %08x:\tCIPProtoTierManagerFactory"), this));
	}

ESock::ACommsFactoryNodeId* CMbufGobblerTierManagerFactory::DoCreateObjectL(ESock::TFactoryQueryBase& /* aQuery */)
	{
	return CMbufGobblerTierManager::NewL(*this);
	}



