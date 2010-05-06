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
// Creating secondary context state
// 
//

/**
 @file 
 @internalComponent
*/
 
#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "spudfsmdebuglogger.h"
#include "pdpfsmnmspace.h"
#include "PDPFSM.h"
#include "cpdpfsm.h"

TInt TPdpStateCreatingSecondary::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateCreatingSecondary::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case PdpFsm::E2ryPdpContextCreated:
		aFsm.ChangeStateToCreatedSecondary();
		SpudManNotify(aFsm, KSecondaryContextCreated, KErrNone);
		return KErrNone;
	case PdpFsm::E2ryPdpContextCreatedFailed:
		iErrorCode = aErrorCode;
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		return KErrNone;

	case PdpFsm::EContextDeletedFailed:
		SPUDFSMVERBOSE_LOG(_L("**** DELETE FAILURE ****"));
		// fall through
	case PdpFsm::EContextDeleted:
		aFsm.ChangeStateToInitialised();
		SpudManNotify(aFsm, KSecondaryContextCreated, iErrorCode);
		iErrorCode = KErrNone;
		return KErrNone;
	}
	
	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}

