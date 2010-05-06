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
// SPUD data multiplexer
// 
//

/**
 @file
 @internalComponent
*/

#include "mux.h"
#include <nifmbuf.h>
#include <in6_if.h>

// Flow control constants expected by the upper protocol stack
const TInt KStopSending		= 0;
const TInt KContinueSending	= 1;

#ifdef __FLOG_ACTIVE
#define SPUDMUX_LOG(x) iBindMan->SpudMan()->x
#else
#define SPUDMUX_LOG(x)
#endif


//
// CSpudMux
//


CSpudMux::CSpudMux(CSpudMan& aNifBase)
	: CNifIfBase(aNifBase)
    {
	}

CSpudMux::~CSpudMux()
    {
    // Notify BindMan to delete the pointer to this object
    if (iBindMan)
    	{
	    iBindMan->MuxClosed();
    	}
	}

/**
Constructs the mux object

@param aBindMan Reference to BindMan object
*/
void CSpudMux::Construct(CBindMan& aBindMan)
	{
	iBindMan = &aBindMan;
	
	// Create a unique interface name
	iIfName.Format(_L("%S[0x%08x]"), &KSpudName, this);
	}

/**
Binds SPUD to the TCP/IP stack.

@param aId Pointer to network stack object (CProtocolBase*)
@leave KErrInUse if SPUD is already bound
*/
void CSpudMux::BindL(TAny* aId)
	{
	iBindMan->SetProtocolBaseL(static_cast<CProtocolBase*>(aId));
	}


/**
Fills in the info object with NIF information.

@param aInfo object to hold information on return
*/
void CSpudMux::Info(TNifIfInfo& aInfo) const
	{
	// Get the binder for the first (default) lower NIF.
	CSpudBinderRef* ref = NULL;
	TRAPD(err, ref = iBindMan->GetAnyRefL());
	if (err == KErrNone)
		{
		// Read the protocol supported value from the lower NIF
		ref->NifBase()->Info(aInfo);
		ASSERT((aInfo.iFlags & (KNifIfIsBase | KNifIfUsesNotify | KNifIfCreatedByLink)) == (KNifIfIsBase | KNifIfUsesNotify | KNifIfCreatedByLink));
		}
	else
		{
		aInfo.iProtocolSupported=KProtocolUnknown;
		}

	// Overwrite these values with our own
	aInfo.iName.Copy(iIfName);
	aInfo.iVersion = TVersion(KSpudMajorVersionNumber, KSpudMinorVersionNumber, KSpudBuildVersionNumber);
	aInfo.iFlags = KNifIfIsBase | KNifIfUsesNotify | KNifIfCreatedByLink;
	}

/**
Takes packets from the IP stack and demultiplexes them by sending each on to
the appropriate lower NIF for processing. The packets are assumed to be
ordered correctly, as appropriate for the QoS guarantees given to each.
This function's responsibility is to pass each packet to the appropriate
lower NIF for processing, where each lower NIF handles a single PDP context.

@param aPacket MBuf chain containing packet
@param aSource Passed unchanged to lower NIF
@return 1 for a successful send, 0 to tell upper layer to flow off
*/
TInt CSpudMux::Send(RMBufChain& aPacket, TAny* aSource)
	{
	// GUQoS places the context ID for the outgoing packet into the Port field
	// in the RMBufChain info header.
	const RMBufPktInfo* info = RMBufPacket::PeekInfoInChain(aPacket);
	TUint contextId(info->iDstAddr.Port());
	if (contextId >= static_cast<TUint>(KMaxPdpContexts))
		{
		// Context ID is illegal; use 0 instead and try to continue
		SPUDMUX_LOG(__FLOG_1(_L("CSpudMux::Send context id %d is out of range; using 0 instead"), contextId));
		contextId = 0;
		}
	const TContextId id(static_cast<TContextId>(contextId));

	// Get the binder for the appropriate lower NIF.
	CSpudBinderRef* ref = NULL;
	TRAPD(err, ref = iBindMan->GetRefL(id));
	if (err != KErrNone)
		{
		// That's odd--GUQoS is sending to an invalid context.
		// Redirect it to the first valid context instead.
		SPUDMUX_LOG(__FLOG_1(_L("CSpudMux::Send context id %d is invalid; searching for another"), id));
		TRAPD(err, ref = iBindMan->GetAnyRefL());
		if (err != KErrNone)
			{
			// Something is really wrong here! No contexts available at all!
			SPUDMUX_LOG(__FLOG_0(_L("Nowhere to send data! Dropping packet")));
			return err;
			}
		}
	TInt rc = ref->NifBase()->Send(aPacket, aSource);

	// See if NIF is flowing off data on this context
	if ((rc == KStopSending) && iBindMan->SpudMan()->AreQoSEventsEnabled())
		{
		// Send flow off indication via GUQoS instead
		iBindMan->SpudMan()->StopSending(id);
		rc = KContinueSending;	// Successful send indication
		}
	return rc;
	}

