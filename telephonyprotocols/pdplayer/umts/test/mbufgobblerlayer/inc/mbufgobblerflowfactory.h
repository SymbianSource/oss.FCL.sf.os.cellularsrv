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

#ifndef MBUFGOBBLERFLOWFACTORY_H__
#define MBUFGOBBLERFLOWFACTORY_H__

#include <e32base.h>
#include <e32std.h>
#include <comms-infras/ss_subconnflow.h>
#include <comms-infras/ss_protflow.h>
#include <comms-infras/ss_subconnprov.h>
#include <elements/nm_signatures.h>
#include "mbufgobbleruids.h"

class CMbufGobblerFlowFactory : public ESock::CSubConnectionFlowFactoryBase
/**
MbufGobbler Flow Factory
*/
	{
public:

	enum { EUid = EMbufGobblerFlowUid };
	static CMbufGobblerFlowFactory* NewL(TAny* aConstructionParameters);

protected:
	CMbufGobblerFlowFactory(TUid aFactoryId, ESock::CSubConnectionFlowFactoryContainer& aParentContainer);
	virtual ESock::CSubConnectionFlowBase* DoCreateFlowL(ESock::CProtocolIntfBase* aProtocol, ESock::TFactoryQueryBase& aQuery);
	};

#endif // MBUFGOBBLERFLOWFACTORY_H__
