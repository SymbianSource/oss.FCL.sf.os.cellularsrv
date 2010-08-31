// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// PDP FSM implementation
// @internalTechnology
// 
//




#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "cpdpfsmTraces.h"
#endif

#include "tpdpstates.h"
#include "cpdpfsm.h"
#include "cpdpfsmfactory.h"
#include "eteldrivernmspace.h"


#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CPdpFsm* CPdpFsm::NewL(TContextId aPdpId, CPdpFsmFactory * aPdpFsmFactory, REtelDriverInput * aEtelDriverInput)
#else
	CPdpFsm* CPdpFsm::NewL(TContextId aPdpId, CPdpFsmFactory * aPdpFsmFactory, REtelDriverInput * aEtelDriverInput, MPdpFsmEventHandler& aPdpFsmEventHandler, SpudMan::TPdpContextType aContextType)
#endif
	{
#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CPdpFsm* self = new(ELeave) CPdpFsm(aPdpId, aPdpFsmFactory, aEtelDriverInput);
#else
	CPdpFsm* self = new(ELeave) CPdpFsm(aPdpId, aPdpFsmFactory, aEtelDriverInput, aPdpFsmEventHandler, aContextType);
#endif
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CPdpFsm::ConstructL()
	{
	iContextConfig.CreateL(PdpFsm::KContextConfigBufferSize);
	iContextConfig.SetMax();
	iContextConfig.FillZ();
	}



#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
CPdpFsm::CPdpFsm(TContextId aPdpId, CPdpFsmFactory * aPdpFsmFactory, REtelDriverInput * aEtelDriverInput)
#else
CPdpFsm::CPdpFsm(TContextId aPdpId, CPdpFsmFactory * aPdpFsmFactory, REtelDriverInput * aEtelDriverInput, MPdpFsmEventHandler& aPdpFsmEventHandler,SpudMan::TPdpContextType aContextType)
:iPdpFsmEventHandler(&aPdpFsmEventHandler)
#endif
// NOTE: Both pointers are valid when called - see CPdpFsmFactory and CPdpFsmInterface. 
// No other caller is expected.
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CTOR_1, ">>CPdpFsm::CPdpFsm()");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CTOR_2, "Pdp ID %d",iPdpId);
	
	// save for later
	iPdpId = aPdpId;
	
	iPdpFsmFactory = aPdpFsmFactory;
	
	iEtelDriverInput = aEtelDriverInput;
	iContextType = aContextType;
	
	// we need a state
	iState = &iPdpFsmFactory->iStateInitialised;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CTOR_3, "<<CPdpFsm::CPdpFsm()");
    }
    

CPdpFsm::~CPdpFsm()
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_DTOR_1, ">>CPdpFsm::~CPdpFsm()");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_DTOR_2, "Pdp ID %d",iPdpId);
	iContextConfig.Close();
	iEtelDriverInput->DeletePdp(iPdpId);
	iSessionInfo.iSessionIds.Close();	
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_DTOR_3, "<<CPdpFsm::~CPdpFsm()");
    }

	
TInt CPdpFsm::Input (const TInt aOperation, const TInt aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_INPUT_1, ">>CPdpFsm::Input()");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_INPUT_2, "Pdp ID %d",iPdpId);
	
#if (OST_TRACE_CATEGORY & OST_TRACE_CATEGORY_DEBUG)
	OstTraceDefExt1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_INPUT_3, "State : %S", iState->iName);


	OstTraceDefExt3(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_INPUT_4, "Pdp ID %d, State '%S', Operation '%S'",iPdpId, iState->iName, *iState->LogOperation(*this, aOperation));
#endif  
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_INPUT_5, "<<CPdpFsm::Input()");
	return iState->Input (*this, aOperation, aParam);
    }
	

#ifdef SYMBIAN_NETWORKING_UMTSR5
void CPdpFsm::Get(RPacketQoS::TQoSR5Requested& aParam)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET_1, ">>CPdpFsm::Get(QoSR5Req)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET_2, "Pdp ID %d",iPdpId);

	ASSERT(iQosRequested.ExtensionId() == TPacketDataConfigBase::KConfigRel5);
	aParam = iQosRequested.RequestedQoSR5();
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET_3, "<<CPdpFsm::Get(QoSR5Req)");
}

