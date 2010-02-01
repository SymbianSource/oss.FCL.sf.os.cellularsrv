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
// implementation of etel driver input
// 
//

/**
 @file 
 @internalComponent
*/

#include <e32def.h>
#include <e32std.h>

#include "ceteldriverfactory.h"
#include "ceteldrivercontext.h"
#include "reteldriverinput.h"
#include "spudteldebuglogger.h"

using namespace EtelDriver;

REtelDriverInput::REtelDriverInput()
: iDriverFactory(NULL)
	{
	}


REtelDriverInput::~REtelDriverInput()
	{
	SPUDTEL_FNLOG("REtelDriverInput::~REtelDriverInput()");
	
	if (iDriverFactory)
	   {
	   delete iDriverFactory;
	   }	  
	}


/** requests to open etel driver

@param aPdpFsmInterface - reference to pdp FSM interface
*/
void REtelDriverInput::OpenL (CPdpFsmInterface& aPdpFsmInterface)
	{
	SPUDTEL_FNLOG("REtelDriverInput::OpenL()");

	// internal check
	iDriverFactory = CEtelDriverFactory::NewL(aPdpFsmInterface);
	if (iDriverFactory)
		{
		iDriverFactory->InitL();
		}
	}


/** closes eTel driver and frees underlying resources
*/
void REtelDriverInput::Close()
	{
	SPUDTEL_FNLOG("REtelDriverInput::Close()");
	
	// cancel all outstanding requests
	if (iDriverFactory)
		{
		CancelAllPdps();
		}

	delete iDriverFactory;
	iDriverFactory = NULL;
	}
	
/** requests to create new pdp context
	Note: this request will not open new primary or secondary context
		  But rather allocate necessary resources to do it.
		  Creation / deletion of eTel contexts should be done though Input method.

@param aPdpId - id of new pdp context
*/
void REtelDriverInput::CreatePdpL (TContextId aPdpId,SpudMan::TPdpContextType aContextType)
	{
	SPUDTEL_FNLOG("REtelDriverInput::CreatePdpL()");
	SPUDTELVERBOSE_INFO_LOG1(_L("pdp id : %d"), aPdpId);
	
	ASSERT(iDriverFactory);
	iDriverFactory->CreatePdpL (aPdpId,aContextType );
	}
	
/** deletes pdp context

@param aPdpId - id of a pdp context
*/
void REtelDriverInput::DeletePdp (TContextId aPdpId)
	{
	SPUDTEL_FNLOG("REtelDriverInput::DeletePdp()");
	SPUDTELVERBOSE_INFO_LOG1(_L("pdp id : %d"), aPdpId);
	
	if(iDriverFactory)
    	{
    	iDriverFactory->FreePdp(aPdpId);	    
    	}
	}

/** requests to perform aOperation for aPdpId context

@param aPdpId - id of a pdp context
@param aOperation - etel driver operation
*/
void REtelDriverInput::Input(TContextId aPdpId, TEtelInput aOperation)
	{
	SPUDTEL_FNLOG("REtelDriverInput::Input()");
	SPUDTEL_INFO_LOG2(_L("REtelDriverInput::Input: pdp id : %d, operation %d"), aPdpId, aOperation);
	ASSERT(iDriverFactory);
	
	iDriverFactory->Context(aPdpId).Input(aOperation);
	}

/** cancels last operation for aPdpId context 

@param aPdpId - id of a pdp context
*/
void REtelDriverInput::CancelPdp (TContextId aPdpId)
	{
	SPUDTEL_FNLOG("REtelDriverInput::CancelPdp()");
	SPUDTEL_INFO_LOG1(_L("REtelDriverInput::CancelPdp: pdp id : %d"), aPdpId);
	ASSERT(iDriverFactory);
	
	iDriverFactory->Context(aPdpId).Cancel();
	}
	
/** starts network notifications for aPdpId context 

@param aPdpId - id of a pdp context
*/
void REtelDriverInput::StartPdpNotifications(TContextId aPdpId)
	{
	SPUDTEL_FNLOG("REtelDriverInput::StartPdpNotifications()");
	ASSERT(iDriverFactory);

	iDriverFactory->StartPdpNotifications(aPdpId);
	}
	
/** cancels network notifications for aPdpId context 

@param aPdpId - id of a pdp context
*/
void REtelDriverInput::CancelPdpNotifications (TContextId aPdpId)
	{
	SPUDTEL_FNLOG("REtelDriverInput::CancelPdpNotifications()");
	SPUDTEL_INFO_LOG1(_L("REtelDriverInput::CancelPdpNotifications: pdp id : %d"), aPdpId);
	ASSERT(iDriverFactory);

	iDriverFactory->CancelPdpNotifications (aPdpId);
	}
	
/** cancels last operations for all created pdp contexts */
void REtelDriverInput::CancelAllPdps ()
	{
	SPUDTEL_FNLOG("REtelDriverInput::CancelAllPdps()");
	ASSERT(iDriverFactory);
	
	for(TContextId i = 0; i < static_cast<TContextId>(iDriverFactory->ContextCount()); i++)
		{
		if (iDriverFactory->HasContext(i))
		    {
    		iDriverFactory->Context(i).Cancel();
		    }
		}
	}

/** cancels all notifications for all pdp contexts */
void REtelDriverInput::CancelAllPdpNotifications ()
	{
	SPUDTEL_FNLOG("REtelDriverInput::CancelAllPdpNotifications()");
	ASSERT(iDriverFactory);

	iDriverFactory->CancelAllPdpNotifications();
	}


