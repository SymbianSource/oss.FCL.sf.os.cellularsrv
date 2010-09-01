// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CFAX32.H"
#include "dial.h"	                      // we no longer depend on DialStor, so must depend directly on Dial

#include "FAXLOG.H"
#include "faxsettings.h"

 #include <commsdattypesv1_1.h>
 #include <commsdatutils.h>
 #include <commsdat_partner.h>

 using namespace CommsDat;

TInt FaxClientThread (TAny * session);  // function declaration needed here
//TInt FaxConverterThread (TAny *);
//RSemaphore jerry;
/********************************************************************/


/********************************************************************/

CFaxTransfer::CFaxTransfer()
	: CBase()
	{}

EXPORT_C CFaxTransfer *CFaxTransfer::NewLC (const TFaxSettings & aFaxSettings)
/** Constructs a CFaxTransfer object, which offers the publicly exported 
Symbian OS Fax Client API. 
As is usual in Symbian OS, the only difference between this function and 
NewL() is that this variant pushes the object to the cleanup stack.

@param aFaxSettings  A reference to a TFaxSettings object which contains 
persistent information applicable to all fax sessions. 
@return  Pointer to the newly created object. 
@leave KErrNoMemory There is insufficient memory to perform the operation. 
@capability None
*/
   {
   CFaxTransfer *self = new (ELeave) CFaxTransfer;
   CleanupStack::PushL (self);
   self->ConstructL (aFaxSettings);
   return self;
   }

EXPORT_C CFaxTransfer *CFaxTransfer::NewL (const TFaxSettings & aFaxSettings)
/** Constructs a CFaxTransfer object, which offers the publicly exported 
Symbian OS Fax Client API.

@param aFaxSettings A reference to a TFaxSettings object, which
contains persistent information applicable to all fax sessions. 
@return A pointer to the newly created object. 
@leave KErrNoMemory There is insufficient memory to perform the operation. 
@capability None
*/
   {
   CFaxTransfer *self = NewLC (aFaxSettings);
   CleanupStack::Pop ();
   return self;
   }
/********************************************************************/

void CFaxTransfer::ConstructL (const TFaxSettings & aFaxSettings)
   {
   // we now copy the TFaxSettings passed to us - we then
   // validate the contents and fill in the DialStor bits.
   // if we don't know the modem class we find it our here
   iFaxSettings = aFaxSettings;
   CFaxSettings *currentsettings;
   currentsettings = CFaxSettings::NewL ();     // bug fix thanks to MartinA
   CleanupStack::PushL (currentsettings);       // currentsettings saved
   currentsettings->ValidateAndSetClassL (&iFaxSettings);       // since this could leave
   CleanupStack::PopAndDestroy ();      // currentsettings deleted

	//   if (((TFaxClass) iFaxSettings.iFaxClass != EClass1) &&
	//     ((TFaxClass) iFaxSettings.iFaxClass != EClass2) &&
	//       ((TFaxClass) iFaxSettings.iFaxClass != EClass2point0))
	//      User::Leave (KFaxCannotAutodetect);

   iFaxSessionSettings.iFaxClass = (TFaxClass) iFaxSettings.iFaxClass;
   iFaxSessionSettings.iFaxId.Copy (iFaxSettings.iFaxId);
   iFaxSessionSettings.iMaxSpeed = iFaxSettings.iMaxSpeed;
   iFaxSessionSettings.iMinSpeed = iFaxSettings.iMinSpeed;
   iFaxSessionSettings.iRxResolution = iFaxSettings.iPreferredResolution;
   iFaxSessionSettings.iRxCompression = iFaxSettings.iPreferredCompression;
   iFaxSessionSettings.iPreferredECM = iFaxSettings.iPreferredECM;
   iFaxSessionSettings.iFaxOnDemandDelay = iFaxSettings.iFaxOnDemandDelay;

   iSource = CFaxTransferSource::NewL ();
   }
/********************************************************************/

CFaxTransfer::~CFaxTransfer ()
/** Destructor

Frees all resources owned by the object, prior to its destruction. */
   {
   delete iSource;
   }
/********************************************************************/

// this function updates the TFaxTransferProgress structure for the
// caller. It should be prior to inspection.

EXPORT_C TInt CFaxTransfer::Progress ()
/** Causes ETel to update the fax progress information in RFax::TProgress.

It should be called prior to displaying the fax progress information.

@return KErrNone if successful, otherwise another of the system-wide error 
codes. 
@capability None
*/
{
	if (iFaxStarted)
		{
		iFaxClientProgress=EProgressRxTx;
		return (iFax.GetProgress (iProgress));
		}
	if (iConverting)
		{
		iFaxClientProgress=EProgressConverting;
		//aProgress=EProgressPreparing;
		return (KErrNone);
		}

	//aProgress=EProgressInitialising;
	return (KErrNone);
}
/********************************************************************/

// this function tells faxtrans what phone number to dial
// obviously only useful in a dialling mode !
// the phone number is translated for the modem and service
// and location required using dialstor facilities if they are
// available - if not, the number is used in its raw state
//
// if use of a raw phonenumber is required then the inline function
// SetPhoneNumber should be called instead.