void CPdpFsm::Get(RPacketQoS::TQoSR5Negotiated& aParam)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET1_1, ">>CPdpFsm::Get(QoSR5Neg)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET1_2, "Pdp ID %d",iPdpId);

	ASSERT(iQosNegotiated.ExtensionId() == TPacketDataConfigBase::KConfigRel5);
	aParam = iQosNegotiated.NegotiatedQoSR5();
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET1_3, "<<CPdpFsm::Get(QoSR5Neg)");
}

#else
// !SYMBIAN_NETWORKING_UMTSR5

void CPdpFsm::Get(RPacketQoS::TQoSR99_R4Requested& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET2_1, ">>CPdpFsm::Get(QosReq)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET2_2, "Pdp ID %d",iPdpId);

	aParam = iQosRequested.RequestedQoSR99_R4();
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET2_3, "<<CPdpFsm::Get(QosReq)");
    }

void CPdpFsm::Get(RPacketQoS::TQoSR99_R4Negotiated& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET3_1, ">>CPdpFsm::Get(QoSNeg)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET3_2, "Pdp ID %d",iPdpId);

	aParam = iQosNegotiated.NegotiatedQoSR99_R4();
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET3_3, "<<CPdpFsm::Get(QoSNeg)");
    }
    
#endif
// SYMBIAN_NETWORKING_UMTSR5


void CPdpFsm::Get(TTFTInfo& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET4_1, ">>CPdpFsm::Get(TFTInfo)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET4_2, "Pdp ID %d",iPdpId);

	aParam = iTFT;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET4_3, "<<CPdpFsm::Get(TFTInfo)");
    }

void CPdpFsm::Get(TTFTOperationCode& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET5_1, ">>CPdpFsm::Get(TFTOperationCode)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET5_2, "Pdp ID %d",iPdpId);

	aParam = iTFTOperationCode;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET5_3, "<<CPdpFsm::Get(TFTOperationCode)");
    }

void CPdpFsm::Get(RPacketContext::TDataChannelV2& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET6_1, ">>CPdpFsm::Get(RPacketContext::TDataChannelV2)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET6_2, "Pdp ID %d",iPdpId);

	aParam = iDataChannelV2;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET6_3, "<<CPdpFsm::Get(RPacketContext::TDataChannelV2)");
    }

void CPdpFsm::Get(TPacketDataConfigBase& aParam)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET7_1, ">>CPdpFsm::Get(TPacketDataConfigBase)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET7_2, "Pdp ID %d",iPdpId);

	TInt rel = GetContextConfigAs<TPacketDataConfigBase>().ExtensionId();
	if (rel == 0)
		{
    		// buffer has not been initialised yet
    		InitialiseContextConfig(aParam.ExtensionId());
    		}

	switch (rel)
    		{
    		case TPacketDataConfigBase::KConfigGPRS:
			Mem::Copy(&aParam, const_cast<TUint8*>(iContextConfig.Ptr()), sizeof(RPacketContext::TContextConfigGPRS));
			break;

    		case TPacketDataConfigBase::KConfigRel99Rel4:
			Mem::Copy(&aParam, const_cast<TUint8*>(iContextConfig.Ptr()), sizeof(RPacketContext::TContextConfigR99_R4));
        		break;

    		case TPacketDataConfigBase::KConfigRel5:
			Mem::Copy(&aParam, const_cast<TUint8*>(iContextConfig.Ptr()), sizeof(RPacketContext::TContextConfig_R5));
			break;
		}
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET7_3, "<<CPdpFsm::Get(TPacketDataConfigBase)");
	}

void CPdpFsm::Get(RPacketContext::TContextStatus& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET8_1, ">>CPdpFsm::Get(RPacketContext::TContextStatus)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET8_2, "Pdp ID %d",iPdpId);

	aParam = iContextStatus;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET8_3, "<<CPdpFsm::Get(RPacketContext::TContextStatus)");
    }


