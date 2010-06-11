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
// PDP FSM factory header
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef CPDPFSMFACTORY_H
#define CPDPFSMFACTORY_H

#include <networking/umtsnifcontrolif.h>

#include "PDPFSM.h"
#include "reteldriverinput.h"
#include "cpdpfsm.h"
#include "tpdpstates.h"

/** factory (and owner) of FSM objects */
class CPdpFsmFactory : public CBase
{
	friend class CPdpFsm;
	friend class TPdpState;
	friend class TPdpStateInitialised; // Needs iEtelOpened
	friend class TPdpStateOpeningPhone; // Needs iEtelOpened
public:
	static CPdpFsmFactory * NewL();
	
	virtual ~CPdpFsmFactory();
	void InitL(TName& aTsyName, CPdpFsmInterface * aPdpFsmInterface);
	
	TInt NewFsmContext(TContextId aPdpId);
	TInt DeleteFsmContext(TContextId aPdpId);
	
	CPdpFsm*  GetFsmContext(TContextId aPdpId);
	
	inline TBool HaveFsmContext(TContextId aPdpId)   const;
	inline TBool ContextIdIsValid(TContextId aPdpId) const;
	inline TBool ContextIsValid(TContextId aPdpId)   const;
	
	void Close (void);
	
	void SpudInput (TContextId aPdpId, TInt aNotification, TInt aParam);

	void SetContextTerminationErrorAndStop(TContextId aPdpId, TInt aErrorCode);

	TInt GetLastErrorCause(TContextId aPdpId, TInt& aLastErrorCause);

	const TName& TsyName(void);

protected:	
private:
	CPdpFsmFactory();

public:
protected:
private:
	TFixedArray<CPdpFsm*, KMaxPdpContexts> iContexts;

	// the states
	TPdpStateInitialised   		 iStateInitialised;
	TPdpStateOpeningPhone		 iStateOpeningPhone;
	TPdpStateCreatingPrimary     iStateCreatingPrimary;
	TPdpStateActivatingPrimary   iStateActivatingPrimary;
	TPdpStateCreatingSecondary   iStateCreatingSecondary;
	TPdpStateCreatedSecondary    iStateCreatedSecondary;
	TPdpStateSettingQoS	   		 iStateSettingQoS;
	TPdpStateSettingTFT	   		 iStateSettingTFT;
	TPdpStateActivatingSecondary iStateActivatingSecondary;
	TPdpStateOpen		   		 iStateOpen;
	TPdpStateChangingQoS   		 iStateChangingQoS;
	TPdpStateChangingTFT   		 iStateChangingTFT;
	TPdpStateGettingNegQoS		 iStateGettingNegQoS;
	TPdpStateModifyingActive	 iStateModifyingActive;
	TPdpStateSuspended	   		 iStateSuspended;
	TPdpStateClosing	   		 iStateClosing;
	TPdpStateStopping	   		 iStateStopping;
	
	CPdpFsmInterface  * iPdpFsmInterface;
	REtelDriverInput  * iEtelDriverInput;
	
	TBool iEtelOpened;
	
	TName iTsyName;	
};


inline void CPdpFsmFactory::SpudInput (TContextId aPdpId, TInt aNotification, TInt aParam)
    {
	iPdpFsmInterface->SpudInput (aPdpId, aNotification, aParam);
    }


inline TBool CPdpFsmFactory::HaveFsmContext(TContextId aPdpId) const
    {
	return (iContexts[aPdpId] != NULL);
    }


inline TBool CPdpFsmFactory::ContextIdIsValid(TContextId aPdpId) const
    {
	return (aPdpId >= 0 && aPdpId < KMaxPdpContexts);
    }


inline TBool CPdpFsmFactory::ContextIsValid(TContextId aPdpId) const
    {
	return (ContextIdIsValid(aPdpId) && HaveFsmContext(aPdpId));
    }

#endif // CPDPFSMFACTORY_H
