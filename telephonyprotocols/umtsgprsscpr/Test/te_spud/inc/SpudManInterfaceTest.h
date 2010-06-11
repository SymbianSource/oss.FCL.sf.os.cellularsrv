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
// Declaration of the SpudManInterface for SPUD FSM unit tests
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef SPUDMANINTERFACETEST_H
#define SPUDMANINTERFACETEST_H

#include <networking/mspudmaninterface.h>

// forward declarations
class CTestRequestListener;

/** Implementation of MSpudManInterface used to test the FSM */
class TSpudManInterfaceTest : public MSpudManInterface
	{
	public:
	TSpudManInterfaceTest(CInputRequestListener& aListener);
	
	void Input(TContextId aPdpId, TInt aEvent, TInt aParam);

	void SetContextTerminationErrorAndStop(TContextId aContextId, TInt aErrorCode);

private:
	/** Used as the interface between the thread running the active objects and the test execute thread.
		Contains the data passed to/from Input requests */
	CInputRequestListener& iListener;
	};
	
#endif
// SPUDMANINTERFACETEST_H

