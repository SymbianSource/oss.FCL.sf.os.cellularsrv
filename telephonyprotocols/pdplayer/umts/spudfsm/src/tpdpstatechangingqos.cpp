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
// ChangingQoS state
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
#include "cpdpfsm.h"

TInt TPdpStateChangingQoS::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateChangingQoS::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case PdpFsm::EQoSSet:
		aFsm.ChangeStateToOpen();
		SpudManNotify (aFsm, KContextQoSSetEvent, KErrNone);
		return KErrNone;
	case PdpFsm::EQoSSetFailed:
		aFsm.ChangeStateToOpen();
		SpudManNotify (aFsm, KContextQoSSetEvent, aErrorCode);
		return KErrNone;
	
	// no default	
	}
	
	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


