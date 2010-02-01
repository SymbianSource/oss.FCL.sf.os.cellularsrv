// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Creating mbms context state
// 
//

/**
 @file 
 @internalComponent
*/
 
#include <networking/umtsnifcontrolif.h>
#include <networking/pdpdef.h>
#include "tpdpstates.h"
#include "spudfsmdebuglogger.h"
#include "pdpfsmnmspace.h"
#include "PDPFSM.h"
#include "cpdpfsm.h"


TInt TPdpStateCreatingMbms::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateCreatingMbms::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);
	
	
	switch (aOperation)
	{
	case PdpFsm::EMbmsPdpContextCreated:
		EtelDriverInput(aFsm, EtelDriver::ESessionUpdate);
		return KErrNone;

	case  SpudMan::EMbmsParameterUpdate:
		aFsm.ChangeStateToCreatedMbms();
		SpudManNotify(aFsm,KMbmsContextCreated, KErrNone);
		return KErrNone;

	case PdpFsm::EMbmsPdpContextCreatedFailed:
		iErrorCode = aErrorCode;
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		return KErrNone;

	case PdpFsm::EContextDeletedFailed:
		SPUDFSMVERBOSE_LOG(_L("**** DELETE FAILURE ****"));
		// fall through
	case PdpFsm::EContextDeleted:
		aFsm.ChangeStateToInitialised();
		SpudManNotify(aFsm, KMbmsContextCreated, iErrorCode);
		iErrorCode = KErrNone;
		return KErrNone;
	}
	
	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}
