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
// QoS change notifier implementation
// 
//

/**
 @file 
 @internalComponent
*/

#include "cqoschangenotifier.h"
#include "spudteldebuglogger.h"
#include <pcktcs.h>


/** c'tor 

@param aId - pdp context id
@param aPacketQoS - etel packet QoS 
@param aPdpFsmInterface - pdp fsm interface
*/
CQoSChangeNotifier::CQoSChangeNotifier(TContextId aId, 
									RPacketQoS& aPacketQoS, 
									CPdpFsmInterface& aPdpFsmInterface)
: CEtelDriverNotifier(aPdpFsmInterface),
  iId(aId),
  iPacketQoS(aPacketQoS),
  iProfilePckg(iProfileBuffer)
  
	{
	SPUDTEL_FNLOG("CQoSChangeNotifier::CQoSChangeNotifier()");
	}
	
CQoSChangeNotifier::~CQoSChangeNotifier()
	{
	SPUDTEL_FNLOG("CQoSChangeNotifier::~CQoSChangeNotifier()");
	Cancel();
	}
	
/** starts notification */
void CQoSChangeNotifier::Start()
	{
	SPUDTEL_FNLOG("CQoSChangeNotifier::Start()");
	if (!IsActive())
		{ 
		iPacketQoS.NotifyProfileChanged(iStatus, iProfilePckg); 
		SetActive();
		SPUDTEL_FNLOG("CQoSChangeNotifier::Start(), iPacketQoS.NotifyProfileChanged()");
		}
	}
	
/** stops notification */	
void CQoSChangeNotifier::DoCancel()
	{
	if (IsActive())
		{ 
		SPUDTELVERBOSE_INFO_LOG(
			_L("CQoSChangeNotifier::DoCancel EPacketQoSNotifyProfileChanged"));
		iPacketQoS.CancelAsyncRequest(EPacketQoSNotifyProfileChanged);
		}
	}

/** notifies pdp fsm 

@param aStatus - request status
*/
void CQoSChangeNotifier::Notify(const TRequestStatus& aStatus)
	{
	if(aStatus == KErrNone)
		{
		SPUDTELVERBOSE_INFO_LOG(_L("FSM set QoSProfile"));

#ifdef SYMBIAN_NETWORKING_UMTSR5
        iPdpFsmInterface.Set(iId, iProfileBuffer.NegotiatedQoSR5());

#else
// !SYMBIAN_NETWORKING_UMTSR5

        iPdpFsmInterface.Set(iId, iProfileBuffer.NegotiatedQoSR99_R4());
#endif
// SYMBIAN_NETWORKING_UMTSR5

		SPUDTELVERBOSE_INFO_LOG(_L("FSM input EQoSProfileChangeNetwork"));
		iPdpFsmInterface.Input(iId, PdpFsm::EQoSProfileChangeNetwork);
		}
	else
		{ 
		SPUDTEL_ERROR_LOG(_L("CQoSChangeNotifier::Notify(), error: %d"), aStatus.Int());
		ASSERT(aStatus == KErrCancel); 
		}
	}



