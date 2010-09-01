// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Defines the CRawIPFlow class, which provides a central point for the Flow
// to communicate with NifMan and the BCA controller. This class creates an
// interface to the IPv4 binder when it is required.
// 
//

/**
 @file
*/

#ifndef RAWIP2FLOW_H
#define RAWIP2FLOW_H

#include <comms-infras/nifif.h>
#include <networking/packetlogger.h>
#include <in_sock.h>
#include <comms-infras/ss_flowbinders.h>
#include <comms-infras/ss_subconnflow.h>
#include "BinderBase.h"
#include "bttlog.h"
#include <comms-infras/linkmessages.h>		// for TLinkMessageId

class CBcaController;
class CBttLogger;
class CPacketLogger;
class CBCAProvision;

class CRawIP2Flow : public ESock::CSubConnectionFlowBase, public ESock::MFlowBinderControl
	{
	friend class CRawIP2FlowFactory;

public:

    //-=========================================================
    // Methods for handling incoming SCPR messages
    //-=========================================================
    void StartFlowL();
	void StopFlow(TInt aError);
	void SubConnectionGoingDown();
	void SubConnectionError(TInt aError);
	void Destroy();
	
	//-=========================================================
	// CSubConnectionProviderBase methods
	//-=========================================================
	MFlowBinderControl* DoGetBinderControlL();

	//-=========================================================
	// Messages::ANode methods
	//-=========================================================
	virtual void ReceivedL(const Messages::TRuntimeCtxId& aSender, const Messages::TNodeId& aRecipient, Messages::TSignatureBase& aMessage);

	//-=========================================================
	// MFlowBinderControl methods
	//-=========================================================
	virtual ESock::MLowerControl* GetControlL(const TDesC8& aProtocol);
	virtual ESock::MLowerDataSender* BindL(const TDesC8& aProtocol, ESock::MUpperDataReceiver* aReceiver, ESock::MUpperControl* aControl);
	virtual void Unbind(ESock::MUpperDataReceiver* aReceiver, ESock::MUpperControl* aControl);
	virtual CSubConnectionFlowBase* Flow(); //returns its own flow

    //-=========================================================
    // Custom methods
    //-=========================================================
    // provide compatability for classes using old CNifIfLink style API and PPP Internal Stop() calls
    void Stop(TInt aReason, MNifIfNotify::TAction aAction);
    // call from Binder - MLowerDataSender
	ESock::MLowerDataSender::TSendResult Send(RMBufChain& aPdu, TUint aProtocol);

	// Used to be :Pure virtuals inherited from MContextSmObserver
	CBcaController* GetBcaController();
	void UpdateContextState(RPacketContext::TContextStatus aState,	TInt aError);

public: 
	virtual void InitialiseL();
	virtual void ShutDown( TInt aError);

	virtual CBinderBase* GetBinder();

public:
	// Notify that link is Up/Down
	void LinkLayerUp();
	void LinkLayerDown(TInt aError);

public:

public:	// Context status retrieval.
	inline RPacketContext::TContextStatus GetContextStatus();
	
protected:
	CRawIP2Flow(ESock::CSubConnectionFlowFactoryBase& aFactory, const Messages::TNodeId& aSubConnId, ESock::CProtocolIntfBase* aProtocolIntf, CBttLogger* aTheLogger);
	void ConstructL();
	virtual ~CRawIP2Flow();
	
protected:
    class TBinderControlProxy : public ESock::MFlowBinderControl
        {
        public:
    	virtual ESock::MLowerControl* GetControlL(const TDesC8& aProtocol);
    	virtual ESock::MLowerDataSender* BindL(const TDesC8& aProtocol, ESock::MUpperDataReceiver* aReceiver, ESock::MUpperControl* aControl);
    	virtual void Unbind(ESock::MUpperDataReceiver* aReceiver, ESock::MUpperControl* aControl);
    	virtual CSubConnectionFlowBase* Flow(); //returns its own flow
    	CRawIP2Flow& RawIP2Flow()
        	{
        	ASSERT(iFlow);
        	return *iFlow;  
        	}
    	
        TBinderControlProxy(CRawIP2Flow& aFlow)
        :iFlow(&aFlow)
            {
            }
        
        CRawIP2Flow* iFlow;
        };
    TBinderControlProxy* iBinderControl;

private:
	void ProvisionConfig(const ESock::RMetaExtensionContainerC& aConfigData);
	void DynamicProvisionConfigL();

	// Utility functions
	void PostProgressMessage(TInt aStage, TInt aError);
    void SendDataClientIdleIfNoClients();

private:// Unowned
	/** networking packet logger for debugging packets */
	__PACKETLOG_DECLARATION_MEMBER;
	CBttLogger* iTheLogger;

	// The protocol to use (IPv4).Note: This is not owned by this class.
	CBinderBase* iBinder;

	const CBCAProvision* iBCAProvisionConfig;						// pointer to BCA provisioning structure in CAccessPointConfig
	const CRawIpAgentConfig* iAgentProvision;				// pointer to Agent provisioning structure in CAccessPointConfig
	TInt iProvisionError;						// error code saved during processing of ProvisionConfig message
	

private:// Owned
	TInt iInitError;

	RPacketContext::TContextStatus iContextStatus;

	CBcaController* iBcaController;
	TBool iStarting:1;
	TBool iStarted:1;
	TBool iStopping:1;
	TBool iBlocked:1;
	};

inline RPacketContext::TContextStatus CRawIP2Flow::GetContextStatus()
/**
 * Returns Context status
 *
 * @return iContextStatus The context status
 */
	{
	return iContextStatus;
	}

#endif // RAWIP2Flow_H
