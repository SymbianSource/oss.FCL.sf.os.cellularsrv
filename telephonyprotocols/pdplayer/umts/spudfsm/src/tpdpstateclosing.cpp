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
// Closing state
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

TInt TPdpStateClosing::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateClosing::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case PdpFsm::EContextDeleted:
		aFsm.ChangeStateToInitialised();
		SpudManNotify (aFsm, KContextDeleteEvent, KErrNone);
		return KErrNone;			
	case PdpFsm::EContextDeletedFailed:
		// There isn't any corrective action that can be taken here. 
		// It is assumed when this context is used again, there will be a failure 
		// or a failure elsewhere down the line that will correct the situation, ultimately
		// involving the user. FSM and ETelDriver are not in a position to take any corrective action.
		SPUDFSMVERBOSE_LOG(_L("**** DELETE FAILURE ****"));
		aFsm.ChangeStateToInitialised();
		SpudManNotify (aFsm, KContextDeleteEvent, aErrorCode);
		return KErrNone;			
	case SpudMan::EContextDelete:
		// already doing this and don't want default action
		return KErrInUse;
	}

	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


