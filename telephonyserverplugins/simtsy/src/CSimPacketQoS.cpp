// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CSimPacketQoS.h"
#include "CSimPhone.h"
#include <pcktcs.h>
#include "CSimPacketContext.h"
#include "Simlog.h"
#include "utils.h"
#include "CSimPubSubChange.h"

static const int KNumberofProfiles = 4;	//< Number chosen at random.  

CSimPacketQoS* CSimPacketQoS::NewL(CSimPhone* aPhone, CSimPacketContext* aPacketContext)
/**
* Standard two phase constructor
*
* @param aPacketContext Pointer to the Packet Context object (CSimPacketContext)
* @return CSimPacketQoS pointer to the packet QoS object created.
* @leave Leaves if no memory or object is not created for any reason.
*/
	{
	CSimPacketQoS* qos=new(ELeave) CSimPacketQoS(aPhone, aPacketContext);
	CleanupStack::PushL(qos);
	qos->ConstructL();
	CleanupStack::Pop();
	return qos;
	}

void CSimPacketQoS::Init()
	{}

/**
* Callback function for when timer expires. Note - this only gets called for Rel 99 profiles
*/
void CSimPacketQoS::TimerCallBack(TInt aId)
	{
	switch(aId)
		{
		case ETimerIdPcktQosNetWork:
			{
			TProfileNegR99 profile = iQosNetworkNotifications->At(iQosNetworkNotificationCount);
			CompleteNotifications(profile);
			break;
			}
		case ETimerIdPcktQosR5Network:
			{
			TProfileNegR5 profile = iQosR5NetworkNotifications->At(iQosR5NetworkNotificationCount);
			CompleteNotifications(profile);
			break;
			}
		case ETimerIdPcktQoSSet:
 			{
 			SetProfile(iSetQoSData->At(0).iTsyReqHandle, iSetQoSData->At(0).iData);
 			iSetQoSData->Delete(0);
 			if (iSetQoSData->Count() != 0)
 				iSetQoSTimer->Start(iSetQoSDelay,this,ETimerIdPcktQoSSet);
 			break;	
 			}
		}
	}

void CSimPacketQoS::PubSubCallback(TInt aProfileIndex)
/**
 * Called by CSimQoSChange when a network QoS change request has been received.
 *
 * @param aProfileIndex index of the QoS profile to set
 */
	{
	if (iCurrentProfileRel == TPacketDataConfigBase::KConfigRel5)
		{
		TInt length = iQosR5NetworkNotifications->Count();
		if (aProfileIndex < length)
			{
			TProfileNegR5& profile = iQosR5NetworkNotifications->At(aProfileIndex);
			if (profile.iContextName.Compare(iPacketContext->ContextName()) == 0)
				{
				CompleteNotifications(profile);
				}
			}
		}
	else // by default, uses R99/R4 notification setup
		{
		TInt length = iQosNetworkNotifications->Count();
		if (aProfileIndex < length)
			{
			TProfileNegR99& profile = iQosNetworkNotifications->At(aProfileIndex);
			if (profile.iContextName.Compare(iPacketContext->ContextName()) == 0)
				{
				CompleteNotifications(profile);
				}
			}
		}
	}

void CSimPacketQoS::CompleteNotifications(TProfileNegR99& aProfile)
	{
	if (MatchContext(aProfile)==KErrNone)
		{
		if (iNotifyProfileR99.iNotifyPending)
			{
			RPacketQoS::TQoSR99_R4Negotiated& qos = *(RPacketQoS::TQoSR99_R4Negotiated*)iNotifyProfileR99.iNotifyData;
			qos.iBER = STATIC_CAST(RPacketQoS::TBitErrorRatio, aProfile.iNegTBitErrorRatio);
			qos.iDeliverErroneousSDU = STATIC_CAST(RPacketQoS::TErroneousSDUDelivery, aProfile.iNegErroneousSDUDelivery);
			qos.iDeliveryOrderReqd = STATIC_CAST(RPacketQoS::TDeliveryOrder, aProfile.iNegDeliveryOrder);
	
			qos.iGuaranteedRate.iUplinkRate = aProfile.iNegGuaranteedUpLinkBitRate;
			qos.iGuaranteedRate.iDownlinkRate = aProfile.iNegGuaranteedDownLinkBitRate;
			
			qos.iMaxRate.iUplinkRate = aProfile.iNegMaxUpLinkRate;
			qos.iMaxRate.iDownlinkRate = aProfile.iNegMaxDownLinkRate;
			qos.iMaxSDUSize = aProfile.iNegMaxSDUSize;

			qos.iSDUErrorRatio = STATIC_CAST(RPacketQoS::TSDUErrorRatio, aProfile.iNegTSDUErrorRatio);
			qos.iTrafficClass = STATIC_CAST(RPacketQoS::TTrafficClass,aProfile.iNegTraffic);
			qos.iTrafficHandlingPriority = STATIC_CAST(RPacketQoS::TTrafficHandlingPriority, aProfile.iNegTrafficHandlingPriority);
			qos.iTransferDelay = aProfile.iNegTransferDelay;
			
			iNotifyProfileR99.iNotifyPending=EFalse;
			ReqCompleted(iNotifyProfileR99.iNotifyHandle, KErrNone);
			}
		if (iNotifyProfileR5.iNotifyPending)
			{
			RPacketQoS::TQoSR5Negotiated& qos = *(RPacketQoS::TQoSR5Negotiated*)iNotifyProfileR5.iNotifyData;
			qos.iBER = STATIC_CAST(RPacketQoS::TBitErrorRatio, aProfile.iNegTBitErrorRatio);
			qos.iDeliverErroneousSDU = STATIC_CAST(RPacketQoS::TErroneousSDUDelivery, aProfile.iNegErroneousSDUDelivery);
			qos.iDeliveryOrderReqd = STATIC_CAST(RPacketQoS::TDeliveryOrder, aProfile.iNegDeliveryOrder);
	
			qos.iGuaranteedRate.iUplinkRate = aProfile.iNegGuaranteedUpLinkBitRate;
			qos.iGuaranteedRate.iDownlinkRate = aProfile.iNegGuaranteedDownLinkBitRate;
			
			qos.iMaxRate.iUplinkRate = aProfile.iNegMaxUpLinkRate;
			qos.iMaxRate.iDownlinkRate = aProfile.iNegMaxDownLinkRate;
			qos.iMaxSDUSize = aProfile.iNegMaxSDUSize;

			qos.iSDUErrorRatio = STATIC_CAST(RPacketQoS::TSDUErrorRatio, aProfile.iNegTSDUErrorRatio);
			qos.iTrafficClass = STATIC_CAST(RPacketQoS::TTrafficClass,aProfile.iNegTraffic);
			qos.iTrafficHandlingPriority = STATIC_CAST(RPacketQoS::TTrafficHandlingPriority, aProfile.iNegTrafficHandlingPriority);
			qos.iTransferDelay = aProfile.iNegTransferDelay;
			qos.iSignallingIndication = EFalse;
			qos.iSourceStatisticsDescriptor = RPacketQoS::ESourceStatisticsDescriptorUnknown;
			
			iNotifyProfileR5.iNotifyPending=EFalse;
			ReqCompleted(iNotifyProfileR5.iNotifyHandle, KErrNone);
			}
		}
	}

void CSimPacketQoS::CompleteNotifications(TProfileNegR5& aProfile)
	{
	if (MatchContext(aProfile)==KErrNone)
		{
		if (iNotifyProfileR99.iNotifyPending)
			{
			RPacketQoS::TQoSR99_R4Negotiated& qos = *(RPacketQoS::TQoSR99_R4Negotiated*)iNotifyProfileR99.iNotifyData;
			qos.iBER = STATIC_CAST(RPacketQoS::TBitErrorRatio, aProfile.iNegTBitErrorRatio);
			qos.iDeliverErroneousSDU = STATIC_CAST(RPacketQoS::TErroneousSDUDelivery, aProfile.iNegErroneousSDUDelivery);
			qos.iDeliveryOrderReqd = STATIC_CAST(RPacketQoS::TDeliveryOrder, aProfile.iNegDeliveryOrder);
	
			qos.iGuaranteedRate.iUplinkRate = aProfile.iNegGuaranteedUpLinkBitRate;
			qos.iGuaranteedRate.iDownlinkRate = aProfile.iNegGuaranteedDownLinkBitRate;
			
			qos.iMaxRate.iUplinkRate = aProfile.iNegMaxUpLinkRate;
			qos.iMaxRate.iDownlinkRate = aProfile.iNegMaxDownLinkRate;
			qos.iMaxSDUSize = aProfile.iNegMaxSDUSize;

			qos.iSDUErrorRatio = STATIC_CAST(RPacketQoS::TSDUErrorRatio, aProfile.iNegTSDUErrorRatio);
			qos.iTrafficClass = STATIC_CAST(RPacketQoS::TTrafficClass,aProfile.iNegTraffic);
			qos.iTrafficHandlingPriority = STATIC_CAST(RPacketQoS::TTrafficHandlingPriority, aProfile.iNegTrafficHandlingPriority);
			qos.iTransferDelay = aProfile.iNegTransferDelay;
			
			iNotifyProfileR99.iNotifyPending=EFalse;
			ReqCompleted(iNotifyProfileR99.iNotifyHandle, KErrNone);
			}
		if (iNotifyProfileR5.iNotifyPending)
			{
			RPacketQoS::TQoSR5Negotiated& qos = *(RPacketQoS::TQoSR5Negotiated*)iNotifyProfileR5.iNotifyData;
			qos.iBER = STATIC_CAST(RPacketQoS::TBitErrorRatio, aProfile.iNegTBitErrorRatio);
			qos.iDeliverErroneousSDU = STATIC_CAST(RPacketQoS::TErroneousSDUDelivery, aProfile.iNegErroneousSDUDelivery);
			qos.iDeliveryOrderReqd = STATIC_CAST(RPacketQoS::TDeliveryOrder, aProfile.iNegDeliveryOrder);
	
			qos.iGuaranteedRate.iUplinkRate = aProfile.iNegGuaranteedUpLinkBitRate;
			qos.iGuaranteedRate.iDownlinkRate = aProfile.iNegGuaranteedDownLinkBitRate;
			
			qos.iMaxRate.iUplinkRate = aProfile.iNegMaxUpLinkRate;
			qos.iMaxRate.iDownlinkRate = aProfile.iNegMaxDownLinkRate;
			qos.iMaxSDUSize = aProfile.iNegMaxSDUSize;

			qos.iSDUErrorRatio = STATIC_CAST(RPacketQoS::TSDUErrorRatio, aProfile.iNegTSDUErrorRatio);
			qos.iTrafficClass = STATIC_CAST(RPacketQoS::TTrafficClass,aProfile.iNegTraffic);
			qos.iTrafficHandlingPriority = STATIC_CAST(RPacketQoS::TTrafficHandlingPriority, aProfile.iNegTrafficHandlingPriority);
			qos.iTransferDelay = aProfile.iNegTransferDelay;
			qos.iSignallingIndication = aProfile.iSignallingIndication;			
			qos.iSourceStatisticsDescriptor = STATIC_CAST(RPacketQoS::TSourceStatisticsDescriptor, aProfile.iNegTSourceStatisticsDescriptor);

			iNotifyProfileR5.iNotifyPending=EFalse;

			ReqCompleted(iNotifyProfileR5.iNotifyHandle, KErrNone);
			}
		}

	}

CSimPacketQoS::CSimPacketQoS(CSimPhone* aPhone, CSimPacketContext* aPacketContext)
	:iPhone(aPhone), iPacketContext(aPacketContext),iCurrentNeg(0), iCurrentNeg99(0), iCurrentNegR5(0),
	iSetProfilePending(EFalse),	iCurrentProfileRel(TPacketDataConfigBase::KConfigGPRS), iSetProfileCallCount(0), 
	iUseNetworkNotificationBool(EFalse), iTimerStartedBool(EFalse), iSetQoSData(NULL)
	
/**
* Trivial Constructor.  Initialises all the data members
*
* @param aPacketContext Pointer to the Packet QoS object (CSimPacketContext)
*/
	{
	iNotifyProfileGPRS.iNotifyPending = EFalse;
	iNotifyProfileR99.iNotifyPending = EFalse;
	iNotifyProfileR5.iNotifyPending = EFalse;
	}

void CSimPacketQoS::ConstructL()
/**
* Second phase of the 2-phase constructor.
* Constructs all the member data and retrieves all the data from the config file specific to this class.
*
* @leave Leaves no memory or any data member does not construct for any reason.
*/
	{
	LOGPACKET1("CSimPacketQoS: Entered constructor");
	iQosNetworkNotificationCount = 0;
	iQosR5NetworkNotificationCount = 0;

	iTimer=CSimTimer::NewL(iPhone);
	iSetQoSData = new (ELeave) CArrayFixFlat<TSetQoSData>(1);
 	iSetQoSTimer = CSimTimer::NewL(iPhone);
	iSimQoSChange = CSimPubSubChange::NewL(this, CSimPubSub::TPubSubProperty(KUidPSSimTsyCategory, KPSSimTsyNetworkQoSChange, KPSSimTsyNetworkQoSChangeKeyType));
	
	LOGPACKET1("Starting to Load and Parse Packet Qos Config File");

	GetGPRSReqProfilesL();
	GetGPRSNegProfilesL();
	GetR99ReqProfilesL();
	GetR99NegProfilesL();
	GetR99NetworkNotificationsL();
	GetR5ReqProfilesL();
	GetR5NegProfilesL();
	GetR5NetworkNotificationsL();
	GetGPRSQosProfileCaps();
	GetR99QosProfileCaps();
	GetR5QosProfileCaps();
	GetProfileFailSettings();
	GetSetQoSSettings();

	LOGPACKET1("...Finished parsing Packet qos config parameters...");
	}

