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
// This file implements the CIPv4ProtocolIf class, which handles the transmission
// of IPv4 data to and from the TCP/IP stack.
// 
//

/**
 @file
*/

#include <etelpckt.h>
#include <in_iface.h>
#include "RawIPNifMain.h"
#include "IPv4ProtocolIf.h"
#ifdef WCDMA_STUB
#include <UMTSNifControlIf.h>
#endif
#define LOG_IP_ADDRESS(desc,addr) _LOG_L2C5(_L8("    " desc " = %d.%d.%d.%d from context"), \
			addr >> 24, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF);

CIPv4ProtocolIf::CIPv4ProtocolIf(CRawIPNifMain& aNifMain, CBttLogger* aTheLogger)
/**
 * Constructor
 */ 
	: CProtocolIfBase(aNifMain,aTheLogger),
	  iTheLogger(aTheLogger), 
	  iSpeedMetric(KDefaultSpeedMetric)
	{
	}

CIPv4ProtocolIf::~CIPv4ProtocolIf()
/**
 *	Destructor
 */
	{
	}

void CIPv4ProtocolIf::BindL(TAny* aId)
/**
 *	Binds protocol to the Nif
 *
 *	@param aId A point the the protocol(TCP/IP)
 */
	{
	_LOG_L1C2(_L8("CIPv4ProtocolIf::BindL [aId=%X]"), aId);

	CProtocolIfBase::BindL(aId); // Call the superclass's method.
	iNotify = iNifMain.Notify();
	}

TInt CIPv4ProtocolIf::Control(TUint aLevel, TUint aName, TDes8& aOption,
							TAny* /*aSource*/)
/**
 * The main function called by the TCP/IP protocol to control the interface.
 * Can perform a variety of general IP tasks (such as getting IP config)
 * and "3G" specific tasks (such as deleting the context).
 *
 * @param aLevel The level of the interface to control - always KSOLInterface
 * @param aName The command to perform
 * @param aOption Data to be input/output as a result of the command
 * @param aSource The source of the command (unused)
 * @return Standard error codes
 */
	{
	_LOG_L1C3(_L8("CIPv4ProtocolIf::Control [aLevel=%d, aName=%d]"),
		aLevel, aName);

	if (aLevel == KSOLInterface)
		{
		switch (aName)
			{
		// General IP configuration commands are below this point.

		case KSoIfInfo:
			// Gets information about the interface, such as name and MTU.
			return WriteIfInfo(aOption);

		case KSoIfConfig:
			// Gets interface configuration, eg. IP address and DNS servers.
			return WriteIfConfig(aOption);

		case KSoIfGetConnectionInfo:
			// Gets IAP and network information.
			return WriteConnectionInfo(aOption);

		case KSoIfHardwareAddr:
			// unsupported because we don't have a h/w address
		case KSoIfCompareAddr:
			// this never actually appears to be called
			break;

		// 3G-specific configuration commands are below this point.
#ifdef WCDMA_STUB
		case KRegisterEventHandler:
			// Raw IP NIF Events are not supported
		case KContextSetEvents:
			// Raw IP NIF Events are not supported
			break;

		case KContextCreate:
			// We don't support creating new secondary contexts.
			break;

		case KContextDelete:
			// Deletes the primary PDP context. This will shut down the Nif.
			return DeleteContext(aOption);

		case KContextActivate:
			// If the IPv4 interface is up, then the context will already have
			// been activated. So this command should fail with 
			// KErrAlreadyExists
			{
			TUint8* ptr = CONST_CAST(TUint8*, aOption.Ptr()); 
			TContextParameters* contextParams =
				REINTERPRET_CAST(TContextParameters*, ptr);

			if (contextParams->iContextInfo.iContextId != 
				STATIC_CAST(TInt8, GetNifMain().GetBcaController()->Nsapi()))
				{
				contextParams->iReasonCode = KErrNotFound;
				}
			else
				{
				contextParams->iContextInfo.iStatus =
					GetNifMain().GetContextStatus();
				contextParams->iReasonCode = KErrAlreadyExists;
				}
			return KErrNone;
			}

		case KNifSetDefaultQoS:
		case KContextQoSSet:
			// Setting the QoS is meaningless over GPRS, so we just return that
			// we don't support these operations.
			break;

		case KContextTFTModify:
			// As we only have one primary context, we don't support anything
			// to do with traffic flow templates, which are used by secondary
			// contexts.
			break;

		case KContextModifyActive:
			// This command is only valid aftermodifying TFT/QoS parameters. 
			// As we don't support any of these operations,
			// this command is never valid.
			break;
#endif
		default:
			break;
			}
		}
	return KErrNotSupported;
	}

