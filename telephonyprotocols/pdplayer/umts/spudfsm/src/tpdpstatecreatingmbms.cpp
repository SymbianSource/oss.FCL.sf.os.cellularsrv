// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "tpdpstatecreatingmbmsTraces.h"
#endif

#include <networking/umtsnifcontrolif.h>
#include <networking/pdpdef.h>
#include "tpdpstates.h"
#include "pdpfsmnmspace.h"
#include "PDPFSM.h"
#include "cpdpfsm.h"


TInt TPdpStateCreatingMbms::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGMBMS_INPUT_1, ">>TPdpStateCreatingMbms::Input()");
	OstTraceDefExt2(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGMBMS_INPUT_2, "aOperation : %S(%d)", *(LogOperation(aFsm, aOperation)), aOperation);
	
	
	switch (aOperation)
	{
	case PdpFsm::EMbmsPdpContextCreated:
		EtelDriverInput(aFsm, EtelDriver::ESessionUpdate);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGMBMS_INPUT_3, "<<TPdpStateCreatingMbms::Input()");
		return KErrNone;

	case  SpudMan::EMbmsParameterUpdate:
		aFsm.ChangeStateToCreatedMbms();
		SpudManNotify(aFsm,KMbmsContextCreated, KErrNone);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGMBMS_INPUT_4, "<<TPdpStateCreatingMbms::Input()");
		return KErrNone;

	case PdpFsm::EMbmsPdpContextCreatedFailed:
		iErrorCode = aErrorCode;
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGMBMS_INPUT_5, "<<TPdpStateCreatingMbms::Input()");
		return KErrNone;

	case PdpFsm::EContextDeletedFailed:
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGMBMS_INPUT_6, "**** DELETE FAILURE ****");
		// fall through
	case PdpFsm::EContextDeleted:
		aFsm.ChangeStateToInitialised();
		SpudManNotify(aFsm, KMbmsContextCreated, iErrorCode);
		iErrorCode = KErrNone;
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGMBMS_INPUT_7, "<<TPdpStateCreatingMbms::Input()");
		return KErrNone;
	}
	
	// default error handling
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATINGMBMS_INPUT_8, "<<TPdpStateCreatingMbms::Input()");
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}
