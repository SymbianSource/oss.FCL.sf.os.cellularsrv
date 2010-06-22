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
// Assigns default values to various packet context related structures
// 
//

/**
 @file
 @internalComponent
*/

#include "DefaultContextParameters.h"

_LIT(KTxtSimTsy,"SIM");
TName KSimTsyName(KTxtSimTsy);
TName& SimTsyName()
	{
	return KSimTsyName;
	}

_LIT(apn, "ACME INTERNET PROVIDERS LTD");
_LIT(protocolAddress, "i-MODE GATEWAY");
_LIT(username, "USER1");
_LIT(password, "PASSWORD1");
_LIT(challenge, "CHALLENGE1");
_LIT(response, "RESPONSE1");
_LIT(primaryDNS, "PRIMARYSERVER1");
_LIT(secondaryDNS, "SECONDARYSERVER1");

void DefaultContextConfigGPRS(RPacketContext::TContextConfigGPRS& aParam)
	{
	aParam.iPdpType = static_cast<RPacketContext::TProtocolType>(1);
	aParam.iAccessPointName.Copy(apn);
	aParam.iPdpAddress.Copy(protocolAddress);
	aParam.iPdpCompression = 1;
	aParam.iAnonymousAccessReqd = static_cast<RPacketContext::TAnonymousAccess>(1);
	aParam.iUseEdge = ETrue;
	aParam.iProtocolConfigOption.iAuthInfo.iProtocol = static_cast<RPacketContext::TAuthProtocol>(1);
	aParam.iProtocolConfigOption.iAuthInfo.iUsername.Copy(username);
	aParam.iProtocolConfigOption.iAuthInfo.iPassword.Copy(password);
	aParam.iProtocolConfigOption.iChallenge.Copy(challenge);
	aParam.iProtocolConfigOption.iResponse.Copy(response);
	aParam.iProtocolConfigOption.iDnsAddresses.iPrimaryDns.Copy(primaryDNS);
	aParam.iProtocolConfigOption.iDnsAddresses.iSecondaryDns.Copy(secondaryDNS);
	aParam.iProtocolConfigOption.iId = 1;
	}

void DefaultQoSR99_R4Requested(RPacketQoS::TQoSR99_R4Requested& aQosRequested)
	{
	aQosRequested.iReqTrafficClass = static_cast<RPacketQoS::TTrafficClass>(2);
	aQosRequested.iMinTrafficClass = static_cast<RPacketQoS::TTrafficClass>(2);
	aQosRequested.iReqDeliveryOrderReqd = static_cast<RPacketQoS::TDeliveryOrder>(2);
	aQosRequested.iMinDeliveryOrderReqd = static_cast<RPacketQoS::TDeliveryOrder>(2);
	aQosRequested.iReqDeliverErroneousSDU = static_cast<RPacketQoS::TErroneousSDUDelivery>(2);
	aQosRequested.iMinDeliverErroneousSDU = static_cast<RPacketQoS::TErroneousSDUDelivery>(2);
	aQosRequested.iReqMaxSDUSize = 2;
	aQosRequested.iMinAcceptableMaxSDUSize = 2;
	aQosRequested.iReqMaxRate.iUplinkRate = 2;
	aQosRequested.iReqMaxRate.iDownlinkRate = 2;
	aQosRequested.iMinGuaranteedRate.iUplinkRate = 2;
	aQosRequested.iMinGuaranteedRate.iDownlinkRate = 2;
	aQosRequested.iReqBER = static_cast<RPacketQoS::TBitErrorRatio>(2);
	aQosRequested.iMaxBER = static_cast<RPacketQoS::TBitErrorRatio>(2);
	aQosRequested.iReqSDUErrorRatio = static_cast<RPacketQoS::TSDUErrorRatio>(2);
	aQosRequested.iMaxSDUErrorRatio = static_cast<RPacketQoS::TSDUErrorRatio>(2);
	aQosRequested.iReqTrafficHandlingPriority = static_cast<RPacketQoS::TTrafficHandlingPriority>(2);
	aQosRequested.iMinTrafficHandlingPriority = static_cast<RPacketQoS::TTrafficHandlingPriority>(2);
	aQosRequested.iReqTransferDelay = 2;
	aQosRequested.iMaxTransferDelay = 2;
	aQosRequested.iReqGuaranteedRate.iUplinkRate = 2;
	aQosRequested.iReqGuaranteedRate.iDownlinkRate = 2;
	aQosRequested.iMinGuaranteedRate.iUplinkRate = 2;
	aQosRequested.iMinGuaranteedRate.iDownlinkRate = 2;
	}
	
