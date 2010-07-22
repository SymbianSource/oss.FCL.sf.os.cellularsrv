// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Umts/Gprs SubConnection Provider Facory header file
// 
//

/**
 @file
 @internalComponent
*/


#ifndef UMTSGPRS_SUBCONNPROVFACTORY_H
#define UMTSGPRS_SUBCONNPROVFACTORY_H

#include <ss_subconnprov.h>
#include <comms-infras/nifif.h>
#include <networking/mspudmaninterface.h>

// The ECOM DLL Uid and Factory Uid for the UmtsGprsSCPR

const TUint KUmtsGprsSubConnectionProviderFactoryId = 0x1020D45F;

class CUmtsGprsSubConnProvdFactory : public CSubConnectionProviderFactoryBase
/** 

@internalTechnology
@released Since 9.3
*/
	{
public:
	static CUmtsGprsSubConnProvdFactory* NewL(TAny* aConstructionParameters);	
	
protected:
	CUmtsGprsSubConnProvdFactory(TUint aFactoryId, CSubConnectionFactoryContainer& aParentContainer);
	virtual ~CUmtsGprsSubConnProvdFactory();
	
	// methods to be overriden for CSubConnectionProviderFactory
	virtual CSubConnectionProviderBase* DoCreateProviderL(CConnectionProviderBase& aConnProvider, RSubConnection::TSubConnType aType);

	};


#endif
// UMTSGPRS_SUBCONNPROVFACTORY_H
