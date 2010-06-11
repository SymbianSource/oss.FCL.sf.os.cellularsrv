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
// The MSpudManInterface handle implementation.
// 
//

/**
 @file
 @internalComponent
*/

#include "RSpudManInterface.h"


RSpudManInterface::RSpudManInterface()
   :iSpudManImpl(NULL)
{
#ifdef __FLOG_ACTIVE
   // Logging tags
   _LIT8(KUmtsGprsLogTag1,"SubConn");
   _LIT8(KUmtsGprsLogTag2,"UmtsGprs");
	__FLOG_OPEN(KUmtsGprsLogTag1, KUmtsGprsLogTag2);
#endif
}


void RSpudManInterface::Init(MSpudManInterface* aSpudManImpl)
{
   __FLOG_2(_L("RSpudManInterface::Init: [this=%08x] [aSpudManImpl=%08x]"), this, aSpudManImpl);
   iSpudManImpl = aSpudManImpl;
}


void RSpudManInterface::Input(TContextId aContextId, TInt aEvent, TInt aParam)
{
   if (iSpudManImpl)
      {
      //ignoring calls to an unitialised handle - apparently there is no 
      //SpudMan.
      __FLOG_4(_L("RSpudManInterface::Input: Sending input. [this=%08x] [aContextId=%d] [aEvent=%d] [aParam=%d]"),
         this, aContextId, aEvent, aParam);
      iSpudManImpl->Input(aContextId, aEvent, aParam);        
      }
   else
      {
      __FLOG_4(_L("RSpudManInterface::Input: Discarding input. [this=%08x] [aContextId=%d] [aEvent=%d] [aParam=%d]"),
         this, aContextId, aEvent, aParam);
      }
}

void RSpudManInterface::SetContextTerminationErrorAndStop(TContextId aContextId, TInt aErrorCode)
{
   if (iSpudManImpl)
      {
      //ignoring calls to an unitialised handle - apparently there is no 
      //SpudMan.
      __FLOG_3(_L("SetContextTerminationErrorAndStop: [this=%08x] [aContextId=%d] [aErrorCode=%d]"),
         this, aContextId, aErrorCode);
      iSpudManImpl->SetContextTerminationErrorAndStop(aContextId, aErrorCode);        
      }
   else
      {
      __FLOG_3(_L("RSpudManInterface::SetContextTerminationErrorAndStop: Discarded! [this=%08x] [aContextId=%d] [aErrorCode=%d]"),
         this, aContextId, aErrorCode);
      }
}

void RSpudManInterface::Close ()
{
   __FLOG_1(_L("RSpudManInterface::Close: [this=%08x]"), this);
   
   if (iSpudManImpl)
      {
      // Clear the member first to avoid ping-ponging Close() messages
      // between the PdpFsmInterface and SpudManInterface
      MSpudManInterface* spudManImpl = iSpudManImpl;
      iSpudManImpl = NULL;
      spudManImpl->Input (0, KPdpFsmShuttingDown, 0);
   }
}
   
