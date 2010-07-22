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
//

/**
 @file
 @internalComponent
*/

// NOTE: This guard define is used to prevent the real PDPFSM.h from being included.
//       This file MUST be included first in test code
#ifndef PDPFSM_H
#define PDPFSM_H


#include <networking/mpdpfsminterface.h>
#include "RSpudManInterface.h"
#include <etelpckt.h>

class CPdpFsmFactory;

/**
Dummy implementation of the MPdpFsmInterface that is seperated from the UmtsGprsSCPR
*/
class CPdpFsmInterface : public MPdpFsmInterface
   {
public:
	//-=========================================================
	// custom methods
	//-=========================================================	
    CPdpFsmInterface();
    ~CPdpFsmInterface();

    // We need this 'factory' method since the test harness has to have a means (albeit indirect) of
    // creating the CPdpFsm objects, but within the Active Scheduler Thread (terminology from the test's
    // design document). and not within the test harness (which is what happens if they get created via
    // 'MPdpFsmInterface' Set() or Get() methods in this test harness). Because the 'CEtelDriverContext'
    // objects that are indirectly created are added to the test thread's ActiveScheduler
    //
    TInt NewFsmContext(TContextId aPdpId);

    void SpudInput (TContextId aPdpId, TInt aNotification, TInt aParam);
	
	//-=========================================================
	// MPdpFsmInterface methods
	//-=========================================================	
	virtual void OpenL(MSpudManInterface * aSpudManInterface, TName& aName);
	virtual void Close();
	
	virtual TInt Input (TContextId aPdpId, const TInt aOperation, const TInt aParam=KErrNone);

	
	
#ifdef SYMBIAN_NETWORKING_UMTSR5
    virtual void SetIMCNSignalling(TBool aImCnSignalling);
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR5Requested& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR5Negotiated& aParam);
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR5Requested& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR5Negotiated& aParam) const;

#else
// !SYMBIAN_NETWORKING_UMTSR5 

	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Requested& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Negotiated& aParam);
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Requested& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Negotiated& aParam) const;
#endif 
// SYMBIAN_NETWORKING_UMTSR5 


	virtual TInt Set(TContextId aPdpId, const TTFTInfo& aParam);
	virtual TInt Set(TContextId aPdpId, const TTFTOperationCode& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TDataChannelV2& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TContextConfigGPRS& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TContextStatus& aParam);

	virtual TInt GetLastErrorCause(TContextId aPdpId, TInt& aLastErrorCause) const;

	virtual TInt Get(TContextId aPdpId, TTFTInfo& aParam) const;
	virtual TInt Get(TContextId aPdpId, TTFTOperationCode& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TDataChannelV2& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TContextConfigGPRS& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TContextStatus& aParam) const;

	virtual void Set(const RPacketService::TStatus aParam); // applies to all contexts
	virtual void Get(RPacketService::TStatus& aParam);// applies to all contexts

	virtual const TName& TsyName(void);

	virtual void SetContextTerminationErrorAndStop(TContextId aPdpId, TInt aErrorCode);

protected:
private:
	// DO NOT provide implementations to these methods: Copying this object should be disabled
	CPdpFsmInterface(CPdpFsmInterface&);
	CPdpFsmInterface& operator= (CPdpFsmInterface& aInput);
	
public:
protected:
private:
	// FSM Factory - churns out FSMs
	CPdpFsmFactory * iPdpFsmFactory;

	RPacketService::TStatus iNetworkStatus;	
	
	RSpudManInterface iSpudManInterface;

	TBool iSpudMainInterfaceSet;
};

#endif
// PDPFSM_H
