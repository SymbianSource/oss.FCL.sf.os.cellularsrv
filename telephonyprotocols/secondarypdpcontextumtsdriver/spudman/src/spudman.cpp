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
// SPUD event manager
// WINSCW compiler has problems disambiguating TMetaDes8 and Meta::TMetaDes8 so
// I prevent metabuffer.h from being recursively included from other headers
// by defining its include guard.
// 
//

/**
 @file
 @internalComponent
*/
#define METABUFFER_H

#include "spudman.h"
#include "bindman.h"
#include "mux.h"

#include <comms-infras/dbaccess.h>
#include "rpdpfsminterface.h"
using namespace SpudMan; // Access the SpudFsm event names

#include <etelpckt.h>
#include <comms-infras/commsdebugutility.h>
#include <in_sock.h>
#include <networking/qos_if.h>
#include <nifman.h>
#include <comms-infras/es_config.h>

#include <networking/umtsgprs_subconnprovfactory.h>
#include <ss_glob.h>
#include <ss_subconnprov.h>

const TUint KQosPlugInProtocolId = 360; //< Plug-in protocol ID

static const TInt KMaxInetAddrPrintSize = 50; //This should be big enough for an IPv6 printout


#ifdef __FLOG_ACTIVE

_LIT(KSpudUnknownLit, "Unknown event");

_LIT(KSpudSoIfControllerPlugInLit, "KSoIfControllerPlugIn");
_LIT(KSpudLitKRegisterEventHandler, "KRegisterEventHandler");
_LIT(KSpudContextSetEventsLit, "KContextSetEvents");
_LIT(KSpudNifSetDefaultQoSLit, "KNifSetDefaultQoS");
_LIT(KSpudContextCreateLit, "KContextCreate");
_LIT(KSpudContextDeleteLit, "KContextDelete");
_LIT(KSpudContextActivateLit, "KContextActivate");
_LIT(KSpudContextQoSSetLit, "KContextQoSSet");
_LIT(KSpudContextTFTModifyLit, "KContextTFTModify");
_LIT(KSpudGetNegQoSLit, "KGetNegQoS");
_LIT(KSpudContextModifyActiveLit, "KContextModifyActive");
_LIT(KInitialisePdpFsmLit, "KInitialisePdpFsm");


static const TDesC *SpudGuQoSEventToText(TInt aEvent)
	{
	switch(aEvent)
		{
	case KSoIfControllerPlugIn:
		return &KSpudSoIfControllerPlugInLit;
	case KRegisterEventHandler:
		return &KSpudLitKRegisterEventHandler;
	case KContextSetEvents:
		return &KSpudContextSetEventsLit;
	case KNifSetDefaultQoS:
		return &KSpudNifSetDefaultQoSLit;
	case KContextCreate:
		return &KSpudContextCreateLit;
	case KContextDelete:
		return &KSpudContextDeleteLit;
	case KContextActivate:
		return &KSpudContextActivateLit;
	case KContextQoSSet:
		return &KSpudContextQoSSetLit;
	case KContextTFTModify:
		return &KSpudContextTFTModifyLit;
	case KGetNegQoS:
		return &KSpudGetNegQoSLit;
	case KContextModifyActive:
		return &KSpudContextModifyActiveLit;
	case KInitialisePdpFsm:
		return &KInitialisePdpFsmLit;
	default:
    	return &KSpudUnknownLit;
		}
	}
	
_LIT(KSpudContextDeleteEventLit, "KContextDeleteEvent");
_LIT(KSpudContextActivateEventLit, "KContextActivateEvent");
_LIT(KSpudContextParametersChangeEventLit, "KContextParametersChangeEvent");
_LIT(KSpudContextBlockedEventLit, "KContextBlockedEvent");
_LIT(KSpudContextUnblockedEventLit, "KContextUnblockedEvent");
_LIT(KSpudNetworkStatusEventLit, "KNetworkStatusEvent");
_LIT(KSpudContextQoSSetEventLit, "KContextQoSSetEvent");
_LIT(KSpudContextTFTModifiedEventLit, "KContextTFTModifiedEvent");
_LIT(KSpudPrimaryContextCreatedLit, "KPrimaryContextCreated");
_LIT(KSpudSecondaryContextCreatedLit, "KSecondaryContextCreated");
_LIT(KSpudGetNegQoSEventLit,"KGetNegQoSEvent");
_LIT(KSpudContextModifyActiveEventLit, "KContextModifyActiveEvent");
_LIT(KPdpFsmShuttingDownLit, "KPdpFsmShuttingDown");

static const TDesC *SpudFsmEventToText(TInt aEvent)
	{
	switch(aEvent)
    	{
	case KContextDeleteEvent:
		return &KSpudContextDeleteEventLit;
	case KContextActivateEvent:
		return &KSpudContextActivateEventLit;
	case KContextParametersChangeEvent:
		return &KSpudContextParametersChangeEventLit;
	case KContextBlockedEvent:
		return &KSpudContextBlockedEventLit;
	case KContextUnblockedEvent:
		return &KSpudContextUnblockedEventLit;
	case KNetworkStatusEvent:
		return &KSpudNetworkStatusEventLit;
	case KContextQoSSetEvent:
		return &KSpudContextQoSSetEventLit;
	case KContextTFTModifiedEvent:
		return &KSpudContextTFTModifiedEventLit;
	case KPrimaryContextCreated:
		return &KSpudPrimaryContextCreatedLit;
	case KSecondaryContextCreated:
		return &KSpudSecondaryContextCreatedLit;
	case KGetNegQoSEvent:
    		return &KSpudGetNegQoSEventLit;
    case KContextModifyActiveEvent:
    	return &KSpudContextModifyActiveEventLit;
   	case KPdpFsmShuttingDown:
    	return &KPdpFsmShuttingDownLit;
    default:
    	return &KSpudUnknownLit;
    	}
	}

_LIT(KSpudStateSpudInactiveLit, "ESpudInactive");
_LIT(KSpudStateSpudHaveQosLit, "ESpudHaveQos");
_LIT(KSpudStateSpudCreatingPrimary, "ESpudCreatingPrimary");
_LIT(KSpudStateSpudStartingPrimaryLit, "ESpudStartingPrimary");
_LIT(KSpudStateSpudStartingPrimaryLowerNifLit, "ESpudStartingPrimaryLowerNif");
_LIT(KSpudStateSpudStartingSecondaryLit, "ESpudStartingSecondary");
_LIT(KSpudStateSpudStartingSecondaryLowerNifLit, "ESpudStartingSecondaryLowerNif");
_LIT(KSpudStateSpudGettingNegQoSLit, "ESpudGettingNegQoS");
_LIT(KSpudStateSpudUpLit, "ESpudUp");
_LIT(KSpudStateSpudFlowOffLit, "ESpudFlowOff");
_LIT(KSpudStateSpudSuspendedLit, "ESpudSuspended");
_LIT(KSpudStateSpudFlowOffAndSuspendedLit, "ESpudFlowOffAndSuspended");
_LIT(KSpudStateSpudLinkDownLit, "ESpudLinkDown");
_LIT(KSpudStateSpudContextDeleteLit, "ESpudContextDelete");
_LIT(KSpudStateSpudWaitLinkDownLit, "ESpudWaitLinkDown");
_LIT(KSpudStateSpudWaitBinderDeleteLit, "ESpudWaitBinderDelete");
_LIT(KSpudStateUnknownLit, "Unknown spud state");
	
static const TDesC *SpudStateToText(TSpudContextStates aState)
	{
	switch(aState)
    	{
	case ESpudInactive:
		return &KSpudStateSpudInactiveLit;
	case ESpudHaveQos:
		return &KSpudStateSpudHaveQosLit;
	case ESpudCreatingPrimary:
		return &KSpudStateSpudCreatingPrimary;
	case ESpudStartingPrimary:
		return &KSpudStateSpudStartingPrimaryLit;
	case ESpudStartingPrimaryLowerNif:
		return &KSpudStateSpudStartingPrimaryLowerNifLit;
	case ESpudStartingSecondary:
		return &KSpudStateSpudStartingSecondaryLit;
	case ESpudStartingSecondaryLowerNif:
		return &KSpudStateSpudStartingSecondaryLowerNifLit;
	case ESpudGettingNegQoS:
		return &KSpudStateSpudGettingNegQoSLit;
	case ESpudUp:
		return &KSpudStateSpudUpLit;
	case ESpudFlowOff:
		return &KSpudStateSpudFlowOffLit;
	case ESpudSuspended:
		return &KSpudStateSpudSuspendedLit;
	case ESpudFlowOffAndSuspended:
		return &KSpudStateSpudFlowOffAndSuspendedLit;
	case ESpudLinkDown:
		return &KSpudStateSpudLinkDownLit;
	case ESpudContextDelete:
		return &KSpudStateSpudContextDeleteLit;
	case ESpudWaitLinkDown:
		return &KSpudStateSpudWaitLinkDownLit;
	case ESpudWaitBinderDelete:
		return &KSpudStateSpudWaitBinderDeleteLit;
	default:
    	return &KSpudStateUnknownLit;
    	}
	}

#endif

CSpudMan::CSpudMan(CNifIfFactory& aFactory, MNifIfNotify* aNotify)
	: CNifIfLink(aFactory),
	iContextStatusOverride(RPacketContext::EStatusUnknown)
	{
    iNotify = aNotify;
    ASSERT(iNotify);
	__FLOG_OPEN(KSpudFirstTag,KSpudLog);
    __FLOG_0(_L("CSpudMan::CSpudMan"));
	}

CSpudMan::~CSpudMan()
   {
   if (iBindMan)
      {
      // Only log if ConstructL() was called, where logger was initialized
      __FLOG_0(_L("CSpudMan::~CSpudMan"));
      }
   
   
   if (AreQoSEventsEnabled())
      {
      // Spud is being destroyed by Nifman. Tell GUQoS to stop bothering SPUD.
      // GUQoS returns the favour by turning off the NIF events within this very call.
      // *********************************************************************************************
      // N.B.: "DEF055691 	GUQoS crashes at shutdown": This defect results in GUQoS crash if the stack
      // closes flows after the NIF proxy was deleted by GUQoS, as a result of KNetworkInterfaceDown event.
      // Because of this defect, we cannot signal KNetworkInterfaceDown from anywhere but here. Once it is fixed,
      // calls from the appropriate places will be enabled, and the line below will not be strictly necessary.
      SendNetworkStatusEvent(KNetworkInterfaceDown, RPacketService::EStatusUnattached);
      // For safety, we should call this from here in any case.
      //**********************************************************************************************
      }
   
   delete iParkedDefaultQoS;
   delete iBinderSweeperNotifierCb;
   
   delete iBindMan;
   iPdpFsmInterface.Close();
   iSipServerAddr.Close();
   __FLOG_CLOSE;
   }

/**
Panics the current thread.

@param aReason Panic reason code
*/
void CSpudMan::Panic(TInt aReason) const
	{
    __FLOG_1(_L("CSpudMan::Panic with reason %d"), aReason);
	User::Panic(KSpudName, aReason);
	}

/**
Construct the Link Protocol Object

@param aBindMan Pointer to BindMan object (ownership is transferred)
@leave leaves if could not allocate memory
*/
void CSpudMan::ConstructL(CBindMan* aBindMan)
	{
    __FLOG_1(_L("CSpudMan starting %x"), iNotify);


	iBindMan = aBindMan;
	ASSERT(iBindMan);
	
	// The lower NIF binder deletion & Nifman notification callback
	iBinderSweeperNotifierCb = new (ELeave) CBinderSweeperNotifierCb(*this);
	}


/**
Implements the Control method of CNifBase - used for retrieval of the P-CSCF (Sip Server) address
*/	
TInt CSpudMan::Control(TUint aLevel,TUint aName,TDes8& aOption, TAny*)
	{
	if (aLevel == KCOLConfiguration && aName == KConnGetSipServerAddr)
		{
		if (aOption.Length() != sizeof (SSipServerAddr))
			{
			__FLOG(_L("CSpudMan::Control - Invalid Descriptior - Descriptor Must contain an SSipServerAddr"));
			Panic();
			}
		SSipServerAddr* sipServerAddr = reinterpret_cast<SSipServerAddr*>(const_cast<TUint8*>(aOption.Ptr()));
		if (sipServerAddr->index < 0 || sipServerAddr->index >= iSipServerAddr.Count())
			{
			__FLOG_1(_L("CSpudMan::Control - Index out of range : value = %d"), sipServerAddr->index);
			return KErrNotFound;
			}
		sipServerAddr->address = iSipServerAddr[sipServerAddr->index];
#ifdef __FLOG_ACTIVE
		TBuf<KMaxInetAddrPrintSize> buf;
		sipServerAddr->address.Output(buf);
		__FLOG_2(_L("CSpudMan::Control Address %S returned for index %d"), &buf, sipServerAddr->index);
#endif
		return KErrNone;				
		}
	return KErrNotSupported;	
	}


/**
Retrieve any GPRS settings required from CommDB

@param aConfigGprs Context configuration parameters to be filled in
*/
void CSpudMan::RetrieveGprsConfig(RPacketContext::TContextConfigGPRS& aConfigGprs) const
	{
	TUint32 pdpType(0);
	Notify()->ReadInt(TPtrC(GPRS_PDP_TYPE), pdpType);
 	__FLOG_1(_L8("ReadInt GPRS_PDP_TYPE [%d]"), pdpType);
	aConfigGprs.iPdpType = STATIC_CAST(RPacketContext::TProtocolType, pdpType);

	Notify()->ReadDes(TPtrC(GPRS_APN), aConfigGprs.iAccessPointName);
  	__FLOG_1(_L8("ReadDes GPRS_APN [%S]"), &aConfigGprs.iAccessPointName);

		
	TBool fromServer;
	aConfigGprs.iPdpAddress.SetLength(0);
	Notify()->ReadBool(TPtrC(GPRS_IP_ADDR_FROM_SERVER), fromServer);
	if (!fromServer)
	    {
    	Notify()->ReadDes(TPtrC(GPRS_IP_ADDR), aConfigGprs.iPdpAddress);
	    }
	

    // We can only use IPv4 or IPv6 - we use the first one listed in the IfNetworks field
    TBuf<KCommsDbSvrMaxFieldLength> networks;
	Notify()->ReadDes(TPtrC(LAN_IF_NETWORKS), networks);

	ASSERT(networks.Length() > 0); // If the IfNetworks field is empty there is a serious misconfiguration
	
	TInt pos = networks.Find(_L(","));
	if (pos == KErrNotFound)
	    {
	    pos = networks.Length();
	    }
	TPtrC protocol(networks.Ptr(), pos);
	_LIT(KIp4, "ip");
	_LIT(KIp6, "ip6");
	if (protocol.CompareF(KIp4) == 0)
	    {
	    // IPv4 settings
    	Notify()->ReadBool(TPtrC(GPRS_IP_DNS_ADDR_FROM_SERVER), fromServer);
    	if (!fromServer)
    	    {
    	    Notify()->ReadDes(TPtrC(GPRS_IP_NAME_SERVER1), aConfigGprs.iProtocolConfigOption.iDnsAddresses.iPrimaryDns);
    	    Notify()->ReadDes(TPtrC(GPRS_IP_NAME_SERVER2), aConfigGprs.iProtocolConfigOption.iDnsAddresses.iSecondaryDns);
    	    }
	    }
	else if (protocol.CompareF(KIp6) == 0)
	    {
	    // IPv6 settings
    	Notify()->ReadBool(TPtrC(GPRS_IP6_DNS_ADDR_FROM_SERVER), fromServer);
    	if (!fromServer)
    	    {
    	    Notify()->ReadDes(TPtrC(GPRS_IP6_NAME_SERVER1), aConfigGprs.iProtocolConfigOption.iDnsAddresses.iPrimaryDns);
    	    Notify()->ReadDes(TPtrC(GPRS_IP6_NAME_SERVER2), aConfigGprs.iProtocolConfigOption.iDnsAddresses.iSecondaryDns);
    	    }
	    }
	else
	    {
	    // Anything else is a serious misconfiguration
	    ASSERT(0);
	    }
	
	RetrieveGprsCompression(aConfigGprs.iPdpCompression);
	 __FLOG_1(_L8("Read GprsCompression [%d]"), aConfigGprs.iPdpCompression);

	RetrieveGprsAnonymousAccess(aConfigGprs.iAnonymousAccessReqd);
 	__FLOG_1(_L8("Read AnonymousAccess [%d]"), aConfigGprs.iAnonymousAccessReqd);

	TBuf<KCommsDbSvrMaxUserIdPassLength> readBuf;
	Notify()->ReadDes(TPtrC(SERVICE_IF_AUTH_NAME), readBuf);
	aConfigGprs.iProtocolConfigOption.iAuthInfo.iUsername.Copy(readBuf);
	readBuf.Zero();
	Notify()->ReadDes(TPtrC(SERVICE_IF_AUTH_PASS), readBuf);
	aConfigGprs.iProtocolConfigOption.iAuthInfo.iPassword.Copy(readBuf);
 	__FLOG_2(_L8("ReadDes SERVICE_IF_AUTH_NAME [%S] and SERVICE_IF_AUTH_PASS [%S]"), &aConfigGprs.iProtocolConfigOption.iAuthInfo.iUsername, 
			 &aConfigGprs.iProtocolConfigOption.iAuthInfo.iPassword);

	aConfigGprs.iUseEdge = EFalse;
	}

