// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implementation file for the Umts/Gprs SubConnection Provider
// 
//

/**
 @file
 @internalComponent
*/



#include <cs_subconevents.h>
#include "UmtsGprs_subconnProv.h"

const TInt KQoSMappingBorderDelay = 250;

#define IN_RANGE_INCLUSIVE(value, left, right) (value >= left && value <= right )
#define MIN(left, right) (left < right ? left : right)

//We need to use a cleanup item here becuase CSubConNotificationEvents are not CBase derived. This means that on cleanup, things are not cleaned up as expected
//We therefore need a TCleanupItem to deal with it.
void SubconEventCleanupFunction(TAny* aEvent)
	{
	CSubConNotificationEvent* ptr = static_cast<CSubConNotificationEvent*>(aEvent);
	delete ptr;
	}

void SubconParamSetCleanupFunction(TAny* aEvent)
	{
	CSubConParameterSet* ptr = static_cast<CSubConParameterSet*>(aEvent);
	delete ptr;
	}
	
#ifdef SYMBIAN_NETWORKING_UMTSR5
void CUmtsGprsSubConnProvd::MapQosParamBundleToEtelL(const CSubConParameterBundle& aIn, 
   RPacketQoS::TQoSR5Requested* aOut)

#else
// !SYMBIAN_NETWORKING_UMTSR5

void CUmtsGprsSubConnProvd::MapQosParamBundleToEtelL(const CSubConParameterBundle& aIn, 
    RPacketQoS::TQoSR99_R4Requested* aOut)
#endif
// SYMBIAN_NETWORKING_UMTSR5
   {
	ASSERT(aOut);

    //Note: There should probably be const and non-const versions of FindFamily. 
    //There is only non-const, hence casting below.
	CSubConParameterFamily* family = const_cast<CSubConParameterBundle&>(aIn).FindFamily(KSubConQoSFamily);
	if (!family)
        {
        User::Leave(KErrArgument);
        }


#ifdef SYMBIAN_NETWORKING_UMTSR5
	// check the UMTS R5 extension first
	CSubConQosR5ParamSet* extRequestedR5 = NULL;
	CSubConQosR5ParamSet* extAcceptableR5 = NULL;
	
	extRequestedR5 = static_cast<CSubConQosR5ParamSet*>(family->FindExtensionSet(
        STypeId::CreateSTypeId(KSubCon3GPPExtParamsFactoryUid,KSubConQosR5ParamsType), CSubConParameterFamily::ERequested));

	extAcceptableR5 = static_cast<CSubConQosR5ParamSet*>(family->FindExtensionSet(
        STypeId::CreateSTypeId(KSubCon3GPPExtParamsFactoryUid,KSubConQosR5ParamsType), CSubConParameterFamily::EAcceptable));

	if (extRequestedR5)
        {
        MapQosR5ExtensionSetToEtel (*extRequestedR5, extAcceptableR5, aOut);
        iParameterRelease = KParameterRel5;
        return;
        }
#endif

	// No R5 so check the UMTS R99/R4 extension next
	CSubConQosR99ParamSet* extRequestedR99 = NULL;
	CSubConQosR99ParamSet* extAcceptableR99 = NULL;
	extRequestedR99 = static_cast<CSubConQosR99ParamSet*>(family->FindExtensionSet(
        STypeId::CreateSTypeId(KSubCon3GPPExtParamsFactoryUid,KSubConQosR99ParamsType), CSubConParameterFamily::ERequested));

	extAcceptableR99 = static_cast<CSubConQosR99ParamSet*>(family->FindExtensionSet(
        STypeId::CreateSTypeId(KSubCon3GPPExtParamsFactoryUid,KSubConQosR99ParamsType), CSubConParameterFamily::EAcceptable));

	if (extRequestedR99)
        {
        MapQosR99ExtensionSetToEtel (*extRequestedR99, extAcceptableR99, aOut);
        iParameterRelease = KParameterRel4Rel99;
        return;
        }
      
      
	// Still nothing specific, try to use the generic params
	CSubConQosGenericParamSet* genericRequested = 
        static_cast<CSubConQosGenericParamSet*>(family->GetGenericSet(CSubConParameterFamily::ERequested));
	CSubConQosGenericParamSet* genericAcceptable = 
        static_cast<CSubConQosGenericParamSet*>(family->GetGenericSet(CSubConParameterFamily::EAcceptable));

    if (genericRequested)
        {
        MapQosGenericSetToEtel (*genericRequested, genericAcceptable, aOut);
        iParameterRelease = KParameterRelGeneric;
        return;
        }
    	
    // Nothing left that we support
    User::Leave(KErrNotSupported);
    }

    
