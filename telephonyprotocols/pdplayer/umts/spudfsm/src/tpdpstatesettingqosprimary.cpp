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
// Setting QoS on primary context state
// 
//

/**
 @file 
 @internalComponent
*/
 
#include <networking\UmtsNifControlIf.h>
#include "tpdpstates.h"
#include "spudfsmdebuglogger.h"
#include "pdpfsmnmspace.h"
#include "cpdpfsm.h"

TInt TPdpStateSettingQoSPrimary::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateSettingQoSPrimary::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case PdpFsm::EQoSSet:
		aFsm.ChangeStateToActivatingPrimary();
		EtelDriverInput(aFsm, EtelDriver::EActivatePdp);
		return KErrNone;
	case PdpFsm::EQoSSetFailed:
		aFsm.ChangeStateToClosing();
		SpudManNotify (aFsm, KPrimaryContextCreated, aErrorCode); // or is it KContextActivateEvent?
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		return KErrNone;
	}
	
	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


