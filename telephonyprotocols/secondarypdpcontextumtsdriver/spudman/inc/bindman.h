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
// SPUD binder manager
// 
//

/**
 @file
 @internalComponent
*/

#ifndef BINDMAN_H
#define BINDMAN_H

#include "spudman.h"
#include <networking/mpdpfsminterface.h>

class CSpudNotify;
class CSpudMux;
class CSpudProtocol;

/**
PDP context states from the perspective of SpudMan
*/
enum TSpudContextStates
	{
	ESpudInactive = 0,
	/** Default QoS has been set and CNifIfLink::Start is awaited */
	ESpudHaveQos,
	/** FSM is creating the primary context */
	ESpudCreatingPrimary,
	/** Start has been called on the SPUD */
	ESpudStartingPrimary,
	/** CNifIfLink::Start on lower NIF called, waiting for LinkLayerUp or Down.*/
	ESpudStartingPrimaryLowerNif,
	/** Secondary context is in the process of being started */
	ESpudStartingSecondary,
	/** CNifIfLink::Start on lower NIF called, waiting for LinkLayerUp or Down.*/
	ESpudStartingSecondaryLowerNif,
	/* Lower NIF is up, waiting for retrieval of the negotiated QoS. */
	ESpudGettingNegQoS,
	/** Spud is up and ready to for data */
	ESpudUp,
	/** Spud is up but currently flowed off by lower NIF */
	ESpudFlowOff,
	/** Spud is up but currently flowed off by the network */
	ESpudSuspended,
	/** Spud is up but currently flowed off by the network and the lower NIF */
	ESpudFlowOffAndSuspended,
	/** Context is going down, but it must still be deleted */
	ESpudLinkDown,
	/** Lower NIF has been stopped */
	ESpudContextDelete,
	/** All communication with GUQoS is finished for this context. */
	ESpudWaitLinkDown,
	/** MNifIfNotify::LinkLayerDown called by lower NIF. Binder must be deleted */
	ESpudWaitBinderDelete
	};


/**
Binder class between lower NIF and SPUD
*/
NONSHARABLE_CLASS(CSpudBinderRef) : public CBase
	{
public:
	CSpudBinderRef(CBindMan& aBindMan);
	~CSpudBinderRef();
	void ConstructL(MNifIfNotify* aNotify, TContextId aContextId);
	void Bind(CNifIfLink* aNifLink, CNifIfBase* aNifBase);

	TBool MatchBase(const CNifIfBase* aBase) const;
	TBool IsBound() const;

	// Setters
	void SetState(TSpudContextStates aState);
    void SetError(TInt aError);

	// Getters
	CNifIfLink* NifLink() const;
	CNifIfBase* NifBase() const;
	CSpudNotify* Notify() const;
	TSpudContextStates State() const;
    TInt Error() const;
	
private:
	/** Reference to BindMan object */
	CBindMan& iBindMan;

	/* Pointer to SpudNotify object */
	CSpudNotify* iNotify;

	/* Pointer to CNifIfLink in lower NIF */
	CNifIfLink* iNifLink;

	/* Pointer to CNifIfBase in lower NIF */
	CNifIfBase* iNifBase;

	/** Current state of the PDP context associated with this lower NIF */
	TSpudContextStates iState;

    /** Context specific failure condition */
    TInt iError;
	};


/**
Binder management class
*/
NONSHARABLE_CLASS(CBindMan) : public CBase
	{
public:
	CBindMan(CSpudMan& aSpudMan, CSpudMux* aMux, CSpudProtocol* aProto);
	~CBindMan();

	void SetProtocolBaseL(CProtocolBase* aProtocolBase) const;
	CSpudBinderRef* GetRefL(TContextId aContextId) const;
	TContextId FindContextIdL(const CNifIfBase* aNifBase) const;
	CSpudBinderRef* GetAnyRefL() const;
	TUint NumContexts() const;
	TBool IsLastContext(TContextId aContextId)const;
	TUint SweepBinders();
	void LoadNifL(const TDesC& aName, CSpudBinderRef& aBinder);

	void MuxClosed();
	CSpudMux* TransferSpudMux();

	// Getters
	CSpudMux* SpudMux() const;
	CSpudMan* SpudMan() const;
	CSpudProtocol* SpudProtocol() const;
	
	CSpudBinderRef* GetNewRefForPrimaryL();
	CSpudBinderRef* GetNewRefForSecondaryL(TContextId& aContextId);

protected:
	void ReadLowerNifName(void);	

private:
	/** Reference to CSpudMan object */
	CSpudMan& iSpudMan;

	/** Pointer to CSpudMux object */
	CSpudMux* iMux;

	/** ETrue if it is our responsibility to delete the iMux object */
	TBool iDeleteMux;

	/** Pointer to CSpudProtocol object */
	CSpudProtocol* iSpudProtocol;

	/** Array of CSpudBinderRef objects */
	TFixedArray<CSpudBinderRef*, KMaxPdpContexts> iBinders;

	/** Name of lower NIF as configured in CommDB */
	TBuf<KConnMaxInterfaceName> iLowerNifName;
	};

#include "bindman.inl"

#endif
