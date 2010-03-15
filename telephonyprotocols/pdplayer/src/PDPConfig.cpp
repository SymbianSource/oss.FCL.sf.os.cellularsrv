// Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// PDP Config structures for TProvision message.
// 
//

/**
 @file
 @internalComponent
*/

#include <hash.h>
#include <e32math.h>
#include <comms-infras/ss_log.h>
#include <in_sock.h>
#include <comms-infras/metadata.h>
#include <comms-infras/agentmcpr.h>
#include <comms-infras/ss_log.h>
#include <comms-infras/ss_tiermanagerutils.h>

#include "PDPProvision.h"

using namespace ESock;

#if defined(__CFLOG_ACTIVE)
#define KPDPMCprTag KESockMetaConnectionTag
_LIT8(KPDPMCprSubTag, "pdpmcpr");
#endif

const TUint8 KGenericNifChallengeSize = 8;
const TUint KGenericNifIdLength = 1;

//
// Attribute table for provisioning structure passed to CFProtocol
//
START_ATTRIBUTE_TABLE(CGPRSProvision, CGPRSProvision::EUid, CGPRSProvision::ETypeId)
// No attributes defined, as no serialisation takes place.
END_ATTRIBUTE_TABLE()

START_ATTRIBUTE_TABLE(CTSYProvision, CGPRSProvision::EUid, CTSYProvision::ETypeId)
// No attributes defined, as no serialisation takes place.
END_ATTRIBUTE_TABLE()

START_ATTRIBUTE_TABLE(CDefaultPacketQoSProvision, CDefaultPacketQoSProvision::EUid, CDefaultPacketQoSProvision::ETypeId)
// No attributes defined, as no serialisation takes place.
END_ATTRIBUTE_TABLE()

START_ATTRIBUTE_TABLE(CImsExtProvision, CImsExtProvision::EUid, CImsExtProvision::ETypeId)
// No attributes defined, as no serialisation takes place.
END_ATTRIBUTE_TABLE()

CGPRSProvision* CGPRSProvision::NewLC(ESock::CCommsDatIapView* aIapView)
	{
	CGPRSProvision* self = new (ELeave) CGPRSProvision;
	CleanupStack::PushL(self);
	self->InitialiseConfigL(aIapView);
	return self;
	}
TInt CGPRSProvision::UmtsGprsRelease() const
	{
	return iUmtsGprsRelease;
	}

