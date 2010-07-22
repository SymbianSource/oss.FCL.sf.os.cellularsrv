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
// spud context element
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef CSPUDCONTEXTELEM_H
#define CSPUDCONTEXTELEM_H

#include "ceteldrivercontext.h"
#include "cpdpstatuschangenotifier.h"
#include "cqoschangenotifier.h"
#include "cpdpconfigchangenotifier.h"

class CEtelDriverFactory;

/** structure to keep context object and associated notifiers */
class CSpudContextElem : CBase
	{
public:
	static CSpudContextElem* NewL(TContextId aId, CEtelDriverFactory& aFactory, CPdpFsmInterface& aPdpFsmInterface);
	virtual ~CSpudContextElem();

	void Start();
	void Cancel();

	void GetLastErrorCause(TInt& aLastErrorCause);
	
    void PdpStatusChangeNotifierCancel();
    void QoSChangeNotifierCancel();
    void PdpConfigChangeNotifierCancel();

	/** etel driver context */
	CEtelDriverContext*			iContext;
	/** pdp status change notifier */
	CPdpStatusChangeNotifier*	iPdpStatusChangeNotifier;
	/** QoS profile change notifier */
	CQoSChangeNotifier*			iQoSChangeNotifier;
	/** pdp config  change notifier */
	CPdpConfigChangeNotifier*	iPdpConfigChangeNotifier;
	
private:
	void ConstructL(TContextId aId, CEtelDriverFactory& aFactory, CPdpFsmInterface& aPdpFsmInterface);
	};

#endif // CSPUDCONTEXTELEM_H