void CSpudMan::RetrieveGprsCompression(TUint& aCompression) const
	{
	aCompression = 0;
	TBool isCompression = EFalse;
	Notify()->ReadBool(TPtrC(GPRS_DATA_COMPRESSION), isCompression);
	if (isCompression)
		{
		aCompression |= RPacketContext::KPdpDataCompression;
		}

	isCompression = EFalse;
	Notify()->ReadBool(TPtrC(GPRS_HEADER_COMPRESSION), isCompression);
	if (isCompression)
		{
		aCompression |= RPacketContext::KPdpHeaderCompression;
		}
	}

void CSpudMan::RetrieveGprsAnonymousAccess(RPacketContext::TAnonymousAccess& aAnonymous) const
	{	
	TBool isAnonymous = EFalse;
	Notify()->ReadBool(TPtrC(GPRS_ANONYMOUS_ACCESS),isAnonymous);
	if (isAnonymous)
		aAnonymous = RPacketContext::ERequired;
	else
		aAnonymous = RPacketContext::ENotRequired;
	}



/**
Indicates whether QoS has been enabled or not.

@return ETrue if QoS has been enabled for this NIF.
*/
TBool CSpudMan::AreQoSEventsEnabled() const
	{
	return iQosEventHandler != NULL;
	}


/**
Returns pointer to BindMan.

@return Pointer to BindMan object
*/
CBindMan* CSpudMan::BindMan() const
	{
	return iBindMan;
	}

/**
Returns pointer to the MNifIfNotify object in NIFMAN.

@return Pointer to MNifIfNotify object
*/
MNifIfNotify* CSpudMan::Notify() const
	{
	ASSERT(iNotify);
	return iNotify;
	}

/**
Set default QoS parameters from the values in CommDb.

@param aQos QoS parameters filled in on exit
*/
void CSpudMan::ReadDefaultQoS(RPacketQoS::TQoSR99_R4Requested& aQos) const
	{
	TBuf<2*KCommsDbSvrMaxColumnNameLength+1> columnName;
	_LIT(KFormatText,"%s\\%s");

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_TRAFFIC_CLASS);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqTrafficClass));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MIN_TRAFFIC_CLASS);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinTrafficClass));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_DELIVERY_ORDER);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqDeliveryOrderReqd));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MIN_DELIVERY_ORDER);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinDeliveryOrderReqd));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_DELIVER_ERRONEOUS_SDU);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqDeliverErroneousSDU));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MIN_DELIVER_ERRONEOUS_SDU);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinDeliverErroneousSDU));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_MAX_SDUSIZE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqMaxSDUSize));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MIN_ACCEPTABLE_MAX_SDU_SIZE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinAcceptableMaxSDUSize));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_MAX_UPLINK_RATE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqMaxRate.iUplinkRate));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_MIN_UPLINK_RATE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinAcceptableMaxRate.iUplinkRate));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_MAX_DOWNLINK_RATE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqMaxRate.iDownlinkRate));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_MIN_DOWNLINK_RATE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinAcceptableMaxRate.iDownlinkRate));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_BER);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqBER));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MAX_BER);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMaxBER));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_SDU_ERROR_RATIO);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqSDUErrorRatio));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MAX_SDU_ERROR_RATIO);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMaxSDUErrorRatio));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_TRAFFIC_HANDLING_PRIORITY);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqTrafficHandlingPriority));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MIN_TRAFFIC_HANDLING_PRIORITY);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinTrafficHandlingPriority));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_TRANSFER_DELAY);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqTransferDelay));
	
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MAX_TRANSFER_DELAY);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMaxTransferDelay));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_GUARANTEED_UPLINK_RATE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqGuaranteedRate.iUplinkRate));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MIN_GUARANTEED_UPLINK_RATE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinGuaranteedRate.iUplinkRate));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_REQ_GUARANTEED_DOWNLINK_RATE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iReqGuaranteedRate.iDownlinkRate));

	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_MIN_GUARANTEED_DOWNLINK_RATE);
	Notify()->ReadInt (columnName, reinterpret_cast<TUint32&>(aQos.iMinGuaranteedRate.iDownlinkRate));
	}

#ifdef SYMBIAN_NETWORKING_UMTSR5
/**
Set default R5 QoS parameters from the values in CommDb.

@param aQos R5 QoS parameters filled in on exit
*/
void CSpudMan::ReadDefaultR5QoS(RPacketQoS::TQoSR5Requested& aQos) const
	{
	ReadDefaultQoS(aQos);
	
	TBuf<2*KCommsDbSvrMaxColumnNameLength+2> columnName;
	_LIT(KFormatText,"%s\\%s");
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_SIGNALLING_INDICATION);
	Notify()->ReadBool (columnName, aQos.iSignallingIndication);
	columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_SOURCE_STATISTICS_DESCRIPTOR);
	Notify()->ReadInt(columnName, reinterpret_cast<TUint32&>(aQos.iSourceStatisticsDescriptor));
	}
#endif
// SYMBIAN_NETWORKING_UMTSR5


/**
Reads TSY name from CommDb.

@param aTsyName TSY name filled in on exit
*/
void CSpudMan::ReadTsyName(TName& aTsyName) const
	{
	TBuf<2*KCommsDbSvrMaxColumnNameLength+1> columnName;
	_LIT(KFormatText,"%s\\%s");
	columnName.Format(KFormatText,MODEM_BEARER,MODEM_TSY_NAME);
	Notify()->ReadDes(columnName, aTsyName); // ignore error
	}

/**
Sets the error code to use for termination, unless it is already set.

@param aError Error code to use when notifying NIFMAN
*/
void CSpudMan::SetTerminateError(TInt aError)
	{
	if (iTerminateError == KErrNone)
		{
		iTerminateError = aError;
		__FLOG_1(_L("Set SPUD termination error to [%d]"), aError);
		
		// There is no specific context associated with this termination
		// So prevent a later overwrite of the error by the ETel error
		if (iETelTerminateError == KErrNone)
			{
			iETelTerminateError = aError;
			}
		}
	}

/**
Sets the error code to use for termination, unless it is already set.

@param aContextId context ID
@param aError Error code to use when notifying NIFMAN
*/
void CSpudMan::SetTerminateError(TContextId aContextId, TInt aError)
	{
	if (iTerminateError == KErrNone)
		{
		iTerminateError = aError;
		
		// Now check to see if ETel was the originator of this error
		if (iETelTerminateError == KErrNone)
			{
			// ask for the last ETel error received - ignore return code
			iPdpFsmInterface.GetLastErrorCause(aContextId, iETelTerminateError);
			}

		if (iETelTerminateError != KErrNone)
			{
			// Overwrite with the most likely original error code!
			iTerminateError = iETelTerminateError;
			}
	
		__FLOG_1(_L("Set SPUD termination error to [%d]"), iTerminateError);
		}
		
	}

//*****************************************************************************
// Events receivers
//*****************************************************************************

/**
Receives events from SpudFsm.

@param aContextId context ID
@param aEvent event ID
@param aParam optional parameter to event
*/
void CSpudMan::Input(TContextId aContextId, TInt aEvent, TInt aParam)
	{
	__FLOG_4(_L("SpudMan::Input: SpudFsm event %S(%d) param[%d] context[%d]"), SpudFsmEventToText(aEvent), aEvent, aParam, aContextId);
	ASSERT(aContextId == KAllContexts || (aContextId >=0 && aContextId < KMaxPdpContexts));
	
	switch (aEvent)
		{
	case KPrimaryContextCreated:
		{
		HandlePrimaryContextCreatedEvent(aContextId, aParam);
		break;
		}
	
	case KContextDeleteEvent:
		{
		HandleContextDeleteEvent(aContextId, aParam);
		break;
		}

	case KSecondaryContextCreated:
		{
		HandleSecondaryContextCreatedEvent(aContextId, aParam);
		break;
		}

	case KContextActivateEvent:
		{
		HandleContextActivateEvent(aContextId, aParam);
		break;
		}

	case KContextQoSSetEvent:
		{
		HandleContextQoSSetEvent(aContextId, aParam);
		break;
		}

	case KContextTFTModifiedEvent:
		{
		HandleContextTFTModifiedEvent(aContextId, aParam);
		break;
		}

	case KGetNegQoSEvent:
		{
		HandleGetNegQoSEvent(aContextId, aParam);
		break;
		}

	case KContextModifyActiveEvent:
		{
		HandleContextModifyActiveEvent(aContextId, aParam);
		break;
		}

	case KNetworkStatusEvent:
		{
		HandleNetworkStatusEvent();
		break;
		}

	case KContextParametersChangeEvent:
		{
		HandleContextParametersChangeEvent(aContextId, aParam);
        break;
		}

	case KContextBlockedEvent:
		{
		HandleContextBlockedEvent(aContextId);
		break;
		}

	case KContextUnblockedEvent:
		{
		HandleContextUnblockedEvent(aContextId);
		break;
		}


   case KPdpFsmShuttingDown:
      {
      __FLOG_0(_L("UmtsGprsSCPR has shutdown the PDP Fsm Interface"));
      iPdpFsmInterface.Close();
      break;
      }

	default:
		__FLOG_1(_L("Unhandled event %d"), aEvent);
		ASSERT(EFalse);
		break;
		}
	}
	
void CSpudMan::InitPdpFsmInterfaceL()
	{
	class XAssociatedNifConnectionProviderQuery : public MFactoryQuery
	/**	Finds the connection provider associated with the specified Nif.
	@internalTechnology
	*/
		{
	public:
		XAssociatedNifConnectionProviderQuery( const TDesC& aName, ::TMetaDes8& aNameBuffer ) : iNifName( aName ), iNifNameBuffer( aNameBuffer )
			{
			}

		virtual TMatchResult Match( TFactoryObjectInfo& aObjectInfo )
			{
			TConnInterfaceName& name = *reinterpret_cast<TConnInterfaceName*>( const_cast<TUint8*>( iNifNameBuffer.iDes->Ptr() ) );
			name.iIndex = 1;

			TInt err = KErrNone;
			TMatchResult result = EContinue;
			
			// Check each of the connection provider interfaces' names to see if it
			// is associated with this NIF.
			while( ETrue )
				{
		        TRAP( err, static_cast<CConnectionProviderBase*>( aObjectInfo.iInfo.iFactoryObject )->ControlL(KCOLProvider, KConnGetInterfaceName, iNifNameBuffer, NULL) );
		        
		        if( err == KErrNone )
		        	{
		        	if( name.iName == iNifName )
			        	{
				       	result = EMatch;
				       	
				       	break;
			        	}
			       	}
			    else
			    	{
			    	break;
			    	}

				name.iIndex ++;
				}
	        	
	        return result;
			}
			
	private:
		const TDesC& iNifName;
		::TMetaDes8& iNifNameBuffer;
		};

	const TUint KShimConnectionProviderFactoryId = 0x10207104; //the same as CSubConnectionProviderFactoryShim

	TSockManData* sockManData = SockManGlobals::Get();
	ASSERT(sockManData);

	CConnectionFactoryContainer* connectionFactories = sockManData->iConnectionFactories;
	ASSERT(connectionFactories);

	CConnectionProviderFactoryBase *factory = connectionFactories->FindFactory(KShimConnectionProviderFactoryId);
	if(!factory)
		{
		User::Leave( KErrNotFound );
		}
		
	CSpudMux* mux = iBindMan->SpudMux();

	// Get the name of our SPUDMUX interface - it uniquely identifies this SPUD object assembly.
	TNifIfInfo info;
	mux->Info( info );

	// Create a buffer to hold the name of each interface we check to see if the interface is our SPUDMUX.
	TPckgBuf<TConnInterfaceName> name;
	TPtrC8 desC( name );
	::TMetaDes8* des = ::TMetaDes8::NewL( &desC );

	XAssociatedNifConnectionProviderQuery connProviderQuery( info.iName, *des );
	
	// Find the connection provider associated with this NIF.
	CConnectionProviderBase* connectionProvider = factory->FindProvider(connProviderQuery);
	delete des;
	if(!connectionProvider)
		{
		User::Leave( KErrNotFound );
		}

	CSubConnectionFactoryContainer* subConnectionFactories = sockManData->iSubConnectionFactories;
	ASSERT(subConnectionFactories);

	TDataClientQuery subConnProviderQuery(connectionProvider, ESubConnPlane, RSubConnection::EAttachToDefault);

	// Find the default subconnection provider.
	CSubConnectionProviderBase* subConnProvider = subConnectionFactories->FindOrCreateProviderL(connectionProvider->CanDoSubConnection(RSubConnection::EAttachToDefault), subConnProviderQuery);
	if(!subConnProvider || subConnProvider->Factory().Id() != KUmtsGprsSubConnectionProviderFactoryId)
		{
		User::Leave( KErrNotFound );
		}

	// Initialise the PDP FSM interface.
	TPckg<CNifIfBase*> nifPckg(mux);
	User::LeaveIfError(subConnProvider->Control(KSOLInterface, KInitialisePdpFsm, nifPckg));
	}

