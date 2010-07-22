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
// Inline methods of the Umts/Gprs SubConnection Provider
// 
//

/**
 @file
 @internalComponent
*/



inline CUmtsGprsSubConnProvd::CUmtsGprsSubConnProvd(
                              CSubConnectionProviderFactoryBase& aFactory, 
	                          CConnectionProviderBase& aConnProvider)
:CSubConnectionProviderBase(aFactory, aConnProvider),
 iContextId(KAllContexts), /*Initialising to an illegal value*/
 iCurrentPdpFsmReq(NULL)			
/** 
Constructs a UMTS SubConnection Provider

@param aFactory CSubConnectionProviderFactoryBase that should own the reference to 'this'
@param aConnProvider a connection that 'this' subconnection belongs to
*/
	{
	__FLOG_OPEN(KUmtsGprsLogTag1, KUmtsGprsLogTag2);
	__FLOG_2(_L("CUmtsGprsSubConnProvd [this=%08x]:\tCUmtsGprsSubConnProvd() [MConnectionDataClient=%08x]"),this,(MConnectionDataClient*)this);
	}

inline void CUmtsGprsSubConnProvd::Done(TInt /*aResult*/, CPdpFsmRequestBase& aThis)
    {
    ASSERT(&aThis == iCurrentPdpFsmReq);
    (void)aThis;
    delete iCurrentPdpFsmReq;
    iCurrentPdpFsmReq = NULL;
    }
    
inline TContextId CUmtsGprsSubConnProvd::GetContextId() const
    {
    return iContextId;
    }

inline TInt CUmtsGprsSubConnProvd::NotifyClientEvent(const CSubConNotificationEvent& aEvent)
    {
    return CSubConnectionProviderBase::NotifyClientEvent(aEvent);
    }


inline CUmtsGprsSubConnProvd::TQosParameterRelease CUmtsGprsSubConnProvd::RequestedQosRelease()
    {
    return iParameterRelease;
    }

inline void CUmtsGprsSubConnProvd::ResetQosRelease ()
    {
    iParameterRelease = CUmtsGprsSubConnProvd::KParameterRelInvalid;
    }


inline CPdpFsmRequestBase::~CPdpFsmRequestBase ()
   {
	__FLOG_CLOSE;
   }
