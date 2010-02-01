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
// SPUD event manager
// 
//

/**
 @file
 @internalComponent
*/

#ifndef SPUDMAN_H
#define SPUDMAN_H

#include <e32base.h>
#include <comms-infras/nifif.h>
#include <networking/umtsnifcontrolif.h>
#include <comms-infras/commsdebugutility.h>
#include <networking/mspudmaninterface.h>

#include "rpdpfsminterface.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <comms-infras/nifprvar_internal.h>
#endif

const TUint KSpudMajorVersionNumber=0;
const TUint KSpudMinorVersionNumber=0;
const TUint KSpudBuildVersionNumber=2;

/** Longest protocol name supported */
const TUint KMaxProtocolName = 20;

#ifdef __FLOG_ACTIVE
// Logging tags
_LIT8(KSpudFirstTag,"spud");
_LIT8(KSpudLog,"spudman.txt");
#endif

class CBinderSweeperNotifierCb;
class CBindMan;
class CSpudBinderRef;
class RPdpFsmInterface;

/**
SPUD Event Manager.
It is responsible for routing events throughout the SPUD and between interface points.
*/
NONSHARABLE_CLASS(CSpudMan) : public CNifIfLink, public MSpudManInterface
	{
public:
	__FLOG_DECLARATION_MEMBER_MUTABLE;

	CSpudMan(CNifIfFactory& aFactory, MNifIfNotify* aNotify);
    ~CSpudMan();
	void ConstructL(CBindMan* aBindMan);
	void Panic(TInt aReason=0) const;

	TBool AreQoSEventsEnabled() const;

	// Getters
	CBindMan* BindMan() const;	
	MNifIfNotify* Notify() const;

	// Events from elsewhere
	TInt GuqosInput(TUint aName, TDes8& aOption);
	void LinkLayerUp(TContextId aContextId);
	void LinkLayerDown(TContextId aContextId, TInt aReason, MNifIfNotify::TAction aAction);
	void StopSending(TContextId aContextId);
	void StartSending(TContextId aContextId);

	// from MSpudManInterface
	void Input (TContextId aPdpId, TInt aEvent, TInt aParam);
	void SetContextTerminationErrorAndStop(TContextId aContextId, TInt aErrorCode);

	// from CNifIfLink
    void AuthenticateComplete(TInt aResult);
	TInt Start();
	void Stop(TInt aReason, MNifIfNotify::TAction aAction);
	CNifIfBase* GetBinderL(const TDesC& aName);
	void Restart(CNifIfBase* aIf);

	// from CNifIfBase
	TInt Send(RMBufChain& aPdu, TAny* aSource=0);
	void Info(TNifIfInfo& aInfo) const;
	TInt Notification(TAgentToNifEventType aEvent, void * aInfo);

    // SPUD methods
    void IfProgress(TContextId aContextId, TInt aStage, TInt aError);
    void IfProgress(TContextId aContextId, TSubConnectionUniqueId aSubConnectionUniqueId, TInt aStage, TInt aError);
	TInt Notification(TContextId aContextId, TNifToAgentEventType aEvent, void * aInfo);
	TInt ReadInt(TContextId aContextId, const TDesC& aField, TUint32& aValue);
	TInt ReadDes(TContextId aContextId, const TDesC& aField, TDes8& aValue);
	TInt ReadDes(TContextId aContextId, const TDesC& aField, TDes16& aValue);
	
	void DisposeOfBinder(CSpudBinderRef* aRef);
	void SweepBindersAndNotify();
	
protected:
	void ReadDefaultQoS(RPacketQoS::TQoSR99_R4Requested& aQos) const;

	
#ifdef SYMBIAN_NETWORKING_UMTSR5
	
	// IM CN signalling flag, Setter and Getter methods
	void SetIMCNSignallingFlagPcoL(RPacketContext::TProtocolConfigOptionV2& aPco);	
	TBool GetIMCNSignallingFlagPcoL(RPacketContext::TProtocolConfigOptionV2& aPco);
	
	void ReadDefaultR5QoS(RPacketQoS::TQoSR5Requested& aQos) const;
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

	void ReadTsyName(TName& aTsyName) const;
	void SetTerminateError(TInt aError);
	void SetTerminateError(TContextId aContextId, TInt aError);
	void RetrieveGprsConfig(RPacketContext::TContextConfigGPRS& aConfigGprs) const;
	void RetrieveGprsCompression(TUint& aCompression) const;
	void RetrieveGprsAnonymousAccess(RPacketContext::TAnonymousAccess& aAnonymous) const;

	void FillInParameters(TContextParameters& aParams, TContextId aContextId, TInt aError=KErrNone) const;
	void FillInContextConfig(TContextConfig& aConfig, TContextId aContextId) const;
	void RaiseEvent(TUint aName, TDes8& aOption) const;
	
	virtual TInt Control(TUint aLevel,TUint aName,TDes8& aOption, TAny* aSource=0);

	// GUQoS event senders
	void SendPrimaryContextCreated(TContextId aContextId, TInt aError);
	void SendContextBlockedEvent(TContextId aContextId);
	void SendContextUnblockedEvent(TContextId aContextId);
	void SendSecondaryContextCreated(TContextId aContextId, TInt aError);
	void SendContextQoSSetEvent(TContextId aContextId, TInt aError);
	void SendContextTFTModifiedEvent(TContextId aContextId, TTFTOperationCode aTFTOperationCode, TInt aError);
	void HandleGetNegQoSEvent(TContextId aContextId, TInt aError);
	void SendContextModifyActiveEvent(TContextId aContextId, TContextConfig& aContextConfig, TInt aError);
	void SendContextDeleteEvent(TContextId aContextId);
	void SendNetworkStatusEvent(TNetworkEventCode aEventCode, RPacketService::TStatus aStatus);
	void SendContextActivateEvent(TContextId aContextId, TContextConfig& aContextConfig, TInt aError);
	void SendContextParametersChangeEvent(TContextId aContextId, TContextConfig& aContextConfig, TInt aError);

private:
	void InitPdpFsmInterfaceL();
	
	void HandlePrimaryContextCreatedEvent(TContextId aContextId, TInt aError);
	void HandleContextDeleteEvent(TContextId aContextId, TInt aError);
	void HandleSecondaryContextCreatedEvent(TContextId aContextId, TInt aError);
	void HandleContextActivateEvent(TContextId aContextId, TInt aError);
	void HandleContextQoSSetEvent(TContextId aContextId, TInt aError);
	void HandleContextTFTModifiedEvent(TContextId aContextId, TInt aError);
	void HandleContextModifyActiveEvent(TContextId aContextId, TInt aError);
	void HandleNetworkStatusEvent();
	void HandleContextParametersChangeEvent(TContextId aContextId, TInt aError);
	void HandleContextBlockedEvent(TContextId aContextId);
	void HandleContextUnblockedEvent(TContextId aContextId);
	
	void SetupSipServerAddrRetrievalL(RPacketContext::TProtocolConfigOptionV2& aPco);
	void SetSipServerAddrL(const RPacketContext::TProtocolConfigOptionV2& aPco);

	void StopContext(TContextId aContextId, TInt aReason, MNifIfNotify::TAction aAction);

private:
	CBindMan* iBindMan;

	/** GUQoS event notifier */
	MNifEvent* iQosEventHandler;

	/** Protocol name */
	TBuf<KMaxProtocolName> iName;

	/** Whether events should be sent to GUQoS or not */
	TBool iQosEventsEnabled;

	/** Interface to ETEL */
	RPdpFsmInterface iPdpFsmInterface;

	/** Temporary storage of context configuration, here to avoid stack overflow */
    TContextConfig iTempContextConfig;
    
	/** Temporary storage of TFT, here to avoid stack overflow */
    mutable TTFTInfo iTempTftInfo;
    
	/** Temporary storage of GPRS context configuration, here to avoid stack overflow */
    mutable RPacketContext::TContextConfigGPRS iTempGprsContext;
    
	/** Temporary storage of channel configuration, here to avoid stack overflow */
    RPacketContext::TDataChannelV2 iTempDataChannelV2;
    
    /** Error code which indicates the cause of the SPUD shutdown */
    TInt iTerminateError;
    
    /** First Error code from ETel */
    TInt iETelTerminateError;
    
    /** Asynchronous sweep of binders marked for deletion, with optional notification to Nifman */
    CBinderSweeperNotifierCb* iBinderSweeperNotifierCb;
	
    /** Default QoS sent from GuQoS before this Nif was ready */
    HBufC8* iParkedDefaultQoS;
    
    /** The address of the P-CSCF (Sip server) */
    RArray<TInetAddr> iSipServerAddr;
    

	/** It may be necessary to report a context status to GUQoS, which is different to the formal
	context status at the very moment of reporting. */
	RPacketContext::TContextStatus iContextStatusOverride;
	
	/** Store the Primary ContextId that might be changed over time */
	TContextId iPrimaryContextId;
	
	};


/** Asynchronously calls into Nifman, to trigger sweeping of binders marked for deletion &
    if necessary, Nifman notification.
	
	When the lower NIF signals LinkLayerDown on SPUD, SPUD has to delete it.
	However, this deletion cannot be done from within CSpudMan::LinkLayerDown, because
	the lower NIF's may still access its members after CSpudMan::LinkLayerDown returns.
	That is, the lower NIF must be deleted asynchronousely. However, if this is the last 
	lower NIF, SPUD must signal MNifIfNotify::LinkLayerDown on Nifman. This triggers asynchronous deletion
	of SPUD itself. So, there can be a race condition between asynchronous deletion of SPUD by Nifman,
	and deletion of the lower NIF by SPUD.	*/	
NONSHARABLE_CLASS(CBinderSweeperNotifierCb): public CAsyncOneShot
	{
	public:
		CBinderSweeperNotifierCb(CSpudMan& aSpudMan);
		
		// CAsyncOneShot virtuals
		void Call();
		void RunL();
	
	private:
		/** We call into Spudman that actually performs the sweeping & notification */
		CSpudMan& iSpudMan;		
    };
	

#endif
