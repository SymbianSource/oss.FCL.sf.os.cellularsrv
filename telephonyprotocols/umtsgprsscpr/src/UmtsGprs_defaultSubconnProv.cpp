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
// Implementation file for the Default Umts/Gprs SubConnection Provider
// 
//

/**
 @file
 @internalComponent
*/


#include "UmtsGprs_defaultSubconnProv.h"
#include "UmtsGprs_subconnProvFactory.h"
#include "PdpFsmRequest.h"

#include <commsdattypesv1_1.h>
#include <ss_connsettings.h>

#ifdef SYMBIAN_NETWORKING_UMTSR5
#include <ip_subconparams.h>
#endif	

CUmtsGprsDefaultSubConnProvd* CUmtsGprsDefaultSubConnProvd::NewL(CSubConnectionProviderFactoryBase& aFactory, 
	                          CConnectionProviderBase& aConnProvider)
    {
    CUmtsGprsDefaultSubConnProvd* provider = new(ELeave)CUmtsGprsDefaultSubConnProvd(aFactory, aConnProvider);
	CleanupStack::PushL(provider);
    provider->ConstructL();
    CleanupStack::Pop(provider);
    return provider;
    }



CUmtsGprsDefaultSubConnProvd::CUmtsGprsDefaultSubConnProvd(
                              CSubConnectionProviderFactoryBase& aFactory, 
	                          CConnectionProviderBase& aConnProvider)
:CUmtsGprsSubConnProvd(aFactory, aConnProvider),
 iPdpFsm(NULL)
/** 
Constructs a default UMTS SubConnection Provider

@param aFactory CSubConnectionProviderFactoryBase that should own the reference to 'this'
@param aConnProvider a connection that 'this' subconnection belongs to
*/
	{
	iContextId = KPrimaryContextId;
	}


void CUmtsGprsDefaultSubConnProvd::ConstructL()
    {
    iPdpFsm = new(ELeave) CPdpFsmInterface(*this);
    }
    
    
CUmtsGprsDefaultSubConnProvd::~CUmtsGprsDefaultSubConnProvd()
    {
    delete iPdpFsm;
    }
    
    
CUmtsGprsSubConnProvd& CUmtsGprsDefaultSubConnProvd::GetDefaultSubConnection()
    {
    return *this;
    }

TBool CUmtsGprsDefaultSubConnProvd::ShouldIDeleteNow()
    {
    // Never allow client leaving events to destroy this default scpr
    // The CPr will call ConnectionGoingDown when it is deleted
    // and then we will delete this.
    return EFalse;
    }


#ifdef SYMBIAN_NETWORKING_UMTSR5

TInt CUmtsGprsDefaultSubConnProvd::AddIMCNInformationToParameterBundleL(TBool aImCnInfo)

/**
Will add the information related to the IMCN Signalling Flag into the SubCon Parameter Bundle
so that it can be accessible by the IPSCPR while restricting the sockets

@param aImCnInfo Grant status of the IMCN signalling flag  
*/

	{
	// Check and create the parameter bundle if NULL
	if (iParameterBundle == NULL)
		{
		iParameterBundle = CSubConParameterBundle::NewL();
		}
		
	// Add information Regarding the grant status to the parameter bundle with family
    CSubConParameterFamily * imcnFamily = CSubConParameterFamily::NewL(*iParameterBundle,KSubConnContextDescrParamsFamily);
	CleanupStack::PushL(imcnFamily);
	CSubConImsExtParamSet *imcnFlag = CSubConImsExtParamSet::NewL(*imcnFamily,CSubConParameterFamily::EGranted);
	CleanupStack::Pop(imcnFamily);
	// Set the Signalling Indication
	imcnFlag->SetImsSignallingIndicator(aImCnInfo);
	
	return KErrNone;
	}

#endif //SYMBIAN_NETWORKING_UMTSR5


