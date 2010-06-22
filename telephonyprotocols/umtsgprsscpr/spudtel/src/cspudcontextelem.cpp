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
// spud context element
// 
//

/**
 @file 
 @internalComponent
*/

#include "cspudcontextelem.h"

/**
@param aId - pdp context id
@param aFactory - reference to etel driver factory
@param aPdpFsmInterface reference to pdp fsm interface
@return new spud context element
*/
CSpudContextElem* CSpudContextElem::NewL(TContextId aId, 
									CEtelDriverFactory& aFactory, 
									CPdpFsmInterface& aPdpFsmInterface)
	{
	CSpudContextElem *me = new(ELeave) CSpudContextElem();
	CleanupStack::PushL(me);
	me->ConstructL(aId, aFactory, aPdpFsmInterface);
	CleanupStack::Pop(me);
	return me;
	}

/** 
@param aId - pdp context id
@param aFactory - reference to etel driver factory
@param aPdpFsmInterface reference to pdp fsm interface
*/
void CSpudContextElem::ConstructL(TContextId aId, 
								CEtelDriverFactory& aFactory, 
								CPdpFsmInterface& aPdpFsmInterface)
	{
	iContext = new(ELeave) CEtelDriverContext(aId, aFactory);
	iPdpStatusChangeNotifier = new(ELeave) CPdpStatusChangeNotifier(aId, 
													iContext->PacketContext(), 
													aPdpFsmInterface);
	iQoSChangeNotifier = new(ELeave) CQoSChangeNotifier(aId, 
													iContext->PacketQoS(), 
													aPdpFsmInterface);
	iPdpConfigChangeNotifier = new(ELeave) CPdpConfigChangeNotifier(aId, 
													iContext->PacketContext(), 
													aPdpFsmInterface);
	}

CSpudContextElem::~CSpudContextElem()
	{
	delete iPdpStatusChangeNotifier;
	delete iQoSChangeNotifier;
	delete iPdpConfigChangeNotifier;
	delete iContext;
	}

/** starts all notifiers */
void CSpudContextElem::Start()
	{
	iPdpStatusChangeNotifier->Start();
	iQoSChangeNotifier->Start();
	iPdpConfigChangeNotifier->Start();
	}
	
/** cancels all notifiers */
void CSpudContextElem::Cancel()
	{
	iPdpStatusChangeNotifier->Cancel();
	iQoSChangeNotifier->Cancel();
	iPdpConfigChangeNotifier->Cancel();
	}

/** Get LastErrorCause

@param aLastErrorCause - LastErrorCause to get
*/
void CSpudContextElem::GetLastErrorCause(TInt& aLastErrorCause)
	{
	RPacketContext &aPacketContext = iContext->PacketContext();
	aPacketContext.GetLastErrorCause(aLastErrorCause);
	}

void CSpudContextElem::PdpStatusChangeNotifierCancel()
	{
	iPdpStatusChangeNotifier->Cancel();
	}

void CSpudContextElem::QoSChangeNotifierCancel()
	{
	iQoSChangeNotifier->Cancel();
	}
void CSpudContextElem::PdpConfigChangeNotifierCancel()
	{
	iPdpConfigChangeNotifier->Cancel();
	}
