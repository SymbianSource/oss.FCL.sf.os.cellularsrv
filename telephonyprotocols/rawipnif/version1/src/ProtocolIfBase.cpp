// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file implements the base class for the protocol interface classes.
// 
//

/**
 @file
*/

#include <nifmbuf.h>
#include "ProtocolIfBase.h"
#include "RawIPNifMain.h"

CProtocolIfBase::CProtocolIfBase(CRawIPNifMain& aNifMain, CBttLogger* aTheLogger)
/**
 * Constructor
 *
 * @param aNifMain A pointer to CRawIPNifMain
 */
	:iNifMain(aNifMain),
	 iTheLogger(aTheLogger)
	{
	}

CProtocolIfBase::~CProtocolIfBase()
/**
 * Destructor
 */
	{
	}

void CProtocolIfBase::BindL(TAny *aId)
/**
 * Binds TCP/IP protocol to Nif
 *
 * @param aId A pointer to protocol object 
 */

	{
	_LOG_L1C2(_L8("CProtocolIfBase::BindL [aId=%X]"), aId);

	if (iProtocol)
		_BTT_PANIC(KNifName, KNifProtocolInUse);
	iProtocol = reinterpret_cast<CProtocolBase*>(aId);
	}

TInt CProtocolIfBase::State()
/**
 * Used to retrieve the Nif state
 *
 * @return The Nif state
 */
	{
	_LOG_L1C1(_L8("CProtocolIfBase::State"));

	return GetNifMain().State();
	}

TInt CProtocolIfBase::Control(TUint aLevel, TUint aName, TDes8& aOption, TAny* aSource)
/**
 * This method will call the control method in IPv4Protocol
 *
 */
	{
	_LOG_L1C1(_L8("CProtocolIfBase::Control"));

	return GetNifMain().Control(aLevel, aName, aOption, aSource);
	}

TInt CProtocolIfBase::Send(RMBufChain& aPdu, TAny* aSource)
/**
 * Send and IP packet. This method is overriden by the IPv4ProtocolIf class.
 *
 * @param aPdu The outgoing packet
 * @param aSource The source of the packet
 * @return Standard error codes
 */
	{
	_LOG_L1C1(_L8("CProtocolIfBase::Send"));

	return GetNifMain().Send(aPdu, aSource);
	}

void CProtocolIfBase::Info(TNifIfInfo& aInfo) const
/**
 * Used to retrieved Nif information
 *
 * @param aInfo Nif Information structure
 */
	{
	_LOG_L1C1(_L8("CProtocolIfBase::Info"));

	aInfo.iVersion = TVersion(KNifMajorVersionNumber, KNifMinorVersionNumber,
		KNifBuildVersionNumber);
	aInfo.iFlags = KNifIfIsBase | KNifIfUsesNotify | KNifIfCreatedByLink;
	// Doesn't fill in aInfo.iName, as this is done by derived classes.
	aInfo.iProtocolSupported = KProtocolInetIp;
	}

TInt CProtocolIfBase::Notification(TAgentToNifEventType /*aEvent*/, void* /*aInfo*/)
/**
 * Method overidden by the notification method on IPv4ProtocolIf class.
 *
 * @param aEvent Not used
 * @param aInfo Not used 
 */
	{
	_LOG_L1C1(_L8("CProtocolIfBase::Notification"));

	return KErrNone;
	}

void CProtocolIfBase::StartSending(CProtocolBase* /*aProtocol*/)
/**
 * Indicates to the protocol layer that the NIF is ready to send packets.
 *
 * @param aProtocol A pointer to a protocol
 */
	{
	_LOG_L1C1(_L8("CProtocolIfBase::StartSending"));

	// Default implementation.
	// Uses iProtocol instead aProtocol.
	iProtocol->StartSending(reinterpret_cast<CProtocolBase*>(this));
	}


void CProtocolIfBase::UpdateContextConfigL(const
	RPacketContext::TContextConfigGPRS& /*aConfig*/)
/**
 * Method overidden by UpdateContextConfig on IPv4ProtocolIf class
 *
 * @param aConfig Not used
 */
	{
	// Default implementation does nothing.
	}

void CProtocolIfBase::UpdateConnectionSpeed(TUint /*aConnectionSpeed*/)
/**
 * Method overidden by UpdateConnectionSpeed on IPv4ProtocolIf class
 *
 * @param aConnectionSpeed Not used
 */
	{
	// Default implementation does nothing.
	}