void CSpudMan::HandlePrimaryContextCreatedEvent(TContextId aContextId, TInt aError)
	{
		
	// Save the contextId for later
	iPrimaryContextId = aContextId;
	
	// Primary context has been created; Start the lower NIF
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__FLOG_2(_L("Got lower NIF binder for context[%d] with error[%d]"), aContextId, rc);
	if (rc == KErrNone)
		{
		// Make sure context was created successfully
		rc = aError;
		ASSERT(ref->IsBound());
		__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
		}

	if (KErrNone == rc) // Context created successfully.
		{
			// Get the details of the P-CSCF 
		RPacketContext::TContextConfigGPRS config;
		
		if (iPdpFsmInterface.Get(aContextId, config) == KErrNone)
			{
			TRAP(rc, SetSipServerAddrL(config.iProtocolConfigOption););
				{
				__FLOG_1(_L("CSpudMan::HandlePrimaryContextCreatedEvent -Error occurred extracting the sip server address: %d"),rc);
				//rc is now non zero, therefore should fail gracefully
				}	
#ifdef SYMBIAN_NETWORKING_UMTSR5
			// Since the value or IMCN Signalling flag also comes in PCO IE buffer along with the SipAddress, we
			// need to extract the value here and set it to Parameter Bundle of SCPR
				TBool imcn = EFalse;		
				TRAPD(err,imcn=GetIMCNSignallingFlagPcoL(config.iProtocolConfigOption););
				__FLOG_1(_L("CSpudMan::GetIMCNSignallingFlagPcoL - returns eror=%d"),err);
				if (err == KErrNone)
					{
					iPdpFsmInterface.SetIMCNSignalling(imcn); //imcn contains ETrue or Efalse
#ifdef __FLOG_ACTIVE
					if (imcn)
						{
						__FLOG_0(_L("CSpudMan::HandlePrimaryContextCreatedEvent - Network ACCEPTS the request for dedicated IMCN Signalling Flag "));
						}
					else
						{
						__FLOG_0(_L("CSpudMan::HandlePrimaryContextCreatedEvent - Network REJECTS the request for dedicated IMCN Signalling Flag "));				
						}
#endif 
					}
					else
					{
					
#ifdef __FLOG_ACTIVE
					__FLOG_1(_L("CSpudMan::HandlePrimaryContextCreatedEvent -Leave Error occurred  %d"),err);
#endif
					}			
#endif // SYMBIAN_NETWORKING_UMTSR5
			}
		}
		
	if (KErrNone == rc) // Sip server address retrieved
		{
		ASSERT(ref->State() == ESpudCreatingPrimary);
	    ref->SetState(ESpudStartingPrimary);
	    // Start the lower NIF and wait for the LinkLayerUp call
		rc = ref->NifLink()->Start(); 
		if(KErrNone == rc)
			{
			ref->SetState(ESpudStartingPrimaryLowerNif);
			}
		// Lower NIF may still report failure to Start by signalling LinkLayerDown with error.
		// E.G. PPP negotiation may fail.
		__FLOG_2(_L("Lower NIF for Primary Context[%d] started with error[%d]."), aContextId, rc);
		}

		
	// Catch-all error handling: Could not create context, or could not start lower NIF.	
	if(KErrNone != rc) 
		{			
		SetTerminateError(rc);	
		
	    SendPrimaryContextCreated(aContextId, rc); // Notify GUQoS of error

 		if(ESpudStartingPrimary == ref->State()) // Context created 
 			{
 			ref->SetState(ESpudWaitLinkDown); // We've just notified GUQoS.
 			// Delete context via SpudFsm
			__FLOG_2(_L("Failed to start lower NIF for Primary PDP context[%d] due to error[%d]. Deleting Primary via SpudFsm."), aContextId, rc);
			rc = iPdpFsmInterface.Input(aContextId, EContextDelete);
			ASSERT(rc == KErrNone);
 			}
 		else 	// Context was not created, else we would not be here. There is nothing to stop and delete.
 			{
 			__FLOG_1(_L("Failed to create Primary context[%d]. Spud will shut down."), aContextId);
			
 			if (AreQoSEventsEnabled())
				{
				// At this point we know that SPUD is about to shut down, because we are
				// deleting the last context. We want to indicate to the upper layers early that
				// data can no longer be sent to SPUD.
									
				// Tell GUQoS to stop bothering SPUD.
				// GUQoS returns the favour by turning off the NIF events within this very call.
				// This means we won't send KNetworkInterfaceDown, even if we try.
		   		// *********************************************************************************************
				// N.B.: "DEF055691 	GUQoS crashes at shutdown": This defect results in GUQoS crash if the stack
    			// closes flows after the NIF proxy was deleted by GUQoS, as a result of KNetworkInterfaceDown event.
    			// As a temporary workaround, this call is disabled, and KNetworkInterfaceDown event is signalled only
    			// from the destructor. Once this defect is fixed, the following line must be uncommented: 
    			// SendNetworkStatusEvent(KNetworkInterfaceDown, RPacketService::EStatusUnattached);
				//**********************************************************************************************
		   		}
			// We are about to delete the primary context NIF: this will notify Nifman that SPUD is finished.
 			DisposeOfBinder(ref);
 			}
 		}		
	}

void CSpudMan::HandleContextDeleteEvent(TContextId aContextId, TInt aError)
	{
	aError = aError; // suppress compiler warning
	// Ignore error on delete--nothing we can do, anyway
	// For the upper layers, we treat this event as KErrDisconnected, because the network has torn down an 
	// existing context.
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	ASSERT(ref->IsBound());
	__FLOG_4(_L("Network deleted context %d, in state %S(%d). Deletion error=%d"),
		aContextId, SpudStateToText(ref->State()), ref->State(), aError);
	
	// Are we about to shut down after this context is deleted?
	TBool shutdownStarted(BindMan()->IsLastContext(aContextId));
	// We must determine this before we make any state transitions. But we can act on this
	// only after we've done all GUQoS notifications to make sure shutdown is graceful.
	
	switch (ref->State()) 
		{
	case ESpudUp:
	case ESpudFlowOff:
	case ESpudSuspended:
	case ESpudFlowOffAndSuspended:
		ref->SetState(ESpudWaitLinkDown);
		SendContextDeleteEvent(aContextId);
		ref->NifLink()->Stop(KErrConnectionTerminated, MNifIfNotify::EDisconnect);
        break;
        
    case ESpudStartingPrimaryLowerNif:
    	ref->SetState(ESpudWaitLinkDown);
    	SendPrimaryContextCreated(aContextId, KErrDisconnected);
    	SendContextDeleteEvent(aContextId);
    	ref->NifLink()->Stop(KErrConnectionTerminated, MNifIfNotify::EDisconnect);
        break;
        	        
	case ESpudGettingNegQoS: // Context was activated. It is assumed that QoS retrieval would be cancelled by deletion.
	case ESpudStartingSecondaryLowerNif:
		ref->SetState(ESpudStartingSecondary);
		FillInContextConfig(iTempContextConfig, aContextId);
		SendContextActivateEvent(aContextId, iTempContextConfig, KErrDisconnected);
		SendContextDeleteEvent(aContextId);
		ref->NifLink()->Stop(KErrConnectionTerminated, MNifIfNotify::EDisconnect);
        break;
		
	case ESpudStartingPrimary: 
		// this should never happen
		SendPrimaryContextCreated(aContextId, KErrDisconnected);
		SendContextDeleteEvent(aContextId);
		DisposeOfBinder(ref);
		break;
	
	case ESpudStartingSecondary:
		// This should never happen, and may cause problems if it does because there could be
		// another outstanding request from GUQoS that never gets completed.
		__FLOG_0(_L("Network should not have deleted context in ESpudStartingSecondary state!"));
		// Fall through and treat the same as ESpudLinkDown
		
	case ESpudLinkDown:
		SendContextDeleteEvent(aContextId);
		DisposeOfBinder(ref);
		break;
	
	case ESpudWaitLinkDown:	
		DisposeOfBinder(ref);
		break;
		
	
	default:
		__FLOG_2(_L("Unexpected state %S(%d)"), SpudStateToText(ref->State()), ref->State());
		ASSERT(EFalse);
		break;
		}
	
	if(shutdownStarted) // Additional steps if we are shutting down as as result of this deletion.
		{
		SetTerminateError(KErrDisconnected); 
		// We don't reuse errorForGuqos, because we may want to keep Nifman errors and GUQoS errors separate.	
		if (AreQoSEventsEnabled())
			{
			// At this point we know that SPUD is about to shut down, because the last context 
			// was just deleted. We want to indicate to the upper layers early that
			// data can no longer be sent to SPUD.
								
			// Tell GUQoS to stop bothering SPUD.
			// GUQoS returns the favour by turning off the NIF events within this very call.
			// This means we send KNetworkInterfaceDown only once.
			// *******************************************************************************************
			// N.B.: "DEF055691 	GUQoS crashes at shutdown": This defect results in GUQoS crash if the stack
    		// closes flows after the NIF proxy was deleted by GUQoS, as a result of KNetworkInterfaceDown event.
    		// As a temporary workaround, this call is disabled, and KNetworkInterfaceDown event is signalled only
    		// from the destructor. Once this defect is fixed, the following line must be uncommented: 
    		// SendNetworkStatusEvent(KNetworkInterfaceDown, RPacketService::EStatusUnattached);
			//**********************************************************************************************
			}	
		}	
	}

void CSpudMan::HandleSecondaryContextCreatedEvent(TContextId aContextId, TInt aError)
	{
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	if (rc == KErrNone)
		{
		ASSERT(ref->IsBound());
		__FLOG_4(_L("Network created secondary context[%d] in state %S(%d). Creation error=%d "),aContextId, SpudStateToText(ref->State()), ref->State(), aError);
		if (ref->State() != ESpudStartingSecondary)
			{
			__FLOG_3(_L("KSecondaryContextCreated context %d is in unexpected state %S(%d)"),
			         aContextId, SpudStateToText(ref->State()), ref->State());
			}
		ref = ref;	// Eliminate compiler warning in release builds
		
		// Make sure context was created successfully
		rc = aError;
		if(KErrNone != rc) // Creation failed: we don't have the context, only the binder. Delete it.
			{
			DisposeOfBinder(ref);
			}
		}
	SendSecondaryContextCreated(aContextId, rc);
	}

void CSpudMan::HandleContextActivateEvent(TContextId aContextId, TInt aError)
	{
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	if (rc == KErrNone)
		{
		// Make sure context was activated successfully
		rc = aError;
	
		ASSERT(ref->IsBound());
		__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
		}

	if (rc == KErrNone)
		{
		ASSERT(ref->State() == ESpudStartingSecondary);
		// Start the lower NIF
		rc = ref->NifLink()->Start(); 
		// Even if we get KErrNone here, we may still get LinkLayerDown with error later.
		__FLOG_2(_L("KContextActivateEvent: Start on lower NIF returned for Context[%d] returned error[%d]"),aContextId, rc);
		}
	
	if(KErrNone == rc) // Start OK.
		{
		
		ref->SetState(ESpudStartingSecondaryLowerNif);
		}
	else
		{
		// Error activating PDP context
		FillInContextConfig(iTempContextConfig, aContextId);
		SendContextActivateEvent(aContextId, iTempContextConfig, rc);
		}
	}

void CSpudMan::HandleContextQoSSetEvent(TContextId aContextId, TInt aError)
	{
	// Notify GUQoS of success or failure
    SendContextQoSSetEvent(aContextId, aError);
	}

void CSpudMan::HandleContextTFTModifiedEvent(TContextId aContextId, TInt aError)
	{
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	ASSERT(ref->IsBound());
	__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
	ref = ref;	// Eliminate compiler warning in release builds

	// Notify GUQoS of success or failure
	TTFTOperationCode opCode;
    iPdpFsmInterface.Get(aContextId, opCode);
    SendContextTFTModifiedEvent(aContextId, opCode, aError);
	}

void CSpudMan::HandleGetNegQoSEvent(TContextId aContextId, TInt aError)
	{
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	
	ASSERT(ref->IsBound());
	__FLOG_4(_L("CSpudMan: Retrieved Negotiated QoS, error=%d: context %d is in state %S(%d)"), aError, aContextId, SpudStateToText(ref->State()), ref->State());
	if (ref->State() == ESpudGettingNegQoS)
		{
		// Negotiated QoS retrieved successfully. At this point the ctx is up both on control and data paths.
		if(KErrNone == aError) 
			{
			// Context is now fully up. Notify GUQoS.
			ref->SetState(ESpudUp);
			// Notify GUQoS of success or failure
			FillInContextConfig(iTempContextConfig, aContextId);
    		SendContextActivateEvent(aContextId, iTempContextConfig, aError);
			
			if(ref->State() == ESpudUp) // GUQoS can delete the context from the activation notification.
				{
				BindMan()->SpudProtocol()->DoStartSending();
				SendContextUnblockedEvent(aContextId);	
				}
			}
		// If the QoS could not be retrieved, we remain in ESpudGettingNegQoS and wait for 
		// the network to delete the context. The rest would be handled from the deletion event.
		
		// N.B. CRITICAL ASSUMPTION: 
		// The network / TSY will delete the ctx if the negotiated QoS could not be retrieved.
		// (The idea is that if QoS negotiation is errored out, then everything is errored out.
		// So we either do not get to this point at all, or will be errored out soon after this handler completes.)
		// This leaves the issue of internal ETel/TSY errors such as OOM conditions.)
		// At this point Spud is idle (no requests outstanding), so nothing will drive us
		// forward to clean up this failure. GUQoS does NOT time out on PDP context creation failure.
		}
	else
		{
		// If we are not getting negotiated QoS, but we receive this notification, then:
		// - ETel has sent us a stray notification totally uncalled for, i.e. it's a bug in ETel or more likely a TSY.
		if(KErrNone == aError)
			{
			__FLOG(_L("WARNING! Negotiated QoS retrieval completed successfully, but totally out of order! See the comments in the code.)"));
			}
		// We don't assert, because it can be a race condition outside of ETel's control:
		// - ETel, lower NIF, GUQoS, Nifman has errored the context out (e.g. ctx deleted by network), so there was a 
		// state transition as a result of that, and now the QoS retrieval has completed after that with KErrNone.
		}
	}


void CSpudMan::HandleContextModifyActiveEvent(TContextId aContextId, TInt aError)
	{
	// Notify GUQoS of success or failure
	FillInContextConfig(iTempContextConfig, aContextId);
    SendContextModifyActiveEvent(aContextId, iTempContextConfig, aError);
	}

void CSpudMan::HandleNetworkStatusEvent()
	{
	// This is a network status change
	RPacketService::TStatus status;
	iPdpFsmInterface.Get(status);
	
	// Assume that anything other than EStatusUnattached means the network
	// is still up, so just ignore this event.
	if (status == RPacketService::EStatusUnattached)
		{
		// Notify GUQoS
		SendNetworkStatusEvent(KNetworkConnectionLost, status);
		}
	else
		{
		__FLOG_1(_L("Ignoring KNetworkStatusEvent with status %d (NOT EStatusUnattached)"),
				 status);
		}
	}

void CSpudMan::HandleContextParametersChangeEvent(TContextId aContextId, TInt aError)
	{
	// This is a status change on an individual context
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	ASSERT(ref->IsBound());
	__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
	if (ref->State() == ESpudGettingNegQoS || ref->State() == ESpudUp || ref->State() == ESpudFlowOff || ref->State() == ESpudSuspended || ref->State() == ESpudFlowOffAndSuspended)
		{
		// Only pass on context changes while the context is up
		FillInContextConfig(iTempContextConfig, aContextId);
		SendContextParametersChangeEvent(aContextId, iTempContextConfig, aError);
		}
	else
		{
		__FLOG_3(_L("KContextParametersChangeEvent ignored on context %d because of nonoperational state %S(%d)"),
				 aContextId, SpudStateToText(ref->State()), ref->State());
        SetTerminateError(aContextId, aError);
		}
	}

