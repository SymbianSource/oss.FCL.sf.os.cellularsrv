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
// Implementation file for the Umts/Gprs SubConnection Provider Factory
// 
//

/**
 @file
 @internalComponent
*/

#include "UmtsGprs_subconnProvFactory.h"
#include <networking/qos3gpp_subconparams.h>
#include "UmtsGprs_defaultSubconnProv.h"
#include <implementationproxy.h>

/**
Data required for instantiating ECOM Plugin
*/
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(KUmtsGprsSubConnectionProviderFactoryId, CUmtsGprsSubConnProvdFactory::NewL),
	};


/**
ECOM Implementation Factory
*/
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

    return ImplementationTable;
    }


CUmtsGprsSubConnProvdFactory* CUmtsGprsSubConnProvdFactory::NewL(TAny* aConstructionParameters)
/**
Constructs a Umts/Gprs SubConnection Provider Factory

@param aConstructionParameters construction data passed by ECOM

@returns pointer to a constructed factory
*/
	{
   CUmtsGprsSubConnProvdFactory* ptr = 
      new (ELeave) CUmtsGprsSubConnProvdFactory(KUmtsGprsSubConnectionProviderFactoryId, 
         *(reinterpret_cast<CSubConnectionFactoryContainer*>(aConstructionParameters)));
                  
	return ptr;
	}

	

CUmtsGprsSubConnProvdFactory::CUmtsGprsSubConnProvdFactory(TUint aFactoryId, 
                                     CSubConnectionFactoryContainer& aParentContainer)
	: CSubConnectionProviderFactoryBase(aFactoryId, aParentContainer)
/**
Umts/Gprs SubConnection Provider Factory Constructor

@param aFactoryId ECOM Implementation Id
@param aParentContainer Object Owner
*/
	{
	}
	

CUmtsGprsSubConnProvdFactory::~CUmtsGprsSubConnProvdFactory()
    {
    }
    

CSubConnectionProviderBase* CUmtsGprsSubConnProvdFactory::DoCreateProviderL(
                                   CConnectionProviderBase& aConnProvider, 
                                   RSubConnection::TSubConnType aType)
  {
  CSubConnectionProviderBase* provider = NULL;
  if (aType == RSubConnection::EAttachToDefault)
    {
    provider = CUmtsGprsDefaultSubConnProvd::NewL(*this, aConnProvider);
    }
  else
    {
    User::Leave(KErrNotSupported);
    }
  return provider;
  }
  
