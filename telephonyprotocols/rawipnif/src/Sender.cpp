// Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Implements the active object that controls the Write() requests. 
// 
//

/**
 @file
*/

#include "Sender.h"
#include "Constants.h"
#include <es_ini.h>


CSender::CSender(CBcaIoController& aObserver, CBttLogger* aTheLogger, TInt aMaxPacketSise)
/**
 * Constructor. Performs standard active object initialisation.
 *
 * @param aObserver Reference to the observer of this state machine
 */
	: CActive(EPriorityUserInput), 
	  iObserver(aObserver),
	  iTheLogger(aTheLogger),
	  iMaxPacketSize(aMaxPacketSise)
	{
    // EPriorityUserInput is higher than the default priority but lower than
    // EPriorityHigh which is used on Receiving (DL having priority), however,
    // we want this to be handled in an expedited manner compared to other
    // active objects in the thread.
	CActiveScheduler::Add(this);
	}

CSender* CSender::NewL(CBcaIoController& aObserver, CBttLogger* aTheLogger, TInt aMaxPacketSise)
/**
 * Two-phase constructor. Creates a new CBcaIoController object, performs 
 * second-phase construction, then returns it.
 *
 * @param aObserver The observer, to which events will be reported
 * @param aTheLogger The logging object
 * @return A newly constructed CBcaIoController object
 */
	{
	CSender* self = new (ELeave) CSender(aObserver, aTheLogger, aMaxPacketSise);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CSender::ConstructL()
/**
 * Second-phase constructor. Creates all the state objects it owns.
 */
	{
	_LOG_L1C1(_L8("CSender::ConstructL"));
	iSendBuffer.CreateL(iMaxPacketSize);
	}

CSender::~CSender()
/**
 * Destructor.
 */
	{
	iSendBuffer.Close();
	Cancel();
	}

void CSender::RunL()
/**
 * This method checks if any error occured in the write operation.  
 */
	{
	_LOG_L1C2(_L8("CSender::RunL [iStatus=%d]"), iStatus.Int());

	if (iStatus!=KErrNone)
		{
		if(iStatus == KErrNoMemory)
			{
			_LOG_L2C1(_L8("WARNING! CSender: Write failed with KErrNoMemory"));
			_LOG_L2C1(_L8("WARNING! CSender: Ignoring packet!!!!"));
			// Write operation failed!! Nif will ignore this packet.
			iObserver.SendComplete();
			}
		else if (iStatus == KErrNotReady)
			{
			_LOG_L2C1(_L8("WARNING! CSender: Write failed with KErrNotReady"));
			_LOG_L2C1(_L8("WARNING! CSender: Ignoring packet!!!!"));
			// Write operation failed!! Nif will ignore this packet.
			iObserver.SendComplete();
			}
		else
			{
			_LOG_L2C1(_L8("ERROR! CSender: Write failed!!!!"));
			// Nif will shut down
			iObserver.Stop(iStatus.Int());
			}
		return;
		}

	else
		{
		// The Ip packet was sent successfuly
		_LOG_L1C1(_L8("***** CSender: Packet Sent."));
		iObserver.SendComplete();
		}
	}

void CSender::DoCancel()
/**
 *	Cancel active request
 */
	{
	_LOG_L1C1(_L8("CSender::DoCancel"));

	(iObserver.Bca())->CancelWrite(); 
	}

void CSender::Send(RMBufChain& aPdu)
/**
 * Copies the specified RMBufChain into a descriptor and sends it.
 *
 * @param aPdu The IP packet to be sent.
 * @return KStopSending, or KErrArgument if the packet is too large.
 */
	{
	_LOG_L1C1(_L8("CSender::Send"));

	// Copy the IP portion of the RMBufChain to the buffer.
	iSendBuffer.SetMax();
	aPdu.CopyOut(iSendBuffer, aPdu.First()->Length());

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
	iObserver.AddHeader(iSendBuffer);
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS

	aPdu.Free();

	(iObserver.Bca())->Write(iStatus, iSendBuffer);
	
	SetActive();
	}

