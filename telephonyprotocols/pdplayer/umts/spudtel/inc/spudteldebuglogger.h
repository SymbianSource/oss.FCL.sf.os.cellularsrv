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
// Event Logger header
// 
//

/**
 @file
 @internalComponent
*/

#ifndef SPUDTELDEBUGLOGGER_H
#define SPUDTELDEBUGLOGGER_H


#include "comms-infras/commsdebugutility.h"

#ifdef __FLOG_ACTIVE

#include <e32base.h>
#include <e32def.h>


_LIT8(KSpudTelLogFile,"SPUDTEL.TXT");
_LIT8(KSpudTelLogFileVerbose,"SPUDTELVERB.TXT");
_LIT8(KSpudTelLogFolder,"SPUD");

// pure informational logging
#if defined(_INFO_LOG)
// Static info Output Logging Macros
#define SPUDTEL_INFO_LOG(text) 					__FLOG_STATIC0(KSpudTelLogFolder,KSpudTelLogFile,text)
#define SPUDTEL_INFO_LOG1(text,a)				__FLOG_STATIC1(KSpudTelLogFolder,KSpudTelLogFile,text, a)
#define SPUDTEL_INFO_LOG2(text,a, b)			__FLOG_STATIC2(KSpudTelLogFolder,KSpudTelLogFile,text,a,b)
#define SPUDTEL_INFO_LOG3(text,a, b, c)			__FLOG_STATIC3(KSpudTelLogFolder,KSpudTelLogFile,text,a,b,c)
#define SPUDTELVERBOSE_INFO_LOG(text) 			__FLOG_STATIC0(KSpudTelLogFolder,KSpudTelLogFileVerbose,text)
#define SPUDTELVERBOSE_INFO_LOG1(text,a)		__FLOG_STATIC1(KSpudTelLogFolder,KSpudTelLogFileVerbose,text, a)
#define SPUDTELVERBOSE_INFO_LOG2(text,a, b)		__FLOG_STATIC2(KSpudTelLogFolder,KSpudTelLogFileVerbose,text,a,b)
#define SPUDTELVERBOSE_INFO_LOG3(text,a, b, c)	__FLOG_STATIC3(KSpudTelLogFolder,KSpudTelLogFileVerbose,text,a,b,c)


_LIT(KSpudTelMsgIn, ">>%S");
_LIT(KSpudTelMsgOut,"<<%S");

class TSpudTelLogFn
{
protected:
	TName iName;

public:
	TSpudTelLogFn(const TDesC& aName) 	{ iName = aName; SPUDTELVERBOSE_INFO_LOG1(KSpudTelMsgIn, &iName);};
	~TSpudTelLogFn()					{ SPUDTELVERBOSE_INFO_LOG1(KSpudTelMsgOut, &iName);};
};

#define SPUDTEL_FNLOG(name) TSpudTelLogFn spudTelLogFn(_L(name))

#else // defined(_INFO_LOG)

#define SPUDTEL_INFO_LOG(text)
#define SPUDTEL_INFO_LOG1(text,a)
#define SPUDTEL_INFO_LOG2(text,a, b)
#define SPUDTEL_INFO_LOG3(text,a, b, c)
#define SPUDTEL_FNLOG(name) 
#define SPUDTELVERBOSE_INFO_LOG(text)
#define SPUDTELVERBOSE_INFO_LOG1(text,a)
#define SPUDTELVERBOSE_INFO_LOG2(text,a, b)
#define SPUDTELVERBOSE_INFO_LOG3(text,a, b, c)

#endif // defined(_INFO_LOG)


// error logging
#if defined(_ERROR_LOG)

#define SPUDTEL_ERROR_LOG(errtext, err)	__FLOG_STATIC1(KSpudTelLogFolder,KSpudTelLogFile, errtext, err)
#define SPUDTEL_ERROR_LOG0(errtext)		__FLOG_STATIC0(KSpudTelLogFolder,KSpudTelLogFile, errtext)

#else // defined(_ERROR_LOG)

#define SPUDTEL_ERROR_LOG(errtext, err)
#define SPUDTEL_ERROR_LOG0(errtext)

#endif // defined(_ERROR_LOG)


#else	// __FLOG_ACTIVE

#define SPUDTEL_INFO_LOG(t)
#define SPUDTEL_INFO_LOG1(t,a)
#define SPUDTEL_INFO_LOG2(t,a,b)
#define SPUDTEL_INFO_LOG3(t,a,b,c)
#define SPUDTEL_FNLOG(name)
#define SPUDTELVERBOSE_INFO_LOG(t)
#define SPUDTELVERBOSE_INFO_LOG1(t,a)
#define SPUDTELVERBOSE_INFO_LOG2(t,a,b)
#define SPUDTELVERBOSE_INFO_LOG3(t,a,b,c)
#define SPUDTEL_ERROR_LOG(errtext, err)
#define SPUDTEL_ERROR_LOG0(errtext)


#endif //__FLOG_ACTIVE
#endif
// SPUDTELDEBUGLOGGER_H

