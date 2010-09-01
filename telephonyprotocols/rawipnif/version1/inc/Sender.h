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
// Defines the active object that controls the BCA Write() requests. 
// 
//

/**
 @file
*/

#ifndef SENDER_H
#define SENDER_H

#include "BcaIoController.h"
#include "bttlog.h"
#include "Constants.h"

class CBttLogger;

class CSender : public CActive
	{
public:
	CSender(CBcaIoController& aObserver, CBttLogger* aTheLogger);
	~CSender();

public: // Inherited from CActive.
	virtual void RunL();
	virtual void DoCancel();

public:
	void Send(RMBufChain& aPdu);
	void SendBuffer(const TDesC8& aBuffer);
	inline TInt SendBufferLength();

private: // Unowned data.
	CBcaIoController& iObserver;
	CBttLogger* iTheLogger;	
	
private:
	//CLASSNAMEDECL(CSender);

	TBuf8<KMaxIPPacketAndHeader> iSendBuffer;
	};

TInt CSender::SendBufferLength()
	{
	return iSendBuffer.MaxLength();
	}

#endif // SENDER_H
