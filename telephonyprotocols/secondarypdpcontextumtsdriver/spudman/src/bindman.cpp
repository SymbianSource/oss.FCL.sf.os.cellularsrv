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
// SPUD binder manager
// 
//

/**
 @file
 @internalComponent
*/

#include "bindman.h"
#include "spudnotify.h"
#include "mux.h"
#include <nifman.h>

/** Default name of lower NIF */
_LIT(KDefaultLowerNifName, "rawip");

#ifdef __FLOG_ACTIVE
#define BINDMAN_LOG(x) SpudMan()->x
#else
#define BINDMAN_LOG(x)
#endif

//*****************************************************************************
// CBindMan
//*****************************************************************************

/**
Constructs BindMan. Ownership of aMux and aProto is transferred to this object.

@param aSpudMan Reference to SpudMan object
@param aMux Pointer to SpudMux object (ownership is transferred)
@param aProto Pointer to SpudProtocol object (ownership is transferred)
*/
CBindMan::CBindMan(CSpudMan& aSpudMan, CSpudMux* aMux, CSpudProtocol* aProto)
	: iSpudMan(aSpudMan), iMux(aMux), iDeleteMux(ETrue), iSpudProtocol(aProto)
    {
    ReadLowerNifName();
	}

CBindMan::~CBindMan()
    {
    BINDMAN_LOG(__FLOG(_L("BindMan::~BindMan")));
    
    #ifdef __FLOG_ACTIVE // Log for diagnostic purposes
    TInt idx;
    for(idx = 0; idx < iBinders.Count(); ++idx)
    	{
    	if(iBinders[idx] && iBinders[idx]->IsBound())
			{
			BINDMAN_LOG(__FLOG_1(_L("~BindMan: deleting bound lower NIF binding for context[%d]"), idx));			
   			}
    	}
    #endif

    iBinders.DeleteAll();
    delete iSpudProtocol;
    if (iDeleteMux)
    	{
    	delete iMux;
    	}
	}

/**
Reads the lower NIF name from CommDb.

@todo This function incorrectly returns the default value when IfParams is of the form:
 "notlowernif=UNWANTED,lowernif=IGNORED".
 This defect is also present in CPppLcp::InitL.
*/
void CBindMan::ReadLowerNifName(void)
	{
	TBuf<KCommsDbSvrMaxFieldLength> params;
	_LIT(KInterfaceIdentifier,"lowernif=");
	const TInt KLinkEqualsLength = 9;	// length of "lowernif="
	ASSERT(static_cast<const TDesC&>(KInterfaceIdentifier).Length() == KLinkEqualsLength);

	// Read IfParams field
	iSpudMan.Notify()->ReadDes(TPtrC(ISP_IF_PARAMS), params);

	TInt paramStartMarker = params.Find(KInterfaceIdentifier);

	if (paramStartMarker == KErrNotFound)
		{
		// "lowernif=" not found in the string - use default lower layer
		iLowerNifName = KDefaultLowerNifName;
		BINDMAN_LOG(__FLOG_1(_L("Lower NIF name not configured; defaulting to %S"), &iLowerNifName));
		}
	else
		{
		// "lowernif=<nifname>" found in the string - extract the lower layer Nif name

		if (paramStartMarker > 0 && params[paramStartMarker-1] != ',')
			{
			// "xyzlowernif=<xxx>" found rather than "lowernif=<xxx>" - use default lower layer
			iLowerNifName = KDefaultLowerNifName;
			BINDMAN_LOG(__FLOG_1(_L("Lower NIF name not configured; defaulting to %S"), &iLowerNifName));
			}
		else
			{
			// String of the form "[...,]lowernif=<nifname>[,...]" found - extract the Nif name
			TPtrC postfix(params.Mid(paramStartMarker + KLinkEqualsLength));
			TInt comma = postfix.Locate(',');
			if (comma != KErrNotFound)
				{
				postfix.Set(postfix.Ptr(), comma);
				}
			iLowerNifName = postfix;
			}
		}
	}

/**
Informs CSpudProtocol of the CProtocolBase pointer from the L3 protocol.
*/
void CBindMan::SetProtocolBaseL(CProtocolBase* aProtocolBase) const
	{
	iSpudProtocol->SetProtocolBaseL(aProtocolBase);
	}

/**
Returns the binder for the given context ID.
@param aContextId context ID
@return Pointer to binder object
@leave KErrBadHandle or KErrNotReady if context ID is invalid or not found
*/
CSpudBinderRef* CBindMan::GetRefL(TContextId aContextId) const
    {
    if (aContextId < 0 || aContextId >= iBinders.Count())
    	{
		BINDMAN_LOG(__FLOG_1(_L("CBindMan::GetRefL invalid context ID %d"), aContextId));
	    User::Leave(KErrBadHandle);
    	}
	CSpudBinderRef* ref = iBinders[aContextId];
	if (ref == NULL)
		{
		// This situation can frequently occur when looping through all binders
	    User::Leave(KErrNotReady);
		}
	return ref;
	}