#ifdef SYMBIAN_NETWORKING_UMTSR5
void CPdpFsm::Set(const RPacketQoS::TQoSR5Requested& aParam)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET_1, ">>CPdpFsm::Set(QoSR5Req)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET_2, "Pdp ID %d",iPdpId);

	iQosRequested = aParam;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET_3, "<<CPdpFsm::Set(QoSR5Req)");
}

void CPdpFsm::Set(const RPacketQoS::TQoSR5Negotiated& aParam)
{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET1_1, ">>CPdpFsm::Set(QoSR5Neg)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET1_2, "Pdp ID %d",iPdpId);

	iQosNegotiated = aParam;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET1_3, "<<CPdpFsm::Set(QoSR5Neg)");
}

#else
// !SYMBIAN_NETWORKING_UMTSR5


void CPdpFsm::Set(const RPacketQoS::TQoSR99_R4Requested& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET2_1, ">>CPdpFsm::Set(QoSReq)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET2_2, "Pdp ID %d",iPdpId);

	iQosRequested = aParam;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET2_3, "<<CPdpFsm::Set(QoSReq)");

    }


void CPdpFsm::Set(const RPacketQoS::TQoSR99_R4Negotiated& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET3_1, ">>CPdpFsm::Set(QoSNeg)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET3_2, "Pdp ID %d",iPdpId);

	iQosNegotiated = aParam;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET3_3, "<<CPdpFsm::Set(QoSNeg)");
    }


#endif 
// SYMBIAN_NETWORKING_UMTSR5


void CPdpFsm::Set(const TTFTInfo& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET4_1, ">>CPdpFsm::Set(TFT)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET4_2, "Pdp ID %d",iPdpId);

	iTFT = aParam;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET4_3, "<<CPdpFsm::Set(TFT)");
    }

void CPdpFsm::Set(const TTFTOperationCode& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET5_1, ">>CPdpFsm::Set(TFTOperationCode)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET5_2, "Pdp ID %d",iPdpId);

	iTFTOperationCode = aParam;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET5_3, "<<CPdpFsm::Set(TFTOperationCode)");
    }

void CPdpFsm::Set(const RPacketContext::TDataChannelV2& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET6_1, ">>CPdpFsm::Set(RPacketContext::TDataChannelV2)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET6_2, "Pdp ID %d",iPdpId);

	iDataChannelV2 = aParam;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET6_3, "<<CPdpFsm::Set(RPacketContext::TDataChannelV2)");
    }

void CPdpFsm::Set(const RPacketContext::TContextStatus& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET7_1, ">>CPdpFsm::Set(RPacketContext::TContextStatus)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET7_2, "Pdp ID %d",iPdpId);

	iContextStatus = aParam;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET7_3, "<<CPdpFsm::Set(RPacketContext::TContextStatus)");
    }

void CPdpFsm::Set(const TPacketDataConfigBase& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET8_1, ">>CPdpFsm::Set(RPacketContext::TContextConfigGPRS)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET8_2, "Pdp ID %d",iPdpId);
	
	switch ((const_cast<TPacketDataConfigBase&>(aParam)).ExtensionId())
	    {
	    case TPacketDataConfigBase::KConfigGPRS:
	        Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &aParam, sizeof(RPacketContext::TContextConfigGPRS));
	        break;
	        
	    case TPacketDataConfigBase::KConfigRel99Rel4:
	        Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &aParam, sizeof(RPacketContext::TContextConfigR99_R4));
	        break;

	    case TPacketDataConfigBase::KConfigRel5:
	        Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &aParam, sizeof(RPacketContext::TContextConfig_R5));
	        break;
	    }
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET8_3, "<<CPdpFsm::Set(RPacketContext::TContextConfigGPRS)");
    }

#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY	
void CPdpFsm::Set(MPdpFsmEventHandler& aPdpFsmEventHandler)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET9_1, ">>CPdpFsm::Set(MPdpFsmEventHandler&)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET9_2, "Pdp ID %d",iPdpId);
	
	if (iPdpFsmEventHandler)
    	{
    	iPdpFsmEventHandler->Event(KContextDeleteEvent, KErrAbort);
    	}
	iPdpFsmEventHandler = &aPdpFsmEventHandler;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET9_3, "<<CPdpFsm::Set(MPdpFsmEventHandler&)");
    }
