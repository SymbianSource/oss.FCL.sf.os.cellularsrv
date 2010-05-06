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
// PDP FSM implementation
// @internalTechnology
// 
//


#include "tpdpstates.h"
#include "cpdpfsm.h"
#include "cpdpfsmfactory.h"
#include "spudfsmdebuglogger.h"
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
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::CPdpFsm()");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	
	// save for later
	iPdpId = aPdpId;
	
	iPdpFsmFactory = aPdpFsmFactory;
	
	iEtelDriverInput = aEtelDriverInput;
	iContextType = aContextType;
	
	// we need a state
	iState = &iPdpFsmFactory->iStateInitialised;
    }
    

CPdpFsm::~CPdpFsm()
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::~CPdpFsm()");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	iContextConfig.Close();
	iEtelDriverInput->DeletePdp(iPdpId);
	iSessionInfo.iSessionIds.Close();	
    }

	
TInt CPdpFsm::Input (const TInt aOperation, const TInt aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Input()");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	
#ifdef _DEBUG // NOT DEBUGRELEASE
	SPUDFSMVERBOSE_LOG1(_L("State : %S"), &iState->iName);
#endif	

	SPUDFSM_LOG3(_L("Pdp ID %d, State '%S', Operation '%S'"),iPdpId, &iState->iName, iState->LogOperation(*this, aOperation));

	return iState->Input (*this, aOperation, aParam);
    }
	

#ifdef SYMBIAN_NETWORKING_UMTSR5
void CPdpFsm::Get(RPacketQoS::TQoSR5Requested& aParam)
{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(QoSR5Req)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	ASSERT(iQosRequested.ExtensionId() == TPacketDataConfigBase::KConfigRel5);
	aParam = iQosRequested.RequestedQoSR5();
}

void CPdpFsm::Get(RPacketQoS::TQoSR5Negotiated& aParam)
{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(QoSR5Neg)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	ASSERT(iQosNegotiated.ExtensionId() == TPacketDataConfigBase::KConfigRel5);
	aParam = iQosNegotiated.NegotiatedQoSR5();
}

#else
// !SYMBIAN_NETWORKING_UMTSR5

void CPdpFsm::Get(RPacketQoS::TQoSR99_R4Requested& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(QosReq)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iQosRequested.RequestedQoSR99_R4();
    }

void CPdpFsm::Get(RPacketQoS::TQoSR99_R4Negotiated& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(QoSNeg)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iQosNegotiated.NegotiatedQoSR99_R4();
    }
    
#endif
// SYMBIAN_NETWORKING_UMTSR5


void CPdpFsm::Get(TTFTInfo& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(TFTInfo)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iTFT;
    }

void CPdpFsm::Get(TTFTOperationCode& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(TFTOperationCode)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iTFTOperationCode;
    }

void CPdpFsm::Get(RPacketContext::TDataChannelV2& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(RPacketContext::TDataChannelV2)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iDataChannelV2;
    }

void CPdpFsm::Get(TPacketDataConfigBase& aParam)
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(TPacketDataConfigBase)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

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
    	}

void CPdpFsm::Get(RPacketContext::TContextStatus& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(RPacketContext::TContextStatus)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iContextStatus;
    }


#ifdef SYMBIAN_NETWORKING_UMTSR5
void CPdpFsm::Set(const RPacketQoS::TQoSR5Requested& aParam)
{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(QoSR5Req)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iQosRequested = aParam;
}

void CPdpFsm::Set(const RPacketQoS::TQoSR5Negotiated& aParam)
{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(QoSR5Neg)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iQosNegotiated = aParam;
}

#else
// !SYMBIAN_NETWORKING_UMTSR5


void CPdpFsm::Set(const RPacketQoS::TQoSR99_R4Requested& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(QoSReq)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iQosRequested = aParam;

    }


void CPdpFsm::Set(const RPacketQoS::TQoSR99_R4Negotiated& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(QoSNeg)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iQosNegotiated = aParam;
    }


#endif 
// SYMBIAN_NETWORKING_UMTSR5


void CPdpFsm::Set(const TTFTInfo& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(TFT)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iTFT = aParam;
    }

void CPdpFsm::Set(const TTFTOperationCode& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(TFTOperationCode)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iTFTOperationCode = aParam;
    }

void CPdpFsm::Set(const RPacketContext::TDataChannelV2& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(RPacketContext::TDataChannelV2)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iDataChannelV2 = aParam;
    }

void CPdpFsm::Set(const RPacketContext::TContextStatus& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(RPacketContext::TContextStatus)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iContextStatus = aParam;
    }

void CPdpFsm::Set(const TPacketDataConfigBase& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(RPacketContext::TContextConfigGPRS)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	
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
    }

#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY	
void CPdpFsm::Set(MPdpFsmEventHandler& aPdpFsmEventHandler)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(MPdpFsmEventHandler&)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	
	if (iPdpFsmEventHandler)
    	{
    	iPdpFsmEventHandler->Event(KContextDeleteEvent, KErrAbort);
    	}
	iPdpFsmEventHandler = &aPdpFsmEventHandler;
    }
#endif  

