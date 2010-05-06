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
// PDP FSM interface header
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef RPDPFSMINTERFACE_H
#define RPDPFSMINTERFACE_H

#include <e32base.h>
#include <etelmm.h>
#include <etelqos.h>

#include <networking/umtsnifcontrolif.h>
#include <networking/mpdpfsminterface.h>
#include <etelpckt.h>



#ifdef SYMBIAN_NETWORKING_UMTSR5
class TQoSR5Requested;
class TQoSR5Negotiated;

	#ifdef _DEBUG
		_LIT(KDescNullPdpFsm, "iFsmImpl=NULL");
	#endif		

#else
class TQoSR99_R4Requested;
class TQoSR99_R4Negotiated;

#endif 
// SYMBIAN_NETWORKING_UMTSR5 


class TFTInfo;
class TFTOperationCode;
class CPdpFsmFactory;

NONSHARABLE_CLASS(RPdpFsmInterface) : public MPdpFsmInterface
/**
 * R-class to proxy methods calls to CPdpFsmInterface.
 *
 * @internalComponent
 */
 {
public:
	RPdpFsmInterface();
	virtual ~RPdpFsmInterface();
	
	virtual void OpenL(MSpudManInterface * aSpudManInterface, TName& aName);
	virtual void Close();

	virtual TInt Input (TContextId aPdpId, const TInt aOperation, const TInt aParam=KErrNone);

	
	
#ifdef SYMBIAN_NETWORKING_UMTSR5

	// This is to set the status of IM CN signalling parameter
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR5Requested& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR5Negotiated& aParam);
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR5Requested& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR5Negotiated& aParam) const;
	virtual void SetIMCNSignalling(TBool aImCnSignalling);
#else 
// !SYMBIAN_NETWORKING_UMTSR5

	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Requested& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Negotiated& aParam);
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Requested& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Negotiated& aParam) const;

#endif 
// SYMBIAN_NETWORKING_UMTSR5 

	virtual TInt Set(TContextId aPdpId, const TTFTInfo& aParam);
	virtual TInt Set(TContextId aPdpId, const TTFTOperationCode& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TDataChannelV2& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TContextConfigGPRS& aParam);
	virtual TInt Set(TContextId aPdpId, const RPacketContext::TContextStatus& aParam);
	
	virtual TInt Get(TContextId aPdpId, TTFTInfo& aParam) const;
	virtual TInt Get(TContextId aPdpId, TTFTOperationCode& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TDataChannelV2& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TContextConfigGPRS& aParam) const;
	virtual TInt Get(TContextId aPdpId, RPacketContext::TContextStatus& aParam) const;
	
	virtual TInt GetLastErrorCause(TContextId aPdpId, TInt& aLastErrorCause) const;

	virtual void Set(RPacketService::TStatus aParam); // applies to all contexts
	virtual void Get(RPacketService::TStatus& aParam); // applies to all contexts

	// additional accessors
	virtual const TName& TsyName(void);
	
	void Init (const MPdpFsmInterface* aSpudMan);
	
	inline TBool IsInitialised ();

	virtual void SetContextTerminationErrorAndStop(TContextId aPdpId, TInt aErrorCode);

protected:
private:
	// DO NOT provide implementations to these methods: Copying this object should be disabled
	RPdpFsmInterface(RPdpFsmInterface&);
	RPdpFsmInterface& operator= (RPdpFsmInterface& aInput);
	
public:
protected:
private:
	// FSM Factory - churns out FSMs
	CPdpFsmFactory* iPdpFsmFactory;

	RPacketService::TStatus iNetworkStatus;
	MPdpFsmInterface* iFsmImpl;
};

IMPORT_C RPdpFsmInterface* NewPdpFsmInterfaceL();

	   
TBool RPdpFsmInterface::IsInitialised ()
   {
   return iFsmImpl != NULL;
   }


#endif
// RPDPFSMINTERFACE_H

