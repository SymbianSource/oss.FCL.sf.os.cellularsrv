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
// PDP config change notifier
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef CPDPCONFIGCHAGENOTIFIER_H
#define CPDPCONFIGCHAGENOTIFIER_H

#include <e32base.h>
#include <etelpckt.h>
#include <networking/umtsnifcontrolif.h>

#include "eteldrivernmspace.h"
#include "ceteldrivernotifier.h"

/** notifier for packet context config */
class CPdpConfigChangeNotifier : public CEtelDriverNotifier
	{
public:
	CPdpConfigChangeNotifier(TContextId aId, RPacketContext& aPacketContext, 
								CPdpFsmInterface& aPdpFsmInterface);
	virtual ~CPdpConfigChangeNotifier();
	
	virtual void Start();

protected:
	virtual void Notify(const TRequestStatus& aStatus);
	virtual void DoCancel();

private:
	/** context id */
	TContextId						iId;
	/** underlying packet context */
	RPacketContext&					iPacketContext;

	/** packet context config */
	RPacketContext::TContextConfigGPRS	iConfigGPRS;
	/** packaged packet context config */
	EtelDriver::TContextConfigGPRSPckg				iConfigGPRSPckg;
	};

#endif // CPDPCONFIGCHAGENOTIFIER_H
