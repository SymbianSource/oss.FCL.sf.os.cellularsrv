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
// The interface to the PDP Context Finite State Machine.
// 
//

/**
 @file
 @internalComponent
*/


#ifndef MPDPFSMINTERFACE_H
#define MPDPFSMINTERFACE_H


#include <etelmm.h>
#include <etelqos.h>
#include <etelpckt.h>

#include <networking/umtsnifcontrolif.h>
#include <networking/mspudmaninterface.h>

/** Maximum number of PDP contexts supported by SPUD 
 This is derived from the number of bits available for NSAPIs
 Each PDP context corresponds with a NSAPI (and each NSAPI at most maps to one primary or secondary PDP Context)
 There are 4 bits allocated for NSAPIs with values 0 - 4 being reserved, leaving 11 values for PDP contexts
 @see GSM24.008
*/
const TContextId KMaxPdpContexts = 11; 

/** Internal pseudo-context ID meaning all contexts */
const TContextId KAllContexts = -1;

/** ID of the primary PDP context 
Primary context is the first context created, and it can be
created only once during Spud lifetime. 
There is a subltly here. There is always a default context.
This just happens to be the primary until it is deleted, at
which point one of the secondries will become the default.
This can happen in a network handover situation.
This is documented in the design spec. */
const TContextId KPrimaryContextId = 0; 


namespace SpudMan
{
	enum SpudManGuQoSSignal
	{
		EContextStart = 0x80,
		EContextCreate,
		EContextDelete,
		EContextActivate,
		ERegisterEventHandler,
		EContextSetEvents,
		EContextQoSSet,
		EContextModifyActive,
		EContextTFTModify,
		ENifSetDefaultQoS,
		EStartupPrimaryContextCreation,
		
		// sentinal
		ESpudManGuQoSSignalSentinal
	};

	enum SpudManSignal
	{
		ESetDefaultQoS = ESpudManGuQoSSignalSentinal,
		ECreatePrimaryPDPContext,
		ECreateSecondaryPDPContext,
		ESuspend,
		EResume,
		E1ryPdpActivate,
		ECancelContextCreate,
		
		EGetNegQoS,

		// sentinal
		ESpudManSignalSentinal
	};

}

class MPdpFsmInterface
/**
 * Definition of PDP Context Finite State Machine interface.
 * UmtsGprsSCPR comes with an implementation of this interface. 
 * The interface is to be used by SpudMan.
 *
 * @internalComponent
 */
 {
public:
	virtual void OpenL(MSpudManInterface* aSpudManInterface, TName& aName) = 0;
	virtual void Close() = 0;
	
	virtual TInt Input (TContextId aPdpId, const TInt aOperation, const TInt aParam=KErrNone) = 0;


#ifdef SYMBIAN_NETWORKING_UMTSR5
	// This is to set the status of IM CN signalling parameter
		
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR5Requested& aParam) = 0;
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR5Negotiated& aParam) = 0;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR5Requested& aParam) const = 0 ;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR5Negotiated& aParam) const = 0;

#else
// !SYMBIAN_NETWORKING_UMTSR5 

	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Requested& aParam) = 0;
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Negotiated& aParam) = 0;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Requested& aParam) const = 0;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Negotiated& aParam) const = 0;
#endif 
// SYMBIAN_NETWORKING_UMTSR5 



	virtual TInt Set(TContextId aPdpId, const TTFTInfo& aParam) = 0;
	virtual TInt Set(TContextId aPdpId, const TTFTOperationCode& aParam) = 0;
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TDataChannelV2& aParam) = 0;
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TContextConfigGPRS& aParam) = 0;
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TContextStatus& aParam) = 0;

	virtual TInt Get(TContextId aPdpId, TTFTInfo& aParam) const = 0;
	virtual TInt Get(TContextId aPdpId, TTFTOperationCode& aParam) const = 0;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TDataChannelV2& aParam) const = 0;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TContextConfigGPRS& aParam) const = 0;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TContextStatus& aParam) const = 0;

	virtual TInt GetLastErrorCause(TContextId aPdpId, TInt& aLastErrorCause) const = 0;

	virtual void Set(const RPacketService::TStatus aParam) = 0; // applies to all contexts
	virtual void Get(RPacketService::TStatus& aParam) = 0; // applies to all contexts

	// additional accessors
	virtual const TName& TsyName(void) = 0;

	virtual void SetContextTerminationErrorAndStop(TContextId aPdpId, TInt aErrorCode) = 0;

#ifdef SYMBIAN_NETWORKING_UMTSR5
   virtual void SetIMCNSignalling(TBool aImCnSignalling) = 0;
#endif //SYMBIAN_NETWORKING_UMTSR5
protected:
private:
	
};


#endif
// MPDPFSMINTERFACE_H