#endif  

void CPdpFsm::EtelInput (EtelDriver::TEtelInput aOperation)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_ETELINPUT_1, ">>CPdpFsm::EtelInput");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_ETELINPUT_2, "Pdp ID %d",iPdpId);
	
	iEtelDriverInput->Input (iPdpId, aOperation);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_ETELINPUT_3, "<<CPdpFsm::EtelInput");
    }

void CPdpFsm::EtelCancel (void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_ETELCANCEL_1, ">>CPdpFsm::EtelCancel");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_ETELCANCEL_2, "Pdp ID %d",iPdpId);
	
	iEtelDriverInput->CancelPdp (iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_ETELCANCEL_3, "<<CPdpFsm::EtelCancel");
    }

void CPdpFsm::SpudInput (TInt aNotification, TInt aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SPUDINPUT_1, ">>CPdpFsm::SpudInput");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SPUDINPUT_2, "Pdp ID %d",iPdpId);
#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY	
	iPdpFsmFactory->SpudInput (iPdpId, aNotification, aParam);
#else
    ASSERT(iPdpFsmEventHandler);
    iPdpFsmEventHandler->Event(aNotification, aParam);
    OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SPUDINPUT_3, "<<CPdpFsm::SpudInput");
#endif
    }

// state change members

void CPdpFsm::ChangeStateToInitialised(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOINITIALISED_1, ">>CPdpFsm::ChangeStateToInitialised");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOINITIALISED_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOINITIALISED_3, "New State : Initialised");
	
	iState = &iPdpFsmFactory->iStateInitialised;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOINITIALISED_4, "<<CPdpFsm::ChangeStateToInitialised");
    }

void CPdpFsm::ChangeStateToOpeningPhone(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOOPENINGPHONE_1, ">>CPdpFsm::ChangeStateToOpeningPhone");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOOPENINGPHONE_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOOPENINGPHONE_3, "New State : OpeningPhone");
	
	iState = &iPdpFsmFactory->iStateOpeningPhone;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOOPENINGPHONE_4, "<<CPdpFsm::ChangeStateToOpeningPhone");
    }

void CPdpFsm::ChangeStateToCreatingPrimary(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGPRIMARY_1, ">>CPdpFsm::ChangeStateToCreatingPrimary");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGPRIMARY_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGPRIMARY_3, "New State : CreatingPrimary");
	
	iState = &iPdpFsmFactory->iStateCreatingPrimary;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGPRIMARY_4, "<<CPdpFsm::ChangeStateToCreatingPrimary");
    }

void CPdpFsm::ChangeStateToActivatingPrimary(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGPRIMARY_1, ">>CPdpFsm::ChangeStateToActivatingPrimary");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGPRIMARY_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGPRIMARY_3, "New State : ActivatingPrimary");
	
	iState = &iPdpFsmFactory->iStateActivatingPrimary;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGPRIMARY_4, "<<CPdpFsm::ChangeStateToActivatingPrimary");
    }

void CPdpFsm::ChangeStateToCreatingSecondary(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGSECONDARY_1, ">>CPdpFsm::ChangeStateToCreatingSecondary");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGSECONDARY_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGSECONDARY_3, "New State : CreatingSecondary");
	
	iState = &iPdpFsmFactory->iStateCreatingSecondary;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGSECONDARY_4, "<<CPdpFsm::ChangeStateToCreatingSecondary");
    }

void CPdpFsm::ChangeStateToCreatedSecondary(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATEDSECONDARY_1, ">>CPdpFsm::ChangeStateToCreatedSecondary");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATEDSECONDARY_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATEDSECONDARY_3, "New State : CreatedSecondary");
	
	iState = &iPdpFsmFactory->iStateCreatedSecondary;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATEDSECONDARY_4, "<<CPdpFsm::ChangeStateToCreatedSecondary");
    }