EXPORT_C void CFaxTransfer::SetPhoneNumberL (TDesC8 & aNumber)
/** Resolves a local telephone number into an international number by 
taking account of the current location and country.
The alternative SetPhoneNumber() should be used if the phone number 
must be used in its raw state. One of these functions must be called 
before CFaxTransfer::Start() when a fax is to be sent. 
This function may leave with KErrNoMemory if there is insufficient 
memory to perform the operation. The leave code is one of the system 
error codes: see system-wide error codes. 
The current location and country information is obtained from the 
location table in the communications database.
The resolved number is then set as the actual phone number to dial.

@param aNumber  Descriptor containing the phone number to be resolved. 
@capability WriteUserData
*/
	{
	SetPhoneNumber (aNumber);
	TBuf < KMaxDialString > rawPhoneNumber;
	rawPhoneNumber.Copy (aNumber);

#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_2);
#else
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
#endif
	CleanupStack::PushL(db);
		
	// Read the currently selected connection preference and find preferred IAP
	TInt prefRank  = 1;
	
	CCDConnectionPrefsRecord *connectionPrefs =
		static_cast<CCDConnectionPrefsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdConnectionPrefsRecord));
	CleanupStack::PushL(connectionPrefs);
	connectionPrefs->iRanking = prefRank;
	connectionPrefs->iDirection = ECommDbConnectionDirectionOutgoing;
	TBool error = connectionPrefs->FindL(*db);	
	
	// The following code is a temporary solution until an issue has been resolved in CommsDat
	// start
	CCDIAPRecord* tempPIapRecord =
		static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	tempPIapRecord->SetRecordId(connectionPrefs->iDefaultIAP);
	connectionPrefs->iDefaultIAP.iLinkedRecord = tempPIapRecord;
	
	CCDIAPRecord* pIapRecord = (CCDIAPRecord*)connectionPrefs->iDefaultIAP.iLinkedRecord;
	pIapRecord->SetRecordId(connectionPrefs->iDefaultIAP);
	pIapRecord->LoadL(*db);
	
	CCDBearerRecordBase* tempBearerRecord =
		static_cast<CCDBearerRecordBase*>(CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
	tempBearerRecord->SetRecordId(pIapRecord->iBearer);
	pIapRecord->iBearer.iLinkedRecord = tempBearerRecord;

	CCDBearerRecordBase* pBearerRecord = (CCDBearerRecordBase*) pIapRecord->iBearer.iLinkedRecord;
	pBearerRecord->SetRecordId(pIapRecord->iBearer);
	pBearerRecord->LoadL(*db);
	// end
	
	TUint32 iapId = pBearerRecord->iRecordTag;
	
	CCDIAPRecord *iapRecord =
		static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	CleanupStack::PushL(iapRecord);
	iapRecord->SetRecordId(iapId);
	iapRecord->LoadL(*db);

	// more temporary code
	// start	
	CCDBearerRecordBase* tempLocationRecord =
		static_cast<CCDBearerRecordBase*>(CCDRecordBase::RecordFactoryL(KCDTIdLocationRecord));
	tempLocationRecord->SetRecordId(iapRecord->iLocation);
	iapRecord->iLocation.iLinkedRecord = tempLocationRecord;
	
	CCDLocationRecord* pLocationRecord = (CCDLocationRecord*)iapRecord->iLocation.iLinkedRecord;
	pLocationRecord->SetRecordId(iapRecord->iLocation);
	pLocationRecord->LoadL(*db);
	// end
	
	//CommsDat Migration: Would this be the correct way to check whether or not a linked record exists:
	if (pLocationRecord == NULL)	
		{
		iPhoneNumber.Copy(rawPhoneNumber);
		}
	else
		{
		TBuf<32> serviceType;
		serviceType.Copy(iapRecord->iServiceType);

		if (!serviceType.Compare(TBuf<32>(DIAL_OUT_ISP)))	// Chargecard only valid for dial out ISP
			{
			// Get service type id
			TUint32 serviceId = iapRecord->iService;
			
			CCDDialOutISPRecord *ispRecord =
				static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
			CleanupStack::PushL(ispRecord);
			ispRecord->SetRecordId(serviceId);
			ispRecord->LoadL(*db);
			TUint32 locationId = 0;
			TUint32 chargecardId = 0;
			TRAPD(err,CommsDatUtils::CCommsDatUtils::ResolvePhoneNumberL(rawPhoneNumber, iPhoneNumber, TParseMode(EForDialing), locationId, chargecardId));
			// if resolving the phone number fails, use the raw phone number
			if (err)
				{
				iPhoneNumber.Copy(rawPhoneNumber);
				}
			
			CleanupStack::PopAndDestroy(ispRecord);
			}
		else
			{
			iPhoneNumber.Copy(rawPhoneNumber);
			}
		}
	CleanupStack::PopAndDestroy(3); // db, connectionPrefs, iapRecord
	}
/********************************************************************/

// in order to cancel a fax session we set a cancel flag in the lowest
// level (CFaxModem) - if this has not yet been fully created then we set
// a flag in the next level up (CFaxModemDriver) instead, which is always
// going to be there as it is created via CFaxTransfer::ConstructL
//
// because of the way that the CFaxModem monitors its request flag
// we need to avoid multiple cancel requests, so only the first call to
// Cancel has any effect on it

EXPORT_C void CFaxTransfer::Cancel ()
/** Tells the fax engine to cancel the fax session at the first convenient 
opportunity.

The caller should wait for cancellation to complete, which will usually be 
signalled by the fax thread's TRequestStatus completing with a KFaxCancelRequested 
error code. After the fax thread completes, Stop() should be called in the 
normal way.

The function can be called at any time after the call to Start(). 
@capability None
*/
   {
   if ((iClientCancel == KRequestPending) || (iClientCancel == KErrNone))
      {
      TRequestStatus *cancelClient = &iClientCancel;
      iDriverThread.RequestComplete (cancelClient, KErrCancel);
      }
   }
/********************************************************************/

EXPORT_C TInt CFaxTransfer::Start (TRequestStatus & aThreadStat)
/**
Start fax sending or receiving session by launching a separate high priority thread.
A call to Start must be paired with a call to Stop as this is an EPOC32 requirement.

@param     aThreadStat  thread logon request status
@return    thread creation code
@capability NetworkServices
@capability ReadUserData
@capability WriteUserData
*/
   {
    // the heap and stack sizes set here (4K each) are pure guesswork
    // CFaxModemDriver has allocated heap space in the parent thread
    // for any objects it needs to create after this stage
    // stack sizes increased by 512K to allow etel to connect

   TInt state = KErrNone;
   TInt heapSize = 0x14000;
   TInt stackSize = 0x14000;

	__FLOG_FAXCLI(_L8(" "));
	__FLOG_FAXCLI(_L8("-------------------------- new log --------------------------"));
	__FLOG_FAXCLI(_L8("CFaxTransfer::Start, starting FaxClientThread"));
	__FLOG_FAXCLI(_L8(" "));

   state = iDriverThread.Create ((_L ("FaxClientThread")), FaxClientThread, stackSize, heapSize, heapSize, this, EOwnerThread);
   if (state)
      {
      state = KFaxThreadError;
      }
   else
      {
      aThreadStat = KRequestPending;
      if (iClientCancel != KErrCancel)
         iClientCancel = KRequestPending;
      iDriverThread.Logon (aThreadStat);
      iDriverThread.SetPriority (EPriorityRealTime);
      iDriverThread.Resume ();
      }
   return (state);
   }
