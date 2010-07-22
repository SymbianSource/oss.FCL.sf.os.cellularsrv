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
// tpdpstatemodifyingactive.cpp
// Modifying active context state
// 
//

/**
 @file 
 @internalComponent
*/
 


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "tpdpstatemodifingactiveTraces.h"
#endif

#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "pdpfsmnmspace.h"
#include "cpdpfsm.h"

TInt TPdpStateModifyingActive::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEMODIFYINGACTIVE_INPUT_1, ">>TPdpStateModifyingActive::Input()");
	OstTraceDefExt2(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEMODIFYINGACTIVE_INPUT_2, "aOperation : %S(%d)", *(LogOperation(aFsm, aOperation)), aOperation);

	switch (aOperation)
	{
	case PdpFsm::EPdpContextModified:
		aFsm.ChangeStateToOpen();
		SpudManNotify(aFsm, KContextModifyActiveEvent, KErrNone);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEMODIFYINGACTIVE_INPUT_3, "<<TPdpStateModifyingActive::Input()");
		return KErrNone;
	case PdpFsm::EPdpContextModifiedFailed:
		aFsm.ChangeStateToOpen();
		SpudManNotify(aFsm, KContextModifyActiveEvent, aErrorCode);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEMODIFYINGACTIVE_INPUT_4, "<<TPdpStateModifyingActive::Input()");
		return KErrNone;
	}
	
	// default error handling
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEMODIFYINGACTIVE_INPUT_5, "<<TPdpStateModifyingActive::Input()");
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


