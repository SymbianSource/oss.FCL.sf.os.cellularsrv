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
// The implementation of the SpudManInterface for SPUD FSM unit tests
// 
//

/**
 @file 
 @internalComponent
*/

#include "InputRequestListener.h"
#include <etelpckt.h>


TSpudManInterfaceTest::TSpudManInterfaceTest(CInputRequestListener& aListener) : iListener(aListener)
	{}

/**
 Signals to the test execute thread that MSpudManInterface::Input has been called
 */
void TSpudManInterfaceTest::Input(TContextId aPdpId, TInt aEvent, TInt aParam)
	{
	if (aEvent == KPdpFsmShuttingDown)
	   {
	   // This event requires no notification - In the real SpudMan it would cause SpudMan
	   // to close down (NULL) the pointer to the PdpFsmInterface provided by the UmtsGprsSCPR.
	   return;
	   }
	
	// The aOperation parameters are broken up into those that are notifications and those that aren't
	// Since notifications can happen anytime, separate request status members need to be kept for each one
	// Other input requests happen serially, so only one request can be pending at any time
	TRequestStatus *status = NULL;
	
    // Here, rather than in the case statement due to rvct compiler warning
#ifdef SYMBIAN_NETWORKING_UMTSR5
    RPacketQoS::TQoSR5Negotiated neg;
#else
    RPacketQoS::TQoSR99_R4Negotiated neg;
#endif
// SYMBIAN_NETWORKING_UMTSR5

	switch (aEvent)
		{
	case KContextParametersChangeEvent:
		iListener.iContextParametersEventData.iPdpId = aPdpId;
		iListener.iPdpFsmInterface->Get(aPdpId, neg);
		iListener.iContextParametersEventData.iNegotiated = neg;

		iListener.iPdpFsmInterface->Get(aPdpId, iListener.iContextParametersEventData.iContextConfig);
		status = &iListener.iFsmContextParameterChangeStatus;
		break;
	case KNetworkStatusEvent:
		status = &iListener.iFsmNetworkStatusStatus;
		break;
	case KContextBlockedEvent:
		iListener.iContextBlockedEventPdpId = aPdpId;
		status = &iListener.iFsmContextSuspendedStatus;
		break;
	case KContextUnblockedEvent:
		iListener.iContextUnblockedEventPdpId = aPdpId;
		status = &iListener.iFsmContextResumedStatus;
		break;
	default:
		iListener.iFsmInputResponseData.iPdpId = aPdpId;
		iListener.iFsmInputResponseData.iEvent = aEvent;
		iListener.iFsmInputResponseData.iParam = aParam;
		status = &iListener.iFsmInputResponseStatus;
		break;
		}
	iListener.iParentThread.RequestComplete(status, KErrNone);	
	}

void TSpudManInterfaceTest::SetContextTerminationErrorAndStop(TContextId /*aContextId*/, TInt /*aErrorCode*/)
   {/*Placeholder for unused function*/}
