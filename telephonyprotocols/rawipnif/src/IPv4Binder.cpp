// Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// This file implements the CIPv4Binder class, which handles the transmission
// of IPv4 data to and from the TCP/IP stack.
// 
//

/**
 @file
*/

#include <etelpckt.h>
#include <in_iface.h>
#include "RawIPFlow.h"
#include "IPv4Binder.h"
#include <comms-infras/linkprovision.h>

using namespace ESock;
#ifdef WCDMA_STUB
#include <networking/umtsnifcontrolif.h>
#endif

#define LOG_IP_ADDRESS(desc,addr) _LOG_L1C5(_L8("    " desc " = %d.%d.%d.%d"), \
			addr >> 24, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF);

CIPv4Binder::CIPv4Binder(CRawIPFlow& aFlow, CBttLogger* aTheLogger)
/**
 * Constructor
 */ 
	: CBinderBase(aFlow,aTheLogger),
	  iSpeedMetric(KDefaultSpeedMetric)
	{
	}

CIPv4Binder::~CIPv4Binder()
/**
 *	Destructor
 */
	{
	}

MLowerDataSender* CIPv4Binder::Bind(MUpperDataReceiver* aUpperReceiver, MUpperControl* aUpperControl)
/**
 * Binds TCP/IP protocol to Flow
 *
 * @param aUpperReceiver A pointer to Upper layer Receive class
 * @param aUpperControl A pointer to Upper layer control class
 */
	{
	CBinderBase::Bind(aUpperReceiver, aUpperControl); // Call the superclass's method.
	return this;
	}

TInt CIPv4Binder::Control(TUint aLevel, TUint aName, TDes8& aOption)
/**
 * The main function called by the TCP/IP protocol to control the interface.
 * Can perform a variety of general IP tasks (such as getting IP config)
 * and "3G" specific tasks (such as deleting the context).
 *
 * @param aLevel The level of the interface to control - always KSOLInterface
 * @param aName The command to perform
 * @param aOption Data to be input/output as a result of the command
 * @return Standard error codes
 */
	{
	_LOG_L1C3(_L8("CIPv4Binder::Control [aLevel=%d, aName=%d]"),
		aLevel, aName);

	if (aLevel == KSOLInterface)
		{
		switch (aName)
			{
		case KSoIfHardwareAddr:
			// unsupported because we don't have a h/w address
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
				STATIC_CAST(TInt8, GetFlow().GetBcaController()->Nsapi()))
				{
				contextParams->iReasonCode = KErrNotFound;
				}
			else
				{
				contextParams->iContextInfo.iStatus =
					GetFlow().GetContextStatus();
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
		(void)aOption;
			break;
			}
		}
	return KErrNotSupported;
	}

TInt CIPv4Binder::GetConfig(TBinderConfig& aConfig)
	{
	_LOG_L1C1(_L8("CIPv4Binder::GetConfig"));
	
    TBinderConfig4* config = TBinderConfig::Cast<TBinderConfig4>(aConfig);
    
   	if(config == NULL)
   		{
   		return KErrNotSupported;
   		}
	
	config->iFamily = KAfInet;		/* KAfInet - selects TBinderConfig4 */
	
	config->iInfo.iFeatures = KIfCanBroadcast | KIfCanMulticast;		/* Feature flags */
	config->iInfo.iMtu = KDefaultMtu;				/* Maximum transmission unit. */
	config->iInfo.iRMtu = KDefaultMtu;				/* Maximum transmission unit for receiving. */
	config->iInfo.iSpeedMetric = iSpeedMetric;		/* approximation of the interface speed in Kbps. */

    LOG_IP_ADDRESS("Local IP address from TBinderConfig", iSettings.iLocalAddr);
	
	config->iAddress.SetAddress(iSettings.iLocalAddr);		/* Interface IP address. */
	config->iNetMask.SetAddress(iSettings.iNetMask);			/* IP netmask. */
	config->iBrdAddr.SetAddress(iSettings.iBroadcastAddr);	/* IP broadcast address. */
	config->iDefGate.SetAddress(iSettings.iDefGateway);		/* IP default gateway or peer address (if known). */
	config->iNameSer1.SetAddress(iSettings.iPrimaryDns);		/* IP primary name server (if any). */
	config->iNameSer2.SetAddress(iSettings.iSecondaryDns);	/* IP secondary name server (if any). */
		
	return KErrNone;
	}