TInt CIPv4ProtocolIf::WriteIfConfig(TDes8& aIfConfig)
/**
 * Writes the configuration of the interface (eg. IP address, DNS servers)
 * into the supplied descriptor.
 *
 * @param aIfConfig Will contain the interface configuration
 * @return KErrNotSupported if an incorrect structure is passed, 
 * otherwise KErrNone
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::WriteIfConfig"));

	if (aIfConfig.Length() != sizeof(TSoInetIfConfig))
		{
		return KErrArgument;
		}

	TUint8* ptr = CONST_CAST(TUint8*, aIfConfig.Ptr());
	TSoInetIfConfig* config = REINTERPRET_CAST(TSoInetIfConfig*, ptr);

	if (config->iFamily != KAfInet)
		{
		return KErrNotSupported;
		}

	TInetAddr::Cast(config->iConfig.iAddress).SetAddress(iSettings.iLocalAddr);
	TInetAddr::Cast(config->iConfig.iNetMask).SetAddress(iSettings.iNetMask);
	TInetAddr::Cast(config->iConfig.iBrdAddr).
		SetAddress(iSettings.iBroadcastAddr);
	TInetAddr::Cast(config->iConfig.iDefGate).SetAddress(iSettings.iDefGateway);
	TInetAddr::Cast(config->iConfig.iNameSer1).
		SetAddress(iSettings.iPrimaryDns);
	TInetAddr::Cast(config->iConfig.iNameSer2).
		SetAddress(iSettings.iSecondaryDns);

	return KErrNone;
	}

TInt CIPv4ProtocolIf::WriteIfInfo(TDes8& aIfInfo)
/**
 * Writes information about the interface into the supplied descriptor.
 *
 * @param aIfInfo Will contain the interface information
 * @return KErrArgument if an incorrect structure is passed, otherwise KErrNone
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::WriteIfInfo"));

	if (aIfInfo.Length() != sizeof(TSoIfInfo))
		{
		return KErrArgument;
		}

	TSoIfInfo* info = (TSoIfInfo*) aIfInfo.Ptr();
	info->iFeatures = KIfCanBroadcast | KIfCanMulticast;
	info->iSpeedMetric = iSpeedMetric;
	info->iMtu = KDefaultMtu;
	WriteIfName(info->iName);

	return KErrNone;
	}

TInt CIPv4ProtocolIf::WriteConnectionInfo(TDes8& aConnectionInfo)
/**
 * Writes information about the IAP and network we're associated with. This
 * is used to route data in a multi-homing environment.
 *
 * @param aConnectionInfo Will contain the connection information
 * @return Standard CommDB error codes
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::WriteConnectionInfo"));

	TSoIfConnectionInfo* connectionInfo =
		(TSoIfConnectionInfo*) aConnectionInfo.Ptr();

	TInt err;
	// Append the 2 commdb filed into this descriptor
	TBuf<2*KCommsDbSvrMaxColumnNameLength+1> fieldName;

	fieldName.Copy(TPtrC(IAP));
	fieldName.Append(KSlashChar);
	fieldName.Append(TPtrC(COMMDB_ID));
	err = GetNifMain().GetAgent()->ReadInt(fieldName, connectionInfo->iIAPId);
	if (err != KErrNone)
		{
		return err;
		}

	fieldName.Copy(TPtrC(IAP));
	fieldName.Append(KSlashChar);
	fieldName.Append(TPtrC(IAP_NETWORK));
	err = GetNifMain().GetAgent()->ReadInt(fieldName, connectionInfo->iNetworkId);
	if (err != KErrNone)
		{
		return err;
		}

	return KErrNone;
	}
#ifdef WCDMA_STUB

TInt CIPv4ProtocolIf::DeleteContext(TDes8& aContextParameters)
/**
 * Deletes a context. As the NIF is responsible for one primary context,
 * this is equivalent to closing down the NIF.
 *
 * @param aContextParameters Parameters of the context to delete
 * @return KErrArgument if an incorrect structure is passed, otherwise KErrNone
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::DeleteContext"));

	if (aContextParameters.Length() != sizeof(TContextParameters))
		{
		return KErrArgument;
		}

	TUint8* ptr = CONST_CAST(TUint8*, aContextParameters.Ptr());
	TContextParameters* params = REINTERPRET_CAST(TContextParameters*, ptr);

	if (params->iContextInfo.iContextId != 
		STATIC_CAST(TInt8, GetNifMain().GetBcaController()->Nsapi()))
		{
		params->iReasonCode = KErrBadName;
		}
	else
		{
		params->iReasonCode = KErrNone; 
		GetNifMain().Stop(KErrNone, MNifIfNotify::EDisconnect);
		}

	return KErrNone;
	}

#endif

void CIPv4ProtocolIf::ReadCommDbGprsSettingsL()
/**
 * Reads GPRS IP settings from CommDB via the agent.
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::ReadCommDbGprsSettings"));

	// Read the IP address settings from CommDB.
	ReadIPv4SettingL(TPtrC(GPRS_IP_ADDR), iSettings.iLocalAddr);
	ReadIPv4SettingL(TPtrC(GPRS_IP_NETMASK), iSettings.iNetMask);
	ReadIPv4SettingL(TPtrC(GPRS_IP_NAME_SERVER1), iSettings.iPrimaryDns);
	ReadIPv4SettingL(TPtrC(GPRS_IP_NAME_SERVER2), iSettings.iSecondaryDns);
	ReadIPv4SettingL(TPtrC(GPRS_IP_GATEWAY), iSettings.iDefGateway);

	// Read whether to get the gateway IP address from the server.
	_BTT_LEAVEIFERROR(GetNifMain().GetAgent()->ReadBool(
		TPtrC(GPRS_IP_ADDR_FROM_SERVER),
		iSettings.iGetGatewayFromServer));

	// Read whether to get the DNS addresses from the server.
	_BTT_LEAVEIFERROR(GetNifMain().GetAgent()->ReadBool(
		TPtrC(GPRS_IP_DNS_ADDR_FROM_SERVER),
		iSettings.iGetDnsFromServer));

	// Because CommDB doesn't define a Broadcast Address field, we must
	// calculate the broadcast address. This is based on the localAddr
	// and the netMask.
	TInetAddr localAddr(iSettings.iLocalAddr, 0);
	TInetAddr netMask(iSettings.iNetMask, 0);
	TInetAddr broadcast;
	broadcast.SubNetBroadcast(localAddr, netMask);
	iSettings.iBroadcastAddr = broadcast.Address();
	}

void CIPv4ProtocolIf::UpdateContextConfigL(const
										  RPacketContext::TContextConfigGPRS&
										  aConfig)
/**
 * Called when the context has been activated to set our IP address and get
 * any other required settings from CommDB.
 *
 * @param aConfig The new context config
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::UpdateContextConfig"));

	// Get the settings which are stored in CommDB.
	ReadCommDbGprsSettingsL();

	// Get our IP address from the GPRS context config.
	TInetAddr address;
	
	TBuf<RPacketContext::KMaxPDPAddressLength> tempAddr;
	tempAddr.Copy(aConfig.iPdpAddress);
	TInt ret = address.Input(tempAddr);

	// We've got our IP address! Let's save it.
	if (ret == KErrNone)
		{
		iSettings.iLocalAddr = address.Address();
		LOG_IP_ADDRESS("Got local IP address", iSettings.iLocalAddr);
		}
	else
		{
		_LOG_L2C2(_L8("Couldn't get IP address from GPRS config (err: %d)"),
			ret);

		// Don't leave on this error: we may still be OK if we read some
		// settings from CommDB.
		}

	// @todo - is this correct. We can only get the DNS addresses
	// from the TSY using the iProtocolConfigOption data. Yet a client could
	// access those DNS config details without knowing about the state of the
	// iSettings.iGetDnsFromServer flag.

	if ((iSettings.iGetDnsFromServer) ||
		((iSettings.iPrimaryDns == 0) &&
		 (iSettings.iSecondaryDns == 0)) ) 
		{
		TBuf<RPacketContext::KMaxPDPAddressLength> tempAddr;
		tempAddr.Copy(aConfig.iProtocolConfigOption.iDnsAddresses.iPrimaryDns);
		ret = address.Input(tempAddr);

		if (ret == KErrNone)
			{
			iSettings.iPrimaryDns = address.Address();
			LOG_IP_ADDRESS("Got primary DNS", iSettings.iPrimaryDns);
			}
		else
			{
			_LOG_L2C2(_L8("Couldn't get primary DNS address from GPRS config (err: %d)"),
				ret);

			// Don't leave on this error: we may still be OK if we read some
			// settings from CommDB.
			}

		tempAddr.Copy(aConfig.iProtocolConfigOption.iDnsAddresses.iSecondaryDns);
		ret = address.Input(tempAddr);

		if (ret == KErrNone)
			{
			iSettings.iSecondaryDns = address.Address();
			LOG_IP_ADDRESS("Got secondary DNS", iSettings.iPrimaryDns);
			}
		else
			{
			_LOG_L2C2(_L8("Couldn't get secondary DNS address from GPRS config (err: %d)"),
				ret);

			// Don't leave on this error: we may still be OK if we read some
			// settings from CommDB.
			}
		}
	else
		{
		LOG_IP_ADDRESS("Using CommDB DNS address - Primary ", iSettings.iPrimaryDns);
		LOG_IP_ADDRESS("                         - Secondary ", iSettings.iSecondaryDns);
		}


	// TProtocolConfigOptionV2::iMiscBuffer is not the correct way to pass a gateway
	// address - data needs to be passed in TLV format but there are no TLV content tags
	// defined for a gateway address and any UMTS/GPRS hardware which claims to be able
	// to supply this address is erroneous (3gpp standard 24.008, section 10.5.6.3).
	// This misuse of iMiscBuffer was preventing other correctly formed parameters from
	// being passed (INC113612).
/*	if (iSettings.iGetGatewayFromServer)
		{
		tempAddr.Copy(aConfig.iProtocolConfigOption.iMiscBuffer);
		ret = address.Input(tempAddr);

		if (ret == KErrNone)
			{
			iSettings.iDefGateway = address.Address();
			LOG_IP_ADDRESS("Got default gateway", iSettings.iDefGateway);
			}
		else
			{
			_LOG_L2C2(_L8("Couldn't get default gateway from GPRS config (err: %d)"),
				ret);
			}
		}*/
	}

