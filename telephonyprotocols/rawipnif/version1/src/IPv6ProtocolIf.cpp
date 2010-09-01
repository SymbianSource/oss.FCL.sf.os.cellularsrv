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
// This file implements the CIPv6ProtocolIf class, which handles the transmission
// of IPv6 data to and from the TCP/IP stack.
// 
//

/**
 @file
*/

#include <e32hal.h>	// UserHal::MachineInfo()
#include <etelpckt.h>
#include <in_iface.h>
#include <in6_if.h>
#include "RawIPNifMain.h"
#include "IPv6ProtocolIf.h"
#ifdef WCDMA_STUB
#include <UMTSNifControlIf.h>
#endif
#define LOG_IP_ADDRESS(desc,addr) _LOG_L2C5(_L8("    " desc " = %d:%d:%d:%d from context"), \
			addr[3], addr[2], addr[1], addr[0]);

CIPv6ProtocolIf::CIPv6ProtocolIf(CRawIPNifMain& aNifMain, CBttLogger* aTheLogger)
/**
 * Constructor
 */ 
	: CProtocolIfBase(aNifMain,aTheLogger),
	  iTheLogger(aTheLogger), 
	  iSpeedMetric(KDefaultSpeedMetric)
	{
	}

CIPv6ProtocolIf::~CIPv6ProtocolIf()
/**
 *	Destructor
 */
	{
	}

void CIPv6ProtocolIf::BindL(TAny* aId)
/**
 *	Binds protocol to the Nif
 *
 *	@param aId A point the the protocol(TCP/IP)
 */
	{
	_LOG_L1C2(_L8("CIPv6ProtocolIf::BindL [aId=%X]"), aId);

	CProtocolIfBase::BindL(aId); // Call the superclass's method.
	iNotify = iNifMain.Notify();
	}

