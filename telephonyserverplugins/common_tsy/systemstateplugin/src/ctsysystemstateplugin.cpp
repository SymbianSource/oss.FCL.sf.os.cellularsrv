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
//

#include "ctsysystemstateplugin.h"
#include <ctsy/tflogger.h>

CCtsySystemStatePlugin* CCtsySystemStatePlugin::NewL()
	{
	CCtsySystemStatePlugin* self = new (ELeave)CCtsySystemStatePlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 	
	return self;
	}

CCtsySystemStatePlugin::CCtsySystemStatePlugin()
	{	
	}
	
void CCtsySystemStatePlugin::ConstructL()
	{	
	// create active object for do RF state changing  
	iAoChangeRfForEmergencyCall = new (ELeave) CChangeRfForEmergencyCall(*this, iSsmEmergencyCallRf);
	User::LeaveIfError(Connect());
	}

CCtsySystemStatePlugin::~CCtsySystemStatePlugin()
	{  
	if (iAoChangeRfForEmergencyCall)
		{
		delete iAoChangeRfForEmergencyCall;
		}
	}	
	

// ---------------------------------------------------------------------------
// CCtsySystemStatePlugin::Connect
// Try to connect to RSsmEmergencyCallRfAdaptation
// returns KErrNone if success, otherwise -- error code
// ---------------------------------------------------------------------------
TInt CCtsySystemStatePlugin::Connect()
	{	
	TFLOGSTRING("CCtsySystemStatePlugin::Connect()");		

	TInt error (KErrNone);
	if (!iConnected)	 
	{	 
        error = iSsmEmergencyCallRf.Connect();  
        if (KErrNone != error)
            {
            return error; 
            }
        iConnected = ETrue; 
        error = iSsmEmergencyCallRf.SetAsPriorityClient();
		if (KErrNone != error)
			{
            TFLOGSTRING2("CCtsySystemStatePlugin::Connect()  - SetAsPriorityClient returned %d", error);
            error = KErrNone;
			}
	}	
	return error;
	}

// ---------------------------------------------------------------------------
// CCtsySystemStatePlugin::ActivateRfForEmergencyCall
// Calls  RSsmEmergencyCallRfAdaptation::ActivateRfForEmergencyCall
// returns KErrNotReady if there is no connection to CCtsySystemStatePlugin,
// otherwise -- error code returned from CCtsySystemStatePlugin
// ---------------------------------------------------------------------------
void CCtsySystemStatePlugin::ActivateRfForEmergencyCall(MCtsySsmPluginCallback* aSsmPluginCallback,
                                                        TCtsySsmCallbackData& aCallbackData)
	{
	TFLOGSTRING("CCtsySystemStatePlugin::ActivateRfForEmergencyCall()" );

	iAoChangeRfForEmergencyCall->ActivateRfForEmergencyCall(aSsmPluginCallback, aCallbackData);	
	}
	
// ---------------------------------------------------------------------------
// CCtsySystemStatePlugin::DeactivateRfAfterEmergencyCall
// Calls  RSsmEmergencyCallRfAdaptation::DeactivateRfAfterEmergencyCall
// returns KErrNotReady if there is no connection to CCtsySystemStatePlugin,
// otherwise -- error code returned from CCtsySystemStatePlugin
// ---------------------------------------------------------------------------
void CCtsySystemStatePlugin::DeactivateRfAfterEmergencyCall()
	{	
	TFLOGSTRING("CCtsySystemStatePlugin::DeactivateRfForEmergencyCall()" );		
	//try to connect
	TInt err = Connect();
	if (err)
		{
	    TFLOGSTRING2("CCtsySystemStatePlugin::DeactivateRfAfterEmergencyCall() failed to connect %d", err);
		//do nothing
		return;
		}
				
	iAoChangeRfForEmergencyCall->DeactivateRfAfterEmergencyCall();
	}
	
// ---------------------------------------------------------------------------
// CCtsySystemStatePlugin::Close
// Close connection to RSsmEmergencyCallRfAdaptation
// ---------------------------------------------------------------------------	
void CCtsySystemStatePlugin::Close()
	{			
	TFLOGSTRING("CCtsySystemStatePlugin::Close()" );				
	if (iConnected)	
		{	
		// if there is some request...
		if (CChangeRfForEmergencyCall::ENotBusy != iAoChangeRfForEmergencyCall->Busy())
			{
			// ... cancel it
			iSsmEmergencyCallRf.Cancel(); 			
			}
					
		iSsmEmergencyCallRf.Close(); 	
		iConnected = EFalse;				
		}
	}
	
	
	
	