/*********************************************************************/

EXPORT_C void CFaxTransfer::Stop ()
/** Kills the fax thread once it has completed. 

Fax threads do not kill themselves, so every successful call to CFaxTransfer::Start() 
must be paired with a call to CFaxTransfer::Stop(). 
@capability NetworkServices
@capability ReadUserData
@capability WriteUserData
*/
   {
   iDriverThread.Close ();
   }
/*********************************************************************/

// this is a utility function which is the entry to our thread
// it isn't part of any class, but we pass the address
// of our CFaxTransfer in so that we can check the
// session parameter and get back to the required function
//
// All possible leaves should be trapped as the return
// from this function is the TRequestStatus which the
// caller to CFaxTransfer::Start is waiting for.

TInt FaxClientThread (TAny * session)
   {


   // start of unecessary bureaucracy - error checking left out
/*#define CSY_NAME _L("ECUART")
#define LDD_NAME _L("ECOMM")
#if defined (__WINS__)
#define PDD_NAME _L("ECDRV")
#else
#define PDD_NAME _L("EUART1")
#endif*/

/*
   User::LoadPhysicalDevice (PDD_NAME);
   User::LoadLogicalDevice (LDD_NAME);*/
// jerry.CreateGlobal(_L("FaxCliSem"),0,EOwnerProcess);
   
   RCommServ server;
   // coverity[check_return]
   server.Connect ();
// end of unecessary bureaucracy

   TInt state;
   CTrapCleanup *cleanup = CTrapCleanup::New ();
// CFaxTransfer *faxsession = (CFaxTransfer *) session;
   CFaxTransfer *faxsession =reinterpret_cast<CFaxTransfer *>(session);
	__FLOG_FAXCLI(_L8("FaxClientThread entering..."));

   state = faxsession->iTelServer.Connect ();
   if (state == KErrNone)
   {
   TBuf<KCommsDbSvrMaxFieldLength> tsyName;
   TRAP(state,faxsession->GetPhoneModuleNameL(tsyName));
   if (state==KErrNone)
	{
      state = faxsession->iTelServer.LoadPhoneModule (tsyName);
      if (state == KErrNone)
         {
		 RTelServer::TPhoneInfo phoneInfo;
		 state = faxsession->GetPhoneInfoForTsy(tsyName,phoneInfo);
		 if (state == KErrNone)
			{
			
			__FLOG_FAXCLI(_L8("FaxClientThread iPhone.Open"));
			state = faxsession->iPhone.Open (faxsession->iTelServer, phoneInfo.iName);
            if (state == KErrNone)
				{
				
				__FLOG_FAXCLI(_L8("FaxClientThread iLine.Open"));
				state = faxsession->iLine.Open (faxsession->iPhone, _L ("Fax"));
				if (state == KErrNone)
					{
					//
					// If the call name has been provided, then open the
					// existing call, otherwise open a new call.
					//
					if (faxsession->iCallName.Length() == 0)
						{
						__FLOG_FAXCLI(_L8("FaxClientThread iCall.OpenNewCall"));
						state = faxsession->iCall.OpenNewCall (faxsession->iLine);
						}
					else
						{
						__FLOG_FAXCLI(_L8("FaxClientThread iCall.OpenExistingCall"));
						state = faxsession->iCall.OpenExistingCall(faxsession->iLine, faxsession->iCallName);
						}

					if (state == KErrNone)
						{
						faxsession->iSource->iPage = 0;
						if (faxsession->iMode & KFaxReceive)
							{
							faxsession->iFaxSessionSettings.iMode = RCall::EReceive;
							TRAP (state, faxsession->FaxReceiveL ());
							
							__FLOG_FAXCLI1(_L8("FaxClientThread FaxReceiveL exited state=%d"),state);
							
							faxsession->iSource->CloseFaxInStore ();
							}		
			            else
					        {
							faxsession->iFaxSessionSettings.iMode = RCall::ETransmit;
							TRAP (state, faxsession->FaxTransmitL ());
							faxsession->iSource->iInstream.Close ();
							}

	                    // we tidy up by deleting any objects created
						// (it does no harm if their pointers are already NULL)
						// and we close any streams (which also does no harm
						// if they have already been closed)

						faxsession->iFaxStarted = EFalse;
						faxsession->iFax.Close ();
						faxsession->iSource->CloseFaxStore ();
						delete faxsession->iSource->iWriteFaxFile;
						faxsession->iSource->iWriteFaxFile = NULL;
		                delete faxsession->iSource->iReadFaxFile;
						faxsession->iSource->iReadFaxFile = NULL;
						
						faxsession->iSource->iOutstream.Close ();
						faxsession->iSource->iInstream.Close ();
						
						RCall::TStatus callStatus;
						faxsession->iCall.GetStatus(callStatus);
						
						__FLOG_FAXCLI1(_L8("FaxClientThread callStatus=%d"),callStatus);

						if (callStatus!=RCall::EStatusIdle)
							faxsession->iCall.HangUp ();
						
						__FLOG_FAXCLI(_L8("FaxClientThread iCall.Close"));
						faxsession->iCall.Close ();
						}
					
					__FLOG_FAXCLI(_L8("FaxClientThread iLine.Close"));
					faxsession->iLine.Close ();
					}
				
				__FLOG_FAXCLI(_L8("FaxClientThread iPhone.Close"));
				faxsession->iPhone.Close ();
				}
			}
		faxsession->iTelServer.UnloadPhoneModule (tsyName);
		}
		}
    faxsession->iTelServer.Close ();
    }

   delete cleanup;

// intercept the etel wrong modem type error
   if (state == KErrEtelWrongModemType) state = KFaxErrWrongModemType;

   return (state);
   }

/*********************************************************************/

