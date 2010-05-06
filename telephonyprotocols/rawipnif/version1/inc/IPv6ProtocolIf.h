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
// This file defines the CIPv6ProtocolIf class, which handles the transmission
// of IPv6 data to and from the TCP/IP stack.
// 
//

/**
 @file
*/

#ifndef IPV6PROTOCOLIF_H__
#define IPV6PROTOCOLIF_H__

#include "ProtocolIfBase.h"
#include <eui_addr.h>

class CBttLogger;

class CIPv6ProtocolIf : public CProtocolIfBase
{
public:
	CIPv6ProtocolIf(CRawIPNifMain& aNifMain, CBttLogger* aTheLogger);
	~CIPv6ProtocolIf();

public:	//NifIfBase support.
	virtual void BindL(TAny *aId);
	virtual TInt Control(TUint aLevel,TUint aName,TDes8& aOption, TAny* aSource);
	virtual TInt Send(RMBufChain& aPdu, TAny* aSource);
	virtual void Info(TNifIfInfo& aInfo) const;
	virtual TInt Notification(TAgentToNifEventType aEvent, void * aInfo);
	virtual void StartSending(CProtocolBase* aProtocol); // It seems not needed for ipv6

	virtual TBool WantsProtocol(TUint16 aProtocolCode);
	virtual void Process(RMBufChain& aPdu);


	virtual void UpdateContextConfigL(const RPacketContext::TContextConfigGPRS&
		aConfig);
	virtual void UpdateConnectionSpeed(TUint aConnectionSpeed);

	// Define the class that will hold the IPv6 setting required by the Nif.
	class TIPv6Settings
		{
	public:
		TIp6Addr iLocalAddr;
		TIp6Addr iPrimaryDns;
		TIp6Addr iSecondaryDns;
		TBool iGetDnsFromServer;
		TBool iGetIpFromServer;
		TE64Addr iLocalIfId;
		//TE64Addr iRemoteIfId;
		};

private:
	void ReadCommDbGprsSettingsL();
	// This method is only used to get GPRS_IP_ADDR.
	void ReadIPv4SettingL(const TDesC& aFieldName, TUint32& aIpAddr);	
	void ReadIPv6SettingL(const TDesC& aFieldName, TIp6Addr& aIpAddr);

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
	TIPv6Settings iSettings;
	TUint iSpeedMetric;
};

void CIPv6ProtocolIf::WriteIfName(TDes& aName) const
/**
 * Used to uniquely identify the current Nif instance
 *
 * @param aName Descriptor that will be initialised.
 */
	{
	aName.Format(_L("%S[0x%X]"), &KIPv6ProtocolIfName, this);
	}

#endif // IPV6PROTOCOLIF_H__