/** 
Maps given requested and acceptable sets of R99/R4 parameters as defined CSubConQosR99ParamSet
into the Etel format (RPacketQoS::TQoSR99_R4Requested).

@param aInRequested requested CSubConQosR99ParamSet to be converted
@param aInAcceptable optional acceptable CSubConQosR99ParamSet to be converted
@param aOut RPacketQoS::TQoSR99_R4Requested derived class to be mapped into.
*/
void CUmtsGprsSubConnProvd::MapQosR99ExtensionSetToEtel(const CSubConQosR99ParamSet& aInRequested,
   const CSubConQosR99ParamSet* aInAcceptable, RPacketQoS::TQoSR99_R4Requested* aOut)
   {
   ASSERT (aOut);
   
	aOut->iReqTrafficClass =  aInRequested.GetTrafficClass();
	aOut->iReqDeliveryOrderReqd = aInRequested.GetDeliveryOrder();
	aOut->iReqDeliverErroneousSDU = aInRequested.GetErroneousSDUDelivery();
	aOut->iReqBER = aInRequested.GetResidualBitErrorRatio();
	aOut->iReqSDUErrorRatio = aInRequested.GetSDUErrorRatio();
	aOut->iReqTrafficHandlingPriority = aInRequested.GetTrafficHandlingPriority();
	aOut->iReqTransferDelay = aInRequested.GetTransferDelay();
	aOut->iReqMaxSDUSize = aInRequested.GetMaxSduSize();
	aOut->iReqMaxRate.iUplinkRate = aInRequested.GetMaxBitrateUplink();
	aOut->iReqMaxRate.iDownlinkRate = aInRequested.GetMaxBitrateDownlink();
	aOut->iReqGuaranteedRate.iUplinkRate = aInRequested.GetGuaBitrateUplink();
	aOut->iReqGuaranteedRate.iDownlinkRate = aInRequested.GetGuaBitrateDownlink();
	
	if (!aInAcceptable)
	   {
	   // when aInAcceptable is ommitted the minimum is the requested
	   aInAcceptable = &aInRequested;
	   }
	   
	aOut->iMinTrafficClass = aInAcceptable->GetTrafficClass();
	aOut->iMinDeliveryOrderReqd = aInAcceptable->GetDeliveryOrder();
	aOut->iMinDeliverErroneousSDU = aInAcceptable->GetErroneousSDUDelivery();
	aOut->iMaxBER = aInAcceptable->GetResidualBitErrorRatio();
	aOut->iMaxSDUErrorRatio = aInAcceptable->GetSDUErrorRatio();
	aOut->iMinTrafficHandlingPriority = aInAcceptable->GetTrafficHandlingPriority();
	aOut->iMaxTransferDelay = aInAcceptable->GetTransferDelay();
	aOut->iMinAcceptableMaxSDUSize = aInAcceptable->GetMaxSduSize();
	aOut->iMinAcceptableMaxRate.iUplinkRate = aInAcceptable->GetMaxBitrateUplink();
	aOut->iMinAcceptableMaxRate.iDownlinkRate = aInAcceptable->GetMaxBitrateDownlink();
	aOut->iMinGuaranteedRate.iUplinkRate = aInAcceptable->GetGuaBitrateUplink();
	aOut->iMinGuaranteedRate.iDownlinkRate = aInAcceptable->GetGuaBitrateDownlink();
   }
    
  
#ifdef SYMBIAN_NETWORKING_UMTSR5
/** 
Maps given requested and acceptable sets of R5 parameters as defined CSubConQosR5ParamSet
into the Etel format (RPacketQoS::TQoSR5Requested).


@param aInRequested requested CSubConQosR5ParamSet to be converted
@param aInAcceptable optional acceptable CSubConQosR5ParamSet to be converted
@param aOut RPacketQoS::TQoSR5Requested derived class to be mapped into.
*/
void CUmtsGprsSubConnProvd::MapQosR5ExtensionSetToEtel(const CSubConQosR5ParamSet& aInRequested,
    const CSubConQosR5ParamSet* aInAcceptable, RPacketQoS::TQoSR5Requested* aOut)
    {
    MapQosR99ExtensionSetToEtel(aInRequested,aInAcceptable,aOut);
    aOut->iSourceStatisticsDescriptor =  aInRequested.GetSourceStatisticsDescriptor();
    aOut->iSignallingIndication = aInRequested.GetSignallingIndicator();
    }
    
    