void CFaxTransfer::GetPhoneModuleNameL(TDes& aModuleName) const
	{
	// AnnW, 9/8/99 - This all assumes that we are taking the modem settings from the 
	// dial out IAP, which is fince for now, but may not be in the future?  This may also 
	// need modifying for Linda?
	
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
	CleanupStack::PushL(db);
	
	// Read the currently selected connection preference and find preferred IAP
	TInt prefRank  = 1;
	
	CCDConnectionPrefsRecord *connectionPrefs = 
		static_cast<CCDConnectionPrefsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdConnectionPrefsRecord));
	CleanupStack::PushL(connectionPrefs);
	connectionPrefs->iRanking = prefRank;
	connectionPrefs->iDirection = ECommDbConnectionDirectionOutgoing;
	connectionPrefs->FindL(*db);
	
	// The following code is a temporary solution until an issue has been resolved in CommsDat
	// start
	CCDIAPRecord* tempPIapRecord =
		static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	tempPIapRecord->SetRecordId(connectionPrefs->iDefaultIAP);
	connectionPrefs->iDefaultIAP.iLinkedRecord = tempPIapRecord;
	
	CCDIAPRecord* pIapRecord = (CCDIAPRecord*)connectionPrefs->iDefaultIAP.iLinkedRecord;
	pIapRecord->SetRecordId(connectionPrefs->iDefaultIAP);
	pIapRecord->LoadL(*db);
	
	CCDBearerRecordBase* tempBearerRecord =
		static_cast<CCDBearerRecordBase*>(CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
	tempBearerRecord->SetRecordId(pIapRecord->iBearer);
	pIapRecord->iBearer.iLinkedRecord = tempBearerRecord;

	CCDBearerRecordBase* pBearerRecord = (CCDBearerRecordBase*) pIapRecord->iBearer.iLinkedRecord;
	pBearerRecord->SetRecordId(pIapRecord->iBearer);
	pBearerRecord->LoadL(*db);
	// end
	
	TUint32 iapId = pBearerRecord->iRecordTag;
	
	CMDBField<TUint32>* bearerField = new(ELeave) CMDBField<TUint32>(KCDTIdIAPBearer);
	CleanupStack::PushL(bearerField);
	bearerField->SetRecordId(iapId);
	bearerField->LoadL(*db);
	TUint32 modemId = *bearerField;
	CleanupStack::PopAndDestroy(bearerField);

	CMDBField<TDesC>* tsyField = new(ELeave) CMDBField<TDesC>(KCDTIdTsyName);
	CleanupStack::PushL(tsyField);
	tsyField->SetRecordId(modemId);
	tsyField->SetMaxLengthL(KMaxTextLength);
	tsyField->LoadL(*db);
	aModuleName = *tsyField;
	CleanupStack::PopAndDestroy(tsyField);
	
	CleanupStack::PopAndDestroy(2); // db, connectionPrefs
	}

void CFaxTransfer::CancelFaxServerSession ()
   {
   if (iMode & KFaxWaitForRing)
      iCall.AnswerIncomingCallCancel ();
   else
      {
      if (iMode & KFaxOffHook)
         iCall.ConnectCancel ();
      else
         iCall.DialCancel ();
      }
   }
/*********************************************************************/
// receiving a fax is easy because it is passive
// just open the file for receiving and off we go

void CFaxTransfer::FaxReceiveL ()
   {

	__FLOG_FAXCLI(_L8("CFaxTransfer::FaxReceiveL entering"));

   if (iMode & KFaxPoll)
      iFaxSessionSettings.iFaxRetrieveType = RCall::EFaxPoll;
   else
      iFaxSessionSettings.iFaxRetrieveType = RCall::EFaxOnDemand;

   iSource->OpenFaxInL (iReceiveFileName);

   SetFaxSettingsL();

   if (iMode & KFaxWaitForRing)
      {//-- answering incoming call if fax is waiting for a call
	  iCall.AnswerIncomingCall (iTransferStatus);	
      if (iClientCancel != KRequestPending)
         {//-- cancel request, leave
         iCall.AnswerIncomingCallCancel ();
		 User::WaitForRequest(iTransferStatus);
         User::Leave (KFaxCancelRequested);
         }
      }
   else	//if (iMode & KFaxWaitForRing) 
      {//-- if fax is not waiting for a call, dial
      if (iMode & KFaxOffHook)
         iCall.Connect (iTransferStatus);
      else
         {
         if (iPhoneNumber.Length() == 0)
            User::Leave (KErrCouldNotConnect);
         iCall.Dial (iTransferStatus, iPhoneNumber);
         }
      }

   TRequestStatus reqStatus;
   RCall::TStatus callStatus;
   iCall.NotifyStatusChange (reqStatus, callStatus);

   for (;;)
      {
      User::WaitForAnyRequest ();
      if (reqStatus != KRequestPending)
         {
         //-- Call status changed
         if (reqStatus.Int () != KErrNone)
            {
            CancelFaxServerSession ();
            User::Leave (reqStatus.Int ());
            }
         if (iMode & KFaxWaitForRing)
            {//-- Fax is waiting for a ring
			if (callStatus == RCall::EStatusRinging)
				{//-- Call status is 'Ringing', continue waiting 
				iCall.NotifyStatusChange(reqStatus, callStatus);
				reqStatus = KRequestPending;
				continue;
				}
			else 
			 //-- due to PIA-586KGE fix (changes in CATAnswerFax::Start()) 'Connecting' may be not noticed here
			 //-- so EStatusConnected state is ok
			 if (callStatus != RCall::EStatusAnswering && callStatus != RCall::EStatusConnected )
					{
					iCall.AnswerIncomingCallCancel ();
					User::Leave (KFaxEtelServerError);
					} //if (callStatus != RCall::EStatusAnswering)
            } //if (iMode & KFaxWaitForRing)
         else
            {//-- Fax is not waiting for a ring 
            if (iMode & KFaxOffHook)
               {
               if (callStatus != RCall::EStatusConnecting)
                  {
                  iCall.ConnectCancel ();
                  User::Leave (KFaxEtelServerError);
                  }
               }
            else if (callStatus != RCall::EStatusDialling)
               {
               iCall.DialCancel ();
               User::Leave (KFaxEtelServerError);
               }
            }
         TInt ret = iFax.Open (iCall);
         if (ret != KErrNone)
            {
            CancelFaxServerSession ();
            User::Leave (ret);
            }
         iFaxStarted = ETrue;
         reqStatus = KRequestPending;
         }//if (reqStatus != KRequestPending)
      else if (iClientCancel != KRequestPending)
         {//-- Fax cancel request
         if (iFaxStarted == EFalse)
            iCall.NotifyStatusChangeCancel ();
         CancelFaxServerSession ();
         User::Leave (KFaxCancelRequested);
         }
      else if (iTransferStatus != KRequestPending)
         {//--iCall.AnswerIncomingCall status changed
         if (iFaxStarted == EFalse)
            iCall.NotifyStatusChangeCancel ();
         User::LeaveIfError (iTransferStatus.Int ());
         break;
         }
      else
         {
         if (iFaxStarted == EFalse)
            iCall.NotifyStatusChangeCancel ();
         CancelFaxServerSession ();
         User::Leave (KErrCompletion);  // stray event handle
         }
      }

   //-- Data transfer phase
   User::LeaveIfError (iFax.GetProgress (iProgress));
   while (iProgress.iPhase == EDataTransfer)
      {
      iSource->iResolu = iProgress.iResolution;
      iSource->iCompression = iProgress.iCompression;
      for (;;)
         {
         iFax.Read (iTransferStatus, iDataBuf);
         User::WaitForRequest (iTransferStatus, iClientCancel);
         if (iClientCancel != KRequestPending)
            {
            iFax.TerminateFaxSession ();
            User::Leave (KFaxCancelRequested);
            }
         User::LeaveIfError (iTransferStatus.Int ());

         // the server has buffered up lines to minimize interaction
         // iDataBuf starts with a TInt containing the number of lines
         // Each line follows, preceded with a TInt containing its length
         // which must be copied as it might not be aligned on a 4-byte
         // boundary - a line of zero length indicates we have reached
         // the end of the page

         TUint8 *nextLine = CONST_CAST (TUint8 *, iDataBuf.Ptr ());
         TInt lengthOfLine=0;
         TInt numberOfLines;

         Mem::Copy (&numberOfLines, nextLine, sizeof (TInt));
         nextLine += sizeof (TInt);

         while (numberOfLines--)
            {
            Mem::Copy (&lengthOfLine, nextLine, sizeof (TInt));
            if (lengthOfLine == 0)
               break;
            nextLine += sizeof (TInt);
            TPtrC8 currentLine (nextLine, lengthOfLine);
            iSource->WriteFaxDataL (currentLine);
            nextLine += lengthOfLine;
            }
         if (lengthOfLine == 0)
            break;
         }
      iSource->WritePageParmsL (iProgress.iAnswerback);
      iFax.WaitForEndOfPage (iTransferStatus);
      User::WaitForRequest (iTransferStatus, iClientCancel);
      if (iClientCancel != KRequestPending)
         {
         iFax.TerminateFaxSession ();
         User::Leave (KFaxCancelRequested);
         }
      User::LeaveIfError (iTransferStatus.Int ());
      User::LeaveIfError (iFax.GetProgress (iProgress));
	  	  
      }
//	iSource->CloseFaxInStore ();
    if (iProgress.iCompression==EModifiedRead)
		 Convert1dL ();
   }
