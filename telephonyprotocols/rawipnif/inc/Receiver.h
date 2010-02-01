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
#include <nifmbuf.h>

class CBttLogger;

class CReceiver : public CActive
	{
public:
	CReceiver(CBcaIoController& aObserver, CBttLogger* aTheLogger, TInt aMaxPacketSise);	
	static CReceiver* NewL(CBcaIoController& aObserver, CBttLogger* aTheLogger, TInt aMaxPacketSise);
	void ConstructL();
	~CReceiver();

public: // Inherited from CActive.
	virtual void RunL();
	virtual void DoCancel();

public:
	void StartListening();

private: // Unowned data.
	CBcaIoController& iObserver;
	CBttLogger* iTheLogger;
	TInt iMaxPacketSise;
	
private:
	
	RBuf8 iData;
	RMBufPacket iRMBufPacket;
	};

#endif // RECEIVER_H
