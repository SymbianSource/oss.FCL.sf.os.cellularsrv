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
// WAPLOG.CPP
// 
//


#ifdef _DEBUG

#include "WapSwsLog.h"

void SwsLog::Write(const TDesC& aDes)
/*
 * Write aText to the log
 */
	{
	RFileLogger::Write(KSWSLogFolder(),KSWSLogFile(),EFileLoggingModeAppend,aDes);
	}

void SwsLog::Printf(TRefByValue<const TDesC> aFmt,...)
/*
 * Write a mulitple argument list to the log, trapping and ignoring any leave
 */
	{
	VA_LIST list;
	VA_START(list,aFmt);
	RFileLogger::WriteFormat(KSWSLogFolder(),KSWSLogFile(),EFileLoggingModeAppend,aFmt,list);
	}
	
#endif // _DEBUG

