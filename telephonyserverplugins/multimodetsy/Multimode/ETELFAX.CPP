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

#include "ETELFAX.H"
#include "mSLOGGER.H"
#include "CALL.H"
#include "PHONE.H"
#include "NOTIFY.H"


CFaxCompletion::CFaxCompletion() 
	: CBase(), iTelObject(NULL), iCall(NULL)
	{}

CFaxCompletion::~CFaxCompletion()
	{}

void CFaxCompletion::RxConnectComplete(TInt aError)
	{
	LOGTEXT(_L8("ETelFaxBase:\tReceive Connect Complete"));
	iCall->ChangeLineStatus(RCall::EStatusConnected);
	__ASSERT_ALWAYS(iCall->ChangeCallStatus(RMobileCall::EStatusConnected) == KErrNone,Panic(EGeneral));
	iTelObject->ReqCompleted(iTsyReqHandle,aError);
	}

void CFaxCompletion::RxFaxDataComplete(TInt aError)
	{
	LOGTEXT(_L8("ETelFaxBase:\tReceive Fax Data Complete"));
	iCall->iPhoneGlobals->iNotificationStore->CheckNotification(iCall->iFaxSession,EFaxReadOrWriteCompleted,aError);
	}

void CFaxCompletion::RxPostPageComplete(TInt aError)
	{
	LOGTEXT(_L8("ETelFaxBase:\tReceive Post Page Complete"));
	iCall->iPhoneGlobals->iNotificationStore->CheckNotification(iCall->iFaxSession,EEndOfFaxPageCompleted,aError);
	}

void CFaxCompletion::TxConnectComplete(TInt aError)
	{
	LOGTEXT2(_L8("ETelFaxBase:\tTransmission Connect Complete with %d"),aError);
	iCall->ChangeLineStatus(RCall::EStatusConnected);		
	__ASSERT_ALWAYS(iCall->ChangeCallStatus(RMobileCall::EStatusConnected) == KErrNone,Panic(EGeneral));
	iTelObject->ReqCompleted(iTsyReqHandle,aError);
	}

void CFaxCompletion::TxFaxDataComplete(TInt aError)
	{
	LOGTEXT(_L8("ETelFaxBase:\tTransmission Fax Data Complete"));
	iCall->iPhoneGlobals->iNotificationStore->CheckNotification(iCall->iFaxSession,EFaxReadOrWriteCompleted,aError);
	}

void CFaxCompletion::TxPostPageComplete(TInt aError)
	{
	LOGTEXT(_L8("ETelFaxBase:\tTransmission Post Page Complete"));
	iCall->iPhoneGlobals->iNotificationStore->CheckNotification(iCall->iFaxSession,EEndOfFaxPageCompleted,aError);
	}

void CFaxCompletion::GetCadenceAndTimeOfLastRing(TTimeIntervalMicroSeconds& aCadence,TTime& aTime)
	{
	aCadence=0;
	aTime=0;
	}

void CFaxCompletion::Configure(TTsyReqHandle aTsyReqHandle,CTelObject* aTelObject)
	{
	iTsyReqHandle = aTsyReqHandle;
	iTelObject = aTelObject;
	iCall = REINTERPRET_CAST(CCallMobileFax*,aTelObject);
	}

CTelObject* CFaxCompletion::TelObject() const
	{
	return iTelObject;
	}
