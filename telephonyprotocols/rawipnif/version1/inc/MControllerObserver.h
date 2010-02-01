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
// Interface class for objects which need to observe the BCA controller.
// 
//

/**
 @file
*/

#ifndef MCONTROLLEROBSERVER_H__
#define MCONTROLLEROBSERVER_H__

#include "MRawIPNifObserverBase.h"
#include <es_mbuf.h>

class MControllerObserver : public MRawIPNifObserverBase
	{
public:
	virtual void Process(RMBufChain& aPdu, TUint16 aProtocolCode)=0;
	virtual void ResumeSending()=0;
	};

#endif // MCONTROLLEROBSERVER_H__
