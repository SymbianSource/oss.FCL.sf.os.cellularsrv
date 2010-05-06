// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Defines the class MRawIPNifObserverBase. All internal NIF observer mixins
// derive from this class.
// 
//

/**
 @file
*/

#ifndef MRAWIPNIFOBSERVERBASE_H__
#define MRAWIPNIFOBSERVERBASE_H__

class MNifIfNotify;
class CBcaIoController;
class CProtocolIfBase;

class MRawIPNifObserverBase
	{
public:
	enum TInitialisationState
		{
		EStart,
		EBcaController,
		EInitialised
		};

public:
	virtual void InitialiseL(TInitialisationState aState, TInt aError)=0;
	virtual void ShutDown(TInitialisationState aState, TInt aError=KErrNone)=0;

public:
	virtual MNifIfNotify* GetAgent()=0;
	virtual CProtocolIfBase* GetProtocolIf()=0;
	};

#endif // MRAWIPNIFOBSERVERBASE_H__
