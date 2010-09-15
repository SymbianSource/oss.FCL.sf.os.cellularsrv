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
// Implements the active object that controls the Read() requests.
// 
//

/**
 @file
*/

#include "Receiver.h"
#include "Constants.h"
#include <es_ini.h>

const TUint KBufferIncreaseStep=500;
const TUint K64k=65535;

CReceiver::CReceiver(CBcaIoController& aObserver, CBttLogger* aTheLogger, TUint aMaxPacketSize)
/**
 * Constructor. Performs standard active object initialisation.
 *
 * @param aObserver Reference to the observer of this state machine
 * @param aTheLogger The logging object
 */
	: CActive(EPriorityHigh), 
	  iObserver(aObserver), 
	  iTheLogger(aTheLogger),
	  iMaxPacketSize(aMaxPacketSize)
	{	
	CActiveScheduler::Add(this);
	}

CReceiver* CReceiver::NewL(CBcaIoController& aObserver, CBttLogger* aTheLogger, TUint aMaxPacketSize)
/**
 * Two-phase constructor. Creates a new CBcaIoController object, performs 
 * second-phase construction, then returns it.
 *
 * @param aObserver The observer, to which events will be reported
 * @param aTheLogger The logging object
 * @return A newly constructed CBcaIoController object
 */
	{
	CReceiver* self = new (ELeave) CReceiver(aObserver, aTheLogger, aMaxPacketSize);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CReceiver::ConstructL()
/**
 * Second-phase constructor. Creates all the state objects it owns.
 */
	{
	_LOG_L1C1(_L8("CReceiver::ConstructL"));
	iData.CreateL(iMaxPacketSize);
	}

CReceiver::~CReceiver()
/**
 * Destructor.
 */
    {
    Cancel();
    // iData is a shared bit of memory between raw ip and bca
    // you cannot delete it while bca might be using it otherwise
    // bad things may happen.
    iData.Close();
    }

void CReceiver::RunL()
/**
 *  Method called when read request completes. This will only be once the BCA has a full IP 
 *  packet in its buffer.
 */
	{
	_LOG_L1C2(_L8("CReceiver::RunL [iStatus=%d]"), iStatus.Int());

	if (iStatus != KErrNone)
		{
		if (iStatus == KErrNoMemory)
			{
			_LOG_L2C1(
				_L8("WARNING! CReceiver: Read failed with KErrNoMemory. Increase buffer."));
			// Read operation failed!! Nif will re-issue the read request. Increase buffer.			
			if ((iMaxPacketSize + KBufferIncreaseStep) > K64k)
			    {
			    // In theory IP packet can't be bigger than 64k, so if we come here something is wrong so stop observer. 
                iObserver.Stop(KErrNoMemory);
			    }
			else
			    {
                iMaxPacketSize += KBufferIncreaseStep;
                TInt err = iData.ReAlloc(iMaxPacketSize);
                if (KErrNoMemory == err)
                    {                
                    iObserver.Stop(KErrNoMemory);
                    }
                else
                    {
                    (iObserver.Bca())->Read(iStatus, iData);    
                    SetActive();
                    }
			    }
			}
		else 
			{
			_LOG_L2C1(_L8("WARNING! CReceiver: Read failed"));
			iObserver.Stop(iStatus.Int());
			}
		return;
		}
	else
	    {
        _LOG_L1C1(_L8("CReceiver: Data Packet Received"));

        // Trap the leave to prevent the flow from being stopped if
        // the MBuf pool is temporarily exhausted.    
		TRAPD(err,iRMBufPacket.CreateL(iData));

        // Immediately execute new read request, regardless of whether MBufs are
        // exhausted or not.
        (iObserver.Bca())->Read(iStatus, iData);
    
        SetActive();
        
        if (err == KErrNone)
            {
        	iRMBufPacket.Pack();
    
#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
        	TUint16 protocolCode = iObserver.RemoveHeader(iRMBufPacket);
#else
        	TUint16 protocolCode = 0;
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
    
        	// Process the packet
        	iObserver.GetObserver().Process(iRMBufPacket, protocolCode);
        	iRMBufPacket.Free();
			}
		else
			{
			// If MBuff Allocation failed then dont process the packet, rely on layers above for recovery
			_LOG_L2C2(_L8("CReceiver::RunL MBuff Allocation Failure [err=%d]"), err);
			}
	    }
	}

TInt CReceiver::RunError(TInt aError)
    {
    _LOG_L2C1(_L8("WARNING! CReceiver::RunError Read failed"));
    iObserver.Stop(aError); 
    return KErrNone;  
    }

void CReceiver::DoCancel()
/**
 *	Cancel active request
 */
	{
	_LOG_L1C1(_L8("CReceiver::DoCancel"));

	(iObserver.Bca())->CancelRead(); 
	}

void CReceiver::StartListening()
/**
 *  This method issues a Read request. This is the API used to receive 
 *  packets from BCA.  
 */
	{
	_LOG_L1C1(_L8("CReceiver::StartListening"));

	// The BCA will complete this read once it has a full IP packet in its buffer.
	(iObserver.Bca())->Read(iStatus, iData);

	SetActive();
	}
