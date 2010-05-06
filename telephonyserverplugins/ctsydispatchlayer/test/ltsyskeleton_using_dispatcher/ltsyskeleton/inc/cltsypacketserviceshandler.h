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
// cltsydispatchpacketserviceshandler.h
//

#ifndef __CLTSYDISPATCHPACKETSERVICESHANDLER_H_
#define __CLTSYDISPATCHPACKETSERVICESHANDLER_H_

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <ctsy/ltsy/mltsydispatchinterface.h>
#include <ctsy/ltsy/mltsydispatchpacketservicesinterface.h>
#include "misdispatchinterfacesupported.h"
#include "ltsymacros.h"

// Can be removed in real LTSY implementation
#include MTEST_INCLUDE_MOCKLTSYENGINE_HEADER

// FORWARD DECLARATIONS
class CCtsyDispatcherCallback;

// Can be removed in real LTSY implementation
MTEST_FORWARD_DECLARE_MOCKLTSYENGINE

// CLASS DECLARATION

/**
 * Implements interfaces from the CTSY Dispatcher. The CTSY Dispatcher uses these
 * interfaces to make requests to the Licensee LTSY.
 */
class CLtsyPacketServicesHandler : 	public CBase,
							public MIsDispatchInterfaceSupported, 
							public MLtsyDispatchPacketServicesPacketAttach,
							public MLtsyDispatchPacketServicesGetPacketAttachMode,
							public MLtsyDispatchPacketServicesGetPacketNetworkRegistrationStatus,
							public MLtsyDispatchPacketServicesPacketDetach,
							public MLtsyDispatchPacketServicesSetPdpContextConfig,
							public MLtsyDispatchPacketServicesModifyActivePdpContext,
							public MLtsyDispatchPacketServicesInitialisePdpContext,
							public MLtsyDispatchPacketServicesDeletePdpContext,
							public MLtsyDispatchPacketServicesSetPacketAttachMode,
							public MLtsyDispatchPacketServicesNotifyPacketStatusChange,
							public MLtsyDispatchPacketServicesSetDefaultPdpContextGprsParams,
							public MLtsyDispatchPacketServicesSetDefaultPdpContextR99R4Params,
							public MLtsyDispatchPacketServicesActivatePdpContext,
							public MLtsyDispatchPacketServicesSetPdpContextQosGprs,
							public MLtsyDispatchPacketServicesSetPdpContextQosR99R4,
							public MLtsyDispatchPacketServicesSetPdpContextQosR5,
							public MLtsyDispatchPacketServicesRejectNetworkInitiatedContextActivationRequest,
							public MLtsyDispatchPacketServicesDeactivatePdpContext,
							public MLtsyDispatchPacketServicesAddPacketFilter,
							public MLtsyDispatchPacketServicesGetStatus,
							public MLtsyDispatchPacketServicesGetStaticCapabilities,
							public MLtsyDispatchPacketServicesGetMaxNoMonitoredServiceLists,
							public MLtsyDispatchPacketServicesGetMaxNoActiveServices,
							public MLtsyDispatchPacketServicesInitialiseMbmsContext,
							public MLtsyDispatchPacketServicesGetMbmsNetworkServiceStatus,
							public MLtsyDispatchPacketServicesUpdateMbmsMonitorServiceList,
							public MLtsyDispatchPacketServicesUpdateMbmsSessionList,
							public MLtsyDispatchPacketServicesRemovePacketFilter

							
	{
public:

	virtual ~CLtsyPacketServicesHandler();
	static CLtsyPacketServicesHandler* NewL(CCtsyDispatcherCallback& aCtsyDispatcherCallback);
	static CLtsyPacketServicesHandler* NewLC(CCtsyDispatcherCallback& aCtsyDispatcherCallback);

	// From MIsDispatchInterfaceSupported
	virtual TBool IsInterfaceSupported(TLtsyDispatchInterfaceApiId aDispatchApiId);
	void IsCallbackIndicatorSupported(TLtsyDispatchIndIdGroup aIdGroup, TUint32& aIndIdBitMask);
	


    // From MLtsyDispatchPacketServicesPacketAttach
    virtual TInt HandlePacketAttachReqL();

    // From MLtsyDispatchPacketServicesGetPacketAttachMode
    virtual TInt HandleGetPacketAttachModeReqL();

    // From MLtsyDispatchPacketServicesGetPacketNetworkRegistrationStatus
    virtual TInt HandleGetPacketNetworkRegistrationStatusReqL();

    // From MLtsyDispatchPacketServicesPacketDetach
    virtual TInt HandlePacketDetachReqL();

    // From MLtsyDispatchPacketServicesSetPdpContextConfig
    virtual TInt HandleSetPdpContextConfigReqL(const TDesC& aContextId,
    				const TDesC8& aAccessPointName,
    				const RPacketContext::TProtocolType aPdpType,
    				const TDesC8& aPdpAddress,
    				const TDesC8& aPcoBuffer);

    // From MLtsyDispatchPacketServicesModifyActivePdpContext
    virtual TInt HandleModifyActivePdpContextReqL(const TDesC& aContextName);

    // From MLtsyDispatchPacketServicesInitialisePdpContext
    virtual TInt HandleInitialisePdpContextReqL(const TDesC& aPrimaryContextName, const TDesC& aSecondaryContextName);

    // From MLtsyDispatchPacketServicesDeletePdpContext
    virtual TInt HandleDeletePdpContextReqL(const TDesC& aContextName);

    // From MLtsyDispatchPacketServicesSetPacketAttachMode
    virtual TInt HandleSetPacketAttachModeReqL(RPacketService::TAttachMode aAttachMode);

    // From MLtsyDispatchPacketServicesNotifyPacketStatusChange
    virtual TInt HandleNotifyPacketStatusChangeReqL();

    // From MLtsyDispatchPacketServicesSetDefaultPdpGprsContextParams
    virtual TInt HandleSetDefaultPdpContextGprsParamsReqL(const RPacketContext::TContextConfigGPRS& aContextConfigGprs);

    // From MLtsyDispatchPacketServicesSetDefaultPdpR99R4ContextParams
    virtual TInt HandleSetDefaultPdpContextR99R4ParamsReqL(const RPacketContext::TContextConfigR99_R4& aContextConfigR99R4);    
    
    // From MLtsyDispatchPacketServicesActivatePdpContext
    virtual TInt HandleActivatePdpContextReqL(const TDesC& aContextName);

    // From MLtsyDispatchPacketServicesSetPdpContextQosGprs    
    virtual TInt HandleSetPdpContextQosGprsReqL(const TDesC& aContextName, const RPacketQoS::TQoSGPRSRequested& aQoSConfig);
    
    // From MLtsyDispatchPacketServicesSetPdpContextQosGprs    
    virtual TInt HandleSetPdpContextQosR99R4ReqL(const TDesC& aContextName, const RPacketQoS::TQoSR99_R4Requested& aQoSConfig);
        
    // From MLtsyDispatchPacketServicesSetPdpContextQosGprs    
    virtual TInt HandleSetPdpContextQosR5ReqL(const TDesC& aContextName, const RPacketQoS::TQoSR5Requested& aQoSConfig);        

    // From MLtsyDispatchPacketServicesRejectNetworkInitiatedContextActivationRequest
    virtual TInt HandleRejectNetworkInitiatedContextActivationRequestReqL();

    // From MLtsyDispatchPacketServicesDeactivatePdpContext
    virtual TInt HandleDeactivatePdpContextReqL(const TDesC& aContextName);

    // From MLtsyDispatchPacketServicesAddPacketFilter
    virtual TInt HandleAddPacketFilterSyncL(const TDesC& aContextName,  CArrayFixFlat<RPacketContext::TPacketFilterV2>& aContextFilter);
    
    // From MLtsyDispatchPacketServicesGetStatus
    virtual TInt HandleGetStatusReqL();
    
    // From MLtsyDispatchPacketServicesRemovePacketFilter
    virtual TInt HandleRemovePacketFilterSyncL(const TDesC& aContextName,  TInt aFilterId);

    // From MLtsyDispatchPacketServicesGetStaticCapabilities
    virtual TInt HandleGetStaticCapabilitiesReqL();
    
    // From MLtsyDispatchPacketServicesGetMaxNoMonitoredServiceLists
    virtual TInt HandleGetMaxNoMonitoredServiceListsReqL();
    
    // From MLtsyDispatchPacketServicesGetMaxNoActiveServices
    virtual TInt HandleGetMaxNoActiveServicesReqL();
    
    // From MLtsyDispatchPacketServicesInitialiseMbmsContext
    virtual TInt HandleInitialiseMbmsContextReqL(const TDesC& aContextName);
    
    // From MLtsyDispatchPacketServicesGetMbmsNetworkServiceStatus
    virtual TInt HandleGetMbmsNetworkServiceStatusReqL(TBool aAttemptAttach);

    // From MLtsyDispatchPacketServicesUpdateMbmsMonitorServiceList
    virtual TInt HandleUpdateMbmsMonitorServiceListsReqL(TMbmsAction aAction, const CPcktMbmsMonitoredServiceList& aServiceList, const CPcktMbmsMonitoredServiceList& aChangeList);

    // From MLtsyDispatchPacketServicesUpdateMbmsSessionList
    virtual TInt HandleUpdateMbmsSessionListReqL(const TDesC& aContextName, TMbmsAction aAction, TMbmsSessionId aSessionId, const TMbmsSessionIdList& aServiceList);

private:
	
	CLtsyPacketServicesHandler(CCtsyDispatcherCallback& aCtsyDispatcherCallback);
	void ConstructL();

private:
	
	// Not owned
	
	/**
	 * Callback object in the CTSY Dispatcher.
	 * 
	 * Used to complete a request back to the CTSY Dispatcher.
	 */
	CCtsyDispatcherCallback& iCtsyDispatcherCallback;
	
public:
	// Can be removed in real LTSY implementation.
	MTEST_DECLARE_MOCKLTSYENGINE

	}; // class CLtsyPacketServicesHandler

#endif // __CLTSYDISPATCHPACKETSERVICESHANDLER_H_

