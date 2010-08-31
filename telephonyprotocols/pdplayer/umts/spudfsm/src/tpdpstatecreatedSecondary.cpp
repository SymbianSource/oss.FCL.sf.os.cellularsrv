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
// State after secondary has been created, but not activated
// 
//

/**
 @file 
 @internalComponent
*/
 


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "tpdpstatecreatedSecondaryTraces.h"
#endif

#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "pdpfsmnmspace.h"
#include "pdpfsmnmspace.h"
#include "PDPFSM.h"
#include "cpdpfsm.h"

TInt TPdpStateCreatedSecondary::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATEDSECONDARY_INPUT_1, ">>TPdpStateCreatedSecondary::Input()");
	OstTraceDefExt2(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATEDSECONDARY_INPUT_2, "aOperation : %S(%d)", *(LogOperation(aFsm, aOperation)), aOperation);

	switch (aOperation)
	{
	case SpudMan::EContextActivate: 
		aFsm.ChangeStateToActivatingSecondary();
		EtelDriverInput(aFsm, EtelDriver::EActivatePdp);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATEDSECONDARY_INPUT_3, "<<TPdpStateCreatedSecondary::Input()");
		return KErrNone;		
	case SpudMan::EContextQoSSet:
		aFsm.ChangeStateToSettingQoS();
		EtelDriverInput(aFsm, EtelDriver::ESetQoS);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATEDSECONDARY_INPUT_4, "<<TPdpStateCreatedSecondary::Input()");
		return KErrNone;
	case SpudMan::EContextTFTModify:
		aFsm.ChangeStateToSettingTFT();
		EtelDriverInput(aFsm, EtelDriver::ESetTft);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATEDSECONDARY_INPUT_5, "<<TPdpStateCreatedSecondary::Input()");
		return KErrNone;
	}
	
	// default error handling
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATECREATEDSECONDARY_INPUT_6, "<<TPdpStateCreatedSecondary::Input()");
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


