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
// PDP FSM factory implementation
// 
//

/**
 @file 
 @internalComponent
*/
 


#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "cpdpfsmfactoryTraces.h"
#endif

#include "cpdpfsmfactory.h"

// NewL 
CPdpFsmFactory* CPdpFsmFactory::NewL ()
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_NEWL_1, ">>CPdpFsmFactory::NewL()");
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_NEWL_2, "<<CPdpFsmFactory::NewL()");
	return new (ELeave) CPdpFsmFactory ();	
    }

#if defined(__VC32__)
#if (_MSC_VER >= 1200)
#pragma warning(disable:4355) 
#endif
#endif
//Cpdpfsmfactory.cpp(22...) : warning C4355: 'this' : used in base member initializer list
// these warnings are benign unless the designed usage of SpudFsm changes such that SpudFsm is used
// as a base class to something else, in which case this code will have to change IFF the this pointer
// passed is used in the states c'tor. Currently, the pointer is just stored.
// In V9, e32def.h turns this warning off.

CPdpFsmFactory::CPdpFsmFactory()
: iStateInitialised (this),
  iStateOpeningPhone (this),
  iStateCreatingPrimary (this),
  iStateActivatingPrimary (this),
  iStateCreatingSecondary (this),
  iStateCreatedSecondary (this),
  iStateSettingQoS (this),
  iStateSettingTFT (this),
  iStateActivatingSecondary (this),
  iStateOpen (this),
  iStateChangingQoS (this),
  iStateChangingTFT (this),
  iStateGettingNegQoS (this),
  iStateModifyingActive (this),
  iStateSuspended (this),
  iStateClosing (this),
  iStateStopping (this),
  iStateCreatingMbms(this),
  iStateActivatingMbms(this),
  iStateCreatedMbms(this)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_CTOR_1, ">>CPdpFsmFactory::CPdpFsmFactory()");
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_CTOR_2, "<<CPdpFsmFactory::CPdpFsmFactory()");
    }


CPdpFsmFactory::~CPdpFsmFactory()
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_DTOR_1, ">>CPdpFsmFactory::~CPdpFsmFactory()");

	iContexts.DeleteAll();
    iContexts.Reset();
	
    delete iEtelDriverInput;   
    OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_DTOR_2, "<<CPdpFsmFactory::~CPdpFsmFactory()");
    }


void CPdpFsmFactory::InitL(const TName& aTsyName, CPdpFsmInterface * aPdpFsmInterface)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_INITL_1, ">>CPdpFsmFactory::InitL()");

	iPdpFsmInterface = aPdpFsmInterface;
	
	iTsyName = aTsyName;

	// lets kick off the ETelDriver first then we only alloc FSMs if it works
	//
	iEtelDriverInput = new (ELeave) REtelDriverInput;

	iEtelDriverInput->OpenL (*iPdpFsmInterface);
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_INITL_2, "<<CPdpFsmFactory::InitL()");
	// Only create PDP contexts if specifically requested.
    }


void CPdpFsmFactory::Close (void)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_CLOSE_1, ">>CPdpFsmFactory::Close()");
	// in OOM conditions iEtelDriveInput may not have successfully
	// been created, check here for safety.
	if (iEtelDriverInput != NULL)
	    {
        iEtelDriverInput->Close();
	    }
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_CLOSE_2, "<<CPdpFsmFactory::Close()");
    }


/** 
@return may return NULL if there is no PDP context with that Id
*/
CPdpFsm* CPdpFsmFactory::GetFsmContext (TContextId aPdpId)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_GETFSMCONTEXT_1, ">>CPdpFsmFactory::GetContext()");
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_GETFSMCONTEXT_2, "<<CPdpFsmFactory::GetContext()");
	return iContexts[aPdpId];
    }


const TName& CPdpFsmFactory::TsyName(void)
    {
	return iTsyName;
    }

#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY	
TInt CPdpFsmFactory::NewFsmContext(TContextId aPdpId)
#else
TContextId CPdpFsmFactory::NewFsmContextL(MPdpFsmEventHandler& aPdpFsmEventHandler,SpudMan::TPdpContextType aContextType)
#endif
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_NEWFSMCONTEXTL_1, ">>CPdpFsmFactory::NewFsmContext()");

#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	TInt ret = KErrNone;
	ASSERT(ContextIdIsValid(aPdpId));
    if (!HaveFsmContext(aPdpId))
        {
	    // Only ever called by non-leaving functions. We're going to have to trap at some point in the calling hierarchy...
	    //
	    TRAP(ret,
	         CPdpFsm* p = CPdpFsm::NewL(aPdpId, this, iEtelDriverInput);
	         iContexts[aPdpId] = p;       
	         );
	    }
    OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_NEWFSMCONTEXTL_2, "<<CPdpFsmFactory::NewFsmContext()");
   return ret;
#else
	TInt i=0;
    while (i < KMaxPdpContexts && iContexts[i] != NULL)
    	{
	  	i++;
    	}
    if (i < KMaxPdpContexts)
        {
        CPdpFsm* p = CPdpFsm::NewL(i, this, iEtelDriverInput, aPdpFsmEventHandler, aContextType);
        iContexts[i] = p;
        iEtelDriverInput->CreatePdpL(i, aContextType);
        }
    OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_NEWFSMCONTEXTL_3, "<<CPdpFsmFactory::NewFsmContext()");
    return i;
#endif	
    }
   
TInt CPdpFsmFactory::DeleteFsmContext(TContextId aPdpId)
    {
	OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_DELETEFSMCONTEXT_1, ">>CPdpFsmFactory::DeleteFsmContext()");
	ASSERT(ContextIsValid(aPdpId));

    delete iContexts[aPdpId];
    iContexts[aPdpId] = NULL;
    OstTraceDef0(OST_TRACE_CATEGORY_DEBUG, TRACE_INTERNALS, CPDPFSMFACTORY_DELETEFSMCONTEXT_2, "<<CPdpFsmFactory::DeleteFsmContext()");
	return KErrNone;
    }
    