/*********************************************************************/

// sending a fax is rather more complex
//
// we need a valid phone number (we don't do polling)
//
// we need a valid list of pages to send
//
// we need to set the resolution we require to that of the
// fax we want to send (use the last page)
//

void CFaxTransfer::FaxTransmitL ()
   {
   
   
   __FLOG_FAXCLI(_L8("CFaxTransfer::FaxTransmitL entering"));

   TBool ConversionStatus=FALSE;
   //TInt err=0;
   if (iSource->iOurPreferredCompression==Prefer2D)
		{	
		TRAPD (retcode,Convert2dL());
		if (retcode==KErrNone)
			ConversionStatus=TRUE;
		else
			{
			ConversionStatus=FALSE;
			iSource->iOurPreferredCompression=Prefer1D;		// if anything goes wrong during conversion
			}												// try the 1D version.
		}
   if ((iSource->iFaxPages == 0) || (iSource->iFaxListEntries == 0))
      User::Leave (KErrNotFound);
   iSource->iSavedFaxListEntries = iSource->iFaxListEntries;
   iSource->iOurFaxEntry.iPageCount = 0; 
   iSource->iInstream.Open (*iSource->iSources);
   iSource->GetNextPageReadyL ();

   if (iSource->iOurPreferredCompression==Prefer1D)
	   iFaxSessionSettings.iTxPages=iSource->iFaxPages;

   // we pass in to the server the resolution of the last added source page

   iFaxSessionSettings.iTxResolution = (TFaxResolution) iSource->iResolu;
   if ((iSource->iOurPreferredCompression==Prefer2D) && ConversionStatus==TRUE)
		iFaxSessionSettings.iTxCompression = (TFaxCompression) EModifiedRead;	
   else
	   iFaxSessionSettings.iTxCompression = (TFaxCompression) iSource->iCompression;

   SetFaxSettingsL();	// pass the settings to the Fax Server
   SetSharedFileHandlesL();

   if (iMode & KFaxOffHook)
      iCall.Connect (iTransferStatus);
   else
      {
      if (iPhoneNumber.Length() == 0)
         User::Leave (KErrCouldNotConnect);
      iCall.Dial (iTransferStatus, iPhoneNumber);
      }

   TRequestStatus reqStatus;
   RCall::TStatus callStatus;
   iCall.NotifyStatusChange (reqStatus, callStatus);

   for (;;)
      {
      User::WaitForAnyRequest ();
      if (reqStatus != KRequestPending)
         {
         if (reqStatus.Int () != KErrNone)
														// initialisation and comm port errors
            {
            CancelFaxServerSession ();
            User::Leave (reqStatus.Int ());
            }
         if (iMode & KFaxOffHook)
            {
            if (callStatus != RCall::EStatusConnecting)
               {
               iCall.ConnectCancel ();
               User::Leave (KFaxEtelServerError);
               }
            }
         else if (callStatus != RCall::EStatusDialling)
            {
            iCall.DialCancel ();
            User::Leave (KFaxEtelServerError);
            }
         TInt ret = iFax.Open (iCall);
         if (ret != KErrNone)
            {
            CancelFaxServerSession ();
            User::Leave (ret);
            }
         iFaxStarted = ETrue;
         reqStatus = KRequestPending;   // to prevent first scenario being
										// chosen each time any request comes in
         }
      else if (iClientCancel != KRequestPending)
         {
         if (iFaxStarted == EFalse)
            iCall.NotifyStatusChangeCancel ();
         CancelFaxServerSession ();
         User::Leave (KFaxCancelRequested);
         }
      else if (iTransferStatus != KRequestPending)
         {
         if (iFaxStarted == EFalse)
            iCall.NotifyStatusChangeCancel ();
         User::LeaveIfError (iTransferStatus.Int ());
         break;
         }
      else
         {
         if (iFaxStarted == EFalse)
            iCall.NotifyStatusChangeCancel ();
         CancelFaxServerSession ();
         User::Leave (KErrCompletion);  // stray event handle
         }
      }

   for (;;)
      {
      User::LeaveIfError (iFax.GetProgress (iProgress));
      TInt thispage = iProgress.iPage;
      TInt thisline = 1;
      ASSERT (iSource->iLines);

		if ((iProgress.iCompression) && (iSource->iSavedFaxListEntries>=1))
			{
			while (iSource->iCompression == EModifiedHuffman)		// loop until we 
			iSource->GetNextPageReadyL ();							// find the 2D document
			}
	  
      //we buffer up lines to minimize client-server interaction
      //iDataBuf starts with a TInt containing the number of lines
      //Each line follows, preceded with a TInt containing its length
      //which must be copied as it might not be aligned on a 4-byte boundary

      TUint8 *startData;
      TUint8 *lineData;
      TInt numberOfLines;
      const TUint8 *maxData;

      TUint8 *currentLineData;
      TInt currentLineLength;

      for (;;)
         {
         lineData = startData = CONST_CAST (TUint8 *, iDataBuf.Ptr ());
         maxData = startData + iDataBuf.MaxLength () - KMaxT4Des - sizeof (TInt);

         iDataBuf.SetMax ();
         numberOfLines = 0;
         lineData += sizeof (TInt);
         for (;;)
            {
            if (thisline > iSource->iLines)
               break;
            thisline++;
            numberOfLines++;
            currentLineData = lineData + sizeof (TInt);
            *currentLineData = 0x0;
            TPtr8 currentLine (currentLineData + 1, 0, KMaxT4Des - 1);
            iSource->ReadFaxData (currentLine);
            currentLineLength = currentLine.Length () + 1;
            Mem::Copy (lineData, &currentLineLength, sizeof (TInt));
            lineData += sizeof (TInt);
            lineData += currentLineLength;
            if (lineData > maxData)
               break;
            }
         Mem::Copy (startData, &numberOfLines, sizeof (TInt));
         iDataBuf.SetLength (lineData - startData);
         iFax.Write (iTransferStatus, iDataBuf);
         User::WaitForRequest (iTransferStatus, iClientCancel);
         if (iClientCancel != KRequestPending)
            {
            iFax.TerminateFaxSession ();
            User::Leave (KFaxCancelRequested);
            }
         User::LeaveIfError (iTransferStatus.Int ());
         if (thisline > iSource->iLines)
            break;
         }
      iDataBuf.Zero ();
      iFax.WaitForEndOfPage (iTransferStatus);
      User::WaitForRequest (iTransferStatus, iClientCancel);
      if (iClientCancel != KRequestPending)
         {
         iFax.TerminateFaxSession ();
         User::Leave (KFaxCancelRequested);
         }
      User::LeaveIfError (iTransferStatus.Int ());
      User::LeaveIfError (iFax.GetProgress (iProgress));
      if (iProgress.iPhase != EDataTransfer)
         break;
      if (thispage == iProgress.iPage)
         iSource->PreviousPageFindL ();
      else
         iSource->GetNextPageReadyL ();
      }
   if ((iProgress.iCompression==EModifiedHuffman) && (iSource->iOurPreferredCompression==Prefer2D))
   {
	   for (TInt i=0; i<(iSource->iFaxPages/2);i++)
	   iSource->GetNextPageReadyL ();						
   }
}

