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
// This file defines the base class for the protocol interface classes.
// 
//

/**
 @file
*/

#ifndef PROTOCOLIFBASE_H__
#define PROTOCOLIFBASE_H__

#include <comms-infras/nifif.h>
#include <es_mbuf.h>

#include "Constants.h"

class CRawIPNifMain;
class CBttLogger;

class CProtocolIfBase : public CNifIfBase
	{
public:
	CProtocolIfBase(CRawIPNifMain& aNifMain, CBttLogger* aTheLogger);
	virtual ~CProtocolIfBase();

public:
	virtual void BindL(TAny *aId);
	virtual TInt State();
	virtual TInt Control(TUint aLevel,TUint aName,TDes8& aOption, TAny* aSource);
	virtual TInt Send(RMBufChain& aPdu, TAny* aSource);
	virtual void Info(TNifIfInfo& aInfo) const;
	virtual TInt Notification(TAgentToNifEventType aEvent, void * aInfo);
	virtual void StartSending(CProtocolBase* aProtocol);

public:
	virtual void UpdateContextConfigL(const RPacketContext::TContextConfigGPRS&
		aConfig);
	virtual void UpdateConnectionSpeed(TUint aConnectionSpeed);

	virtual TBool WantsProtocol(TUint16 aProtocolCode)=0;
	virtual void Process(RMBufChain& aPdu) = 0; 

public:
	inline CRawIPNifMain& GetNifMain();

protected:
    CProtocolBase* iProtocol;
	CRawIPNifMain& iNifMain;

private:
	CBttLogger* iTheLogger;
	};

CRawIPNifMain& CProtocolIfBase::GetNifMain()
/**
 * Returns to reference to RawIPNifMain
 *
 * @return reference to CRawIPNifMain class
 */
	{
	return iNifMain;
	}

#endif