void CPdpFsm::EtelInput (EtelDriver::TEtelInput aOperation)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::EtelInput");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	
	iEtelDriverInput->Input (iPdpId, aOperation);
    }

void CPdpFsm::EtelCancel (void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::EtelCancel");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	
	iEtelDriverInput->CancelPdp (iPdpId);
    }

void CPdpFsm::SpudInput (TInt aNotification, TInt aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::SpudInput");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY	
	iPdpFsmFactory->SpudInput (iPdpId, aNotification, aParam);
#else
    ASSERT(iPdpFsmEventHandler);
    iPdpFsmEventHandler->Event(aNotification, aParam);
#endif
    }

// state change members

void CPdpFsm::ChangeStateToInitialised(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToInitialised");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : Initialised"));
	
	iState = &iPdpFsmFactory->iStateInitialised;
    }

void CPdpFsm::ChangeStateToOpeningPhone(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToOpeningPhone");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : OpeningPhone"));
	
	iState = &iPdpFsmFactory->iStateOpeningPhone;
    }

void CPdpFsm::ChangeStateToCreatingPrimary(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToCreatingPrimary");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : CreatingPrimary"));
	
	iState = &iPdpFsmFactory->iStateCreatingPrimary;
    }

void CPdpFsm::ChangeStateToActivatingPrimary(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToActivatingPrimary");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : ActivatingPrimary"));
	
	iState = &iPdpFsmFactory->iStateActivatingPrimary;
    }

void CPdpFsm::ChangeStateToCreatingSecondary(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToCreatingSecondary");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : CreatingSecondary"));
	
	iState = &iPdpFsmFactory->iStateCreatingSecondary;
    }

void CPdpFsm::ChangeStateToCreatedSecondary(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToCreatedSecondary");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : CreatedSecondary"));
	
	iState = &iPdpFsmFactory->iStateCreatedSecondary;
    }

void CPdpFsm::ChangeStateToSettingTFT(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToSettingTFT");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : SettingTFT"));
	
	iState = &iPdpFsmFactory->iStateSettingTFT;
    }

void CPdpFsm::ChangeStateToSettingQoS(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToSettingQoS");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : SettingQoS"));
	
	iState = &iPdpFsmFactory->iStateSettingQoS;
    }

void CPdpFsm::ChangeStateToActivatingSecondary(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToActivatingSecondary");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : ActivatingSecondary"));
	
	iState = &iPdpFsmFactory->iStateActivatingSecondary;
    }

void CPdpFsm::ChangeStateToOpen(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToOpen");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : Open"));
	
	iState = &iPdpFsmFactory->iStateOpen;
    }

void CPdpFsm::ChangeStateToChangingQoS(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToChangingQoS");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : ChangingQoS"));
	
	iState = &iPdpFsmFactory->iStateChangingQoS;
    }

void CPdpFsm::ChangeStateToChangingTFT(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToChangingTFT");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : ChangingTFT"));
	
	iState = &iPdpFsmFactory->iStateChangingTFT;
    }

void CPdpFsm::ChangeStateToGettingNegQoS(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToGettingNegQoS");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : GettingNegQoS"));
	
	iState = &iPdpFsmFactory->iStateGettingNegQoS;
    }



void CPdpFsm::ChangeStateToModifingActive(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToModifingActive");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : ModifingActive"));
	
	iState = &iPdpFsmFactory->iStateModifyingActive;
    }

void CPdpFsm::ChangeStateToSuspended(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToSuspended");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : Suspended"));
	
	iState = &iPdpFsmFactory->iStateSuspended;
    }

void CPdpFsm::ChangeStateToClosing(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToClosing");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : Closing"));
	
	iState = &iPdpFsmFactory->iStateClosing;
    }

void CPdpFsm::ChangeStateToStopping(void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToStopping");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : Stopping"));
	
	iState = &iPdpFsmFactory->iStateStopping;
    }
    
void CPdpFsm::ChangeStateToCreatingMbms(void)
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToCreatingMbms");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : Creating"));
	
	iState = &iPdpFsmFactory->iStateCreatingMbms;
	}
	
void CPdpFsm::ChangeStateToActivatingMbms(void)
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToActivatingMbms");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : Activating"));
	
	iState = &iPdpFsmFactory->iStateActivatingMbms;	
	}

void CPdpFsm::ChangeStateToCreatedMbms(void)
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::ChangeStateToCreatingMbms");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	SPUDFSM_LOG(_L("New State : Created"));
	
	iState = &iPdpFsmFactory->iStateCreatedMbms;	
	}


	
void CPdpFsm::Get(RPacketMbmsContext::TContextConfigMbmsV1& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(RPacketMbmsContext::TContextConfigMbmsV1)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iMbms;
    }
    
    
void CPdpFsm::Set(const RPacketMbmsContext::TContextConfigMbmsV1& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(RPacketMbmsContext::TContextConfigMbmsV1)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iMbms = aParam ;
    }
    
    
void CPdpFsm::Get(TSessionOperatioInfo& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(Session)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iSessionInfo;
    }
    
    
void CPdpFsm::Set(const TSessionOperatioInfo& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(Session)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iSessionInfo = aParam ;
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


