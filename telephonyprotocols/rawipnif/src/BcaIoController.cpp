// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Implements the interface to BCA.
// 
//

/**
 @file BcaIoController.cpp
*/

#include <e32uid.h>
#include <nifmbuf.h>

#include "Constants.h"
#include "BcaIoController.h"
#include "Sender.h"
#include "Receiver.h"

CBcaIoController::CBcaIoController(MControllerObserver& aObserver,
	CBttLogger* aTheLogger)
/**
 * Constructor. 
 *
 * @param aObserver Reference to the observer of this state machine
 * @param aTheLogger The logging object
 */
	: 
	  CBcaController(aObserver, aTheLogger),
	  iMBca(NULL),
	  iSender(NULL),
	  iReceiver(NULL),
	  iLoader(NULL)	  
	{
	}

CBcaIoController* CBcaIoController::NewL(MControllerObserver& aObserver, CBttLogger* aTheLogger)
/**
 * Two-phase constructor. Creates a new CBcaIoController object, performs 
 * second-phase construction, then returns it.
 *
 * @param aObserver The observer, to which events will be reported
 * @param aTheLogger The logging object
 * @return A newly constructed CBcaIoController object
 */
	{
	CBcaIoController* self = new (ELeave) CBcaIoController(aObserver, aTheLogger);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CBcaIoController::ConstructL()
/**
 * Second-phase constructor. Creates all the state objects it owns.
 */
	{
	_LOG_L1C1(_L8("CBcaIoController::ConstructL"));
	BaseConstructL();
	iSender = CSender::NewL(*this, iTheLogger, iMaxTxPacketSize);
	iReceiver = CReceiver::NewL(*this, iTheLogger, iMaxRxPacketSize);
	iLoader = new (ELeave) CBcaControl(*this, iTheLogger);
	}


CBcaIoController::~CBcaIoController()
/**
 * Destructor.
 */
	{
	delete iReceiver;
	delete iSender;
	delete iLoader;

	}

/** sets the BCA Stack name

* @param aBcaStack Text composed of bca stack and next bca names
*/	
void CBcaIoController::SetBcaStackAndName(const TDesC& aBcaStack, const TDesC& aBcaName)
	{
	iBcaName.Set(aBcaName);
	iBcaStack.Set(aBcaStack);
	}
	

void CBcaIoController::StartL()
/**
 *  Used to kick off the initialisation for this module
 */
	{
	_LOG_L1C1(_L8("CBcaIoController::StartL is called."));

	InitialiseBcaL();
	}

void CBcaIoController::Stop(TInt aError)
/**
 *  Used to shutdown this module. This will cancel all the outstanding 
 *  requests on the active objects owned by this module and shutdown.
 * @param aError the passed in error code as to why Stop has been called
 */
	{
	_LOG_L1C1(_L8("CBcaIoController::Stop is called."));

	//Stop all the active objects
	iReceiver->Cancel();

	if(GetSendState() == ESending)
		{
		iSender->Cancel();
		}

	// Update module state
	SendState(EShuttingDown);
	
    iLoader->ShutdownBca(aError);  
	}

void CBcaIoController::InitialiseBcaL()
/**
 *  Load & Initialise Bca.
 */
	{
	_LOG_L1C1(_L8("<<CBcaIoController::InitialiseBcaL"));
    iLoader->StartLoadL();
	}

void CBcaIoController::BcaProcess(TDesC8& aPdu)
/**
 *  This method will pass on the received data to CRawIPNifMain. 
 *
 * @param aPdu a data packet
 */
	{
	_LOG_L1C1(_L8(">>CBcaIoController::Process"));

	Process(aPdu);

	_LOG_L1C1(_L8("<<CBcaIoController::Process"));
	}

void CBcaIoController::BcaSend(RMBufChain& aPdu)
/**
 *  This method is called by CRawIPNifMain in order to send a packet down
 * to the BCA. The CSender active object will be activated by calling the send
 * method.
 *
 *	@param aPdu a data packet
 */
	{
	_LOG_L1C1(_L8("CBcaIoController::BcaSend"));

	iSender->Send(aPdu);
	}

TInt CBcaIoController::BcaSendBufferLength()
	{
	return iSender->SendBufferLength();
	}

void CBcaIoController::BcaSendComplete()	
/**
 *  This method is called after a packet was sent to the board. 
 *  If allowed by flow contol flags the NIF can signal the TCP/IP
 *  protocol indicating that is available to send more packets.
 */
	{
	_LOG_L1C1(_L8("CBcaController::SendComplete"));

	SendComplete();
	}

	
CBcaControl::CBcaControl(CBcaIoController& aObserver, CBttLogger* aTheLogger)
/**
 * Constructor. Performs standard active object initialisation.
 *
 * @param aObserver Reference to the observer of this state machine
 * @param aTheLogger The logging object
 */
	: CActive(EPriorityStandard), 
	  iObserver(aObserver), 
	  iTheLogger(aTheLogger),
	  iMBca(NULL),
	  iState(EIdling),
	  iError(KErrNone)
	  
	{
	CActiveScheduler::Add(this);
	}
	
CBcaControl::~CBcaControl()
/**
 * Destructor.
 */
	{
	Cancel();
	if(iMBca)
		{
        //If the Bca is still open, close it
        if(EBcaOpened == iState)
            {
            iMBca->Close();
            }
        //delete the BCA instance
		iMBca->Release();	
		}
	// Library will be Closed when iBcaDll is destroyed.
	}

void CBcaControl::RunL()
/**
 *  Called after request is completed. 
 *  
 */
	{
	_LOG_L1C1(_L8("CBcaControl::RunL() called"));
	switch (iState)
		{
		//in this state, Ioctl is called to set IAP ID, check the result of
		// Ioctl, then either set the BCA stack with another Ioctl call, 
		// open the BCA (if there's no BCA stack to set), or stop the NIF.
		case EIdling:
			{
			if(iStatus == KErrNone || iStatus == KErrNotSupported)
				{
				if(iStatus == KErrNotSupported)
					{
					_LOG_L1C1(_L8("This BCA does not support IAPID set"));
					}
				else
					{
					_LOG_L2C1(_L8("This BCA supports IAPID set"));
					}
				
				TPtrC bcaStack = iObserver.BcaStack();
				if(bcaStack.Length())
					{
					TBuf8<KMaxName> remainingBcaStack8;
					remainingBcaStack8.Copy(bcaStack);
					iMBca->Ioctl(iStatus, KBcaOptLevelGeneric,KBCASetBcaStack,remainingBcaStack8);
					}
				else
					{
					TRequestStatus* statusPtr=&iStatus;
					User::RequestComplete(statusPtr,KErrNone);
					}
				iState = EIAPSet;
				SetActive();	
				}
			else
				{
				_LOG_L1C2(_L8("ERROR in BCA IAPID set = %d"), iStatus.Int());
				iObserver.Stop(iStatus.Int());
				}
			
			break;
			}
			
		//in this case, we receive the result of Ioctl call to set Bca Stack.
		// Check the result of Ioctl, then Open the Bca or stop the NIF
		case EIAPSet:
			{
			if(iStatus == KErrNotSupported || iStatus == KErrNone)
				{
				if(iStatus == KErrNotSupported)
					{
					_LOG_L1C1(_L8("This BCA does not support BCA stacking"));
					}
				else
					{
					_LOG_L2C1(_L8("This BCA supports BCA stacking"));
					}
				iMBca->Open(iStatus, iObserver.Port());
				iState = EBcaStackSet;
				SetActive();	
				}
			else
				{
				_LOG_L2C2(_L8("ERROR in BCA stack set = %d"), iStatus.Int());
				iObserver.Stop(iStatus.Int());
				}
			break;
			}
		
		//in this state, BCA Open is called. Checks the result of Open.
		// If it is successful,then start the NIF. Otherwise stops the NIF.
		case EBcaStackSet:
			{
			if(iStatus != KErrNone && iStatus !=  KErrAlreadyExists)
				{
				_LOG_L2C2(_L8("ERROR in BCA Open = %d"), iStatus.Int());
				iObserver.Stop(iStatus.Int());
				}
			else
				{
                iState = EBcaOpened;
                //Activate the receiver Active Object
				iObserver.Receiver().StartListening();
				_LOG_L1C1(_L8("CBcaIoController Is Initialised"));
				TRAPD(err, iObserver.GetObserver().InitialiseL(MRawIPObserverBase::EBcaController,KErrNone));
				if(err != KErrNone)
					{
					_LOG_L2C2(_L8("ERROR in BCA Open Initialise observer = %d"), err);
					iObserver.Stop(err);
					}
				}
			break;
			}

		//in this state, BCA is Shutdown, shutdown the NIF.
		case EClosing:
			{
			// linklayer shutdown
			iState = EIdling;
			iObserver.GetObserver().ShutDown(MControllerObserver::EBcaController, iError);
			break;
			}
		// Wrong state.
		default:
			{
			_LOG_L1C1(_L8("ERROR CBcaControl::RunL(): Unknown state"));
			_BTT_PANIC(KNifName, KBcaUnkownState);
			break;
			}
		}

	}
	
void CBcaControl::DoCancel()
/**
 *	cancel active request. 
 */
	{
	_LOG_L1C1(_L8("CBcaControl::DoCancel called."));
	_LOG_L2C2(_L8("iState value is %d"), iState);
	switch (iState)
		{
		case EIdling:
		case EIAPSet:
	    case EBcaStackSet:
			if(iMBca)
				{
				iMBca->CancelIoctl();
				}
			iState = EIdling;
			break;
		case EClosing:
            iState = EIdling;		    
            break;    
		default:
			_LOG_L2C1(_L8("ERROR CBcaControl::DoCancel(): Unknown state"));
			_BTT_PANIC(KNifName, KBcaUnkownState);
			break;
		}
	}
	
void CBcaControl::StartLoadL()
/**
 *  This method loads the C32BCA library and uses Ioctl to set the Bca iIapId. 
 */
	{
	_LOG_L1C1(_L8("CBcaControl::StartLoad"));
	
	//iMBca should not be initialized at this point
	__ASSERT_DEBUG(!iMBca,Panic(KBcaAlreadyExists));
	
	//We don't expect iMBca here, but if it occurs, we delete previous BCA Instance
	if(iMBca)
	    {
        //If the state is still "open", close it first
        if(EBcaOpened == iState)
            {
            iMBca->Close();
            }
        iMBca->Release();
        }

	// Loads Bca Dll and creates a Bca instance;
	User::LeaveIfError(iBcaDll.iObj.Load(iObserver.BcaName()));
	
	TNewBcaFactoryL newBcaFactoryProcL = (TNewBcaFactoryL)iBcaDll.iObj.Lookup(1);
	if (NULL == newBcaFactoryProcL)
		{
		_LOG_L1C2(_L8("Library entry point found error %d"), KErrBadLibraryEntryPoint);
		User::Leave(KErrBadLibraryEntryPoint);	
		}
	
	MBcaFactory* bcaFactory = (*newBcaFactoryProcL)();

	if(!bcaFactory)
		{
		_LOG_L1C2(_L8("BcaFactory creation error %d"), KErrCompletion);
		User::Leave(KErrCompletion);	
		}
	CleanupReleasePushL(*bcaFactory);
	
	iMBca = bcaFactory->NewBcaL();
	CleanupStack::PopAndDestroy(bcaFactory);
	
	iObserver.SetBca(iMBca); //Pass BCA pointer.

	TPckg<TUint32> aOpt(iObserver.IapId());
	iMBca->Ioctl(iStatus,KBcaOptLevelGeneric,KBCASetIapId,aOpt);
	
	iState = EIdling;
	SetActive();
	}

	
void CBcaControl::ShutdownBca(TInt aError)
/**
 *  Bca Shutdown.
 
 *  @param aError the error code to shutdown the NIF. 
 */
	{
	__ASSERT_DEBUG(iMBca,Panic(KBcaNotExist));
	Cancel();
        
    //We should only call shutdown or close if we have successfully opened a BCA Channel
    if((iMBca) && (EBcaOpened == iState))
        {
        if(aError == KErrConnectionTerminated )
            {
            _LOG_L1C1(_L8("This is an emergency shutdown, it kills the NIF immediately."));
            // It is a emergency shutdown, it kills the NIF immediately.
            iMBca->Close();
            iState = EIdling;
            iObserver.GetObserver().ShutDown(MControllerObserver::EBcaController, aError);
            }
        else
            {
            _LOG_L1C1(_L8("This is a graceful termination which takes a while."));
            //It is a graceful termination which takes a while.
            iError = aError;
            iState = EClosing;
            iMBca->Shutdown(iStatus);
            SetActive();    
            }
        }
    else //nothing to shutdown, just notify linklayer down.
        {
        _LOG_L1C1(_L8("Bca is not initialized or opened, bring the linklayer down"));
        iState = EIdling;
        iObserver.GetObserver().ShutDown(MControllerObserver::EBcaController, aError);
        }
  
	}

/** Panic function for RawIpNif 

* @param aPanic panic code */
void Panic(TRawIPNifPanic aPanic)
	{
	User::Panic(KNifName,aPanic);
	}