/**
Processes notifications from agent.

@param aEvent event type
@param aInfo data associated with event
@return error code
*/
TInt CSpudMux::Notification(TAgentToNifEventType aEvent, void* aInfo)
	{
	SPUDMUX_LOG(__FLOG_1(_L("CSpudMux::Notification received event %d"), aEvent));
	TInt rc = KErrNotSupported;
	switch (aEvent)
		{
	case EAgentToNifEventTypeModifyInitialTimer:
	case EAgentToNifEventTypeDisableTimers:
	case EAgentToNifEventTypeEnableTimers:
		// Send notification to all lower NIFs
		rc =  KErrNotReady;
		TContextId i;
		for (i=0; i < KMaxPdpContexts; ++i)
			{
			CSpudBinderRef* ref = NULL;
			TRAP(rc, ref = iBindMan->GetRefL(i));
			if (rc == KErrNone)
				{
				rc = ref->NifBase()->Notification(aEvent, aInfo);
				}
			}
		break;

	case EAgentToNifEventTypeDisableConnection:
		SPUDMUX_LOG(__FLOG_0(_L("CSpudMux::Notification received EAgentToNifEventTypeDisableConnection")));
		// TODO: How to handle this event?  Just fall through and ignore for now.
	default:
		break;
		}

	return rc;
	}

/**
Returns the current state of the interface

@return TIfStatus indicating the current state of the interface
*/
TInt CSpudMux::State()
	{
	// Get the binder for the first (default) lower NIF.
	CSpudBinderRef* ref = NULL;
	TRAPD(err, ref = iBindMan->GetAnyRefL());
	if (err != KErrNone)
		{
		return EIfDown;
		}
	
	// Use the state of the first lower NIF as the state of the SPUD
	return ref->NifBase()->State();
	}

/**
Controls the NIF

@param aLevel The intended level for this control option
@param aName The name of the control option
@param aOption Any data associated with this control option, contained within a TPckg(Buf)
@param aSource If provided, an identifier for the source of the control option; by default, zero
@return KErrNone if successful; otherwise one of the standard Symbian OS error codes
*/
TInt CSpudMux::Control(TUint aLevel, TUint aName, TDes8& aOption, TAny* aSource)
	{
	if (aLevel==KSOLInterface)
		{
		switch (aName)
			{
		// From elsewhere
		case KSoIfInfo:
		case KSoIfInfo6:
		case KSoIfConfig:
		case KSoIfCompareAddr:
		case KSoIfGetConnectionInfo:
		case KSoIfHardwareAddr:
			{
			// Get the binder for the first (default) lower NIF.
			CSpudBinderRef* ref = NULL;
			TRAPD(err, ref = iBindMan->GetAnyRefL());
			if (err != KErrNone)
				{
				return err;
				}
			return ref->NifBase()->Control(aLevel, aName, aOption, aSource);
			}

		default:
			// Unknown event
			// Assume it's for GUQoS, so let SpudMan handle it and error out the unknown ones
			// These include:
			//   KSoIfControllerPlugIn
        	//   KRegisterEventHandler
			//   KContextSetEvents
			//   KNifSetDefaultQoS
			//   KContextCreate
			//   KContextDelete
			//   KContextActivate
			//   KContextQoSSet
			//   KContextTFTModify
			//   KContextModifyActive
			return iBindMan->SpudMan()->GuqosInput(aName, aOption);
			}
		}
	return KErrNotSupported;
	}

