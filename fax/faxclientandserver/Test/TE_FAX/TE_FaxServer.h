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
// Telephony Fax Test server header file.
// 
//

/**
 @file
 @internalComponent
*/

#ifndef __TE_FAXSERVER_H__
#define __TE_FAXSERVER_H__

#include <test/testexecuteserverbase.h>
#include "TE_FaxBase.h"


class CFaxTestServer : public CTestServer
	{
public:
	static CFaxTestServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);

private:
	};

#endif // __TE_FAXSERVER_H__