void CSimPacketQoS::GetGPRSReqProfilesL()
	{
	TProfileConfig profile;
	TUint8 digit = 0;

	iProfiles = new(ELeave) CArrayFixFlat<TProfileConfig>(KNumberofProfiles);

	TInt count = iPacketContext->CfgFile()->ItemCount(KQosProfileReqGPRS);
	
	//< Read in all the data for setting GPRS Requested Profile parameters
	for(TInt i = 0; i < count; i++)
		{
		const CTestConfigItem *item = iPacketContext->CfgFile()->Item(KQosProfileReqGPRS,i);
		if(!item)
			break;

		TPtrC8 minPrecedence, reqPrecedence, minDelay, reqDelay, minPeek, reqPeek;
		TPtrC8 minReliability, reqReliability, minMean, reqMean;

		TInt ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,minPrecedence);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::minPrecedence",ret,0,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(minPrecedence, digit)==KErrNone)
				profile.iMinPrecedence = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,reqPrecedence);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::reqPrecedence",ret,1,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(reqPrecedence, digit)==KErrNone)
				profile.iReqPrecedence = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,minDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::minDelay",ret,2,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(minDelay, digit)==KErrNone)
				profile.iMinDelay = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,reqDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::reqDelay",ret,3,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(reqDelay, digit)==KErrNone)
				profile.iReqDelay = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,minReliability);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::minReliability",ret,4,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(minReliability, digit)==KErrNone)
				profile.iMinReliability = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,reqReliability);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::reqReliability",ret,5,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(reqReliability, digit)==KErrNone)
				profile.iReqReliability = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,6,minPeek);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::minPeek",ret,6,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(minPeek, digit)==KErrNone)
				profile.iMinPeekThroughput = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,7,reqPeek);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::reqPeek",ret,7,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(reqPeek, digit)==KErrNone)
				profile.iReqPeekThroughput = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,8,minMean);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::minMean",ret,8,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(minMean, digit)==KErrNone)
				profile.iMinMeanThroughput = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,9,reqMean);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::reqMean",ret,9,&KQosProfileReqGPRS);
			continue;
			}
		else
			{
			if(AsciiToNum(reqMean, digit)==KErrNone)
				profile.iReqMeanThroughput = digit;
			}

		iProfiles->AppendL(profile);
		}
	}
	
void CSimPacketQoS::GetGPRSNegProfilesL()
	{
	TNegProfileConfig negProfile;
	TUint8 digit = 0;

	iNegProfiles = new(ELeave) CArrayFixFlat<TNegProfileConfig>(KNumberofProfiles);


	TInt count = iPacketContext->CfgFile()->ItemCount(KQosProfileNegGPRS);
	//< Read in all the data for setting GPRS Negotiated Profile parameters
	for(TInt i = 0; i < count; i++)
		{
		const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosProfileNegGPRS,i);
		if(!item)
			break;

		TPtrC8 negPrecedence, negDelay, negPeek, negReliability, negMean;

		TInt ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,negPrecedence);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::negPrecedence",ret,0,&KQosProfileNegGPRS);
			continue;
			}
		else
			if(AsciiToNum(negPrecedence, digit)==KErrNone)
				negProfile.iNegPrecedence = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,negDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::negDelay",ret,1,&KQosProfileNegGPRS);
			continue;
			}
		else
			if(AsciiToNum(negDelay, digit)==KErrNone)
				negProfile.iNegDelay = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,negReliability);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::negReliability",ret,2,&KQosProfileNegGPRS);
			continue;
			}
		else
			if(AsciiToNum(negReliability, digit)==KErrNone)
				negProfile.iNegReliability = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,negPeek);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::negPeek",ret,3,&KQosProfileNegGPRS);
			continue;
			}
		else
			if(AsciiToNum(negPeek, digit)==KErrNone)
				negProfile.iNegPeekThroughput = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,negMean);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqGPRS::negMean",ret,4,&KQosProfileNegGPRS);
			continue;
			}
		else
			if(AsciiToNum(negMean, digit)==KErrNone)
				negProfile.iNegMeanThroughput = digit;

		iNegProfiles->AppendL(negProfile);
		}
	}
	
	
void CSimPacketQoS::GetR99ReqProfilesL()
	{
	TProfileReqR99 profileReq99;
	TUint8 digit = 0;

	iReqR99Profiles = new(ELeave) CArrayFixFlat<TProfileReqR99>(KNumberofProfiles);

	// parse Release Rel99 profile parameters
	TInt count = iPacketContext->CfgFile()->ItemCount(KQosProfileReqR99);
	
	for(TInt i = 0; i < count; i++)
		{
		const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosProfileReqR99,i);
		if(!item)
			break;

		// continue to load item's parameters
		TPtrC8 contextName, reqTraffic, minTraffic, reqDeliveryOrder, minDeliveryOrder;
		TPtrC8 reqErroneousSDU, minErroneousSDU;
		TInt minUpLinkBitRate, minDownLinkBitRate, reqUpLinkBitRate, reqDownLinkBitRate;
		TInt maxSDUSize, minSDUSize;
		TPtrC8 reqBitErrorRatio, minBitErrorRatio, reqSDUErrorRatio, minSDUErrorRatio;
		TPtrC8 reqTrafficHandlingPriority, minTrafficHandlingPriority;
		TInt reqTransferDelay, minTransferDelay, reqGuaranteedUpLinkBitRate, reqGuaranteedDownLinkBitRate;
		TInt minGuaranteedUpLinkBitRate, minGuaranteedDownLinkBitRate;

		TInt ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,0,contextName);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::contextName",ret,0,KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iContextName.Copy(contextName);
			if (profileReq99.iContextName.Match(iPacketContext->ContextName())==KErrNotFound)
				{
				LOGPARSERR("QosProfileReqR99::contextName",KErrArgument,0,&KQosProfileReqR99);
				continue;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,reqTraffic);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqTraffic",ret,1,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqTraffic, digit)==KErrNone)
				{
				profileReq99.iReqTraffic=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,minTraffic);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minTraffic",ret,2,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(minTraffic, digit)==KErrNone)
				{
				profileReq99.iMinTraffic=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,reqDeliveryOrder);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqDeliveryOrder",ret,3,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqDeliveryOrder, digit)==KErrNone)
				{
				profileReq99.iReqDeliveryOrder=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,minDeliveryOrder);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minDeliveryOrder",ret,4,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(minDeliveryOrder, digit)==KErrNone)
				{
				profileReq99.iMinDeliveryOrder=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,reqErroneousSDU);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqErroneousSDU",ret,5,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqErroneousSDU, digit)==KErrNone)
				{
				profileReq99.iReqErroneousSDU=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,6,minErroneousSDU);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minErroneousSDU",ret,6,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(minErroneousSDU, digit)==KErrNone)
				{
				profileReq99.iMinErroneousSDU=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,7,maxSDUSize);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::maxSDUSize",ret,7,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iMaxSDUSize=maxSDUSize;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,8,minSDUSize);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minSDUSize",ret,8,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iMinSDUSize=minSDUSize;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,9,reqUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqUpLinkBitRate",ret,9,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iReqUpLinkTBitRate=reqUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,10,reqDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqDownLinkBitRate",ret,10,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iReqDownLinkTBitRate= reqDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,11,minUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minUpLinkBitRate",ret,11,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iMinUpLinkTBitRate= minUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,12,minDownLinkBitRate);
		if(ret!=KErrNone)
			{
			continue;
			}
		else 
			{
			profileReq99.iMinDownLinkTBitRate= minDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,13,reqBitErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqBitErrorRatio",ret,13,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqBitErrorRatio, digit)==KErrNone)
				{
				profileReq99.iReqTBitErrorRatio=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,14,minBitErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minBitErrorRatio",ret,14,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(minBitErrorRatio, digit)==KErrNone)
				{
				profileReq99.iMinTBitErrorRatio=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,15,reqSDUErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("- QosProfileReqR99::reqSDUErrorRatio",ret,15,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqSDUErrorRatio, digit)==KErrNone)
				{
				profileReq99.iReqTSDUErrorRatio=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,16,minSDUErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("- QosProfileReqR99::minSDUErrorRatio",ret,16,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(minSDUErrorRatio, digit)==KErrNone)
				{
 				profileReq99.iMinTSDUErrorRatio=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,17,reqTrafficHandlingPriority);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqTrafficHandlingPriority",ret,17,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqTrafficHandlingPriority, digit)==KErrNone)
				{
				profileReq99.iReqTTrafficHandlingPriority=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,18,minTrafficHandlingPriority);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minTrafficHandlingPriority",ret,18,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			if(AsciiToNum(minTrafficHandlingPriority, digit)==KErrNone)
				{
				profileReq99.iMinTTrafficHandlingPriority=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,19,reqTransferDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqTransferDelay",ret,19,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iReqTransferDelay= reqTransferDelay;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,20,minTransferDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minTransferDelay",ret,20,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iMinTransferDelay= minTransferDelay;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,21,reqGuaranteedUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqGuaranteedUpLinkBitRate",ret,21,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iReqGuaranteedUpLinkTBitRate= reqGuaranteedUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,22,reqGuaranteedDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::reqGuaranteedDownLinkBitRate",ret,22,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iReqGuaranteedDownLinkTBitRate= reqGuaranteedDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,23,minGuaranteedUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minGuaranteedUpLinkBitRate",ret,23,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iMinGuaranteedUpLinkTBitRate= minGuaranteedUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,24,minGuaranteedDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR99::minGuaranteedDownLinkBitRate",ret,24,&KQosProfileReqR99);
			continue;
			}
		else 
			{
			profileReq99.iMinGuaranteedDownLinkTBitRate= minGuaranteedDownLinkBitRate;
			}

		iReqR99Profiles->AppendL(profileReq99);
		}
	}
	
void CSimPacketQoS::GetR99NegProfilesL()
	{
	TProfileNegR99 profileNeg99;
	TUint8 digit = 0;

	iNegR99Profiles = new(ELeave) CArrayFixFlat<TProfileNegR99>(KNumberofProfiles);

	// parse Negotiated Rel99 profile parameters
	TInt count = iPacketContext->CfgFile()->ItemCount(KQosProfileNegR99);
	for(TInt i = 0; i < count; i++)
		{
		const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosProfileNegR99,i);
		if(!item)
			break;
	
		TPtrC8 contextName, duration, negTraffic, negDeliveryOrder, negErroneousSDUDelivery;
		TPtrC8 negBitErrorRatio, negSDUErrorRatio, negTrafficHandlingPriority;
		TInt negTransferDelay, negGuaranteedUpLinkBitRate, negGuaranteedDownLinkBitRate;
		TInt negMaxUpLinkRate, negMaxDownLinkRate, negMaxSDUSize;
		 
		TInt ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,contextName);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::contextName",ret,0,&KQosProfileNegR99);
			continue;
			}
		else
			{
			profileNeg99.iContextName.Copy(contextName);
			if (profileNeg99.iContextName.Match(iPacketContext->ContextName())==KErrNotFound)
				{
				LOGPARSERR("QosProfileNegR99::contextName",KErrArgument,0,&KQosProfileNegR99);
				continue;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,duration);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::duration",ret,1,&KQosProfileNegR99);
			continue;
			}
		else
			{
			if(AsciiToNum(duration, digit)==KErrNone)
				profileNeg99.iDuration = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,negTraffic);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negTraffic",ret,2,&KQosProfileNegR99);
			continue;
			}
		else
			{
			if(AsciiToNum(negTraffic, digit)==KErrNone)
				profileNeg99.iNegTraffic = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,negDeliveryOrder);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negDeliveryOrder",ret,3,&KQosProfileNegR99);
			continue;
			}
		else
			{
			if(AsciiToNum(negDeliveryOrder, digit)==KErrNone)
				profileNeg99.iNegDeliveryOrder = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,negErroneousSDUDelivery);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negErroneousSDUDelivery",ret,4,&KQosProfileNegR99);
			continue;
			}
		else
			{
			if(AsciiToNum(negErroneousSDUDelivery, digit)==KErrNone)
				profileNeg99.iNegErroneousSDUDelivery = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,negMaxSDUSize);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negMaxSDUSize",ret,5,&KQosProfileNegR99);
			continue;
			}
		else
			{
			profileNeg99.iNegMaxSDUSize =  negMaxSDUSize;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,6,negBitErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negBitErrorRatio",ret,6,&KQosProfileNegR99);
			continue;
			}
		else
			{
			if(AsciiToNum(negBitErrorRatio, digit)==KErrNone)
				profileNeg99.iNegTBitErrorRatio = digit;
			}

		
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,7,negSDUErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negSDUErrorRatio",ret,7,&KQosProfileNegR99);
			continue;
			}
		else
			{
			if(AsciiToNum(negSDUErrorRatio, digit)==KErrNone)
				profileNeg99.iNegTSDUErrorRatio = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,8,negTrafficHandlingPriority);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negTrafficHandlingPriority",ret,8,&KQosProfileNegR99);
			continue;
			}
		else
			{
			if(AsciiToNum(negTrafficHandlingPriority, digit)==KErrNone)
				profileNeg99.iNegTrafficHandlingPriority = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,9,negTransferDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negTransferDelay",ret,9,&KQosProfileNegR99);
			continue;
			}
		else
			{
			profileNeg99.iNegTransferDelay =  negTransferDelay;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,10,negGuaranteedUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negGuaranteedUpLinkBitRate",ret,10,&KQosProfileNegR99);
			continue;
			}
		else
			{
			profileNeg99.iNegGuaranteedUpLinkBitRate =  negGuaranteedUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,11,negGuaranteedDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negGuaranteedDownLinkBitRate",ret,11,&KQosProfileNegR99);
			continue;
			}
		else
			{
			profileNeg99.iNegGuaranteedDownLinkBitRate =  negGuaranteedDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,12,negMaxUpLinkRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negMaxUpLinkRate",ret,12,&KQosProfileNegR99);
			continue;
			}
		else
			{
			profileNeg99.iNegMaxUpLinkRate =  negMaxUpLinkRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,13,negMaxDownLinkRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR99::negMaxDownLinkRate",ret,13,&KQosProfileNegR99);
			continue;
			}
		else
			{
			profileNeg99.iNegMaxDownLinkRate =  negMaxDownLinkRate;
			}

		iNegR99Profiles->AppendL(profileNeg99);
		}
	}
	
