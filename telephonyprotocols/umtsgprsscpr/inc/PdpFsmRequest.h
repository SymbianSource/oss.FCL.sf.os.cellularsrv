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
// PDP Context control requests.
// 
//

/**
 @file
 @internalComponent
*/

#ifndef CPDPFSMREQUEST_H
#define CPDPFSMREQUEST_H

#include "UmtsGprs_subconnProv.h"

/** The class commits previous modifications to an active PDP context.

@internalTechnology
@released Since 9.3
*/
class CModifyActivePDPCtx : public CPdpFsmRequestBase
{
public:
    virtual TInt Start();
    virtual TBool PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam);

    CModifyActivePDPCtx(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner);
    virtual ~CModifyActivePDPCtx();
};



#ifdef SYMBIAN_NETWORKING_UMTSR5
/** The class modifies R5 QoS on an active PDP context.

@internalTechnology
@released Since 9.3
*/
class CModifyR5QoS : public CPdpFsmRequestBase, public MPdpFsmRequestOwner
{
public:
	virtual void Done( TInt aResult, CPdpFsmRequestBase& aThis );
	virtual TInt Start();
	virtual TBool PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam);
	virtual ~CModifyR5QoS();
	static CModifyR5QoS* NewL(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner, RPacketQoS::TQoSR5Requested& aParams);

protected:
	CModifyR5QoS(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner, RPacketQoS::TQoSR5Requested& aParams);    
	
	void ConstructL();
	
private:
    void GrantedSetupL();
    void RejectedSetupL(TInt aResult);
private:
    CModifyActivePDPCtx* iModifyActive;
	RPacketQoS::TQoSR5Requested i5;
};


#else
// !SYMBIAN_NETWORKING_UMTSR5

/**
The class modifies R99/R4 QoS on an active PDP context.

@internalTechnology
@released Since 9.3
*/
class CModifyR4QoS : public CPdpFsmRequestBase, public MPdpFsmRequestOwner
{
public:
    virtual void Done( TInt aResult, CPdpFsmRequestBase& aThis );
    virtual TInt Start();
    virtual TBool PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam);
    virtual ~CModifyR4QoS();
    

    static CModifyR4QoS* NewL(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner, RPacketQoS::TQoSR99_R4Requested aParams);

protected:
    CModifyR4QoS(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner, RPacketQoS::TQoSR99_R4Requested aParams); 
	
	void ConstructL();
    
private:
    void GrantedSetupL();
    void RejectedSetupL(TInt aResult);
private:
    CModifyActivePDPCtx* iModifyActive;
    RPacketQoS::TQoSR99_R4Requested i99;

};


#endif 
// SYMBIAN_NETWORKING_UMTSR5

#endif
// CPDPFSMREQUEST_H
