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
// Pdp MCPR Factory
// 
//

/**
 @file
 @internalComponent
*/

#include <etelqos.h>
#include <ecom/implementationproxy.h>
#include <comms-infras/ss_log.h>
#include <ss_glob.h>
#include <ecom/ecom.h>

#include <comms-infras/ss_msgintercept.h>

#include "pdpmcprfactory.h"
#include "pdpmcpr.h"



#ifdef _DEBUG
#define KPdpMCprFactoryTag KESockMetaConnectionTag
_LIT8(KPdpMCprFactorySubTag, "pdpmcprfactory");
#endif

using namespace ESock;

//-=========================================================
//
// CPdpMetaConnectionProviderFactory methods
//
//-=========================================================	
CPdpMetaConnectionProviderFactory* CPdpMetaConnectionProviderFactory::NewL(TAny* aParentContainer)
	{
	__CFLOG_VAR((KPdpMCprFactoryTag, KPdpMCprFactorySubTag, _L8("CPdpMetaConnectionProviderFactory ::\tNewL(%08x)"), aParentContainer));
 	return new (ELeave) CPdpMetaConnectionProviderFactory(TUid::Uid(CPdpMetaConnectionProviderFactory::iUid), *(reinterpret_cast<CMetaConnectionFactoryContainer*>(aParentContainer)));
	}

CPdpMetaConnectionProviderFactory::CPdpMetaConnectionProviderFactory(TUid aFactoryId, CMetaConnectionFactoryContainer& aParentContainer)
	: CMetaConnectionProviderFactoryBase(aFactoryId,aParentContainer)
	{
	__CFLOG_VAR((KPdpMCprFactoryTag, KPdpMCprFactorySubTag, _L8("CPdpMetaConnectionProviderFactory %08x:\tCPdpMetaConnectionProviderFactory Constructor"), this));
	}

ESock::ACommsFactoryNodeId* CPdpMetaConnectionProviderFactory::DoCreateObjectL(ESock::TFactoryQueryBase& aQuery)
	{
	const TMetaConnectionFactoryQuery& query = static_cast<const TMetaConnectionFactoryQuery&>(aQuery);
	CMetaConnectionProviderBase* provider = CPdpMetaConnectionProvider::NewL(*this, query.iProviderInfo);	

	ESOCK_DEBUG_REGISTER_GENERAL_NODE(iUid, provider);
	
	return provider;
	}