#ifdef WCDMA_STUB

TInt CIPv4Binder::DeleteContext(TDes8& aContextParameters)
/**
 * Deletes a context. As the NIF is responsible for one primary context,
 * this is equivalent to closing down the NIF.
 *
 * @param aContextParameters Parameters of the context to delete
 * @return KErrArgument if an incorrect structure is passed, otherwise KErrNone
 */
	{
	_LOG_L1C1(_L8("CIPv4Binder::DeleteContext"));

	if (aContextParameters.Length() != sizeof(TContextParameters))
		{
		return KErrArgument;
		}

	TUint8* ptr = CONST_CAST(TUint8*, aContextParameters.Ptr());
	TContextParameters* params = REINTERPRET_CAST(TContextParameters*, ptr);

	if (params->iContextInfo.iContextId != 
		STATIC_CAST(TInt8, GetFlow().GetBcaController()->Nsapi()))
		{
		params->iReasonCode = KErrBadName;
		}
	else
		{
		params->iReasonCode = KErrNone; 
		GetFlow().Stop(KErrNone, MNifIfNotify::EDisconnect);
		}

	return KErrNone;
	}

#endif
/**
 * Called when the context has been activated to set our IP address and get
 * any other required settings from CommDB.
 *
 * @param aConfig The new context config
 */
