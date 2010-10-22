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
// Tier Manager
//   (management plane)
//

/**
 @file
 @internalComponent
*/

#include "mbufgobblertiermanager.h"
#include "mbufgobblertiermanageractivities.h"
#include "mbufgobblerlog.h"
#include "mbufgobblertestflags.h"

#include <comms-infras/coretiermanageractivities.h>

#include "mbufgobblerproviderselector.h"

_LIT8(KNodeName, "CMbufGobblerTierManager");

CMbufGobblerTierManager* CMbufGobblerTierManager::NewL(ESock::CTierManagerFactoryBase& aFactory)
	{
	CMbufGobblerTierManager* self = new (ELeave) CMbufGobblerTierManager(aFactory);
 	CleanupStack::PushL(self);
 	self->ConstructL();
 	CleanupStack::Pop(self);
 	return self;
	}

CMbufGobblerTierManager::CMbufGobblerTierManager(ESock::CTierManagerFactoryBase& aFactory)                                   
:	CCoreTierManager(aFactory,MbufGobblerTMActivities::MbufGobblerTMActivitiesMap::Self())
	{
	MBUFGOBBLER_LOG_NODE_CREATE(KMbufGobblerTierSubTag, CMbufGobblerTierManager, KNodeName, this->NodeId().Ptr());
	}

void CMbufGobblerTierManager::ConstructL()
	{
	CCoreTierManager::ConstructL();
	MBUFGOBBLER_TEST_DATA_INIT
    MBUFGOBBLER_TEST_POINT(KTierUp,KErrNone)
	}

CMbufGobblerTierManager::~CMbufGobblerTierManager()
	{
	MBUFGOBBLER_LOG_NODE_DESTROY(KMbufGobblerTierSubTag, CMbufGobblerTierManager, KNodeName, this->NodeId().Ptr());
	}

ESock::MProviderSelector* CMbufGobblerTierManager::DoCreateProviderSelectorL(const Meta::SMetaData& aSelectionPreferences)
	{
    return CMbufGobblerProviderSelector::NewL(aSelectionPreferences);
	}

void CMbufGobblerTierManager::ReceivedL(const Messages::TRuntimeCtxId& aSender, const Messages::TNodeId& aRecipient, Messages::TSignatureBase& aMessage)
	{
	LOGMESSAGE(KMbufGobblerTierSubTag, KNodeName, this, aSender, aRecipient, aMessage);
	LOG_NODE_INFO(KMbufGobblerTierSubTag, KNodeName, *this, iClientCount);
  	MeshMachine::TNodeContext<CMbufGobblerTierManager> ctx(*this, aMessage, aSender, aRecipient);
   	CCoreTierManager::Received(ctx);
    User::LeaveIfError(ctx.iReturn);
	}







