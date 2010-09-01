// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// RawIP MCPR Factory
// 
//

/**
 @file
 @internalComponent
*/

#include "rawipmcprfactory.h"
#include "rawipmcpr.h"
#include <comms-infras/ss_log.h>
#include <ss_glob.h>

#include <comms-infras/ss_msgintercept.h>

#ifdef __CFLOG_ACTIVE
#define KRawIPMCprFactoryTag KESockMetaConnectionTag
// _LIT8(KRawIPMCprFactorySubTag, "rawipmcprfactory");
#endif // __CFLOG_ACTIVE

using namespace ESock;

//-=========================================================
//
// CRawIpMetaConnectionProviderFactory methods
//
//-=========================================================	
CRawIpMetaConnectionProviderFactory* CRawIpMetaConnectionProviderFactory::NewL(TAny* aParentContainer)
	{
	__CFLOG_VAR((KRawIPMCprFactoryTag, KRawIPMCprFactorySubTag, _L8("CRawIpMetaConnectionProviderFactory ::\tNewL(%08x)"), aParentContainer));
 	return new (ELeave) CRawIpMetaConnectionProviderFactory(TUid::Uid(CRawIpMetaConnectionProviderFactory::iUid), *(reinterpret_cast<CMetaConnectionFactoryContainer*>(aParentContainer)));
	}

CRawIpMetaConnectionProviderFactory::CRawIpMetaConnectionProviderFactory(TUid aFactoryId, CMetaConnectionFactoryContainer& aParentContainer)
	: CMetaConnectionProviderFactoryBase(aFactoryId,aParentContainer)
	{
	__CFLOG_VAR((KRawIPMCprFactoryTag, KRawIPMCprFactorySubTag, _L8("CRawIpMetaConnectionProviderFactory %08x:\tCRawIpMetaConnectionProviderFactory Constructor"), this));
	}

ACommsFactoryNodeId* CRawIpMetaConnectionProviderFactory::DoCreateObjectL(ESock::TFactoryQueryBase& aQuery)
	{
	const TMetaConnectionFactoryQuery& query = static_cast<const TMetaConnectionFactoryQuery&>(aQuery);
	CMetaConnectionProviderBase* provider = CRawIpMetaConnectionProvider::NewL(*this, query.iProviderInfo);	
	
	ESOCK_DEBUG_REGISTER_GENERAL_NODE(iUid, provider);

	return provider;
	}

