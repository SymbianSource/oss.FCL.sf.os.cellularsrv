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
// CommsDebugUtil logging support for the CTSY Dispatcher.
//




/**
 @file
 @internalAll 
*/

#ifndef __LTSYLOGGER_H_
#define __LTSYLOGGER_H_

#include <e32base.h>

#ifdef _DEBUG

#include <comms-infras/commsdebugutility.h>
_LIT8(KTsySubSystem, "tsy");
_LIT8(KTsyCompnt, "ctsydis");

	/** Regular logging macro */
	#define LOG(format, ARGS...)	\
		{ \
		RFileLogger::WriteFormat(KTsySubSystem, KTsyCompnt, format, ##ARGS); \
		}
	
	/** Macros to log function entry and exit */
	//#define TSYLOGENTRYEXIT(aFunc) 	TLogEntryExit __logger((aFunc), KTsyCompnt)
	
	//#define TSYLOGENTRYEXITARGS(aFunc, aFmt, ARGS...) 	TLogEntryExit __logger((aFunc), KTsyCompnt, (aFmt), ##ARGS); 

	#define TSYLOGENTRYEXIT 	/*lint -save -e40*/	TLogEntryExit __logger(TPtrC8((const TUint8*)__PRETTY_FUNCTION__), KTsyCompnt)/*lint -restore*/
	
	// N.B. The function name does not need to be specified
	#define TSYLOGENTRYEXITARGS(aFmt, ARGS...) 	/*lint -save -e40*/ TLogEntryExit __logger(TPtrC8((const TUint8*)__PRETTY_FUNCTION__), KTsyCompnt, (aFmt), ##ARGS)/*lint -restore*/

	#define TSYLOGSETEXITERR(aErr) 	__logger.SetExitErrorCode( aErr )


/**
Automatic class that logs function entry and exit
*/
class TLogEntryExit
	{
public:
	IMPORT_C TLogEntryExit(const TDesC8& aFnName, const TDesC8& aLayer);
	IMPORT_C TLogEntryExit(const TDesC8& aFnName, const TDesC8& aLayer, TRefByValue<const TDesC8> aFmt, ...);
	IMPORT_C TLogEntryExit(const TDesC8& aFnName, const TDesC8& aLayer, TRefByValue<const TDesC16> aFmt, ...);
	IMPORT_C ~TLogEntryExit();
	inline TInt SetExitErrorCode(TInt aErr);
private:
	TPtrC8 iFnName;
	TPtrC8 iLayer;
	TInt   iErr;
	}; // class TLogEntryExit

/** 
Set the error that will be logged at function exit

@param aErr error code to log
@return the given error code. This is to allow to pipe the function call
*/
TInt TLogEntryExit::SetExitErrorCode(TInt aErr)
	{
	return iErr = aErr;
	}
	
	
	
#else // _DEBUG

	#define LOG(format, ARGS...)
	#define TSYLOGENTRYEXIT
	#define TSYLOGENTRYEXITARGS(aFunc, aFmt, ARGS...)
	#define TSYLOGSETEXITERR(aErr)	aErr // So that we don't lose the return code in UREL!

	
/**
Stub class for urel builds, required for exports.
*/
class TLogEntryExit
	{
public:
	IMPORT_C TLogEntryExit(const TDesC8& aFnName, const TDesC8& aLayer);
	IMPORT_C TLogEntryExit(const TDesC8& aFnName, const TDesC8& aLayer, TRefByValue<const TDesC8> aFmt, ...);
	IMPORT_C TLogEntryExit(const TDesC8& aFnName, const TDesC8& aLayer, TRefByValue<const TDesC16> aFmt, ...);
	IMPORT_C ~TLogEntryExit();
	};



#endif // _DEBUG

#endif // __LTSYLOGGER_H_
