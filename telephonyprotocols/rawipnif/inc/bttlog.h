// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Defines logger for RawIP NIF.
// 
//

/**
 @file
*/

#ifndef BTTLOG_H__
#define BTTLOG_H__

#include <comms-infras/commsdebugutility.h>

#if defined(__FLOG_ACTIVE) || defined(__DEBUGRAWIPSERIALPORT__)
// Uncomment any of these macros if you want logging.
// -> BTT_LOG_1 : For general logging
// -> BTT_LOG_2 : For error and warning logging
// -> BTT_LOG_3 : For IP packet information
#define	__BTT_LOG_1__ 1
//#define __BTT_LOG_2__ 1
//#define __BTT_LOG_3__ 1
#endif

#ifdef __BTT_LOG_1__
#define _LOG_L1C1(AAA)						iTheLogger->Write(AAA)
#define _LOG_L1C2(AAA, BBB)					iTheLogger->WriteFormat(AAA, BBB)
#define _LOG_L1C3(AAA, BBB, CCC)			iTheLogger->WriteFormat(AAA, BBB, CCC)
#define _LOG_L1C4(AAA, BBB, CCC, DDD)		iTheLogger->WriteFormat(AAA, BBB, CCC, DDD)
#define _LOG_L1C5(AAA, BBB, CCC, DDD, EEE)	iTheLogger->WriteFormat(AAA, BBB, CCC, DDD, EEE)
#else
#define _LOG_L1C1(AAA)
#define _LOG_L1C2(AAA, BBB)
#define _LOG_L1C3(AAA, BBB, CCC)
#define _LOG_L1C4(AAA, BBB, CCC, DDD)
#define _LOG_L1C5(AAA, BBB, CCC, DDD, EEE)  
#endif // __BTT_LOG_1__

#ifdef __BTT_LOG_2__
#define _LOG_L2C1(AAA)						iTheLogger->Write(AAA)
#define _LOG_L2C2(AAA, BBB)					iTheLogger->WriteFormat(AAA, BBB)
#define _LOG_L2C3(AAA, BBB, CCC)			iTheLogger->WriteFormat(AAA, BBB, CCC)
#define _LOG_L2C4(AAA, BBB, CCC, DDD)		iTheLogger->WriteFormat(AAA, BBB, CCC, DDD)
#define _LOG_L2C5(AAA, BBB, CCC, DDD, EEE)	iTheLogger->WriteFormat(AAA, BBB, CCC, DDD, EEE)
#else
#define _LOG_L2C1(AAA)
#define _LOG_L2C2(AAA, BBB)
#define _LOG_L2C3(AAA, BBB, CCC)
#define _LOG_L2C4(AAA, BBB, CCC, DDD)
#define _LOG_L2C5(AAA, BBB, CCC, DDD, EEE)  
#endif // __BTT_LOG_2__

#ifdef __BTT_LOG_3__
#define _LOG_L3C1(AAA)						iTheLogger->Write(AAA)
#define _LOG_L3C2(AAA, BBB)					iTheLogger->WriteFormat(AAA, BBB)
#define _LOG_L3C3(AAA, BBB, CCC)			iTheLogger->WriteFormat(AAA, BBB, CCC)
#define _LOG_L3C4(AAA, BBB, CCC, DDD)		iTheLogger->WriteFormat(AAA, BBB, CCC, DDD)
#define _LOG_L3C5(AAA, BBB, CCC, DDD, EEE)	iTheLogger->WriteFormat(AAA, BBB, CCC, DDD, EEE)
#else
#define _LOG_L3C1(AAA)
#define _LOG_L3C2(AAA, BBB)
#define _LOG_L3C3(AAA, BBB, CCC)
#define _LOG_L3C4(AAA, BBB, CCC, DDD)
#define _LOG_L3C5(AAA, BBB, CCC, DDD, EEE)  
#endif // __BTT_LOG_3__

#ifdef __BTT_LOG_1__
#define __BTT_LOGGING__
#endif

#ifdef __BTT_LOG_2__
#define __BTT_LOGGING__
#endif

#ifdef __BTT_LOG_3__
#define __BTT_LOGGING__
#endif

#ifdef __BTT_LOGGING__
#define _BTT_LEAVEIFERROR(a)	iTheLogger->VerboseLeaveL(__FILE__, __LINE__, a)
#define _BTT_PANIC(CAT,	CODE)	iTheLogger->VerbosePanic(__FILE__, __LINE__, CODE, (TText8*)#CODE, CAT)
#else
#define _BTT_LEAVEIFERROR(a)	User::LeaveIfError(a)
#define _BTT_PANIC(CAT, CODE)	User::Panic(CAT, CODE)
#endif // __BTT_LOGGING__

/**
 * static class containing methods for logging
 * text to a logfile.
 */
class CBttLogger : public CBase
	{
public:
	static CBttLogger* NewL(const TDesC8& aTag, const TDesC8& aFileName, const TUint32 aInstanceId);
	~CBttLogger();

public:
	void Write(const TDesC8& aText);
	void WriteFormat(TRefByValue<const TDesC8> aFmt, ...);
	void VerboseLeaveL(char* aFile, TInt aLine, TInt aReason);
	void VerbosePanic(char* aFile, 
		TInt aLine, 
		TInt aPanicCode, 
		TText8* aPanicName,
		const TDesC& aPanicCategory);

protected:
	CBttLogger();
	void ConstructL(const TDesC8& aSubDir, const TDesC8& aFileName, const TUint32 aInstanceId);

private:
    class TLogIgnoreOverflow8 : public TDes8Overflow
        {
        public:
            void Overflow(TDes8& /*aDes*/) { }
        };

private: // owned

	TUint32 iInstanceId;
	__FLOG_DECLARATION_MEMBER;
	};

#endif // BTTLOG_H__
