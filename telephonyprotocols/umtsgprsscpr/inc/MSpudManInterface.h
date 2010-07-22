// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The call-back interface of MPdpFsmInterface used by UmtsGprsSCPR to flag
// operation results to SpudMan.
// 
//

/**
 @file
 @internalComponent
*/

#ifndef MSPUDMANINTERFACE_H
#define MSPUDMANINTERFACE_H

#include <networking/umtsnifcontrolif.h>
#include <in_iface.h>

/**
Identifier used in a Control call for initialising the RPdpFsmInterface (in SpudMan) with a
pointer to the real C-class (in the UmtsGprsSCPR).
*/
const TInt KInitialisePdpFsm = 0x80;

/**
Identifier used in a SpudInput call to signal the SpudMan that the UmtsGprsSCPR is shutting
down and that the real CPdpFsmInterface object will be deleted.
*/
const TUint KPdpFsmShuttingDown = 0xff; 

/** Name of SPUD */
_LIT(KSpudName, "spud");

/**
Interface class for SpudMan, to be used by SpudFsm.
*/
class MSpudManInterface
{
public:
	virtual void Input(TContextId aContextId, TInt aEvent, TInt aParam) = 0;

	virtual void SetContextTerminationErrorAndStop(TContextId aContextId, TInt aErrorCode) = 0;

};

#endif
// MSPUDMANINTERFACE_H

