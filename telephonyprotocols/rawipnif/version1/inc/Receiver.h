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
// Defines the active object that controls the BCA Read() requests. 
// 
//

/**
 @file
*/

#ifndef RECEIVER_H
#define RECEIVER_H

#include "BcaIoController.h"
#include "bttlog.h"
#include "Constants.h"

class CBttLogger;

class CReceiver : public CActive
	{
public:
	CReceiver(CBcaIoController& aObserver, CBttLogger* aTheLogger);
	~CReceiver();

public: // Inherited from CActive.
	virtual void RunL();
	virtual void DoCancel();

public:
	void StartListening();

private: // Unowned data.
	CBcaIoController& iObserver;
	CBttLogger* iTheLogger;
	
private:
	TBuf8<KMaxIPPacketAndHeader> iData;
	};

#endif // RECEIVER_H