void CFaxTransfer::SetFaxSettingsL()
	{
   TInt error;
   RPhone::TStatus phoneStatus;
   
   error=iPhone.GetStatus(phoneStatus);
	if (error == KErrNone)
		{
		if ((phoneStatus.iModemDetected == RPhone::EDetectedNotPresent) || (phoneStatus.iModemDetected == RPhone::EDetectedUnknown))
			{
			TRequestStatus initialiseStatus;
			iPhone.Initialise(initialiseStatus);
			User::WaitForAnyRequest();
			if (iClientCancel != KRequestPending)
				{
				iPhone.InitialiseCancel();					// issue initialiseCancel command
				User::WaitForRequest(initialiseStatus);		// wait for it to complete
				User::Leave(KFaxCancelRequested);			// leave with  KFaxCancelRequested
				}	
			User::LeaveIfError(initialiseStatus.Int());
			}
		}   
   TInt ret=KErrNone;	// so communication with modem has begun...
   TInt count=0;		// we have a loop where we try three times to set fax settings in the
						// unlikely event that the initialisation sequence has just started
						// so the phoneStatus gives ModemDetected Present, but the supported fax
						// classes have not yet been ascertained.
   do 
	{
	ret=iCall.SetFaxSettings (iFaxSessionSettings);	// retry until init sequence has found out what
													// the fax classes supported are.
	if (ret==KErrEtelUnknownModemCapability)
		User::After(500000);
	}
   while (ret==KErrEtelUnknownModemCapability && iClientCancel==KRequestPending && count++<3);
   if (iClientCancel!=KRequestPending)
	   ret=KFaxCancelRequested;
   User::LeaveIfError(ret);
   }
void CFaxTransfer::SetSharedFileHandlesL()
	{
	//Open a file server session and a file handle on the private header file to be shared.
	RFs sharedFs;
	RFile sharedHeaderFile;
	
	User::LeaveIfError(sharedFs.Connect());
	CleanupClosePushL(sharedFs);
	//allow it to be shared
	User::LeaveIfError(sharedFs.ShareProtected());
	
	//get path 
	TFileName headerFullPath;
	CFaxHeaderLines::GeneratePathForHeaderFileL(headerFullPath);
	
	//Open the file in read-only mode
	User::LeaveIfError(sharedHeaderFile.Open(sharedFs, headerFullPath, EFileRead));
	CleanupClosePushL(sharedHeaderFile);
	
	User::LeaveIfError(iCall.AdoptFaxSharedHeaderFile(sharedHeaderFile));
	CleanupStack::PopAndDestroy();//sharedHeaderFile
	CleanupStack::PopAndDestroy();//sharedFs
	}
	
