// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file
 @internalAll 
*/

#ifndef __FAX_H__
#define __FAX_H__

#include "CALL.H"
#include "ETELFAX.H"

//
// CFaxHayes
//

class CFaxHayes : public CFaxBase
/**
@internalComponent
*/
	{
public:
	enum TDataDirection
		{
		ESendData,
		EReceiveData,
		EUnknown
		};
	static CFaxHayes* NewL(CCallMobileFax* aCallFaxHayes,CPhoneGlobals* aPhoneGlobals);
	CFaxHayes(CCallMobileFax* aCallFaxHayes,CPhoneGlobals* aPhoneGlobals);
	void ConstructL();
	~CFaxHayes();
	virtual void Init();
	static void CloseFax(TAny* aObj);
public: 
	virtual CTelObject::TReqMode ReqModeL(const TInt aIpc);
	virtual TInt RegisterNotification(const TInt aIpc);
	virtual TInt DeregisterNotification(const TInt aIpc);
    virtual TInt Read(const TTsyReqHandle aTsyReqHandle,TDes8* aFaxPage);
    virtual TInt Write(const TTsyReqHandle aTsyReqHandle,TDesC8* aFaxPage);
	virtual TInt WaitForEndOfPage(const TTsyReqHandle aTsyReqHandle);
	virtual TInt TerminateFaxSession(const TTsyReqHandle aTsyReqHandle);
	virtual TInt CheckAndSetRegistrationParams(const TInt aIpc,const TDes8* aDes1,const TDes8* aDes2);
	RHandleBase* GlobalKernelObjectHandle();
	

private:
	CPhoneGlobals* iPhoneGlobals;
	CCallMobileFax* iOwner;
	TDataDirection iDataDirection;
	};

#endif
