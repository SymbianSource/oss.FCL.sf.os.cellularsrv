// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Creating primary context state
// 
//

/**
 @file 
 @internalComponent
*/
 


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "tpdpstatecreatingprimaryTraces.h"
#endif

#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "pdpfsmnmspace.h"
#include "cpdpfsm.h"
#include "PDPFSM.h"

TInt TPdpStateCreatingPrimary::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGPRIMARY_INPUT_1, ">>TPdpStateCreatingPrimary::Input()");
	OstTraceDefExt2(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGPRIMARY_INPUT_2, "aOperation : %S(%d)", *(LogOperation(aFsm, aOperation)), aOperation);

	switch (aOperation)
	{
	case PdpFsm::E1ryPdpContextCreated:
		aFsm.ChangeStateToActivatingPrimary();
		EtelDriverInput(aFsm, EtelDriver::EActivatePdp);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGPRIMARY_INPUT_3, "<<TPdpStateCreatingPrimary::Input()");
		return KErrNone;
	case PdpFsm::E1ryPdpContextCreatedFailed: 
		aFsm.ChangeStateToInitialised();
		SpudManNotify (aFsm, KPrimaryContextCreated, aErrorCode); // or is it KContextActivateEvent?
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGPRIMARY_INPUT_5, "<<TPdpStateCreatingPrimary::Input()");
		return KErrNone;
   case SpudMan::ECancelContextCreate:
		aFsm.ChangeStateToStopping();
		EtelDriverCancel(aFsm);
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGPRIMARY_INPUT_6, "<<TPdpStateCreatingPrimary::Input()");
		return KErrNone;
	}

	// default error handling
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGPRIMARY_INPUT_7, "<<TPdpStateCreatingPrimary::Input()");
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


