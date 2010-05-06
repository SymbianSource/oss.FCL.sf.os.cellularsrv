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
// Implements CBttLogger.
// 
//

/**
 @file
*/

#include "bttlog.h"

#ifdef __BTT_LOGGING__

#ifdef __DEBUGRAWIPSERIALPORT__
#include <e32svr.h>
#endif

//Enable this if you want time stamps for each log statements.
//This is computation ineffective and not recommended.
//#define __TIME_STAMP_ENABLED__

CBttLogger* CBttLogger::NewL(const TDesC8& aTag, const TDesC8& aFileName, const TUint32 aInstanceId)
/**
 * Factory method for CBttLogger.
 * 
 * @param aSubDir The subdirectory of c:\logs\gprsbtt to log to.
 * @param aFileName The filename to log to.
 * @return Ownership of a new CBttLogger.
 */
	{
	CBttLogger* self = new(ELeave) CBttLogger;
	CleanupStack::PushL(self);
	self->ConstructL(aTag, aFileName, aInstanceId);
	CleanupStack::Pop(self);
	return self;
	}

#ifdef __FLOG_ACTIVE
void CBttLogger::ConstructL(const TDesC8& aTag, const TDesC8& aFileName, const TUint32 aInstanceId)
#else
void CBttLogger::ConstructL(const TDesC8&, const TDesC8&, const TUint32 aInstanceId)
#endif
/**
 * 2nd-phase construction.
 * Hurricane/Typhoon - we can log to a particular directory
 */
	{
	iInstanceId = aInstanceId;
	__FLOG_OPEN(aTag, aFileName);
	}

//#endif

CBttLogger::CBttLogger()
/**
 * Constructor
 */
 	{
	}

CBttLogger::~CBttLogger()
/**
 * Destructor
 */
	{
	__FLOG_CLOSE;
	}

void CBttLogger::Write(const TDesC8& aText)
/**
 * Write an 8-bit descriptor to the log file
 *
 * @param aText The test to write
 */	
	{
	TBuf8<KLogBufferSize> buf;
	// put the instance id to identify which nif we are
	// logging from
	_LIT8(KInstanceFormat, "RawIp 0x%08X: ");
	buf.Format(KInstanceFormat, iInstanceId);
	
#ifdef __TIME_STAMP_ENABLED__
	// log to our own file in Hurricane/Typhoon
	_LIT8(KTimeFormat, "%02d.%02d:%02d:%06d ");
	
	TTime now;
	now.UniversalTime();
	TDateTime dateTime;
	dateTime = now.DateTime();
	buf.AppendFormat(KTimeFormat, 
		dateTime.Hour(), 
		dateTime.Minute(), 
		dateTime.Second(), 
		dateTime.MicroSecond());
#endif

	buf.Append(aText);

#ifdef __DEBUGRAWIPSERIALPORT__
	TBuf<KLogBufferSize> buf2;
	// TEMP: Do not print single % since appear as format char to RDebug::Print
	for (TInt i = 0; i < buf.Length(); i++)
		{
		if (buf[i] == '%')
			buf2.Append(_L("."));
		else
			buf2.Append(buf[i]);
		}
	RDebug::Print(buf2);
#else
	__FLOG(buf);
#endif
	}

void CBttLogger::WriteFormat(TRefByValue<const TDesC8> aFmt, ...)
/**
 * Write an 8-bit format list to the log file
 */	
	{
    VA_LIST list;
    VA_START(list, aFmt);

	// log to our own file in Hurricane/Typhoon
	TBuf8<KLogBufferSize> buf;
	buf.AppendFormatList(aFmt, list);
	Write(buf);
	}

void CBttLogger::VerboseLeaveL(char* aFile, TInt aLine, TInt aReason)
/**
 * Make a verbose leave - write name of file and line number to the log just 
 * before leaving.
 *
 * @param aFile The file we're leaving from.
 * @param aLine The line number we're leaving from.
 * @param aReason The leave code.
 */
	{
	// only leave if non-zero value
	if ( aReason == KErrNone )
		{
		return;
		}

	_LIT8(KLeavePrefix, "LEAVE: " );

	TPtrC8 fullFileName((const TUint8*)aFile);
	TPtrC8 fileName(fullFileName.Ptr()+fullFileName.LocateReverse('\\')+1);

	TBuf8<256> buf;
	buf.Append(KLeavePrefix);
	buf.AppendFormat(_L8(" aReason=%d [file %S, line %d]"), aReason, &fileName, 
		aLine);
	Write(buf);

	__FLOG_CLOSE;

	// finally
	User::Leave(aReason);
	}

void CBttLogger::VerbosePanic(char* aFile, 
							   TInt aLine, 
							   TInt aPanicCode, 
							   TText8* aPanicName,
							   const TDesC& aPanicCategory)
/**
 * Make a verbose panic - write name of file and line number to the
 * log just before panicking.
 *
 * @param aFile The file that's panicking.
 * @param aLine The line number that's panicking.
 * @param aReason The panic code.
 * @param aPanicName The text of the panic code.
 * @param aPanicCategory The panic category.
 */
	{
	_LIT8(KLeavePrefix, "PANIC: " );

	TPtrC8 fullFileName((const TUint8*)aFile);
	TPtrC8 fileName(fullFileName.Ptr()+fullFileName.LocateReverse('\\')+1);

	TBuf8<256> buf;
	buf.Append(KLeavePrefix);
	buf.AppendFormat(_L8(" %d = %s [file %S, line %d]"), 
		aPanicCode, 
		aPanicName, 
		&fileName, 
		aLine);
	Write(buf);

	__FLOG_CLOSE;

	// finally
	User::Panic(aPanicCategory, aPanicCode);
	}

#endif // __BTT_LOGGING__