void CIPv4ProtocolIf::UpdateConnectionSpeed(TUint aConnectionSpeed)
/**
 * Sets the speed metric to return to TCP/IP, based on what the TSY tells us.
 *
 * @param aConnectionSpeed Our connection speed
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::UpdateConnectionSpeed"));

	iSpeedMetric = aConnectionSpeed;
	}

void CIPv4ProtocolIf::ReadIPv4SettingL(const TDesC& aFieldName,
									   TUint32& aIpAddr)
/**
 * Reads an IPv4 address from CommDB, via the TRP agent. It leaves if an error
 * other than KErrNotFound occurs.
 *
 * @param aFieldName The name of the field to read
 * @param aIpAddr Will contain the IP address
 */
	{
#ifdef __BTT_LOGGING__
	TBuf8<256> debugBuffer;
	debugBuffer.Copy(aFieldName);
	_LOG_L1C2(_L8("CIPv4ProtocolIf::ReadIp4Setting [aFieldName=%S]"),
		&debugBuffer);
#endif

	TBuf<KCommsDbSvrMaxFieldLength> name;
	TInetAddr ip4Addr;

	TInt ret = GetNifMain().GetAgent()->ReadDes(aFieldName, name);

	if (ret == KErrNone)
		{
		// We've successfully read an IP address, so convert it into a number.
		ip4Addr.Input(name);
		aIpAddr = ip4Addr.Address();
		}
	else
		{
		// We couldn't get the IP address, so set it to 0.0.0.0. Note that we
		// only leave here if we got a more serious error than "not found".
		aIpAddr = 0;
		if (ret != KErrNotFound)
			{
			_BTT_LEAVEIFERROR(ret);
			}
		}
	}