void CUmtsGprsSubConnProvd::MapQosR99ExtensionSetToEtel(const CSubConQosR99ParamSet& aInRequested,
    const CSubConQosR99ParamSet* aInAcceptable, RPacketQoS::TQoSR5Requested* aOut)
    {
    MapQosR99ExtensionSetToEtel(aInRequested, aInAcceptable, static_cast<RPacketQoS::TQoSR99_R4Requested*>(aOut));
    aOut->iSourceStatisticsDescriptor =  RPacketQoS::ESourceStatisticsDescriptorUnknown;
    aOut->iSignallingIndication = EFalse;
    }

#endif 
// SYMBIAN_NETWORKING_UMTSR5   
    
/** 
Given a set of generic parameters, the method derives the R99 value for the traffic class.


@param aIn requested CSubConQosGenericParamSet to be converted
@return the value of RPacketQoS::TTrafficClass
*/	
RPacketQoS::TTrafficClass CUmtsGprsSubConnProvd::DeriveTrafficClass(const CSubConQosGenericParamSet& aIn )
    {
    //-----------------------------------
    // Traffic class
    //-----------------------------------
    if (IN_RANGE_INCLUSIVE(aIn.GetUpLinkDelay(), 0, KQoSMappingBorderDelay-1) ||
        IN_RANGE_INCLUSIVE(aIn.GetDownLinkDelay(), 0, KQoSMappingBorderDelay-1) )
	    {
	    return RPacketQoS::ETrafficClassConversational;
	    }
    else if (aIn.GetUpLinkDelay() >= KQoSMappingBorderDelay || 
    		 aIn.GetDownLinkDelay() >= KQoSMappingBorderDelay )
	    {
	    return RPacketQoS::ETrafficClassStreaming;	
	    }  
    else if (IN_RANGE_INCLUSIVE(aIn.GetUpLinkPriority(), 1, 3) ||
    		 IN_RANGE_INCLUSIVE(aIn.GetDownLinkPriority(), 1, 3))
	    {
	    return RPacketQoS::ETrafficClassInteractive;	
	    }

	return RPacketQoS::ETrafficClassBackground;	
	}
	
	
/** 
Given a set of generic parameters, the method derives the R99 value for the traffic priority.


@param aIn requested CSubConQosGenericParamSet to be converted
@return the value of RPacketQoS::TTrafficHandlingPriority
*/	
RPacketQoS::TTrafficHandlingPriority CUmtsGprsSubConnProvd::DeriveTrafficPriority(const CSubConQosGenericParamSet& aIn )
    {    
    //
    // Traffic handling priority
    //
    if (aIn.GetUpLinkPriority() == 1 || aIn.GetDownLinkPriority() == 1)
	    {
	    return RPacketQoS::ETrafficPriority1;	
	    }
    else if (aIn.GetUpLinkPriority() == 2 || aIn.GetDownLinkPriority() == 2)
	    {
	    return RPacketQoS::ETrafficPriority2;
	    }

	return RPacketQoS::ETrafficPriority3;
    }
	    
    
