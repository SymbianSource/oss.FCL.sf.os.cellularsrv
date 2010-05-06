// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef MOCKLTSYINDICATORIDS_H_
#define MOCKLTSYINDICATORIDS_H_

#include <ctsy/ltsy/mltsydispatchinterface.h>
#include <e32def.h>

// API IDs used by the Mock LTSY

const TInt KDispatchCallControlFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchCallControlNotifyCallStatusChangeIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchCallControlReceiveUuiIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 2;
const TInt KMockLtsyDispatchCallControlNotifyDataCallCapsChangeIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 3;
const TInt KMockLtsyDispatchCallControlNotifyIncomingCallIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 4;
const TInt KMockLtsyDispatchCallControlNotifyHscsdInfoChangeIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 5;
const TInt KMockLtsyDispatchCallControlNotifyCallEventIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 6;
const TInt KMockLtsyDispatchCallControlNotifyCallInfoChangeIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 7;
const TInt KMockLtsyDispatchCallControlNotifyIccCallForwardingStatusChangeIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 8;
const TInt KMockLtsyDispatchCallControlRemoteAlertingToneChangeIndId = KDispatchCallControlFuncUnitId + KDispatchCallControlFuncUnitIndIdOffset + 9;

const TInt KDispatchCallControlMultipartyFuncUnitIndIdOffset = 1000;

const TInt KDispatchCellBroadcastFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchCellBroadcastGsmBroadcastNotifyMessageReceivedIndId = KDispatchCellBroadcastFuncUnitId + KDispatchCellBroadcastFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchCellBroadcastWcdmaBroadcastMessageReceivedIndId = KDispatchCellBroadcastFuncUnitId + KDispatchCellBroadcastFuncUnitIndIdOffset + 2;

const TInt KDispatchPacketServicesFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchPacketServicesNotifyPacketAttachModeChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchPacketServicesNotifyQosProfileChangedIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 2;
const TInt KMockLtsyDispatchPacketServicesNotifyRatTransferCapsChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 3;
const TInt KMockLtsyDispatchPacketServicesNotifyPacketMsClassChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 4;
const TInt KMockLtsyDispatchPacketServicesNotifyPacketDynamicCapsChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 5;
const TInt KMockLtsyDispatchPacketServicesNotifyPdpContextAddedIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 6;
const TInt KMockLtsyDispatchPacketServicesNotifyNetworkInitiatedContextActivationRequestIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 7;
const TInt KMockLtsyDispatchPacketServicesNotifyPdpContextConfigChangedIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 8;
const TInt KMockLtsyDispatchPacketServicesNotifyPdpContextStatusChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 9;
const TInt KMockLtsyDispatchPacketServicesNotifyPacketNetworkRegistrationStatusIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 10;
const TInt KMockLtsyDispatchPacketServicesNotifyPdpContextConnectionSpeedChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 11;
const TInt KMockLtsyDispatchPacketServicesNotifyMbmsContextConfigChangedIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 12;
const TInt KMockLtsyDispatchPacketServicesNotifyMbmsNetworkServiceStatusChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 13;
const TInt KMockLtsyDispatchPacketServicesNotifyMbmsServiceAvailabilityChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 14;
const TInt KMockLtsyDispatchPacketServicesNotifyConnectionInfoChangeIndId = KDispatchPacketServicesFuncUnitId + KDispatchPacketServicesFuncUnitIndIdOffset + 15;

const TInt KDispatchPhonebookEnFuncUnitIndIdOffset = 1000;

const TInt KDispatchPhonebookFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchPhonebookStoreResetCacheIndId = KDispatchPhonebookFuncUnitId + KDispatchPhonebookFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchPhonebookStoreSetFdnPhonebookInfoIndId = KDispatchPhonebookFuncUnitId + KDispatchPhonebookFuncUnitIndIdOffset + 2;

const TInt KDispatchPhoneFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchPhoneNotifyNetworkModeChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchPhoneNotifyEgprsInfoChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 2;
const TInt KMockLtsyDispatchPhoneNotifySignalStrengthChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 3;
const TInt KMockLtsyDispatchPhoneNotifyNetworkRegistrationStatusChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 5;
const TInt KMockLtsyDispatchPhoneNotifyNitzInfoChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 6;
const TInt KMockLtsyDispatchPhoneNotifyBatteryInfoChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 7;
const TInt KMockLtsyDispatchPhoneNotifyCurrentNetworkChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 8;
const TInt KMockLtsyDispatchPhoneNotifyRfStatusChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 9;
const TInt KMockLtsyDispatchPhoneNotifyNspsStatusChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 10;
const TInt KMockLtsyDispatchPhoneNotifyNetworkSelectionSettingChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 11;
const TInt KMockLtsyDispatchPhoneNotifyRauEventIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 12;
const TInt KMockLtsyDispatchPhoneNotifyNetworkConnectionFailureIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 13;
const TInt KMockLtsyDispatchCustomPhoneNotifyDtmfEventIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 14;
const TInt KMockLtsyDispatchPhoneNotifyDtmfEventIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 15;
const TInt KMockLtsyDispatchPhoneNotifyHspaStatusChangedIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 16;
const TInt KMockLtsyDispatchPhoneNotifyCellInfoChangedIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 17;
const TInt KMockLtsyDispatchPhoneNotifyCellInfoChangeIndId = KDispatchPhoneFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 18;