void CGPRSProvision::InitialiseConfigL(ESock::CCommsDatIapView* aIapView)
    {	
	TInt getErr = aIapView->GetInt(KCDTIdUmtsGprsRelease | KCDTIdOutgoingGprsRecord, iUmtsGprsRelease);
		
	if (getErr == KErrNotFound || getErr == KErrUnknown)
	    {
	    // default to GPRS
	    getErr = KErrNone;
	    iUmtsGprsRelease = TPacketDataConfigBase::KConfigGPRS;
	    }
	    
	TBool useEdge = EFalse;
	
	aIapView->GetBoolL(KCDTIdWCDMAUseEdge | KCDTIdOutgoingGprsRecord, useEdge);
	
	getErr = InitialiseScratchContext(iUmtsGprsRelease);
	//-====================================================
	//LENIENT PROVISION
	//-====================================================
	//With some provision parameters (e.g.: pdpAddress), CGPRSProvision 
	//tries to be lenient as technically they can be supplied by
	//someone further in the chain (e.g.: GPRS), if not found in the db.
	switch (iUmtsGprsRelease)
		{
    	case TPacketDataConfigBase::KConfigGPRS:
			{
            RPacketContext::TContextConfigGPRS& contextGPRS = GetScratchContextAs<RPacketContext::TContextConfigGPRS>();
            RetrievePdpTypeL(contextGPRS.iPdpType, aIapView);
            RetrieveApnNameL(contextGPRS.iAccessPointName, aIapView);
            RetrieveIPAndDnsSettingsL(contextGPRS.iPdpAddress,
                contextGPRS.iProtocolConfigOption.iDnsAddresses.iPrimaryDns,
                contextGPRS.iProtocolConfigOption.iDnsAddresses.iSecondaryDns, aIapView);
            RetrieveAuthenticationInfoL(contextGPRS.iProtocolConfigOption, aIapView);
            // GPRS specific
        	RetrieveGprsCompression(contextGPRS.iPdpCompression, aIapView);
        	RetrieveGprsAnonymousAccess(contextGPRS.iAnonymousAccessReqd, aIapView);
        	contextGPRS.iUseEdge = useEdge;
			}
			break;
	    case TPacketDataConfigBase::KConfigRel5:
			{
            // Rel 5 specific
            RPacketContext::TContextConfig_R5& contextR5 = GetScratchContextAs<RPacketContext::TContextConfig_R5>();
            RetrieveUmtsCompressionL(contextR5.iPdpHeaderCompression, contextR5.iPdpDataCompression, aIapView);
            
            RetrievePdpTypeL(contextR5.iPdpType, aIapView);
            RetrieveApnNameL(contextR5.iAccessPointName, aIapView);
            RetrieveIPAndDnsSettingsL(contextR5.iPdpAddress,
            		contextR5.iProtocolConfigOption.iDnsAddresses.iPrimaryDns,
            		contextR5.iProtocolConfigOption.iDnsAddresses.iSecondaryDns, aIapView);
            RetrieveAuthenticationInfoL(contextR5.iProtocolConfigOption, aIapView);
            contextR5.iUseEdge = useEdge;

    		RetrieveUmtsPacketFlowIdentifierL(contextR5.iPFI, aIapView);
			}
			break;
	    case TPacketDataConfigBase::KConfigRel99Rel4:
			{
            RPacketContext::TContextConfigR99_R4& contextUMTS = GetScratchContextAs<RPacketContext::TContextConfigR99_R4>();

            RetrievePdpTypeL(contextUMTS.iPdpType, aIapView);
            RetrieveApnNameL(contextUMTS.iAccessPointName, aIapView);
            RetrieveIPAndDnsSettingsL(contextUMTS.iPdpAddress,
                contextUMTS.iProtocolConfigOption.iDnsAddresses.iPrimaryDns,
                contextUMTS.iProtocolConfigOption.iDnsAddresses.iSecondaryDns, aIapView);
            RetrieveAuthenticationInfoL(contextUMTS.iProtocolConfigOption, aIapView);
            contextUMTS.iUseEdge = useEdge;
            RetrieveUmtsPacketFlowIdentifierL(contextUMTS.iPFI, aIapView);
			}
			break;
	    default:
	    	User::Leave(KErrNotSupported);
		}
    }

void CGPRSProvision::RetrieveGprsCompression(TUint& aCompression, ESock::CCommsDatIapView* aIapView) const
	{
	aCompression = 0;
	TBool isCompression = EFalse;
	aIapView->GetBool(KCDTIdWCDMADataCompression | KCDTIdOutgoingGprsRecord, isCompression);
	if (isCompression)
		{
		aCompression |= RPacketContext::KPdpDataCompression;
		}

	isCompression = EFalse;
	aIapView->GetBool(KCDTIdWCDMAHeaderCompression | KCDTIdOutgoingGprsRecord, isCompression);
	if (isCompression)
		{
		aCompression |= RPacketContext::KPdpHeaderCompression;
		}
	__CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CGPRSProvision [this=%08x]::InitialiseConfigL() KCDTIdWCDMADataCompression|KCDTIdWCDMAHeaderCompression [%d]"), this, aCompression));
	}

void CGPRSProvision::RetrieveGprsAnonymousAccess(RPacketContext::TAnonymousAccess& aAnonymous, ESock::CCommsDatIapView* aIapView) const
	{	
	TBool isAnonymous = EFalse;
	aIapView->GetBool(KCDTIdWCDMAAnonymousAccess | KCDTIdOutgoingGprsRecord,isAnonymous);
	if (isAnonymous)
		aAnonymous = RPacketContext::ERequired;
	else
		aAnonymous = RPacketContext::ENotRequired;
	
	__CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CGPRSProvision [this=%08x]::InitialiseConfigL() KCDTIdWCDMAAnonymousAccess [%d]"), this, aAnonymous));
	}

void CGPRSProvision::RetrieveApnNameL(TDes8& aApnName, ESock::CCommsDatIapView* aIapView) const
	{
	HBufC* buf = NULL;
	TInt getErr;

	if ((getErr = aIapView->GetText(KCDTIdAPN | KCDTIdOutgoingGprsRecord, buf)) == KErrNone)
    		{
    		aApnName.Copy(*buf);
    		delete buf;
    		}
    	else if (getErr != KErrNotFound )
    		{
    		User::Leave(getErr);
    		}
	__CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CGPRSProvision [this=%08x]::InitialiseConfigL() KCDTIdAPN [%S]"), this, &aApnName));
	}

