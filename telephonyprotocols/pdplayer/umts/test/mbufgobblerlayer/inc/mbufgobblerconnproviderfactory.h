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
// Connection Provider Factory
//  (control plane)
//

/**
 @file
 @internalComponent
*/

#ifndef CMBUFGOBBLERCONNPROVIDERFACTORY_H
#define CMBUFGOBBLERCONNPROVIDERFACTORY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <comms-infras/ss_connprov.h>
#include <comms-infras/ss_nodemessages_legacy.h>
#include <comms-infras/ss_legacyinterfaces.h>

#include "mbufgobbleruids.h"



// CLASS DECLARATION

/**
 *  CMbufGobblerConnProviderFactory
 * 
 */
class CMbufGobblerConnProviderFactory : public ESock::CConnectionProviderFactoryBase
	{
public:
	// Constructors and destructor
	enum { iUid = EMbufGobblerCPrUid };
	static CMbufGobblerConnProviderFactory* NewL(TAny* aParentContainer);
	/**
	 * Destructor.
	 */
	~CMbufGobblerConnProviderFactory();

protected: //from CConnectionProviderFactoryBase
	virtual ESock::ACommsFactoryNodeId* DoCreateObjectL(ESock::TFactoryQueryBase& /* aQuery */);
	virtual void CMbufGobblerConnProviderFactory::EnumerateConnectionsL(RPointerArray<ESock::TSourcedConnectionInfo>& aConnectionInfoPtrArray);
	

private: //class not intended for derivation 
	/**
	 * Constructor for performing 1st stage construction
	 */
	CMbufGobblerConnProviderFactory(TUid aFactoryId, ESock::CConnectionFactoryContainer& aParentContainer);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // CMBUFGOBBLERCONNPROVIDERFACTORY_H