/** 
Maps given requested and acceptable sets of generic QoS parameters as defined CSubConQosGenericParamSet
into the Etel format (RPacketQoS::TQoSR99_R4Requested).


@param aInRequested requested CSubConQosGenericParamSet to be converted
@param aInAcceptable optional acceptable CSubConQosGenericParamSet to be converted
@param aOut RPacketQoS::TQoSR99_R4Requested to be produced.
*/	
void CUmtsGprsSubConnProvd::MapQosGenericSetToEtel(const CSubConQosGenericParamSet& aInRequested, 
   const CSubConQosGenericParamSet* aInAcceptable, RPacketQoS::TQoSR99_R4Requested* aOut)
	{
	ASSERT (aOut);
	
	aOut->iReqDeliveryOrderReqd = RPacketQoS::EDeliveryOrderNotRequired;
	aOut->iReqDeliverErroneousSDU = RPacketQoS::EErroneousSDUDeliveryNotRequired;;
	aOut->iReqBER = RPacketQoS::EBERUnspecified;
   aOut->iReqSDUErrorRatio = RPacketQoS::ESDUErrorRatioUnspecified;

	aOut->iReqTrafficClass = DeriveTrafficClass(aInRequested);
	aOut->iReqTrafficHandlingPriority = DeriveTrafficPriority(aInRequested);
	aOut->iReqTransferDelay = MIN(aInRequested.GetUpLinkDelay(), aInRequested.GetDownLinkDelay());
	aOut->iReqMaxSDUSize = aInRequested.GetUpLinkMaximumPacketSize();
	aOut->iReqMaxRate.iUplinkRate = aInRequested.GetUplinkBandwidth();
	aOut->iReqMaxRate.iDownlinkRate = aInRequested.GetDownlinkBandwidth();
	aOut->iReqGuaranteedRate.iUplinkRate = aInRequested.GetUplinkBandwidth();
	aOut->iReqGuaranteedRate.iDownlinkRate = aInRequested.GetDownlinkBandwidth();
	
	if (!aInAcceptable)
	   {
	   // when aInAcceptable is ommitted the minimum is the requested
	   aInAcceptable = &aInRequested;
	   }
	
	aOut->iMinDeliveryOrderReqd = RPacketQoS::EDeliveryOrderNotRequired;
	aOut->iMinDeliverErroneousSDU = RPacketQoS::EErroneousSDUDeliveryNotRequired;
	aOut->iMaxBER = RPacketQoS::EBERUnspecified;
	aOut->iMaxSDUErrorRatio = RPacketQoS::ESDUErrorRatioUnspecified;

	aOut->iMinTrafficClass = DeriveTrafficClass(*aInAcceptable);
	aOut->iMinTrafficHandlingPriority = DeriveTrafficPriority(*aInAcceptable);
	aOut->iMaxTransferDelay = MIN(aInAcceptable->GetUpLinkDelay(), aInAcceptable->GetDownLinkDelay());
	aOut->iMinAcceptableMaxSDUSize = aInAcceptable->GetUpLinkMaximumPacketSize();
	aOut->iMinAcceptableMaxRate.iUplinkRate = aInAcceptable->GetUplinkBandwidth();
	aOut->iMinAcceptableMaxRate.iDownlinkRate = aInAcceptable->GetDownlinkBandwidth();
	aOut->iMinGuaranteedRate.iUplinkRate = aInAcceptable->GetUplinkBandwidth();
	aOut->iMinGuaranteedRate.iDownlinkRate = aInAcceptable->GetDownlinkBandwidth();
	}



/**
Maps QoS parameters received from the network via Etel into a CSubConGenEventParamsGranted class
so that it is usable by the RSubConnection API.

@param aNetworkQoS  - A pointer to an Etel TPacketDataConfigBase class containing the
QoS to be mapped into the QoS event.
@param aGranted - A reference to the CSubConGenEventParamsGranted object that should receive the mapped
QoS parameters.
*/
void CUmtsGprsSubConnProvd::MapQosEtelToGrantedParamsL (TPacketDataConfigBase* aNetworkQoS,CSubConGenEventParamsGranted& aGranted)
    {
    ASSERT (aNetworkQoS);
    
    // Always set the generic set
    MapQosEtelToGenericSetL ((static_cast<RPacketQoS::TQoSR99_R4Negotiated*>(aNetworkQoS)), aGranted);


    
    switch (iParameterRelease)
        {
#ifdef SYMBIAN_NETWORKING_UMTSR5
        case KParameterRel5:
            // Control client sent us an R5 qos request
            MapQosEtelToR5SetL ((static_cast<RPacketQoS::TQoSR5Negotiated*>(aNetworkQoS)), aGranted);
            break;
#endif
// SYMBIAN_NETWORKING_UMTSR5
            
        case KParameterRelInvalid:
#ifdef SYMBIAN_NETWORKING_UMTSR5
            // This will be the case if the client hasn't actually requested a level of QoS yet
            // but the network has notified us of a QoS change. We don't know what the client might
            // support so return both sets (if the define is set, else just what the code supports)
            MapQosEtelToR5SetL ((static_cast<RPacketQoS::TQoSR5Negotiated*>(aNetworkQoS)), aGranted);
#endif
            MapQosEtelToR99SetL ((static_cast<RPacketQoS::TQoSR99_R4Negotiated*>(aNetworkQoS)), aGranted);
            break;
            
        case KParameterRel4Rel99:
            // Control client sent us an R4/R99 qos request
            MapQosEtelToR99SetL ((static_cast<RPacketQoS::TQoSR99_R4Negotiated*>(aNetworkQoS)), aGranted);
            break;
        
        case KParameterRelGeneric:
            // Generic set always returned. Done above.
            break;
        }
    }
    