void CUmtsGprsDefaultSubConnProvd::DoParametersAboutToBeSetL(CSubConParameterBundle& aParameterBundle)
/** Should set parameters on the primary/default PDP context represented by 'this'

@param 
*/
{
	__FLOG_1(_L("CUmtsGprsDefaultSubConnProvd [this=%08x]:\tDoParametersAboutToBeSetL()"), this);

	if (iCurrentPdpFsmReq)
	{
	User::Leave(KErrInUse);
	}

#ifdef SYMBIAN_NETWORKING_UMTSR5
	// For PREQ 635, The information regarding the dedicated signalling indication is used to be stored in the
	// iParameterBundle of the Base class. Since after this call and lower layer calls (if any) is completed then 
	// iParameterBundle will be replaced by aParameterBundle coming as formal function parameters. 

	// To be able to save the information if somebody calls RSubConnection::SetParameters(...), which is there in 
	// existing Parameter Bundle we need to copy the information about IMCN Signalling flag which is already there 
	// iParameterBundle into aParameterBundle. 
	
	// The catch here is that The layer above this must call the SetParameters() to get the parameter values, because
	// This particular call adds the granted status

	// Check for the existance of the appropriate family
	CSubConParameterFamily * imCnFamily = iParameterBundle != NULL ? iParameterBundle->FindFamily(KSubConnContextDescrParamsFamily): NULL;
	if (imCnFamily)
		{
		// we have the Family, get the status of IMCN signalling Flag
		CSubConImsExtParamSet *imCnSig = static_cast<CSubConImsExtParamSet*>
    	(imCnFamily->FindExtensionSet(STypeId::CreateSTypeId(KSubConIPParamsUid,KSubConImsExtParamsType), 
    	CSubConParameterFamily::EGranted));
		
		// We have the variable, copy the contents
		if(imCnSig)
    		{ 
    		CSubConParameterFamily * contextFamily = CSubConParameterFamily::NewL(aParameterBundle,KSubConnContextDescrParamsFamily);
			CleanupStack::PushL(contextFamily);
    	  	CSubConImsExtParamSet *imCnFlag = CSubConImsExtParamSet::NewL(*contextFamily,CSubConParameterFamily::EGranted);
			CleanupStack::Pop(contextFamily);
		    imCnFlag->SetImsSignallingIndicator(imCnSig->GetImsSignallingIndicator());
    	  	}
		}

#endif // SYMBIAN_NETWORKING_UMTSR5 

    ResetQosRelease();
	
#ifdef SYMBIAN_NETWORKING_UMTSR5
    
        RPacketQoS::TQoSR5Requested requestedParams;
        MapQosParamBundleToEtelL(aParameterBundle, &requestedParams);
        CModifyR5QoS* modifyR5QoS = CModifyR5QoS::NewL(*this, *this, requestedParams);
        iCurrentPdpFsmReq = modifyR5QoS;
        modifyR5QoS->Start();
        
#else
		RPacketQoS::TQoSR99_R4Requested requestedParams;
		MapQosParamBundleToEtelL(aParameterBundle, &requestedParams);
		CModifyR4QoS* modifyR4QoS = CModifyR4QoS::NewL(*this, *this, requestedParams);
		iCurrentPdpFsmReq = modifyR4QoS;
		modifyR4QoS->Start();
		
#endif // SYMBIAN_NETWORKING_UMTSR5

}
    