void CSimPacketQoS::GetR99NetworkNotificationsL()
	{
	TInt ret;
	TUint8 digit = 0;
	
	iQosNetworkNotifications = new(ELeave) CArrayFixFlat<TProfileNegR99>(KNumberofProfiles);

	// parse the Qos Network Notification parameters
	iNetworkNotificationType = ETimer;
	const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosNetworkNotificationType);
	if (item)
		{
		TPtrC8 networkNotificationType;
		ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,networkNotificationType);
		if(ret==KErrNone && networkNotificationType.Compare(KNotificationTypePublishSubscribe)==0)
			{
			iNetworkNotificationType = EPublishAndSubscribe;
			}
		}
		
	TInt count=iPacketContext->CfgFile()->ItemCount(KQosNetworkNotification);
	for(TInt i = 0; i < count; i++)
		{
		item=iPacketContext->CfgFile()->Item(KQosNetworkNotification,i);
		if(!item)
			break;
		
		TProfileNegR99 qosNetworkNotification;
		TPtrC8 contextName, duration, negTraffic, negDeliveryOrder, negErroneousSDUDelivery;
		TPtrC8 negBitErrorRatio, negSDUErrorRatio, negTrafficHandlingPriority;
		TInt negTransferDelay, negGuaranteedUpLinkBitRate, negGuaranteedDownLinkBitRate;
		TInt negMaxUpLinkRate, negMaxDownLinkRate, negMaxSDUSize;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,0,contextName);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::contextName",ret,0,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosNetworkNotification.iContextName.Copy(contextName);
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,duration);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::duration",ret,1,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(duration, digit)==KErrNone)
				qosNetworkNotification.iDuration = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,negTraffic);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negTraffic",ret,2,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negTraffic, digit)==KErrNone)
				qosNetworkNotification.iNegTraffic = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,negDeliveryOrder);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negDeliveryOrder",ret,3,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negDeliveryOrder, digit)==KErrNone)
				qosNetworkNotification.iNegDeliveryOrder = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,negErroneousSDUDelivery);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negErroneousSDUDelivery",ret,4,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negErroneousSDUDelivery, digit)==KErrNone)
				qosNetworkNotification.iNegErroneousSDUDelivery = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,negMaxSDUSize);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negMaxSDUSize",ret,5,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosNetworkNotification.iNegMaxSDUSize =  negMaxSDUSize;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,6,negBitErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negBitErrorRatio",ret,6,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negBitErrorRatio, digit)==KErrNone)
				qosNetworkNotification.iNegTBitErrorRatio = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,7,negSDUErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negSDUErrorRatio",ret,7,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negSDUErrorRatio, digit)==KErrNone)
				qosNetworkNotification.iNegTSDUErrorRatio = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,8,negTrafficHandlingPriority);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negTrafficHandlingPriority",ret,8,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negTrafficHandlingPriority, digit)==KErrNone)
				qosNetworkNotification.iNegTrafficHandlingPriority = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,9,negTransferDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negTransferDelay",ret,9,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosNetworkNotification.iNegTransferDelay =  negTransferDelay;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,10,negGuaranteedUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negGuaranteedUpLinkBitRate",ret,10,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosNetworkNotification.iNegGuaranteedUpLinkBitRate =  negGuaranteedUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,11,negGuaranteedDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negGuaranteedDownLinkBitRate",ret,11,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosNetworkNotification.iNegGuaranteedDownLinkBitRate =  negGuaranteedDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,12,negMaxUpLinkRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negMaxUpLinkRate",ret,12,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosNetworkNotification.iNegMaxUpLinkRate =  negMaxUpLinkRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,13,negMaxDownLinkRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosNetworkNotification::negMaxDownLinkRate",ret,13,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosNetworkNotification.iNegMaxDownLinkRate = negMaxDownLinkRate;
			}

		iQosNetworkNotifications->AppendL(qosNetworkNotification);
		}
		


	}

void CSimPacketQoS::GetR99QosProfileCaps()
	{
	TUint8 digit = 0;
	
	// parse Caps Rel99 profile parameters
	const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosProfileCapsR99);
	if(item)
		{
		TPtrC8 trafficCap, deliveryOrderReqCap, deliverErroneousSDUCap;
		TPtrC8 BERCap, SDUErrorRatioCap, trafficHandlingPriorityCap;

		TInt ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,0,trafficCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR99::trafficCap",ret,0,&KQosProfileCapsR99);
			}
		else
			if(AsciiToNum(trafficCap, digit)==KErrNone)
				iCapsR99.iTraffic = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,deliveryOrderReqCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR99::deliveryOrderReqCap",ret,1,&KQosProfileCapsR99);
			}
		else
			if(AsciiToNum(deliveryOrderReqCap, digit)==KErrNone)
				iCapsR99.iDeliveryOrderReq = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,deliverErroneousSDUCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR99::deliverErroneousSDUCap",ret,2,&KQosProfileCapsR99);
			}
		else
			if(AsciiToNum(deliverErroneousSDUCap, digit)==KErrNone)
				iCapsR99.iDeliverErroneousSDU = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,BERCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR99::BERCap",ret,3,&KQosProfileCapsR99);
			}
		else
			if(AsciiToNum(BERCap, digit)==KErrNone)
				iCapsR99.iBER = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,SDUErrorRatioCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR99::SDUErrorRatioCap",ret,4,&KQosProfileCapsR99);
			}
		else
			if(AsciiToNum(SDUErrorRatioCap, digit)==KErrNone)
				iCapsR99.iSDUErrorRatio = digit;


		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,trafficHandlingPriorityCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR99::trafficHandlingPriorityCap",ret,5,&KQosProfileCapsR99);
			}
		else
			if(AsciiToNum(trafficHandlingPriorityCap, digit)==KErrNone)
				iCapsR99.iTrafficHandlingPriority = digit;
		}

	}

void CSimPacketQoS::GetR5QosProfileCaps()
	{
	TUint8 digit = 0;
	
	// parse Caps Rel5 profile parameters
	const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosProfileCapsR5);
	if(item)
		{
		TPtrC8 trafficCap, deliveryOrderReqCap, deliverErroneousSDUCap;
		TPtrC8 BERCap, SDUErrorRatioCap, trafficHandlingPriorityCap;
		TPtrC8 signallingIndication, sourceStatisticsDescriptor;

		TInt ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,0,trafficCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR5::trafficCap",ret,0,&KQosProfileCapsR5);
			}
		else
			if(AsciiToNum(trafficCap, digit)==KErrNone)
				iCapsR5.iTraffic = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,deliveryOrderReqCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR5::deliveryOrderReqCap",ret,1,&KQosProfileCapsR5);
			}
		else
			if(AsciiToNum(deliveryOrderReqCap, digit)==KErrNone)
				iCapsR5.iDeliveryOrderReq = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,deliverErroneousSDUCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR5::deliverErroneousSDUCap",ret,2,&KQosProfileCapsR5);
			}
		else
			if(AsciiToNum(deliverErroneousSDUCap, digit)==KErrNone)
				iCapsR5.iDeliverErroneousSDU = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,BERCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR5::BERCap",ret,3,&KQosProfileCapsR5);
			}
		else
			if(AsciiToNum(BERCap, digit)==KErrNone)
				iCapsR5.iBER = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,SDUErrorRatioCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR5::SDUErrorRatioCap",ret,4,&KQosProfileCapsR5);
			}
		else
			if(AsciiToNum(SDUErrorRatioCap, digit)==KErrNone)
				iCapsR5.iSDUErrorRatio = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,trafficHandlingPriorityCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR5::trafficHandlingPriorityCap",ret,5,&KQosProfileCapsR5);
			}
		else
			if(AsciiToNum(trafficHandlingPriorityCap, digit)==KErrNone)
				iCapsR5.iTrafficHandlingPriority = digit;
		
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,6,signallingIndication);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR5::signallingIndication",ret,6,&KQosProfileCapsR5);
			}
		else
			if(AsciiToNum(signallingIndication, digit)==KErrNone)
				iCapsR5.iSignallingIndication = digit;
			
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,7,sourceStatisticsDescriptor);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsR5::sourceStatisticsDescriptor",ret,7,&KQosProfileCapsR5);
			}
		else
			if(AsciiToNum(sourceStatisticsDescriptor, digit)==KErrNone)
				iCapsR5.iNegTSourceStatisticsDescriptor = digit;
		}
	}

void CSimPacketQoS::GetProfileFailSettings()
	{
	const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KSetProfileFail);
	if(item)
		{
		TPtrC8 contextName, numberOfTimes, errorCode;
		TInt32 setProfileDigit;
		TInt ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,contextName);

		TName tempContextName;
		tempContextName.Copy(contextName);		
		if (iPacketContext->ContextName().Compare(tempContextName)==0)
			{
			ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,numberOfTimes);
			if(ret!=KErrNone)
				{
				LOGPARSERR("SetProfileFail::numberOfTimes",ret,1,&KSetProfileFail);
				}
			else
				{
				if(AsciiToNum(numberOfTimes, setProfileDigit)==KErrNone)
					iSetProfileFail.iNumberOfTimes = setProfileDigit;
				}
		
			ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,errorCode);
			if(ret!=KErrNone)
				{
				LOGPARSERR("SetProfileFail::errorCode",ret,2,&KSetProfileFail);
				}
			else
				{
				if(AsciiToNum(errorCode, setProfileDigit)==KErrNone)
					iSetProfileFail.iFailErrorCode = setProfileDigit;
				}
			}
		else
			{
			iSetProfileFail.iNumberOfTimes = 0;
			iSetProfileFail.iFailErrorCode = KErrNone;
			}	
		}
	}

void CSimPacketQoS::GetGPRSQosProfileCaps()
	{
	TUint8 digit = 0;

	const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosProfileCapsGPRS);
	if(item)
		{
		TPtrC8 negPrecedenceCap, negDelayCap, negPeekCap, negReliabilityCap, negMeanCap;

		TInt ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,negPrecedenceCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsGPRS::negPrecedenceCap",ret,0,&KQosProfileCapsGPRS);
			}
		else
			if(AsciiToNum(negPrecedenceCap, digit)==KErrNone)
				iCapsGPRS.iPrecedence = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,negDelayCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsGPRS::negDelayCap",ret,1,&KQosProfileCapsGPRS);
			}
		else
			if(AsciiToNum(negDelayCap, digit)==KErrNone)
				iCapsGPRS.iDelay = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,negReliabilityCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsGPRS::negReliabilityCap",ret,2,&KQosProfileCapsGPRS);
			}
		else
			if(AsciiToNum(negReliabilityCap, digit)==KErrNone)
				iCapsGPRS.iReliability = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,negPeekCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsGPRS::negPeekCap",ret,3,&KQosProfileCapsGPRS);
			}
		else
			if(AsciiToNum(negPeekCap, digit)==KErrNone)
				iCapsGPRS.iPeak = digit;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,negMeanCap);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileCapsGPRS::negMeanCap",ret,4,&KQosProfileCapsGPRS);
			}
		else
			if(AsciiToNum(negMeanCap, digit)==KErrNone)
				iCapsGPRS.iMean = digit;

		}
	}

	