/**
Maps QoS parameters received from the network via Etel into a 3GPP Release R4/R99 set and
sets it in the given event.

@param aNetworkQoS  - A pointer to an Etel RPacketQoS::TQoSR99_R4Requested derived class containing
the QoS to be mapped.
@param aGranted - A reference to the CSubConGenEventParamsGranted object that should receive the
extension QoS parameters.
*/
void CUmtsGprsSubConnProvd::MapQosEtelToR99SetL (RPacketQoS::TQoSR99_R4Negotiated* aNetworkQoS,
   CSubConGenEventParamsGranted& aGranted)
   {
   ASSERT (aNetworkQoS);
   
   CSubConQosR99ParamSet* r99Extension = CSubConQosR99ParamSet::NewL ();
   CleanupStack::PushL(TCleanupItem(SubconParamSetCleanupFunction,r99Extension));
   r99Extension->SetTrafficClass(aNetworkQoS->iTrafficClass);
   r99Extension->SetDeliveryOrder(aNetworkQoS->iDeliveryOrderReqd);
   r99Extension->SetErroneousSDUDelivery(aNetworkQoS->iDeliverErroneousSDU);
   r99Extension->SetResidualBitErrorRatio(aNetworkQoS->iBER);
   r99Extension->SetSDUErrorRatio(aNetworkQoS->iSDUErrorRatio);
   r99Extension->SetTrafficHandlingPriority(aNetworkQoS->iTrafficHandlingPriority);
   r99Extension->SetTransferDelay(aNetworkQoS->iTransferDelay);
   r99Extension->SetMaxSduSize(aNetworkQoS->iMaxSDUSize);
   r99Extension->SetMaxBitrateUplink(aNetworkQoS->iMaxRate.iUplinkRate);
   r99Extension->SetMaxBitrateDownlink(aNetworkQoS->iMaxRate.iDownlinkRate);
   r99Extension->SetGuaBitrateUplink(aNetworkQoS->iGuaranteedRate.iUplinkRate);
   r99Extension->SetGuaBitrateDownlink(aNetworkQoS->iGuaranteedRate.iDownlinkRate);
   
   aGranted.AddExtensionSetL (r99Extension);
   CleanupStack::Pop(); //r99Extension
   }

#ifdef SYMBIAN_NETWORKING_UMTSR5
/**
Maps QoS parameters received from the network via Etel into a 3GPP Release R5 set and
sets it in the given event.

@param aNetworkQoS  - A pointer to an Etel RPacketQoS::TQoSR5Requested derived class containing
the QoS to be mapped.
@param aGranted - A reference to the CSubConGenEventParamsGranted object that should receive the
extension QoS parameters.
*/
void CUmtsGprsSubConnProvd::MapQosEtelToR5SetL (RPacketQoS::TQoSR5Negotiated* aNetworkQoS,
   CSubConGenEventParamsGranted& aGranted)
   {
   ASSERT (aNetworkQoS);
   
   CSubConQosR5ParamSet* r5Extension = CSubConQosR5ParamSet::NewL ();
   CleanupStack::PushL(TCleanupItem(SubconParamSetCleanupFunction,r5Extension));
   r5Extension->SetTrafficClass(aNetworkQoS->iTrafficClass);
   r5Extension->SetDeliveryOrder(aNetworkQoS->iDeliveryOrderReqd);
   r5Extension->SetErroneousSDUDelivery(aNetworkQoS->iDeliverErroneousSDU);
   r5Extension->SetResidualBitErrorRatio(aNetworkQoS->iBER);
   r5Extension->SetSDUErrorRatio(aNetworkQoS->iSDUErrorRatio);
   r5Extension->SetTrafficHandlingPriority(aNetworkQoS->iTrafficHandlingPriority);
   r5Extension->SetTransferDelay(aNetworkQoS->iTransferDelay);
   r5Extension->SetMaxSduSize(aNetworkQoS->iMaxSDUSize);
   r5Extension->SetMaxBitrateUplink(aNetworkQoS->iMaxRate.iUplinkRate);
   r5Extension->SetMaxBitrateDownlink(aNetworkQoS->iMaxRate.iDownlinkRate);
   r5Extension->SetGuaBitrateUplink(aNetworkQoS->iGuaranteedRate.iUplinkRate);
   r5Extension->SetGuaBitrateDownlink(aNetworkQoS->iGuaranteedRate.iDownlinkRate);

   r5Extension->SetSourceStatisticsDescriptor(aNetworkQoS->iSourceStatisticsDescriptor);
   r5Extension->SetSignallingIndicator(aNetworkQoS->iSignallingIndication);		
   
   aGranted.AddExtensionSetL (r5Extension);
   CleanupStack::Pop(); //r5Extension
   }

    
