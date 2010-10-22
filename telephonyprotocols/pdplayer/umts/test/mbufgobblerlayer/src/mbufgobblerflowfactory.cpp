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
// The factory class which is used to instantiate the MbufGobbler flow.
//  (data plane)
//

/**
 @file
 @internalComponent
*/

#include "mbufgobblerflowfactory.h"
#include "mbufgobblerflow.h"
#include "mbufgobblerlog.h"

// =====================================================================================
//
// MbufGobbler Flow Factory
//

CMbufGobblerFlowFactory* CMbufGobblerFlowFactory::NewL(TAny* aConstructionParameters)
/**
Constructs a Default SubConnection Flow Factory

@param aConstructionParameters construction data passed by ECOM

@returns pointer to a constructed factory
*/
	{
	CMbufGobblerFlowFactory* ptr = new (ELeave) CMbufGobblerFlowFactory(TUid::Uid(CMbufGobblerFlowFactory::EUid), *(reinterpret_cast<ESock::CSubConnectionFlowFactoryContainer*>(aConstructionParameters)));
	
	return ptr;
	}


CMbufGobblerFlowFactory::CMbufGobblerFlowFactory(TUid aFactoryId, ESock::CSubConnectionFlowFactoryContainer& aParentContainer)
	: CSubConnectionFlowFactoryBase(aFactoryId, aParentContainer)
/**
Default SubConnection Flow Factory Constructor

@param aFactoryId ECOM Implementation Id
@param aParentContainer Object Owner
*/
	{
	}


ESock::CSubConnectionFlowBase* CMbufGobblerFlowFactory::DoCreateFlowL(ESock::CProtocolIntfBase* aProtocol, ESock::TFactoryQueryBase& aQuery)
	{
	const ESock::TDefaultFlowFactoryQuery& query = static_cast<const ESock::TDefaultFlowFactoryQuery&>(aQuery);
	ESock::CSubConnectionFlowBase *temp = CMbufGobblerFlow::NewL(*this, query.iSCprId, aProtocol);
	return temp;
	}