void CSpudMan::HandleContextBlockedEvent(TContextId aContextId)
	{
	// Context is suspended (see StopSending)
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	ASSERT(ref->IsBound());
	__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
	switch (ref->State())
		{
	case ESpudUp:
		{
		ref->SetState(ESpudSuspended);
		if (AreQoSEventsEnabled())
			{
			SendContextBlockedEvent(aContextId);
			}
			
		// Send an IfProgress to Nifman so that RConnection::ProgressNotification reports the blockage etc.
		Notify()->IfProgress(KDataTransferTemporarilyBlocked, KErrNone);
		break;
		}

	case ESpudFlowOff:
		ref->SetState(ESpudFlowOffAndSuspended);
			
		// Send an IfProgress to Nifman so that RConnection::ProgressNotification reports the blockage etc.
		Notify()->IfProgress(KDataTransferTemporarilyBlocked, KErrNone);
		break;

	case ESpudSuspended:
	case ESpudFlowOffAndSuspended:
		// Ignore this since we're already suspended
		break;

	default:
		// Ignore this since we're still starting up or shutting down
		__FLOG_1(_L("Can't send blocked event now on context %d"), aContextId);
		break;
		}
		
	// TODO: Probably need to send IfProgress here where aStage==KDataTransferTemporarilyBlocked
	// or Notification with aEvent=EAgentToNifEventTypeDisableTimers
	//
	// NOTE: The IfProgress has now been implemented so that callers of RConnection::ProgressNotification
	// expecting to receive KDataTransferTemporarilyBlocked actually get it if the PDP context is
	// suspended (details of problem in INC107930).
	}

void CSpudMan::HandleContextUnblockedEvent(TContextId aContextId)
	{
	// Context is suspended (see StartSending)
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	ASSERT(ref->IsBound());
	__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
	switch (ref->State())
		{
	case ESpudUp:
	case ESpudFlowOff:
		// Ignore this since we're already in an unsuspended state
		break;

	case ESpudSuspended:
		{
		ref->SetState(ESpudUp);

		BindMan()->SpudProtocol()->DoStartSending();

		if (AreQoSEventsEnabled())
			{
			SendContextUnblockedEvent(aContextId);
			}
		Notify()->IfProgress(KLinkLayerOpen, KErrNone);
		break;
		}

	case ESpudFlowOffAndSuspended:
		{
		ref->SetState(ESpudFlowOff);
		Notify()->IfProgress(KLinkLayerOpen, KErrNone);
		break;
		}

	default:
		// Ignore this since we're still starting up or shutting down
		__FLOG_1(_L("Can't send unblocked event now on context %d; this may cause problems"), aContextId);
		break;
		}
	}

/**
Receives event from GUQoS.

@param aName event identification
@param aOption optional data associated with event
@return error code
*/
TInt CSpudMan::GuqosInput(TUint aName, TDes8& aOption)
	{
	__FLOG_2(_L("SpudMan::GuqosInput: GUQoS event %S(%d)"), SpudGuQoSEventToText(aName), aName);
	switch (aName)
		{
	case KSoIfControllerPlugIn:
        {
        // Indicate that we want GUQoS
        TSoIfControllerInfo& opt = *reinterpret_cast<TSoIfControllerInfo*>(const_cast<TUint8*>(aOption.Ptr()));
        _LIT(KQosPlugInName, "guqos");
        opt.iPlugIn = KQosPlugInName;
        opt.iProtocolId = KQosPlugInProtocolId;
        return KErrNone;
        }

    case KRegisterEventHandler:
        {
        // GUQoS has passed a pointer to its event handler
        const TEvent *handler = reinterpret_cast<const TEvent *>(aOption.Ptr());
        iQosEventHandler = static_cast<MNifEvent *>(handler->iEvent);
        ASSERT(iQosEventHandler);
        return KErrNone;
        }

	case KContextSetEvents:
		{
        if (!AreQoSEventsEnabled())
        	{
        	// Event handler must be registered first
            return KErrGeneral;
        	}
        const TBool* eventsEnabledPtr = reinterpret_cast<const TBool *>(aOption.Ptr());
        ASSERT(eventsEnabledPtr);
        iQosEventsEnabled = *eventsEnabledPtr;

        // Has a primary PDP context has already been created?
       	CSpudBinderRef* ref = NULL;
		TRAPD(rc, ref = BindMan()->GetAnyRefL());
		const TBool havePrimary(rc == KErrNone && (ref->State() == ESpudUp || ref->State() == ESpudFlowOff || ref->State() == ESpudSuspended || ref->State() == ESpudFlowOffAndSuspended));

        if (iQosEventsEnabled && havePrimary)
        	{
        	// iPrimaryContextId == KPrimaryContextId == 0 at startup
            SendPrimaryContextCreated(iPrimaryContextId, KErrNone);
            }
        return KErrNone;
		}

	case KNifSetDefaultQoS:
		{
         ASSERT(aOption.Ptr());
         const TContextParameters& opt = *reinterpret_cast<const TContextParameters*>(aOption.Ptr());

         if (!iPdpFsmInterface.IsInitialised()) 
            {
            __FLOG_0(_L("CSpudMan::GuqosInput: iPdpFsmInterface not initialised, parking KNifSetDefaultQoS."));
            
            if (iParkedDefaultQoS == NULL)
               {
               TRAPD(err, iParkedDefaultQoS = HBufC8::NewL (sizeof (TContextParameters)));
               if (err != KErrNone)
                  {
                  __FLOG_0(_L("CSpudMan::GuqosInput: Failed to park default QoS."));
                  return err;
                  }
               }
               
            iParkedDefaultQoS->Des().Copy (aOption);

            return KErrNone;
            }
         
       	CSpudBinderRef* ref = NULL;
       	// Lower NIF for primary context has already been loaded by factory
		TRAPD(rc, ref = BindMan()->GetRefL(iPrimaryContextId));
		__ASSERT_ALWAYS(rc == KErrNone, Panic());
		ASSERT(ref->IsBound());
		
		ASSERT(ref->State() == ESpudInactive);
        ref->SetState(ESpudHaveQos);
        	
#ifdef SYMBIAN_NETWORKING_UMTSR5
		// Store R5 QoS parameters
			RPacketQoS::TQoSR5Requested qos;
#else
		// Store QoS parameters
			RPacketQoS::TQoSR99_R4Requested qos;
#endif 
// SYMBIAN_NETWORKING_UMTSR5
		
		opt.iContextConfig.GetUMTSQoSReq(qos); 
		rc = iPdpFsmInterface.Set(iPrimaryContextId, qos);	

        if (rc != KErrNone)
			{
			__FLOG_1(_L("Setting default QoS on primary context failed with error [%d]. SPUD will shut down."), rc);
			SetTerminateError(iPrimaryContextId, rc); 
			
			SendPrimaryContextCreated(iPrimaryContextId, rc); // Notify GUQoS of error
			
			// At this point we know that SPUD is about to shut down, because we could not bring the 
	 		// primary context UP.
	
			//**********************************************************************************************
			// Tell GUQoS to stop bothering SPUD.
			// GUQoS returns the favour by turning off the NIF events within this very call.
	        // N.B.: "DEF055691 	GUQoS crashes at shutdown": This defect results in GUQoS crash if the stack
	        // closes flows after the NIF proxy was deleted by GUQoS, as a result of KNetworkInterfaceDown event.
	        // As a temporary workaround, this call is disabled, and KNetworkInterfaceDown event is signalled only
	        // from the destructor. Once this defect is fixed, the following line must be uncommented: 
	        // SendNetworkStatusEvent(KNetworkInterfaceDown, RPacketService::EStatusUnattached);
			//**********************************************************************************************
			
			__FLOG_1(_L("Setting default QoS on primary context failed: mark lower NIF binder for context[%d] for async deletion."), iPrimaryContextId);
			// We are about to delete the primary context NIF: this will notify Nifman that SPUD is finished.
			DisposeOfBinder(ref);
			}
			
        return KErrNone;
		}

	case KContextCreate:
		{
        ASSERT(aOption.Ptr());
		TContextParameters& opt=*reinterpret_cast<TContextParameters*>(const_cast<TUint8*>(aOption.Ptr()));
		ASSERT(opt.iContextType == ESecondaryContext);

		TContextId id(KAllContexts);	// placeholder context ID
		CSpudBinderRef* ref = NULL;
		TRAPD(rc, ref = BindMan()->GetNewRefForSecondaryL(id););
		if(KErrNone != rc)
			{
			__FLOG_1(_L("Error %d creating a binder for the lower NIF"), rc);
			opt.iReasonCode = rc;  // The error code is the only argument we can pass to GUQoS, because there is no context.
    		return KErrNone;
    		}


		TRAP(rc, BindMan()->LoadNifL(iName, *ref);)
		if (rc != KErrNone)
			{
			__FLOG_1(_L("Error %d loading the lower NIF"), rc);
			FillInParameters(opt, id, rc);
			return KErrNone;
			}

		ASSERT(ref->IsBound());
        ref->SetState(ESpudStartingSecondary);


		// Reset the default QoS and TFT here. KContextQoSSet should arrive soon
		// with the proper values.
#ifdef SYMBIAN_NETWORKING_UMTSR5
			RPacketQoS::TQoSR5Requested qos;
#else
			RPacketQoS::TQoSR99_R4Requested qos;
#endif 
// SYMBIAN_NETWORKING_UMTSR5 
		rc = iPdpFsmInterface.Set(id, qos);

		TTFTInfo tft;
        rc = iPdpFsmInterface.Set(id, tft);
		
		// Pass in the new context ID to SpudFsm to use for the new context

		// Notify SpudFsm
		__FLOG_1(_L("Sending SpudFsm event ECreateSecondaryPDPContext context %d"), id);
		rc = iPdpFsmInterface.Input(id, ECreateSecondaryPDPContext);

		// Set up the synchronous response
		FillInParameters(opt, id, rc);
    	return KErrNone;
		}
		
	case KContextDelete:
		{
        ASSERT(aOption.Ptr());
        TContextParameters& opt = *reinterpret_cast<TContextParameters*>(const_cast<TUint8*>(aOption.Ptr()));
       	CSpudBinderRef* ref = NULL;
		TRAPD(rc, ref = BindMan()->GetRefL(opt.iContextInfo.iContextId));
		if (rc != KErrNone)
			{
			__FLOG_1(_L("Error: KContextDelete specifies context %d which does not exist"), opt.iContextInfo.iContextId);
			opt.iReasonCode = rc;  // The error code is the only argument we can pass to GUQoS, because there is no context.
    		return KErrNone;
			}
		
		ASSERT(ref->IsBound());
		__FLOG_3(_L("KContextDelete context[%d] in state %S(%d)"), opt.iContextInfo.iContextId, SpudStateToText(ref->State()), ref->State());

		switch (ref->State())
			{
		// NIF is up: need to stop the NIF first, then delete the binder.	
		case ESpudGettingNegQoS:
		case ESpudStartingSecondaryLowerNif: 
		// Assumption: GUQoS will never delete primary context.
		case ESpudUp:
		case ESpudFlowOff:
		case ESpudSuspended:
		case ESpudFlowOffAndSuspended:
	        ref->SetState(ESpudContextDelete);
	        
	        if(BindMan()->IsLastContext(opt.iContextInfo.iContextId)) // Are we about to shutdown after this?
	        	{
	           	SetTerminateError(KErrCancel); // KErrCancel is normally used to indicate "graceful" 
	        	}							   // user-initiated shutdown.
		   
	        // GUQoS ordered the shutdown: Network is still OK: initiate graceful shutdown of the lower NIF.
	        ref->NifLink()->Stop(KErrCancel, MNifIfNotify::EDisconnect);
	        break;
	    
	    case ESpudStartingSecondary:  // NIF not started: need to delete the context.
			ref->SetState(ESpudWaitLinkDown); 

			// Delete context via SpudFsm
			__FLOG_1(_L("Sending SpudFsm event EContextDelete context %d"), opt.iContextInfo.iContextId);
			rc = iPdpFsmInterface.Input(opt.iContextInfo.iContextId, EContextDelete);
			ASSERT(rc == KErrNone);
			break;

		case ESpudWaitLinkDown:
			// We're in the middle of deleting. Ignore this request.
			break;

		case ESpudLinkDown:
			// We're in the middle of deleting, but we no longer need to notify
			// GUQoS when done.
			ref->SetState(ESpudWaitLinkDown);
			break;
	
		case ESpudWaitBinderDelete: // context deleted, NIF about to be deleted: ignore.
			break;
			
		default:
			__FLOG_2(_L("Unexpected state %S(%d)"), SpudStateToText(ref->State()), ref->State());
	    	ASSERT(EFalse);
	    	break;
			}

		FillInParameters(opt, opt.iContextInfo.iContextId, rc);
        return KErrNone;
		}
		
	case KContextActivate:
		{
        ASSERT(aOption.Ptr());
		TContextParameters& opt=*reinterpret_cast<TContextParameters*>(const_cast<TUint8*>(aOption.Ptr()));
       	CSpudBinderRef* ref = NULL;
		// Validate context ID
		TRAPD(rc, ref = BindMan()->GetRefL(opt.iContextInfo.iContextId));
		if (KErrNone != rc)
			{
			opt.iReasonCode = rc; // Can supply error code only, as the context does not exist.
			__FLOG_1(_L("Error: KContextActivate specifies context %d which does not exist."), opt.iContextInfo.iContextId);
			return KErrNone;
			}
		
		if(!ref->IsBound() || ref->State() == ESpudWaitBinderDelete)
			{
			__FLOG_1(_L("Error: KContextActivate specifies context %d which is not bound to lower NIF."), opt.iContextInfo.iContextId);
			rc = KErrNotReady; // that's what the GuQoS docs say
			FillInParameters(opt, opt.iContextInfo.iContextId, rc);
			return KErrNone;
			}
			
		__FLOG_3(_L("KContextActivate context %d in state %S(%d)"), opt.iContextInfo.iContextId, SpudStateToText(ref->State()), ref->State());

        ASSERT(ref->State() == ESpudStartingSecondary || ref->State() == ESpudUp);


		// Notify SpudFsm
		__FLOG_1(_L("Sending SpudFsm event EContextActivate context %d"), opt.iContextInfo.iContextId);
		rc = iPdpFsmInterface.Input(opt.iContextInfo.iContextId, EContextActivate);
		// Set up the synchronous response
		FillInParameters(opt, opt.iContextInfo.iContextId, rc);
        return KErrNone;
		}

	case KContextQoSSet:
		{
		TContextParameters& opt=*reinterpret_cast<TContextParameters*>(const_cast<TUint8*>(aOption.Ptr()));
       	CSpudBinderRef* ref = NULL;
		// Validate context ID
		TRAPD(rc, ref = BindMan()->GetRefL(opt.iContextInfo.iContextId));
		if (KErrNone != rc)
			{
			__FLOG_1(_L("Error: KContextQoSSet specifies context %d which does not exist"), opt.iContextInfo.iContextId);
			opt.iReasonCode = rc; // Can only supply error code, as the context does not exist.
			return KErrNone;
			}		
		
		if(!ref->IsBound() || ref->State() == ESpudWaitBinderDelete)
			{
			__FLOG_1(_L("Error: KContextQoSSet specifies context %d which is not bound to the lower NIF."), opt.iContextInfo.iContextId);
			rc = KErrNotReady; // that's what the GuQoS docs say
			FillInParameters(opt, opt.iContextInfo.iContextId, rc);
			return KErrNone;
			}
		__FLOG_3(_L("KContextQoSSet context %d in state %S(%d)"), opt.iContextInfo.iContextId, SpudStateToText(ref->State()), ref->State());

		ASSERT(ref->State() == ESpudStartingSecondary
            || ref->State() == ESpudUp
            || ref->State() == ESpudFlowOff
            || ref->State() == ESpudSuspended
            || ref->State() == ESpudFlowOffAndSuspended
            || ref->State() == ESpudLinkDown );
		
#ifdef SYMBIAN_NETWORKING_UMTSR5
        RPacketQoS::TQoSR5Requested qos;
#else
    RPacketQoS::TQoSR99_R4Requested qos;
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

		// Store QoS parameters
        opt.iContextConfig.GetUMTSQoSReq(qos); 
		rc = iPdpFsmInterface.Set(opt.iContextInfo.iContextId, qos);

		// Notify SpudFsm
		if (rc == KErrNone)
			{
			__FLOG_1(_L("Sending SpudFsm event EContextQoSSet context %d"), opt.iContextInfo.iContextId);
			rc = iPdpFsmInterface.Input(opt.iContextInfo.iContextId, EContextQoSSet);
			}
		FillInParameters(opt, opt.iContextInfo.iContextId, rc);
        return KErrNone;
		}

	case KContextTFTModify:
		{
		TContextParameters& opt=*reinterpret_cast<TContextParameters*>(const_cast<TUint8*>(aOption.Ptr()));
       	CSpudBinderRef* ref = NULL;
		// Validate context ID
		TRAPD(rc, ref = BindMan()->GetRefL(opt.iContextInfo.iContextId));
		if (KErrNone != rc)
			{
			__FLOG_1(_L("Error: KContextTFTModify specifies context %d which does not exist"), opt.iContextInfo.iContextId);
			opt.iReasonCode = rc; // Can only supply error code as the context does not exist.
			return KErrNone;
			}

		// keep local reference to Primary Context up to date
		if (opt.iContextType == EPrimaryContext) 
			{
			iPrimaryContextId = opt.iContextInfo.iContextId;
			}
			
		if(!ref->IsBound() || ref->State() == ESpudWaitBinderDelete)
			{
			__FLOG_1(_L("Error: KContextTFTModify specifies context %d which is not bound to the lower NIF."), opt.iContextInfo.iContextId);
			rc = KErrNotReady; // that's what the GuQoS docs say
			FillInParameters(opt, opt.iContextInfo.iContextId, rc);
			return KErrNone;
			}
		__FLOG_3(_L("KContextTFTModify context %d in state %S(%d)"), opt.iContextInfo.iContextId, SpudStateToText(ref->State()), ref->State());

		ASSERT(ref->State() == ESpudStartingSecondary || 
			   ref->State() == ESpudUp ||
		       ref->State() == ESpudWaitLinkDown  ||
		       ref->State() == ESpudLinkDown ||
		       ref->State() == ESpudGettingNegQoS);
		// It is assumed that the TFT can be modified before the ctx reported activation 

		// Store TFT parameters
		TTFTInfo tft;
        opt.iContextConfig.GetTFTInfo(tft); 

        rc = iPdpFsmInterface.Set(opt.iContextInfo.iContextId, tft);
		if (rc != KErrNone)
			{
			__FLOG_1(_L("Error %d setting TFT"), rc);
			FillInParameters(opt, opt.iContextInfo.iContextId, rc);
			return KErrNone;
			}
        rc = iPdpFsmInterface.Set(opt.iContextInfo.iContextId, opt.iTFTOperationCode);
		if (rc != KErrNone)
			{
			__FLOG_1(_L("Error %d setting TFT operation code"), rc);
			FillInParameters(opt, opt.iContextInfo.iContextId, rc);
			return KErrNone;
			}

		// Notify SpudFsm
		__FLOG_1(_L("Sending SpudFsm event EContextTFTModify context %d"), opt.iContextInfo.iContextId);
		rc = iPdpFsmInterface.Input(opt.iContextInfo.iContextId, EContextTFTModify);

		FillInParameters(opt, opt.iContextInfo.iContextId, rc);
        return KErrNone;
		}

	case KContextModifyActive:
		{
		TContextParameters& opt=*reinterpret_cast<TContextParameters*>(const_cast<TUint8*>(aOption.Ptr()));
       	CSpudBinderRef* ref = NULL;
		// Validate context ID
		TRAPD(rc, ref = BindMan()->GetRefL(opt.iContextInfo.iContextId));
		if (KErrNone != rc)
			{
			__FLOG_1(_L("Error: KContextModifyActive specifies context %d which does not exist"), opt.iContextInfo.iContextId);
			opt.iReasonCode = rc;
			return KErrNone;
			}
			
		if(!ref->IsBound() || ref->State() == ESpudWaitBinderDelete)
			{
			__FLOG_1(_L("Error: KContextModifyActive specifies context %d which is not bound to the lower NIF."), opt.iContextInfo.iContextId);
			rc = KErrNotReady; // that's what the GuQoS docs say
			FillInParameters(opt, opt.iContextInfo.iContextId, rc);
			return KErrNone;
			}

		// Notify SpudFsm
		__FLOG_1(_L("Sending SpudFsm event EContextModifyActive context %d"), opt.iContextInfo.iContextId);
		rc = iPdpFsmInterface.Input(opt.iContextInfo.iContextId, EContextModifyActive);

		FillInParameters(opt, opt.iContextInfo.iContextId, rc);
        return KErrNone;
		}
		
	case KInitialisePdpFsm:
      {
      MPdpFsmInterface* pdpFsm = *reinterpret_cast<MPdpFsmInterface**>(const_cast<TUint8*>(aOption.Ptr()));
      iPdpFsmInterface.Init(pdpFsm);
      return KErrNone;
      }

	default:
		__FLOG_1(_L("Unhandled event %d"), aName);
		break;
		}
	return KErrNotSupported;
	}

