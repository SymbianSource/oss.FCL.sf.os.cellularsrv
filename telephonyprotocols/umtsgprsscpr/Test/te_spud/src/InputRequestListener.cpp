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
// Implementation of CInputRequestListener
// 
//

/**
 @file
 @internalComponent
*/


#include "CPdpFsmInterface.h"

#include "SpudUnitTestServer.h"
#include "InputRequestListener.h"
#include <in_iface.h>
#include "reteldriverinput.h"
#include "ceteldriverfactory.h"
#include "cpdpfsmfactory.h"
#include "DefaultContextParameters.h"

CInputRequestListener* CInputRequestListener::NewL(TBool aUseTestPdpInterface)
	{
	CInputRequestListener *me = new(ELeave) CInputRequestListener(aUseTestPdpInterface);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

CInputRequestListener::CInputRequestListener(TBool aUseTestPdpFsmInterface) : CActive(EPriorityStandard), iUseTestPdpFsmInterface(aUseTestPdpFsmInterface)
	{
	iReadyForInputRequest = KRequestPending;
	iThreadDestructed     = KRequestPending;
	iThreadInitialized    = KRequestPending;

	iEtelDriverInputResponseStatus             = KRequestPending;
	iEtelDriverConfigGPRSNotificationStatus    = KRequestPending;
	iEtelDriverContextStatusNotificationStatus = KRequestPending;
	iEtelDriverQosNotificationStatus           = KRequestPending;
	iEtelDriverServiceNotificationStatus       = KRequestPending;
	
	iFsmInputResponseStatus          = KRequestPending;
	iFsmNetworkStatusStatus          = KRequestPending;
	iFsmContextParameterChangeStatus = KRequestPending;
	iFsmContextSuspendedStatus       = KRequestPending;
	iFsmContextResumedStatus         = KRequestPending;	
    iFsmObjectCreateResponse         = KRequestPending;	
	}
	
void CInputRequestListener::ConstructL()
	{
	iSpudManInterface = new(ELeave) TSpudManInterfaceTest(*this);
	}

CInputRequestListener::~CInputRequestListener()
	{
	Cancel();
	if (iEtelDriver)
		{
		iEtelDriver->Close();
		delete iEtelDriver;
		iEtelDriver = NULL;
		}
	
	if (iPdpFsmInterfaceTest)
		{
		iPdpFsmInterfaceTest->Close();
		delete iPdpFsmInterfaceTest;
		iPdpFsmInterfaceTest = NULL;
		}

	if (iPdpFsmInterface)
		{
		iPdpFsmInterface->Close();
		delete iPdpFsmInterface;
		iPdpFsmInterface = NULL;
		}
	}
	
void CInputRequestListener::DoCancel()
	{
	iStatus = KErrCancel;
	}
	
/**
 Wrapper for private method CActive::SetActive()
 */
void CInputRequestListener::Activate()
	{
	iStatus = KRequestPending;
	SetActive();
	}

/**
 If iStatus was completed with EEtelDriverInputRequest, REtelDriverInput::Input is called with the parameters supplied in iInputData
 If iStatus was completed with EEtelDriverCancelPdpRequest, REtelDriverInput::Cancel is called with the parameters supplied in iEtelDriverCancelRequestData
 If iStatus was completed with EFsmInputRequest, RPdpFsmInterface::Input is called with the parameters supplied in iFsmInputRequestData
 If iStatus was completed with ECleanup, or anything else, the active scheduler is stopped
 */
void CInputRequestListener::RunL()
	{
	TRequestStatus status = iStatus;
	iStatus = KRequestPending;
	
	if (status == EEtelDriverInputRequest     || 
	    status == EEtelDriverCancelPdpRequest ||
	    status == EFsmInputRequest            || 
	    status == EFsmObjectCreate)
		{
		switch (status.Int())
		    {
		    case EEtelDriverInputRequest:
		        {
    			// the main test thread requested a call to REtelDriverInput::Input
    			iEtelDriver->Input(iEtelDriverInputRequestData.iPdpId, iEtelDriverInputRequestData.iOperation); // error
    			break;
    			}
		    case EEtelDriverCancelPdpRequest:
		        {
    			iEtelDriver->CancelPdp(iEtelDriverCancelRequestPdpId);
    			break;
    			}
		    case EFsmInputRequest:
		        {
    			iPdpFsmInterface->Input(iFsmInputRequestData.iPdpId, iFsmInputRequestData.iOperation);
    			break;
    			}
		    case EFsmObjectCreate:
    			{
    			TInt err = iPdpFsmInterface->NewFsmContext(iFsmInputRequestData.iPdpId);

                // Reproduce completion  as per 'SpudManInterfaceTest::Input()' 
                // - since we're not being called asynchronously from EtelPkt
                
        		iFsmInputResponseData.iPdpId = iFsmInputRequestData.iPdpId;

        		TRequestStatus *readyStatus = &iFsmObjectCreateResponse;
        	 	iParentThread.RequestComplete(readyStatus, err);
        	 	break;
    			}
    		default:
    		    ASSERT(EFalse);
    		}

		// we are finished with the iInputData or iControlData parameters, so this signals to the main test thread
		// that this data can be updated with new parameters
		TRequestStatus *readyStatus = &iReadyForInputRequest;
	 	iParentThread.RequestComplete(readyStatus, KErrNone);
	 	
		SetActive(); 	
		}
	else
		{
		// the status was ECleanup, or we don't recognize the status, so abort the active scheduler, and the thread will be exited
		CActiveScheduler::Stop();
		}
	}
	
/**
 Creates the unit under test - the FSM if iUseTestPdpFsmInterface is false, and the EtelDriver otherwise
 */
void CInputRequestListener::CreateUnitUnderTestL()
	{
	if (iUseTestPdpFsmInterface)
		{
		CreateEtelDriverL();
		}
	else
		{
		CreatePdpFsmL();
		}
	}

/**
 Create and initialize the SPUDTEL
 */
void CInputRequestListener::CreateEtelDriverL()
	{
	iEtelDriver = NewEtelDriverInputL();
	
	iPdpFsmInterfaceTest = new(ELeave)CPdpFsmInterfaceTest(*this);

	iEtelDriver->OpenL(*iPdpFsmInterfaceTest);
	iPdpFsmInterfaceTest->OpenL(NULL, SimTsyName());
	
	for (TContextId i = 0; i < KMaxPdpContexts; i++)
		{
		iEtelDriver->CreatePdpL(i);
		}
	}

/**
 Create and initialize the SPUD FSM
 */
void CInputRequestListener::CreatePdpFsmL()
	{
	iPdpFsmInterface = new(ELeave)CPdpFsmInterface();
	iPdpFsmInterface->OpenL(iSpudManInterface, SimTsyName());
	}

/**
 Delete and cleanup and resources associated with SPUDTEL and SPUDFSM
 */
void CInputRequestListener::DeleteUnitUnderTest()
	{
	if (iEtelDriver)
		{
		iEtelDriver->Close();
		delete iEtelDriver;
		iEtelDriver = NULL;
		}
		
	if (iPdpFsmInterfaceTest)
		{
		iPdpFsmInterfaceTest->Close();
		delete iPdpFsmInterfaceTest;
		iPdpFsmInterfaceTest = NULL;
		}

	if (iPdpFsmInterface)
		{
		iPdpFsmInterface->Close();
		delete iPdpFsmInterface;
		iPdpFsmInterface = NULL;
		}
	}