#endif 
// SYMBIAN_NETWORKING_UMTSR5 


/**
Maps QoS parameters received from the network via Etel into a generic set and sets it in the given event.

@param aNetworkQoS  - A pointer to an Etel RPacketQoS::TQoSR99_R4Requested derived class containing the QoS
to be mapped.
@param aFamily - A reference to the CSubConGenEventParamsGranted object that should receive the
generic QoS parameters.
*/
void CUmtsGprsSubConnProvd::MapQosEtelToGenericSetL (RPacketQoS::TQoSR99_R4Negotiated* aNetworkQoS,
   CSubConGenEventParamsGranted& aGranted)
   {
   ASSERT (aNetworkQoS);
   
   CSubConQosGenericParamSet* genericQoS = CSubConQosGenericParamSet::NewL ();
   
   genericQoS->SetDownlinkBandwidth (aNetworkQoS->iGuaranteedRate.iDownlinkRate);
   genericQoS->SetUplinkBandwidth (aNetworkQoS->iGuaranteedRate.iUplinkRate);
   genericQoS->SetDownLinkDelay (aNetworkQoS->iTransferDelay);
   genericQoS->SetUpLinkDelay (aNetworkQoS->iTransferDelay);
   genericQoS->SetDownLinkMaximumPacketSize (aNetworkQoS->iMaxSDUSize);
   genericQoS->SetUpLinkMaximumPacketSize (aNetworkQoS->iMaxSDUSize);
   
   switch (aNetworkQoS->iTrafficHandlingPriority) 
      {
         case RPacketQoS::ETrafficPriority1:
            genericQoS->SetDownLinkPriority (RPacketQoS::ETrafficPriority1);
            genericQoS->SetUpLinkPriority (RPacketQoS::ETrafficPriority1);
            break;

         case RPacketQoS::ETrafficPriority2:
            genericQoS->SetDownLinkPriority (RPacketQoS::ETrafficPriority2);
            genericQoS->SetUpLinkPriority (RPacketQoS::ETrafficPriority2);
            break;

         case RPacketQoS::ETrafficPriority3:
            genericQoS->SetDownLinkPriority (RPacketQoS::ETrafficPriority3);
            genericQoS->SetUpLinkPriority (RPacketQoS::ETrafficPriority3);
            break;
            
         default:
            genericQoS->SetDownLinkPriority (RPacketQoS::ETrafficPriorityUnspecified);
            genericQoS->SetUpLinkPriority (RPacketQoS::ETrafficPriorityUnspecified);
            break;
      }
      
   aGranted.SetGenericSet (genericQoS);
   }
   

   
CPdpFsmInterface& CUmtsGprsSubConnProvd::GetPdpFsmInterface()
    {
    return GetDefaultSubConnection().GetPdpFsmInterface();
    }


TBool CUmtsGprsSubConnProvd::PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam)
    {
    if (iCurrentPdpFsmReq)
        {
        TBool handled(EFalse);
        handled = iCurrentPdpFsmReq->PdpFsmEvent(aContextId, aEvent, aParam);
        return handled;
        }

	// Any events that are not a result of a request that we've made must be forwarded to
	// control clients.

	switch(aEvent)
		{
		case KContextParametersChangeEvent:
	    // For QoS Changed the event must be processed here, it doesn't occur as a response
    	// to something we've requested.
        __FLOG_2(_L("CUmtsGprsSubConnProvd [this=%08x]:\tPdpFsmEvent(): QoS Params Changed [aParam=%d]"), this, aParam);
        TRAP_IGNORE(NotifyParamsChangedL(aParam));
		break;
		
		case KContextDeleteEvent:
		// For network-initiate close of PDP context.
        __FLOG_2(_L("CUmtsGprsSubConnProvd [this=%08x]:\tPdpFsmEvent(): Context Delete Event [aParam=%d]"), this, aParam);
        TRAP_IGNORE(NotifySubConDownL(aParam));
		break;
		
		case KContextBlockedEvent:		// A PDP context becomes blocked
		case KContextUnblockedEvent:	// A PDP context becomes unblocked
		case KNetworkStatusEvent:		// For different network indications	
		case KContextQoSSetEvent:		// Response to KContextQoSSet
		case KContextTFTModifiedEvent:	// Response to KContextTFTModify
		case KPrimaryContextCreated:	// Primary context at startup created
		case KSecondaryContextCreated:	// Secondary context created
		case KContextModifyActiveEvent:	// Response to KModifyActive
		case KGetNegQoSEvent:			// Response to KGetNegQoS. ** Used internally by NIF **
		default:
		// Do nothing.
		break;
		}

    // Although we have notified the change of qos to the client application we haven't handled this
    // event. We can get here because the network changed the QoS without a request from us, or
    // because a secondary context (actually owned by GuQoS) has been promoted to default. 
    return EFalse;
    }
    
