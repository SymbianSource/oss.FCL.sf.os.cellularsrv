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
// Stopping state
// 
//

/**
 @file 
 @internalComponent
*/



#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "tpdpstatestoppingTraces.h"
#endif

#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "pdpfsmnmspace.h"
#include "PDPFSM.h"
#include "cpdpfsm.h"

TInt TPdpStateStopping::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATESTOPPING_INPUT_1, ">>TPdpStateStopping::Input()");
	OstTraceDefExt2(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATESTOPPING_INPUT_2, "aOperation : %S(%d)", *(LogOperation(aFsm, aOperation)), aOperation);

	switch (aOperation)
	{
	case PdpFsm::EContextDeleted:
		aFsm.ChangeStateToInitialised();
		SpudManNotify (aFsm, KPrimaryContextCreated, KErrGeneral);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATESTOPPING_INPUT_3, "<<TPdpStateStopping::Input()");
		return KErrNone;			
	case PdpFsm::EContextDeletedFailed:
		// There isn't any corrective action that can be taken here. 
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATESTOPPING_INPUT_4, "**** DELETE FAILURE ****");
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATESTOPPING_INPUT_5, "*** DELETE FAILURE ***");
		aFsm.ChangeStateToInitialised();
		EtelDriverCancel (aFsm);
		SpudManNotify (aFsm, KPrimaryContextCreated, KErrNone);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATESTOPPING_INPUT_6, "<<TPdpStateStopping::Input()");
		return KErrNone;
	case SpudMan::EContextDelete:
		// already doing this and don't want default action
	    OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATESTOPPING_INPUT_7, "<<TPdpStateStopping::Input()");
		return KErrInUse;
	}

	// default error handling
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATESTOPPING_INPUT_8, "<<TPdpStateStopping::Input()");
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