void SecondQoSR99_R4Requested(RPacketQoS::TQoSR99_R4Requested& aQosRequested)
	{
	aQosRequested.iReqTrafficClass = static_cast<RPacketQoS::TTrafficClass>(1);
	aQosRequested.iMinTrafficClass = static_cast<RPacketQoS::TTrafficClass>(1);
	aQosRequested.iReqDeliveryOrderReqd = static_cast<RPacketQoS::TDeliveryOrder>(1);
	aQosRequested.iMinDeliveryOrderReqd = static_cast<RPacketQoS::TDeliveryOrder>(1);
	aQosRequested.iReqDeliverErroneousSDU = static_cast<RPacketQoS::TErroneousSDUDelivery>(1);
	aQosRequested.iMinDeliverErroneousSDU = static_cast<RPacketQoS::TErroneousSDUDelivery>(1);
	aQosRequested.iReqMaxSDUSize = 1;
	aQosRequested.iMinAcceptableMaxSDUSize = 1;
	aQosRequested.iReqMaxRate.iUplinkRate = 1;
	aQosRequested.iReqMaxRate.iDownlinkRate = 1;
	aQosRequested.iMinGuaranteedRate.iUplinkRate = 1;
	aQosRequested.iMinGuaranteedRate.iDownlinkRate = 1;
	aQosRequested.iReqBER = static_cast<RPacketQoS::TBitErrorRatio>(1);
	aQosRequested.iMaxBER = static_cast<RPacketQoS::TBitErrorRatio>(1);
	aQosRequested.iReqSDUErrorRatio = static_cast<RPacketQoS::TSDUErrorRatio>(1);
	aQosRequested.iMaxSDUErrorRatio = static_cast<RPacketQoS::TSDUErrorRatio>(1);
	aQosRequested.iReqTrafficHandlingPriority = static_cast<RPacketQoS::TTrafficHandlingPriority>(1);
	aQosRequested.iMinTrafficHandlingPriority = static_cast<RPacketQoS::TTrafficHandlingPriority>(1);
	aQosRequested.iReqTransferDelay = 1;
	aQosRequested.iMaxTransferDelay = 1;
	aQosRequested.iReqGuaranteedRate.iUplinkRate = 1;
	aQosRequested.iReqGuaranteedRate.iDownlinkRate = 1;
	aQosRequested.iMinGuaranteedRate.iUplinkRate = 1;
	aQosRequested.iMinGuaranteedRate.iDownlinkRate = 1;
	}

#ifdef SYMBIAN_NETWORKING_UMTSR5
void DefaultQoSR5Requested(RPacketQoS::TQoSR5Requested& aQosRequested)
    {
    DefaultQoSR99_R4Requested (aQosRequested);
    aQosRequested.iSignallingIndication = EFalse;
    aQosRequested.iSignallingIndication = RPacketQoS::ESourceStatisticsDescriptorUnknown;
    }
    
void SecondQoSR5Requested(RPacketQoS::TQoSR5Requested& aQosRequested)
    {
    SecondQoSR99_R4Requested (aQosRequested);
    aQosRequested.iSignallingIndication = EFalse;
    aQosRequested.iSignallingIndication = RPacketQoS::ESourceStatisticsDescriptorUnknown;
    }

#endif
	
void DefaultQoSR99_R4Negotiated(RPacketQoS::TQoSR99_R4Negotiated& aQosNegotiated)
	{
	aQosNegotiated.iTrafficClass = static_cast<RPacketQoS::TTrafficClass>(1);
	aQosNegotiated.iDeliveryOrderReqd = static_cast<RPacketQoS::TDeliveryOrder>(1);
	aQosNegotiated.iDeliverErroneousSDU = static_cast<RPacketQoS::TErroneousSDUDelivery>(1);
	aQosNegotiated.iMaxSDUSize = 1;
	aQosNegotiated.iBER = static_cast<RPacketQoS::TBitErrorRatio>(1);
	aQosNegotiated.iSDUErrorRatio = static_cast<RPacketQoS::TSDUErrorRatio>(1);
	aQosNegotiated.iTrafficHandlingPriority = static_cast<RPacketQoS::TTrafficHandlingPriority>(1);
	aQosNegotiated.iTransferDelay = 1;
	aQosNegotiated.iGuaranteedRate.iUplinkRate = 1;
	aQosNegotiated.iGuaranteedRate.iDownlinkRate = 1;
	aQosNegotiated.iMaxRate.iUplinkRate = 1;
	aQosNegotiated.iMaxRate.iDownlinkRate = 1;
	}
	
void SecondQoSR99_R4Negotiated(RPacketQoS::TQoSR99_R4Negotiated& aQosNegotiated)
	{
	aQosNegotiated.iTrafficClass = static_cast<RPacketQoS::TTrafficClass>(2);
	aQosNegotiated.iDeliveryOrderReqd = static_cast<RPacketQoS::TDeliveryOrder>(2);
	aQosNegotiated.iDeliverErroneousSDU = static_cast<RPacketQoS::TErroneousSDUDelivery>(2);
	aQosNegotiated.iMaxSDUSize = 2;
	aQosNegotiated.iBER = static_cast<RPacketQoS::TBitErrorRatio>(2);
	aQosNegotiated.iSDUErrorRatio = static_cast<RPacketQoS::TSDUErrorRatio>(2);
	aQosNegotiated.iTrafficHandlingPriority = static_cast<RPacketQoS::TTrafficHandlingPriority>(2);
	aQosNegotiated.iTransferDelay = 2;
	aQosNegotiated.iGuaranteedRate.iUplinkRate = 2;
	aQosNegotiated.iGuaranteedRate.iDownlinkRate = 2;
	aQosNegotiated.iMaxRate.iUplinkRate = 2;
	aQosNegotiated.iMaxRate.iDownlinkRate = 2;
	}





