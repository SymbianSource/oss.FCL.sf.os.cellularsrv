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

#ifndef MBUFGOBBLERMETACONNPROVIDER_H
#define MBUFGOBBLERMETACONNPROVIDER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <comms-infras/coremcpr.h>
#include "mbufgobblertestflags.h"

// CLASS DECLARATION

/**
 *  CMbufGobblerMetaConnectionProvider
 * 
 */
	
class CMbufGobblerMetaConnectionProvider : public CCoreMetaConnectionProvider
	{
public:
	static CMbufGobblerMetaConnectionProvider* NewL(ESock::CMetaConnectionProviderFactoryBase& aFactory,
											const ESock::TProviderInfo& aProviderInfo);
	~CMbufGobblerMetaConnectionProvider();
	 
protected:
	CMbufGobblerMetaConnectionProvider(ESock::CMetaConnectionProviderFactoryBase& aFactory,
	const ESock::TProviderInfo& aProviderInfo, const MeshMachine::TNodeActivityMap& aActivityMap);

	void ConstructL();
	
	// From MCFNode Interface 
	void ReceivedL(const Messages::TRuntimeCtxId& aSender, const Messages::TNodeId& aRecipient, Messages::TSignatureBase& aCFMessage);
private:
	TInt iClientCount; //holds count of clients that node is joined with
	MBUFGOBBLER_TEST_DATA	
	};


#endif // MBUFGOBBLERMETACONNPROVIDER_H