void CUmtsGprsDefaultSubConnProvd::GetQoSDefaultsL(RPacketQoS::TQoSR99_R4Requested* aParams)
{
   CConnectionSettings& settingAccess = iConnectionProvider->SettingsAccessL();
	TUint32 data;
	    
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqTrafficClass),data, NULL);
	aParams->iReqTrafficClass = (RPacketQoS::TTrafficClass)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMinTrafficClass),data, NULL);
	aParams->iMinTrafficClass = (RPacketQoS::TTrafficClass)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqDeliveryOrder),data, NULL);
	aParams->iReqDeliveryOrderReqd = (RPacketQoS::TDeliveryOrder)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMinDeliveryOrder),data, NULL);
	aParams->iMinDeliveryOrderReqd = (RPacketQoS::TDeliveryOrder)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqDeliverErroneousSDU),data, NULL);
	aParams->iReqDeliverErroneousSDU = (RPacketQoS::TErroneousSDUDelivery)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMinDeliverErroneousSDU),data, NULL);
	aParams->iMinDeliverErroneousSDU = (RPacketQoS::TErroneousSDUDelivery)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqMaxSDUSize),data, NULL);
	aParams->iReqMaxSDUSize = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMinAcceptableMaxSDUSize),data, NULL);
	aParams->iMinAcceptableMaxSDUSize = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqMaxUplinkRate),data, NULL);
	aParams->iReqMaxRate.iUplinkRate = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqMinUplinkRate),data, NULL);
	aParams->iMinAcceptableMaxRate.iUplinkRate = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqMaxDownlinkRate),data, NULL);
	aParams->iReqMaxRate.iDownlinkRate = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqMinDownlinkRate),data, NULL);
	aParams->iMinAcceptableMaxRate.iDownlinkRate = (TInt)data;
		
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqBER),data, NULL);
	aParams->iReqBER = (RPacketQoS::TBitErrorRatio)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMaxBER),data, NULL);
	aParams->iMaxBER = (RPacketQoS::TBitErrorRatio)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqSDUErrorRatio),data, NULL);
	aParams->iReqSDUErrorRatio = (RPacketQoS::TSDUErrorRatio)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMaxSDUErrorRatio),data, NULL);
	aParams->iMaxSDUErrorRatio = (RPacketQoS::TSDUErrorRatio)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqTrafficHandlingPriority),data, NULL);
	aParams->iReqTrafficHandlingPriority = (RPacketQoS::TTrafficHandlingPriority)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMinTrafficHandlingPriority),data, NULL);
	aParams->iMinTrafficHandlingPriority = (RPacketQoS::TTrafficHandlingPriority)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqTransferDelay),data, NULL);
	aParams->iReqTransferDelay = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMaxTransferDelay),data, NULL);
	aParams->iMaxTransferDelay = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqGuaranteedUplinkRate),data, NULL);
	aParams->iReqGuaranteedRate.iUplinkRate = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMinGuaranteedUplinkRate),data, NULL);			
	aParams->iMinGuaranteedRate.iUplinkRate = (TInt)data;
	
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSReqGuaranteedDownlinkRate),data, NULL);
	aParams->iMinGuaranteedRate.iDownlinkRate = (TInt)data;
		
	settingAccess.GetIntSettingL(TPtrC(KCDTypeNameGPRSMinGuaranteedDownlinkRate),data, NULL);
	aParams->iMinGuaranteedRate.iDownlinkRate = (TInt)data;
}




/** 
This override of the DoControl() method is used to initialise the RPdpFsmInterface object
in the SpudMan with a pointer to the real CPdpFsmInterface object owned by the 
subconnection provider.

@param aOptionLevel
@param aOptionName
@param aOption
*/
TInt CUmtsGprsDefaultSubConnProvd::DoControl(TUint aOptionLevel, TUint aOptionName, TDes8& aOption)
    {
	__FLOG_1(_L("CUmtsGprsDefaultSubConnProvd::DoControl [%08x]"), this);
	if (aOptionLevel == KSOLInterface && aOptionName == KInitialisePdpFsm)
    	{
    	CNifIfBase* nif = *reinterpret_cast<CNifIfBase**>(const_cast<TUint8*>(aOption.Ptr()));
    	ASSERT(nif != NULL);
        TPckg<MPdpFsmInterface*> pdpfsm(iPdpFsm);    	
    	return nif->Control(KSOLInterface, KInitialisePdpFsm, pdpfsm);
    	}

    return CUmtsGprsSubConnProvd::Control(aOptionLevel, aOptionName, aOption);
    }
    
    
CPdpFsmInterface& CUmtsGprsDefaultSubConnProvd::GetPdpFsmInterface()
    {
    return *iPdpFsm;
    }
    
