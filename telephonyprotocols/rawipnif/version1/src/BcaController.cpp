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
// Implements the interface to Bca & flow control.
// 
//

/**
 @file
*/

#include <e32uid.h>
#include <nifmbuf.h>
#include <es_ini.h>
#include "BcaController.h"

_LIT(KRawIpIniFile, "rawip.ini");
_LIT(KLinkLit, "link");
_LIT(KHighmarkLit, "highmark");
//In order not to flow off SPUD everytime we set the default to 1
const TUint KDefaultBufferSize=1;

CBcaController::CBcaController(MControllerObserver& aObserver,
	CBttLogger* aTheLogger)
/**
 * Constructor. 
 */
	: iObserver(aObserver),  
	  iTheLogger(aTheLogger),
	  iTxFlowControl(EFlowControlOff), 
	  iTxContextActive(ETrue), 
	  iSendState(EIdle),
	  iMaxSendQueueLen(0),
	  iNumPacketsInSendQueue(0)
	{
	iSendQueue.Init();
	}


CBcaController::~CBcaController()
/**
 * Destructor.
 */
	{
	iSendQueue.Free();
	iNumPacketsInSendQueue = 0;

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
	delete iIPTagHeader;
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
	}

void CBcaController::BaseConstructL()
	{
	_LOG_L1C1(_L8("CBcaController::BaseConstructL"));

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
	iIPTagHeader = new (ELeave) CIPTagHeader(iTheLogger);
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
	CESockIniData* iniData = NULL;
	TRAPD(res, iniData = CESockIniData::NewL(KRawIpIniFile));
	CleanupStack::PushL(iniData);
	
	if(res!=KErrNone)
		{
		_LOG_L1C2(_L8("RawIp ini file %S not found. Default values will be used."), &KRawIpIniFile);
		CleanupStack::PopAndDestroy();
		return;
		}
	
	//here process the file
	if(!iniData->FindVar(KLinkLit(), KHighmarkLit(), iMaxSendQueueLen))
		{
		iMaxSendQueueLen = KDefaultBufferSize;
		}
	
	CleanupStack::PopAndDestroy();
	}

void CBcaController::UpdateInternalFlowFlag(TFlowControl aValue)
/**
 *  Updates Internal Flow flag and resumes the data flow if
 *  necessary.
 *
 * @param aValue the new state of iInternalFlow
 */
	{
	_LOG_L1C3(_L8("CBcaController::UpdateInternalFlowFlag[NewValue=%d, iSendState=%d]"),
		aValue, iSendState);

	if(iTxFlowControl == aValue)
		{
		// C32 Sent the same indication signal twice. Nif will ignore it.
		_LOG_L2C1(_L8("WARNING CBcaController: Received same indication twice"));
		return;
		}
	
	// Update the flag value.
	iTxFlowControl = aValue;

	if(iTxFlowControl == EFlowControlOff)
		{
		// If the indication received turned flow control off...
		if (IsTxPossible() && (iSendState == EIdle))
			{
			// ... if the NIF is in the EWaiting state
			// then the data flow can be resumed. 
			ResumeSending();
			}
		}		
	else
		{
		// if the Flow Control is on we can remove all queued write requests
		EmptySendQueue();
		}
	}

void CBcaController::UpdateContextStateFlag(TBool aValue)
/**
 *  Updates Context State flag and resumes the data flow if
 *  necessary.
 *
 * @param aValue the new state of iTxContextState
 */
	{
	_LOG_L1C3(_L8("CBcaController::UpdateContextStateFlag[NewValue=%d, OldValue=%d]"),
		aValue, iTxContextActive);
	
	if(iTxContextActive == aValue)
		{
		return;
		}

	// Update the flag value.
	iTxContextActive = aValue;

	if(iTxContextActive)
		{
		// If the PDP context is active and...
		if (IsTxPossible() && (iSendState == EIdle))
			{
			// ... if the NIF is in the EWaiting state
			// then the data flow can be resumed. 
			ResumeSending();
			}
		}
	else
		{
		// if the PDP context is suspended we can remove all queued write requests
		EmptySendQueue();
		}
	}

