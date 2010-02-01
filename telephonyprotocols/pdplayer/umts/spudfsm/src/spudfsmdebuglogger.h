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
// SpudFsm Data and Event Logger header
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef SPUDFSMDEBUGLOGGER_H
#define SPUDFSMDEBUGLOGGER_H

#include "comms-infras/commsdebugutility.h"

#ifdef __FLOG_ACTIVE

#include <e32base.h>

_LIT8(KSpudFsmLogFileVerbose,"SPUDFSMVERB.TXT");
_LIT8(KSpudFsmLogFile,"SPUDFSM.TXT");
_LIT8(KSpudFsmLogFolder,"SPUD");

// Static Output Logging Macros
#define SPUDFSMVERBOSE_LOG(text) 			__FLOG_STATIC0(KSpudFsmLogFolder,KSpudFsmLogFileVerbose,text)
#define SPUDFSMVERBOSE_LOG1(text,a)		    __FLOG_STATIC1(KSpudFsmLogFolder,KSpudFsmLogFileVerbose,text, a)
#define SPUDFSMVERBOSE_LOG2(text,a, b)		__FLOG_STATIC2(KSpudFsmLogFolder,KSpudFsmLogFileVerbose,text,a,b)
#define SPUDFSMVERBOSE_LOG3(text,a, b, c)	__FLOG_STATIC3(KSpudFsmLogFolder,KSpudFsmLogFileVerbose,text,a,b,c)
#define SPUDFSM_LOG(text) 					__FLOG_STATIC0(KSpudFsmLogFolder,KSpudFsmLogFile,text)
#define SPUDFSM_LOG1(text,a)		    	__FLOG_STATIC1(KSpudFsmLogFolder,KSpudFsmLogFile,text, a)
#define SPUDFSM_LOG2(text,a, b)				__FLOG_STATIC2(KSpudFsmLogFolder,KSpudFsmLogFile,text,a,b)
#define SPUDFSM_LOG3(text,a, b, c)			__FLOG_STATIC3(KSpudFsmLogFolder,KSpudFsmLogFile,text,a,b,c)


_LIT(KSpudFsmMsgIn, ">>%S");
_LIT(KSpudFsmMsgOut,"<<%S");

class TSpudFsmLogVerboseFn
{
	protected:
	TBuf<100> iName;

	public:
	TSpudFsmLogVerboseFn(const TDesC& aName) 	{ iName = aName; SPUDFSMVERBOSE_LOG1(KSpudFsmMsgIn, &iName);};
	~TSpudFsmLogVerboseFn()						{ SPUDFSMVERBOSE_LOG1(KSpudFsmMsgOut, &iName);};
};

#define SPUDFSMVERBOSE_FNLOG(name) TSpudFsmLogVerboseFn spudFsmVerboseFnLog(_L(name))

// error logging
#if defined(_ERROR_LOG)
#define SPUDFSM_ERROR_LOG(errtext, err)	{ __FLOG_STATIC0(KSpudFsmLogFolder,KSpudFsmLogFile, _L("ERROR: ")); \
										__FLOG_STATIC1(KSpudFsmLogFolder,KSpudFsmLogFile, errtext, err); }
#else // defined(_ERROR_LOG)
#define SPUDFSM_ERROR_LOG(errtext, err)
#endif // defined(_ERROR_LOG)

#else // __FLOG_ACTIVE

#define SPUDFSM_LOG(t)
#define SPUDFSM_LOG1(t,a)
#define SPUDFSM_LOG2(t,a,b)
#define SPUDFSM_LOG3(t,a,b,c)
#define SPUDFSMVERBOSE_LOG(t)
#define SPUDFSMVERBOSE_LOG1(t,a)
#define SPUDFSMVERBOSE_LOG2(t,a,b)
#define SPUDFSMVERBOSE_LOG3(t,a,b,c)
#define SPUDFSMVERBOSE_FNLOG(name)
#define SPUDFSM_ERROR_LOG(errtext, err)

#endif // __FLOG_ACTIVE


#endif
// SPUDFSMDEBUGLOGGER_H