TInt CIPv4ProtocolIf::Send(RMBufChain& aPdu, TAny* aSource)
/**
 * Called by the protocol to send an outgoing IP packet to the network.
 *
 * @param aPdu The outgoing packet
 * @param aSource The source of the packet
 * @return Standard error codes
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::Send"));

#ifdef __BTT_LOGGING__
	LogPacket(aPdu);
#endif

	// Return <0: an error occurred
	// Return  0: no error, but don't send any more packets

	return GetNifMain().SendPacket(aPdu, aSource, KIp4FrameType);
	}

void CIPv4ProtocolIf::Info(TNifIfInfo& aInfo) const
/**
 * Called by the protocol to get information about the NIF.
 *
 * @param aInfo Will contain NIF information
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::Info"));

	// The parent class sets everything except the name...
	CProtocolIfBase::Info(aInfo);
	// ...which we set here.
	WriteIfName(aInfo.iName);
	}

TInt CIPv4ProtocolIf::Notification(TAgentToNifEventType /*aEvent*/, 
	void* /*aInfo*/)
/**
 * The Nif will ignore any notification sent
 *
 * @param aEvent Not used
 * @param aInfo Not used 
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::Notification"));

	return KErrNone;
	}

void CIPv4ProtocolIf::StartSending(CProtocolBase* aProtocol)
/**
 * Indicates to the protocol layer that the NIF is ready to send packets.
 *
 * @param aProtocol A pointer to a protocol
 */
	{
	_LOG_L1C2(_L8("CIPv4ProtocolIf::StartSending [aProtocol=%X]"), aProtocol);

	CProtocolIfBase::StartSending(aProtocol);
	}

