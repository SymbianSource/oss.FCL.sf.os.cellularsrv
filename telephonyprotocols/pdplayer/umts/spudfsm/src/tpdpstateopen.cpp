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
// Open state
// 
//

/**
 @file 
 @internalComponent
*/

#include <etelpckt.h>
#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "spudfsmdebuglogger.h"
#include "pdpfsmnmspace.h"
#include "PDPFSM.h"
#include "cpdpfsm.h"

TInt TPdpStateOpen::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateOpen::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	TInt result = KErrNone;
	
	switch (aOperation)
	{
	case PdpFsm::EQoSProfileChangeNetwork:
//		RPacketQoS::TQoSR99_R4Negotiated
		SpudManNotify (aFsm, KContextParametersChangeEvent, KErrNone);
		break;
	case PdpFsm::EConfigGPRSChangeNetwork:
//		//RPacketContext::TContextConfigGPRS
		SpudManNotify (aFsm, KContextParametersChangeEvent, KErrNone);
		break;
	case PdpFsm::EContextStatusChangeNetwork:
		if (aFsm.iContextStatus == RPacketContext::EStatusSuspended)
    		{
			aFsm.ChangeStateToSuspended();
			SpudManNotify(aFsm, KContextBlockedEvent, KErrNone); 
    		}
		// network initiated context deletion - inform the state machine that
		// it needs to do 'normal' deletion handling
		else if (aFsm.iContextStatus == RPacketContext::EStatusInactive)
		    {
		    SpudManNotify(aFsm, KContextDeleteEvent, KErrNone);
		    }
		else if (aFsm.iContextStatus == RPacketContext::EStatusDeleted)
    		{
			  SpudManNotify(aFsm, KContextDeleteEvent, KErrNone);
    		}
		else if (aFsm.iContextStatus == RPacketContext::EStatusDeactivating)
		    {
		    // context is being deactivated (torn down internally), this should never happen
		    // however it is likely that if it were implemented this way, then the Inactive 
		    // would soon follow. 
		    }
		else
    		{
			// no change in state - the upper layer must change the state if it needs to
			SpudManNotify (aFsm, KContextParametersChangeEvent, aErrorCode);
    		}
		break;
	case SpudMan::EContextDelete:
		aFsm.ChangeStateToClosing();
		EtelDriverInput(aFsm, EtelDriver::EContextDelete);
		break;
	case SpudMan::EContextQoSSet:
		aFsm.ChangeStateToChangingQoS();
		EtelDriverInput(aFsm, EtelDriver::ESetQoS);
		break;
	case SpudMan::EContextModifyActive:
		aFsm.ChangeStateToModifingActive();
		EtelDriverInput(aFsm, EtelDriver::EModifyActive);
		break;
	case SpudMan::EContextTFTModify:
		aFsm.ChangeStateToChangingTFT();
		EtelDriverInput(aFsm, EtelDriver::EChangeTft);
		break;
	case SpudMan::ESuspend: // think this is superfluous - DAN will see at review :)
		aFsm.ChangeStateToSuspended();
		SpudManNotify(aFsm, KContextBlockedEvent, KErrNone);
		break;
	case SpudMan::EMbmsSessionUpdate:
		EtelDriverInput(aFsm, EtelDriver::ESessionUpdate);
		break;
		
	case SpudMan::EMbmsParameterUpdate:
		SpudManNotify (aFsm, KContextParametersChangeEvent, aErrorCode);
		break;
		
	default: // default error handling
	    result = TPdpState::Input(aFsm, aOperation, aErrorCode);
	    break;
	}
	
	return result;
}


