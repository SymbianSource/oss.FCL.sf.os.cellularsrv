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
// Setting TFT state
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
#include "cpdpfsm.h"

TInt TPdpStateSettingTFT::Input (CPdpFsm& aFsm, const TInt aOperation, const TInt aErrorCode)
{
	SPUDFSMVERBOSE_FNLOG("TPdpStateSettingTFT::Input()");
	SPUDFSMVERBOSE_LOG2(_L("aOperation : %S(%d)"), LogOperation(aFsm, aOperation), aOperation);

	switch (aOperation)
	{
	case PdpFsm::ETftSet:
		if (aFsm.iContextType != SpudMan::EMbms)
			{
			aFsm.ChangeStateToCreatedSecondary();
			}
		else
			{
			RPacketContext::TContextStatus 		contextStatus;
			aFsm.Get(contextStatus);
			if (contextStatus == RPacketContext::EStatusActive)
				{
				aFsm.ChangeStateToOpen();
				}
			else
				{
				aFsm.ChangeStateToCreatedMbms();
				}
			}
		SpudManNotify(aFsm, KContextTFTModifiedEvent, KErrNone);
		return KErrNone;
	case PdpFsm::ETftSetFailed:
		if (aFsm.iContextType != SpudMan::EMbms)
			{
			aFsm.ChangeStateToCreatedSecondary();
			}
		else
			{
			aFsm.ChangeStateToCreatedMbms();
			}
		SpudManNotify(aFsm, KContextTFTModifiedEvent, aErrorCode);
		return KErrNone;
	}
	
	// default error handling
	return TPdpState::Input(aFsm, aOperation, aErrorCode);
}


