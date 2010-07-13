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
//

#ifndef __WAPLOG_H__
#define __WAPLOG_H__

#ifdef _DEBUG

	#include <comms-infras/commsdebugutility.h>
	#define LOG(MSG) MSG

	#if defined (DETAILED_LOG)
		#define LOG_DETAILED(MSG) MSG
	#else
		#define LOG_DETAILED(MSG)
	#endif

	_LIT(KSWSLogFolder,"sws");
	_LIT(KSWSLogFile,"sws.txt");
	_LIT(KEndOfLine,"\r\n");

	class SwsLog
		{
	public:
		static void Write(const TDesC& aDes);
	    static void Printf(TRefByValue<const TDesC> aFmt, ...);
		};
#else

	#define LOG(MSG)
	#define LOG_DETAILED(MSG)

#endif

#endif
