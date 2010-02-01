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
// SPUD data multiplexer
// 
//

/**
 @file
 @internalComponent
*/

#ifndef MUX_H
#define MUX_H

#include "spudman.h"
#include "bindman.h"
#include <in_iface.h>
#include <es_prot.h>

/**
Data multiplexer class.
Dispatches packets from GUQoS to the appropriate PDP context.
*/
NONSHARABLE_CLASS(CSpudMux) : public CNifIfBase
	{
public:
	CSpudMux(CSpudMan& aNifBase);
	~CSpudMux();
	void Construct(CBindMan& aBindMan);
	
	// setters
	void SetProtocolBase(CProtocolBase* aProtocolBase);

	// CNifIfBase
	void Info(TNifIfInfo& aInfo) const;
	TInt Send(RMBufChain& aPacket, TAny*);
	TInt Notification(TAgentToNifEventType aEvent, void* /*aInfo*/);
	void BindL(TAny* aId);
	TInt State();
	TInt Control(TUint aLevel, TUint aName, TDes8& aOption, TAny* aSource);


private:
	TInterfaceName iIfName;
	CBindMan* iBindMan;
	CProtocolBase* iProtocolBase;
	};


/**
Data receiver class.
Receives data from the network and sends it to the network stack.
*/
NONSHARABLE_CLASS(CSpudProtocol) : public CProtocolBase
	{
public:
	CSpudProtocol();
	~CSpudProtocol();
	void Construct(CBindMan& aBindMan);

	void SetProtocolBaseL(CProtocolBase* aProtocolBase);
	void DoStartSending() const;

	// CProtocolBase
	void Identify(TServerProtocolDesc *aDesc) const;
	void StartSending(CProtocolBase* aProtocol);
	void Process(RMBufChain& aChain, CProtocolBase* aSourceProtocol);
	void Process(TDes8& aPDU, TSockAddr* aFrom, TSockAddr* aTo, CProtocolBase* aSourceProtocol);
	void Error(TInt aError, CProtocolBase* aSourceProtocol);

private:
	CBindMan* iBindMan;
	CProtocolBase* iProtocolBase;
	};

#endif
