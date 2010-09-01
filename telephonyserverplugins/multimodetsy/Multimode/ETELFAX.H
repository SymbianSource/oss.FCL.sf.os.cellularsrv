// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalAll 
*/

#ifndef __ETELFAX_H
#define __ETELFAX_H

#include "et_tsy.h"
#include "faxstd.h"

#include <faxserv.h>	// new!!

//
//	CFaxCompletion class is used by Fax Server to complete functions
//
class CCallMobileFax;
class CPhoneGlobals;
class CTelObject;
class CFaxCompletion : public MFaxCompletionBase, public CBase
	{
public:
	CFaxCompletion();
	~CFaxCompletion();

// Rx Fax
	virtual void RxConnectComplete(TInt aError);
	virtual void RxFaxDataComplete(TInt aError);
	virtual void RxPostPageComplete(TInt aError);

// Tx Fax
	virtual void TxConnectComplete(TInt aError);
	virtual void TxFaxDataComplete(TInt aError);
	virtual void TxPostPageComplete(TInt aError);

	virtual void GetCadenceAndTimeOfLastRing(TTimeIntervalMicroSeconds& aCadence,TTime& aTime);
public:
	void Configure(TTsyReqHandle aTsyReqHandle,CTelObject* aTelObject);
	CTelObject* TelObject() const;
private:
	TTsyReqHandle iTsyReqHandle;	//	}	Used for client-side synchronous requests
	CTelObject* iTelObject;			//	}
	CCallMobileFax* iCall;
	};

#endif