void CPdpFsm::ChangeStateToSettingTFT(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSETTINGTFT_1, ">>CPdpFsm::ChangeStateToSettingTFT");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSETTINGTFT_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSETTINGTFT_3, "New State : SettingTFT");
	
	iState = &iPdpFsmFactory->iStateSettingTFT;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSETTINGTFT_4, "<<CPdpFsm::ChangeStateToSettingTFT");
    }

void CPdpFsm::ChangeStateToSettingQoS(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSETTINGQOS_1, ">>CPdpFsm::ChangeStateToSettingQoS");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSETTINGQOS_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSETTINGQOS_3, "New State : SettingQoS");
	
	iState = &iPdpFsmFactory->iStateSettingQoS;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSETTINGQOS_4, "<<CPdpFsm::ChangeStateToSettingQoS");
    }

void CPdpFsm::ChangeStateToActivatingSecondary(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGSECONDARY_1, ">>CPdpFsm::ChangeStateToActivatingSecondary");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGSECONDARY_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGSECONDARY_3, "New State : ActivatingSecondary");
	
	iState = &iPdpFsmFactory->iStateActivatingSecondary;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGSECONDARY_4, "<<CPdpFsm::ChangeStateToActivatingSecondary");
    }

void CPdpFsm::ChangeStateToOpen(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOOPEN_1, ">>CPdpFsm::ChangeStateToOpen");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOOPEN_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOOPEN_3, "New State : Open");
	
	iState = &iPdpFsmFactory->iStateOpen;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOOPEN_4, "<<CPdpFsm::ChangeStateToOpen");
    }

void CPdpFsm::ChangeStateToChangingQoS(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCHANGINGQOS_1, ">>CPdpFsm::ChangeStateToChangingQoS");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCHANGINGQOS_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCHANGINGQOS_3, "New State : ChangingQoS");
	
	iState = &iPdpFsmFactory->iStateChangingQoS;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCHANGINGQOS_4, "<<CPdpFsm::ChangeStateToChangingQoS");
    }

void CPdpFsm::ChangeStateToChangingTFT(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCHANGINGTFT_1, ">>CPdpFsm::ChangeStateToChangingTFT");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCHANGINGTFT_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCHANGINGTFT_3, "New State : ChangingTFT");
	
	iState = &iPdpFsmFactory->iStateChangingTFT;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCHANGINGTFT_4, "<<CPdpFsm::ChangeStateToChangingTFT");
    }

void CPdpFsm::ChangeStateToGettingNegQoS(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOGETTINGNEGQOS_1, ">>CPdpFsm::ChangeStateToGettingNegQoS");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOGETTINGNEGQOS_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOGETTINGNEGQOS_3, "New State : GettingNegQoS");
	
	iState = &iPdpFsmFactory->iStateGettingNegQoS;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOGETTINGNEGQOS_4, "<<CPdpFsm::ChangeStateToGettingNegQoS");
    }



void CPdpFsm::ChangeStateToModifingActive(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOMODIFINGACTIVE_1, ">>CPdpFsm::ChangeStateToModifingActive");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOMODIFINGACTIVE_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOMODIFINGACTIVE_3, "New State : ModifingActive");
	
	iState = &iPdpFsmFactory->iStateModifyingActive;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOMODIFINGACTIVE_4, "<<CPdpFsm::ChangeStateToModifingActive");
    }

void CPdpFsm::ChangeStateToSuspended(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSUSPENDED_1, ">>CPdpFsm::ChangeStateToSuspended");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSUSPENDED_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSUSPENDED_3, "New State : Suspended");
	
	iState = &iPdpFsmFactory->iStateSuspended;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSUSPENDED_4, "<<CPdpFsm::ChangeStateToSuspended");
    }

void CPdpFsm::ChangeStateToClosing(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCLOSING_1, ">>CPdpFsm::ChangeStateToClosing");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCLOSING_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCLOSING_3, "New State : Closing");
	
	iState = &iPdpFsmFactory->iStateClosing;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCLOSING_4, "<<CPdpFsm::ChangeStateToClosing");
    }

