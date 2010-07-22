// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Interface to the PDP Context Finite State Machine
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef PDPFSM_H
#define PDPFSM_H


#include <networking/mpdpfsminterface.h>
#include "RSpudManInterface.h"

class CPdpFsmFactory;
class CUmtsGprsDefaultSubConnProvd;


class CPdpFsmInterface : public MPdpFsmInterface
/**
 * UmtsGprsSCPR implemenation for the MPdpFsmInterface
 *
 * @internalComponent
 */
 {
 public:
	//-=========================================================
	// custom methods
	//-=========================================================	
    CPdpFsmInterface(CUmtsGprsDefaultSubConnProvd& aOwner);
	virtual ~CPdpFsmInterface();

    void SpudInput (TContextId aPdpId, TInt aNotification, TInt aParam);
	
	//-=========================================================
	// MPdpFsmInterface methods
	//-=========================================================	
	virtual void OpenL(MSpudManInterface * aSpudManInterface, TName& aName);
	virtual void Close();

	virtual TInt Input (TContextId aPdpId, const TInt aOperation, const TInt aParam=KErrNone);

	virtual TInt Set(TContextId aPdpId, const TTFTInfo& aParam);
	virtual TInt Set(TContextId aPdpId, const TTFTOperationCode& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TDataChannelV2& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TContextConfigGPRS& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TContextStatus& aParam);


#ifdef SYMBIAN_NETWORKING_UMTSR5
	
	// This is to set the status of IM CN signalling parameter
	virtual void SetIMCNSignalling(TBool aImCnSignalling);
	
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR5Requested& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR5Negotiated& aParam);
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR5Requested& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR5Negotiated& aParam) const;

#else
// !SYMBIAN_NETWORKING_UMTSR5 

	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Requested& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Negotiated& aParam) const;
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Requested& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Negotiated& aParam);

#endif 
// SYMBIAN_NETWORKING_UMTSR5 


	virtual TInt Get(TContextId aPdpId, TTFTInfo& aParam) const;
	virtual TInt Get(TContextId aPdpId, TTFTOperationCode& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TDataChannelV2& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TContextConfigGPRS& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TContextStatus& aParam) const;

	virtual TInt GetLastErrorCause(TContextId aPdpId, TInt& aLastErrorCause) const;

	virtual void Set(const RPacketService::TStatus  aParam); // applies to all contexts
	virtual void Get(RPacketService::TStatus& aParam);// applies to all contexts

	virtual const TName& TsyName(void);

	void SetContextTerminationErrorAndStop(TContextId aPdpId, TInt aErrorCode);

 protected:
 private:
	// DO NOT provide implementations to these methods: Copying this object should be disabled
	CPdpFsmInterface(CPdpFsmInterface&);
	CPdpFsmInterface& operator= (CPdpFsmInterface& aInput);
	
 public:
 protected:
 private:
	// FSM Factory - churns out FSMs
	CPdpFsmFactory*               iPdpFsmFactory;

	RPacketService::TStatus       iNetworkStatus;	
	
	CUmtsGprsDefaultSubConnProvd& iDefaultSubConnProvd;
	RSpudManInterface             iSpudManInterface;

};

#endif
// PDPFSM_H
