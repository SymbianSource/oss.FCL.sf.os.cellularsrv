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
// Implements the CRawIPNifMain class, which provides a central point for the NIF
// to communicate with NifMan and the BCA controllers. This class creates an interface
// to the IPv4 protocol when it is required.
// 
//

/**
 @file
*/

#include <f32file.h>
#include <nifman.h>
#include <nifmbuf.h>
#include <in_iface.h>
#include <nifvar.h>
#include "IPv4ProtocolIf.h"
#include "IPv6ProtocolIf.h"
#include "RawIPNifMain.h"

#ifdef __FLOG_ACTIVE
//These variables are used only if flog is active.
_LIT8(KTcpDumpFirstTag,"TcpDump");
static const TUint16 KTcpDumpLinkType = 12;
#endif

CRawIPNifMain::CRawIPNifMain(CNifIfFactory& aFactory, MNifIfNotify* aNotify, CBttLogger* aTheLogger)
/**
 * Constructor.
 *
 * @param aFactory Reference to the factory which created this object.
 * @param aTheLogger The logging object, ownership is passed to this object
 */
	: CNifIfLink(aFactory), 
	  iTheLogger(aTheLogger),
	  iInitError(KErrNone), 
	  iState(EIfDown)
	{
	// To aid debugging.
	iNotify = aNotify;
	}

void CRawIPNifMain::Info(TNifIfInfo& aInfo) const
/**
 * Provides information about the NIF.
 *
 * @param aInfo Information structure to be filled in
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::Info"));

	aInfo.iVersion = TVersion(KNifMajorVersionNumber, KNifMinorVersionNumber,
		KNifBuildVersionNumber);
	aInfo.iFlags = KNifIfIsBase | 
				   KNifIfUsesNotify | 
				   KNifIfIsLink |
	               KNifIfCreatedByFactory |
				   KNifIfCreatesBinder;
	aInfo.iName = KRawIPNifName;
	aInfo.iProtocolSupported = KProtocolUnknown;
	}

void CRawIPNifMain::ConstructL(const TDesC& /*aName*/)
/**
 * Second-phase constructor. Creates the Etel
 * control engine
 *
 * @param aName The name of the NIF (unused)
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::ConstructL"));

	iBcaController = CBcaIoController::NewL(*this, iTheLogger);
	}

CRawIPNifMain::~CRawIPNifMain()
/**
 * Destructor. Cancels and deletes all owned active objects.
 */
	{
	// Note that we don't delete iProtocolIf because it's not owned by us.
	delete iBcaController;
	// This class also deletes the logging object
	delete iTheLogger;
	// Cleanup when stop was not called - harmless as it null's the pointer
	__PACKETLOG_DELETE;
	}

TInt CRawIPNifMain::Control(TUint /*aLevel*/, TUint /*aName*/,
                             TDes8& /*aOption*/, TAny* /*aSource*/)