TInt CFaxTransfer::GetPhoneInfoForTsy(const TDes& aTsyName, RTelServer::TPhoneInfo& aPhoneInfo) const
//
//	Finds the index of the phone which belongs to TSY named "aTsyName", and retrieves its info.
//
	{
	 TInt count=0;
	 iTelServer.EnumeratePhones(count);
	 TName matchTsyName;
	 TInt ret = iTelServer.GetTsyName(0,matchTsyName);
	 if (ret == KErrNone)
		{
		 TInt i=0;
		 if (count>1)
			{
			while (matchTsyName.CompareF(aTsyName)!=KErrNone && i++<count && ret==KErrNone)
				ret = iTelServer.GetTsyName(i,matchTsyName);
			}
		 ASSERT(matchTsyName.CompareF(aTsyName)==KErrNone);
		 iTelServer.GetPhoneInfo(i,aPhoneInfo);
		}
	 return ret;
	}

/********************************************************************/

EXPORT_C void CFaxTransfer::AddSourceL (const TFileName & aFaxPageStore, TFaxPreferredCompression aPreferredCompression)
/** Specifies which pages of a fax store file should be sent. 
It can be called more than once to add multiple pages from different files. Typically, 
this function is called twice for each transmission: once to queue the cover sheet, 
and a second time to queue the remainder of the fax.
The overloaded variants are supplied primarily to help error recovery in cases when 
a fax transmission is either incomplete or when specific pages need to be resent. 
When transmitting a fax, at least one variant of this function must be called before 
CFaxTransfer::Start().

@param aFaxPageStore  Name of the fax store file from which to take pages.
@param  aPreferredCompression Preferred compression.
@capability WriteUserData
*/
   {
   iSource->AddSourceL (aFaxPageStore, 1,aPreferredCompression);
   }

EXPORT_C void CFaxTransfer::AddSourceL (const TFileName & aFaxPageStore, TInt aStartPage,TFaxPreferredCompression aPreferredCompression)
/** Specifies which pages of a fax store file should be sent. 
It can be called more than once to add multiple pages from different files. 
Typically, this function is called twice for each transmission: once to queue 
the cover sheet, and a second time to queue the remainder of the fax.
The overloaded variants are supplied primarily to help error recovery in cases 
when a fax transmission is either incomplete or when specific pages need to be 
resent. When transmitting a fax, at least one variant of this function must be 
called before CFaxTransfer::Start().

@param aFaxPageStore  Name of the fax store file from which to take pages. 
@param aStartPage  Page in file to start from. If omitted, the file is sent 
from the start. 
@param  aPreferredCompression Preferred compression.
@capability WriteUserData
*/
   {
   iSource->AddSourceL (aFaxPageStore, aStartPage, aPreferredCompression);
   }

EXPORT_C void CFaxTransfer::AddSourceL (const TFileName & aFaxPageStore, TInt aStartPage, TInt aEndPage, TFaxPreferredCompression aPreferredCompression)
/**Specifies which pages of a fax store file should be sent. 
It can be called more than once to add multiple pages from different files. 
Typically, this function is called twice for each transmission: once to queue 
the cover sheet, and a second time to queue the remainder of the fax.
The overloaded variants are supplied primarily to help error recovery in cases 
when a fax transmission is either incomplete or when specific pages need to be 
resent. When transmitting a fax, at least one variant of this function must be 
called before CFaxTransfer::Start().

@param aFaxPageStore  Name of the fax store file from which to take pages. 
@param aStartPage  Page in file to start from. If omitted, the file is sent from the start. 
@param aEndPage  Page in file to stop sending. If omitted, transmission continues to the end. 
@param  aPreferredCompression Preferred compression.
@capability WriteUserData
*/
   {
   iSource->AddSourceL (aFaxPageStore, aStartPage, aEndPage, aPreferredCompression);
   }

//EXPORT_C void CFaxTransfer::AddSourceL (const TFileName & aFaxPageStore, const TFileName & aFaxPageStore2)
//	{
//	iSource->AddSourceL(aFaxPageStore, aFaxPageStore2);
//	}

EXPORT_C void CFaxTransfer::RemoveAllSources ()
/** Clears the complete list of pages previously selected for faxing using one 
of the variants of AddSourceL(). 

Removal of individual items from the list of pages to be fax is not possible. 

This function must be used between successive fax transmissions if previously 
sent pages aren't to be sent again. 
@capability WriteUserData
*/
   {
   iSource->RemoveAllSources ();
   }

/********************************************************************/
void CFaxTransfer::Convert2dL ()
	{	
	TInt err=0;
	TInt k=0;
	TBuf<64> filename;
	CWriteFaxFile*  writeFaxFile;
	CReadFaxFile*   readFaxFile;
	TRawScanLine	decodedScanLine;
	//TFaxBufSenderId sd;
	TFaxBufSenderId senderId;


	writeFaxFile = CWriteFaxFile::NewL();
	CleanupStack::PushL(writeFaxFile);
	readFaxFile  = CReadFaxFile::NewL();
	CleanupStack::PushL(readFaxFile);
	iConverting=TRUE;
	
	TRAP (err,readFaxFile->OpenL(iSource->iOurFaxEntry.iFaxPageStore));
	if (err!=KErrNone)
		{
		iConverting=FALSE;
		User::Leave(err);
		}

 	readFaxFile->iReadFaxPages->SetPageL(0);
	TFaxPageInfo info = readFaxFile->iReadFaxPages->CurrentPageInfo();
	if (info.iCompression != EModifiedHuffman)
		User::Panic(_L("Not a 1D file"),1);

	//writeFaxFile->OpenL(_L("c:\\blank2D.fax"),64);
	filename.Copy(iSource->iOurFaxEntry.iFaxPageStore);
	filename.Append(_L("2d"));
	// the second push in OpenL doesn't cause a double deletion of writeFaxFile, since it is pushed by using TCleanupItem.
	// coverity [double_push]
	writeFaxFile->OpenL(filename,64);
	//writeFaxFile->OpenL(iSource->iOurFaxEntry.iFaxPageStore,64);
	

	for (k=0; k<iSource->iFaxPages; k++)
	{
	readFaxFile->iReadFaxPages->SetPageL(k);
	TFaxPageInfo info = readFaxFile->iReadFaxPages->CurrentPageInfo();
	if (info.iCompression != EModifiedHuffman)
		User::Panic(_L("Not a 1D file"),1);
	writeFaxFile->iWriteFaxPages->StartPage(info.iResolution, EModifiedRead);
	for (TInt n = info.iNumScanLines ; n  ; n--)
		{
		readFaxFile->iReadFaxPages->GetScanLineL(decodedScanLine);	
		writeFaxFile->iWriteFaxPages->AddScanLineL(decodedScanLine);
		}

	writeFaxFile->iWriteFaxPages->EndPageL(info.iResolution,senderId, EModifiedRead);
	}// end of for statement
		
	writeFaxFile->CommitL();
	writeFaxFile->Close();
	readFaxFile->Close();
		
	AddSourceL(filename,Prefer2D);
	iFaxSessionSettings.iTxPages = (iSource->iFaxPages/2);	// sent only the 2d version of the document
															// which means half the attached pages
	CleanupStack::PopAndDestroy(2);
	iConverting=FALSE;
}	