TInt CBcaController::Send(RMBufChain& aPdu)
/**
 *  This method is called by CRawIPNifMain in order to send a packet down
 * to the BCA. 
 *
 *	@param aPdu a data packet
 */
	{
	_LOG_L1C1(_L8(">>CBcaController::Send"));

	// Check if NIF is shutting down
	if (iSendState == EShuttingDown)
		{
		_LOG_L2C1(_L8("    ERROR: Nif is shutting down"));
		
		aPdu.Free();

		return KErrDisconnected;
		}
	
	// check that this packet isnt too big - If it is, we dont want to send it or
	// add it to our queue
	if ((aPdu.Length() - aPdu.First()->Length()) > BcaSendBufferLength())
		{
		_LOG_L2C1(_L8("Packet is too large - discarding"));
		_LOG_L1C1(_L8("<<CSender::Send -> Error"));

		aPdu.Free();
		return KErrArgument;
		}
	
	if (iSendState == ESending)
		// If this happens, it means that TCP/IP has sent us an IP packet
		// while we're still sending the previous one. 
		{
		// check that the queue isnt full NB. this check should not be needed as when the 
		// queue becomes full the IP layer shouldnt send any more packets until it is told to
		if (!IsSendQueueFull())
			{
			_LOG_L1C1(_L8("    Sender busy, appending packet to queue"));
			//We know that flow control is off and context isnt suspended so can add to queue
			AppendToSendQueue(aPdu);
			
			return IsSendQueueFull() ? KStopSending : KContinueSending;
			}
			
		_LOG_L1C1(_L8("    Queue is full, upper layer is still sending packets, potential memory problems."));
		AppendToSendQueue(aPdu);
		return KStopSending;
		}

	// If we have got here then a write isnt currently happening
	// We dont need to check flow control is off and context isnt suspended as the BCA always
	// has room for one packet, so send the packet

	if(!IsSendQueueEmpty())
		{
		//make sure that we don't change the order of packets!
		//first send what has already been lined up
		RMBufChain tmpPdu;
		_LOG_L1C1(_L8("    Packet removed from queue to send"));
		RemoveFromSendQueue(tmpPdu);
		AppendToSendQueue(aPdu);
		
		// Update module state
		_LOG_L2C1(_L8("     set State to ESending"));
		iSendState = ESending;
		
		BcaSend(tmpPdu);
		}
	else
		{
		// Update module state
		_LOG_L2C1(_L8("     set State to ESending"));
		iSendState = ESending;
		 
		BcaSend(aPdu);
		}
		
	_LOG_L2C1(_L8("<<CBcaController::Send - return StopSending/ContinueSending"));
	return IsSendQueueFull() ? KStopSending : KContinueSending;
	}

void CBcaController::SendComplete()	
/**
 *  This method is called after a packet was sent to the board. 
 *  If allowed by flow contol flags the NIF can signal the TCP/IP
 *  protocol indicating that is available to send more packets.
 */
	{
	_LOG_L1C1(_L8("CBcaController::SendComplete"));
	_LOG_L2C1(_L8("     set State to EIdle"));
	
	iSendState = EIdle;

	if (IsTxPossible())
		ResumeSending();
	}

TBool CBcaController::IsTxPossible()
/**
 *  This method returns ETrue if both TX flags are set to ETrue
 *
 * @return The Flow control state
 */
	{
	_LOG_L1C3(_L8("CBcaController::IsTxPossible (contextActive %d, flowcontrol %d)"), 
		iTxContextActive, iTxFlowControl);

	if(iTxContextActive && (iTxFlowControl == EFlowControlOff))
		return ETrue;
	else
		return EFalse;
	}

