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
// Umts/Gprs SubConnection Provider class definition.
// 
//

/**
 @file
 @internalComponent
*/


#ifndef UMTSGPRS_SUBCONNPROV_H
#define UMTSGPRS_SUBCONNPROV_H


#include <ss_subconnprov.h>
#include <comms-infras/commsdebugutility.h>

#include <cs_subconparams.h>
#include <cs_subconevents.h>

#include "PDPFSM.h"
#include <networking/qos3gpp_subconparams.h>


#ifdef __FLOG_ACTIVE
// Logging tags
_LIT8(KUmtsGprsLogTag1,"SubConn");
_LIT8(KUmtsGprsLogTag2,"UmtsGprs");
#endif

class CPdpFsmRequestBase;

void SubconEventCleanupFunction(TAny* aEvent);
void SubconParamSetCleanupFunction(TAny* aEvent);


/** The class represents an onwer/issuer of a CPdpFsmRequestBase

@internalTechnology
@released Since 9.3
*/
class MPdpFsmRequestOwner
{
    public:
	/** Called by the owned CPdpFsmRequestBase to flag the completion of the
	* request back to 'this'
	*
	  @param aResult - the request result indication.
	  @param aParam  - the request completed (in case 'this' owns more requests).
	*/
    virtual void Done( TInt aResult, CPdpFsmRequestBase& aThis ) = 0;
};





