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
// Implements the factory class which is used to instantiate the RAW IP NIF.
// 
//

/**
 @file
*/

#include "RawIPFlowFactory.h"
#include "RawIPFlow.h"
#include "bttlog.h"

using namespace ESock;

// =====================================================================================
//
// RawIP Flow Factory
//

CRawIPFlowFactory* CRawIPFlowFactory::NewL(TAny* aConstructionParameters)
/**
Constructs a Default SubConnection Flow Factory

@param aConstructionParameters construction data passed by ECOM

@returns pointer to a constructed factory
*/
	{
	CRawIPFlowFactory* ptr = new (ELeave) CRawIPFlowFactory(TUid::Uid(KRawIpFlowImplUid), *(reinterpret_cast<CSubConnectionFlowFactoryContainer*>(aConstructionParameters)));
	return ptr;
	}


CRawIPFlowFactory::CRawIPFlowFactory(TUid aFactoryId, CSubConnectionFlowFactoryContainer& aParentContainer)
	: CSubConnectionFlowFactoryBase(aFactoryId, aParentContainer)
/**
Default SubConnection Flow Factory Constructor

@param aFactoryId ECOM Implementation Id
@param aParentContainer Object Owner
*/
	{
	}


CSubConnectionFlowBase* CRawIPFlowFactory::DoCreateFlowL(ESock::CProtocolIntfBase* aProtocol, ESock::TFactoryQueryBase& aQuery)
	{
#ifdef __BTT_LOGGING__
	iTheLogger = CBttLogger::NewL(KNifSubDir, KRefFile, User::FastCounter());
#endif // __BTT_LOGGING__

	_LOG_L1C1(_L8("Raw IP logging started."));
	
	const TDefaultFlowFactoryQuery& query = static_cast<const TDefaultFlowFactoryQuery&>(aQuery);
 	CRawIPFlow* s = new (ELeave) CRawIPFlow(*this, query.iSCprId, aProtocol, iTheLogger);
	CleanupStack::PushL(s);
	s->ConstructL();
	CleanupStack::Pop(s);

	return s;
	}