void CSimPacketQoS::GetR5ReqProfilesL()
{
	TProfileReqR5 profileReq5;
	TUint8 digit = 0;

	iReqR5Profiles = new(ELeave) CArrayFixFlat<TProfileReqR5>(KNumberofProfiles);

	// parse Release Rel99 profile parameters
	TInt count = iPacketContext->CfgFile()->ItemCount(KQosProfileReqR5);
	
	for(TInt i = 0; i < count; i++)
		{
		const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosProfileReqR5,i);
		if(!item)
			break;

		// continue to load item's parameters
		TPtrC8 contextName, reqTraffic, minTraffic, reqDeliveryOrder, minDeliveryOrder;
		TPtrC8 reqErroneousSDU, minErroneousSDU;
		TInt minUpLinkBitRate, minDownLinkBitRate, reqUpLinkBitRate, reqDownLinkBitRate;
		TInt maxSDUSize, minSDUSize;
		TPtrC8 reqBitErrorRatio, minBitErrorRatio, reqSDUErrorRatio, minSDUErrorRatio;
		TPtrC8 reqTrafficHandlingPriority, minTrafficHandlingPriority;
		TInt reqTransferDelay, minTransferDelay, reqGuaranteedUpLinkBitRate, reqGuaranteedDownLinkBitRate;
		TInt minGuaranteedUpLinkBitRate, minGuaranteedDownLinkBitRate;
		TInt signallingIndication;
		TPtrC8 reqTSourceStatisticsDescriptor;

		TInt ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,0,contextName);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::contextName",ret,0,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iContextName.Copy(contextName);
			if (profileReq5.iContextName.Match(iPacketContext->ContextName())==KErrNotFound)
				{
				LOGPARSERR("QosProfileReqR5::contextName",KErrArgument,0,&KQosProfileReqR5);
				continue;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,reqTraffic);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqTraffic",ret,1,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqTraffic, digit)==KErrNone)
				{
				profileReq5.iReqTraffic=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,minTraffic);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minTraffic",ret,2,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(minTraffic, digit)==KErrNone)
				{
				profileReq5.iMinTraffic=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,reqDeliveryOrder);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqDeliveryOrder",ret,3,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqDeliveryOrder, digit)==KErrNone)
				{
				profileReq5.iReqDeliveryOrder=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,minDeliveryOrder);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minDeliveryOrder",ret,4,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(minDeliveryOrder, digit)==KErrNone)
				{
				profileReq5.iMinDeliveryOrder=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,reqErroneousSDU);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqErroneousSDU",ret,5,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqErroneousSDU, digit)==KErrNone)
				{
				profileReq5.iReqErroneousSDU=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,6,minErroneousSDU);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minErroneousSDU",ret,6,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(minErroneousSDU, digit)==KErrNone)
				{
				profileReq5.iMinErroneousSDU=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,7,maxSDUSize);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::maxSDUSize",ret,7,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iMaxSDUSize=maxSDUSize;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,8,minSDUSize);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minSDUSize",ret,8,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iMinSDUSize=minSDUSize;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,9,reqUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqUpLinkBitRate",ret,9,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iReqUpLinkTBitRate=reqUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,10,reqDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqDownLinkBitRate",ret,10,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iReqDownLinkTBitRate= reqDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,11,minUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minUpLinkBitRate",ret,11,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iMinUpLinkTBitRate= minUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,12,minDownLinkBitRate);
		if(ret!=KErrNone)
			{
			continue;
			}
		else 
			{
			profileReq5.iMinDownLinkTBitRate= minDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,13,reqBitErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqBitErrorRatio",ret,13,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqBitErrorRatio, digit)==KErrNone)
				{
				profileReq5.iReqTBitErrorRatio=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,14,minBitErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minBitErrorRatio",ret,14,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(minBitErrorRatio, digit)==KErrNone)
				{
				profileReq5.iMinTBitErrorRatio=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,15,reqSDUErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("- QosProfileReqR5::reqSDUErrorRatio",ret,15,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqSDUErrorRatio, digit)==KErrNone)
				{
				profileReq5.iReqTSDUErrorRatio=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,16,minSDUErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("- QosProfileReqR5::minSDUErrorRatio",ret,16,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(minSDUErrorRatio, digit)==KErrNone)
				{
 				profileReq5.iMinTSDUErrorRatio=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,17,reqTrafficHandlingPriority);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqTrafficHandlingPriority",ret,17,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqTrafficHandlingPriority, digit)==KErrNone)
				{
				profileReq5.iReqTTrafficHandlingPriority=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,18,minTrafficHandlingPriority);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minTrafficHandlingPriority",ret,18,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(minTrafficHandlingPriority, digit)==KErrNone)
				{
				profileReq5.iMinTTrafficHandlingPriority=digit;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,19,reqTransferDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqTransferDelay",ret,19,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iReqTransferDelay= reqTransferDelay;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,20,minTransferDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minTransferDelay",ret,20,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iMinTransferDelay= minTransferDelay;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,21,reqGuaranteedUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqGuaranteedUpLinkBitRate",ret,21,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iReqGuaranteedUpLinkTBitRate= reqGuaranteedUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,22,reqGuaranteedDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqGuaranteedDownLinkBitRate",ret,22,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iReqGuaranteedDownLinkTBitRate= reqGuaranteedDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,23,minGuaranteedUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minGuaranteedUpLinkBitRate",ret,23,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iMinGuaranteedUpLinkTBitRate= minGuaranteedUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,24,minGuaranteedDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::minGuaranteedDownLinkBitRate",ret,24,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iMinGuaranteedDownLinkTBitRate= minGuaranteedDownLinkBitRate;
			}
		
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,25,signallingIndication);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::signallingIndication",ret,25,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			profileReq5.iSignallingIndication= signallingIndication;
			}
			
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,26,reqTSourceStatisticsDescriptor);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileReqR5::reqTSourceStatisticsDescriptor",ret,26,&KQosProfileReqR5);
			continue;
			}
		else 
			{
			if(AsciiToNum(reqTSourceStatisticsDescriptor, digit)==KErrNone)
				{
				profileReq5.iReqTSourceStatisticsDescriptor=digit;
				}
			}
		
		iReqR5Profiles->AppendL(profileReq5);
		}
}

void CSimPacketQoS::GetR5NegProfilesL()
{
	TProfileNegR5 profileNeg5;
	TUint8 digit = 0;

	iNegR5Profiles = new(ELeave) CArrayFixFlat<TProfileNegR5>(KNumberofProfiles);

	// parse Negotiated Rel5 profile parameters
	TInt count = iPacketContext->CfgFile()->ItemCount(KQosProfileNegR5);
	for(TInt i = 0; i < count; i++)
		{
		const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosProfileNegR5,i);
		if(!item)
			break;
	
		TPtrC8 contextName, duration, negTraffic, negDeliveryOrder, negErroneousSDUDelivery;
		TPtrC8 negBitErrorRatio, negSDUErrorRatio, negTrafficHandlingPriority;
		TInt negTransferDelay, negGuaranteedUpLinkBitRate, negGuaranteedDownLinkBitRate;
		TInt negMaxUpLinkRate, negMaxDownLinkRate, negMaxSDUSize;
		TInt  signallingIndication;
 		TPtrC8 reqTSourceStatisticsDescriptor;
		 
		TInt ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,contextName);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::contextName",ret,0,&KQosProfileNegR5);
			continue;
			}
		else
			{
			profileNeg5.iContextName.Copy(contextName);
			if (profileNeg5.iContextName.Match(iPacketContext->ContextName())==KErrNotFound)
				{
				LOGPARSERR("QosProfileNegR5::contextName",KErrArgument,0,&KQosProfileNegR5);
				continue;
				}
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,duration);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::duration",ret,1,&KQosProfileNegR5);
			continue;
			}
		else
			{
			if(AsciiToNum(duration, digit)==KErrNone)
				profileNeg5.iDuration = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,negTraffic);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negTraffic",ret,2,&KQosProfileNegR5);
			continue;
			}
		else
			{
			if(AsciiToNum(negTraffic, digit)==KErrNone)
				profileNeg5.iNegTraffic = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,negDeliveryOrder);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negDeliveryOrder",ret,3,&KQosProfileNegR5);
			continue;
			}
		else
			{
			if(AsciiToNum(negDeliveryOrder, digit)==KErrNone)
				profileNeg5.iNegDeliveryOrder = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,negErroneousSDUDelivery);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negErroneousSDUDelivery",ret,4,&KQosProfileNegR5);
			continue;
			}
		else
			{
			if(AsciiToNum(negErroneousSDUDelivery, digit)==KErrNone)
				profileNeg5.iNegErroneousSDUDelivery = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,negMaxSDUSize);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negMaxSDUSize",ret,5,&KQosProfileNegR5);
			continue;
			}
		else
			{
			profileNeg5.iNegMaxSDUSize =  negMaxSDUSize;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,6,negBitErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negBitErrorRatio",ret,6,&KQosProfileNegR5);
			continue;
			}
		else
			{
			if(AsciiToNum(negBitErrorRatio, digit)==KErrNone)
				profileNeg5.iNegTBitErrorRatio = digit;
			}

		
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,7,negSDUErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negSDUErrorRatio",ret,7,&KQosProfileNegR5);
			continue;
			}
		else
			{
			if(AsciiToNum(negSDUErrorRatio, digit)==KErrNone)
				profileNeg5.iNegTSDUErrorRatio = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,8,negTrafficHandlingPriority);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negTrafficHandlingPriority",ret,8,&KQosProfileNegR5);
			continue;
			}
		else
			{
			if(AsciiToNum(negTrafficHandlingPriority, digit)==KErrNone)
				profileNeg5.iNegTrafficHandlingPriority = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,9,negTransferDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negTransferDelay",ret,9,&KQosProfileNegR5);
			continue;
			}
		else
			{
			profileNeg5.iNegTransferDelay =  negTransferDelay;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,10,negGuaranteedUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negGuaranteedUpLinkBitRate",ret,10,&KQosProfileNegR5);
			continue;
			}
		else
			{
			profileNeg5.iNegGuaranteedUpLinkBitRate =  negGuaranteedUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,11,negGuaranteedDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negGuaranteedDownLinkBitRate",ret,11,&KQosProfileNegR5);
			continue;
			}
		else
			{
			profileNeg5.iNegGuaranteedDownLinkBitRate =  negGuaranteedDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,12,negMaxUpLinkRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negMaxUpLinkRate",ret,12,&KQosProfileNegR5);
			continue;
			}
		else
			{
			profileNeg5.iNegMaxUpLinkRate =  negMaxUpLinkRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,13,negMaxDownLinkRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::negMaxDownLinkRate",ret,13,&KQosProfileNegR5);
			continue;
			}
		else
			{
			profileNeg5.iNegMaxDownLinkRate =  negMaxDownLinkRate;
			}
		
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,14,signallingIndication);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::signallingIndication",ret,14,&KQosProfileNegR5);
			continue;
			}
		else
			{
			profileNeg5.iSignallingIndication =  signallingIndication;
			}
			
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,15,reqTSourceStatisticsDescriptor);
		if(ret!=KErrNone)
			{
			LOGPARSERR("QosProfileNegR5::reqTSourceStatisticsDescriptor",ret,15,&KQosProfileNegR5);
			continue;
			}
		else
			{
			if(AsciiToNum(reqTSourceStatisticsDescriptor, digit)==KErrNone)
				profileNeg5.iNegTSourceStatisticsDescriptor = digit;
			}
		

		iNegR5Profiles->AppendL(profileNeg5);
		}
}

void CSimPacketQoS::GetR5NetworkNotificationsL()
	{
	TInt ret;
	TUint8 digit = 0;
	
	iQosR5NetworkNotifications = new(ELeave) CArrayFixFlat<TProfileNegR5>(KNumberofProfiles);

	// parse the Qos Network Notification parameters
	iNetworkNotificationType = ETimer;
	const CTestConfigItem* item = iPacketContext->CfgFile()->Item(KQosNetworkNotificationType);
	if (item)
		{
		TPtrC8 networkNotificationType;
		ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,networkNotificationType);
		if(ret==KErrNone && networkNotificationType.Compare(KNotificationTypePublishSubscribe)==0)
			{
			iNetworkNotificationType = EPublishAndSubscribe;
			}
		}
		
	TInt count=iPacketContext->CfgFile()->ItemCount(KQosR5NetworkNotification);
	for(TInt i = 0; i < count; i++)
		{
		item=iPacketContext->CfgFile()->Item(KQosR5NetworkNotification,i);
		if(!item)
			break;
		
		TProfileNegR5 qosR5NetworkNotification;
		TPtrC8 contextName, duration, negTraffic, negDeliveryOrder, negErroneousSDUDelivery;
		TPtrC8 negBitErrorRatio, negSDUErrorRatio, negTrafficHandlingPriority;
		TInt negTransferDelay, negGuaranteedUpLinkBitRate, negGuaranteedDownLinkBitRate;
		TInt negMaxUpLinkRate, negMaxDownLinkRate, negMaxSDUSize;
		TInt  signallingIndication;
		TPtrC8 reqTSourceStatisticsDescriptor;

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,0,contextName);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::contextName",ret,0,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosR5NetworkNotification.iContextName.Copy(contextName);
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,duration);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::duration",ret,1,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(duration, digit)==KErrNone)
				qosR5NetworkNotification.iDuration = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,negTraffic);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negTraffic",ret,2,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negTraffic, digit)==KErrNone)
				qosR5NetworkNotification.iNegTraffic = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,negDeliveryOrder);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negDeliveryOrder",ret,3,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negDeliveryOrder, digit)==KErrNone)
				qosR5NetworkNotification.iNegDeliveryOrder = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,4,negErroneousSDUDelivery);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negErroneousSDUDelivery",ret,4,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negErroneousSDUDelivery, digit)==KErrNone)
				qosR5NetworkNotification.iNegErroneousSDUDelivery = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,5,negMaxSDUSize);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negMaxSDUSize",ret,5,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosR5NetworkNotification.iNegMaxSDUSize =  negMaxSDUSize;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,6,negBitErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negBitErrorRatio",ret,6,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negBitErrorRatio, digit)==KErrNone)
				qosR5NetworkNotification.iNegTBitErrorRatio = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,7,negSDUErrorRatio);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negSDUErrorRatio",ret,7,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negSDUErrorRatio, digit)==KErrNone)
				qosR5NetworkNotification.iNegTSDUErrorRatio = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,8,negTrafficHandlingPriority);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negTrafficHandlingPriority",ret,8,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(negTrafficHandlingPriority, digit)==KErrNone)
				qosR5NetworkNotification.iNegTrafficHandlingPriority = digit;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,9,negTransferDelay);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negTransferDelay",ret,9,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosR5NetworkNotification.iNegTransferDelay =  negTransferDelay;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,10,negGuaranteedUpLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negGuaranteedUpLinkBitRate",ret,10,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosR5NetworkNotification.iNegGuaranteedUpLinkBitRate =  negGuaranteedUpLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,11,negGuaranteedDownLinkBitRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negGuaranteedDownLinkBitRate",ret,11,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosR5NetworkNotification.iNegGuaranteedDownLinkBitRate =  negGuaranteedDownLinkBitRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,12,negMaxUpLinkRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negMaxUpLinkRate",ret,12,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosR5NetworkNotification.iNegMaxUpLinkRate =  negMaxUpLinkRate;
			}

		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,13,negMaxDownLinkRate);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::negMaxDownLinkRate",ret,13,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosR5NetworkNotification.iNegMaxDownLinkRate = negMaxDownLinkRate;
			}
			
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,14,signallingIndication);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::signallingIndication",ret,14,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			qosR5NetworkNotification.iSignallingIndication =  signallingIndication;
			}
			
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,15,reqTSourceStatisticsDescriptor);
		if(ret!=KErrNone)
			{
			LOGPARSERR("qosR5NetworkNotification::reqTSourceStatisticsDescriptor",ret,15,&KQosNetworkNotificationType);
			continue;
			}
		else
			{
			if(AsciiToNum(reqTSourceStatisticsDescriptor, digit)==KErrNone)
				qosR5NetworkNotification.iNegTSourceStatisticsDescriptor = digit;
			}

		iQosR5NetworkNotifications->AppendL(qosR5NetworkNotification);
		}
	}

void CSimPacketQoS::GetSetQoSSettings()
/**
* Gets set qos delay settins from config file
*
*/
 	{
 	TInt ret;
 	iSetQoSDelay = 0;
 	iSetQoSDelayAt=0;
 	 	
  	const CTestConfigItem* item=iPacketContext->CfgFile()->Item(KSetQoSDelay);
 	if(item)
 		{
 		TInt delay = 0;
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,0,delay);
		if(ret!=KErrNone || delay < 0)
			{
			LOGPARSERR("SetQoSDelay::delay",ret,0,&KSetQoSDelay);
			}
		else
			{
			iSetQoSDelay = delay;
			}
			
		TInt at = 0;
		ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,1,at);
		if(ret != KErrNone)
			{
			LOGPARSERR("SetQoSDelay::at",ret,1,&KSetQoSDelay);
			}
		else
			{
			iSetQoSDelayAt = at;
			}
 		}
 	}
 
	
