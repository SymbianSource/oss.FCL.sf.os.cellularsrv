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
// UmtsGprs_subconnProv.h
// Umts/Gprs Default SubConnection Provider class definition.
// 
//

/**
 @file
 @internalComponent
*/


#ifndef UMTSGPRS_DEFAULTSUBCONNPROV_H
#define UMTSGPRS_DEFAULTSUBCONNPROV_H


#include "UmtsGprs_subconnProv.h"


/** 
Specialised subconnection provider for handling default UMTS contexts.

@internalTechnology
@released Since 9.3
*/
class CUmtsGprsDefaultSubConnProvd : public CUmtsGprsSubConnProvd
	{
    friend class CUmtsGprsSubConnProvdFactory; //for the protected constructor.
public:

	/** Modifies iContextId of 'this' to aContextId. This method should be called
	* in the handover scenario, where the current default is dropped and a new
	* one is elected.
	* @param aContextId the id of the new default PDP context.
	*/
	void NewDefaultElected(TContextId aContextId);
	
#ifdef SYMBIAN_NETWORKING_UMTSR5
	
	/** This parameter adds the IMCN information to the Default 
	* SubConnection Parameter Bundle
	*/
	TInt AddIMCNInformationToParameterBundleL(TBool aImCnInfo);
	
#endif //SYMBIAN_NETWORKING_UMTSR5

    virtual TBool ShouldIDeleteNow();


protected:

	//-=========================================================
	// CSubConnectionProviderBase methods
	//-=========================================================
	virtual void DoParametersAboutToBeSetL(CSubConParameterBundle& aParameterBundle);
   virtual TInt DoControl(TUint aOptionLevel, TUint aOptionName, TDes8& aOption);

	//-=========================================================
	// C'tor & D'tor methods
	//-=========================================================
	CUmtsGprsDefaultSubConnProvd(CSubConnectionProviderFactoryBase& aFactory, 
	                             CConnectionProviderBase& aConnProvider);
	                           
	static CUmtsGprsDefaultSubConnProvd* NewL(CSubConnectionProviderFactoryBase& aFactory, 
	                             CConnectionProviderBase& aConnProvider);
	                             
	void ConstructL();
	
	// Don't allow clients to invoke the destructor.
	// (Only the CCommsFactoryBase should do this)
	virtual ~CUmtsGprsDefaultSubConnProvd();
	
	
	
	//-=========================================================
	// Custom methods
	//-=========================================================
	/** 
	  @return the default CUmtsGprsSubConnProvd for 'this'
	*/
	virtual CUmtsGprsSubConnProvd& GetDefaultSubConnection();
	
	
	/**
	Returns the PdpFsmInterface instance, which can be used to talk to SpudFsm
   
   @return the local instance (pertaining to this connection) of CPdpFsmInterface.
	*/
	virtual CPdpFsmInterface& GetPdpFsmInterface();
	
	/**
	Reads default QoS parameters from the database.
	
   @param aParams - The QoS parameter structure to be filled in by this method.
	*/
	void GetQoSDefaultsL(RPacketQoS::TQoSR99_R4Requested* aParams);
	
	//-=========================================================
	// Members
	//-=========================================================
	CPdpFsmInterface* iPdpFsm;

	};


#include "UmtsGprs_defaultSubconnProv.inl"
#endif
// UMTSGPRS_DEFAULTSUBCONNPROV_H
