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
// PDP config change notifier
// 
//

/**
 @file 
 @internalComponent
*/

#include <e32base.h>
#include <pcktcs.h>

#include "cpdpconfigchangenotifier.h"
#include "PDPFSM.h"
#include "spudteldebuglogger.h"
#include "pdpfsmnmspace.h"


/**
@param aId - pdp context id
@param aPacketContext - reference to etel packet context
@param aPdpFsmInterface - reference to pdp fsm interface
*/
CPdpConfigChangeNotifier::CPdpConfigChangeNotifier(TContextId aId,
										RPacketContext& aPacketContext,
										CPdpFsmInterface& aPdpFsmInterface)
: CEtelDriverNotifier(aPdpFsmInterface),
  iId(aId),
  iPacketContext(aPacketContext),
  iConfigGPRS(),
  iConfigGPRSPckg(iConfigGPRS)
	{
	}

CPdpConfigChangeNotifier::~CPdpConfigChangeNotifier()
	{
	Cancel();
	}

/** initiate notification */
void CPdpConfigChangeNotifier::Start()
	{
	if (!IsActive())
		{ 
		iPacketContext.NotifyConfigChanged(iStatus, iConfigGPRSPckg); 
		SetActive();
		}
	}

/** stops notification */	
void CPdpConfigChangeNotifier::DoCancel()
	{
	if(IsActive())
		{ 
		SPUDTELVERBOSE_INFO_LOG(
			_L("CPdpConfigChangeNotifier::DoCancel EPacketContextNotifyConfigChanged"));
		iPacketContext.CancelAsyncRequest(EPacketContextNotifyConfigChanged); 
		}
	}

/** notifies pdp fsm 

@param aStatus - request status
*/
void CPdpConfigChangeNotifier::Notify(const TRequestStatus& aStatus)
	{
	if(aStatus == KErrNone)
		{
		SPUDTELVERBOSE_INFO_LOG(_L("FSM set ConfigGPRS"));
		iPdpFsmInterface.Set(iId, iConfigGPRS);
		SPUDTELVERBOSE_INFO_LOG(_L("FSM input EConfigGPRSChangeNetwork"));
		iPdpFsmInterface.Input(iId, PdpFsm::EConfigGPRSChangeNetwork);
		}
	else
		{ 
		SPUDTEL_ERROR_LOG(_L("CPdpConfigChangeNotifier::Notify(), error: %d"), 
						aStatus.Int());
		// Not all TSYs support RPacketContext::NotifyConfigChanged so we handle
		// KErrNotSupported silently (PDEF118981).
		ASSERT((aStatus == KErrCancel) || (aStatus == KErrNotSupported));
		}
	}