class CUmtsGprsSubConnProvd : public CSubConnectionProviderBase, public MConnectionDataClient, public MPdpFsmRequestOwner
/** The class represents UMTS/GPRS-proprietary subconnection provider.
    It is designed to operate in the near proximity to Etel and
    convert downcalls to Etel's language. This class represents both 
    TSubConnTypes: { EAttachToDefault, ECreateNew }

@internalTechnology
@released Since 9.3
*/
	{
protected:
    enum TQosParameterRelease
        {
        KParameterRelInvalid = 0,
        KParameterRelGeneric = 1,
        KParameterRel4Rel99 = 4,
        KParameterRel5 = 5,
        };


public:
	//-=========================================================
	// Custom methods
	//-=========================================================	

#ifdef SYMBIAN_NETWORKING_UMTSR5

	/**
	Scans given parameter bundle for supported parameter sets
	The first one found gets converted to RPacketQoS::TQoSR5Requested. 
    The supported parameter sets are (in order of priority):
    1) KSubConQosR5ParamsType
    2) KSubConQosR99ParamsType
    3) Generic QoS 
	
	@param aIn CSubConParameterBundle to be converted.
	@param aOut RPacketQoS::TQoSR5Requested to be populated from aIn
	@exception KErrNotSupported - if CUmtsGprsSubConnProvd does not support conversion from what's
	in the supplied parameter bundle.
	*/
	void MapQosParamBundleToEtelL(const CSubConParameterBundle& aIn,
	    RPacketQoS::TQoSR5Requested* aOut);

#else
// !SYMBIAN_NETWORKING_UMTSR5
	
	/** Scans given parameter bundle for supported parameter sets
	The first one found gets converted to RPacketQoS::TQoSR99_R4Requested. 
    The supported parameter sets are (in order of priority):
    1) KSubConQosR99ParamsType
    2) Generic QoS 
	
	@param aIn CSubConParameterBundle to be converted.
	@param aOut RPacketQoS::TQoSR99_R4Requested to be populated from aIn
	@exception KErrNotSupported - if CUmtsGprsSubConnProvd does not support conversion from what's
	in the supplied parameter bundle.
	*/
	void MapQosParamBundleToEtelL(const CSubConParameterBundle& aIn,
	    RPacketQoS::TQoSR99_R4Requested* aOut);

#endif 
// SYMBIAN_NETWORKING_UMTSR5 
	
	/**
   Maps QoS parameters received from the network via Etel into a CSubConGenEventParamsGranted class
   for client event notification.

   @param aNetworkQoS  - A pointer to an Etel TPacketDataConfigBase class containing the
   QoS to be mapped into the event.
   @param aGranted - A reference to the CSubConGenEventParamsGranted object that should receive the mapped
   QoS parameters.
	*/
   void MapQosEtelToGrantedParamsL (TPacketDataConfigBase* aNetworkQoS,
      CSubConGenEventParamsGranted& aGranted);
   
   /** Returns the default CUmtsGprsSubConnProvd for 'this'
	* IMPORTANT: This method is left pure virtual because its implementation, 
	  although trivial, is not needed until CUmtsGprsSubConnProvd fully spports PDP 
	  context management.
	  
	  @return the default CUmtsGprsSubConnProvd for 'this'
	*/
	virtual CUmtsGprsSubConnProvd& GetDefaultSubConnection() = 0;
	
	/** Called by SpudFsm to inform about events and responses regarding
	* the PDP context 'this' represents. The event format complies to UmtsIf interface.
	* CUmtsGprsSubConnProvd forwards the event to the active/pending request 
	* (iCurrentPdpFsmReq) or ignores the event if no request exists.
	*
	  @param aEvent the event type
	  @param aParam additional data
	*/
    virtual TBool PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam);	
	
	/** Returns the PdpFsmInterface instance, which can be used to talk to SpudFsm
	 * 
	  @return the local instance (pertaining to this connection) of CPdpFsmInterface.
	*/
	virtual CPdpFsmInterface& GetPdpFsmInterface();
	
	
	/** Returns context id of the PDP context 'this' represents.
	 * 
	  @return context id of the PDP context 'this' represents.
	*/
	TContextId GetContextId() const;
	
	/** Sends request to the control clients. This method is the overload of
	 * the CSubConnectionProviderBase protected counterpart. It's to be used
	 * by CPdpFsmRequestBase objects.
	 * 
	  @param aEvent - the notification to be sent. 
	*/
    TInt NotifyClientEvent(const CSubConNotificationEvent& aEvent);	
	

	//-=========================================================
	// MPdpFsmRequestOwner methods
	//-=========================================================	
	void Done(TInt aResult, CPdpFsmRequestBase& aThis);
    
	//-=========================================================
	// MConnectionDataClient methods
	//-=========================================================	
	virtual void Notify(TNotify aNotifyType,  CConnectionProviderBase* aConnProvider, TInt aError, const CConNotificationEvent* aConNotificationEvent);
  	virtual void AttachToNext(CSubConnectionProviderBase* aSubConnProvider);
	virtual void ConnectionGoingDown(CConnectionProviderBase& aConnProvider);
	virtual void ConnectionError(TInt aStage, TInt aError);

	//-=========================================================
	// CSubConnectionProviderBase methods
	//-=========================================================
	virtual void DoDataClientJoiningL(MSubConnectionDataClient& aDataClient);
	virtual void DoDataClientLeaving(MSubConnectionDataClient& aDataClient);
	virtual void DoControlClientJoiningL(MSubConnectionControlClient& aControlClient);
	virtual void DoControlClientLeaving(MSubConnectionControlClient& aControlClient);
	virtual void DoSourceAddressUpdate(MSubConnectionDataClient& aDataClient, const TSockAddr& aSource);
	virtual void DoDestinationAddressUpdate(MSubConnectionDataClient& aDataClient, const TSockAddr& aDestination);
	virtual void DoDataClientRouted(MSubConnectionDataClient& aDataClient, const TSockAddr& aSource, const TSockAddr& aDestination, const TDesC8& aConnectionInfo);
	virtual void DoParametersAboutToBeSetL(CSubConParameterBundle& aParameterBundle);
	virtual TInt DoControl(TUint aOptionLevel, TUint aOptionName, TDes8& aOption);
	virtual CConnDataTransfer& DoDataTransferL();
	virtual MConnectionDataClient* DoSelfConnectionDataClient();
   virtual void DoStartL();
	virtual void DoStop();
	virtual CSubConnectionProviderBase* DoNextLayer();

