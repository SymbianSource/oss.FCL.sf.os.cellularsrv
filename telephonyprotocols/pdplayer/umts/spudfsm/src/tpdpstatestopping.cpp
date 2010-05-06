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
// Stopping state
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

TInt TPdpStateStopping::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateStopping::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case PdpFsm::EContextDeleted:
		aFsm.ChangeStateToInitialised();
		SpudManNotify (aFsm, KPrimaryContextCreated, KErrGeneral);
		return KErrNone;			
	case PdpFsm::EContextDeletedFailed:
		// There isn't any corrective action that can be taken here. 
		SPUDFSMVERBOSE_LOG(_L("**** DELETE FAILURE ****"));
		SPUDFSM_LOG(_L("*** DELETE FAILURE ***"));
		aFsm.ChangeStateToInitialised();
		EtelDriverCancel (aFsm);
		SpudManNotify (aFsm, KPrimaryContextCreated, KErrNone);
		return KErrNone;
	case SpudMan::EContextDelete:
		// already doing this and don't want default action
		return KErrInUse;
	}

	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