/***********************************************************************************/
// This function takes the received 2D file and converts it to 1D
// The 2D file will be deleted and the 1D version of the file will 
// inherit the name of the received 2D version
/************************************************************************************/
void CFaxTransfer::Convert1dL ()
	{	
	TInt err=0;
	TInt k=0;
	TBuf<64> filename;
	CWriteFaxFile* writeFaxFile;
	CReadFaxFile*  readFaxFile;
	TRawScanLine decodedScanLine;
	//TFaxBufSenderId sd;
	TFaxBufSenderId senderId;

	writeFaxFile = CWriteFaxFile::NewL();
	CleanupStack::PushL(writeFaxFile);
	readFaxFile  = CReadFaxFile::NewL();
	CleanupStack::PushL(readFaxFile);	
	iConverting=TRUE;

	//TRAP (err,readFaxFile->OpenL(iSource->iOurFaxEntry.iFaxPageStore));
	TRAP (err,readFaxFile->OpenL(iReceiveFileName));
	if (err!=KErrNone)
		User::Leave(err);
		
 	readFaxFile->iReadFaxPages->SetPageL(0);
	TFaxPageInfo info = readFaxFile->iReadFaxPages->CurrentPageInfo();
	//if (info.iCompression != EModifiedHuffman)
	if (info.iCompression != EModifiedRead)
		User::Panic(_L("Not a 2D file"),1);
	
	//filename.Copy(iSource->iOurFaxEntry.iFaxPageStore);
	filename.Copy(iReceiveFileName);
	filename.Append(_L("1d"));
	// the second push in OpenL doesn't cause a double deletion of writeFaxFile, since it is pushed by using TCleanupItem.
	// coverity [double_push]
	writeFaxFile->OpenL(filename,64);
	//	writeFaxFile->OpenL(iSource->iOurFaxEntry.iFaxPageStore,64);
	

	TInt iRxPages=readFaxFile->iReadFaxPages->NumPages();

	for (k=0; k<iRxPages; k++)
	{
	readFaxFile->iReadFaxPages->SetPageL(k);
	TFaxPageInfo info = readFaxFile->iReadFaxPages->CurrentPageInfo();
	//if (info.iCompression != EModifiedHuffman)
	if (info.iCompression != EModifiedRead)
		User::Panic(_L("Not a 2D file"),1);
	//writeFaxFile->iWriteFaxPages->StartPage(info.iResolution, EModifiedRead);
	writeFaxFile->iWriteFaxPages->StartPage(info.iResolution, EModifiedHuffman);
	for (TInt n = info.iNumScanLines ; n  ; n--)
		{
		readFaxFile->iReadFaxPages->GetScanLineL(decodedScanLine);	
		writeFaxFile->iWriteFaxPages->AddScanLineL(decodedScanLine);
		}

	//writeFaxFile->iWriteFaxPages->EndPageL(info.iResolution,senderId, EModifiedRead);
	writeFaxFile->iWriteFaxPages->EndPageL(info.iResolution,senderId, EModifiedHuffman);
	}// end of for statement
		
	writeFaxFile->CommitL();
	writeFaxFile->Close();
	readFaxFile->Close();
	
	RFs FileServer;
	err = FileServer.Connect();
	if (err !=KErrNone)
		User::Leave(err);
	User::LeaveIfError(FileServer.Delete(iReceiveFileName));

	User::LeaveIfError(FileServer.Rename(filename,iReceiveFileName));
	
	FileServer.Close();

	CleanupStack::PopAndDestroy(2);	
	iConverting=FALSE;
//	AddSourceL(filename,Prefer2D);
//	iFaxSessionSettings.iTxPages = (iSource->iFaxPages/2);	// sent only the 2d version of the document
	}	


/*

CFaxTransfer::StartThread()
	{
	TInt state = KErrNone;
	TInt heapSize = 0x14000;	
	TInt stackSize = 0x14000;

	reinterpret_cast<TAny*>(this);  // this points to a CFaxTransfer Object
	state = iConverterThread.Create ((_L ("FaxConverterThread")), FaxConverterThread, stackSize, heapSize, heapSize, this, EOwnerThread);
	if (state)
		{
		state = KFaxThreadError;
		}
	else
		{
		//aThreadStat = KRequestPending;
		//if (iClientCancel != KErrCancel)
		//iClientCancel = KRequestPending;
		//iDriverThread.Logon (aThreadStat);
		
		iConverterThread.SetPriority (EPriorityRealTime);
		iConverterThread.Resume ();
		}
	return state;
	}

TInt FaxConverterThread (TAny * session)
	{
	TInt m;
	CTrapCleanup *cleanup = CTrapCleanup::New ();
	CFaxTransfer *faxtransfer =reinterpret_cast<CFaxTransfer *>(session);
	RSemaphore aSemaphore;
	TFindSemaphore mysemaphore(_L("FaxCli*"));

    TFullName      theName (_L("FaxCliSem"));
//  RSemaphore     theSem;             // derived from RHandleBase
    
  if ((mysemaphore.Next(theName))==KErrNone)
        {
        aSemaphore.Open(mysemaphore,EOwnerThread);
        }


	TRAP(m,faxtransfer->Convert2dL());

	aSemaphore.Signal();	
	return KErrNone;
	}
*/