void CBcaController::Process(TDesC8& aPdu)
/**
 *  This method will pass on the received data to CBttNifMain. 
 *
 * @param aPdu a data packet
 */
	{
	_LOG_L1C1(_L8(">>CBcaController::Process"));

	TInt ret;

	// Create a packet object.
	RMBufPacket packet;
	TRAP(ret, packet.CreateL(aPdu));
	if (ret != KErrNone)
		{
		// Couldn't create package. Packet will be ignored...
		_LOG_L1C2(_L8("<<CBcaController::Process couldn't create MBuf [ret=%d]"), ret);
		return;
		}
	else
		// Package created...
		{
#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
		TUint16 protocolCode = iIPTagHeader->RemoveHeader(packet);
#else
		TUint16 protocolCode = 0;
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS

		packet.Pack();
		// Process the packet
		GetObserver().Process(packet, protocolCode);
		}

	_LOG_L1C1(_L8("<<CBcaController::Process"));
	}

void CBcaController::ResumeSending()
/**
 *  Used to indicate to the TCP/IP protocol layer that the NIF is ready to 
 *	process more packets.
 */
	{
	_LOG_L1C1(_L8("CBcaIoController::ResumeSending"));

	// If there are still some packets in the queue to be sent, then carry
	// on sending them.
	// NB. we only want to send more packets from the queue if we are currently EIdle
	if (iSendState == EIdle) 
		{
		if(!IsSendQueueEmpty())
			{
			RMBufChain tmpPdu;
			_LOG_L1C1(_L8("    Packet removed from queue to send"));
			RemoveFromSendQueue(tmpPdu);
			
			// Update module state
			_LOG_L2C1(_L8("     set State to ESending"));
			iSendState = ESending;
			
			BcaSend(tmpPdu);
			}
		if(IsSendQueueEmpty())
			{
			iObserver.ResumeSending();
			}
		}
	// if iSendState = ESending - do nothing
	}
	
#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
void CBcaController::SetType(TUint16 aType)
{
/**
 *  Used to specify the type of the IP header.
 */
	_LOG_L1C1(_L8("CBcaController::SetType"));
	
	iIPTagHeader->SetType(aType);	
}

void CBcaController::AddHeader(TDes8& aDes)
/**
 *  Used to add the IP header to the packet before sending to the BCA.
 */
	{
	_LOG_L1C1(_L8("CBcaController::AddHeader"));

	iIPTagHeader->AddHeader(aDes);
	}

TUint16 CBcaController::RemoveHeader(RMBufChain& aPdu)
/**
 *  Used to remove the IP header from the received the packet before sending to the 
 *  TCP/IP layer.  
 * @return The IP header that has been removed from the packet
 */
	{
	_LOG_L1C1(_L8("CBcaController::RemoveHeader"));

	return (iIPTagHeader->RemoveHeader(aPdu));
	}	
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS

TBool CBcaController::IsSendQueueEmpty()
/**
 *  Indicator of whether the BufferQueue is empty
 * @return TBool.  ETrue if bufferQueue is emtpy, EFalse if queue is not empty
 */
	{
	return iSendQueue.IsEmpty();
	}
	
TBool CBcaController::IsSendQueueFull()
/**
 *  Indicator of whether the BufferQueue is full
 * @return TBool.  ETrue if bufferQueue is full, EFalse if queue is not full
 */
	{
	return iNumPacketsInSendQueue >= iMaxSendQueueLen;
	}	

void CBcaController::AppendToSendQueue(RMBufChain& aPdu)
/**
 * Appends the packet aPdu to the queue. 
 * Increments the packet count. Doesn't do error checking.
 * @param aChain buffer chain to be added
 */
	{
	iSendQueue.Append(aPdu);
	iNumPacketsInSendQueue++;
	}
	
TBool CBcaController::RemoveFromSendQueue(RMBufChain& aPdu)
/**
 * Removes the packet aPdu from the queue. 
 * Decrements the packet count.
 * @param aChain buffer chain to be added
 * @return False if chain is empty
 */
	{
	TBool ret = iSendQueue.Remove(aPdu);
	if(ret)
		{
		iNumPacketsInSendQueue--;
		}
	return ret;
	}
	
void CBcaController::EmptySendQueue()
/**
 * Removes all the packets from the send queue. Initializes the
 * send queue and sets the packet count to 0.
 */
	{
	iSendQueue.Free();
	iSendQueue.Init();
	iNumPacketsInSendQueue = 0;
	}
