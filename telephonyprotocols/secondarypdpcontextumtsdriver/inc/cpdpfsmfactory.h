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

#include <networking\UMTSNifControlIf.h>
#include "rpdpfsminterface.h"
#include "reteldriverinput.h"
#include "cpdpfsm.h"
#include "spudmanif.h"
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
	void InitL(MSpudManInterface * aSpudManInterface, TName& aTsyName, RPdpFsmInterface * aPdpFsmInterface);
	
	CPdpFsm * GetContext (TContextId aPdpId);
	
	void Close (void);
	
	void SpudInput (TContextId aPdpId, TInt aNotification, TInt aParam);
	void SetContextTerminationErrorAndStop(TContextId aPdpId, TInt aErrorCode);

	const TName& TsyName(void);

protected:	
private:
	CPdpFsmFactory();

public:
protected:
private:
	TFixedArray<CPdpFsm, KMaxPdpContexts> iContexts;

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
	TPdpStateModifyingActive	 iStateModifyingActive;
	TPdpStateSuspended	   		 iStateSuspended;
	TPdpStateClosing	   		 iStateClosing;
	TPdpStateStopping	   		 iStateStopping;
	
	MSpudManInterface * iSpudManInterface;
	RPdpFsmInterface  * iPdpFsmInterface;
	REtelDriverInput  * iEtelDriverInput;
	
	TBool iEtelOpened;
	
	TName iTsyName;	
};

inline void CPdpFsmFactory::SpudInput (TContextId aPdpId, TInt aNotification, TInt aParam)
{
	iSpudManInterface->Input (aPdpId, aNotification, aParam);
}

inline void CPdpFsmFactory::SetContextTerminationErrorAndStop(TContextId aPdpId, TInt aErrorCode)
	{
	iSpudManInterface->SetContextTerminationErrorAndStop(aPdpId, aErrorCode);
	}

#endif // CPDPFSMFACTORY_H
