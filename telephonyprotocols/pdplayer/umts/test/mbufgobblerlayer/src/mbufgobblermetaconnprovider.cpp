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
// Meta Connection Provider
//  (management plane)
//

/**
 @file
 @internalComponent
*/

#include "mbufgobblermetaconnprovider.h"
#include "mbufgobblermetaconnprovideractivities.h"
#include "mbufgobblerlog.h"
#include "mbufgobblertestflags.h"

_LIT8(KNodeName, "CMbufGobblerMetaConnectionProvider");

CMbufGobblerMetaConnectionProvider* CMbufGobblerMetaConnectionProvider::NewL(ESock::CMetaConnectionProviderFactoryBase& aFactory,
                                   const ESock::TProviderInfo& aProviderInfo)
	{
	CMbufGobblerMetaConnectionProvider* self = new (ELeave) CMbufGobblerMetaConnectionProvider(aFactory,aProviderInfo,MbufGobblerMCprActivities::MbufGobblerMCprActivitiesMap::Self());
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CMbufGobblerMetaConnectionProvider::CMbufGobblerMetaConnectionProvider(ESock::CMetaConnectionProviderFactoryBase& aFactory,
				const ESock::TProviderInfo& aProviderInfo, const MeshMachine::TNodeActivityMap& aActivityMap)
	: CCoreMetaConnectionProvider(aFactory, aProviderInfo, aActivityMap)
	{
	MBUFGOBBLER_LOG_NODE_CREATE(KMbufGobblerMcprSubTag, CMbufGobblerMetaConnectionProvider, KNodeName, this->NodeId().Ptr());	
	}

void CMbufGobblerMetaConnectionProvider::ConstructL()
	{
	CCoreMetaConnectionProvider::ConstructL();
	MBUFGOBBLER_TEST_DATA_INIT
    MBUFGOBBLER_TEST_POINT(KMCPrUp,KErrNone)
	}

CMbufGobblerMetaConnectionProvider::~CMbufGobblerMetaConnectionProvider()
	{
	MBUFGOBBLER_LOG_NODE_DESTROY(KMbufGobblerMcprSubTag, CMbufGobblerMetaConnectionProvider, KNodeName, this->NodeId().Ptr());
	}

void CMbufGobblerMetaConnectionProvider::ReceivedL(const Messages::TRuntimeCtxId& aSender, const Messages::TNodeId& aRecipient, Messages::TSignatureBase& aCFMessage)
	{   
	LOGMESSAGE(KMbufGobblerMcprSubTag, KNodeName, this, aSender, aRecipient, aCFMessage);
	LOG_NODE_INFO(KMbufGobblerMcprSubTag, KNodeName, *this, iClientCount);
	MeshMachine::TNodeContext<CMbufGobblerMetaConnectionProvider> ctx(*this, aCFMessage, aSender, aRecipient);
	CCoreMetaConnectionProvider::Received(ctx);
	User::LeaveIfError(ctx.iReturn);
	}