/**
Returns the binder for any lower NIF that is available and bound.
@return Pointer to binder object
@leave KErrNotReady if no lower NIF is found
*/
CSpudBinderRef* CBindMan::GetAnyRefL() const
	{
    TInt index;
    for (index=0; index < iBinders.Count(); ++index)
    	{
    	CSpudBinderRef* ref = iBinders[index];
    	if (ref && ref->IsBound() && ref->State() != ESpudWaitBinderDelete)
    		{
    		return ref;
    		}
    	}
	BINDMAN_LOG(__FLOG_0(_L("CBindMan::GetAnyRefL Can't find any bound NIF")));
    User::Leave(KErrNotReady);
    return NULL;	// never reached
	}

/**
Returns the context ID the given lower NIF.
@param aNifBase pointer to lower NIF
@return context ID
@leave KErrNotReady if lower NIF is not found
*/
TContextId CBindMan::FindContextIdL(const CNifIfBase* aNifBase) const
	{
    TContextId index;
    for (index=0; index < iBinders.Count(); ++index)
    	{
    	CSpudBinderRef* ref = iBinders[index];
    	if (ref && ref->MatchBase(aNifBase))
    		{
    		return index;
    		}
    	}
	BINDMAN_LOG(__FLOG_1(_L("CBindMan::FindContextIdL Can't find aNifBase %x"), aNifBase));
    User::Leave(KErrNotReady);
    return KAllContexts;	// never reached
	}

/**
Returns the number of bound contexts.
@return Number of bound contexts
*/
TUint CBindMan::NumContexts() const
	{
    TInt index;
    TUint count = 0;
    for (index=0; index < iBinders.Count(); ++index)
    	{
    	CSpudBinderRef* ref = iBinders[index];
    	if (ref && ref->IsBound())
    		{
    		++count;
    		}
    	}
	return count;
	}
	
/**
Checks if this is is the last 'valid' context in SPUD, i.e. it is bound and not marked for deletion.

@param aContextID the ID of the context to check
@return ETrue if and only if aContextId (the subject bindre) is the only valid, bound, unmarked binder.
*/
TBool CBindMan::IsLastContext(TContextId aContextId)const
	{
	ASSERT(aContextId >= 0 && NumContexts() > 0); // we shouldn't be called unless we have at least 1 ctx
	
	TUint numLiveBoundBinders(0); 
	TBool isTargetBinderFound(EFalse); // The binder for the subject context exists, is bound, and not marked for deletion.
	
	TInt binderIdx;
	for(binderIdx = 0; binderIdx < iBinders.Count(); ++binderIdx) // sift through all the binders
		{
		if(iBinders[binderIdx] && // binder exists
		   iBinders[binderIdx]->IsBound() &&  // is bound
		   ESpudWaitBinderDelete != iBinders[binderIdx]->State() // not marked for deletion
		)
			{
			++numLiveBoundBinders;
			
			if(binderIdx == aContextId)
				{
				isTargetBinderFound = ETrue;
				}											
			}
		}
	ASSERT(isTargetBinderFound); // sanity check: we have a referene to the subject binder,
	                             // and it is 'valid': bound and not marked for deletion.	
	
	if(0 == numLiveBoundBinders) // No valid bound binders remain.
		{
		ASSERT(EFalse); // spudman thinks it has valid binders, when there are none.
		
		// In release builds, we can't say that there are more binders, because we have no
		// references to them. 
		return ETrue; // EFalse implies that we have at least 1 binder
		}
	else if(1 == numLiveBoundBinders) 
		{
		// In release builds: if live binder is not the subject context, we can't claim that the 
		// subject context is the last one, or SPUD may shutdown when it has a context.
		return isTargetBinderFound; // Is there another valid binder beside the subject?
		}
	else // > 1 
		{
		// Regardless of whether the subject context was found, we have more.
		return EFalse;
		}
	}
	
/**
Creates and returns a pointer to a new binder object. Ownership remains with BindMan;
DeleteRef() must be called to delete the binder reference object.

@param aContextId Holds the new context ID on return
@return Pointer to binder object
@leave KErrNotSupported if we are out of PDP contexts, or KErrNoMemory if out of RAM
*/
CSpudBinderRef* CBindMan::GetNewRefForSecondaryL(TContextId& aContextId)
    {
    // Reusing the slot of the primary context is not allowed.
    static const TContextId KFirst2ndaryCtxIdx(KPrimaryContextId + 1);
    TContextId index;
    for (index = KFirst2ndaryCtxIdx; index < iBinders.Count(); ++index)
    	{
    	CSpudBinderRef*& ref = iBinders[index];
    	if (ref == NULL)
    		{
    		// Create a binder in this new slot
    		aContextId = index;
    		// ref is a reference to a pointer that is owned by Bindman. It will be deleted in the
    		// destructor. There is no need for CleanupStack.
    		ref = new (ELeave) CSpudBinderRef(*iSpudMan.BindMan());
    		ref->ConstructL(iSpudMan.Notify(), aContextId);
    		return ref;
    		}
    	else if (!ref->IsBound())
			{
			// This binder was previously constructed but unbound
			ASSERT(ref->Notify() == iSpudMan.Notify());
    		aContextId = index;
    		return ref;
			}
    	}
	BINDMAN_LOG(__FLOG_0(_L("CBindMan::GetNewRefForSecondaryL No room for new binder reference")));
    User::Leave(KErrNotSupported);
    return NULL;	// never reached
	}



