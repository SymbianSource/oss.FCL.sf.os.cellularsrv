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
// The implementation of the RPdpFsmInterface for SPUD TEL unit tests
// 
//

/**
 @file
 @internalComponent
*/

#include "PdpFsmInterfaceTest.h"
#include "InputRequestListener.h"
#include "DefaultContextParameters.h"
#include <etelpckt.h>


CPdpFsmInterfaceTest::CPdpFsmInterfaceTest(CInputRequestListener& aListener) : iListener(aListener), iTftOperationCode(KAddFilters)
	{
	DefaultTftInfo(iTftInfo);
	}

const TName& CPdpFsmInterfaceTest::TsyName()
	{
	return SimTsyName();
	}

/**
 Signals to the test execute thread that RPdpFsmInterface::Input has been called
 */
TInt CPdpFsmInterfaceTest::Input(TContextId aPdpId, const TInt aOperation, const TInt aParam)
	{
	// The aOperation parameters are broken up into those that are notifications and those that aren't
	// Since notifications can happen anytime, separate request status members need to be kept for each one
	// Other input requests happen serially, so only one request can be pending at any time
	TRequestStatus *status = NULL;
	switch (aOperation)
		{
	case PdpFsm::EQoSProfileChangeNetwork:		
		status = &iListener.iEtelDriverQosNotificationStatus;
		break;
	case PdpFsm::EConfigGPRSChangeNetwork:
		status = &iListener.iEtelDriverConfigGPRSNotificationStatus;
		break;
	case PdpFsm::EContextStatusChangeNetwork:
		status = &iListener.iEtelDriverContextStatusNotificationStatus;
		break;
	case PdpFsm::EServiceStatusChangeNetwork:
		status = &iListener.iEtelDriverServiceNotificationStatus;
		break;
	default:
		iListener.iEtelDriverInputResponseData.iPdpId = aPdpId;
		iListener.iEtelDriverInputResponseData.iSignal = aOperation;
		status = &iListener.iEtelDriverInputResponseStatus;
		break;
		}
	iListener.iParentThread.RequestComplete(status, aParam);
	
	return KErrNone;
	}



#ifdef SYMBIAN_NETWORKING_UMTSR5
// Gets just return default values
TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, RPacketQoS::TQoSR5Negotiated& /*aParam*/) const
	{
	ASSERT(EFalse);
	return KErrNone;
	}

TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, RPacketQoS::TQoSR5Requested& aQoSR5Requested) const
	{
	DefaultQoSR99_R4Requested(aQoSR5Requested);
	aQoSR5Requested.iSignallingIndication = EFalse;
	aQoSR5Requested.iSourceStatisticsDescriptor = RPacketQoS::ESourceStatisticsDescriptorUnknown;
	return KErrNone;
	}
	
#else
// !SYMBIAN_NETWORKING_UMTSR5

// Gets just return default values
TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, RPacketQoS::TQoSR99_R4Negotiated& /*aParam*/) const
	{
	ASSERT(EFalse);
	return KErrNone;
	}

TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, RPacketQoS::TQoSR99_R4Requested& aQoSR99_R4Requested) const
	{
	DefaultQoSR99_R4Requested(aQoSR99_R4Requested);
	return KErrNone;
	}


#endif 
// SYMBIAN_NETWORKING_UMTSR5 



TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, RPacketContext::TContextConfigGPRS& aParam) const
	{
	DefaultContextConfigGPRS(aParam);
	return KErrNone;
	}
	
TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, TTFTOperationCode& aParam) const
	{
	aParam = iTftOperationCode;
	return KErrNone;
	}

TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, TTFTInfo& aTFTInfo) const
	{
	aTFTInfo = iTftInfo;
	return KErrNone;
	}

TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, RPacketContext::TDataChannelV2& aDataChannelV2) const
	{
	DefaultDataChannel(aDataChannelV2);
	return KErrNone;
	}
	
TInt CPdpFsmInterfaceTest::Get(TContextId /*aPdpId*/, RPacketContext::TContextStatus& aParam) const
	{
	aParam = RPacketContext::EStatusUnknown;
	return KErrNone;
	}

void CPdpFsmInterfaceTest::Get(RPacketService::TStatus& aParam)
	{
	aParam = RPacketService::EStatusActive;
	}

TInt CPdpFsmInterfaceTest::GetLastErrorCause(TContextId /*aPdpId*/, TInt& /*aLastErrorCause*/) const
	{
	/*Placeholder for unused function*/
	return KErrNone;
	}

/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(TContextId aPdpId, const RPacketContext::TContextConfigGPRS& aContextConfigGPRS)
	{
	iListener.iNotifyContextConfigData.iPdpId = aPdpId;
	iListener.iNotifyContextConfigData.iContextConfigGPRS = aContextConfigGPRS;
	return KErrNone;
	}

/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(TContextId aPdpId, const RPacketContext::TContextStatus& aParam)
	{
	iListener.iNotifyContextStatusData.iPdpId = aPdpId;
	iListener.iNotifyContextStatusData.iContextStatus = aParam;
	return KErrNone;
	}

/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
void CPdpFsmInterfaceTest::Set(const RPacketService::TStatus aServiceStatus)
	{
	iListener.iPacketServiceStatus = aServiceStatus;
	}

/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(TContextId aPdpId, const RPacketContext::TDataChannelV2& aDataChannelV2)
	{
	iListener.iNotifyContextDataChannelData.iPdpId = aPdpId;
	iListener.iNotifyContextDataChannelData.iDataChannelV2 = aDataChannelV2;
	return KErrNone;
	}


#ifdef SYMBIAN_NETWORKING_UMTSR5
/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(TContextId /*aPdpId*/, const RPacketQoS::TQoSR5Requested& /*aParam*/)
	{
	ASSERT(EFalse);
	return KErrNone;
	}

/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(const TContextId aPdpId, const RPacketQoS::TQoSR5Negotiated& aQoSR5Negotiated)
	{
	iListener.iNotifyQosNegotiatedData.iPdpId = aPdpId;
	iListener.iNotifyQosNegotiatedData.iNegotiated = aQoSR5Negotiated;
	return KErrNone;
	}

#else
// !SYMBIAN_NETWORKING_UMTSR5 

/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(TContextId /*aPdpId*/, const RPacketQoS::TQoSR99_R4Requested& /*aParam*/)
	{
	ASSERT(EFalse);
	return KErrNone;
	}

/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Negotiated& aQoSR99_R4Negotiated)
	{
	iListener.iNotifyQosNegotiatedData.iPdpId = aPdpId;
	iListener.iNotifyQosNegotiatedData.iNegotiated = aQoSR99_R4Negotiated;
	return KErrNone;
	}

#endif 
// SYMBIAN_NETWORKING_UMTSR5 




/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(TContextId /*aPdpId*/, const TTFTOperationCode& aParam)
	{
	iTftOperationCode = aParam;
	return KErrNone;
	}

/**
 Store the parameters passed to Set to be compared by the test execute thread
 */
TInt CPdpFsmInterfaceTest::Set(TContextId /*aPdpId*/, const TTFTInfo& aParam)
	{
	iTftInfo = aParam;
	return KErrNone;
	}
	
void CPdpFsmInterfaceTest::SetContextTerminationErrorAndStop(TContextId /*aPdpId*/, TInt /*aErrorCode*/)
	{/*Placeholder for unused function*/}


