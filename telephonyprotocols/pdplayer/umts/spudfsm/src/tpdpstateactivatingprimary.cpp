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
// Activating primary state
// 
//

/**
 @file 
 @internalComponent
*/
 


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "tpdpstateactivatingprimaryTraces.h"
#endif

#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "pdpfsmnmspace.h"
#include "cpdpfsm.h"
#include "PDPFSM.h"
	
TInt TPdpStateActivatingPrimary::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEACTIVATINGPRIMARY_INPUT_1, ">>TPdpStateActivatingPrimary::Input()");
	OstTraceDefExt2(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEACTIVATINGPRIMARY_INPUT_2, "aOperation : %S(%d)", *LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case PdpFsm::EPdpActivated:
		// GuQoS assumes the primary context becomes active when created, so the context status
		// must be updated before sending the event
		aFsm.Set(RPacketContext::EStatusActive);
		aFsm.ChangeStateToOpen();
		SpudManNotify (aFsm, KPrimaryContextCreated, KErrNone);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEACTIVATINGPRIMARY_INPUT_3, "<<TPdpStateActivatingPrimary::Input()");
		return KErrNone;
	case PdpFsm::EPdpActivatedFailed:
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEACTIVATINGPRIMARY_INPUT_4, "*** FAILURE ***");
		aFsm.ChangeStateToClosing();
		
		SpudManNotify (aFsm, KPrimaryContextCreated, aErrorCode);
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEACTIVATINGPRIMARY_INPUT_5, "<<TPdpStateActivatingPrimary::Input()");
		return KErrNone;
  case SpudMan::ECancelContextCreate:
		aFsm.ChangeStateToStopping();
		EtelDriverCancel(aFsm);
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEACTIVATINGPRIMARY_INPUT_6, "<<TPdpStateActivatingPrimary::Input()");
		return KErrNone;
	// no default	
	}

	// default error handling
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEACTIVATINGPRIMARY_INPUT_7, "<<TPdpStateActivatingPrimary::Input()");
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