void CGPRSProvision::RetrievePdpTypeL(RPacketContext::TProtocolType& aPdpType, ESock::CCommsDatIapView* aIapView) const	
	{
	TUint32 pdpType(0);

	aIapView->GetIntL(KCDTIdWCDMPDPType | KCDTIdOutgoingGprsRecord, pdpType);
	aPdpType = static_cast<RPacketContext::TProtocolType>(pdpType);
	__CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CGPRSProvision [this=%08x]::InitialiseConfigL() KCDTIdWCDMPDPType=%d"), this, aPdpType)); 	
	}

void CGPRSProvision::RetrieveAuthenticationInfoL(RPacketContext::TProtocolConfigOptionV2& aProtocolConfigOption, ESock::CCommsDatIapView* aIapView)
    {
    HBufC* buf = NULL;
    TInt getErr;

    __CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CGPRSProvision [this=%08x]::RetrieveAuthenticationInfoL()"), this));
    
    getErr = aIapView->GetText(KCDTIdWCDMAIfAuthName | KCDTIdOutgoingGprsRecord, buf);
    if ( getErr == KErrNone )
        {
        ASSERT(buf);
        aProtocolConfigOption.iAuthInfo.iUsername.Copy(*buf);
        delete buf;
        buf = NULL;

        __CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CGPRSProvision [this=%08x]::RetrieveAuthenticationInfoL() KCDTIdWCDMAIfAuthName [%S] "), this, &aProtocolConfigOption.iAuthInfo.iUsername));

        aProtocolConfigOption.iId = 1;

        TBool isDisableAuth = EFalse;
        getErr = aIapView->GetBool(KCDTIdWCDMADisablePlainTextAuth | KCDTIdOutgoingGprsRecord,isDisableAuth);
        __CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CGPRSProvision [this=%08x]::RetrieveAuthenticationInfoL() KCDTIdWCDMADisablePlainTextAuth [%d]"), this, isDisableAuth));
        if (getErr == KErrNone)
            {
            getErr = aIapView->GetText(KCDTIdWCDMAIfAuthPass | KCDTIdOutgoingGprsRecord, buf);
            if ( getErr == KErrNone )
                {
                ASSERT(buf);
                aProtocolConfigOption.iAuthInfo.iPassword.Copy(*buf);
                delete buf;
                buf = NULL;

                __CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CGPRSProvision [this=%08x]::RetrieveAuthenticationInfoL() KCDTIdWCDMAIfAuthPass [%S]"), this, &aProtocolConfigOption.iAuthInfo.iPassword));

                if (isDisableAuth)  //If Disable, CHAP will be used.
                    {
                    aProtocolConfigOption.iAuthInfo.iProtocol = RPacketContext::EProtocolCHAP;
                    CreateChallengeAndResponseForChapL(aProtocolConfigOption);
                    }
                else
                    {
                    aProtocolConfigOption.iAuthInfo.iProtocol = RPacketContext::EProtocolPAP;
                    }
                }
            else if (getErr == KErrNotFound)
                {
                if (isDisableAuth) //ERROR: CHAP used without password???
                    {
                    User::Leave(KErrArgument);
                    }
                else    //PAP used without password.
                    {
                    aProtocolConfigOption.iAuthInfo.iProtocol = RPacketContext::EProtocolPAP;
                    }
                }
            else    //getErr != KErrNotFound
                {
                User::Leave(getErr);
                }
            }
        else
            {
            User::Leave(getErr);
            }
        }
    else if (getErr == KErrNotFound )   //No Authentication Required.
        {
        aProtocolConfigOption.iAuthInfo.iProtocol = RPacketContext::EProtocolNone;
        }
    else    //getErr != KErrNotFound
        {
        User::Leave(getErr);
        }
    }

