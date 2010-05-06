// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Global functions - DLL entry point and general panic
// 
//

/**
 @file Nd_dll.cpp 
 @internalComponent
*/

#include <e32std.h>
#include <cdbcols.h>
#include "ND_STD.H"
#include "SLOGGER.H"
#include "ND_DIRCT.H"
#include "ND_DLIN.H"
#include "ND_DLUP.H"
#include "ND_DBACC.H"


/**
General panic function.

Global functions - DLL entry point and general panic

@param aPanic is the netdial panic which should be used in User::Panic().
*/
GLDEF_C void NetDialPanic(TNetDialPanic aPanic)
	{
	__FLOG_STMT(_LIT8(logString,"NetDial Panic %d");)
	__FLOG_STATIC1(KNetDialLogFolder(),KNetDialLogFile(),TRefByValue<const TDesC8>(logString()),aPanic);
	_LIT(KNetDialPanic,"NetDial");
	User::Panic(KNetDialPanic,aPanic);
	}

