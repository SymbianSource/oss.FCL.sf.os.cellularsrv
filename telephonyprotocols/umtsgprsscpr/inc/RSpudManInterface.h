// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The MSpudManInterface handle class definition.
// 
//

/**
 @file
 @internalComponent
*/
 


#ifndef RSPUDMANINTERFACE_H
#define RSPUDMANINTERFACE_H

#include <networking/mspudmaninterface.h>
#include <comms-infras/commsdebugutility.h>


/**
Client side implementation of MSpudManInterface. 
This is a very simplistic implementation that just holds a refernce to
the server side and forwards all the calls to it. This implementation
can be changed in the future to facilitate for IPC.
*/
class RSpudManInterface : public MSpudManInterface
{
public:
   RSpudManInterface();
   void Init(MSpudManInterface* aSpudManImpl);
   void Close ();
   virtual void Input(TContextId aContextId, TInt aEvent, TInt aParam);

   virtual void SetContextTerminationErrorAndStop(TContextId aContextId, TInt aErrorCode);

   inline ~RSpudManInterface ()
      {
   	__FLOG_CLOSE;
      }
   

private:
   MSpudManInterface* iSpudManImpl;

public:
	__FLOG_DECLARATION_MEMBER;
};

#endif
// RSPUDMANINTERFACE_H
