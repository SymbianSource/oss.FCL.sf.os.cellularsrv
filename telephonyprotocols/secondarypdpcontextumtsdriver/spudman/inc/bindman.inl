// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef BINDMAN_INL
#define BINDMAN_INL

/**
Returns a pointer to the SpudMux object and transfers ownership.

@return Pointer to SpudMux (ownership is transferred)
*/
inline CSpudMux* CBindMan::TransferSpudMux()
	{
	ASSERT(iDeleteMux);
	iDeleteMux = EFalse;
	ASSERT(iMux);
	return iMux;
	}

/**
Returns a pointer to the SpudMux object.

@return pointer to SpudMux
*/
inline CSpudMux* CBindMan::SpudMux() const
	{
	ASSERT(iMux);
	return iMux;
	}

/**
Returns a pointer to the SpudMan object.

@return pointer to SpudMan
*/
inline CSpudMan* CBindMan::SpudMan() const
	{
	return &iSpudMan;
	}

/**
Returns a pointer to the SpudProtocol object.

@return pointer to SpudProtocol
*/
inline CSpudProtocol* CBindMan::SpudProtocol() const
	{
	return iSpudProtocol;
	}

/**
Removes the pointer to the mux object once NIFMAN has deleted it.
*/
inline void CBindMan::MuxClosed()
	{
	iMux = NULL;
	}

/**
Determines if this binder ref has been bound to a NIF yet.
If not, then it can be used for a new connection.

@return ETrue if object is already bound
*/
inline TBool CSpudBinderRef::IsBound() const
	{
	return iNifLink != NULL;
	}

/**
Determines if this object is a binder for the given lower NIF.
@return ETrue on a match
*/
inline TBool CSpudBinderRef::MatchBase(const CNifIfBase* aBase) const
	{
	return aBase == iNifBase;
	}

/**
Returns a pointer to the CNifIfLink object.

@return pointer to CNifIfLink
*/
inline CNifIfLink* CSpudBinderRef::NifLink() const
	{
	ASSERT(iNifLink);
	return iNifLink;
	}

/**
Returns a pointer to the CNifIfBase object.

@return pointer to CNifIfBase
*/
inline CNifIfBase* CSpudBinderRef::NifBase() const
	{
	ASSERT(iNifBase);
	return iNifBase;
	}

/**
Returns a pointer to the CSpudNotify object.

@return pointer to CSpudNotify
*/
inline CSpudNotify* CSpudBinderRef::Notify() const
	{
	ASSERT(iNotify);
	return iNotify;
	}

/**
Returns the state of the associated PDP context.

@return state
*/
inline TSpudContextStates CSpudBinderRef::State() const
	{
	return iState;
	}

/**
Sets the state of the associated PDP context.

@param new state
*/
inline void CSpudBinderRef::SetState(TSpudContextStates aState)
	{
	// Binder must not be touched once it is designated for deletion, else we can leak it.
	ASSERT(ESpudWaitBinderDelete != iState);
	iState = aState;	
	}

/**
Stores an error code detailing cause of context failure.

@param error code
*/
inline void CSpudBinderRef::SetError(TInt aError)
    {
    iError = aError;
    }

/**
Returns the context failure error
*/
inline TInt CSpudBinderRef::Error() const
    {
    return iError;
    }

/**
Binds the binder object to the lower NIF.
*/
inline void CSpudBinderRef::Bind(CNifIfLink* aNifLink, CNifIfBase* aNifBase)
	{
	iNifBase = aNifBase;
	iNifLink = aNifLink;
	ASSERT(iNifBase);
	ASSERT(iNifLink);
	}

#endif
