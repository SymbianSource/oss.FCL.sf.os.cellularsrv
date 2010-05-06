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
// PDP status change notifier
// 
//

/**
 @file 
 @internalComponent
*/

#include <e32base.h>

#include "cpdpstatuschangenotifier.h"
#include "PDPFSM.h"
#include "spudteldebuglogger.h"
#include "pdpfsmnmspace.h"

#include <pcktcs.h>


/**
@param aId - pdp context id
@param aPacketContext - reference to etel packet context
@param aPdpFsmInterface - reference to pdp fsm interface
*/
CPdpStatusChangeNotifier::CPdpStatusChangeNotifier(TContextId aId,
										RPacketContext& aPacketContext,
										CPdpFsmInterface& aPdpFsmInterface)
: CEtelDriverNotifier(aPdpFsmInterface),
  iId(aId),
  iPacketContext(aPacketContext),
  iContextStatus(RPacketContext::EStatusUnknown)
	{
	}

CPdpStatusChangeNotifier::~CPdpStatusChangeNotifier()
	{
	Cancel();
	}

/** initiates notification */
void CPdpStatusChangeNotifier::Start()
	{
	if (!IsActive() && (iContextStatus != RPacketContext::EStatusDeleted))
		{ 
		iPacketContext.NotifyStatusChange(iStatus, iContextStatus); 
		SetActive();
		}
	}

/** stops notification */	
void CPdpStatusChangeNotifier::DoCancel()
	{
	if(IsActive())
		{ 
		SPUDTELVERBOSE_INFO_LOG(
			_L("CPdpStatusChangeNotifier::DoCancel EPacketContextNotifyStatusChange"));
		iPacketContext.CancelAsyncRequest(EPacketContextNotifyStatusChange); 
		}
	}

/** notifies pdp fsm 

@param aStatus - request status
*/
void CPdpStatusChangeNotifier::Notify(const TRequestStatus& aStatus)
	{
	if(aStatus == KErrNone)
		{
		SPUDTELVERBOSE_INFO_LOG(_L("Notified of context status change"));
		RPacketContext::TContextStatus aOldContextStatus;
		iPdpFsmInterface.Get(iId,aOldContextStatus);
		// only notify of change if it has actually changed
		if (iContextStatus != aOldContextStatus)
			{
			SPUDTELVERBOSE_INFO_LOG(_L("FSM input EContextStatusChangeNetwork"));
			iPdpFsmInterface.Set(iId, iContextStatus);
            TInt err = KErrNone;
            iPacketContext.GetLastErrorCause(err); // Ignore error return code.
			iPdpFsmInterface.Input(iId,  PdpFsm::EContextStatusChangeNetwork, err);
			}
		}
	else
		{ 
		SPUDTEL_ERROR_LOG(_L("CPdpStatusChangeNotifier::Notify(), error: %d"), 
						aStatus.Int());
		ASSERT(aStatus == KErrCancel); 
		}
	}
	
	
	
	
	
/**
@param aId - pdp context id
@param aPacketContext - reference to etel packet context
@param aPdpFsmInterface - reference to pdp fsm interface
*/
CMbmsPdpStatusChangeNotifier::CMbmsPdpStatusChangeNotifier(TContextId aId,
										RPacketMbmsContext& aMbmsPacketContext,
										CPdpFsmInterface& aPdpFsmInterface)
: CEtelDriverNotifier(aPdpFsmInterface),
  iId(aId),
  iMbmsPacketContext(aMbmsPacketContext),
  iContextStatus(RPacketContext::EStatusUnknown)
	{
	}

CMbmsPdpStatusChangeNotifier::~CMbmsPdpStatusChangeNotifier()
	{
	Cancel();
	}

/** initiates notification */
void CMbmsPdpStatusChangeNotifier::Start()
	{
	if (!IsActive() && (iContextStatus != RPacketContext::EStatusDeleted))
		{ 
		iMbmsPacketContext.NotifyStatusChange(iStatus, iContextStatus); 
		SetActive();
		}
	}

/** stops notification */	
void CMbmsPdpStatusChangeNotifier::DoCancel()
	{
	if(IsActive())
		{ 
		SPUDTELVERBOSE_INFO_LOG(
			_L("CMbmsPdpStatusChangeNotifier::DoCancel EPacketContextNotifyStatusChange"));
		iMbmsPacketContext.CancelAsyncRequest(EPacketContextNotifyStatusChange); 
		}
	}

/** notifies pdp fsm 

@param aStatus - request status
*/
void CMbmsPdpStatusChangeNotifier::Notify(const TRequestStatus& aStatus)
	{
	if(aStatus == KErrNone)
		{
		SPUDTELVERBOSE_INFO_LOG(_L("Notified of context status change"));
		RPacketContext::TContextStatus aOldContextStatus;
		iPdpFsmInterface.Get(iId,aOldContextStatus);
		// only notify of change if it has actually changed
		if (iContextStatus != aOldContextStatus)
			{
			SPUDTELVERBOSE_INFO_LOG(_L("FSM input EContextStatusChangeNetwork"));
			iPdpFsmInterface.Set(iId, iContextStatus);
            TInt err = KErrNone;
            iMbmsPacketContext.GetLastErrorCause(err); // Ignore error return code.
			iPdpFsmInterface.Input(iId, PdpFsm::EContextStatusChangeNetwork, err);
			}
		}
	else
		{ 
		SPUDTEL_ERROR_LOG(_L("CMbmsPdpStatusChangeNotifier::Notify(), error: %d"), 
						aStatus.Int());
		ASSERT(aStatus == KErrCancel); 
		}
	}
	
