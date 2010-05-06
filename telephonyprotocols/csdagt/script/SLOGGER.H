/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* Netdial Data and Event Logger header.
* 
*
*/



/**
 @file Slogger.h
 @internalComponent
*/

#ifndef __SLOGGER_H__
#define __SLOGGER_H__

#include <comms-infras/commsdebugutility.h>
#include <e32base.h>

/**
@internalComponent
*/
_LIT8(KNetDialLogFile,"NETDIAL.TXT");
_LIT8(KNetDialLogFolder,"NETDIAL");

_LIT(KIllegalTranstionPanicString,"NetDial (%d %d)");
_LIT(KCompletedPhaseLogString,"NetDial:\tCompleted Phase '%S' with Error %d");
_LIT(KPhaseTransitionLogString,"NetDial:\tPhase Transition from Phase %d to Phase %d");
_LIT(KIllegalPhaseTransitionLogString,"NetDial:\tIllegal Phase Transition from Phase %d to Phase %d");
_LIT(KStopLogString,"NetDial:\tStopped, Phase %d");
_LIT(KServiceStartedLogString,"NetDial:\tService Started");
_LIT(KServiceStartedReconnLogString,"NetDial:\tService Started On Reconnection");


#endif	// __SLOGGER_H__
