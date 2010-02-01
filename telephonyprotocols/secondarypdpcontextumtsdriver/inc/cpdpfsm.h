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
// PDP FSM header
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef CPDPFSM_H
#define CPDPFSM_H

#include <e32base.h>
#include <etelmm.h>
#include <etelqos.h>
#include <networking\UMTSNifControlIf.h>
#include "reteldriverinput.h"
#include "pdpfsmnmspace.h"

#include "tpdpstates.h"

class CPdpFsmFactory;
class RPacketQoS;
class TQoSR99_R4Requested;
class TQoSR99_R4Negotiated;


#ifdef SYMBIAN_NETWORKING_UMTSR5
class TQoSR5Requested;
class TQoSR5Negotiated; 
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

class TFTInfo;
class TFTOperationCode;

class CPdpFsm : public CBase
{
	friend class TPdpState;
	friend class TPdpStateInitialised;
	friend class TPdpStateOpeningPhone;
	friend class TPdpStateCreatingPrimary;
	friend class TPdpStateActivatingPrimary;
	friend class TPdpStateCreatingSecondary;
	friend class TPdpStateCreatedSecondary;
	friend class TPdpStateSettingQoS;
	friend class TPdpStateSettingTFT;
	friend class TPdpStateActivatingSecondary;
	friend class TPdpStateOpen;
	friend class TPdpStateChangingQoS;
	friend class TPdpStateChangingTFT;
	friend class TPdpStateModifyingActive;
	friend class TPdpStateSuspended;
	friend class TPdpStateClosing;
	friend class TPdpStateStopping;
	
public:
	CPdpFsm();
	~CPdpFsm();
	
	TInt Input (const TInt aOperation, const TInt aParam);
	
	void Get(RPacketQoS::TQoSR99_R4Requested& aParam);
	void Get(RPacketQoS::TQoSR99_R4Negotiated& aParam);

	
#ifdef SYMBIAN_NETWORKING_UMTSR5
	void Get(RPacketQoS::TQoSR5Requested& aParam);
	void Get(RPacketQoS::TQoSR5Negotiated& aParam);
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

	void Get(TTFTInfo& aParam);
	void Get(TTFTOperationCode& aParam);
	void Get(RPacketContext::TDataChannelV2& aParam);
	void Get(RPacketContext::TContextConfigGPRS& aParam);
	void Get(RPacketContext::TContextStatus& aParam);
 	
	void Set(const RPacketQoS::TQoSR99_R4Requested& aParam);
	void Set(const RPacketQoS::TQoSR99_R4Negotiated& aParam);
	
	
#ifdef SYMBIAN_NETWORKING_UMTSR5
	void Set(const RPacketQoS::TQoSR5Requested& aParam);
	void Set(const RPacketQoS::TQoSR5Negotiated& aParam);
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

	void Set(const TTFTInfo& aParam);
	void Set(const TTFTOperationCode& aParam);
	void Set(const RPacketContext::TDataChannelV2& aParam);
	void Set(const RPacketContext::TContextConfigGPRS& aParam);

	void Set(const RPacketContext::TContextStatus& aParam);
   
    void InitL (TContextId aPdpId, CPdpFsmFactory * aPdpFsmFactory, REtelDriverInput * aEtelDriverInput);
    
    void EtelInput (EtelDriver::TEtelInput aOperation);
	void EtelCancel (void);
    void SpudInput (TInt aNotification, TInt aParam);
    void GetLastErrorCause  (TInt& aLastErrorCause);
    
protected:
private:
	void ChangeStateToInitialised(void);
	void ChangeStateToOpeningPhone(void);
	void ChangeStateToCreatingPrimary(void);
	void ChangeStateToActivatingPrimary(void);
	void ChangeStateToCreatingSecondary(void);
	void ChangeStateToCreatedSecondary(void);
	void ChangeStateToSettingQoS(void);
	void ChangeStateToSettingTFT(void);
	void ChangeStateToActivatingSecondary(void);
	void ChangeStateToOpen(void);
	void ChangeStateToChangingQoS(void);
	void ChangeStateToChangingTFT(void);
	void ChangeStateToModifingActive(void);
	void ChangeStateToSuspended(void);
	void ChangeStateToClosing(void);
	void ChangeStateToStopping(void);

	
public:
protected:
private:
	
	// local data cache for this context
	RPacketQoS::TQoSR99_R4Requested    iQoSReq;
	RPacketQoS::TQoSR99_R4Negotiated   iQoSNeg;

	
#ifdef SYMBIAN_NETWORKING_UMTSR5
	RPacketQoS::TQoSR5Requested    iQoSR5Req;
	RPacketQoS::TQoSR5Negotiated   iQoSR5Neg;
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

	RPacketContext::TDataChannelV2     iDataChannelV2;
	RPacketContext::TContextStatus     iContextStatus;
	TTFTInfo 						   iTFT;
	TTFTOperationCode				   iTFTOperationCode;
	RPacketContext::TContextConfigGPRS iGPRS;

	CPdpFsmFactory   * iPdpFsmFactory; // need stuff from  factory
	REtelDriverInput * iEtelDriverInput;
	
	TPdpState * iState; // current state
	
	TContextId  iPdpId; // our instance PDP context id - fixed when allocateed

	enum TContextType
	{
		ENone,
		EPrimary,
		ESecondary
	} iContextType;
	

};



#endif // CPDPFSM_H