void CGPRSProvision::CreateChallengeAndResponseForChapL(RPacketContext::TProtocolConfigOptionV2& aProtocolConfigOption)
    {
    //Challenge
    TTime currentTime;
    currentTime.UniversalTime();
    
    TInt64 seedValue = currentTime.Int64();

    TUint8 challenge[KGenericNifChallengeSize] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    TUint8 i=0;
    while(i < KGenericNifChallengeSize)
        {
        challenge[i] = (TUint8)(Math::Rand(seedValue)%256);
        aProtocolConfigOption.iChallenge.Append(challenge[i++]);
        }

    //Response
    TBuf8<KGenericNifIdLength+KCommsDbSvrMaxColumnNameLength+KGenericNifChallengeSize> message;
    message.Append(aProtocolConfigOption.iId);
    message.Append(aProtocolConfigOption.iAuthInfo.iPassword);
    message.Append(aProtocolConfigOption.iChallenge);
    
    TInt length = 1 /*iId length */ + aProtocolConfigOption.iAuthInfo.iPassword.Length() + KGenericNifChallengeSize;

    HBufC8* buf = HBufC8::NewL(length);
    
    CleanupStack::PushL(buf);
     
    TPtr8 ptr((TUint8*)buf->Des().Ptr(),length);
    
    ptr.Copy(message);
    
    CMD5* md5=0;
    md5 = CMD5::NewL();
    
    CleanupStack::PushL(md5);
    
    TPtrC8 Response = md5->Hash(ptr);
    
    aProtocolConfigOption.iResponse.Copy(Response);
    
    CleanupStack::PopAndDestroy(2);
    }

void CGPRSProvision::RetrieveIPAndDnsSettingsL(TDes8& aPdpAddress, TDes8& aDns1, TDes8& aDns2, ESock::CCommsDatIapView* aIapView) const
	{
	HBufC* buf = NULL;

	aPdpAddress.SetLength(0);
	aDns1.SetLength(0);
	aDns2.SetLength(0);
	
	TBool fromServer;
	aIapView->GetBoolL(KCDTIdWCDMAIPAddrFromServer | KCDTIdOutgoingGprsRecord, fromServer);
	if (!fromServer)
	    {
    	aIapView->GetTextL(KCDTIdWCDMAIPAddr | KCDTIdOutgoingGprsRecord, buf);
    	TInetAddr addr;
    	User::LeaveIfError(addr.Input(*buf));
    	// sending ASCII to MA but checking the validity of the address above.
    	aPdpAddress.Copy(*buf);
    	delete buf;
    	buf = NULL;
	    }
	
    // We can only use IPv4 or IPv6 - we use the first one listed in the IfNetworks field
	aIapView->GetTextL(KCDTIdWCDMAIfNetworks | KCDTIdOutgoingGprsRecord, buf);

	TInt pos = buf->Find(_L(","));
	if (pos == KErrNotFound)
	    {
	    pos = buf->Length();
	    }

	TBufC<KCommsDbSvrMaxFieldLength> temp(buf->Mid(0, pos));
	delete buf;
	buf = NULL;
	_LIT(KIp4, "ip");
	_LIT(KIp6, "ip6");
	if (temp.CompareF(KIp4) == 0)
	    {
	    // IPv4 settings
    	aIapView->GetBoolL(KCDTIdWCDMAIPDNSAddrFromServer | KCDTIdOutgoingGprsRecord, fromServer);
    	if (!fromServer)
    	    {
    	    aIapView->GetTextL(KCDTIdWCDMAIPNameServer1 | KCDTIdOutgoingGprsRecord, buf);
            TInetAddr addr;
            User::LeaveIfError(addr.Input(*buf));
            // sending ASCII to MA but checking the validity of the address above.
    	    aDns1.Copy(*buf);
        	delete buf;
        	buf = NULL;
    	    aIapView->GetTextL(KCDTIdWCDMAIPNameServer2 | KCDTIdOutgoingGprsRecord, buf);
    	    User::LeaveIfError(addr.Input(*buf));
            // sending ASCII to MA but checking the validity of the address above.
        	aDns2.Copy(*buf);
    	    delete buf;
        	buf = NULL;
    	    }
	    }
	else if (temp.CompareF(KIp6) == 0)
	    {
	    // IPv6 settings
    	aIapView->GetBoolL(KCDTIdWCDMAIP6DNSAddrFromServer | KCDTIdOutgoingGprsRecord, fromServer);
    	if (!fromServer)
    	    {
    	    aIapView->GetTextL(KCDTIdWCDMAIP6NameServer1 | KCDTIdOutgoingGprsRecord, buf);
            TInetAddr addr;
            User::LeaveIfError(addr.Input(*buf));
            // sending ASCII to MA but checking the validity of the address above.
            aDns1.Copy(*buf);
    	    delete buf;
    		buf = NULL;
    	    aIapView->GetTextL(KCDTIdWCDMAIP6NameServer2 | KCDTIdOutgoingGprsRecord, buf);
            User::LeaveIfError(addr.Input(*buf));
            // sending ASCII to MA but checking the validity of the address above.
          	aDns2.Copy(*buf);
        	delete buf;
            buf = NULL;
    	    }
	    }
	else
	    {
	    // Anything else is a serious misconfiguration
	    User::Leave(KErrArgument);
	    }
	}