void CPdpFsm::ChangeStateToStopping(void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSTOPPING_1, ">>CPdpFsm::ChangeStateToStopping");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSTOPPING_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSTOPPING_3, "New State : Stopping");
	
	iState = &iPdpFsmFactory->iStateStopping;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOSTOPPING_4, "<<CPdpFsm::ChangeStateToStopping");
    }
    
void CPdpFsm::ChangeStateToCreatingMbms(void)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGMBMS_1, ">>CPdpFsm::ChangeStateToCreatingMbms");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGMBMS_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGMBMS_3, "New State : Creating");
	
	iState = &iPdpFsmFactory->iStateCreatingMbms;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATINGMBMS_4, "<<CPdpFsm::ChangeStateToCreatingMbms");
	}
	
void CPdpFsm::ChangeStateToActivatingMbms(void)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGMBMS_1, ">>CPdpFsm::ChangeStateToActivatingMbms");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGMBMS_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGMBMS_3, "New State : Activating");
	
	iState = &iPdpFsmFactory->iStateActivatingMbms;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOACTIVATINGMBMS_4, "<<CPdpFsm::ChangeStateToActivatingMbms");
	}

void CPdpFsm::ChangeStateToCreatedMbms(void)
	{
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATEDMBMS_1, ">>CPdpFsm::ChangeStateToCreatingMbms");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATEDMBMS_2, "Pdp ID %d",iPdpId);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATEDMBMS_3, "New State : Created");
	
	iState = &iPdpFsmFactory->iStateCreatedMbms;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_CHANGESTATETOCREATEDMBMS_4, "<<CPdpFsm::ChangeStateToCreatingMbms");
	}


	
void CPdpFsm::Get(RPacketMbmsContext::TContextConfigMbmsV1& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET9_1, ">>CPdpFsm::Get(RPacketMbmsContext::TContextConfigMbmsV1)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET9_2, "Pdp ID %d",iPdpId);

	aParam = iMbms;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET9_3, "<<CPdpFsm::Get(RPacketMbmsContext::TContextConfigMbmsV1)");
    }
    
    
void CPdpFsm::Set(const RPacketMbmsContext::TContextConfigMbmsV1& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET10_1, ">>CPdpFsm::Get(RPacketMbmsContext::TContextConfigMbmsV1)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET10_2, "Pdp ID %d",iPdpId);

	iMbms = aParam ;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET10_3, "<<CPdpFsm::Get(RPacketMbmsContext::TContextConfigMbmsV1)");
    }
    
    
void CPdpFsm::Get(TSessionOperatioInfo& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET10_1, ">>CPdpFsm::Get(Session)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET10_2, "Pdp ID %d",iPdpId);

	aParam = iSessionInfo;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_GET10_3, "<<CPdpFsm::Get(Session)");
    }
    
    
void CPdpFsm::Set(const TSessionOperatioInfo& aParam)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET11_1, ">>CPdpFsm::Get(Session)");
	OstTraceDef1(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET11_2, "Pdp ID %d",iPdpId);

	iSessionInfo = aParam ;
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSM_SET11_3, "<<CPdpFsm::Get(Session)");
    }    

void CPdpFsm::InitialiseContextConfig(TUint32 aConfigRel)
    {
    ASSERT(aConfigRel == TPacketDataConfigBase::KConfigGPRS
        || aConfigRel == TPacketDataConfigBase::KConfigRel99Rel4
        || aConfigRel == TPacketDataConfigBase::KConfigRel5);
    
    switch(aConfigRel)
        {
        case TPacketDataConfigBase::KConfigGPRS:
            {
            RPacketContext::TContextConfigGPRS tmp;
            Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &tmp, sizeof(tmp));
            }
            break;
            
        case TPacketDataConfigBase::KConfigRel99Rel4:
            {
            RPacketContext::TContextConfigR99_R4 tmp;
            Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &tmp, sizeof(tmp));
            }
            break;
            
        case TPacketDataConfigBase::KConfigRel5:
            {
            RPacketContext::TContextConfig_R5 tmp;
            Mem::Copy(const_cast<TUint8*>(iContextConfig.Ptr()), &tmp, sizeof(tmp));
            }
            break;
        }
    }