CSimPacketQoS::~CSimPacketQoS()
/**
* Trivial Destructor
* Closes all CObject type objects and destroys all other objects created in the ConstructL()
*
*/
	{
	LOGPACKET1("CSimPacketQoS: Entered destructor");

	if (iProfiles != NULL)
	{
		iProfiles->Delete(0,iProfiles->Count());
		delete iProfiles;
	}
	if (iNegProfiles != NULL)
	{
		iNegProfiles->Delete(0,iNegProfiles->Count());
		delete iNegProfiles;
	}
	if (iReqR99Profiles != NULL)
	{
		iReqR99Profiles->Delete(0, iReqR99Profiles->Count());
		delete iReqR99Profiles;
	}
	if (iNegR99Profiles != NULL)
	{
		iNegR99Profiles->Delete(0,iNegR99Profiles->Count());
		delete iNegR99Profiles;
	}
	if (iReqR5Profiles != NULL)
	{
		iReqR5Profiles->Delete(0, iReqR5Profiles->Count());
		delete iReqR5Profiles;
	}
	if (iNegR5Profiles != NULL)
	{
		iNegR5Profiles->Delete(0,iNegR5Profiles->Count());
		delete iNegR5Profiles;
	}
	if (iCapsR99Profiles != NULL)
	{
		iCapsR99Profiles->Delete(0, iCapsR99Profiles->Count());
		delete iCapsR99Profiles;
	}
	if (iQosNetworkNotifications != NULL)
	{
		iQosNetworkNotifications->Delete(0, iQosNetworkNotifications->Count());
		delete iQosNetworkNotifications;
	}
	if (iQosR5NetworkNotifications != NULL)
	{
		iQosR5NetworkNotifications->Delete(0, iQosR5NetworkNotifications->Count());
		delete iQosR5NetworkNotifications;
	}
	if (iTimer != NULL)
	{
		delete iTimer;
	}
	if (iSetQoSTimer != NULL)
 	{
 		delete iSetQoSTimer;
 	}	

	delete iSimQoSChange;
	
	delete iSetQoSData;
	}

CTelObject* CSimPacketQoS::OpenNewObjectByNameL(const TDesC& /*aName*/)
/**
* Opening object(s) from RPacketQoS is not supported
*
* @param aName Name of object to open
* @return CTelObject pointer to the object created.
*/
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}

CTelObject* CSimPacketQoS::OpenNewObjectL(TDes& /*aNewName*/)
/**
* Opening object(s) from RPacketQoS is not supported
*
* @param aNewName Name of object to open
* @return CTelObject pointer to the object created.
*/
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}

CTelObject::TReqMode CSimPacketQoS::ReqModeL(const TInt aIpc)
/**
* ReqModeL is called from the server's CTelObject::ReqAnalyserL
* in order to check the type of request it has.
* 
* 
* @param aIpc the ipc number that identifies the client request
* @return CTelObject::TReqMode The request mode to be used for this request
* @leave Leaves if not supported by this tsy
*/
	{	
	CTelObject::TReqMode ret=0;
	switch (aIpc)
		{
		case EPacketQoSSetProfileParams:
		case EPacketQoSGetProfileParams:
		case EPacketQoSGetProfileCaps:	
			break;

		case EPacketQoSNotifyProfileChanged:
			ret=KReqModeMultipleCompletionEnabled | KReqModeRePostImmediately;
			break;

		default:
			User::Leave(KErrNotSupported);
			break;
		}

	return ret;
	}

TInt CSimPacketQoS::RegisterNotification(const TInt aIpc)
/**
* RegisterNotification is called when the server recognises that this notification
* is being posted for the first time on this sub-session object.
* 
* It enables the TSY to "turn on" any regular notification messages that it may receive 
* from the phone
*
* @param aIpc the ipc number that identifies the client request
* @return err KErrNone if fine
*/
	{
	switch (aIpc)
		{
		case EPacketQoSNotifyProfileChanged:
			LOGPACKET1("CSimPacketQoS: RegisterNotification - Profile Changed");
			return KErrNone;
		default:
			// Unknown or invalid IPC
			LOGPACKET1("CSimPacketQoS: Register error, unknown IPC");
			return KErrNotSupported;
		}
	}

TInt CSimPacketQoS::DeregisterNotification(const TInt aIpc)
/**
* DeregisterNotification is called when the server recognises that this notification
* will not be posted again because the last client to have a handle on this sub-session
* object has just closed the handle.
*
* It enables the TSY to "turn off" any regular notification messages that it may 
* receive from the phone
*
* @param aIpc the ipc number that identifies the client request
* @return err KErrNone if fine
*/
	{
	switch (aIpc)
		{
		case EPacketQoSNotifyProfileChanged:
			LOGPACKET1("CSimPacketQoS: DeregisterNotification - Profile Changed");
			return KErrNone;
		default:
			// Unknown or invalid IPC
			LOGPACKET1("CSimPacketQoS: Deregister error, unknown IPC");
			return KErrNotSupported;
		}
	}

TInt CSimPacketQoS::NumberOfSlotsL(const TInt aIpc)
/**
* NumberOfSlotsL is called by the server when it is registering a new notification
* It enables the TSY to tell the server how many buffer slots to allocate for
* "repost immediately" notifications that may trigger before clients collect them
*
* @param aIpc the ipc number that identifies the client request
* @return err KErrNone if fine
*/
	{
	TInt numberOfSlots=1;
	switch (aIpc)
		{
		case EPacketQoSNotifyProfileChanged:
			LOGPACKET1("CGprsDGprsTsy: Registered with 5 slots");
			numberOfSlots=5;
			break;
		default:
			// Unknown or invalid IPC
			LOGPACKET1("CSimPacketQoS: Number of Slots error, unknown IPC");
			User::Leave(KErrNotSupported);
			break;
		}  
	return numberOfSlots;
	}


TInt CSimPacketQoS::ExtFunc(const TTsyReqHandle aTsyReqHandle,const TInt aIpc,
							  const TDataPackage& aPackage)
/**
* ExtFunc is called by the server when it has a "extended", i.e. non-core ETel request
* for the TSY to process.
* A request handle, request type and request data are passed to the TSY
* 
* @param aTsyReqHandle  The request handle for completing the request 
* @param aIpc Ipc representing the request
* @param aPackage any data associated with the request
* @return err KErrNone if request completes ok
*/
	{
	/**
	* for UMTS on specified number, SetProfileFail occurs
	**/
	switch (aIpc)
		{
		case EPacketQoSSetProfileParams:
		{
			if (iSetProfileCallCount <= iSetProfileFail.iNumberOfTimes	// if the SetProfileFail limit has not been reached
				|| iSetProfileFail.iNumberOfTimes==0)					// call SetProfile()
			{
 			
 				// only delay setprofile when the user wants to
 				if (iSetQoSDelay > 0 && iSetProfileCallCount == iSetQoSDelayAt)
 				{
 				TSetQoSData setQoSData;
 				setQoSData.iTsyReqHandle = aTsyReqHandle;
 				setQoSData.iData = aPackage.Des1n();
 
 				TRAPD(ret, iSetQoSData->AppendL(setQoSData) );
 				if (ret != KErrNone)
                 	{
                 	LOGPACKET1("CSimPacketQoS::ExtFunc AppendL call fail");
                 	return ret;
                 	}
                 	
               iSetProfileCallCount++;		
 				iSetQoSTimer->Start(iSetQoSDelay,this,ETimerIdPcktQoSSet);				

 				return KErrNone;		
 			
 				}
 				else
 				{			
				iSetProfileCallCount++;
				return SetProfile(aTsyReqHandle, aPackage.Des1n());	
 				}
				
			}
			else
			{
				iSetProfileCallCount=0;							
				iSetProfileFail.iNumberOfTimes = 0;
				ReqCompleted(aTsyReqHandle,iSetProfileFail.iFailErrorCode);
				return KErrNone;
			}
		}
		case EPacketQoSGetProfileParams:
			return GetProfile(aTsyReqHandle, aPackage.Des1n());
		case EPacketQoSNotifyProfileChanged:
			return NotifyProfileChanged(aTsyReqHandle, aPackage.Des1n());			
		case EPacketQoSGetProfileCaps:
			return GetProfileCaps(aTsyReqHandle, aPackage.Des1n());
		case EPacketQoSNotifyProfileChangedCancel:
			return NotifyProfileChangedCancel(aTsyReqHandle); 
		case EPacketQoSSetProfileParamsCancel:
			return SetProfileCancel(aTsyReqHandle);
		case EPacketQoSGetProfileParamsCancel:
			return GetProfileCancel(aTsyReqHandle);
		case EPacketQoSGetProfileCapsCancel:
			return GetProfileCapsCancel(aTsyReqHandle);
		default:
			return KErrNotSupported;
		}
	}

TInt CSimPacketQoS::CancelService(const TInt aIpc,const TTsyReqHandle aTsyReqHandle)
/**
* CancelService is called by the server when it is "cleaning-up" any still outstanding
* asynchronous requests before closing a client's sub-session.
* This will happen if a client closes its R-class handle without cancelling outstanding asynchronous requests.
* 
* @param aTsyReqHandle  The request handle for completing the request 
* @param aIpc Ipc representing the request
* @return err KErrNone if request completes ok
*/
	{
	LOGPACKET1("CSimPacketQoS: - CancelService called");
	switch (aIpc)
		{
		case EPacketQoSSetProfileParams:
			return SetProfileCancel(aTsyReqHandle);
		case EPacketQoSGetProfileParams:
			return GetProfileCancel(aTsyReqHandle);
		case EPacketQoSGetProfileCaps:
			return GetProfileCapsCancel(aTsyReqHandle);
		case EPacketQoSNotifyProfileChanged:
			return NotifyProfileChangedCancel(aTsyReqHandle);
		default:
			return KErrGeneral; 
		} 
	}