/**
 * Method to set up NIF parameters. This is unused; instead, the Control()
 * method on the IPv4 interface will be called to configure the NIF.
 *
 * @param aLevel The network level of the parameter to be set (unused)
 * @param aName The name of the parameter to be set (unused)
 * @param aOption The parameter to be written to (unused)
 * @param aSource The source of the method call (unused)
 * @return Always KErrNotSupported
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::Control"));

	return KErrNotSupported;
	}

TInt CRawIPNifMain::State()
/**
 * Returns the current state of the NIF. This will be one of EIfUp or EIfDown.
 *
 * @return The state of the NIF
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::State"));

	return iState;
	}

TInt CRawIPNifMain::Start()
/**
 * Starts the NIF up. This process involves connecting to the BCA.  When startup is
 * complete, LinkLayerUp() will be called.
 *
 * @return Allways KErrNone
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::Start"));

	TRAPD(err1,InitialiseL(MControllerObserver::EStart, KErrNone));
	if (err1)
		{
		return err1;
		}
	
	
	//the name calculation should be done only if logging is enabled
#ifdef __FLOG_ACTIVE
	const TUint KModemNameLen = KCommsDbSvrMaxColumnNameLength + 10;  // need enough for ppp-XXX.txt - e.g. ppp-comm-1.txt, ppp-btcomm-10.txt etc
	TBuf8<KModemNameLen> modemName;
	
	//first add modem name
	_LIT(KModemPortName, "ModemBearer\\PortName");
	_LIT8(KStupidCharacters, "::");
	iNotify->ReadDes(KModemPortName, modemName);
	const TInt pos = modemName.FindC((const TDesC8&)KStupidCharacters);
	if(pos != KErrNotFound)
		{
		modemName.Delete(pos, 2);
		}
		
	//append time stamp
	const TUint KTimeStampLen = 8;
	TBuf8<KTimeStampLen> logFileName;
	
	_LIT8(KTimeFormat, "%08X");
	TUint32 counter = User::FastCounter();
	logFileName.Format(KTimeFormat, counter);
		
	TRAPD(err,__PACKETLOG_NEWL(KTcpDumpFirstTag, logFileName, CPacketLogger::ETcpDump, KTcpDumpLinkType));
	if (err)
		{
		_LOG_L1C1(_L8("Trapped leave from __PACKETLOG_NEWL"));
		}
	
	const TUint KLogTextLen = KModemNameLen+KTimeStampLen+30;
	TBuf8<KLogTextLen> logText;
	_LIT8(KLogTimeText, "TcpDump log file time stamp:");
	_LIT8(KLogModemText, " for modem:");
	logText.Append(KLogTimeText);
	logText.Append(logFileName);
	logText.Append(KLogModemText);
	logText.Append(modemName);
	_LOG_L1C1(logText);
#endif

	return KErrNone;
	}


void CRawIPNifMain::LinkLayerUp()
/**
 * This function is called when initialisation is complete and the NIF is
 * ready to send/receive data. It notifies NifMan and the IP interface that it's
 * ready to go.
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::LinkLayerUp"));

	iState = EIfUp;
	iNotify->LinkLayerUp();

	iNotify->IfProgress(KLinkLayerOpen, KErrNone);

	iProtocolIf->StartSending(reinterpret_cast<CProtocolBase*>(this));
	}

void CRawIPNifMain::LinkLayerDown(TInt aError)
/**
 * This function is called when the context has been deactivated and the NIF
 * is ready to be deleted, or if there is an error on startup. Its job is to
 * notify NifMan that the link has gone down, so it can delete the NIF.
 *
 * @param aError An error code to propagate to NifMan
 */
	{
	_LOG_L1C2(_L8("CRawIPNifMain::LinkLayerDown [aError=%d]"), aError);

	iState = EIfDown;

	iNotify->IfProgress(KLinkLayerClosed, aError);

	iNotify->LinkLayerDown(aError, MNifIfNotify::EDisconnect);
	}

void CRawIPNifMain::Stop(TInt aError, MNifIfNotify::TAction /*aAction*/)
/**
 * This function is called by NifMan to bring down the link. It starts
 * the asynchronous context deactivation process; LinkLayerDown() will be
 * called when the context has been deactivated.
 *
 * @param aError A code specifying why the link is to be brought down
 * @param aAction The action to take: disconnect or reconnect
 */
	{
	_LOG_L1C2(_L8("CRawIPNifMain::Stop [aError=%d]"), aError);
	__PACKETLOG_DELETE;
	ShutDown(MControllerObserver::EInitialised, aError);
	}

void CRawIPNifMain::Restart(CNifIfBase* /*aIf*/)
/**
 * This is just an empty implementation of the CNifIfLink derived method.
 * It is used as part of functionality to allow a NIF to specify that
 * one of its binders has gone down but the link layer has not.
 *
 * @param Pointer to the protocol interface object
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::Restart called"));
	}

TInt CRawIPNifMain::Send(RMBufChain& /*aPdu*/, TAny* /*aProtocol*/)
/**
 * Unused. The Send() method on the IPv4 protocol interface should be used
 * to send IPv4 packets.
 *
 * @return Always KErrNotSupported
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::Send"));

	// Only the bearers' Send functions should be called.
	return KErrNotSupported;
	}

TInt CRawIPNifMain::SendPacket(RMBufChain& aPdu, TAny* /*aProtocol*/,
							   TUint16 /*aType*/)
/**
 * Sends a packet, via the BCA controller. This method is protocol-agnostic,
 * and is called by the IPv4 protocol interface to actually send packets.
 *
 * @param aPdu The packet to send
 * @return A standard error code
 */
	{
	_LOG_L1C2(_L8("CRawIPNifMain::SendPacket [length=%d]"),
		aPdu.Length() - aPdu.First()->Length());

	__PACKETLOG_WRITE_PACKET(aPdu, 0);
	TInt ret = iBcaController->Send(aPdu);
	
	return ret;
	}

