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
//

#ifdef USING_CTSY_DISPATCHER

/**
 * This file contains additional function definitions for the CMmPhoneTsy class
 * in CTSY for use when the CTSY is used with the CTSY Dispatcher.
 */

#include "cmmphonetsy.h"
#include "cmmtsyreqhandlestore.h"

// ---------------------------------------------------------------------------
// CMmPhoneTsy::CompleteGetPhoneId
// Sets iPhoneIdentity fields and completes GetPhoneId.
// ---------------------------------------------------------------------------
//
void CMmPhoneTsy::CompleteGetPhoneId(
	const RMobilePhone::TMobilePhoneIdentityV1& aPhoneId,
    TInt aError )
    {
	if ( KErrNone == aError )
		{
		// Copy each field
		if(aPhoneId.iManufacturer.Length() > 0)
			{
			iPhoneIdentity.iManufacturer.Copy(aPhoneId.iManufacturer);
			}
		if(aPhoneId.iModel.Length() > 0)
			{
			iPhoneIdentity.iModel.Copy(aPhoneId.iModel);
			}
		if(aPhoneId.iRevision.Length() > 0)
			{
			iPhoneIdentity.iRevision.Copy(aPhoneId.iRevision);
			}
		if(aPhoneId.iSerialNumber.Length() > 0)
			{
			iPhoneIdentity.iSerialNumber.Copy(aPhoneId.iSerialNumber);
			}

TFLOGSTRING("TSY: CMmPhoneTsy::CompleteGetPhoneId :");
TFLOGSTRING2("				Manufacturer: %S,", &iPhoneIdentity.iManufacturer);
TFLOGSTRING2("				Model: %S,", &iPhoneIdentity.iModel);
TFLOGSTRING2("				Revision: %S,", &iPhoneIdentity.iRevision);
TFLOGSTRING2("				SerialNumber: %S", &iPhoneIdentity.iSerialNumber);
        }

    //reset req handle. Returns the deleted req handle
    iTsyReqHandleStore->ResetTsyReqHandle(EMultimodePhoneGetPhoneId );
   
    TInt getPhoneIdRequestsCount = iGetPhoneIdRequests.Count();

    for ( TInt i=0; i < getPhoneIdRequestsCount ; i++ )
        {
        TGetPhoneIdRequest* req = iGetPhoneIdRequests[i];
    
        *(req->iRetPhoneId) = iPhoneIdentity;
        ReqCompleted(req->iReqHandle, aError);   
        }
    iGetPhoneIdRequests.ResetAndDestroy();
    }

#endif //USING_CTSY_DISPATCHER
