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
// implementation for service change notifier
// 
//

/**
 @file 
 @internalComponent
*/

#include <e32def.h>

#include "cservicechangenotifier.h"
#include "PDPFSM.h"
#include "spudteldebuglogger.h"
#include "pdpfsmnmspace.h"

#include <pcktcs.h>

/** c'tor

@param aPacketService - reference to packet service
@param aPdpFsmInterface - reference to pdp fsm interface
*/
CServiceChangeNotifier::CServiceChangeNotifier(RPacketService& aPacketService,
										CPdpFsmInterface& aPdpFsmInterface)
: CEtelDriverNotifier(aPdpFsmInterface),
  iPacketService(aPacketService),
  iServiceStatus(RPacketService::EStatusUnattached)
	{
	}

CServiceChangeNotifier::~CServiceChangeNotifier()
	{
	Cancel();
	}

/** initiates notification */
void CServiceChangeNotifier::Start()
	{
	if (!IsActive())
		{ 
		iPacketService.NotifyStatusChange(iStatus, iServiceStatus); 
		SetActive();
		}
	}

/** notifies pdp fsm 

@param aStatus - request status
*/
void CServiceChangeNotifier::Notify(const TRequestStatus& aStatus)
	{
	if(aStatus == KErrNone)
		{
		SPUDTELVERBOSE_INFO_LOG(_L("FSM set ServiceStatus"));
		iPdpFsmInterface.Set(iServiceStatus);
		SPUDTELVERBOSE_INFO_LOG(_L("FSM input EServiceStatusChangeNetwork"));
		iPdpFsmInterface.Input(KAllContexts, 
								PdpFsm::EServiceStatusChangeNetwork);
		}
	else
		{ 
		SPUDTEL_ERROR_LOG(_L("CServiceChangeNotifier::Notify(), error: %d"), 
							aStatus.Int());
		ASSERT(aStatus == KErrCancel); 
		}
	}

/** cancels notification */	
void CServiceChangeNotifier::DoCancel()
	{
	if(IsActive())
		{ 
		SPUDTELVERBOSE_INFO_LOG(
			_L("CServiceChangeNotifier::DoCancel EPacketNotifyStatusChange"));
		iPacketService.CancelAsyncRequest(EPacketNotifyStatusChange); 
		}
	}