void CRawIPNifMain::Process(RMBufChain& aPdu, TUint16 aProtocolCode)
/**
 * Handles incoming packets which have been passed up from the BCA controller.
 * If the IPv4 interface wants to process the packet, it hands it on; otherwise,
 * the packet is binned.
 *
 * @param aPdu The incoming packet
 */
	{
	_LOG_L1C2(_L8("CRawIPNifMain::Process [aPdu length=%d]"),
		aPdu.Length() - aPdu.First()->Length());

	// If the packet has zero or negative length, bin it.
	if ((aPdu.Length() - aPdu.First()->Length()) <= 0)
		{
		_LOG_L1C1(_L8("Dumped packet: illegal length"));
		aPdu.Free();
		return;
		}

	// See if the protocol interface wants the packet.
	if (iProtocolIf->WantsProtocol(aProtocolCode)) 
		{
		__PACKETLOG_WRITE_PACKET(aPdu, 0);
		iProtocolIf->Process(aPdu);
		}
	else
		{
		// The protocol didn't want the packet, so bin it.
		_LOG_L1C2(_L8("Packet was not processed: freeing (protocol code: %X)"),
			aProtocolCode);
		aPdu.Free();
		}
	}

void CRawIPNifMain::ResumeSending()
/**
 * Notifies the protocol interface that it can resume sending packets.
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::ResumeSending"));

	iProtocolIf->StartSending(reinterpret_cast<CProtocolBase*>(this));
	}

void CRawIPNifMain::AuthenticateComplete(TInt /*aResult*/)
/**
 * Notifies NifMan when authentication is complete. This function is never
 * actually called.
 *
 * @param aResult The result of the authentication (unused)
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::AuthenticateComplete"));
	}

TInt CRawIPNifMain::Notification(TAgentToNifEventType aEvent,
								  void* aInfo)
/**
 * Provides a mechanism for the notification of events. 
 *
 * @param aEvent The event type (unused)
 * @param aInfo Event information (unused)
 */
	{
	_LOG_L1C2(_L8("CRawIPNifMain::Notification [%d]"), aEvent);

	
	switch (aEvent)
	{
	case (EAgentToNifEventTypeDisableTimers) : //GPRS suspension
		{
		_LOG_L1C1(_L8("CRawIPNifMain::Received Suspend from Agent..."));

		// Let the BCA controller know that data can no longer be sent over
		// the PDP context.
		iBcaController->UpdateContextStateFlag(EFalse);

		break;
		}
				
	case (EAgentToNifEventTypeEnableTimers) : //GPRS resumption
		{
		_LOG_L1C1(_L8("CRawIPNifMain::Received Resume from Agent..."));
		iBcaController->UpdateContextStateFlag(ETrue);

		break;
		}
			
	case (EAgentToNifEventTsyConfig) : 
		{
		_LOG_L1C1(_L8("CRawIPNifMain::Received context config from Agent..."));
		
		RPacketContext::TContextConfigGPRS* localPtr = (reinterpret_cast<RPacketContext::TContextConfigGPRS*>(aInfo));

		TRAPD(err, GetProtocolIf()->UpdateContextConfigL(*localPtr));

		if (err)
			{
			_LOG_L1C1(_L8("Trapped leave from UpdateContextConfigL"));
			}
		break;
		}
			
	case (EAgentToNifEventTsyConnectionSpeed) : 
		{
		TUint connectionSpeed = reinterpret_cast<TUint>(aInfo);
		_LOG_L1C2(_L8("CRawIPNifMain::Connection Speed Received [%d]"), connectionSpeed);

		GetProtocolIf()->UpdateConnectionSpeed(connectionSpeed);
		break;
		}
			
	case (EAgentToNifEventTypeDisableConnection) :
		{
		// This is handled by NIFMAN and passed to NifMain as a Stop() call
		_LOG_L1C1(_L8("CRawIPNifMain::Received Disable connection from Agent..."));
		break;
		}
			
	case (EAgentToNifEventTypeGetDataTransfer) :
		{
		_LOG_L1C1(_L8("CRawIPNifMain::Received Get Data Transfer from Agent..."));
		break;
		}
			
	default : 
		{
		_LOG_L1C2(_L8("CRawIPNifMain::Received Notification [%d] from Agent..."), aEvent);
		break;
		}
	}

	return KErrNone;
	}

