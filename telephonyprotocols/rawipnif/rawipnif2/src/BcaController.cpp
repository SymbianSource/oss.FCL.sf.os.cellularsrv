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

#include "Constants.h"
#include "BcaController.h"

CBcaController::CBcaController(CRawIP2Flow& aRawIPFlow,CBttLogger* aTheLogger)
/**
 * Constructor. Performs standard active object initialisation.
 *
 * @param aRawIPFlow Reference to the RawIp2Flow
 * @param aTheLogger The logging object
 */
	: CActive(EPriorityStandard), 	  
	  iTheLogger(aTheLogger),	 
	  iMBca(NULL),	  
	  iState(EIdling),
	  iRawIPFlow(aRawIPFlow),
	  iUpperControl(NULL),
	  iUpperDataReceiver(NULL),	 
	  iBCAProvisionConfig(NULL),
	  iBcaParams(NULL)
	{
	CActiveScheduler::Add(this);
	}
	 
CBcaController* CBcaController::NewL(CRawIP2Flow& aRawIPFlow,CBttLogger* aTheLogger)
/**
 * Two-phase constructor. Creates a new CBcaController object, performs 
 * second-phase construction, then returns it.
 *
 * @param aRawIPFlow Reference to the RawIp2Flow
 * @param aTheLogger The logging object
 * @return A newly constructed CBcaController object
 */
	{
	CBcaController* self = new (ELeave) CBcaController(aRawIPFlow,aTheLogger);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CBcaController::ConstructL()
/**
 * Second-phase constructor. Creates all the state objects it owns.
 */
	{
	_LOG_L1C1(_L8("CBcaController::ConstructL"));
	
	}
  
 
CBcaController::~CBcaController()
/**
 * Destructor.
 */
	{
	Cancel();
	if(iMBca)
		{
		iMBca->Release();	
		}
	delete iBcaParams;		
	// Library will be Closed when iBcaDll is destroyed.
	}

void CBcaController::RunL()
/**
 *  AO async callback method. Called after request is completed. 
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
				
				TPtrC bcaStack = iBCAProvisionConfig->GetBCAStack();
				if(bcaStack.Length())
					{
					TBuf8<KMaxName> remainingBcaStack8;
					remainingBcaStack8.Copy(bcaStack);
					iMBca->Control(iStatus, KBcaOptLevelGeneric,KBCASetBcaStack,remainingBcaStack8);
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
				Stop(iStatus.Int());
				}
			
			break;
			}			
		//in this case, we receive the result of Ioctl call to set Bca Stack.
		// Check the result of Ioctl, then Open and start the Bca or stop the NIF
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
				iBcaParams = new(ELeave) MBca2::TBcaParams(const_cast<CBCAProvision*>(iBCAProvisionConfig)->GetCommsPond(), iBCAProvisionConfig->GetPortName());
			
				TInt aErr = iMBca->Open(*iUpperControl,*iUpperDataReceiver,*iBcaParams);
				if ( aErr != KErrNone)
					{					
					_LOG_L2C2(_L8("ERROR in BCA Open = %d"), aErr);
					Stop(iStatus.Int());
					iState = EIdling;
					}
				else
					{
					iMBca->Start();					
					}
				}
			else
				{
				_LOG_L2C2(_L8("ERROR in BCA stack set = %d"), iStatus.Int());
				Stop(iStatus.Int());
				}
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
	
void CBcaController::DoCancel()
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
			if(iMBca)
				{
				iMBca->CancelControl();
				}
			iState = EIdling;
			break;
		default:
			_LOG_L2C1(_L8("ERROR CBcaControl::DoCancel(): Unknown state"));
			_BTT_PANIC(KNifName, KBcaUnkownState);
			break;
		}
	}
	
void CBcaController::StartLoadL(const CBCAProvision* aBCAProvisionConfig,MUpperControl* aControl, 
		MUpperDataReceiver* aData)
/**
 *  This method loads the C32BCA library and uses Ioctl to set the Bca iIapId.
 *  
 * @param aBCAProvisionConfig pointer to CBCAProvision for extracting the pond
 * @param aControl pointer to BasebandChannelAdaptation2::MUpperControl passed to the MBca2 to pass control information up the stack
 * * @param aData pointer to BasebandChannelAdaptation2::MUpperDataReceiver passed to the MBca2 to push packets up the stack
 * @return none 
 */
	{
	_LOG_L1C1(_L8("CBcaControl::StartLoad"));

	iBCAProvisionConfig = aBCAProvisionConfig;
	iUpperControl = aControl;
	iUpperDataReceiver = aData;
	// Loads Bca Dll and creates a Bca instance;
	User::LeaveIfError(iBcaDll.iObj.Load(iBCAProvisionConfig->GetBCAName()));
	
	TNewBca2FactoryL newBca2FactoryProcL = (TNewBca2FactoryL)iBcaDll.iObj.Lookup(1);
	if (NULL == newBca2FactoryProcL)
		{
		_LOG_L1C2(_L8("Library entry point found error %d"), KErrBadLibraryEntryPoint);
		User::Leave(KErrBadLibraryEntryPoint);	
		}
	
	MBca2Factory* bcaFactory = (*newBca2FactoryProcL)();

	if(!bcaFactory)
		{
		_LOG_L1C2(_L8("BcaFactory creation error %d"), KErrCompletion);
		User::Leave(KErrCompletion);	
		}
	CleanupReleasePushL(*bcaFactory);
	
	iMBca = bcaFactory->NewBcaL();
	CleanupStack::PopAndDestroy(bcaFactory);
	
	TPckg<TUint32> aOpt(iBCAProvisionConfig->GetIAPid());
	iMBca->Control(iStatus,KBcaOptLevelGeneric,KBCASetIapId,aOpt);
	
	iState = EIdling;
	SetActive();
	}

void CBcaController::Stop(TInt aError)
/**
 *  Used to shutdown this module. This will cancel all the outstanding 
 *  requests on the active objects owned by this module and shutdown.
 */
	{
	_LOG_L1C1(_L8("CBcaController::Stop is called."));

	if(iMBca)
		{
		iMBca->Close();
		}
	else 
		{
		_LOG_L1C1(_L8("CBcaController::Stop Bca is not initialized. Bring the link layer down"));
		iRawIPFlow.LinkLayerDown(aError);
		}
  	}


/** Panic function for RawIpNif 

* @param aPanic panic code */
void Panic(TRawIP2NifPanic aPanic)
	{
	_LOG_L2C2(_L8("Panic code for RawIpNif = %d"), aPanic);
	User::Panic(KNifName,aPanic);

	}