void CUmtsGprsSubConnProvd::NotifySubConDownL(TInt aError)
	{
   	CSubConGenEventSubConDown* event = CSubConGenEventSubConDown::NewL();
	CleanupStack::PushL(TCleanupItem(SubconEventCleanupFunction, event));
	event->SetError(aError);
	NotifyClientEvent(*event);
	CleanupStack::PopAndDestroy (); //event
	}


void CUmtsGprsSubConnProvd::NotifyParamsChangedL(TInt aError)
    {
    CSubConGenEventParamsChanged* event = CSubConGenEventParamsChanged::NewL();
    CleanupStack::PushL(TCleanupItem(SubconEventCleanupFunction, event));
    event->SetFamily (KSubConQoSFamily);
    event->SetError (aError);

#ifdef SYMBIAN_NETWORKING_UMTSR5  
	RPacketQoS::TQoSR5Negotiated negotiatedQoS;
#else
    RPacketQoS::TQoSR99_R4Negotiated negotiatedQoS;
#endif 

    GetPdpFsmInterface().Get(GetContextId(), negotiatedQoS);
    MapQosEtelToGrantedParamsL(&negotiatedQoS, *event);
	NotifyClientEvent (*event);
	CleanupStack::PopAndDestroy (); //event
    }

    
MConnectionDataClient* CUmtsGprsSubConnProvd::DoSelfConnectionDataClient()
    {
    return this;
    }    


CPdpFsmRequestBase::CPdpFsmRequestBase(CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner)
   :iProvider(aProvider), iOwner(aOwner)
   {
   __FLOG_OPEN(KUmtsGprsLogTag1, KUmtsGprsLogTag2);     
   }


CSubConnectionProviderBase* CUmtsGprsSubConnProvd::DoNextLayer()
    {
	__FLOG_1(_L("CUmtsGprsSubConnProvd::DoNextLayer [this=%08x] - this is (by design) the last layer"), this);
	return NULL;
    }


//-=========================================================
//
// Ignored overrides.
//
//-=========================================================	