const TInt KDispatchSatFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchSatUssdControlSupportedChangeIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchSatPcmdIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 2;
const TInt KMockLtsyDispatchSatNotifyProactiveSimSessionEndIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 3;
const TInt KMockLtsyDispatchSatNotifyLocalInfoIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 4;
const TInt KMockLtsyDispatchSatTimeZoneChangeIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 5;
const TInt KMockLtsyDispatchSatAccessTechnologyChangeIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 6;
const TInt KMockLtsyDispatchSatCallConnectedIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 7;
const TInt KMockLtsyDispatchSatSsChangeIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 8;
const TInt KMockLtsyDispatchSatCallDisconnectedIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 9;
const TInt KMockLtsyDispatchSatMtCallIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 10;
const TInt KMockLtsyDispatchSatCallControlEnvelopeResponseIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 11;
const TInt KMockLtsyDispatchSatCreateCallControlEnvelopeIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 12;
const TInt KMockLtsyDispatchSatCreateMoSmControlEnvelopeIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 13;
const TInt KMockLtsyDispatchSatImsiChangedIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 14;
const TInt KMockLtsyDispatchSatLocationStatusIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 15;
const TInt KMockLtsyDispatchSatRemoveEventListIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 16;
const TInt KMockLtsyDispatchSatCreateRefreshTerminalRspIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 17;
const TInt KMockLtsyDispatchSatMoSmControlEnvelopeResponseIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 18;
const TInt KMockLtsyDispatchSatCreateSmsPpDownloadEnvelopeIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 19;
const TInt KMockLtsyDispatchSatCreateSmsDeliveryReportIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 20;
const TInt KMockLtsyDispatchSatCreateCellBroadcastEnvelopeIndId = KDispatchSatFuncUnitId + KDispatchSatFuncUnitIndIdOffset + 21;


const TInt KDispatchSecurityFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchSecurityBootNotifySecurityReadyIndId = KDispatchSecurityFuncUnitId + KDispatchSecurityFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchSecurityNotifySecurityEventIndId = KDispatchSecurityFuncUnitId + KDispatchSecurityFuncUnitIndIdOffset + 2;
const TInt KMockLtsyDispatchSecurityNotifyLockInfoChangeIndId = KDispatchSecurityFuncUnitId + KDispatchSecurityFuncUnitIndIdOffset + 3;
const TInt KLtsyDispatchSecurityNotifyCipheringInfoChangeApiId = KDispatchSecurityFuncUnitId + KDispatchSecurityFuncUnitIndIdOffset + 4;

const TInt KDispatchSimFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchSimRefreshSimFilesIndId = KDispatchSimFuncUnitId + KDispatchSimFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchSimNotifyIccMessageWaitingIndicatorsChangeIndId = KDispatchSimFuncUnitId + KDispatchSimFuncUnitIndIdOffset + 2;
const TInt KMockLtsyDispatchSimNotifyApnListChangeIndId = KDispatchSimFuncUnitId + KDispatchSimFuncUnitIndIdOffset + 3;
const TInt KMockLtsyDispatchSimNotifyApnControlListServiceStatusChangeIndId = KDispatchSimFuncUnitId + KDispatchSimFuncUnitIndIdOffset + 4;
const TInt KLtsyDispatchSimNotifySimCardStatusApiId = KDispatchSimFuncUnitId + KDispatchSimFuncUnitIndIdOffset + 5;

const TInt KDispatchSmsFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchSmsNotifyReceiveSmsMessageIndId = KDispatchSmsFuncUnitId + KDispatchSmsFuncUnitIndIdOffset + 1;

const TInt KDispatchSupplementaryServicesFuncUnitIndIdOffset = 1000;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyCallBarringStatusChangeIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 1;
const TInt KMockLtsyDispatchSupplementaryServicesReceiveUssdMessageIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 2;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyCallWaitingStatusChangeIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 3;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyCallForwardingStatusChangeIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 4;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyCallForwardingActiveIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 5;
const TInt KMockLtsyDispatchSupplementaryServicesGetDiagnosticOctetsIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 6;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyAdditionalInfoIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 7;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyRequestCompleteIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 8;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyNetworkEventForwardModeIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 9;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyNetworkEventCallWaitingIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 10;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyNetworkEventHoldModeIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 11;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyNetworkEventConfrenceIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 12;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyNetworkEventCugIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 13;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyNetworkEventClirSuppressionIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 14;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyNetworkEventEctCallStateIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 15;
const TInt KMockLtsyDispatchSupplementaryServicesNotifyNetworkEventIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchSupplementaryServicesFuncUnitIndIdOffset + 16;
const TInt KMockLtsyDispatchSupplementaryServicesNotifySendNetworkServiceRequestIndId = KDispatchSupplementaryServicesFuncUnitId + KDispatchPhoneFuncUnitIndIdOffset + 17;

#endif /*MOCKLTSYINDICATORIDS_H_*/
