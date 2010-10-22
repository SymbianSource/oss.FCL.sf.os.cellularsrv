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
// Connection Provider
//  (control plane)
//

/**
 @file
 @internalComponent
*/

#ifndef CMBUFGOBBLERCONNPROVIDER_H
#define CMBUFGOBBLERCONNPROVIDER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <comms-infras/corecpr.h>
#include "mbufgobblertestflags.h"




// CLASS DECLARATION

/**
 *  CMbufGobblerConnProvider
 * 
 */

//comment on diffeent super class options e.g. CMobilityConnectionProvider
class ESock::CConnectionProviderFactoryBase;
class CMbufGobblerConnProvider : public CCoreConnectionProvider
	{
public:
	// Constructors and destructor
	static CMbufGobblerConnProvider* NewL(ESock::CConnectionProviderFactoryBase& aFactory);

	/**
	 * Destructor.
	 */
	~CMbufGobblerConnProvider();

	
protected: //from CCoreConnectionProvider
	void ReceivedL(const Messages::TRuntimeCtxId& aSender, const Messages::TNodeId& aRecipient, Messages::TSignatureBase& aMessage);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMbufGobblerConnProvider(ESock::CConnectionProviderFactoryBase& aFactory);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
private:
	TInt iClientCount; //holds count of clients that node is joined with

	MBUFGOBBLER_TEST_DATA
	};

#endif // CMBUFGOBBLERCONNPROVIDER_H