/**
Receives link up indication from a lower NIF.

@param aContextId PDP context ID of the associated lower NIF
*/
void CSpudMan::LinkLayerUp(TContextId aContextId)
	{
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	ASSERT(ref->IsBound() && ref->State() != ESpudWaitBinderDelete);
	
	__FLOG_3(_L("Lower NIF LinkLayerUp on context[%d], in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
	switch (ref->State())
		{
	case ESpudStartingPrimaryLowerNif:
		{
	    ref->SetState(ESpudUp);
		if (AreQoSEventsEnabled())
			{
			SendPrimaryContextCreated(aContextId, KErrNone);
			}
		// If QoS is not yet enabled, SendPrimaryContextCreated() will be called when it is

		Notify()->LinkLayerUp();

		// If mobile IP is enabled, this progress notification should not be sent here.
		// This will need to be addressed if MIP is ever enabled for UMTS.
		Notify()->IfProgress(KLinkLayerOpen, KErrNone);
		}
		break;

	case ESpudStartingSecondaryLowerNif:
		{
		// lower nif is up, now get the Negotiated QoS
	    ref->SetState(ESpudGettingNegQoS);
		
		TInt rc = iPdpFsmInterface.Input(aContextId, EGetNegQoS);
		ASSERT(rc == KErrNone);
		// Now wait for retrieval of negotiated QoS.
		// The following 2 functions will be fired from CSpudMan::HandleGetNegQoSEvent():
		//    FillInContextConfig(iTempContextConfig, aContextId);
		//    SendContextActivateEvent(aContextId, iTempContextConfig, KErrNone);
		}
		break;
		
	default:
		__FLOG_3(_L("Lower NIF LinkLayerUp on context[%d], in unexpected state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
		ASSERT(EFalse);
		}
	}

/**
Receives link down indication from a lower NIF.

@param aContextId Valid PDP context ID of the associated lower NIF
@param aReason A Symbian OS error code indicating the reason for the link closing down
@param aAction The action that should be taken as a result of link layer down being signalled
*/
void CSpudMan::LinkLayerDown(TContextId aContextId, TInt aReason, MNifIfNotify::TAction aAction)
	{
	__FLOG_3(_L("CSpudMan::LinkLayerDown: context %d reason %d action %d"), aContextId, aReason, aAction);

	if (aAction == MNifIfNotify::ENoAction)
		{
		// This call indicates a renegotiation of the lower link, not that the link has
		// actually gone down. This is of no interest to SPUD.
		__FLOG_0(_L("Ignoring MNifIfNotify::ENoAction"));
		return;
		}

	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());

	
	TBool isUpperFlowOn = EFalse; // Can GUQoS attempt to send on this PDP context?
	
	switch (ref->State())
		{
	case ESpudUp:
		isUpperFlowOn = ETrue; // ESpudUp is the only state where GUQoS is allowed to send on us.
		// Fall through
	case ESpudFlowOff:// NIF & context were activated succcessfully.
	case ESpudSuspended:
	case ESpudFlowOffAndSuspended:
		ref->SetState(ESpudLinkDown); // Must Notify GUQoS that context is down.
		
		// Must inform upper layers they can no longer send on this context.
		if (isUpperFlowOn && AreQoSEventsEnabled()) 
			{
			// At this point the context status does not indicate to GUQoS that the context is about to go down.
			// Most likely, the status is EStatusActive, indicating that the context can handle requests.
			// Since the lower NIF is down, we are about to delete the context, so logically, this context
			// is EStatusDeactivating.(From spudman's PoV, a context is a union of lower NIF and ETel's RPacketContext)
			// GUQoS does not know this, it may try to issue requests,  
			// such as send packets on this context, modify TFT, modify QoS, etc.
			// Note: this does not seem to happen, but it is feasible.
			
			// N.B. Ideally, we should signal context change status in not just in EStateUp, but in 
			// EStateFlowOff etc, i.e. any state in which the context is alive. Unfortunately, GUQoS 
			// does not allow us to do this - there is no pure "ContextStatusChange" upcall on GUQoS.
			// Context Parameters Change upcall should not be used, because GUQoS interprets it as a
			// QoS parameters change, and sends QoSEventAdapt to the QoS framework. This is wrong.
			//
			// We could send ContextBlocked in FlowOff / Suspended states, however, this is problematic,
			// because GUQoS has already received one of these notifications. We could "trick" it and cycle
			// the state (send Flow On / Resume, then Flow Off / Suspend again), but this hackery should be
			// avoided unless absolutely necessary. So far, GUQoS does not seem to do anything beside sending
			// a packet on a deactivaing context, so we only worry about it.
			
			// N.B. SpudFSM is asynchronous, it will not issue a an actual deletion request on ETel until this RunL
			// returns. This means that the context status will not be updated to EStatusDeactiving in ETel 
			// until then, so when we fill in context paramters, the status is going to be EStatusActive.
			// We "sneak in" the correct status by overriding it. Doing it in SpudFSM would be wrong, because
			// SpudFSM reflects the ETel side of the context, so overriding the status there is misleading,
			// because it can be used internally by SpudFSM / SpudTel.
			
			__FLOG_0(_L("Upper flow on the context is On: flow Off GUQoS to prevent it from sending on a context with dead lower NIF."));
			iContextStatusOverride = RPacketContext::EStatusDeactivating; 
			SendContextBlockedEvent(aContextId); // resets the override
			isUpperFlowOn = EFalse; 
			}
		
		// The only way we can notify GUQoS once the context has been activated is
		// to delete it via the SpudFsm
		__FLOG_1(_L("Sending SpudFsm event EContextDelete context %d"), aContextId);
		rc = iPdpFsmInterface.Input(aContextId, EContextDelete);
		__FLOG_1(_L("SpudFsm::Input() returned status %d"), rc);
		if (rc == KErrNotReady) //This is becuase the FSM has been shut down via UMTSGPRSSCPR
			{
			HandleContextDeleteEvent(aContextId, aReason); //Just pretend that the fsm returned something sensible
			}
		ASSERT(rc == KErrNone || rc == KErrNotReady);
		break;

	case ESpudGettingNegQoS: // CNifIfLink::Stop will handle this just as if the NIF was still being CNifIfLink::Start'ed.
	case ESpudStartingSecondaryLowerNif: // Attempt to bring up the lower NIF was made.
		ref->SetState(ESpudStartingSecondary); // GUQoS should delete us later. 
		// Notify GUQoS that Activation failed. It will take the control from here, most likely deleting the context.
		FillInContextConfig(iTempContextConfig, aContextId);
		if(ref->Error() != KErrNone)
			{
			SendContextActivateEvent(aContextId, iTempContextConfig, ref->Error());
			}
		else
			{
			SendContextActivateEvent(aContextId, iTempContextConfig, aReason);
			}
		break;
	
	case ESpudContextDelete:
		ref->SetState(ESpudWaitLinkDown); // GUQoS triggered us: don't need to notify it.

		// Delete context via SpudFsm
		__FLOG_1(_L("Sending SpudFsm event EContextDelete context %d"), aContextId);
		rc = iPdpFsmInterface.Input(aContextId, EContextDelete);
		__FLOG_1(_L("SpudFsm::Input() returned status %d"), rc);
		ASSERT(rc == KErrNone);
		break;
		
	case ESpudStartingSecondary:
	case ESpudLinkDown: // Added for INC066156.
	case ESpudWaitLinkDown:
		// Link has finally gone down. Context is already deleted.
    	__FLOG_1(_L("Lower NIF reported LinkLayerDown: mark the binder for context[%d] for async deletion"), aContextId);
		DisposeOfBinder(ref);
		break;
	
	case ESpudStartingPrimaryLowerNif:
		{
		SetTerminateError(aContextId, aReason); // SPUD is going to terminate.
		SetTerminateError(KErrCouldNotConnect);
		
		// The primary context is managed by SPUD. We delete it ourselves.
		__FLOG_1(_L("Sending SpudFsm event EContextDelete context %d"), aContextId);
		ref->SetState(ESpudWaitLinkDown);
		rc = iPdpFsmInterface.Input(aContextId, EContextDelete);
		__FLOG_1(_L("SpudFsm::Input() returned status %d"), rc);
		ASSERT(rc == KErrNone);
		
		// Meanwhile, notify GUQoS about the failed primary creation
		if (AreQoSEventsEnabled())
			{
			SendPrimaryContextCreated(aContextId, aReason);
			}
		break;
		}	
			
	default:
		__FLOG_2(_L("Unexpected state %S(%d)"), SpudStateToText(ref->State()), ref->State());
		ASSERT(EFalse);
		break;
		}
	}

/**
Receives flow off indication from a lower NIF.

@param aContextId Valid PDP context ID of the associated lower NIF
*/
void CSpudMan::StopSending(TContextId aContextId)
	{
	__FLOG_1(_L("CSpudMan::StopSending context %d"), aContextId);
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
	switch (ref->State())
		{
	case ESpudUp:
		ref->SetState(ESpudFlowOff);
		if (AreQoSEventsEnabled())
			{
			SendContextBlockedEvent(aContextId);
			}
		break;

	case ESpudSuspended:
		ref->SetState(ESpudFlowOffAndSuspended);
		break;

	case ESpudContextDelete:
	case ESpudWaitLinkDown:
	case ESpudLinkDown: //sometimes this leaks in
		// ignore
		break;	
	
	default:
		// error
		__FLOG_2(_L("Unexpected state %S(%d)"), SpudStateToText(ref->State()), ref->State());
		ASSERT(EFalse);
		break;
		}
	}

/**
Receives flow on indication from a lower NIF.

@param aContextId Valid PDP context ID of the associated lower NIF
*/
void CSpudMan::StartSending(TContextId aContextId)
	{
	__FLOG_1(_L("CSpudMan::StartSending context %d"), aContextId);
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	__ASSERT_ALWAYS(rc == KErrNone, Panic());
	__FLOG_3(_L("context %d is in state %S(%d)"), aContextId, SpudStateToText(ref->State()), ref->State());
	switch (ref->State())
		{
	case ESpudGettingNegQoS: // Lower NIF for the 2ndary ctx is up, waiting for negotiated QoS.
		break; // GUQoS, TCP/IP stack will be notified when negotiated QoS is retrieved. 
		// Notes: 
		// 1.there is a *potential* race condition where the negotiated QoS is retrived before lower NIF signals StartSending.
		// This does not seem to happen with PPP. If this happens somehow, the ctx will be in the "UP" state and it will be handled correctly.
		// 2. Theoretically, the lower NIF may report LinkLayerUp (triggering retrieval of negotiated QoS), then signal 
		// StartSending much later (e.g.  a PPP implementation singalling LinkLayerUp from LCP, but StartSending from NCP).
		// SPUD CANNOT HANDLE THIS.
		// Existing Raw IP and PPP NIFs signal StartSending immediately after LinkLayerUp, so this has no consequences.

	case ESpudUp:
		// This can happen for the initial StartSending after the lower NIF comes up
		// Treat it the same as ESpudFlowOff and fall through.
	case ESpudFlowOff:
		ref->SetState(ESpudUp);

		// This must only be done AFTER the KPrimaryContextCreated event is sent (which in this state it is)
		// It's not clear if StartSending is needed/allowed in addition to the GUQoS event.
	    BindMan()->SpudProtocol()->DoStartSending();

		if (AreQoSEventsEnabled())
			{
			SendContextUnblockedEvent(aContextId);
			}
		break;

	case ESpudFlowOffAndSuspended:
		ref->SetState(ESpudSuspended);
		break;

	case ESpudSuspended:
	case ESpudContextDelete:
	case ESpudWaitLinkDown:
		// ignore
		__FLOG_1(_L("Ignored StartSending on context %d (this should be OK)"), aContextId);
		break;	
	
	default:
		// We have encountered a serious problem. If we get StartSending before reaching the ESpudUp
		// state, we'll lose it and the upper networking protocol will never be notified.
		// As long as the lower NIF calls LinkLayerUp before StartSending, we'll be fine.
		__FLOG_1(_L("Can't send unblocked event now on context %d; this may cause problems"), aContextId);
		ASSERT(EFalse);
		break;
		}
	}


//*****************************************************************************
// Event senders to GUQoS
//*****************************************************************************

/**
Sends event to GUQoS.

@param aName event identifier
@param aOption TPckg<> event data
*/
void CSpudMan::RaiseEvent(TUint aName, TDes8& aOption) const
	{
	__FLOG_2(_L("Sending event %S(%d) to GUQoS"), SpudFsmEventToText(aName), aName);
	iQosEventHandler->Event(reinterpret_cast<CProtocolBase*>(iBindMan->SpudMux()), aName, aOption); 
	}

/**
Fills in common event parameters for the given context.

@param aParams parameter structure
@param aContextId Valid PDP context ID
*/
void CSpudMan::FillInParameters(TContextParameters& aParams, TContextId aContextId, TInt aError) const
	{
   	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = BindMan()->GetRefL(aContextId));
	ASSERT(rc == KErrNone);
	
	TContextType type(EContextTypeUnknown);
	if (rc == KErrNone)
		{
		switch (ref->State())
			{
		case ESpudHaveQos:
		case ESpudCreatingPrimary:
		case ESpudStartingPrimary:
			type = EPrimaryContext;
			break;
			
		case ESpudStartingSecondary:
			type = ESecondaryContext;
			break;
		
		default:
			type = (aContextId == iPrimaryContextId) ? EPrimaryContext : ESecondaryContext;;
			break;
			}
		}
    aParams.iContextType = type; // Context type
    aParams.iReasonCode = aError;  // Error code
    //aParams.iContextInfo.iStatus = StateToStatus(*ref);
    iPdpFsmInterface.Get(aContextId, aParams.iContextInfo.iStatus);
    aParams.iContextInfo.iContextId = aContextId;
	}

/**
Fill in context configuration parameter structure using SpudFsm's parameters.

@param aConfig parameter structure
@param aContextId PDP context ID
*/
void CSpudMan::FillInContextConfig(TContextConfig& aConfig, TContextId aContextId) const
	{
#ifdef SYMBIAN_NETWORKING_UMTSR5
		RPacketQoS::TQoSR5Negotiated qos;
#else
		RPacketQoS::TQoSR99_R4Negotiated qos;
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

	iPdpFsmInterface.Get(aContextId, qos);
	aConfig.SetUMTSQoSNeg(qos);

	iPdpFsmInterface.Get(aContextId, iTempTftInfo);
	aConfig.SetTFTInfo(iTempTftInfo);

	iPdpFsmInterface.Get(aContextId, iTempGprsContext);
	aConfig.SetContextConfig(iTempGprsContext);
	}

/**
Sends KPrimaryContextCreated event to GUQoS.
*/
void CSpudMan::SendPrimaryContextCreated(TContextId aContextId, TInt aError)
	{
	__FLOG_2(_L("SendPrimaryContextCreated context %d error %d"), aContextId, aError);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	// We make this assumption in various places
	ASSERT(aContextId == 0);

	TContextParameters primaryContextCreatedEvent;
	FillInParameters(primaryContextCreatedEvent, aContextId, aError);
	TPckg<TContextParameters> event(primaryContextCreatedEvent);
	RaiseEvent(KPrimaryContextCreated, event);
	}


/**
Sends KSecondaryContextCreated event to GUQoS.

@param aContextId Context ID
@param aError error code
*/
void CSpudMan::SendSecondaryContextCreated(TContextId aContextId, TInt aError)
	{
	__FLOG_2(_L("SendSecondaryContextCreated context %d error %d"), aContextId, aError);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}

	// We make this assumption in various places
	ASSERT(aContextId != 0);
	
	TContextParameters event;
	FillInParameters(event, aContextId, aError);
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KSecondaryContextCreated, eventPckg);
	}


