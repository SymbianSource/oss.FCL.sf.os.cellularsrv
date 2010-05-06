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
// Changing TFT state
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

TInt TPdpStateChangingTFT::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateChangingTFT::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case PdpFsm::ETftChanged:
		aFsm.ChangeStateToOpen();
		SpudManNotify (aFsm, KContextTFTModifiedEvent, KErrNone);
		return KErrNone;
	case PdpFsm::ETftChangedFailed:
		aFsm.ChangeStateToOpen();
		SpudManNotify (aFsm, KContextTFTModifiedEvent, aErrorCode);
		return KErrNone;
		
	// no default
	}

	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}

