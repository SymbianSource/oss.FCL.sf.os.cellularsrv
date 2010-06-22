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
// Open state
// 
//

/**
 @file 
 @internalComponent
*/



#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "tpdpstateopenTraces.h"
#endif

#include <etelpckt.h>
#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "pdpfsmnmspace.h"
#include "PDPFSM.h"
#include "cpdpfsm.h"

TInt TPdpStateOpen::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_1, ">>TPdpStateOpen::Input()");
	OstTraceDefExt2(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_2, "aOperation : %S(%d)", *(LogOperation(aFsm, aOperation)), aOperation);

	switch (aOperation)
	{
	case PdpFsm::EQoSProfileChangeNetwork:
//		RPacketQoS::TQoSR99_R4Negotiated
		SpudManNotify (aFsm, KContextParametersChangeEvent, KErrNone);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_3, "<<TPdpStateOpen::Input()");
		return KErrNone;
	case PdpFsm::EConfigGPRSChangeNetwork:
//		//RPacketContext::TContextConfigGPRS
		SpudManNotify (aFsm, KContextParametersChangeEvent, KErrNone);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_4, "<<TPdpStateOpen::Input()");
		return KErrNone;
	case PdpFsm::EContextStatusChangeNetwork:
		if (aFsm.iContextStatus == RPacketContext::EStatusSuspended)
    		{
			aFsm.ChangeStateToSuspended();
			SpudManNotify(aFsm, KContextBlockedEvent, KErrNone); 
    		}
		else if (aFsm.iContextStatus == RPacketContext::EStatusDeleted)
    		{
			aFsm.ChangeStateToClosing();
			EtelDriverInput(aFsm, EtelDriver::EContextDelete);
    		}
		else
    		{
			// no change in state - the upper layer must change the state if it needs to
			SpudManNotify (aFsm, KContextParametersChangeEvent, aErrorCode);
    		}
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_5, "<<TPdpStateOpen::Input()");
		return KErrNone;
	case SpudMan::EContextDelete:
		aFsm.ChangeStateToClosing();
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_6, "<<TPdpStateOpen::Input()");
		return KErrNone;
	case SpudMan::EContextQoSSet:
		aFsm.ChangeStateToChangingQoS();
		EtelDriverInput(aFsm, EtelDriver::ESetQoS);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_7, "<<TPdpStateOpen::Input()");
		return KErrNone;
	case SpudMan::EContextModifyActive:
		aFsm.ChangeStateToModifingActive();
		EtelDriverInput(aFsm, EtelDriver::EModifyActive);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_8, "<<TPdpStateOpen::Input()");
		return KErrNone;
	case SpudMan::EContextTFTModify:
		aFsm.ChangeStateToChangingTFT();
		EtelDriverInput(aFsm, EtelDriver::EChangeTft);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_9, "<<TPdpStateOpen::Input()");
		return KErrNone;
	case SpudMan::ESuspend: // think this is superfluous - DAN will see at review :)
		aFsm.ChangeStateToSuspended();
		SpudManNotify(aFsm, KContextBlockedEvent, KErrNone);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_10, "<<TPdpStateOpen::Input()");
		return KErrNone;
	case SpudMan::EMbmsSessionUpdate:
		EtelDriverInput(aFsm, EtelDriver::ESessionUpdate);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_11, "<<TPdpStateOpen::Input()");
		return KErrNone;
		
	case SpudMan::EMbmsParameterUpdate:
		SpudManNotify (aFsm, KContextParametersChangeEvent, aErrorCode);
		OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_12, "<<TPdpStateOpen::Input()");
		return KErrNone;
	}
	
	// default error handling
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, TPDPSTATEOPEN_INPUT_13, "<<TPdpStateOpen::Input()");
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


