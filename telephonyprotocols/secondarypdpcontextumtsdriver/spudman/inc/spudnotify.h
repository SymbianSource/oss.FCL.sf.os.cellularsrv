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
// NIFMAN notification proxy class
// 
//

/**
 @file
 @internalComponent
*/

#ifndef SPUDNOTIFY_H
#define SPUDNOTIFY_H

#include <e32base.h>
#include <comms-infras/nifif.h>
#include <networking/umtsnifcontrolif.h>

class CSpudBinderRef;
class CBindMan;

/**
 * Interface from the nif and the protocol layers to Nifman
 */
NONSHARABLE_CLASS(CSpudNotify) : public CBase, public MNifIfNotify
    {
public:
    CSpudNotify(CBindMan& aBindMan, MNifIfNotify* aNotify, TContextId aContextId);
    ~CSpudNotify();

    virtual void LinkLayerDown(TInt aReason, TAction aAction);
	virtual void LinkLayerUp();
    virtual void NegotiationFailed(CNifIfBase* aIf, TInt aReason);
    virtual TInt Authenticate(TDes& aUsername, TDes& aPassword);
    virtual void CancelAuthenticate();
	virtual TInt GetExcessData(TDes8& aBuffer);


	virtual void IfProgress(TInt aStage, TInt aError);
	virtual void IfProgress(TSubConnectionUniqueId aSubConnectionUniqueId, TInt aStage, TInt aError);
	virtual void OpenRoute();
	virtual void CloseRoute();
	virtual TInt Notification(TNifToAgentEventType aEvent, void * aInfo = NULL);
	virtual void BinderLayerDown(CNifIfBase* aBinderIf, TInt aReason, TAction aAction);
 	virtual TInt PacketActivity(TDataTransferDirection aDirection, TUint aBytes, TBool aResetTimer);
	virtual void NotifyDataSent(TSubConnectionUniqueId aSubConnectionUniqueId, TUint aUplinkVolume);
	virtual void NotifyDataReceived(TSubConnectionUniqueId aSubConnectionUniqueId, TUint aDownlinkVolume);
	virtual void NifEvent(TNetworkAdaptorEventType aEventType, TUint aEvent, const TDesC8& aEventData, TAny* aSource=0);



protected:
	virtual TInt DoReadInt(const TDesC& aField, TUint32& aValue,const RMessagePtr2* aMessage);
    virtual TInt DoWriteInt(const TDesC& aField, TUint32 aValue,const RMessagePtr2* aMessage);
    virtual TInt DoReadDes(const TDesC& aField, TDes8& aValue,const RMessagePtr2* aMessage);
    virtual TInt DoReadDes(const TDesC& aField, TDes16& aValue,const RMessagePtr2* aMessage);
    virtual TInt DoWriteDes(const TDesC& aField, const TDesC8& aValue,const RMessagePtr2* aMessage);
	virtual TInt DoWriteDes(const TDesC& aField, const TDesC16& aValue,const RMessagePtr2* aMessage);
	virtual TInt DoReadBool(const TDesC& aField, TBool& aValue,const RMessagePtr2* aMessage);
    virtual TInt DoWriteBool(const TDesC& aField, TBool aValue,const RMessagePtr2* aMessage);


private:
    CBindMan& iBindMan;
    MNifIfNotify* iNotify;
    TContextId iContextId;
    };


#endif /*SPUDNOTIFY_H*/