/**
Sends KContextBlockedEvent event to GUQoS.
*/
void CSpudMan::SendContextBlockedEvent(TContextId aContextId)
	{
	__FLOG_2(_L("SendContextBlockedEvent context %d error %d"), aContextId, KErrNone);
	
	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}

	TContextParameters event;
	FillInParameters(event, aContextId);
	
	/** The status we want to signal to GUQoS may be different from the context status we get from SpudFSM
	E.g., if we are about to deactivate the context, the logical status of the context is EStatusDeactivating, rather than
	EStatusActive, even though that's what SpudFSM will tell us */
	if(RPacketContext::EStatusUnknown != iContextStatusOverride)
		{
		__FLOG_2(_L("SendContextBlockedEvent: context status overriden to %d, original: %d."), 
			iContextStatusOverride, event.iContextInfo.iStatus);
			
		event.iContextInfo.iStatus = iContextStatusOverride;			
		iContextStatusOverride = RPacketContext::EStatusUnknown; 
		}
	
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KContextBlockedEvent, eventPckg);
	}

/**
Sends KContextUnblockedEvent event to GUQoS.
*/
void CSpudMan::SendContextUnblockedEvent(TContextId aContextId)
	{
	__FLOG_2(_L("SendContextUnblockedEvent context %d error %d"), aContextId, KErrNone);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	
	TContextParameters event;
	FillInParameters(event, aContextId);
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KContextUnblockedEvent, eventPckg);
	}

/**
Sends KContextQoSSetEvent event to GUQoS.

@param aContextId Context ID
@param aError error code
*/
void CSpudMan::SendContextQoSSetEvent(TContextId aContextId, TInt aError)
	{
	__FLOG_2(_L("SendContextQoSSetEvent context %d error %d"), aContextId, aError);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	
	TContextParameters event;
	FillInParameters(event, aContextId, aError);
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KContextQoSSetEvent, eventPckg);
	}

/**
Sends KContextTFTModifiedEvent event to GUQoS.

@param aContextId Context ID
#param aTFTOperationCode TFT operation code
@param aError error code
*/
void CSpudMan::SendContextTFTModifiedEvent(TContextId aContextId, TTFTOperationCode aTFTOperationCode, TInt aError)
	{
	__FLOG_2(_L("SendContextTFTModifiedEvent context %d error %d"), aContextId, aError);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	
	TContextParameters event;
	FillInParameters(event, aContextId, aError);
	// Also need to fill in TTFTOperationCode, an undocumented requirement
	event.iTFTOperationCode = aTFTOperationCode;
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KContextTFTModifiedEvent, eventPckg);
	}

/**
Sends KContextModifyActiveEvent event to GUQoS.

@param aContextId Context ID
@param aContextConfig Configuration parameters for this context
@param aError error code
*/
void CSpudMan::SendContextModifyActiveEvent(TContextId aContextId, TContextConfig& aContextConfig, TInt aError)
	{
	__FLOG_2(_L("SendContextModifyActiveEvent context %d error %d"), aContextId, aError);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	
	TContextParameters event;
	FillInParameters(event, aContextId, aError);
	event.iContextConfig = aContextConfig;
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KContextModifyActiveEvent, eventPckg);
	}

/**
Sends KContextActivateEvent event to GUQoS.

@param aContextId Context ID
@param aContextConfig Configuration parameters for this context
@param aError error code
*/
void CSpudMan::SendContextActivateEvent(TContextId aContextId, TContextConfig& aContextConfig, TInt aError)
	{
	__FLOG_2(_L("SendContextActivateEvent context %d error %d"), aContextId, aError);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	
	TContextParameters event;
	FillInParameters(event, aContextId, aError);
	event.iContextConfig = aContextConfig;
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KContextActivateEvent, eventPckg);
	}

/**
Sends KContextParametersChangeEvent event to GUQoS.

@param aContextId Context ID
@param aContextConfig Configuration parameters for this context
@param aError error code
*/
void CSpudMan::SendContextParametersChangeEvent(TContextId aContextId, TContextConfig& aContextConfig, TInt aError)
	{
	__FLOG_2(_L("SendContextParametersChangeEvent context %d error %d"), aContextId, aError);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	
	TContextParameters event;
	FillInParameters(event, aContextId, aError);
	event.iContextConfig = aContextConfig;
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KContextParametersChangeEvent, eventPckg);
	}

/**
Sends KContextDeleteEvent event to GUQoS.

@param aContextId Context ID
*/
void CSpudMan::SendContextDeleteEvent(TContextId aContextId)
	{
	__FLOG_2(_L("SendContextDeleteEvent context %d error %d"), aContextId, KErrNone);

	if (!iQosEventsEnabled)
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	
	TContextParameters event;
	FillInParameters(event, aContextId);
	TPckg<TContextParameters> eventPckg(event);
	RaiseEvent(KContextDeleteEvent, eventPckg);
	}

/**
Sends KNetworkStatusEvent event to GUQoS.

@param aEventCode Event code
@param aStatus Network status
*/
void CSpudMan::SendNetworkStatusEvent(TNetworkEventCode aEventCode, RPacketService::TStatus aStatus)
	{
	__FLOG_2(_L("SendNetworkStatusEvent event code %d status %d"), aEventCode, aStatus);

	if (!iQosEventsEnabled) // QoS events not turned on yet, or have been turned off by GUQoS
		{
		__FLOG_0(_L("Error: sending events is disabled - GuQoS not present ?"));
		return;
		}
	
	TNetworkParameters event;
	event.iNetworkEventCode = aEventCode;
	event.iNetworkStatus = aStatus;
	TPckg<TNetworkParameters> eventPckg(event);
	RaiseEvent(KNetworkStatusEvent, eventPckg);
	}


//*****************************************************************************
// CNifIfLink methods
//*****************************************************************************


/**
Return the link protocol handler object.

@param aName Protocol name desired
@return Pointer to link protocol handler (ownership is transferred)
*/
CNifIfBase* CSpudMan::GetBinderL(const TDesC& aName)
	{
    __FLOG_1(_L("CSpudMan::GetBinderL %S"), &aName);
    iName = aName;
	return static_cast<CNifIfBase*>(iBindMan->TransferSpudMux());
	}

/**
Return information about the SPUD NIF.

@param aInfo Receives the NIF interface info
*/
void CSpudMan::Info(TNifIfInfo& aInfo) const
	{
	CSpudBinderRef* ref = NULL;
	// Get the binder for the first (default) lower NIF.
	TRAPD(err, ref = iBindMan->GetAnyRefL());
	if (err == KErrNone)
		{
		// Read the protocol supported value from the lower NIF
		ref->NifLink()->Info(aInfo);
		ASSERT(aInfo.iFlags == (KNifIfIsBase | KNifIfUsesNotify | KNifIfIsLink | KNifIfCreatedByFactory | KNifIfCreatesBinder));
		}
	else
		{
		aInfo.iProtocolSupported=KProtocolUnknown;
		}
	
	aInfo.iVersion = TVersion(KSpudMajorVersionNumber, KSpudMinorVersionNumber, KSpudBuildVersionNumber);
	aInfo.iFlags = KNifIfIsBase | 
                   KNifIfUsesNotify | 
                   KNifIfIsLink | 
                   KNifIfCreatedByFactory | 
                   KNifIfCreatesBinder;
	aInfo.iName = KSpudName;
	}

/**
Processes notifications from Agent

@param aEvent Event type
@param aInfo Data relating to event

@return Error code
*/
TInt CSpudMan::Notification(TAgentToNifEventType aEvent, void * aInfo)
	{
	__FLOG_1(_L("CSpudMan::Notification event %d"), aEvent);
	TInt rc = KErrNotSupported;
	switch (aEvent)
		{
	case EAgentToNifEventTypeModifyInitialTimer:
	case EAgentToNifEventTypeDisableTimers:
	case EAgentToNifEventTypeEnableTimers:
	case EAgentToNifEventTsyConfig:
	case EAgentToNifEventTsyConnectionSpeed:
		// Send notification to all lower NIFs
		rc = KErrNotReady;
		TContextId i;
		for (i=0; i < KMaxPdpContexts; ++i)
			{
			CSpudBinderRef* ref = NULL;
			TRAP(rc, ref = iBindMan->GetRefL(i));
			if (rc == KErrNone)
				{
				rc = ref->NifLink()->Notification(aEvent, aInfo);
				}
			}
		break;

	case EAgentToNifEventTypeGetDataTransfer:
		{
		TPckg<RPacketContext::TDataVolume>* totalDataPackage = (TPckg<RPacketContext::TDataVolume>*) aInfo;
		RPacketContext::TDataVolume& totalData = (*totalDataPackage)();
		totalData.iBytesSent = 0;
		totalData.iOverflowCounterSent = 0;
		totalData.iBytesReceived = 0;
		totalData.iOverflowCounterReceived = 0;

		RPacketContext::TDataVolume data;
		TPckg<RPacketContext::TDataVolume> dataPackage(data);

		// Add up data reported by all NIFs
		rc = KErrNotReady;
		TContextId i;
		for (i=0; i < KMaxPdpContexts; ++i)
			{
			CSpudBinderRef* ref = NULL;
			TRAP(rc, ref = iBindMan->GetRefL(i));
			if (rc == KErrNone)
				{
				rc = ref->NifLink()->Notification(aEvent, &dataPackage);
				if (rc == KErrNone)
					{
					totalData.iBytesSent += data.iBytesSent;
					totalData.iOverflowCounterSent += data.iOverflowCounterSent;
					totalData.iBytesReceived += data.iBytesReceived;
					totalData.iOverflowCounterReceived += data.iOverflowCounterReceived;
					}
				}
			}
		break;
		}

	case EAgentToNifEventTypeDisableConnection:
		// TODO: what to do with this?
	default:
		__FLOG_1(_L("Notification event %d was ignored"), aEvent);
		break;
		}

	return rc;
	}