TInt CSimPacketQoS::SetProfile(const TTsyReqHandle aTsyReqHandle,const TDesC8* aConfig)
/**
* Sets the Quality of Service Requested and Minimum Profile Parameters supported.
*
* In this TSY, we compare the value sent by the client with the data read from the configuration file.
* If they are not the same then the data sent has been corrupted.
*
* @param aTsyReqHandle Tsy Request handle for the client request
* @param aConfig pointer containing the parameters to set to.
* @return KErrNone
*/
	{
	LOGPACKET1("CSimPacketQoS::SetProfile called");
	TPckg<TPacketDataConfigBase>* configBase = (TPckg<TPacketDataConfigBase>*)aConfig;
	TPacketDataConfigBase& configBaseV1 = (*configBase)();

	TInt err = KErrGeneral;

	// GPRS
	if(configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigGPRS)
		{
		TPckg<RPacketQoS::TQoSGPRSRequested>* profileConfig = (TPckg<RPacketQoS::TQoSGPRSRequested>*)aConfig;
		RPacketQoS::TQoSGPRSRequested& qosConfigV1 = (*profileConfig)();

		for(TInt i=0; i<iProfiles->Count(); i++)
			{
			TProfileConfig profile = iProfiles->At(i);

			if ((qosConfigV1.iMinDelay != profile.iMinDelay)||
			(qosConfigV1.iMinMeanThroughput != profile.iMinMeanThroughput)||
			(qosConfigV1.iMinPeakThroughput != profile.iMinPeekThroughput)||
			(qosConfigV1.iMinPrecedence != profile.iMinPrecedence)||
			(qosConfigV1.iMinReliability != profile.iMinReliability)||
			(qosConfigV1.iReqDelay != profile.iReqDelay)||
			(qosConfigV1.iReqMeanThroughput != profile.iReqMeanThroughput)||
			(qosConfigV1.iReqPeakThroughput != profile.iReqPeekThroughput)||
			(qosConfigV1.iReqPrecedence != profile.iReqPrecedence)||
			(qosConfigV1.iReqReliability != profile.iReqReliability))
				err=KErrCorrupt;
			else
				{
				err=KErrNone;
				iSetProfilePending = ETrue;
				// Check for a pending profile notification.
				if(iNotifyProfileGPRS.iNotifyPending)
					{
					iNotifyProfileGPRS.iNotifyPending=EFalse;					
					RPacketQoS::TQoSGPRSNegotiated notifyProfile;
					//echo the requested values back to the notify message and complete
					notifyProfile.iDelay = qosConfigV1.iReqDelay;
					notifyProfile.iMeanThroughput = qosConfigV1.iReqMeanThroughput;
					notifyProfile.iPeakThroughput = qosConfigV1.iReqPeakThroughput;
					notifyProfile.iPrecedence = qosConfigV1.iReqPrecedence;
					notifyProfile.iReliability = qosConfigV1.iReqReliability;					
					*(RPacketQoS::TQoSGPRSNegotiated*)iNotifyProfileGPRS.iNotifyData = notifyProfile;
					ReqCompleted(iNotifyProfileGPRS.iNotifyHandle,KErrNone);
					}
				iUseNetworkNotificationBool = EFalse;
				break;
				}
			}
		ReqCompleted(aTsyReqHandle,err);
		}
		// UMTS
	else if (configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigRel99Rel4)
			{
			TPckg<RPacketQoS::TQoSR99_R4Requested>* rel99Config = (TPckg<RPacketQoS::TQoSR99_R4Requested>*)aConfig;
			RPacketQoS::TQoSR99_R4Requested& qosConfigV1 = (*rel99Config)();
			
			for(TInt i=0; i<iReqR99Profiles->Count(); i++)
				{
				TProfileReqR99 profile = iReqR99Profiles->At(i);

				if (MatchContext(profile)==KErrNone)
					{
					LOGPACKET1("====REQ QOS===");
	
					
					
				//	LOGPACKET3("Req Traffic [%d] [%d]", qosConfigV1.iReqTrafficClass, profile.iReqTraffic);
				//	LOGCOMMON3("Min Traffic [%d] [%d]", qosConfigV1.iMinTrafficClass, profile.iMinTraffic);
					LOGPACKET3("Req Del order [%d] [%d]",qosConfigV1.iReqDeliveryOrderReqd, profile.iReqDeliveryOrder);
					LOGPACKET3("Min Del order [%d] [%d]",qosConfigV1.iMinDeliveryOrderReqd , profile.iMinDeliveryOrder);
					LOGPACKET3("Req Del err SDU [%d] [%d]",qosConfigV1.iReqDeliverErroneousSDU , profile.iReqErroneousSDU);
					LOGPACKET3("Min Del err SDU [%d] [%d]",qosConfigV1.iMinDeliverErroneousSDU , profile.iMinErroneousSDU);
 					LOGPACKET3("Max Sdu Size [%d] [%d]",qosConfigV1.iReqMaxSDUSize , profile.iMaxSDUSize);
 					LOGPACKET3("Min Sdu Size [%d] [%d]",qosConfigV1.iMinAcceptableMaxSDUSize , profile.iMinSDUSize);
  					LOGPACKET3("Req Down [%d] [%d]",qosConfigV1.iReqMaxRate.iDownlinkRate , profile.iReqDownLinkTBitRate);
  					LOGPACKET3("Req Up [%d] [%d]", qosConfigV1.iReqMaxRate.iUplinkRate , profile.iReqUpLinkTBitRate);
					LOGPACKET3("Min Guar Down[%d] [%d]",qosConfigV1.iMinGuaranteedRate.iDownlinkRate , profile.iMinGuaranteedDownLinkTBitRate);
					LOGPACKET3("Min Guar Up [%d] [%d]",qosConfigV1.iMinGuaranteedRate.iUplinkRate , profile.iMinGuaranteedUpLinkTBitRate);
					LOGPACKET3("Req BER [%d] [%d]",qosConfigV1.iReqBER , profile.iReqTBitErrorRatio);
					LOGPACKET3("Max BER [%d] [%d]",qosConfigV1.iMaxBER , profile.iMinTBitErrorRatio);
					LOGPACKET3("Req SDU err ratio [%d] [%d]",qosConfigV1.iReqSDUErrorRatio , profile.iReqTSDUErrorRatio);
					LOGPACKET3("Max SDU err ration [%d] [%d]",qosConfigV1.iMaxSDUErrorRatio , profile.iMinTSDUErrorRatio);
					LOGPACKET3("Req Traffic Handling priority [%d] [%d]",qosConfigV1.iReqTrafficHandlingPriority , profile.iReqTTrafficHandlingPriority);
					LOGPACKET3("Min Traffic Handling priority [%d] [%d]",qosConfigV1.iMinTrafficHandlingPriority , profile.iMinTTrafficHandlingPriority);
					LOGPACKET3("Req Transfer Delay [%d] [%d]",qosConfigV1.iReqTransferDelay , profile.iReqTransferDelay);
					LOGPACKET3("Max Transfer Delay [%d] [%d]",qosConfigV1.iMaxTransferDelay , profile.iMinTransferDelay);
					LOGPACKET3("Req Guar downlink bitrate [%d] [%d]",qosConfigV1.iReqGuaranteedRate.iDownlinkRate , profile.iReqGuaranteedDownLinkTBitRate);
					LOGPACKET3("Req Guar uplink bitrate [%d] [%d]",qosConfigV1.iReqGuaranteedRate.iUplinkRate , profile.iReqGuaranteedUpLinkTBitRate);
					LOGPACKET3("Min Guar downlink [%d] [%d]",qosConfigV1.iMinGuaranteedRate.iDownlinkRate , profile.iMinGuaranteedDownLinkTBitRate);
					LOGPACKET3("Min Guar uplink  [%d] [%d]",qosConfigV1.iMinGuaranteedRate.iUplinkRate , profile.iMinGuaranteedUpLinkTBitRate);
						
					
					
					
					
					//-----------------------
					if (
				//		( qosConfigV1.iReqTrafficClass != profile.iReqTraffic)  ||
				//		(qosConfigV1.iMinTrafficClass != profile.iMinTraffic)||
						(qosConfigV1.iReqDeliveryOrderReqd != profile.iReqDeliveryOrder)||
						(qosConfigV1.iMinDeliveryOrderReqd != profile.iMinDeliveryOrder)||
						(qosConfigV1.iReqDeliverErroneousSDU != profile.iReqErroneousSDU)||
						(qosConfigV1.iMinDeliverErroneousSDU != profile.iMinErroneousSDU)||
 						( (profile.iMaxSDUSize >= 0) && (qosConfigV1.iReqMaxSDUSize != profile.iMaxSDUSize) )||
 						( (profile.iMinSDUSize >= 0) && (qosConfigV1.iMinAcceptableMaxSDUSize != profile.iMinSDUSize) )||
  						( (profile.iReqDownLinkTBitRate >= 0) && (qosConfigV1.iReqMaxRate.iDownlinkRate != profile.iReqDownLinkTBitRate) )||
  						( (profile.iReqUpLinkTBitRate >= 0) && (qosConfigV1.iReqMaxRate.iUplinkRate != profile.iReqUpLinkTBitRate) )||
						(qosConfigV1.iMinGuaranteedRate.iDownlinkRate != profile.iMinGuaranteedDownLinkTBitRate)||
						(qosConfigV1.iMinGuaranteedRate.iUplinkRate != profile.iMinGuaranteedUpLinkTBitRate)||
						(qosConfigV1.iReqBER != profile.iReqTBitErrorRatio) ||
						(qosConfigV1.iMaxBER != profile.iMinTBitErrorRatio) ||
						(qosConfigV1.iReqSDUErrorRatio != profile.iReqTSDUErrorRatio) ||
						(qosConfigV1.iMaxSDUErrorRatio != profile.iMinTSDUErrorRatio) ||
						(qosConfigV1.iReqTrafficHandlingPriority != profile.iReqTTrafficHandlingPriority) ||
						(qosConfigV1.iMinTrafficHandlingPriority != profile.iMinTTrafficHandlingPriority) ||
						(qosConfigV1.iReqTransferDelay != profile.iReqTransferDelay) ||
						(qosConfigV1.iMaxTransferDelay != profile.iMinTransferDelay) ||
						( (profile.iReqGuaranteedDownLinkTBitRate >= 0) && (qosConfigV1.iReqGuaranteedRate.iDownlinkRate != profile.iReqGuaranteedDownLinkTBitRate) )||
						( (profile.iReqGuaranteedUpLinkTBitRate >= 0) && (qosConfigV1.iReqGuaranteedRate.iUplinkRate != profile.iReqGuaranteedUpLinkTBitRate))||
						(qosConfigV1.iMinGuaranteedRate.iDownlinkRate != profile.iMinGuaranteedDownLinkTBitRate) ||
						(qosConfigV1.iMinGuaranteedRate.iUplinkRate != profile.iMinGuaranteedUpLinkTBitRate)
						)
						{
						err=KErrCorrupt;
						}
					else
						{
						err=KErrNone;
						iSetProfilePending = ETrue;
						iCurrentNeg99=i;
						iCurrentProfileRel = TPacketDataConfigBase::KConfigRel99Rel4;
						if (iNotifyProfileR99.iNotifyPending || iNotifyProfileR5.iNotifyPending)
							{
							CompleteNotifications(iNegR99Profiles->At(iCurrentNeg99));
							}
						
						iUseNetworkNotificationBool = EFalse;
						break;
						}
					}
				else
					{
					err = KErrNotFound;
					}
				}
			ReqCompleted(aTsyReqHandle,err);
			}
	else if (configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigRel5)
		{			
		TPckg<RPacketQoS::TQoSR5Requested>* rel5Config = (TPckg<RPacketQoS::TQoSR5Requested>*)aConfig;
		RPacketQoS::TQoSR5Requested& qosConfigV1 = (*rel5Config)();

		for(TInt i=0; i<iReqR5Profiles->Count(); i++)
			{
			TProfileReqR5 profile = iReqR5Profiles->At(i);

			if (MatchContext(profile)==KErrNone)
				{
				if ((qosConfigV1.iReqTrafficClass != profile.iReqTraffic ||
					(qosConfigV1.iMinTrafficClass != profile.iMinTraffic)||
					(qosConfigV1.iReqDeliveryOrderReqd != profile.iReqDeliveryOrder)||
					(qosConfigV1.iMinDeliveryOrderReqd != profile.iMinDeliveryOrder)||
					(qosConfigV1.iReqDeliverErroneousSDU != profile.iReqErroneousSDU)||
					(qosConfigV1.iMinDeliverErroneousSDU != profile.iMinErroneousSDU)||
					( (profile.iMaxSDUSize >= 0) && (qosConfigV1.iReqMaxSDUSize != profile.iMaxSDUSize) )||
					( (profile.iMinSDUSize >= 0) && (qosConfigV1.iMinAcceptableMaxSDUSize != profile.iMinSDUSize) )||
  					( (profile.iReqDownLinkTBitRate >= 0) && (qosConfigV1.iReqMaxRate.iDownlinkRate != profile.iReqDownLinkTBitRate) )||
  					( (profile.iReqUpLinkTBitRate >= 0) && qosConfigV1.iReqMaxRate.iUplinkRate != profile.iReqUpLinkTBitRate) )||
					(qosConfigV1.iMinGuaranteedRate.iDownlinkRate != profile.iMinGuaranteedDownLinkTBitRate)||
					(qosConfigV1.iMinGuaranteedRate.iUplinkRate != profile.iMinGuaranteedUpLinkTBitRate)||
					(qosConfigV1.iReqBER != profile.iReqTBitErrorRatio) ||
					(qosConfigV1.iMaxBER != profile.iMinTBitErrorRatio) ||
					(qosConfigV1.iReqSDUErrorRatio != profile.iReqTSDUErrorRatio) ||
					(qosConfigV1.iMaxSDUErrorRatio != profile.iMinTSDUErrorRatio) ||
					(qosConfigV1.iReqTrafficHandlingPriority != profile.iReqTTrafficHandlingPriority) ||
					(qosConfigV1.iMinTrafficHandlingPriority != profile.iMinTTrafficHandlingPriority) ||
					(qosConfigV1.iReqTransferDelay != profile.iReqTransferDelay) ||
					(qosConfigV1.iMaxTransferDelay != profile.iMinTransferDelay) ||
					( (profile.iReqGuaranteedDownLinkTBitRate >= 0) && (qosConfigV1.iReqGuaranteedRate.iDownlinkRate != profile.iReqGuaranteedDownLinkTBitRate) )||
					( (profile.iReqGuaranteedUpLinkTBitRate >= 0) && (qosConfigV1.iReqGuaranteedRate.iUplinkRate != profile.iReqGuaranteedUpLinkTBitRate) )||
					(qosConfigV1.iMinGuaranteedRate.iDownlinkRate != profile.iMinGuaranteedDownLinkTBitRate) ||
					(qosConfigV1.iMinGuaranteedRate.iUplinkRate != profile.iMinGuaranteedUpLinkTBitRate) ||
					(qosConfigV1.iSignallingIndication != profile.iSignallingIndication) ||
					(qosConfigV1.iSourceStatisticsDescriptor != profile.iReqTSourceStatisticsDescriptor))
					{
					err=KErrCorrupt;
					}
				else
					{
					err=KErrNone;
					iSetProfilePending = ETrue;
					iCurrentNegR5=i;
					iCurrentProfileRel = TPacketDataConfigBase::KConfigRel5;

					if (iNotifyProfileR99.iNotifyPending || iNotifyProfileR5.iNotifyPending)
						{
						CompleteNotifications(iNegR5Profiles->At(iCurrentNegR5));
						}
					iUseNetworkNotificationBool = EFalse;
					break;
					}
				}
			else
				{
				err = KErrNotFound;
				}
			}
		ReqCompleted(aTsyReqHandle,err);
		}
	return KErrNone;
	}

TInt CSimPacketQoS::SetProfileCancel(const TTsyReqHandle  aTsyReqHandle )
{
/**
* Cancels the Setconfig request.
*
* @param aTsyReqHandle Tsy Request handle for the client request
* @return err KErrNone 
*/
	LOGPACKET1("CSimPacketQoS::SetProfileCancel called");
	
	TInt index  = 0;
	if (TSetQoSData::Find(iSetQoSData, aTsyReqHandle, index) == KErrNone)
		{
		iSetQoSData->Delete(index);
		}
	if (iSetQoSData->Count() == 0)
		{
		iSetQoSTimer->Cancel();
		}
	
	ReqCompleted(aTsyReqHandle, KErrCancel);
	
  	return KErrNone;
  }


