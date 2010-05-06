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
// @file qosextn_log.h
// Header file for qos extn API log
//



/**
 @internalComponent
*/
#if !defined(__LOG_H__)
#define __LOG_H__

#if !defined(_DEBUG)
#  define LOG(a)
#else
#  define LOG(a) a 
#  define _LOG

#include <flogger.h>

_LIT(KQoSExtnLogFolder,"qosextnapi");
_LIT(KQoSExtnLogFile,"qosextnapi.txt");  

class Log
	{
public:
	static inline void Write(const TDesC& aDes);
	static inline void Printf(TRefByValue<const TDesC> aFmt, ...);
	static inline void Printf(TRefByValue<const TDesC8> aFmt, ...);
	static inline void HexDump(const TText* aHeader, const TText* aMargin, const TUint8* aPtr, TInt aLen);
	};

inline void Log::Write(const TDesC& aDes)
{
	RFileLogger::Write(KQoSExtnLogFolder(), KQoSExtnLogFile(), EFileLoggingModeAppend, aDes);
}
	

inline void Log::Printf(TRefByValue<const TDesC> aFmt, ...)
{
	VA_LIST list;
	VA_START(list,aFmt);
	RFileLogger::WriteFormat(KQoSExtnLogFolder(), KQoSExtnLogFile(), EFileLoggingModeAppend, aFmt, list);
}

inline void Log::Printf(TRefByValue<const TDesC8> aFmt, ...)
{
	VA_LIST list;
	VA_START(list,aFmt);
	RFileLogger::WriteFormat(KQoSExtnLogFolder(), KQoSExtnLogFile(), EFileLoggingModeAppend, aFmt, list);
}

inline void Log::HexDump(const TText* aHeader, const TText* aMargin, const TUint8* aPtr, TInt aLen)
{
	RFileLogger::HexDump(KQoSExtnLogFolder(), KQoSExtnLogFile(), EFileLoggingModeAppend, aHeader, aMargin, aPtr, aLen);
}

#endif // !defined(_DEBUG)

#endif //__LOG_H__

