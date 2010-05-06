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
// Implements the factory class which is used to instantiate the RAW IP NIF.
// 
//

/**
 @file
*/

#include <f32file.h>
#include "RawIPNifMainFactory.h"
#include "RawIPNifMain.h"
#include "bttlog.h"

void CRawIPNifMainFactory::InstallL()
/**
 * This function is pure virtual in CNifFactory, so we have to define it
 * here. It is called by NifMan before NewInterfaceL().
 * However, it doesn't do anything.
 */
	{
	}

CNifIfBase* CRawIPNifMainFactory::NewInterfaceL(const TDesC& aName) 
/**
 * Factory function which creates an instance of the RAW IP NIF.
 *
 * @param aName The name of the NIF
 * @return A newly instantiated RAW IP NIF
 */
	{
	return NewInterfaceL(aName, NULL);
	}

CNifIfBase* CRawIPNifMainFactory::NewInterfaceL(const TDesC& aName,MNifIfNotify* aNotify)
/**
 * Factory function which creates an instance of the RAW IP NIF.
 *
 * @param aName The name of the NIF
 * @param aNotify supplies details from "IfParams" field in CommDb - not used in btt
 * @return A newly instantiated RAW IP NIF
 */
	{
#ifdef __BTT_LOGGING__
	iTheLogger = CBttLogger::NewL(KNifSubDir, KRefFile, User::FastCounter());
#endif // __BTT_LOGGING__

	_LOG_L1C1(_L8("Raw IP.NIF logging started."));
	_LOG_L1C2(_L8("         aNotify = %x"), aNotify);

 	CRawIPNifMain* s = new (ELeave) CRawIPNifMain(*this, aNotify, iTheLogger);
	CleanupStack::PushL(s);
	s->ConstructL(aName);
	CleanupStack::Pop(s);

	return s;
	}

TInt CRawIPNifMainFactory::Info(TNifIfInfo& /*aInfo*/, TInt /*aIndex*/) const
/**
 * This function is pure virtual in CNifFactory, so we have to define it
 * here. However, it doesn't do anything, and is never called by NifMan.
 *
 * @param aInfo Ignored
 * @param aIndex Ignored
 * @return Always KErrNone
 */
	{
	return KErrNone;
	}