void CIPv4Binder::UpdateContextConfigL(const TPacketDataConfigBase& aConfig)
	{
	_LOG_L1C1(_L8("CIPv4Binder::UpdateContextConfig"));

	// Get our IP address from the GPRS context config.
	TInetAddr address;
	
	TBuf<RPacketContext::KMaxPDPAddressLength> tempAddr;
	
	const RPacketContext::TProtocolConfigOptionV2* pco;
	TInt rel = const_cast<TPacketDataConfigBase&>(aConfig).ExtensionId();
	if (rel == TPacketDataConfigBase::KConfigGPRS) 
	    {
	    tempAddr.Copy(static_cast<const RPacketContext::TContextConfigGPRS&>(aConfig).iPdpAddress);
	    pco = &static_cast<const RPacketContext::TContextConfigGPRS&>(aConfig).iProtocolConfigOption;
	    }
    else
        {
        ASSERT(rel == TPacketDataConfigBase::KConfigRel99Rel4 || rel == TPacketDataConfigBase::KConfigRel5);
	    tempAddr.Copy(static_cast<const RPacketContext::TContextConfigR99_R4&>(aConfig).iPdpAddress);
	    pco = &static_cast<const RPacketContext::TContextConfigR99_R4&>(aConfig).iProtocolConfigOption;
        }
	TInt ret = address.Input(tempAddr);

	// We've got our IP address! Let's save it.
	if (ret == KErrNone)
		{
		iSettings.iLocalAddr = address.Address();
		LOG_IP_ADDRESS("Got local IP address from context", iSettings.iLocalAddr);
		iSettings.iDefGateway = address.Address();
		_LOG_L1C1(_L8("Set Default Gateway to local IP address"));
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
		tempAddr.Copy(pco->iDnsAddresses.iPrimaryDns);
		ret = address.Input(tempAddr);

		if (ret == KErrNone)
			{
			iSettings.iPrimaryDns = address.Address();
			LOG_IP_ADDRESS("Got primary DNS from context PCO", iSettings.iPrimaryDns);
			}
		else
			{
			_LOG_L2C2(_L8("Couldn't get primary DNS address from GPRS config (err: %d)"),
				ret);

			// Don't leave on this error: we may still be OK if we read some
			// settings from CommDB.
			}

		tempAddr.Copy(pco->iDnsAddresses.iSecondaryDns);
		ret = address.Input(tempAddr);

		if (ret == KErrNone)
			{
			iSettings.iSecondaryDns = address.Address();
			LOG_IP_ADDRESS("Got secondary DNS from context PCO", iSettings.iPrimaryDns);
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

void CIPv4Binder::UpdateConnectionSpeed(TUint aConnectionSpeed)
/**
 * Sets the speed metric to return to TCP/IP, based on what the TSY tells us.
 *
 * @param aConnectionSpeed Our connection speed
 */
	{
	_LOG_L1C1(_L8("CIPv4Binder::UpdateConnectionSpeed"));

	iSpeedMetric = aConnectionSpeed;
	}


MLowerDataSender::TSendResult CIPv4Binder::Send(RMBufChain& aPdu)
/**
 * Called by the protocol to send an outgoing IP packet to the network.
 *
 * @param aPdu The outgoing packet
 * @return MLowerDataSender::ESendBlocked or ESendAccepted based on state of flow.
 */
	{
	_LOG_L1C1(_L8("CIPv4Binder::Send"));

#ifdef __BTT_LOGGING__
	LogPacket(aPdu);
#endif

	// Return ESendBlocked: flow cannot accept any more packets [blocked, queue full, etc]
	// Return ESendAccepted: flow has accepted this packet and can accept another.

	return GetFlow().SendPacket(aPdu, NULL, KIp4FrameType);
	}

TInt CIPv4Binder::Notification(TAgentToNifEventType /*aEvent*/, 
	void* /*aInfo*/)
/**
 * The Nif will ignore any notification sent
 *
 * @param aEvent Not used
 * @param aInfo Not used 
 */
	{
	_LOG_L1C1(_L8("CIPv4Binder::Notification"));

	return KErrNone;
	}

void CIPv4Binder::StartSending()
/**
 * Indicates to the protocol layer that the NIF is ready to send packets.
 *
 * @param aProtocol A pointer to a protocol
 */
	{
	_LOG_L1C1(_L8("CIPv4Binder::StartSending()"));

	CBinderBase::StartSending();
	}

TBool CIPv4Binder::WantsProtocol(TUint16 aProtocolCode)
/**
 * Indicates the type of protocol implemented by this class.
 *
 * @param aProtocolCode The protocol type
 */
	{
	_LOG_L1C2(_L8("CIPv4Binder::WantsProtocol [aProtocolCode=%X]"),
		aProtocolCode);

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
	return ((aProtocolCode & 0x00FF) == KIp4FrameType);
#else
	(void) aProtocolCode;
	return ETrue;
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
	}

void CIPv4Binder::Process(RMBufChain& aPdu)
/**
 * Called when an incoming IP packet has arrived. Send packets up to the
 * TCP/IP stack.
 *
 * @param aPdu The incoming packet
 */
	{
	_LOG_L1C1(_L8("CIPv4Binder::Process"));

#ifdef __BTT_LOGGING__
	LogPacket(aPdu);
#endif

	// Pass incoming packets up to the protocol, unless it hasn't
	// been bound yet.
	if (iUpperReceiver)						// ASSERT(iUpperReceiver) ?
		{
		_LOG_L1C1(_L8("CIPv4Binder: Packet Sent to TCP/IP Protocol!!!"));
		iUpperReceiver->Process(aPdu);
		}
	else 
		{
		_LOG_L2C1(_L8("WARNING: dumping incoming packet, no protocol bound"));
		aPdu.Free();
		}
	}

//
// MLowerControl methods
//

TInt CIPv4Binder::GetName(TDes& aName)
/**
*/
	{
	WriteIfName(aName);
	return KErrNone;
	}

//
// CBinderBase methods
//

void CIPv4Binder::SetProvision(const CIPConfig& aProvision)
/**
Set provisioning information for IPv4 binder.

Called from RawIP Flow.

@param aProvision Provisioning structure from Control side.
*/
	{
	_LOG_L1C1(_L8("CIPv4Binder::SetProvision"));

	iSettings.iLocalAddr	 = aProvision.GetIpAddress();
	iSettings.iNetMask		 = aProvision.GetIpNetMask();
	iSettings.iBroadcastAddr = aProvision.GetBroadCastAddr();
	iSettings.iDefGateway	 = aProvision.GetIpGateway();
	iSettings.iPrimaryDns	 = aProvision.GetIp4NameServer1();
	iSettings.iSecondaryDns	 = aProvision.GetIp4NameServer2();
	iSettings.iGetGatewayFromServer = aProvision.GetIpAddrFromServer();
	iSettings.iGetDnsFromServer		= aProvision.GetIp4DNSAddrFromServer();
	
    LOG_IP_ADDRESS("Local IP address from Provisioning", iSettings.iLocalAddr);	
	}

#ifdef __BTT_LOGGING__
void CIPv4Binder::LogPacket(const RMBufChain& aPacket)
/**
* Logs packet information into log file.
*
* @param aPacket The packet 
*/
	{
	_LOG_L1C1(_L8("CIPv4Binder::LogPacket"));

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