void CRawIPNifMain::BindL(TAny *aId)
/**
 * Called by NifMan to bind the NIF to a specific protocol. This is never
 * actually called; instead, the BindL() function of the IPv4 interface is used.
 *
 * @param aId A pointer to the protocol to be bound to
 * @leave KErrAlreadyExists when already bound
 */
	{
	_LOG_L1C2(_L8("CRawIPNifMain::BindL [aId=%X]"), aId);
	
	if (iProtocol)
		{
		User::Leave(KErrAlreadyExists);
		}
	iProtocol = reinterpret_cast<CProtocolBase*>(aId);
	}

CNifIfBase* CRawIPNifMain::GetBinderL(const TDesC& aName)
/**
 * Creates a new protocol interface, for supported protocols. This is currently
 * only IPv4 and ICMP, both of which use the IPv4 protocol interface class.
 *
 * @param aName The name of the protocol
 * @return A newly created protocol interface object
 * @leave KErrNotSupported if protocol is not supported
 */
	{
#ifdef __BTT_LOGGING__
	TBuf8<256> debugBuffer;
	debugBuffer.Copy(aName);
	_LOG_L1C2(_L8("CRawIPNifMain::GetBinderL [aName=%S]"), &debugBuffer);
#endif

	// IPv4 and ICMP Protocols.
	if (!aName.CompareF(KDescIp) || !aName.CompareF(KDescIcmp))
		{
		if (!iProtocolIf)
			{
			iProtocolIf = new (ELeave) CIPv4ProtocolIf(*this, iTheLogger);
#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS			
			iBcaController->SetType(KIp4FrameType);
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
			}
		}
	// IPv6 Protocol
	else if (!aName.CompareF(KDescIp6))
		{
		if (!iProtocolIf)
			{
			iProtocolIf = new (ELeave) CIPv6ProtocolIf(*this, iTheLogger);
#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS			
			iBcaController->SetType(KIp6FrameType);
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
			}
		}		
	else
		{
		User::Leave(KErrNotSupported);
		}

    return iProtocolIf;
	}

void CRawIPNifMain::InitialiseL(TInitialisationState aState, TInt aError)
/**
 * This method controls the initialisation sequence of the different modules
 * in the NIF. When a module completes all the initialisation tasks it will 
 * call again this method with the new state. If an error occurs then the 
 * ShutDown() method will be called.
 *  
 * @param aState The current initialisation step
 * @param aError A possible error
 */	
	{
	_LOG_L1C3(_L8("CRawIPNifMain::InitialiseL [aState=%d ,aError=%d]"),
		aState, aError);

	if (aError != KErrNone)
		{
		_LOG_L2C2(_L8("  *** FAILED to initialise NIF *** Error =%d"),aError);
	
		// Initialise shutdown sequence
		switch (aState)
		{
		case MControllerObserver::EBcaController:
			ShutDown(MControllerObserver::EInitialised, aError);
			break;
		default:
			_LOG_L2C3(_L8("ERROR CRawIPNifMain: Unknown state:%d in NIF: %S"), aState, &KNifName);
			User::Leave(KErrUnknown);	
			break;
			}
		return;
		}
	
	switch (aState)
		{
	case MControllerObserver::EStart:
		{
		_LOG_L1C1(_L8("Calling Agent for TSY Config"));

		GetAgent()->Notification(ENifToAgentEventTsyConfig, NULL);

		_LOG_L1C1(_L8("Calling Agent for TSY Connection Speed"));
		GetAgent()->Notification(ENifToAgentEventTsyConnectionSpeed, NULL);

		//Read the port name from CommDB.
		_LOG_L1C1(_L8("Attempting to read Port Name..."));
		TBuf<2*KCommsDbSvrMaxColumnNameLength> columnName;
		_LIT(KModemNameFormatText,"%s\\%s");
		columnName.Format(KModemNameFormatText ,MODEM_BEARER,MODEM_PORT_NAME);
		TName tempBuf;
		GetAgent()->ReadDes(columnName,tempBuf);

		_LOG_L1C2(_L8("Port details %S"),&tempBuf);
		iBcaController->SetPort(tempBuf);

		//Read the BCA name from CommDB.
		columnName.Format(KModemNameFormatText ,MODEM_BEARER,BCA_STACK);
        tempBuf.SetLength(0); // Clear the buffer in case the next ReadDes() fails
		GetAgent()->ReadDes(columnName,tempBuf);
		
		_LOG_L1C2(_L8("bcaName details %S"),&tempBuf);
		iBcaController->SetBcaStackAndName(tempBuf);

		//Read IAPID from CommDB.
		//IAP ID is used in BCA Ioctl method to set IapId for accessing CommDB.
		TUint32 iapId(0);
		columnName.Format(KModemNameFormatText ,IAP,COMMDB_ID);
		GetAgent()->ReadInt(columnName,iapId);
		_LOG_L1C2(_L8("IAP ID details %u"),iapId);
		iBcaController->SetIapId(iapId);
		
		UpdateContextState(RPacketContext::EStatusActive, KErrNone);

		iBcaController->StartL();
		}
		break;
	case MControllerObserver::EBcaController:
		_LOG_L1C1(_L8("  ***** NIF INITIALISED *****"));
		LinkLayerUp();
		break;
	default:
		_LOG_L2C3(_L8("ERROR CRawIPNifMain: Unknown state:%d in NIF: %S"), aState, &KNifName);
		User::Leave(KErrUnknown);	
		break;
		}
	}

