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
	static CReceiver* NewL(CBcaIoController& aObserver, CBttLogger* aTheLogger, TUint aMaxPacketSize);
	~CReceiver();

	// Inherited from CActive.
	virtual void RunL();
	virtual void DoCancel();
	void StartListening();
	
private:
	CReceiver(CBcaIoController& aObserver, CBttLogger* aTheLogger, TUint aMaxPacketSize);
	void ConstructL();

private: // Unowned data.
	CBcaIoController& iObserver;
	CBttLogger* iTheLogger;
	TUint iMaxPacketSize;
	RBuf8 iData;
	RMBufPacket iRMBufPacket;
	};

#endif // RECEIVER_H