TInt CSimPacketQoS::GetProfile(const TTsyReqHandle aTsyReqHandle,TDes8* aConfig)
/**
* Returns to the client the Quality of Service Requested and Minimum Profile Parameters supported.
* 
* In this TSY, Return the profile read from the config file and its up to the client to compare it
* with the values its expecting.
*
* @param aTsyReqHandle Tsy Request handle for the client request
* @param aConfig pointer containing the parameters to Get to.
* @return KErrNone
*/
	{
	LOGPACKET1("CSimPacketQoS::GetProfile called");
	TPckg<TPacketDataConfigBase>* configBase = (TPckg<TPacketDataConfigBase>*)aConfig;
	TPacketDataConfigBase& configBaseV1 = (*configBase)();
	
	// GPRS
	if(configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigGPRS)
		{
		TPckg<RPacketQoS::TQoSGPRSNegotiated>* qosConfigGPRSPckg = (TPckg<RPacketQoS::TQoSGPRSNegotiated>*)aConfig;
		RPacketQoS::TQoSGPRSNegotiated& qosConfigV1 = (*qosConfigGPRSPckg)();
		
		TNegProfileConfig negProfile = iNegProfiles->At(iCurrentNeg++);

		qosConfigV1.iDelay = STATIC_CAST(RPacketQoS::TQoSDelay,negProfile.iNegDelay);
		qosConfigV1.iMeanThroughput = STATIC_CAST(RPacketQoS::TQoSMeanThroughput,negProfile.iNegMeanThroughput);
		qosConfigV1.iPeakThroughput = STATIC_CAST(RPacketQoS::TQoSPeakThroughput,negProfile.iNegPeekThroughput);
		qosConfigV1.iPrecedence = STATIC_CAST(RPacketQoS::TQoSPrecedence, negProfile.iNegPrecedence);
		qosConfigV1.iReliability = STATIC_CAST(RPacketQoS::TQoSReliability, negProfile.iNegReliability);

		ReqCompleted(aTsyReqHandle,KErrNone);
		}
	
	// UMTS
	else if (configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigRel99Rel4)
		{
		TPckg<RPacketQoS::TQoSR99_R4Negotiated>* qosConfigR99 = (TPckg<RPacketQoS::TQoSR99_R4Negotiated>*)aConfig;
		RPacketQoS::TQoSR99_R4Negotiated& qosConfigNegR99 = (*qosConfigR99)();

		// check context names match, if not, find correct one

		if (!iUseNetworkNotificationBool)
			{
			TProfileNegR99* R99NegProfile;
			TInt matchContextErr;
			// profile was set using R5 params, get negociated profile from R5NegProfile
			if (iCurrentProfileRel == TPacketDataConfigBase::KConfigRel5)
				{
				TProfileNegR5& R5NegProfile = iNegR5Profiles->At(iCurrentNegR5);
				matchContextErr = MatchContext(R5NegProfile);
				R99NegProfile = &R5NegProfile;
				}
			else // default is to use the format of the client request 
				{
				R99NegProfile = &iNegR99Profiles->At(iCurrentNeg99);
				matchContextErr = MatchContext(*R99NegProfile);
				}

			if (matchContextErr==KErrNone)
				{
				qosConfigNegR99.iMaxSDUSize = R99NegProfile->iNegMaxSDUSize;
				qosConfigNegR99.iTransferDelay = R99NegProfile->iNegTransferDelay;
				qosConfigNegR99.iTrafficClass = STATIC_CAST(RPacketQoS::TTrafficClass, R99NegProfile->iNegTraffic);
				qosConfigNegR99.iDeliveryOrderReqd = STATIC_CAST(RPacketQoS::TDeliveryOrder, R99NegProfile->iNegDeliveryOrder);
				qosConfigNegR99.iDeliverErroneousSDU = STATIC_CAST(RPacketQoS::TErroneousSDUDelivery, R99NegProfile->iNegErroneousSDUDelivery);
				qosConfigNegR99.iTrafficHandlingPriority = STATIC_CAST(RPacketQoS::TTrafficHandlingPriority, R99NegProfile->iNegTrafficHandlingPriority);
				qosConfigNegR99.iSDUErrorRatio = STATIC_CAST(RPacketQoS::TSDUErrorRatio, R99NegProfile->iNegTSDUErrorRatio);
				qosConfigNegR99.iBER = STATIC_CAST(RPacketQoS::TBitErrorRatio, R99NegProfile->iNegTBitErrorRatio);
				qosConfigNegR99.iGuaranteedRate.iDownlinkRate = R99NegProfile->iNegGuaranteedDownLinkBitRate;
				qosConfigNegR99.iGuaranteedRate.iUplinkRate = R99NegProfile->iNegGuaranteedUpLinkBitRate;
				qosConfigNegR99.iMaxRate.iDownlinkRate = R99NegProfile->iNegMaxDownLinkRate;
				qosConfigNegR99.iMaxRate.iUplinkRate = R99NegProfile->iNegMaxUpLinkRate;

				ReqCompleted(aTsyReqHandle,KErrNone);
				}
			else
				{
				ReqCompleted(aTsyReqHandle,KErrNotFound);
				}
			}
		else
			{
			TProfileNegR99* R99NegProfile;
			TInt matchContextErr;
			if (iCurrentProfileRel == TPacketDataConfigBase::KConfigRel5)
				{
				TProfileNegR5& R5NegProfile = iQosR5NetworkNotifications->At(iQosR5NetworkNotificationCount);
				matchContextErr = MatchContext(R5NegProfile);
				R99NegProfile = &R5NegProfile;
				}
			else // default is to use the format of the client request 
				{
				R99NegProfile = &iQosNetworkNotifications->At(iQosNetworkNotificationCount);
				matchContextErr = MatchContext(*R99NegProfile);
				}

			if (matchContextErr==KErrNone)
				{
				qosConfigNegR99.iMaxSDUSize = R99NegProfile->iNegMaxSDUSize;
				qosConfigNegR99.iTransferDelay = R99NegProfile->iNegTransferDelay;
				qosConfigNegR99.iTrafficClass = STATIC_CAST(RPacketQoS::TTrafficClass, R99NegProfile->iNegTraffic);
				qosConfigNegR99.iDeliveryOrderReqd = STATIC_CAST(RPacketQoS::TDeliveryOrder, R99NegProfile->iNegDeliveryOrder);
				qosConfigNegR99.iDeliverErroneousSDU = STATIC_CAST(RPacketQoS::TErroneousSDUDelivery, R99NegProfile->iNegErroneousSDUDelivery);
				qosConfigNegR99.iTrafficHandlingPriority = STATIC_CAST(RPacketQoS::TTrafficHandlingPriority, R99NegProfile->iNegTrafficHandlingPriority);
				qosConfigNegR99.iSDUErrorRatio = STATIC_CAST(RPacketQoS::TSDUErrorRatio, R99NegProfile->iNegTSDUErrorRatio);
				qosConfigNegR99.iBER = STATIC_CAST(RPacketQoS::TBitErrorRatio, R99NegProfile->iNegTBitErrorRatio);
				qosConfigNegR99.iGuaranteedRate.iDownlinkRate = R99NegProfile->iNegGuaranteedDownLinkBitRate;
				qosConfigNegR99.iGuaranteedRate.iUplinkRate = R99NegProfile->iNegGuaranteedUpLinkBitRate;
				qosConfigNegR99.iMaxRate.iDownlinkRate = R99NegProfile->iNegMaxDownLinkRate;
				qosConfigNegR99.iMaxRate.iUplinkRate = R99NegProfile->iNegMaxUpLinkRate;

				ReqCompleted(aTsyReqHandle,KErrNone);
				}
			else
				{
				ReqCompleted(aTsyReqHandle,KErrNotFound);
				}
			}
		}
	else if (configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigRel5)
		{
		TPckg<RPacketQoS::TQoSR5Negotiated>* qosConfigR5 = (TPckg<RPacketQoS::TQoSR5Negotiated>*)aConfig;
		RPacketQoS::TQoSR5Negotiated& qosConfigNegR5 = (*qosConfigR5)();

		// check context names match, if not, find correct one

		if (!iUseNetworkNotificationBool)
			{
			TProfileNegR5* R5NegProfile;
			TProfileNegR99* R99NegProfile;
			TInt matchContextErr;

			// profile was set using R99/R4 params, get negociated profile from R99NegProfile
			if (iCurrentProfileRel == TPacketDataConfigBase::KConfigRel99Rel4)
				{
				R99NegProfile = &iNegR99Profiles->At(iCurrentNeg99);
				matchContextErr = MatchContext(*R99NegProfile);
				R5NegProfile = NULL;
				}
			else // default is to use the format of the client request 
				{
				R5NegProfile = &iNegR5Profiles->At(iCurrentNegR5);
				matchContextErr = MatchContext(*R5NegProfile);
				// R99 profile is the same as R5
				R99NegProfile = R5NegProfile;
				}

			if (matchContextErr==KErrNone)
				{
				qosConfigNegR5.iMaxSDUSize = R99NegProfile->iNegMaxSDUSize;
				qosConfigNegR5.iTransferDelay = R99NegProfile->iNegTransferDelay;
				qosConfigNegR5.iTrafficClass = STATIC_CAST(RPacketQoS::TTrafficClass, R99NegProfile->iNegTraffic);
				qosConfigNegR5.iDeliveryOrderReqd = STATIC_CAST(RPacketQoS::TDeliveryOrder, R99NegProfile->iNegDeliveryOrder);
				qosConfigNegR5.iDeliverErroneousSDU = STATIC_CAST(RPacketQoS::TErroneousSDUDelivery, R99NegProfile->iNegErroneousSDUDelivery);
				qosConfigNegR5.iTrafficHandlingPriority = STATIC_CAST(RPacketQoS::TTrafficHandlingPriority, R99NegProfile->iNegTrafficHandlingPriority);
				qosConfigNegR5.iSDUErrorRatio = STATIC_CAST(RPacketQoS::TSDUErrorRatio, R99NegProfile->iNegTSDUErrorRatio);
				qosConfigNegR5.iBER = STATIC_CAST(RPacketQoS::TBitErrorRatio, R99NegProfile->iNegTBitErrorRatio);
				qosConfigNegR5.iGuaranteedRate.iDownlinkRate = R99NegProfile->iNegGuaranteedDownLinkBitRate;
				qosConfigNegR5.iGuaranteedRate.iUplinkRate = R99NegProfile->iNegGuaranteedUpLinkBitRate;
				qosConfigNegR5.iMaxRate.iDownlinkRate = R99NegProfile->iNegMaxDownLinkRate;
				qosConfigNegR5.iMaxRate.iUplinkRate = R99NegProfile->iNegMaxUpLinkRate;
				if (R5NegProfile != NULL)
					{
					qosConfigNegR5.iSignallingIndication = R5NegProfile->iSignallingIndication;
					qosConfigNegR5.iSourceStatisticsDescriptor = STATIC_CAST(RPacketQoS::TSourceStatisticsDescriptor, R5NegProfile->iNegTSourceStatisticsDescriptor);
					}
				else
					{
					qosConfigNegR5.iSignallingIndication = EFalse;
					qosConfigNegR5.iSourceStatisticsDescriptor = RPacketQoS::ESourceStatisticsDescriptorUnknown;
					}
				
				ReqCompleted(aTsyReqHandle,KErrNone);
				}
			else
				{
				ReqCompleted(aTsyReqHandle,KErrNotFound);
				}
			}
		else
			{
			TProfileNegR5* R5NegProfile;
			TProfileNegR99* R99NegProfile;
			TInt matchContextErr;

			// profile was set using R99/R4 params, get negociated profile from R99NegProfile
			if (iCurrentProfileRel == TPacketDataConfigBase::KConfigRel99Rel4)
				{
				R99NegProfile = &iQosNetworkNotifications->At(iQosNetworkNotificationCount);
				matchContextErr = MatchContext(*R99NegProfile);
				R5NegProfile = NULL;
				}
			else // default is to use the format of the client request 
				{
				R5NegProfile = &iQosR5NetworkNotifications->At(iQosR5NetworkNotificationCount);
				matchContextErr = MatchContext(*R5NegProfile);
				// R99 profile is the same as R5
				R99NegProfile = R5NegProfile;
				}

			if (matchContextErr==KErrNone)
				{
				qosConfigNegR5.iMaxSDUSize = R99NegProfile->iNegMaxSDUSize;
				qosConfigNegR5.iTransferDelay = R99NegProfile->iNegTransferDelay;
				qosConfigNegR5.iTrafficClass = STATIC_CAST(RPacketQoS::TTrafficClass, R99NegProfile->iNegTraffic);
				qosConfigNegR5.iDeliveryOrderReqd = STATIC_CAST(RPacketQoS::TDeliveryOrder, R99NegProfile->iNegDeliveryOrder);
				qosConfigNegR5.iDeliverErroneousSDU = STATIC_CAST(RPacketQoS::TErroneousSDUDelivery, R99NegProfile->iNegErroneousSDUDelivery);
				qosConfigNegR5.iTrafficHandlingPriority = STATIC_CAST(RPacketQoS::TTrafficHandlingPriority, R99NegProfile->iNegTrafficHandlingPriority);
				qosConfigNegR5.iSDUErrorRatio = STATIC_CAST(RPacketQoS::TSDUErrorRatio, R99NegProfile->iNegTSDUErrorRatio);
				qosConfigNegR5.iBER = STATIC_CAST(RPacketQoS::TBitErrorRatio, R99NegProfile->iNegTBitErrorRatio);
				qosConfigNegR5.iGuaranteedRate.iDownlinkRate = R99NegProfile->iNegGuaranteedDownLinkBitRate;
				qosConfigNegR5.iGuaranteedRate.iUplinkRate = R99NegProfile->iNegGuaranteedUpLinkBitRate;
				qosConfigNegR5.iMaxRate.iDownlinkRate = R99NegProfile->iNegMaxDownLinkRate;
				qosConfigNegR5.iMaxRate.iUplinkRate = R99NegProfile->iNegMaxUpLinkRate;

				if (R5NegProfile != NULL)
					{
					qosConfigNegR5.iSignallingIndication = R5NegProfile->iSignallingIndication;
					qosConfigNegR5.iSourceStatisticsDescriptor = STATIC_CAST(RPacketQoS::TSourceStatisticsDescriptor, R5NegProfile->iNegTSourceStatisticsDescriptor);
					}
				else
					{
					qosConfigNegR5.iSignallingIndication = EFalse;
					qosConfigNegR5.iSourceStatisticsDescriptor = RPacketQoS::ESourceStatisticsDescriptorUnknown;
					}

				ReqCompleted(aTsyReqHandle,KErrNone);
				}
			else
				{
				ReqCompleted(aTsyReqHandle,KErrNotFound);
				}
			}
		}
	return KErrNone; 
	}

TInt CSimPacketQoS::GetProfileCancel(const TTsyReqHandle /*aTsyReqHandle*/)
/**
* Cancels the GetProfile request.  This method is not supported in this version of the TSY.
*
* @param aTsyReqHandle Tsy Request handle for the client cancel request
* @return KErrNone
*/
	{
	LOGPACKET1("CSimPacketQoS::GetProfileCancel called");
	return KErrNone;
	}