TBool CIPv4ProtocolIf::WantsProtocol(TUint16 aProtocolCode)
/**
 * Indicates the type of protocol implemented by this class.
 *
 * @param aProtocolCode The protocol type
 */
	{
	_LOG_L1C2(_L8("CIPv4ProtocolIf::WantsProtocol [aProtocolCode=%X]"),
		aProtocolCode);

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
	return ((aProtocolCode & 0x00FF) == KIp4FrameType);
#else
	(void) aProtocolCode; // disable compiler warning
	return ETrue;
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
	}

void CIPv4ProtocolIf::Process(RMBufChain& aPdu)
/**
 * Called when an incoming IP packet has arrived. Send packets up to the
 * TCP/IP stack.
 *
 * @param aPdu The incoming packet
 */
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::Process"));

#ifdef __BTT_LOGGING__
	LogPacket(aPdu);
#endif

	// Pass incoming packets up to the protocol, unless it hasn't
	// been bound yet.
	if (iProtocol)
		{
		_LOG_L1C1(_L8("CIPv4ProtocolIf: Packet Sent to TCP/IP Protocol!!!"));
		iProtocol->Process(aPdu, reinterpret_cast<CProtocolBase*>(this));
		}
	else 
		{
		_LOG_L2C1(_L8("WARNING: dumping incoming packet, no protocol bound"));
		aPdu.Free();
		}
	}