/** Convert the given parameter budle to Etel language (r99) and if at the time possible, submit the request SPUDFSM.
@param 
*/
void CUmtsGprsSubConnProvd::DoParametersAboutToBeSetL(CSubConParameterBundle& /*aParameterBundle*/)
    {
	__FLOG_1(_L("CUmtsGprsSubConnProvd::DoParametersAboutToBeSetL [this=%08x]"), this);

	iParameterRelease = KParameterRelInvalid;
    //TODO: implement set qos on secondaries when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }


CConnDataTransfer& CUmtsGprsSubConnProvd::DoDataTransferL()
    {
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.    
	User::Leave(KErrNotSupported);
	return iNextLayer->DataTransferL();    
    }


    
/**
From MConnectionDataClient
*/
void CUmtsGprsSubConnProvd::Notify(TNotify /*aNotifyType*/, CConnectionProviderBase* /*aConnProvider*/, 
                                   TInt /*aError*/, const CConNotificationEvent* /*aConNotificationEvent*/)
    {   
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }

/**
From MConnectionDataClient
*/
void CUmtsGprsSubConnProvd::AttachToNext(CSubConnectionProviderBase* /*aSubConnProvider*/)
    {
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }
    
/**
From MConnectionDataClient
*/
void CUmtsGprsSubConnProvd::ConnectionGoingDown(CConnectionProviderBase& aConnProvider)
   {
   (void)aConnProvider;
	__FLOG_2(_L("CUmtsGprsSubConnProvd [this=%08x]:\tConnectionGoingDown() [aConnProvider=%08x]"), this, &aConnProvider);
	iConnectionProvider = NULL;
	DeleteMeNow();
   }

/**
From MConnectionDataClient
*/
void CUmtsGprsSubConnProvd::ConnectionError(TInt /*aStage*/, TInt /*aError*/)
    {   
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }
    
/**
Function called by Connection Provider when a socket is to be added to the QoS Flow ('this')

@param aDataClient Data Client to add to the QoS Channel
*/
void CUmtsGprsSubConnProvd::DoDataClientJoiningL(MSubConnectionDataClient& aDataClient)
    {
    (void)aDataClient;    
	__FLOG_3(_L("CUmtsGprsSubConnProvd [this=%08x]:\tDoDataClientJoiningL() [iDataClients.Count=%d] [aDataClient=%08x]"), this, iDataClients.Count(), &aDataClient);
	//We're not interested in knowing about MSubConnectionDataClient joining this sub-connection
    }
    
/**
Function called by Connection Provider when a socket is to be removed from the QoS Flow ('this')

@param aDataClient Data Client to remove from this QoS Channel
*/
void CUmtsGprsSubConnProvd::DoDataClientLeaving(MSubConnectionDataClient& aDataClient)
    {
    (void)aDataClient;
	__FLOG_3(_L("CUmtsGprsSubConnProvd [this=%08x]:\tDoDataClientLeaving() [iDataClients.Count=%d] [aDataClient=%08x]"), this, iDataClients.Count(), &aDataClient);
	 aDataClient.LeaveComplete (*this);
    }
    
/**
Function called by SubConnection Provider when a control client is to join to this the QoS Flow ('this')

@param aControlClient Control Client to remove from this QoS Channel
*/
void CUmtsGprsSubConnProvd::DoControlClientJoiningL(MSubConnectionControlClient& aControlClient)
    {
    (void)aControlClient;
	__FLOG_3(_L("CUmtsGprsSubConnProvd [this=%08x]:\tDoControlClientJoiningL() [iControlClients.Count=%d] [aControlClient=%08x]"), this, iControlClients.Count(), &aControlClient);
	//We're not interested in knowing about MSubConnectionControlClient joining this sub-connection
    }
    
/**
Function called by SubConnection Provider when a control client is to be removed from the QoS Flow ('this')

@param aControlClient Control Client to remove from this QoS Channel
*/
void CUmtsGprsSubConnProvd::DoControlClientLeaving(MSubConnectionControlClient& aControlClient)
    {
    (void)aControlClient;
	__FLOG_3(_L("CUmtsGprsSubConnProvd [this=%08x]:\tDoControlClientLeaving() [iControlClients.Count=%d] [aControlClient=%08x]"), this, iControlClients.Count(), &aControlClient);
	//We're not interested in knowing about MSubConnectionControlClient leaving this sub-connection
    }


/** 
@param 
*/
void CUmtsGprsSubConnProvd::DoSourceAddressUpdate(MSubConnectionDataClient& /*aDataClient*/, const TSockAddr& /*aSource*/)
    {
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }
    
/** 
@param 
*/
void CUmtsGprsSubConnProvd::DoDestinationAddressUpdate(MSubConnectionDataClient& /*aDataClient*/, const TSockAddr& /*aDestination*/)
    {
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }
    
void CUmtsGprsSubConnProvd::DoDataClientRouted(MSubConnectionDataClient& /*aDataClient*/, const TSockAddr& /*aSource*/, const TSockAddr& /*aDestination*/, const TDesC8& /*aConnectionInfo*/)
/** 

@param 
*/
    {
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }

TInt CUmtsGprsSubConnProvd::DoControl(TUint /*aOptionLevel*/, TUint /*aOptionName*/, TDes8& /*aOption*/)
/** 

@param 
*/
    {
	__FLOG_1(_L("CUmtsGprsSubConnProvd::DoControl [this=%08x]"), this);
	return KErrNotSupported;
    }
 
void CUmtsGprsSubConnProvd::DoStartL()
/** 

@param 
*/
    {
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }
    
void CUmtsGprsSubConnProvd::DoStop()
/** 

@param 
*/
    {
    //TODO: implement when UmtsGprsSCPR enables ECreateNew SubConnection Providers.
    }


CUmtsGprsSubConnProvd::~CUmtsGprsSubConnProvd ()
   {
	__FLOG_1(_L("~CUmtsGprsSubConnProvd [this=%08x]"), this);
	if (iConnectionProvider)
		{
		iConnectionProvider->Leave(*this);
		}   
    delete iCurrentPdpFsmReq;
    __FLOG_CLOSE;
   }