/**
Start the link.
At this point only the primary PDP context is valid.

@return Error code
*/
TInt CSpudMan::Start()
	{
	__FLOG_1(_L("CSpudMan::Start(0x%x)"), this);

	// SpudTel needs TSY name from CommDb
	TName tsyName;
	ReadTsyName(tsyName);
	
	// Initialise SpudFsm
	TRAPD(err, InitPdpFsmInterfaceL());
	if (err != KErrNone)
	  {
	  __FLOG_1(_L("CSpudMan::Start: Failed to initialise the PDP Fsm Interface,Error = %d"),err);
	  return err;
     }

	// Open SpudFsm
	TRAP(err, iPdpFsmInterface.OpenL(this, tsyName));
	if (err != KErrNone)
	  {
	  __FLOG_1(_L("CSpudMan::Start: Failed to open the PDP Fsm Interface,Error = %d"),err);
	  return err;
     }


	// re-initialise the temporary data structure before retrieving 
	// GPRS config parameters from CommDB
   __FLOG_0(_L("CSpudMan::Start: Getting default GPRS settings from Commdb"));
	RetrieveGprsConfig(iTempGprsContext);
	
	TRAP(err, SetupSipServerAddrRetrievalL(iTempGprsContext.iProtocolConfigOption););
	
#ifdef SYMBIAN_NETWORKING_UMTSR5
	// Add the IMCN Signalling Status flag. IM CN status flag is retrieved from the Database
	// Request For the status of IM CN dedicated signalling context
	TRAP(err,SetIMCNSignallingFlagPcoL(iTempGprsContext.iProtocolConfigOption));
	
	// Not sure what can be done after trapping the error, because its not an error condition for starting of 
	// Primary PDP context.
#ifdef __FLOG_ACTIVE
	if(err != KErrNone)
		{
		__FLOG_1(_L("CSpudMan::Start: Failed to set IM CN signalling Flag.Error = %d"),err);
		}
#endif
#endif // SYMBIAN_NETWORKING_UMTSR5
	
	iPdpFsmInterface.Set(iPrimaryContextId, iTempGprsContext);
	if (err != KErrNone)
        {
        __FLOG_1(_L("CSpudMan::Start: Setup sip server address retrieval. Failed with %d"),err);
        return err;
        }
        
	if (iParkedDefaultQoS != NULL) 
        {
        __FLOG_0(_L("CSpudMan::Start: Found parked QoS settings from GuQoS"));
        
        TPtr8 qos(iParkedDefaultQoS->Des());
        GuqosInput (KNifSetDefaultQoS, qos);
        
        delete iParkedDefaultQoS;
        iParkedDefaultQoS = NULL;
        }

	CSpudBinderRef* ref = NULL;
	// Get the binder for the first (default) lower NIF.
	TRAP(err, ref = iBindMan->GetRefL(iPrimaryContextId));
	if (err != KErrNone)
		{
		__FLOG_0(_L("CSpudMan::Start: Error - no context could be found"));
		return err;
		}

    ASSERT(ref->State() == ESpudInactive || ref->State() == ESpudHaveQos);
    ASSERT(ref->State() != ESpudWaitBinderDelete);
    
    if (ref->State() == ESpudWaitBinderDelete)
    	{
	    return KErrNotReady;
    	}
    
	if (ref->State() != ESpudHaveQos)
		{
		__FLOG_0(_L("CSpudMan::Start: No QoS parameters have been set - is GuQoS present?"));
	
		// Sets default QoS parameters because either
      //    1) they weren't supplied by GUQoS - this shouldn't happen
      //    2) or GuQoS has been configured out
#ifdef SYMBIAN_NETWORKING_UMTSR5
		// Sets default R5 QoS parameters because they weren't supplied by GUQoS.
			RPacketQoS::TQoSR5Requested qos;
			ReadDefaultR5QoS(qos);
#else
			RPacketQoS::TQoSR99_R4Requested qos;
			ReadDefaultQoS(qos);
#endif 
// SYMBIAN_NETWORKING_UMTSR5 		
			iPdpFsmInterface.Set(iPrimaryContextId, qos); // ignore any error
		
		}

	// Set default TFT
	TTFTInfo tft;
    iPdpFsmInterface.Set(iPrimaryContextId, tft); // ignore any error
    

	// Have Etel create a context
	ref->SetState(ESpudCreatingPrimary);
	__FLOG_1(_L("CSpudMan::Start: Sending SpudFsm event ECreatePrimaryPDPContext context %d"), iPrimaryContextId);
	TInt rc = iPdpFsmInterface.Input(iPrimaryContextId, ECreatePrimaryPDPContext);
	// TODO: handle errors properly
	ASSERT(rc == KErrNone);
	rc = rc; // Eliminate compiler warning in release builds

	return KErrNone;
    }

/**
Cleanly stop the link.

@param aReason The reason the link is going down
@param aAction The action to take once the link is down
*/
void CSpudMan::Stop(TInt aReason, MNifIfNotify::TAction aAction)
	{
	__FLOG_3(_L("CSpudMan::Stop: reason %d action %d. %d contexts exist."), aReason, aAction, BindMan()->NumContexts());
	ASSERT(BindMan()->NumContexts()); // Primary PDP context is created in the factory.
	
	SetTerminateError(aReason); // Store this error code for use when the SPUD goes down
	if (AreQoSEventsEnabled())
		{
		// Spud was administratively stopped. It can be some time before SPUD signals LinkLayerDown.
		// In the meanwhile, we can receive requests from GUQoS that can interfere with the shutdown.
		// To prevent this, we tell GUQoS to stop bothering SPUD.
		// GUQoS returns the favour by turning off the NIF events within this very call.
		// This means we will not send KNetworkInterfaceDown again, even though we'll try.
		// *********************************************************************************************
		// N.B.: "DEF055691 	GUQoS crashes at shutdown": This defect results in GUQoS crash if the stack
	    // closes flows after the NIF proxy was deleted by GUQoS, as a result of KNetworkInterfaceDown event.
	    // As a temporary workaround, this call is disabled, and KNetworkInterfaceDown event is signalled only
	    // from the destructor. Once this defect is fixed, the following line must be uncommented: 
	    // SendNetworkStatusEvent(KNetworkInterfaceDown, RPacketService::EStatusUnattached);
		//**********************************************************************************************
		}
		
	
	// Send Stop to all lower NIFs that were started but not stopped yet
	TContextId i;
	for (i=0; i < KMaxPdpContexts; ++i)
		{
		StopContext(i, aReason, aAction);
		}		
	// Eventually, the last lower NIF will call LinkLayerDown to trigger the final cleanup
    }

/**
Cleanly stop a context.

@param aReason The reason the link is going down
@param aAction The action to take once the link is down
@param aContextId context
*/
void CSpudMan::StopContext(TContextId aContextId, TInt aReason, MNifIfNotify::TAction aAction)
	{
	CSpudBinderRef* ref = NULL;
	TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
	if (rc == KErrNone && // Binder exists
		ref->IsBound()) // Is bound to a lower NIF.
		{
		
		// Save the Context failure reason
        if (ref->Error() == KErrNone) 
        	{
        	ref->SetError(aReason);
			}

		switch(ref->State()) // NIFs in some states are not eligible for Stop.
			{
			// Context created and NIF started.
			case ESpudStartingPrimaryLowerNif: // Waiting for LinkLayerUp/Down
			case ESpudStartingSecondaryLowerNif:// Waiting for LinkLayerUp/Down
			case ESpudGettingNegQoS: // Waiting for retrieval of negotiated QoS, context activated
			case ESpudUp: // LinkLayerUp received, NIF is up.
			case ESpudFlowOff: // LinkLayerUp received, NIF is up.
			case ESpudSuspended: // LinkLayerUp received, NIF is up.
			case ESpudFlowOffAndSuspended: // LinkLayerUp received, NIF is up.
				__FLOG_2(_L("Lower NIF binder for context[%d] is in state[%S]: Stopping lower NIF."),aContextId,SpudStateToText(ref->State()));
				// Stop the NIF and delete the context via SpudFsm: 
				ref->NifLink()->Stop(aReason, aAction);
				// stay in the Up state so that that GUQoS is notified.
				break;
				
			// Context is being created
			case ESpudCreatingPrimary:
				// SpudFsm will clean up the context and generate a context created event with an error
				rc = iPdpFsmInterface.Input(aContextId, ECancelContextCreate);
				break;
				
			// Context created, but NIF not started
			case ESpudStartingPrimary: 
				ref->SetState(ESpudWaitLinkDown);
				// Any outstanding  SpudFsm request will be cancelled by this delete request.
				rc = iPdpFsmInterface.Input(aContextId, EContextDelete);
				aReason = (KErrNone != aReason) ? aReason : KErrCancel; // Must not be KErrNone.
				SendPrimaryContextCreated(aContextId, aReason);
				break;
				
			case ESpudStartingSecondary: 	
				// Delete the context via SpudFsm
				ref->SetState(ESpudLinkDown); // We'll notify GUQoS from deletion event handler.
				__FLOG_1(_L("Context[%d] created: Sending SpudFsm event EContextDelete"), aContextId);
					
				// Any outstanding  SpudFsm request will be cancelled by this delete request.
				rc = iPdpFsmInterface.Input(aContextId, EContextDelete);
				ASSERT(rc == KErrNone);
				break;
				
			// Can't call Stop: the NIF either not started, or stopped already
			case ESpudContextDelete: // Context deleted by GUQoS, Stop was called.
			case ESpudWaitLinkDown: // Stop was called, waiting for LinkLayerDown
			case ESpudWaitBinderDelete: // LinkLayerDown received, queued for deletion
			case ESpudLinkDown:	    // LinkLayerDown received, not queued for deletion.
				__FLOG_2(_L("Lower NIF binder for context[%d] is in state[%S], and is not eligible for Stop."),aContextId,SpudStateToText(ref->State()));
				break;
				
			case ESpudHaveQos:
			default:
				__FLOG_2(_L("Lower NIF binder for context[%d] is in unexpected state[%S]."),aContextId,SpudStateToText(ref->State()));
				ASSERT(EFalse);
				break;		
			}
		}
	}

/**
Send a packet across the link.
This function should not be called; the Mux is the one that should get the data.

@param aPacket MBuf chain containing packet (ignored)
@param aSource (ignored)

@return Error code, or 1 if packet was queued,
        or KErrNone to flow off sender
*/
TInt CSpudMan::Send(RMBufChain& /*aPacket*/, TAny* /*aSource*/)
	{
    _LIT(KPanicMsg, "CSpudMan");
    User::Panic(KPanicMsg, KErrNotSupported);
	return KErrNotSupported;	// never reached
    }


/**
Receives notification from NIFMAN that the authenticate data is ready.
*/
void CSpudMan::AuthenticateComplete(TInt aResult)
	{
	// Send AuthenticateComplete to all lower NIFs
	TContextId i;
	for (i=0; i < KMaxPdpContexts; ++i)
		{
		CSpudBinderRef* ref = NULL;
		TRAPD(rc, ref = iBindMan->GetRefL(i));
		if (rc == KErrNone)
			{
			ref->NifLink()->AuthenticateComplete(aResult);
			}
		}
	}

void CSpudMan::Restart(CNifIfBase* /*aIf*/)
	{
	// TODO: Is it safe to simply ignore this?
	__FLOG_0(_L("CSpudMan::Restart. Ignored."));
    ASSERT(EFalse);
	}


//*****************************************************************************
// SPUD methods
//*****************************************************************************

/**
Receives progress notifications from lower NIF.

@param aContextId Context ID of lower NIF
@param aStage Progress stage
@param aError Error code
*/
void CSpudMan::IfProgress(TContextId aContextId, TInt aStage, TInt aError)
	{
	__FLOG_3(_L("CSpudMan::IfProgress context ID %d received stage %d error %d"),
			 aContextId, aStage, aError);
 	// Eliminate compiler warnings in release builds
	aContextId = aContextId;
	aStage = aStage;
	aError = aError;
	
    // Drop all progress indications from lower NIFs on the floor because they'll just confuse NIFMAN.
    // SpudMan generates its own progress notifications.
    }

/**
Receives progress notifications from lower NIF.

@param aContextId Context ID of lower NIF
@param aSubConnectionUniqueId Subconnection ID
@param aStage Progress stage
@param aError Error code
*/
void CSpudMan::IfProgress(TContextId aContextId, TSubConnectionUniqueId aSubConnectionUniqueId, TInt aStage, TInt aError)
	{
	__FLOG_4(_L("CSpudMan::IfProgress context ID %d subconnection ID %d received stage %d error %d"),
			 aContextId, aSubConnectionUniqueId, aStage, aError);
 	// Eliminate compiler warnings in release builds
	aContextId = aContextId;
	aSubConnectionUniqueId = aSubConnectionUniqueId;
	aStage = aStage;
	aError = aError;

    // Drop all progress indications from lower NIFs on the floor because they'll just confuse NIFMAN.
    // SpudMan generates its own progress notifications.
    }

/**
Receives notifications from lower NIF to agent.

@param aContextId Valid context ID of lower NIF
@param aEvent Event type
@param aInfo Additional information for event (ignored)
@return KErrNone on success, or KErrNotSupported
*/
TInt CSpudMan::Notification(TContextId aContextId, TNifToAgentEventType aEvent, void* /*aInfo*/)
	{
	__FLOG_2(_L("CSpudMan::Notification context ID %d event ID %d"), aContextId, aEvent);
	switch (aEvent)
		{
	case ENifToAgentEventTsyConfig:
		{
		// Return GPRS context structure to lower NIF
		CSpudBinderRef* ref = NULL;
		TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
		__ASSERT_ALWAYS(rc == KErrNone, Panic());
		ASSERT(ref->IsBound());

	    iPdpFsmInterface.Get(aContextId, iTempGprsContext);
        ref->NifLink()->Notification(EAgentToNifEventTsyConfig, reinterpret_cast<void*>(&iTempGprsContext));
	    return KErrNone;
		}

	case ENifToAgentEventTsyConnectionSpeed:
		{
		// Return connection speed to lower NIF
		CSpudBinderRef* ref = NULL;
		TRAPD(rc, ref = iBindMan->GetRefL(aContextId));
		__ASSERT_ALWAYS(rc == KErrNone, Panic());
		ASSERT(ref->IsBound());

#ifdef SYMBIAN_NETWORKING_UMTSR5
		RPacketQoS::TQoSR5Negotiated params;
#else
		RPacketQoS::TQoSR99_R4Negotiated params;
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

		iPdpFsmInterface.Get(aContextId, params);
		ref->NifLink()->Notification(EAgentToNifEventTsyConnectionSpeed,
	    						 reinterpret_cast<void*>(static_cast<TUint>(params.iMaxRate.iUplinkRate)));

	    return KErrNone;
		}

	default:
		// Just ignore all the other notifications
		__FLOG_0(_L("Ignoring notification"));
		break;
		}

	return KErrNotSupported;
	}