TInt CSimPacketQoS::GetProfileCaps(const TTsyReqHandle aTsyReqHandle,TDes8* aConfig)
/**
* Returns to the client the capabilities of the TSY.
* 
* In this TSY, Return the profile read from the config file and its up to the client to compare it
* with the values its expecting.
*
* @param aTsyReqHandle Tsy Request handle for the client request
* @param aConfig pointer containing the parameters to Get to.
* @return KErrNone
*/
	{
	LOGPACKET1("CSimPacketQoS::GetProfileCaps called");
	TPckg<TPacketDataConfigBase>* configBase = (TPckg<TPacketDataConfigBase>*)aConfig;
	TPacketDataConfigBase& configBaseV1 = (*configBase)();

	// GPRS
	if(configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigGPRS)
		{
		TPckg<RPacketQoS::TQoSCapsGPRS>* qosCapsGPRSPckg = (TPckg<RPacketQoS::TQoSCapsGPRS>*)aConfig;
		RPacketQoS::TQoSCapsGPRS& qosCaps = (*qosCapsGPRSPckg)();

		qosCaps.iDelay = STATIC_CAST(RPacketQoS::TQoSDelay,iCapsGPRS.iDelay);
		qosCaps.iMean = STATIC_CAST(RPacketQoS::TQoSMeanThroughput,iCapsGPRS.iMean);
		qosCaps.iPeak = STATIC_CAST(RPacketQoS::TQoSPeakThroughput,iCapsGPRS.iPeak);
		qosCaps.iPrecedence = STATIC_CAST(RPacketQoS::TQoSPrecedence, iCapsGPRS.iPrecedence);
		qosCaps.iReliability = STATIC_CAST(RPacketQoS::TQoSReliability, iCapsGPRS.iReliability);
		ReqCompleted(aTsyReqHandle,KErrNone);
		}
	
	// UMTS
	else if (configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigRel99Rel4)
		{
		TPckg<RPacketQoS::TQoSCapsR99_R4>* qosCapsR99Pckg = (TPckg<RPacketQoS::TQoSCapsR99_R4>*)aConfig;
		RPacketQoS::TQoSCapsR99_R4& r99Caps = (*qosCapsR99Pckg)();

		r99Caps.iBER = iCapsR99.iBER;
		r99Caps.iDeliverErroneousSDU = iCapsR99.iDeliverErroneousSDU;
		r99Caps.iDeliveryOrderReqd = iCapsR99.iDeliveryOrderReq;
		r99Caps.iSDUErrorRatio = iCapsR99.iSDUErrorRatio;
		r99Caps.iTrafficClass = iCapsR99.iTraffic;
		r99Caps.iTrafficHandlingPriority = iCapsR99.iTrafficHandlingPriority;
		ReqCompleted(aTsyReqHandle,KErrNone);
		}
	else if (configBaseV1.ExtensionId()==TPacketDataConfigBase::KConfigRel5)
		{
		TPckg<RPacketQoS::TQoSCapsR5>* qosCapsR5Pckg = (TPckg<RPacketQoS::TQoSCapsR5>*)aConfig;
		RPacketQoS::TQoSCapsR5& r5Caps = (*qosCapsR5Pckg)();
		r5Caps.iBER = iCapsR5.iBER;
		r5Caps.iDeliverErroneousSDU = iCapsR5.iDeliverErroneousSDU;
		r5Caps.iDeliveryOrderReqd = iCapsR5.iDeliveryOrderReq;
		r5Caps.iSDUErrorRatio = iCapsR5.iSDUErrorRatio;
		r5Caps.iTrafficClass = iCapsR5.iTraffic;
		r5Caps.iTrafficHandlingPriority = iCapsR5.iTrafficHandlingPriority;
		r5Caps.iSignallingIndication = iCapsR5.iSignallingIndication;
		r5Caps.iSourceStatisticsDescriptor = iCapsR5.iNegTSourceStatisticsDescriptor;
		ReqCompleted(aTsyReqHandle,KErrNone);
		}


	return KErrNone;
	}

TInt CSimPacketQoS::GetProfileCapsCancel(const TTsyReqHandle /*aTsyReqHandle*/)
/**
* Cancels the GetProfileCaps request.  This method is not supported in this version of the TSY.
*
* @param aTsyReqHandle Tsy Request handle for the client cancel request
* @return KErrNone
*/
	{
	LOGPACKET1("CSimPacketQoS::GetProfileCapsCancel called");
	return KErrNone;
	}

TInt CSimPacketQoS::NotifyProfileChanged(const TTsyReqHandle aTsyReqHandle, TDes8* aConfig)
/**
* Records a client interest in being notified of a change in the QoS Profile.
*
* @param aTsyReqHandle Tsy Request handle for the client request
* @param aConfig pointer to the profile to store the profile info when the profile changes.
* @return KErrNone
*/
{
	TPckg<TPacketDataConfigBase>* configBase = (TPckg<TPacketDataConfigBase>*)aConfig;
	TPacketDataConfigBase& configBaseV1 = (*configBase)();
	

	if (configBaseV1.ExtensionId() == TPacketDataConfigBase::KConfigGPRS)
		{
		__ASSERT_ALWAYS(!iNotifyProfileGPRS.iNotifyPending,SimPanic(ENotificationAlreadyPending));
		iNotifyProfileGPRS.iNotifyPending = ETrue;
		iNotifyProfileGPRS.iNotifyHandle = aTsyReqHandle;
		iNotifyProfileGPRS.iNotifyData = &configBaseV1;
		}
	
	else if (configBaseV1.ExtensionId() == TPacketDataConfigBase::KConfigRel99Rel4)
		{
		__ASSERT_ALWAYS(!iNotifyProfileR99.iNotifyPending,SimPanic(ENotificationAlreadyPending));
		iNotifyProfileR99.iNotifyPending = ETrue;
		iNotifyProfileR99.iNotifyHandle = aTsyReqHandle;
		iNotifyProfileR99.iNotifyData = &configBaseV1;
		// start network events only if the release format used to set this QoS profile is R99_R4
		if (!iUseNetworkNotificationBool && iNetworkNotificationType == ETimer && iCurrentProfileRel == TPacketDataConfigBase::KConfigRel99Rel4)
			{
			TInt count=iQosNetworkNotifications->Count();

			for(TInt i = 0; i<count; i++)
				{
				TProfileNegR99 qos = (TProfileNegR99)iQosNetworkNotifications->At(i);
				if(qos.iContextName.Compare(iPacketContext->ContextName()) == 0)
					{	
					if (iTimerStartedBool)
						{
						delete iTimer;
						iTimer = NULL;
						TRAP_IGNORE(iTimer=CSimTimer::NewL(iPhone));	
						}
					iUseNetworkNotificationBool = ETrue;
					iQosNetworkNotificationCount = i;
					iTimerStartedBool=ETrue;

					if (iTimer != NULL)
						iTimer->Start(qos.iDuration, this, ETimerIdPcktQosNetWork);
					break;
					}
				}
			}			
		}
	else if (configBaseV1.ExtensionId() == TPacketDataConfigBase::KConfigRel5)
		{
		__ASSERT_ALWAYS(!iNotifyProfileR5.iNotifyPending,SimPanic(ENotificationAlreadyPending));
		iNotifyProfileR5.iNotifyPending = ETrue;
		iNotifyProfileR5.iNotifyHandle = aTsyReqHandle;
		iNotifyProfileR5.iNotifyData = &configBaseV1;
		// start network events only if the release format used to set this QoS profile is R5
		if (!iUseNetworkNotificationBool && iNetworkNotificationType == ETimer && iCurrentProfileRel == TPacketDataConfigBase::KConfigRel5)
			{
			TInt count=iQosR5NetworkNotifications->Count();

			for(TInt i = 0; i<count; i++)
				{
				TProfileNegR5 qos = (TProfileNegR5)iQosR5NetworkNotifications->At(i);
				if(qos.iContextName.Compare(iPacketContext->ContextName()) == 0)
					{
					if (iTimerStartedBool)
						{
						delete iTimer;
						iTimer = NULL;
						TRAP_IGNORE(iTimer=CSimTimer::NewL(iPhone));	
						}
					iUseNetworkNotificationBool = ETrue;
					iQosR5NetworkNotificationCount = i;
					iTimerStartedBool=ETrue;

					if (iTimer != NULL)
						iTimer->Start(qos.iDuration, this, ETimerIdPcktQosR5Network);
					break;
					}
				}
			}			
		}
	return KErrNone;
}

TInt CSimPacketQoS::NotifyProfileChangedCancel(const TTsyReqHandle aTsyReqHandle)
/**
* Cancel a client's interest in being notified when the profile configuration changes
* This is acheived simply by resetting the flag that indicates a notification is pending.
* 
* @param aTsyReqHandle Tsy Request handle for the client cancel request
* @return KErrNone
*/
{
	LOGPACKET1("CSimPacketQoS::NotifyProfileChangedCancel called");
	if(iNotifyProfileGPRS.iNotifyPending &&	iNotifyProfileGPRS.iNotifyHandle == aTsyReqHandle)
		{
		iNotifyProfileGPRS.iNotifyPending=EFalse;
		ReqCompleted(aTsyReqHandle,KErrCancel);
		}
	
	else if(iNotifyProfileR99.iNotifyPending &&	iNotifyProfileR99.iNotifyHandle == aTsyReqHandle)
		{
		iNotifyProfileR99.iNotifyPending=EFalse;
		if (iTimer != NULL)
			iTimer->Cancel();
		ReqCompleted(aTsyReqHandle,KErrCancel);
		}
	else if(iNotifyProfileR5.iNotifyPending &&	iNotifyProfileR5.iNotifyHandle == aTsyReqHandle)
		{
		iNotifyProfileR5.iNotifyPending=EFalse;
		if (iTimer != NULL)
			iTimer->Cancel();
		ReqCompleted(aTsyReqHandle,KErrCancel);
		}
	return KErrNone;
}

/**
*	This function matches the Rel99 Release profile to one in the array of Rel99 Release profiles
*/
TInt CSimPacketQoS::MatchContext(TProfileReqR99& aR99ReqProfile)
{
	if (iPacketContext->ContextName().Compare(aR99ReqProfile.iContextName)==0)
		{
		return KErrNone;
		}
	else
		{
		TInt count = iReqR99Profiles->Count();
		for (TInt i = 0; i<count; i++)
			{
			TProfileReqR99 profile = iReqR99Profiles->At(i);
			
			if (iPacketContext->ContextName().Compare(profile.iContextName)==0)
				{
				aR99ReqProfile = profile;
				return KErrNone;
				}
			}
		}
	return KErrNotFound;
}


/**
*	This function matches the Rel99 Negotiated profile to one in the array of Rel99 Negotiated profiles
*/
TInt CSimPacketQoS::MatchContext(TProfileNegR99& aR99NegProfile)
{
	if (iPacketContext->ContextName().Compare(aR99NegProfile.iContextName)==0)
		{
		return KErrNone;
		}
	else
		{
		TInt count = iNegR99Profiles->Count();
		for (TInt i = 0; i<count; i++)
			{
			TProfileNegR99 profile = iNegR99Profiles->At(i);
			
			if (iPacketContext->ContextName().Compare(profile.iContextName)==0)
				{
				aR99NegProfile = profile;
				return KErrNone;
				}
			}
		}
	return KErrNotFound;
}

/**
*	This function matches the Rel5 Release profile to one in the array of Rel5 Release profiles
*/
TInt CSimPacketQoS::MatchContext(TProfileReqR5& aR5ReqProfile)
{
	if (iPacketContext->ContextName().Compare(aR5ReqProfile.iContextName)==0)
		{
		return KErrNone;
		}
	else
		{
		TInt count = iReqR5Profiles->Count();
		for (TInt i = 0; i<count; i++)
			{
			TProfileReqR5 profile = iReqR5Profiles->At(i);
			
			if (iPacketContext->ContextName().Compare(profile.iContextName)==0)
				{
				aR5ReqProfile = profile;
				return KErrNone;
				}
			}
		}
	return KErrNotFound;
}


/**
*	This function matches the Rel99 Negotiated profile to one in the array of Rel99 Negotiated profiles
*/
TInt CSimPacketQoS::MatchContext(TProfileNegR5& aR5NegProfile)
{
	if (iPacketContext->ContextName().Compare(aR5NegProfile.iContextName)==0)
		{
		return KErrNone;
		}
	else
		{
		TInt count = iNegR5Profiles->Count();
		for (TInt i = 0; i<count; i++)
			{
			TProfileNegR5 profile = iNegR5Profiles->At(i);
			
			if (iPacketContext->ContextName().Compare(profile.iContextName)==0)
				{
				aR5NegProfile = profile;
				return KErrNone;
				}
			}
		}
	return KErrNotFound;
}

void CSimPacketQoS::ClearChangeQoSPending()
	{
	iSetProfilePending = EFalse;
	}
	
/**
*  Sets the current negotiated profile to the required profile parameters
*/
TInt CSimPacketQoS::PerformProfileChange()
	{
	if (iSetProfilePending)
		{
		iSetProfilePending = EFalse;
		return KErrNone;
		}
	return KErrNotReady;
	}
	
TInt CSimPacketQoS::TSetQoSData::Find(CArrayFixFlat<TSetQoSData>* aSetQoSArray, TTsyReqHandle aTsyReqHandle, TInt& aIndex)
/**
* Finds a qos data set from array using the tsy request handle
*
* @param CArrayFixFlat<TSetQoSData>* aSetQoSArray - Array of qos data stored
* @param TTsyReqHandle aTsyReqHandle  - Tsy request handle for the client
* @param TInt& aIndex - Index of current qos data set
* @return KErrNone or KErrNotFound 
*/

	{
	for (TInt i = 0; i < aSetQoSArray->Count(); i++)
		{
		if (aSetQoSArray->At(i).iTsyReqHandle == aTsyReqHandle)
			{
			aIndex = i;
			return KErrNone;
			}
		}
	aIndex = -1;
	return KErrNotFound;
	}	
