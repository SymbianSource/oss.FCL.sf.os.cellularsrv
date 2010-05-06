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
//

#ifndef _CONFIG_
#define _CONFIG_

#include <e32std.h>
#include <f32file.h>

_LIT(KConfigFile, "C:\\dispatcher_ipc_config.txt");

class TConfig
	{
public:
	TConfig();

	TBool IsSupported(TInt aIpc);
	TInt SetSupportedValue(TInt aIpc, TBool aSupported);
	TInt Reset();
	/**
	 * Push self to CleanupStack so that Reset is always called
	 */
	void PushL();
	
private:	
	static void ResetThis(TAny* aThis);
	};

#endif
