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

void CPdpFsm::ConstructL()
	{
	// Create EtelDriver for this ID - does nothing on ETel
	iEtelDriverInput->CreatePdpL(iPdpId);
	
	// we need a state
	iState = &iPdpFsmFactory->iStateInitialised;
	}
	
CPdpFsm* CPdpFsm::NewL(TContextId aPdpId, CPdpFsmFactory * aPdpFsmFactory, REtelDriverInput * aEtelDriverInput)
	{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::NewL()");
	CPdpFsm* self = new (ELeave)CPdpFsm(aPdpId, aPdpFsmFactory, aEtelDriverInput);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CPdpFsm::CPdpFsm(TContextId aPdpId, CPdpFsmFactory * aPdpFsmFactory, REtelDriverInput * aEtelDriverInput) :
	iPdpFsmFactory(aPdpFsmFactory),
	iEtelDriverInput(aEtelDriverInput),
	iPdpId(aPdpId)
// NOTE: Both pointers are valid when called - see CPdpFsmFactory and CPdpFsmInterface. 
// No other caller is expected.
    {
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	}
	
CPdpFsm::~CPdpFsm()
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::~CPdpFsm()");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
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

void CPdpFsm::Get(RPacketContext::TContextConfigGPRS& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Get(RPacketContext::TContextConfigGPRS)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	aParam = iGPRS;
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

void CPdpFsm::Set(const RPacketContext::TContextConfigGPRS& aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::Set(RPacketContext::TContextConfigGPRS)");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);

	iGPRS = aParam;
    }

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

void CPdpFsm::GetLastErrorCause  (TInt& aLastErrorCause)
{
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::GetLastErrorCause");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	
	iEtelDriverInput->GetLastErrorCause (iPdpId, aLastErrorCause);
}

void CPdpFsm::SpudInput (TInt aNotification, TInt aParam)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsm::SpudInput");
	SPUDFSMVERBOSE_LOG1(_L("Pdp ID %d"),iPdpId);
	
	iPdpFsmFactory->SpudInput (iPdpId, aNotification, aParam);
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
