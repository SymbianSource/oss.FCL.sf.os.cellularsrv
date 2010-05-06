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
// State after mbms has been created, but not activated
// 
//

/**
 @file 
 @internalComponent
*/
 
#include <networking/umtsnifcontrolif.h>
#include "tpdpstates.h"
#include "spudfsmdebuglogger.h"
#include "pdpfsmnmspace.h"
#include "pdpfsmnmspace.h"
#include "PDPFSM.h"
#include "cpdpfsm.h"




TInt TPdpStateCreatedMbms::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateCreatedMbms::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case SpudMan::EContextActivate: 
		aFsm.ChangeStateToActivatingMbms();
		EtelDriverInput(aFsm, EtelDriver::EActivateMbmsPdp);
		return KErrNone;		
	case SpudMan::EContextTFTModify:
		RPacketContext::TContextStatus 		contextStatus;
		aFsm.Get(contextStatus);
		if (contextStatus == RPacketContext::EStatusActive)
			{
			aFsm.ChangeStateToChangingTFT();
			EtelDriverInput(aFsm, EtelDriver::EChangeTft);
			}
		else
			{
			aFsm.ChangeStateToSettingTFT();
			EtelDriverInput(aFsm, EtelDriver::ESetTft);
			}
		return KErrNone;
	case SpudMan::EMbmsSessionUpdate:
	        aFsm.ChangeStateToOpen();
	        EtelDriverInput(aFsm, EtelDriver::ESessionUpdate);
	        return KErrNone;
	
	}
	
	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