// ---------------------------------------------------------------------------	
//  Active object for changing RF State
// ---------------------------------------------------------------------------	
CChangeRfForEmergencyCall::CChangeRfForEmergencyCall(CCtsySystemStatePlugin& aCtsySystemStatePlugin, 
													 RSsmEmergencyCallRfAdaptation& aSsmEmergencyCallRf) :
			                                         CActive(EPriorityStandard),
			                                         iCtsySystemStatePlugin(aCtsySystemStatePlugin), 
			                                         iSsmEmergencyCallRf(aSsmEmergencyCallRf)
   {
   CActiveScheduler::Add(this);
   }

// ---------------------------------------------------------------------------  
//  Destructor for Active object
// ---------------------------------------------------------------------------	
CChangeRfForEmergencyCall::~CChangeRfForEmergencyCall()
		{
		Cancel();
		}
	
// ---------------------------------------------------------------------------  
//  Active object ActivateRfForEmergencyCall
// ---------------------------------------------------------------------------  
void CChangeRfForEmergencyCall::ActivateRfForEmergencyCall(MCtsySsmPluginCallback* aSsmPluginCallback,
														   TCtsySsmCallbackData& aCallbackData)
   {	
   TFLOGSTRING("CChangeRfForEmergencyCall::ActivateRfForEmergencyCall()" );     

   if (EDeactivateBusy == iBusy)
      {
      // cancel any request
	  Cancel();
	  iBusy = ENotBusy;				
      }
   else if (EActivateBusy == iBusy)
      {
      if (aSsmPluginCallback)
          {
		  aSsmPluginCallback->SsmPluginCallback(KErrServerBusy, aCallbackData);
          }
	  return;
      }
		
   // store pointer to callback interface
   iSsmPluginCallback = aSsmPluginCallback;
		
   // copy data for callback
   iCallbackData.iReqHandle = aCallbackData.iReqHandle;		
   RMobileENStore::TEmergencyNumber* number;
   aCallbackData.iNumberPackage.UnPackData(&number);				
   iCallbackData.iNumberPackage.PackData(number);
	
   iSsmEmergencyCallRf.ActivateRfForEmergencyCall(iStatus);
   iBusy = EActivateBusy;
	
   SetActive();
   }
	
// ---------------------------------------------------------------------------  
//  Active object DeactivateRfAfterEmergencyCall
// --------------------------------------------------------------------------- 
void CChangeRfForEmergencyCall::DeactivateRfAfterEmergencyCall()
   {	
   TFLOGSTRING("CChangeRfForEmergencyCall::DeactivateRfAfterEmergencyCall()" );     

		// if we've already sent request  
		if (EActivateBusy == iBusy)
			{
			// cancel any request
			Cancel();
			iBusy = ENotBusy;
			iSsmPluginCallback  = NULL; // to prevent calling of callback (but I think there shoul be Cancel)		
			return;			
			}
		else if (EDeactivateBusy == iBusy)							
			{
			// do nothing
			return;
			}
			
		iSsmEmergencyCallRf.DeactivateRfForEmergencyCall(iStatus);
		iBusy = EDeactivateBusy;
	
		SetActive();		
		}
// ---------------------------------------------------------------------------  
//  Active object Busy method
// --------------------------------------------------------------------------- 		
TBool CChangeRfForEmergencyCall::Busy()
   {	
   return iBusy;
   }

// ---------------------------------------------------------------------------  
//  RunL for Active object
// ---------------------------------------------------------------------------  	
void CChangeRfForEmergencyCall::RunL()
   {	
   iBusy = ENotBusy;		
   
   // close SSM
   if (iSsmPluginCallback)
      {
	  // just call callback interface and pass into it error code
	  iSsmPluginCallback->SsmPluginCallback(iStatus.Int(), iCallbackData);
      iSsmPluginCallback = NULL;	
      }	
   }

// ---------------------------------------------------------------------------  
//  Cancel for Active object
// --------------------------------------------------------------------------- 		
void CChangeRfForEmergencyCall::DoCancel()
   {
   iSsmEmergencyCallRf.Cancel(); 
   iBusy = ENotBusy;
   }

