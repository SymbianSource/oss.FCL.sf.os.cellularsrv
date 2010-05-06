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
// SPUD factory class and DLL entry point
// 
//

/**
 @file
 @internalComponent
*/

#include "spudfactory.h"
#include "spudman.h"

/**
Spud destructor.
*/
CSpudFactory::~CSpudFactory()
	{
	}

/**
This function is pure virtual in CNifFactory, so we have to define it
here. It is called by NifMan before NewInterfaceL().
*/
void CSpudFactory::InstallL()
	{
	}

/**
Factory function which creates an instance of the SPUD.

@param aName The name of the NIF
@param aNotify supplies details from "IfParams" field in CommDb
@return A newly-instantiated SPUD NIF
*/
CNifIfBase* CSpudFactory::NewInterfaceL(const TDesC& aName, MNifIfNotify* aNotify)
	{
	__FLOG_STATIC0(KSpudFirstTag, KSpudLog, _L("CSpudFactory::NewInterfaceL"));
   _LIT(KSpudDesc, "spud");
	if(aName.CompareF(KSpudDesc))
		{
		__FLOG_STATIC2(KSpudFirstTag, KSpudLog, _L("CSpudFactory::NewInterfaceL requested %S, not %S"), &aName, &KSpudDesc);
		User::Leave(KErrNotSupported);
		}
	
	// This is a complicated construction phase as many of the objects need to
	// contain pointers to several of the others.
	CSpudMan* spudMan = new (ELeave) CSpudMan(*this, aNotify);
	CleanupStack::PushL(spudMan);

	CSpudMux* mux = new (ELeave) CSpudMux(*spudMan);
	CleanupStack::PushL(mux);

	CSpudProtocol* protocol = new (ELeave) CSpudProtocol();
	CleanupStack::PushL(protocol);

	CBindMan* bindMan = new (ELeave) CBindMan(*spudMan, mux, protocol);
	// Ownership of mux and protocol have been transferred to bindMan
	CleanupStack::Pop(2);

	CleanupStack::PushL(bindMan);
	spudMan->ConstructL(bindMan);
	// Ownership of bindMan has been transferred to spudMan
	CleanupStack::Pop(bindMan);

	protocol->Construct(*bindMan);
	mux->Construct(*bindMan);

	CSpudBinderRef& primaryCtxBinder = *bindMan->GetNewRefForPrimaryL();
	
	// Read the protocol name from CommDB
	TName protoName;
	aNotify->ReadDes(TPtrC(SERVICE_IF_NETWORKS), protoName); // ignore error; will be caught in LoadNifL

    // SPUD only handles one protocol at a time
    ASSERT(protoName.Length() && protoName.Locate(',') == KErrNotFound);
	
	// Load a NIF to handle the primary context
	bindMan->LoadNifL(protoName, primaryCtxBinder);
	CleanupStack::Pop(spudMan);

	// Ownership of spudMan is passed to the caller
	return spudMan;
	}

/**
Factory function which creates an instance of the SPUD.
This version doesn't have the required notify pointer, so we unconditionally leave.

@param aName The name of the NIF
@return Never
@leave KErrNotSupported unconditionally
*/
CNifIfBase* CSpudFactory::NewInterfaceL(const TDesC& /*aName*/)
	{
	__FLOG_STATIC0(KSpudFirstTag, KSpudLog, _L("CSpudFactory::NewInterfaceL not supported without MNifIfNotify"));
    User::Leave(KErrNotSupported);
	return NULL;	// never reached
    }

/**
Return information about modules within this NIF.
This function is pure virtual in CNifFactory, so we have to define it here.
However, it doesn't do anything, and is never called by NifMan.

@param aInfo On successful return, contains information about a module within the NIF
@param aIndex An index into CNifIfBase/CNifIfLink-derived objects in the NIF, starting from 0.
 It is up to the NIF how it assigns indices to objects.
@return KErrNotSupported
*/
TInt CSpudFactory::Info(TNifIfInfo& /*aInfo*/, TInt /*aIndex*/) const
	{
	__FLOG_STATIC0(KSpudFirstTag, KSpudLog, _L("CSpudFactory::Info not supported"));
    ASSERT(EFalse);
	return KErrNotSupported;
	}


// Force export of non-mangled name
extern "C"
	{

/**
Polymorphic DLL entry point
*/
EXPORT_C CNifFactory *NewInterfaceFactoryL()
	{
	return new (ELeave) CSpudFactory;
	}

	}