/**
Read a boolean field from the connection settings provider.
Intercepts reads of CommPort and returns the correct value.

@param aContextId Valid context ID of lower NIF
@param aField The name of the field to read
@param aValue On return, contains the value of the field read
@return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
*/
TInt CSpudMan::ReadInt(TContextId aContextId, const TDesC& aField, TUint32& aValue)
	{
	
	//This fix is needed to ensure that multiple PPP channels can be used for different PDP contexts. 
	//The returned value of ECommDbCdmaNaiMobileIp will cause PPP to skip external IP configuration (NCPIP).
	if ( (TPtrC(CDMA_NAI_TYPE) == aField) && (iPrimaryContextId != aContextId) )
		{
		__FLOG_2(_L("CSpudMan::ReadInt context ID %d field  = CDMA_NAI_TYPE - Therefore explicitly setting Value to ECommDbCdmaNaiMobileIp"),
			aContextId, &aField);
		__FLOG(_L("No call to AgentRef Will be make"));
		// Lower NIF is requesting the NAI type
		aValue = ECommDbCdmaNaiMobileIp;
		return KErrNone;
		}
	__FLOG_2(_L("CSpudMan::ReadInt context ID %d field %S"), aContextId, &aField);
	// Read CommDB normally
	return Notify()->ReadInt(aField, aValue);
	}

/**
Read a 8-bit descriptor field from the connection settings provider.
Intercepts reads of CommPort and returns the value returned from ETel.

@param aContextId Valid context ID of lower NIF
@param aField The name of the field to read
@param aValue On return, contains the value of the field read
@return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
*/
TInt CSpudMan::ReadDes(TContextId aContextId, const TDesC& aField, TDes8& aValue)
	{
	__FLOG_2(_L("CSpudMan::ReadDes context ID %d field %S"), aContextId, &aField);
	TBuf<2*KCommsDbSvrMaxColumnNameLength+1> columnName;
	_LIT(KFormatText,"%s\\%s");

	columnName.Format(KFormatText,MODEM_BEARER,MODEM_PORT_NAME);
	if (columnName == aField)
		{
		// Lower NIF is requesting the CSY port name
		// Use the TDes16 version of ReadDes to retrieve the data
		TBuf16<KCommsDbSvrMaxFieldLength> data;
		TInt rc(ReadDes(aContextId, aField, data));
		aValue.Copy(data);
		return rc;
		}

	columnName.Format(KFormatText,MODEM_BEARER,MODEM_CSY_NAME);
	if (columnName == aField)
		{
		// Lower NIF is requesting the CSY file name
		// Use the TDes16 version of ReadDes to retrieve the data
		TBuf16<KCommsDbSvrMaxFieldLength> data;
		TInt rc(ReadDes(aContextId, aField, data));
		aValue.Copy(data);
		return rc;
		}

	// Read CommDB normally
	return Notify()->ReadDes(aField, aValue);
	}

/**
Read a 16-bit descriptor field from the connection settings provider.
Intercepts reads of CommPort and returns the value returned from ETel.

@param aContextId Valid context ID of lower NIF
@param aField The name of the field to read
@param aValue On return, contains the value of the field read
@return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
*/
TInt CSpudMan::ReadDes(TContextId aContextId, const TDesC& aField, TDes16& aValue)
    {
	__FLOG_2(_L("CSpudMan::ReadDes context ID %d field %S"), aContextId, &aField);
	TBuf<2*KCommsDbSvrMaxColumnNameLength+1> columnName;
	_LIT(KFormatText,"%s\\%s");

	columnName.Format(KFormatText,MODEM_BEARER,MODEM_PORT_NAME);
	if (columnName == aField)
		{
		// Lower NIF is requesting the CSY port name
	    iPdpFsmInterface.Get(aContextId, iTempDataChannelV2);
		__FLOG_1(_L("Returning ETel port name %S"), &iTempDataChannelV2.iPort);
		aValue.Copy(iTempDataChannelV2.iPort);
		return KErrNone;
		}

	columnName.Format(KFormatText,MODEM_BEARER,MODEM_CSY_NAME);
	if (columnName == aField)
		{
		// Lower NIF is requesting the CSY file name
	    iPdpFsmInterface.Get(aContextId, iTempDataChannelV2);
		__FLOG_1(_L("Returning ETel CSY name %S"), &iTempDataChannelV2.iCsy);
		aValue.Copy(iTempDataChannelV2.iCsy);
		return KErrNone;
		}

	return Notify()->ReadDes(aField, aValue);
    }

/**
Marks the binder to the lower NIF for asynchronous deletion 

@param aRef the binder
@pre the binder must be bound the lower NIF.
*/
void CSpudMan::DisposeOfBinder(CSpudBinderRef* aRef)
	{
	ASSERT(aRef);
	ASSERT(aRef->IsBound()); // We can only mark - sweep bound instances
	ASSERT(aRef->State() != ESpudWaitBinderDelete);
	aRef->SetState(ESpudWaitBinderDelete);
	iBinderSweeperNotifierCb->Call(); // Queue deletion of marked binders & optional Nifman notification.
	}

/**
Sweeps the set of lower NIF binding, deleting the marked ones. If no contexts remain after,
notifies Nifman that SPUD has gone down.

@param aContextId The ID of the context to delete
@param aReason error code that is passed to Nifman
*/
void CSpudMan::SweepBindersAndNotify()
	{
	const TUint KNumContextsRemaining(BindMan()->SweepBinders());
	if (0 == KNumContextsRemaining)
		{
		SetTerminateError(KErrAbort); // This is a last ditched effort to provide termination
		// error code. We cannot determine in all cases what has caused SPUD to terminate.
		// E.g. if several secondary contexts were deleted by the network, which  of them caused SPUD termination?
		// In such case we say that SPUD is shutting down due to internal event (namely, last context deletion).
		
		__FLOG_3(_L("Last lower NIF has been deleted: Notifying Nifman with action EDisconnect[%d], progress KLinkLayerClosed[%d], reason[%d]"),	
		MNifIfNotify::EDisconnect, KLinkLayerClosed, iTerminateError);
		
		// Once we've notified LinkLayerDown & IfProgress, we are finished. Nifman will delete us any moment after
		// the RunL we are working from returns.
		__FLOG(_L("SPUD is finished, and expects to be deleted by Nifman. Reason: last PDP context has gone down, possibly due to Stop on SPUD."));

		// Tell Nifman clients that SPUD is finished.
		Notify()->LinkLayerDown(iTerminateError, MNifIfNotify::EDisconnect); 
		Notify()->IfProgress(KLinkLayerClosed, iTerminateError);
		}
	else
		{
		__FLOG_1(_L("There are [%d] contexts remaining."), KNumContextsRemaining);
		}
	}

void CSpudMan::SetupSipServerAddrRetrievalL(RPacketContext::TProtocolConfigOptionV2& aPco)
	{
	__FLOG(_L("CSpudMan::SetupSipServerAddrRetrieval - Requesting the P-CSCF address from the PCO buffer"));
	
	TPtr8 pcoPtr(const_cast<TUint8*>(aPco.iMiscBuffer.Ptr()),aPco.iMiscBuffer.Length(),aPco.iMiscBuffer.MaxLength());
	
	// attach TTlv to the buffer
	TTlvStruct<RPacketContext::TPcoId,RPacketContext::TPcoItemDataLength> tlv(pcoPtr,0);
	tlv.AppendItemL(RPacketContext::TPcoId(RPacketContext::EEtelPcktPCSCFAddressRequest), 
		TPtr8(static_cast<TUint8*>(NULL), 0, 0));
	aPco.iMiscBuffer.SetLength(pcoPtr.Length());
	}
	

#ifdef SYMBIAN_NETWORKING_UMTSR5
	
void CSpudMan::SetIMCNSignallingFlagPcoL(RPacketContext::TProtocolConfigOptionV2& aPco)
/**
Put the value for IMCN Signalling flag in the pco buffer if it is set in database

@param PCO IE Buffer
*/
	{
	TBool imcn=EFalse;
	TBuf<2*KCommsDbSvrMaxColumnNameLength+2> columnName;
    _LIT(KFormatText,"%s\\%s");
    columnName.Format(KFormatText,QOS_UMTS_R99_AND_ON_TABLE,GPRS_QOS_IM_CN_SIGNALLING_INDICATOR);
    TRAPD(ret, Notify()->ReadBool(columnName,imcn););
	__FLOG_1(_L("CSpudMan::SetIMCNSignallingFlagPcoL - Requesting IMCN Signalling status from Database: error = %d"),ret);

	 if (imcn && ret==KErrNone )

	 {
	  TPtr8 pcoPtr(const_cast<TUint8*>(aPco.iMiscBuffer.Ptr()),aPco.iMiscBuffer.Length(),aPco.iMiscBuffer.MaxLength());
	  TTlvStruct<RPacketContext::TPcoId,RPacketContext::TPcoItemDataLength> tlv(pcoPtr,0);
	  tlv.AppendItemL(RPacketContext::TPcoId(RPacketContext::EEtelPcktIMCNMSSubsystemSignallingFlag ), 
	  TPtr8(static_cast<TUint8*>(NULL), 0, 0));
	  aPco.iMiscBuffer.SetLength(pcoPtr.Length());
	 }
	}
TBool CSpudMan::GetIMCNSignallingFlagPcoL(RPacketContext::TProtocolConfigOptionV2& aPco)
/**
Get the value for IMCN Signalling from the network pco buffer

@param PCO IE Buffer
*/
	{
	
	__FLOG(_L("CSpudMan::GetIMCNSignallingFlagPcoL - Retrieving the IMCN signalling Flag from the PCO buffer"));
	
	TPtr8 pcoPtr(const_cast<TUint8*>(aPco.iMiscBuffer.Ptr()),aPco.iMiscBuffer.Length(),aPco.iMiscBuffer.MaxLength());
	TTlvStruct<RPacketContext::TPcoId,RPacketContext::TPcoItemDataLength> tlv(pcoPtr,0);
	tlv.ResetCursorPos();

	TInt err = tlv.NextItemL(RPacketContext::EEtelPcktIMCNNetworkSubsystemSignallingFlag,pcoPtr);
	return (err == KErrNone);
	
	}

#endif // SYMBIAN_NETWORKING_UMTSR5
	
	
void CSpudMan::SetSipServerAddrL(const RPacketContext::TProtocolConfigOptionV2& aPco)
	{
	__FLOG(_L("CSpudMan::SetSipServerAddr - Retrieving the P-CSCF address from the PCO buffer"));
	iSipServerAddr.Reset(); //Free all existing entries
	TPtr8 pcoPtr(const_cast<TUint8*>(aPco.iMiscBuffer.Ptr()),aPco.iMiscBuffer.Length(),aPco.iMiscBuffer.MaxLength());
	TTlvStruct<RPacketContext::TPcoId,RPacketContext::TPcoItemDataLength> 
		tlv(pcoPtr,0);
		
	tlv.ResetCursorPos();
	TIp6Addr addr;
	TPtr8 addrPtr(NULL, 0);
	TPckg<TIp6Addr> addrPckg(addr);
		
	while (tlv.NextItemL(RPacketContext::EEtelPcktPCSCFAddress,addrPtr) == KErrNone)
		{
		TInetAddr inetAddr;
		addrPckg.Copy(addrPtr);
		inetAddr.SetAddress(addr);
		TBuf<KMaxInetAddrPrintSize> testbuf;
		inetAddr.Output(testbuf);
		__FLOG_1(_L("CSpudMan::SetSipServerAddr - P-CSCF address ---> %S"),&testbuf);
		if (testbuf.Length()) //ie the address is invalid
			{
			iSipServerAddr.AppendL(inetAddr);
			}
		}
	}
	
void CSpudMan::SetContextTerminationErrorAndStop(TContextId aContextId, TInt aErrorCode)
	{
	__FLOG_2(_L("SetContextTerminateError on StatusEvent: aContextId[%d], aErrorCode[%d]"),	
		aContextId, aErrorCode);
	
	// If there is no error then simply return
	if (KErrNone == aErrorCode) return;
	
    // If secondary context, store error code in individual contexts reference
    // and stop the secondary context
    if(aContextId != iPrimaryContextId)
        {
			StopContext(aContextId, aErrorCode, MNifIfNotify::EDisconnect);
        }
    else
        {
        // This is a problem with the Primary context so stop and disconnect
        // Now save the termination error code if not already set
   		if (iTerminateError == KErrNone)
			{
			iTerminateError = aErrorCode;
			}

		// This may be the first ETel error code so save it
		if (iETelTerminateError == KErrNone)
			{
			iETelTerminateError = aErrorCode;
			}

        // Primary context has a problem so disconnect
        Stop(aErrorCode, MNifIfNotify::EDisconnect);
        }
	}
		
//*************************************************************************
// CLowerNifBinderDeletionCb
// Asynchronous deletion of CSpudBinderRefs and notification to Nifman
//*************************************************************************
// Use Spudman's logging.
// Because we are owned by Spudman, we don't have to worry about the logger being deleted.
#ifdef __FLOG_ACTIVE
#define BINDER_SWEEPER_LOG(x) iSpudMan.x
#else
#define BINDER_SWEEPER_LOG(x)
#endif

// Construct a High-Priority AO that calls into the SPUD
// This will work with any priority AO, but because we are releasing memory and 
// potentially notifying Nifman, we want to run ASAP.
CBinderSweeperNotifierCb::CBinderSweeperNotifierCb(CSpudMan& aSpudMan)
	:
	CAsyncOneShot(CActive::EPriorityHigh), 
	iSpudMan(aSpudMan)
	{
	}

// Queues the deletion callback 
void CBinderSweeperNotifierCb::Call()
	{
	if(!IsActive()) // We can be called again before we had a chance to run.
		{
		BINDER_SWEEPER_LOG(__FLOG(_L("CBinderSweeperNotifierCb: Queueing async deletion of dead lower NIF bindings."));)
		CAsyncOneShot::Call();
		return;
		}
	BINDER_SWEEPER_LOG(__FLOG(_L("CBinderSweeperNotifierCb: Async deletion of dead lower NIF bindings is already queued."));)
	}


// Called by ActiveScheduler.
//
// If the lower NIF deletion is attempted after Nifman deletes the SPUD 
// (from CNifAgentRef::DisconnectionComplete), the lower NIF deletion AO is corrupted in the 
// ActiveScheduler, causing ESock thread to crash. To prevent this, lower NIFs are deleted 
// before signalling LinkLayerDown to Nifman. When a lower NIF signals LinkLayerDown, a callback into Spudman is queued.
// This callback deletes the lower NIFs that are eligible for deletion, and notifies Nifman, if necessary.	*/	
void CBinderSweeperNotifierCb::RunL()
	{
	iSpudMan.SweepBindersAndNotify();
	}
	
