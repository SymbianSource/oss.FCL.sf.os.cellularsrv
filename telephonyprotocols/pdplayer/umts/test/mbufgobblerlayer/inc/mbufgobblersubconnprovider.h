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
// Sub Connection Provider
//  (control plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERSUBCONNPROVIDER_H
#define MBUFGOBBLERSUBCONNPROVIDER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <comms-infras/corescpr.h>
#include "mbufgobblertestflags.h"

// CLASS DECLARATION

/**
 *  CMbufGobblerSubconnProvider
 * 
 */
class ESock::CSubConnectionProviderFactoryBase;
class CMbufGobblerSubconnProvider : public CCoreSubConnectionProvider
	{
public:
	// Constructors and destructor
	static CMbufGobblerSubconnProvider* NewL(ESock::CSubConnectionProviderFactoryBase& aFactory);

	/**
	 * Destructor.
	 */
	~CMbufGobblerSubconnProvider();

	
protected: //from CCoreSubConnectionProvider
	void ReceivedL(const Messages::TRuntimeCtxId& aSender, const Messages::TNodeId& aRecipient, Messages::TSignatureBase& aMessage);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMbufGobblerSubconnProvider(ESock::CSubConnectionProviderFactoryBase& aFactory);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
private:
	TInt iClientCount; //holds count of clients that node is joined with
	MBUFGOBBLER_TEST_DATA	
	};

#endif // MBUFGOBBLERSUBCONNPROVIDER_H