void CGPRSProvision::RetrieveUmtsCompressionL(RPacketContext::THeaderCompression& aHeaderCompression, RPacketContext::TDataCompression& aDataCompression, ESock::CCommsDatIapView* aIapView) const
	{
	TUint32 headerComp;
	TUint32 dataComp;
	TInt err;
	
	err = aIapView->GetInt(KCDTIdR5DataCompression | KCDTIdOutgoingGprsRecord, headerComp);
	if (err == KErrNotFound || err == KErrUnknown)
	    {
	    err = KErrNone;
	    headerComp = RPacketContext::ENoHeaderCompression;
	    }
	User::LeaveIfError(err);
	
	aHeaderCompression = static_cast<RPacketContext::THeaderCompression>(headerComp);
	
	err = aIapView->GetInt(KCDTIdR5HeaderCompression | KCDTIdOutgoingGprsRecord, dataComp);
	if (err == KErrNotFound || err == KErrUnknown)
	    {
	    err = KErrNone;
	    dataComp = RPacketContext::ENoDataCompression;
	    }
	User::LeaveIfError(err);

	aDataCompression = static_cast<RPacketContext::TDataCompression>(dataComp);	
	}

void CGPRSProvision::RetrieveUmtsPacketFlowIdentifierL(RPacketContext::TPacketFlowIdentifier& aPfi, ESock::CCommsDatIapView* aIapView) const
	{
	TUint32 pfi;
	TInt err = aIapView->GetInt(KCDTIdPacketFlowIdentifier | KCDTIdOutgoingGprsRecord, pfi);
	if (err == KErrNotFound || err == KErrUnknown)
	    {
	    err = KErrNone;
	    pfi = RPacketContext::EBestEffort;
	    }
	User::LeaveIfError(err);
	aPfi = static_cast<RPacketContext::TPacketFlowIdentifier>(pfi);
	}


TInt CGPRSProvision::InitialiseScratchContext(TUint32 aConfigRel) const
	{
    switch(aConfigRel)
        {
        case TPacketDataConfigBase::KConfigGPRS:
            {
            RPacketContext::TContextConfigGPRS tmp;
            Mem::Copy(const_cast<TUint8*>(iScratchContext.Ptr()), &tmp, sizeof(tmp));
            }
            break;
            
        case TPacketDataConfigBase::KConfigRel99Rel4:
            {
            RPacketContext::TContextConfigR99_R4 tmp;
            Mem::Copy(const_cast<TUint8*>(iScratchContext.Ptr()), &tmp, sizeof(tmp));
            }
            break;
            
        case TPacketDataConfigBase::KConfigRel5:
            {
            RPacketContext::TContextConfig_R5 tmp;
            Mem::Copy(const_cast<TUint8*>(iScratchContext.Ptr()), &tmp, sizeof(tmp));
            }
            break;
        default:
        	return KErrNotSupported;
        }
    return KErrNone;
    }


CTSYProvision* CTSYProvision::NewLC(ESock::CCommsDatIapView* aIapView)
	{
	CTSYProvision* self = new (ELeave) CTSYProvision;
	CleanupStack::PushL(self);
	self->InitialiseConfigL(aIapView);
	return self;
	}

void CTSYProvision::InitialiseConfigL(ESock::CCommsDatIapView* aIapView)
    {
	HBufC* buf = NULL;
	
	aIapView->GetTextL(KCDTIdTsyName, buf);
	iTsyName.Copy(*buf);
	__CFLOG_VAR((KPDPMCprTag, KPDPMCprSubTag, _L8("CTSYProvision [this=%08x]::InitialiseConfigL() KCDTIdTsyName=%s"), this, &iTsyName)); 	
	delete buf;
    }

