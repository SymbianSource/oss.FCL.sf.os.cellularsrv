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
// This file defines the CIPv4ProtocolIf class, which handles the transmission
// of IPv4 data to and from the TCP/IP stack.
// 
//

/**
 @file
*/

#ifndef IPV4PROTOCOLIF_H__
#define IPV4PROTOCOLIF_H__

#include "ProtocolIfBase.h"

class CBttLogger;

class CIPv4ProtocolIf : public CProtocolIfBase
{
public:
	CIPv4ProtocolIf(CRawIPNifMain& aNifMain, CBttLogger* aTheLogger);
	~CIPv4ProtocolIf();

public:	//NifIfBase support.
	virtual void BindL(TAny *aId);
	virtual TInt Control(TUint aLevel,TUint aName,TDes8& aOption, TAny* aSource);
	virtual TInt Send(RMBufChain& aPdu, TAny* aSource);
	virtual void Info(TNifIfInfo& aInfo) const;
	virtual TInt Notification(TAgentToNifEventType aEvent, void * aInfo);
	virtual void StartSending(CProtocolBase* aProtocol);

	virtual TBool WantsProtocol(TUint16 aProtocolCode);
	virtual void Process(RMBufChain& aPdu);

	virtual void UpdateContextConfigL(const RPacketContext::TContextConfigGPRS&
		aConfig);
	virtual void UpdateConnectionSpeed(TUint aConnectionSpeed);

	// Define the class that will hold the IPv4 setting required by the Nif.
	class TIPv4Settings
		{
	public:
		// As this is IPv4, all addresses are 32-bit.
		TUint32	iLocalAddr;
		TUint32	iNetMask;
		TUint32	iBroadcastAddr;
		TUint32	iDefGateway;
		TUint32	iPrimaryDns;
		TUint32	iSecondaryDns;
		TBool iGetGatewayFromServer;
		TBool iGetDnsFromServer;
		};

private:
	void ReadCommDbGprsSettingsL();
	void ReadIPv4SettingL(const TDesC& aFieldName, TUint32& aIpAddr);

	TInt WriteIfConfig(TDes8& aIfConfig);
	TInt WriteIfInfo(TDes8& aIfInfo);

	TInt WriteConnectionInfo(TDes8& aConnectionInfo);

#ifdef WCDMA_STUB
	TInt DeleteContext(TDes8& aContextParameters);
#endif

	inline void WriteIfName(TDes& aName) const;

#ifdef __BTT_LOGGING__
	void LogPacket(const RMBufChain& aPacket);
#endif // __BTT_LOGGING__

private: //unowned
	CBttLogger* iTheLogger;

private: //owned
	TIPv4Settings iSettings;
	TUint iSpeedMetric;
};

void CIPv4ProtocolIf::WriteIfName(TDes& aName) const
/**
 * Used to uniquely identify the current Nif instance
 *
 * @param aName Descriptor that will be initialised.
 */
	{
	aName.Format(_L("%S[0x%X]"), &KIPv4ProtocolIfName, this);
	}

#endif // IPV4PROTOCOLIF_H__