protected:
	
	//-=========================================================
	// Custom methods
	//-=========================================================	
    CUmtsGprsSubConnProvd(CSubConnectionProviderFactoryBase& aFactory, 
        CConnectionProviderBase& aConnProvider);

    // Don't allow clients to invoke the destructor.
    // (Only the CCommsFactoryBase should do this)
    virtual ~CUmtsGprsSubConnProvd();

    static void MapQosR99ExtensionSetToEtel(const CSubConQosR99ParamSet& aInRequested,
        const CSubConQosR99ParamSet* aInAcceptable,
        RPacketQoS::TQoSR99_R4Requested* aOut);

    static void MapQosGenericSetToEtel(const CSubConQosGenericParamSet& aInRequested, 
        const CSubConQosGenericParamSet* aInAcceptable,
        RPacketQoS::TQoSR99_R4Requested* aOut);
                               

    static void MapQosEtelToGenericSetL (RPacketQoS::TQoSR99_R4Negotiated* aNetworkQoS,
        CSubConGenEventParamsGranted& aGranted);

    static void MapQosEtelToR99SetL (RPacketQoS::TQoSR99_R4Negotiated* aNetworkQoS,
        CSubConGenEventParamsGranted& aGranted);

	void NotifySubConDownL(TInt aError);
    void NotifyParamsChangedL(TInt aError);

    inline void ResetQosRelease();
    inline TQosParameterRelease RequestedQosRelease();


#ifdef SYMBIAN_NETWORKING_UMTSR5
    static void MapQosR5ExtensionSetToEtel(const CSubConQosR5ParamSet& aInRequested,
        const CSubConQosR5ParamSet* aInAcceptable,
        RPacketQoS::TQoSR5Requested* aOut);

    static void MapQosR99ExtensionSetToEtel(const CSubConQosR99ParamSet& aInRequested,
        const CSubConQosR99ParamSet* aInAcceptable, RPacketQoS::TQoSR5Requested* aOut);

    static void MapQosEtelToR5SetL (RPacketQoS::TQoSR5Negotiated* aNetworkQoS,
        CSubConGenEventParamsGranted& aGranted);
#endif 
// SYMBIAN_NETWORKING_UMTSR5 

	/** 
	Given a set of generic parameters, the method derives the R99 value for the traffic class.

	@param aIn requested CSubConQosGenericParamSet to be converted
	@return the value of RPacketQoS::TTrafficClass
	*/	
	static RPacketQoS::TTrafficClass DeriveTrafficClass(const CSubConQosGenericParamSet& aIn );
	
	/** 
	Given a set of generic parameters, the method derives the R99 value for the traffic priority.

	@param aIn requested CSubConQosGenericParamSet to be converted
	@return the value of RPacketQoS::TTrafficHandlingPriority
	*/		
	static RPacketQoS::TTrafficHandlingPriority DeriveTrafficPriority(const CSubConQosGenericParamSet& aIn );
	
	/*
	The PDP context id 'this' represents. This is semantically equal to the SpudFSM PDP
	Context identification scheme.
	*/
	TContextId iContextId;

	/*
	Holds the current request towards PDPFSM
	*/
	CPdpFsmRequestBase* iCurrentPdpFsmReq;


private:
	TQosParameterRelease iParameterRelease;


public:
	__FLOG_DECLARATION_MEMBER;
	};
	
	
	
	
	
/**
The base class for a PDP Context control asynchronous requests. 

@internalTechnology
@released Since 9.3
*/
class CPdpFsmRequestBase
{
public:
    
	/** 
	C'tor

	@param aProvider - the sub connection provider 'this' is directly or indirectly owned by.
	*                  Also represents a specific PDP context.
	@param aOwner    - the immediate owner of 'this' (could be same as aProvider or could be
	*                  request (when nested). 
	*/		
    CPdpFsmRequestBase( CUmtsGprsSubConnProvd& aProvider, MPdpFsmRequestOwner& aOwner);

    virtual ~CPdpFsmRequestBase();
    
	/** 
	The method should issue the request

	@return the immediate request result.
	*/		
    virtual TInt Start() = 0;
    
    
	/** 
	The callback method called by the direct owner of 'this' to forward events/results
	caused by issuing 'this'. 'This' can interpret the events and decide whether to
	flag the completion back to its owner.

	@param aContextId The context Id that the event occured on
	@param aEvent The identifier of the event that occured
	@param aParam Data relavent to the event. Usually a system-wide error code
	@return ETrue if the event was of interested and handled, EFalse if wasn't
	handled and should be passed on to SpudMan
	*/	
	virtual TBool PdpFsmEvent(TContextId aContextId, TInt aEvent, TInt aParam) = 0;
    
protected:
    CUmtsGprsSubConnProvd& iProvider;
    MPdpFsmRequestOwner& iOwner;
    TContextId iContextId;

public:
	__FLOG_DECLARATION_MEMBER;
};



#include "UmtsGprs_subconnProv.inl"
#endif
// UMTSGPRS_SUBCONNPROV_H