#ifdef __BTT_LOGGING__
void CIPv4ProtocolIf::LogPacket(const RMBufChain& aPacket)
/**
* Logs packet information into log file.
*
* @param aPacket The packet 
*/
	{
	_LOG_L1C1(_L8("CIPv4ProtocolIf::LogPacket"));

	TInt mBufLength = aPacket.Length() - aPacket.First()->Length();

	_LOG_L3C2(_L8("Analysis of %d byte packet:"), mBufLength);

	//Note: All the constants used on this method are a pragmatic guess of the
	//IP header fields. The only porpose of this method is logging.

	if (mBufLength < 20)
		{
		_LOG_L3C2(_L8(" -doesn't appear to be a valid IPv4 packet (length=%d)")
			, mBufLength);
		return;
		}

	// Get a pointer to the packet's payload.
	const TUint8* payloadPtr = aPacket.First()->Next()->Ptr();

	if ((payloadPtr[0] & 0xF0) != 0x40)
		{
		_LOG_L3C2(_L8(" - doesn't appear to be an IPv4 packet (version=0x%X)"),
			(payloadPtr[0] & 0xF0) >> 4);
		return;
		}

	if ((payloadPtr[0] & 0xF) != 0x5)
		{
		_LOG_L3C2(_L8(" - doesn't have a standard IP header (length=0x%X)"),
			payloadPtr[0] & 0xF);
		return;
		}

	_LOG_L3C5(_L8(" - src addr: %d.%d.%d.%d"), payloadPtr[12], payloadPtr[13],
		payloadPtr[14], payloadPtr[15]);
	_LOG_L3C5(_L8(" - dst addr: %d.%d.%d.%d"), payloadPtr[16], payloadPtr[17],
		payloadPtr[18], payloadPtr[19]);

	if (payloadPtr[9] == 0x06)
		{
		_LOG_L3C1(_L8(" - appears to be a TCP packet"));
		if (mBufLength < 40)
			{
			_LOG_L3C2(_L8(" - but is too short (length=0x%X)"), mBufLength);
			return;
			}
		_LOG_L3C3(_L8(" - src port: %d, dst port: %d"),
			(payloadPtr[20] << 8) + payloadPtr[21],
			(payloadPtr[22] << 8) + payloadPtr[23]);
		_LOG_L3C3(_L8(" - seq #: 0x%08X, ack #: 0x%08X"),
			(payloadPtr[24] << 24) + (payloadPtr[25] << 16) +
			(payloadPtr[26] << 8) + payloadPtr[27],
			(payloadPtr[28] << 24) + (payloadPtr[29] << 16) +
			(payloadPtr[30] << 8) + payloadPtr[31]);

		TBuf8<100> flagsSet;
		flagsSet.Copy(_L8(" - flags set: "));

		// Write description of payload's flags to "flagsSet"
		if (payloadPtr[33] & 0x01)
			{
			flagsSet.Append(_L8("FIN "));
			}
		if (payloadPtr[33] & 0x02)
			{
			flagsSet.Append(_L8("SYN "));
			}
		if (payloadPtr[33] & 0x04)
			{
			flagsSet.Append(_L8("RST "));
			}
		if (payloadPtr[33] & 0x08)
			{
			flagsSet.Append(_L8("PSH "));
			}
		if (payloadPtr[33] & 0x10)
			{
			flagsSet.Append(_L8("ACK "));
			}
		if (payloadPtr[33] & 0x20)
			{
			flagsSet.Append(_L8("URG "));
			}
		_LOG_L3C1(flagsSet);

		TInt dataOffset = payloadPtr[32] >> 2; // in bytes
		if ((dataOffset > 0) && (mBufLength > dataOffset + 30))
			{
			TBuf8<100> data;
			data.Copy(_L8(" - data begins: "));
			for (TInt i = dataOffset + 20; i < dataOffset + 100; i++)
				{
				// We skip some bytes at the end of the MBuf, as they're junk.
				if (i >= (mBufLength - 10))
					{
					break;
					}
				if (TChar(payloadPtr[i]).IsPrint())
					{
					data.Append(TChar(payloadPtr[i]));
					}
				else
					{
					data.Append(TChar('?'));
					}
				}
			_LOG_L3C1(data);
			}
		}
	else if (payloadPtr[9] == 0x01)
		{
		_LOG_L3C1(_L8(" - appears to be an ICMP packet"));
		if (mBufLength < 24)
			{
			_LOG_L3C2(_L8(" - but is too short (length=0x%X)"), mBufLength);
			return;
			}

		if (payloadPtr[20] == 0x8)
			{
			_LOG_L3C1(_L8(" - is an echo request"));
			}
		else if (payloadPtr[20] == 0x0)
			{
			_LOG_L3C1(_L8(" - is an echo reply"));
			}
		else
			{
			_LOG_L3C2(_L8(" - unknown type (0x%02X)"), payloadPtr[20]);
			return;
			}

		if (mBufLength >= 28)
			{
			_LOG_L3C3(_L8(" - ID: 0x%04X, seq #: 0x%04X"),
				(payloadPtr[24] << 8) + payloadPtr[25],
				(payloadPtr[26] << 8) + payloadPtr[27]);
			}
		}
	else if (payloadPtr[9] == 0x11)
		{
		_LOG_L3C1(_L8(" - appears to be a UDP packet"));
		if (mBufLength < 28)
			{
			_LOG_L3C2(_L8(" - but is too short (length=0x%X)"), mBufLength);
			return;
			}
		_LOG_L3C3(_L8(" - src port: %d, dst port: %d"),
			(payloadPtr[20] << 8) + payloadPtr[21],
			(payloadPtr[22] << 8) + payloadPtr[23]);
		}
	else
		{
		_LOG_L3C2(_L8(" - appears to be for an unknown protocol (0x%X)"),
			payloadPtr[9]);
		}
	}
#endif // __BTT_LOGGING__