TUint8 defaultSrcAddr[RPacketContext::KIPAddressSize] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 1, 1, 1, 1};
TUint8 defaultSrcAddrSubnetMask[RPacketContext::KIPAddressSize] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255};

static void DefaultPacketFilter1(RPacketContext::TPacketFilterV2& entry)
	{
	entry.iId = 0;
	entry.iEvaluationPrecedenceIndex = 1;
	
	for (TUint i = 0; i < RPacketContext::KIPAddressSize; i++)
		{
		entry.iSrcAddr[i] = defaultSrcAddr[i];
		entry.iSrcAddrSubnetMask[i] = defaultSrcAddrSubnetMask[i];
		}
	entry.iProtocolNumberOrNextHeader = 1;
	entry.iSrcPortMin = 1;
	entry.iSrcPortMax = 1;
	entry.iDestPortMin = 1;
	entry.iDestPortMax = 1;
	entry.iIPSecSPI = 1;
	entry.iTOSorTrafficClass = 1;
	entry.iFlowLabel = 1;
	}
	
static void DefaultPacketFilter2(RPacketContext::TPacketFilterV2& entry)
	{
	entry.iId = 1;
	entry.iEvaluationPrecedenceIndex = 2;
	
	for (TUint i = 0; i < RPacketContext::KIPAddressSize; i++)
		{
		entry.iSrcAddr[i] = defaultSrcAddr[i];
		entry.iSrcAddrSubnetMask[i] = defaultSrcAddrSubnetMask[i];
		}
	entry.iProtocolNumberOrNextHeader = 2;
	entry.iSrcPortMin = 2;
	entry.iSrcPortMax = 2;
	entry.iDestPortMin = 2;
	entry.iDestPortMax = 2;
	entry.iIPSecSPI = 2;
	entry.iTOSorTrafficClass = 2;
	entry.iFlowLabel = 2;
	}

static void DefaultPacketFilter3(RPacketContext::TPacketFilterV2& entry)
	{
	entry.iId = 2;
	entry.iEvaluationPrecedenceIndex = 3;
	
	for (TUint i = 0; i < RPacketContext::KIPAddressSize; i++)
		{
		entry.iSrcAddr[i] = defaultSrcAddr[i];
		entry.iSrcAddrSubnetMask[i] = defaultSrcAddrSubnetMask[i];
		}
	entry.iProtocolNumberOrNextHeader = 3;
	entry.iSrcPortMin = 3;
	entry.iSrcPortMax = 3;
	entry.iDestPortMin = 3;
	entry.iDestPortMax = 3;
	entry.iIPSecSPI = 3;
	entry.iTOSorTrafficClass = 3;
	entry.iFlowLabel = 3;
	}

void DefaultTftInfo(TTFTInfo& aTTFTInfo)
	{
	RPacketContext::TPacketFilterV2 entry1, entry2;

	DefaultPacketFilter1(entry1);
	DefaultPacketFilter2(entry2);

	aTTFTInfo.AddPacketFilter(entry1);
	aTTFTInfo.AddPacketFilter(entry2);
	}
	
void SecondDefaultTftInfo(TTFTInfo& aTTFTInfo)
	{
	RPacketContext::TPacketFilterV2 entry;

	DefaultPacketFilter3(entry);

	aTTFTInfo.AddPacketFilter(entry);
	}
	
void CombinedDefaultTftInfo(TTFTInfo& aTTFTInfo)
	{
	RPacketContext::TPacketFilterV2 entry1, entry2, entry3;

	DefaultPacketFilter1(entry1);
	DefaultPacketFilter2(entry2);
	DefaultPacketFilter3(entry3);

	aTTFTInfo.AddPacketFilter(entry1);
	aTTFTInfo.AddPacketFilter(entry2);
	aTTFTInfo.AddPacketFilter(entry3);
	}

void DefaultContextStatus(RPacketContext::TContextStatus& aStatus)
	{
	aStatus = RPacketContext::EStatusInactive;
	}
	
void DefaultServiceStatus(RPacketService::TStatus& aStatus)
	{
	aStatus = RPacketService::EStatusAttached;
	}
	
_LIT(csy, "SIMTSYLOOPBACK.CSY");
_LIT(port, "SIMTSYLOOPBACK::0");
void DefaultDataChannel(RPacketContext::TDataChannelV2& aDataChannel)
	{
	aDataChannel.iCsy = csy;
	aDataChannel.iPort = port;		
	aDataChannel.iChannelId = port;
	}