/** 
Creates and returns a pointer to the new binder for the Primary PDP context.
The primary is a special case, because it can be created only once, and it uses a special
ID = 0.  If the primary fails then a secondary can be promoted to primary status 
even in this case ID = 0 is not re-used because GetNewRefForSecondaryL prevents this.
This means that KPrimaryContextId is only significant once at startup time!

Ownership remains with BindMan;
DeleteRef() must be called to delete the binder reference object.

@return the pointer to the binder object for the primary pdp context.
@leave if the construction of the binder leaves.
*/
CSpudBinderRef* CBindMan::GetNewRefForPrimaryL()
	{
	CSpudBinderRef*& primRef = iBinders[KPrimaryContextId];
	if(primRef) // We must not try to create the primary more than once.
		{
		ASSERT(EFalse);
		User::Leave(KErrAlreadyExists); // Release builds
		}
	// primRef is a reference to a pointer that is owned by Bindman. It will be deleted in the
    // destructor. There is no need for CleanupStack.
	primRef = new (ELeave) CSpudBinderRef(*iSpudMan.BindMan());
	primRef->ConstructL(iSpudMan.Notify(), KPrimaryContextId);
	return primRef;
	}



/**
Loads a new lower NIF and bind to the binder ref

@param aName Protocol name desired
@param aBinder the binder to bind the lower NIF into.
@see CNifAgentRef::ServiceStarted()
*/
void CBindMan::LoadNifL(const TDesC& aName, CSpudBinderRef& aBinder)
	{
	BINDMAN_LOG(__FLOG_2(_L("CBindMan::LoadNifL loading NIF %S for protocol %S"), &iLowerNifName, &aName));
	
	CNifIfLink* lowerNif=static_cast<CNifIfLink*>(Nif::CreateInterfaceL(iLowerNifName, aBinder.Notify()));

	// Increment reference count
	lowerNif->Open();

	CleanupClosePushL(*lowerNif);
	// Create CNifIfBase and bind it
	// Maybe binding should be left to CSpudBinderRef
	CNifIfBase* nifBase = lowerNif->GetBinderL(aName);
	CleanupStack::Pop();
	
	// Increment reference count
	nifBase->Open();
	
	// Bind the lower NIF into the SPUD
	aBinder.Bind(lowerNif, nifBase);
	
	// Bind the lower NIF to SpudProtocol
	nifBase->BindL(iSpudProtocol);
	}


/** Deletes the dead (marked for deletion) references to lower NIF bindings..

@return the number of contexts remaining after the deletion
*/
TUint CBindMan::SweepBinders()
	{
	TUint liveContexts(0);
	TInt binderIdx;
	for(binderIdx = 0; binderIdx < iBinders.Count(); ++binderIdx) // Sift through all the binders
		{
		if(iBinders[binderIdx] && iBinders[binderIdx]->IsBound()) // Binder exists and is bound to a lower NIF 
			{
			if(ESpudWaitBinderDelete == iBinders[binderIdx]->State()) //Must be deleted
				{
				BINDMAN_LOG(__FLOG_1(_L("CBindMan::SweepBinders: deleting binder for context[%d]"), binderIdx));
				delete iBinders[binderIdx];
				iBinders[binderIdx] = NULL;
				}
			else // Binder exists and is bound, but is not eligible for deletion. 
				{
				BINDMAN_LOG(__FLOG_1(_L("CBindMan::SweepBinders: context[%d] is alive."), binderIdx));
				++liveContexts;
				}
			}
		// Binder is not bound. We don't care. It will be taken care of in the destructor.
		}		 
	return liveContexts; 
	}

//*****************************************************************************
// CSpudBinderRef
//*****************************************************************************


CSpudBinderRef::CSpudBinderRef(CBindMan& aBindMan)
	:iBindMan(aBindMan)
	{
	}

CSpudBinderRef::~CSpudBinderRef()
	{
	if (IsBound())
		{
	   	// This call causes the NIF base to delete itself
	   	iNifBase->Close();

	   	// This call causes the NIF link to delete itself
	   	iNifLink->Close();
		}

   	delete iNotify;
	}

/**
Creates a SpudNotify object.
@leave KErrNoMemory if out of RAM
*/
void CSpudBinderRef::ConstructL(MNifIfNotify* aNotify, TContextId aContextId)
	{
	iNotify = new (ELeave) CSpudNotify(iBindMan, aNotify, aContextId);
	}