//
// CSpudProtocol
//


CSpudProtocol::CSpudProtocol()
	{
	}

CSpudProtocol::~CSpudProtocol()
	{
	}

/**
Constructs the SpudProtocol object

@param aBindMan reference to BindMan object
*/
void CSpudProtocol::Construct(CBindMan& aBindMan)
	{
	iBindMan = &aBindMan;
	}

void CSpudProtocol::SetProtocolBaseL(CProtocolBase* aProtocolBase)
	{
	if(iProtocolBase)
		{
		SPUDMUX_LOG(__FLOG_0(_L("CSpudProtocol::SetProtocolBaseL already bound to protocol")));
		User::Leave(KErrInUse);
		}

	iProtocolBase = aProtocolBase;
	ASSERT(iProtocolBase);
	}


void CSpudProtocol::Identify(TServerProtocolDesc *aDesc) const
	{
	ASSERT(iProtocolBase);
    iProtocolBase->Identify(aDesc);
	}

/**
Receives an indication that the lower NIF is ready to send packets.

@param aProtocol CNifIfBase pointer of lower NIF
*/
void CSpudProtocol::StartSending(CProtocolBase* aProtocol)
	{
	TContextId id = KAllContexts;
	TRAPD(rc, id = iBindMan->FindContextIdL(reinterpret_cast<CNifIfBase*>(aProtocol)));
	__ASSERT_ALWAYS(rc == KErrNone, iBindMan->SpudMan()->Panic());
	iBindMan->SpudMan()->StartSending(id);
	}

/**
Receives PDU from the lower NIF and passes it to the upper protocol layer.

@param aChain Datagram to process
@param aSourceProtocol CNifIfBase pointer of lower NIF (ignored)
*/
void CSpudProtocol::Process(RMBufChain& aChain, CProtocolBase* /*aSourceProtocol*/)
	{
	ASSERT(iProtocolBase);
    iProtocolBase->Process(aChain, reinterpret_cast<CProtocolBase*>(iBindMan->SpudMux()));
	}

/**
Receives PDU from the lower NIF and passes it to the upper protocol layer.

@param aPDU Datagram to process
@param aFrom Source address
@param aTo Destination address
@param aSourceProtocol (ignored)
*/
void CSpudProtocol::Process(TDes8& aPDU, TSockAddr* aFrom, TSockAddr* aTo, CProtocolBase* /*aSourceProtocol*/)
	{
	ASSERT(iProtocolBase);
    iProtocolBase->Process(aPDU, aFrom, aTo, reinterpret_cast<CProtocolBase*>(iBindMan->SpudMux()));
	}

/**
Propagates error conditions up the stack, eventually to socket service providers.

@param aError The error code 
@param aSourceProtocol (ignored)
*/
void CSpudProtocol::Error(TInt aError, CProtocolBase* /*aSourceProtocol*/)
	{
	ASSERT(iProtocolBase);
    iProtocolBase->Error(aError, reinterpret_cast<CProtocolBase*>(iBindMan->SpudMux()));
	}

/**
Calls StartSending on the upper network protocol.
*/
void CSpudProtocol::DoStartSending() const
	{
	ASSERT(iProtocolBase);
	iProtocolBase->StartSending(reinterpret_cast<CProtocolBase*>(iBindMan->SpudMux()));
	}

