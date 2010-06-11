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
// Changing TFT state
// 
//

/**
 @file 
 @internalComponent
*/
 


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "tpdpstatechangingtftTraces.h"
#endif

#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "pdpfsmnmspace.h"
#include "cpdpfsm.h"

TInt TPdpStateChangingTFT::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECHANGINGTFT_INPUT_1, ">>TPdpStateChangingTFT::Input()");
	OstTraceDefExt2(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECHANGINGTFT_INPUT_2, "aOperation : %S(%d)", *(LogOperation(aFsm, aOperation)), aOperation);

	switch (aOperation)
	{
	case PdpFsm::ETftChanged:
		aFsm.ChangeStateToOpen();
		SpudManNotify (aFsm, KContextTFTModifiedEvent, KErrNone);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECHANGINGTFT_INPUT_3, "<<TPdpStateChangingTFT::Input()");
		return KErrNone;
	case PdpFsm::ETftChangedFailed:
		aFsm.ChangeStateToOpen();
		SpudManNotify (aFsm, KContextTFTModifiedEvent, aErrorCode);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECHANGINGTFT_INPUT_4, "<<TPdpStateChangingTFT::Input()");
		return KErrNone;
		
	// no default
	}

	// default error handling
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECHANGINGTFT_INPUT_5, "<<TPdpStateChangingTFT::Input()");
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}