TInt CIPv6ProtocolIf::Control(TUint aLevel, TUint aName, TDes8& aOption,
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
	_LOG_L1C3(_L8("CIPv6ProtocolIf::Control [aLevel=%d, aName=%d]"),
		aLevel, aName);

	if (aLevel == KSOLInterface)
		{
		switch (aName)
			{
		// General IP configuration commands are below this point.

		case KSoIfInfo6:
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
			// If the IPv6 interface is up, then the context will already have
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

TInt CIPv6ProtocolIf::WriteIfConfig(TDes8& aIfConfig)
/**
 * Writes the configuration of the interface (eg. IP address, DNS servers)
 * into the supplied descriptor.
 *
 * @param aIfConfig Will contain the interface configuration
 * @return KErrNotSupported if an incorrect structure is passed, 
 * otherwise KErrNone
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::WriteIfConfig"));
	__ASSERT_DEBUG((TUint)aIfConfig.MaxLength() >= sizeof (TSoInet6IfConfig), User::Panic(KIPv6ProtocolIfName, 0));

	if ((TUint)aIfConfig.MaxLength() < sizeof(TSoInet6IfConfig))
		{
		return KErrArgument;
		}

	TUint8* ptr = CONST_CAST(TUint8*, aIfConfig.Ptr());
	TSoInet6IfConfig* config = REINTERPRET_CAST(TSoInet6IfConfig*, ptr);

	TEui64Addr& localId = TEui64Addr::Cast(config->iLocalId);
	localId = iSettings.iLocalIfId;

	// If required, configure static DNS addresses
	if (!iSettings.iPrimaryDns.IsUnspecified())
		{
		config->iNameSer1.SetAddress(iSettings.iPrimaryDns);
		if (!iSettings.iSecondaryDns.IsUnspecified())
			config->iNameSer2.SetAddress(iSettings.iSecondaryDns);
		}
		
	config->idPaddingBits = 0;

	return KErrNone;
	}

TInt CIPv6ProtocolIf::WriteIfInfo(TDes8& aIfInfo)
/**
 * Writes information about the interface into the supplied descriptor.
 *
 * @param aIfInfo Will contain the interface information
 * @return KErrArgument if an incorrect structure is passed, otherwise KErrNone
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::WriteIfInfo"));
	__ASSERT_DEBUG((TUint)aIfInfo.MaxLength() >= sizeof (TSoIfInfo6), User::Panic(KIPv6ProtocolIfName, 0));

	if ((TUint)aIfInfo.MaxLength() < sizeof(TSoIfInfo6))
		{
		return KErrArgument;
		}

	TSoIfInfo6* info = (TSoIfInfo6*) aIfInfo.Ptr();
	info->iFeatures = KIfCanBroadcast | KIfCanMulticast;
	info->iSpeedMetric = iSpeedMetric;
	info->iMtu = KDefaultMtu;
	info->iRMtu = KDefaultMtu;
	WriteIfName(info->iName);

	return KErrNone;
	}

TInt CIPv6ProtocolIf::WriteConnectionInfo(TDes8& aConnectionInfo)
/**
 * Writes information about the IAP and network we're associated with. This
 * is used to route data in a multi-homing environment.
 *
 * @param aConnectionInfo Will contain the connection information
 * @return Standard CommDB error codes
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::WriteConnectionInfo"));

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

TInt CIPv6ProtocolIf::DeleteContext(TDes8& aContextParameters)
/**
 * Deletes a context. As the NIF is responsible for one primary context,
 * this is equivalent to closing down the NIF.
 *
 * @param aContextParameters Parameters of the context to delete
 * @return KErrArgument if an incorrect structure is passed, otherwise KErrNone
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::DeleteContext"));

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

void CIPv6ProtocolIf::ReadCommDbGprsSettingsL()
/**
 * Reads GPRS IP settings from CommDB via the agent.
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::ReadCommDbGprsSettings"));
	
	// Read he DNS addresses..
	ReadIPv6SettingL(TPtrC(GPRS_IP6_NAME_SERVER1), iSettings.iPrimaryDns);
	ReadIPv6SettingL(TPtrC(GPRS_IP6_NAME_SERVER2), iSettings.iSecondaryDns);	
	// Read whether to get the DNS addresses from the server.
	_BTT_LEAVEIFERROR(GetNifMain().GetAgent()->ReadBool(
		TPtrC(GPRS_IP6_DNS_ADDR_FROM_SERVER),
		iSettings.iGetDnsFromServer));
	
	// Read whether to get IPv4 address from the server
	// This is only needed for the integration tests. If it's true then 
	// the IPv4 address will be used to build up the IPv6 address. 
	_BTT_LEAVEIFERROR(GetNifMain().GetAgent()->ReadBool(
		TPtrC(GPRS_IP_ADDR_FROM_SERVER),
		iSettings.iGetIpFromServer));
		

	if (iSettings.iGetIpFromServer == EFalse)
		{
		// Sets the IPv6 Link-local address from IpAddr.
		// LocalId is derived from IpAddr and it's further used to set the Link-local
		// address elsewhere by adding a prefix (FE80::) in front.
		// For IpAddr: 192.168.1.1, link-local address will be FE80::C0A8:101.
		TUint32 ipAddr;	
		ReadIPv4SettingL(TPtrC(GPRS_IP_ADDR), ipAddr);
		const TUint8 constantId[8] = { 0, 0, 0, 0, 
									ipAddr >> 24, (ipAddr >> 16) & 0xFF, 
									(ipAddr >> 8) & 0xFF, ipAddr & 0xFF }; 
		iSettings.iLocalIfId.SetAddr(constantId, sizeof (constantId));
		}
	else
		{
			
		//
		// Use the 64 bit id of MARM machines as our interface id
		//
		TMachineInfoV1Buf machineInfo;	
		UserHal::MachineInfo(machineInfo);
		iSettings.iLocalIfId.SetAddr(machineInfo().iMachineUniqueId);
		iSettings.iLocalIfId.SetUniversalBit(0);
		//
		// In WINS environment the id is zero which is no-no
		//
		if (iSettings.iLocalIfId.IsZero())
			{
			iSettings.iLocalIfId.SetAddrRandomNZ();
			}		

		}
	}

void CIPv6ProtocolIf::UpdateContextConfigL(const
										  RPacketContext::TContextConfigGPRS&
										  aConfig)
/**
 * Called when the context has been activated to set our IP address and get
 * any other required settings from CommDB.
 *
 * @param aConfig The new context config
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::UpdateContextConfig"));

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
		iSettings.iLocalAddr = address.Ip6Address();
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
		((iSettings.iPrimaryDns.IsUnspecified()) &&
		 (iSettings.iSecondaryDns.IsUnspecified())) ) 
		{
		TBuf<RPacketContext::KMaxPDPAddressLength> tempAddr;
		tempAddr.Copy(aConfig.iProtocolConfigOption.iDnsAddresses.iPrimaryDns);
		ret = address.Input(tempAddr);

		if (ret == KErrNone)
			{
			iSettings.iPrimaryDns = address.Ip6Address();
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
			iSettings.iSecondaryDns = address.Ip6Address();
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
	}

void CIPv6ProtocolIf::UpdateConnectionSpeed(TUint aConnectionSpeed)
/**
 * Sets the speed metric to return to TCP/IP, based on what the TSY tells us.
 *
 * @param aConnectionSpeed Our connection speed
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::UpdateConnectionSpeed"));

	iSpeedMetric = aConnectionSpeed;
	}

void CIPv6ProtocolIf::ReadIPv4SettingL(const TDesC& aFieldName,
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
	_LOG_L1C2(_L8("CIPv6ProtocolIf::ReadIp4Setting [aFieldName=%S]"),
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

void CIPv6ProtocolIf::ReadIPv6SettingL(const TDesC& aFieldName,
									   TIp6Addr& aIpAddr)
/**
 * Reads an IPv6 address from CommDB, via the TRP agent. It leaves if an error
 * other than KErrNotFound occurs.
 *
 * @param aFieldName The name of the field to read
 * @param aIpAddr Will contain the IP address
 */
	{
#ifdef __BTT_LOGGING__
	TBuf8<256> debugBuffer;
	debugBuffer.Copy(aFieldName);
	_LOG_L1C2(_L8("CIPv6ProtocolIf::ReadIp6Setting [aFieldName=%S]"),
		&debugBuffer);
#endif

	TBuf<KCommsDbSvrMaxFieldLength> name;
	TInetAddr ip6Addr;

	TInt ret = GetNifMain().GetAgent()->ReadDes(aFieldName, name);

	if (ret == KErrNone)
		{
		// We've successfully read an IP address, so convert it into a number.
		ip6Addr.Input(name);
		aIpAddr = ip6Addr.Ip6Address();
		}
	else
		{
		// We couldn't get the IP address, so set it to 0.0.0.0. Note that we
		// only leave here if we got a more serious error than "not found".
		aIpAddr = KInet6AddrNone;
		if (ret != KErrNotFound)
			{
			_BTT_LEAVEIFERROR(ret);
			}
		}
	}

TInt CIPv6ProtocolIf::Send(RMBufChain& aPdu, TAny* aSource)
/**
 * Called by the protocol to send an outgoing IP packet to the network.
 *
 * @param aPdu The outgoing packet
 * @param aSource The source of the packet
 * @return Standard error codes
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::Send"));

#ifdef __BTT_LOGGING__
	LogPacket(aPdu);
#endif

	// Return <0: an error occurred
	// Return  0: no error, but don't send any more packets

	return GetNifMain().SendPacket(aPdu, aSource, KIp6FrameType);
	}

void CIPv6ProtocolIf::Info(TNifIfInfo& aInfo) const
/**
 * Called by the protocol to get information about the NIF.
 *
 * @param aInfo Will contain NIF information
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::Info"));

	// The parent class sets everything except the name...
	CProtocolIfBase::Info(aInfo);
	// ...which we set here.
	WriteIfName(aInfo.iName);
	}

TInt CIPv6ProtocolIf::Notification(TAgentToNifEventType /*aEvent*/, 
	void* /*aInfo*/)
/**
 * The Nif will ignore any notification sent
 *
 * @param aEvent Not used
 * @param aInfo Not used 
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::Notification"));

	return KErrNone;
	}

void CIPv6ProtocolIf::StartSending(CProtocolBase* aProtocol)
/**
 * Indicates to the protocol layer that the NIF is ready to send packets.
 *
 * @param aProtocol A pointer to a protocol
 */
	{
	_LOG_L1C2(_L8("CIPv6ProtocolIf::StartSending [aProtocol=%X]"), aProtocol);

	CProtocolIfBase::StartSending(aProtocol);
	}

TBool CIPv6ProtocolIf::WantsProtocol(TUint16 aProtocolCode)
/**
 * Indicates the type of protocol implemented by this class.
 *
 * @param aProtocolCode The protocol type
 */
	{
	_LOG_L1C2(_L8("CIPv6ProtocolIf::WantsProtocol [aProtocolCode=%X]"),
		aProtocolCode);

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
	return ((aProtocolCode & 0x00FF) == KIp6FrameType);
#else
	(void) aProtocolCode; // disable compiler warning
	return ETrue;
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
	}

void CIPv6ProtocolIf::Process(RMBufChain& aPdu)
/**
 * Called when an incoming IP packet has arrived. Send packets up to the
 * TCP/IP stack.
 *
 * @param aPdu The incoming packet
 */
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::Process"));

#ifdef __BTT_LOGGING__
	LogPacket(aPdu);
#endif

	// Pass incoming packets up to the protocol, unless it hasn't
	// been bound yet.
	if (iProtocol)
		{
		_LOG_L1C1(_L8("CIPv6ProtocolIf: Packet Sent to TCP/IP Protocol!!!"));
		iProtocol->Process(aPdu, reinterpret_cast<CProtocolBase*>(this));
		}
	else 
		{
		_LOG_L2C1(_L8("WARNING: dumping incoming packet, no protocol bound"));
		aPdu.Free();
		}
	}

#ifdef __BTT_LOGGING__
void CIPv6ProtocolIf::LogPacket(const RMBufChain& aPacket)
/**
* Logs packet information into log file.
*
* @param aPacket The packet 
*/
	{
	_LOG_L1C1(_L8("CIPv6ProtocolIf::LogPacket"));

	TInt mBufLength = aPacket.Length() - aPacket.First()->Length();

	_LOG_L3C2(_L8("Analysis of %d byte packet:"), mBufLength);

	//Note: All the constants used on this method are a pragmatic guess of the
	//IP header fields. The only porpose of this method is logging.

	if (mBufLength < 40)
		{
		_LOG_L3C2(_L8(" -doesn't appear to be a valid IPv6 packet (length=%d)")
			, mBufLength);
		return;
		}

	// Get a pointer to the packet's payload.
	const TUint8* payloadPtr = aPacket.First()->Next()->Ptr();

	if ((payloadPtr[0] & 0xF0) != 0x60)
		{
		_LOG_L3C2(_L8(" - doesn't appear to be an IPv6 packet (version=0x%X)"),
			(payloadPtr[0] & 0xF0) >> 4);
		return;
		}

	_LOG_L3C2(_L8(" - traffic class: 0x%X"), 
					((payloadPtr[0] & 0xF) << 4) | ((payloadPtr[1] & 0xF0) >> 4));
	_LOG_L3C2(_L8(" - flow label: 0x%X"), 
					((payloadPtr[1] & 0x0F) << 16) | (payloadPtr[2] << 8) | payloadPtr[3]);
	_LOG_L3C2(_L8(" - payload length: 0x%X"), 
					(payloadPtr[4] << 16) | payloadPtr[5]);
	_LOG_L3C2(_L8(" - next header: 0x%08X"), payloadPtr[6]);
	_LOG_L3C2(_L8(" - hop limit: 0x%08X"), payloadPtr[7]);
	}
#endif // __BTT_LOGGING__