void CRawIPNifMain::ShutDown(TInitialisationState aState, TInt aError)
/**
 * This method controls the shutdown sequence of the different modules
 * in the NIF. When a module completes all the shutdown tasks it will 
 * call again this method with the new state. 
 * 
 * @param aState The current state
 * @param aError A possible error (only during initialisation)
 */	
	{
	_LOG_L1C3(_L8("CRawIPNifMain::ShutDown [aState=%d ,aError=%d]"),
		aState, aError);

	if (aError != KErrNone)
		{
		// NIF is shutting down due to an error. 
		// The error code must be stored in order to call linklayer down 
		// passing that value
		iInitError = aError;
		}

	switch (aState)
		{
	case MControllerObserver::EInitialised:
		iBcaController->Stop(aError);
		break;
	case MControllerObserver::EBcaController:
		LinkLayerDown(iInitError);
		break;
	default:
		_LOG_L2C1(_L8("ERROR CRawIPNifMain: Unknown Shutdown step"));
		_BTT_PANIC(KNifName,KNifUnknownShutDownState);
		break;
		}
	}

MNifIfNotify* CRawIPNifMain::GetAgent()
/**
 * Returns a pointer to the MNifIfNotify class used to comunicate with the Agent 
 * 
 * @return a pointer to MNifIfNotify
 */
	{
	return iNotify;
	}
	
CProtocolIfBase* CRawIPNifMain::GetProtocolIf()
/**
 * Returns a pointer to the CIPv4ProtocolIf class
 * 
 * @return a pointer to a CProtocolIfBase derived class
 */
	{
	return iProtocolIf; 
	}

CBcaIoController* CRawIPNifMain::GetBcaController()
/**
 * Returns a pointer to the CBcaIoController class
 * 
 * @return a pointer to CBcaIoController 
 */
	{
	return iBcaController;
	}

void CRawIPNifMain::UpdateContextState(
	RPacketContext::TContextStatus aState, TInt /*aError*/)
/**
 * Updates the state of the iContextState variable 
 * 
 * @param aState The new state
 * @param aError A possible error
 */
	{
	_LOG_L1C2(_L8("CRawIPNifMain::UpdateContextState [State=%d]"), aState);

	// Note that we do not need to close the NIF down if there's been an
	// error, as the context state machine will do this for us.
	
	iContextStatus = aState;
	}

void CRawIPNifMain::UpdateContextConfig(
	RPacketContext::TContextConfigGPRS aConfig)
/**
 * Updates the state of the iContextConfig variable 
 * 
 * @param aConfig The new config
 */
	{
	_LOG_L1C1(_L8("CRawIPNifMain::UpdateContextConfig"));

	// Note that we do not need to close the NIF down if there's been an
	// error, as the context state machine will do this for us.
	
	iContextConfig = aConfig;
	}