CDefaultPacketQoSProvision* CDefaultPacketQoSProvision::NewL(ESock::CCommsDatIapView* aIapView)
	{
	CDefaultPacketQoSProvision* self = new (ELeave) CDefaultPacketQoSProvision;
	CleanupStack::PushL(self);
	self->InitialiseConfigL(aIapView);
	CleanupStack::Pop(self);
	return self;
	}
	
 void CDefaultPacketQoSProvision::InitialiseConfigL(ESock::CCommsDatIapView* aIapView)
    {
    TUint32 data;
	    
	aIapView->GetIntL(KCDTIdWCDMAReqTrafficClass,data);
	iParams.iReqTrafficClass = (RPacketQoS::TTrafficClass)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMinTrafficClass,data);
	iParams.iMinTrafficClass = (RPacketQoS::TTrafficClass)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqDeliveryOrder,data);
	iParams.iReqDeliveryOrderReqd = (RPacketQoS::TDeliveryOrder)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMinDeliveryOrder,data);
	iParams.iMinDeliveryOrderReqd = (RPacketQoS::TDeliveryOrder)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqDeliverErroneousSDU,data);
	iParams.iReqDeliverErroneousSDU = (RPacketQoS::TErroneousSDUDelivery)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMinDeliverErroneousSDU,data);
	iParams.iMinDeliverErroneousSDU = (RPacketQoS::TErroneousSDUDelivery)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqMaxSDUSize,data);
	iParams.iReqMaxSDUSize = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMinAcceptableMaxSDUSize,data);
	iParams.iMinAcceptableMaxSDUSize = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqMaxUplinkRate,data);
	iParams.iReqMaxRate.iUplinkRate = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqMinUplinkRate,data);
	iParams.iMinAcceptableMaxRate.iUplinkRate = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqMaxDownlinkRate,data);
	iParams.iReqMaxRate.iDownlinkRate = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqMinDownlinkRate,data);
	iParams.iMinAcceptableMaxRate.iDownlinkRate = (TInt)data;
		
	aIapView->GetIntL(KCDTIdWCDMAReqBER,data);
	iParams.iReqBER = (RPacketQoS::TBitErrorRatio)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMaxBER,data);
	iParams.iMaxBER = (RPacketQoS::TBitErrorRatio)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqSDUErrorRatio,data);
	iParams.iReqSDUErrorRatio = (RPacketQoS::TSDUErrorRatio)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMaxSDUErrorRatio,data);
	iParams.iMaxSDUErrorRatio = (RPacketQoS::TSDUErrorRatio)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqTrafficHandlingPriority,data);
	iParams.iReqTrafficHandlingPriority = (RPacketQoS::TTrafficHandlingPriority)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMinTrafficHandlingPriority,data);
	iParams.iMinTrafficHandlingPriority = (RPacketQoS::TTrafficHandlingPriority)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqTransferDelay,data);
	iParams.iReqTransferDelay = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMaxTransferDelay,data);
	iParams.iMaxTransferDelay = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqGuaranteedUplinkRate,data);
	iParams.iReqGuaranteedRate.iUplinkRate = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAMinGuaranteedUplinkRate,data);			
	iParams.iMinGuaranteedRate.iUplinkRate = (TInt)data;
	
	aIapView->GetIntL(KCDTIdWCDMAReqGuaranteedDownlinkRate,data);
	iParams.iReqGuaranteedRate.iDownlinkRate = (TInt)data;
		
	aIapView->GetIntL(KCDTIdWCDMAMinGuaranteedDownlinkRate,data);
	iParams.iMinGuaranteedRate.iDownlinkRate = (TInt)data;

	TBool bdata = EFalse;
#ifdef _DEBUG
	TInt debugErr = 
#endif
	aIapView->GetBool(KCDTIdWCDMASignallingIndication,bdata);
	iParams.iSignallingIndication = bdata;
    }
    
CImsExtProvision* CImsExtProvision::NewLC(ESock::CCommsDatIapView* aIapView)
	{
	CImsExtProvision* self = new (ELeave) CImsExtProvision;
	CleanupStack::PushL(self);
	self->InitialiseConfigL(aIapView);
	return self;
	}

void CImsExtProvision::InitialiseConfigL(ESock::CCommsDatIapView* aIapView)
    {
	TBool bdata = EFalse;
	aIapView->GetBool(KCDTIdWCDMAImCmSignallingIndication,bdata);
	iImsSignalIndicator = bdata;
    }


