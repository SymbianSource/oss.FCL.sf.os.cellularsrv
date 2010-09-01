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
// CFaxModem class implementation
// 
//

/**
 @file
*/


#include "FAXSERV.H"
#include <e32hal.h>
#include "FAXMODEM.H"

#include "FAXLOG.H"

// we embed this so that the session log tells us the version used

#define FaxTransVersionString "FAX SERVER SESSION LOG (Release 033)" 

/********************************************************************/

CFaxModem::CFaxModem (RFax::TProgress & aProgress)      
:iProgress (aProgress)
   {
   }
/********************************************************************/

CFaxModem *CFaxModem::NewLC (TFaxServerSessionSettings * aFaxServerSessionSettings, RFax::TProgress & aProgress)
   {
   CFaxModem *self = new (ELeave) CFaxModem (aProgress);
   CleanupStack::PushL (self);
   self->ConstructL (aFaxServerSessionSettings);
   return self;
   }
/********************************************************************/

CFaxModem *CFaxModem::NewL (TFaxServerSessionSettings * aFaxServerSessionSettings, RFax::TProgress & aProgress)
   {
   CFaxModem *self = NewLC (aFaxServerSessionSettings, aProgress);
   CleanupStack::Pop ();
   return self;
   }
/********************************************************************/

void CFaxModem::ConstructL (TFaxServerSessionSettings * aFaxServerSessionSettings)
   {
   TInt r;
//   TBufC < 24 > logname (_L ("C:\\SYSTEM\\FAXLOG.TXT"));
//   RFsBase closefile;

    __FLOG_FAXSRV( _L8("CFaxModem::ConstructL entering"));

   iVerbose = 1;

// we start off with our timer functions

   iStartTime.UniversalTime();

// work out the granularity of the clock

   TTimeIntervalMicroSeconds32 ourGranularity;
   if (UserHal::TickPeriod (ourGranularity) == KErrNotSupported)
      User::Leave (KErrNotSupported);
   iGranularity = ourGranularity.Int ();

// calibrate a 2 ms timing loop - see notes at start of cfaxTransfer::ExportL
// this is only used for class 1 when iTimingLoopDelay is true

   iCalls = 0;
   TInt t;
   t = clock () + iGranularity;
   while (t > clock ())
	   ;        // wait for the clock to tick once
   t = clock () + iGranularity; // reset target
   while (t > clock ())         // wait for it to tick again
      iCalls++;                 // this time counting calls to clock
   iCalls *= (CLK_TCK / iGranularity);  // work out calls per second
   iCalls /= 500;               // and work out calls per 2 ms
   if (iCalls == 0)
      iCalls++;

#if defined (__WINS__)
#define PDD_NAME _L("ECDRV")
#define LDD_NAME _L("ECOMM")
#else
#define PDD_NAME _L("EUART1")
#define LDD_NAME _L("ECOMM")
#endif

   r = iFileSession.Connect (); // we may need the RFs link for PDD/LDD loading as well as the log
   if (r)
      User::Leave (KFaxFileSessionError);
   iFileSessionOpen = ETrue;

   if (iVerbose)
      {
      
      
//This is a log file that would normally only be created in debug builds, however due to the time critical nature of fax it is always generated
//to avoid timing differences between debug and release builds.
//With the introduction of platform security it not advisable to have a log file in a public directory so will now be created in the private dir 
//of the fax server.

	//create log in private path of server
	TFileName logfile;
	TDriveUnit driveUnit(EDriveC);	
	TDriveName drive=driveUnit.Name();
	logfile.Insert(0, drive);	
	//append private path
	TPath privatePath;
	iFileSession.PrivatePath(privatePath);
	logfile.Append(privatePath);
	//append subdir & file name
	logfile.Append(_L("fax\\FAXLOG.TXT"));	
	
	//generate dir
	TInt kerr(iFileSession.MkDirAll(logfile)); //the directory may not exist, So create one.
	if(kerr != KErrAlreadyExists)
		{
		User::LeaveIfError(kerr);
		}
	r = iFile.Replace(iFileSession, logfile, EFileWrite | EFileShareAny);
	if (r)
		{
		User::Leave (KFaxLogCreateError);	
		}
	iFileOpen = ETrue;

      }
/*
   r = User::LoadPhysicalDevice (PDD_NAME);
   if ((r != KErrNone) && (r != KErrAlreadyExists))
     User::Leave (KFaxPDDError);

   r = User::LoadLogicalDevice (LDD_NAME);
   if ((r != KErrNone) && (r != KErrAlreadyExists))
      User::Leave (KFaxLDDError);  
*/

   RCommServ s;
   r = s.Connect ();
   if (r)
      User::Leave (KFaxCommsServerError);

   
   r = s.LoadCommModule (aFaxServerSessionSettings->iPortDriverName);
   if (r)
      User::Leave (KFaxCommsPortError);   

   r = iCommSession.Open (s, aFaxServerSessionSettings->iCommPortName, ECommShared);
   if (r)
      User::Leave (KFaxCannotOpenPort);  

   iPortOpen = ETrue;

   iCommSession.Config (iRS232Settings);

   iRS232Settings ().iRate = EBps19200;

   iRS232Settings ().iHandshake = 0;

   iRS232Settings ().iTerminator[0] = Ketx;
   iRS232Settings ().iTerminator[1] = Kxon;
   iRS232Settings ().iTerminator[2] = Klinefeed;
   iRS232Settings ().iTerminatorCount = 3;

   r = iCommSession.SetConfig (iRS232Settings);
   if (r)
      User::Leave (KFaxConfigError);

   iCommSession.SetSignals (KSignalDTR, 0);
   iCommSession.SetSignals (KSignalRTS, 0);

   iCommSession.SetReceiveBufferLength (KBufSize);
   iCommSession.ResetBuffers();
	
   iTransmitBuffer.Zero ();
   iSendone.Zero ();
   SendL (iSendone);            // activate port -  switch on DTR/RTS
   iSendone.SetMax ();          // we send single characters via iSendone[0]

   TBuf8 < 20 > thisclass;
   thisclass.SetMax ();

   iOurMessage.Format (_L8 (FaxTransVersionString));
   ProgressUpdateL ();
   iOurMessage.Format (_L8 ("The clock ticks at %d Hz"), CLK_TCK / iGranularity);
   ProgressUpdateL ();
   iOurMessage.Format (_L8 ("Calibration of 2 ms timer at %u calls"), iCalls);
   ProgressUpdateL ();

   Speed (EBps19200);
   iOurMessage.Format (_L8 ("Initialising at 19200 bps ...."));
   ProgressUpdateL ();


// added by JerryC from faxtrans cfaxmodm.cpp

   
	  Silence (CLK_TCK);
      TxcharL (Kreturn);
      Silence ();
//	  Silence ();
//	  Silence ();
	  //  GetMatchL(_L8("OK"),3);
	  

//   while (iCommSession.QueryReceiveBuffer()!=0) GetMatch (_L8 ("OK"), 1);
//   ExportL (_L8 ("AT+FCLASS=?\r"));
//   GetMatchL (_L8 ("OK"), 3);
      
//   GetMatchL (_L8 ("OK"), 3);


   //r = CLK_TCK;
   //while (RxcharWaitL (r));

// the addition ends here

//   TxcharL (Kreturn);

   iModemSet = ETrue;
   iTimingLoopDelay = EFalse;

   //-- Switch modem into command mode and clear its buffer. 
   SetModemCommandModeL(); 

   if ((aFaxServerSessionSettings->iFaxClass) == (TFaxClass) EClassAuto)
      {
      ExportL (_L8 ("AT+FCLASS=?\r"));
      if(0 == ImportL (thisclass, 2))
    	  {
    	  User::Leave (KFaxErrWrongModemType);
    	  }

      iOurMessage.Format (_L8 ("%S"), &thisclass);
      ProgressUpdateL ();
      if ((thisclass.FindF (_L8 ("ERROR"))) >= 0)
         User::Leave (KFaxErrWrongModemType);
      GetMatchL (_L8 ("OK"), 1);

      if ((thisclass.FindF (_L8 ("2.0"))) >= 0)
         {
         aFaxServerSessionSettings->iFaxClass = EClass2point0;
         }
      else if ((thisclass.FindF (_L8 ("2.1"))) >= 0)
         {
         aFaxServerSessionSettings->iFaxClass = EClass2point0;
         }
      else if ((thisclass.FindF (_L8 ("2"))) >= 0)
         {
         aFaxServerSessionSettings->iFaxClass = EClass2;
         }
      else if ((thisclass.FindF (_L8 ("1"))) >= 0)
         {
         aFaxServerSessionSettings->iFaxClass = EClass1;
         }
      else if ((thisclass.FindF (_L8 ("1.0"))) >= 0)
         {
         aFaxServerSessionSettings->iFaxClass = EClass1;
         }
      else
         User::Leave (KFaxErrWrongModemType);

      User::Leave (KFaxOnlyJustAutoDetected);
      }

   thisclass.Copy (_L8 ("AT+FCLASS="));
   if (aFaxServerSessionSettings->iFaxClass == EClass2point0)
      {
      thisclass.Append (_L8 ("2.0\r"));
      }
   else if (aFaxServerSessionSettings->iFaxClass == EClass2)
      {
      thisclass.Append (_L8 ("2\r"));
      }
   else if (aFaxServerSessionSettings->iFaxClass == EClass1)
      {
      thisclass.Append (_L8 ("1\r"));
      iTimingLoopDelay = ETrue;
      }
   else
      User::Leave (KFaxErrWrongModemType);

   Silence ();
   ExportL (thisclass);
   if ((GetMatchL (_L8 ("OK"), 1)) == 0)		
      User::Leave (KFaxErrWrongModemType);

   //Speed (EBps19200);

   if (aFaxServerSessionSettings->iFaxInitString.Length ())
      {
      ExportL (aFaxServerSessionSettings->iFaxInitString);
      ExportL (_L8 ("\r"));
      //GetMatchL (_L8 ("OK"), 3);
      }
	else 
		{
		ExportL (_L8 ("AT\r"));
		}
   if ((GetMatchL (_L8 ("OK"), 3)) == 0)
      User::Leave (KFaxBadInitialization);
   
    __FLOG_FAXSRV( _L8("CFaxModem::ConstructL exiting"));

   }
  
/********************************************************************/

CFaxModem::~CFaxModem ()
	{
//	iOurMessage.Format (_L8 ("Entering CFaxModem Destructor"));
//	ProgressUpdateL ();
	
    __FLOG_FAXSRV( _L8("CFaxModem::~CFaxModem"));

	iCancel = 2;

	if (iModemSet)
		{
		Xonoff ();
		Dropdtr ();
		
		TRAPD(ret, SetModemCommandModeL()); //-- Switch modem into command mode and clear its buffer. 
		TRAP (ret, HangupProcedureL());		//-- Issue hung up commands
		
		Silence ();
		}

	if (iPortOpen)
		{
		iCommSession.ResetBuffers ();
		iCommSession.Close ();
		}

	if (iFileOpen)
		{
		iFile.Close ();
		}

	if (iFileSessionOpen)
		iFileSession.Close ();
	}

/**
* Hang up. Senda 'ATH' command to the modem and sets Echo off, fax class = 0
*
* @note		This function can leave
*/
void CFaxModem::HangupProcedureL()
	{
	ExportL (_L8 ("ATH\r"));
	GetMatchL (_L8 ("OK"), 2);
	ExportL (_L8 ("ATE0+FCLASS=0\r"));  // Defect fix PBN-4ZLLX7, NM 23/07/01
	GetMatchL (_L8 ("OK"), 2);
	ExportL (_L8 ("\r"));
	}


/**
* Switch modem into command mode and clear its buffer. 
* LYN-585JMR defect fix. By Dmitry Lyokhin. 11.03.02
*
* @note		This function can leave
*/
void CFaxModem::SetModemCommandModeL(void)
{
	const TInt K_uS_ComWait=500000;			//-- used in cleaning up buffer. 0.5 sec timeout.
	
	ExportL (_L8("+++")); //-- switch the  modem to command mode
   
  	//-- clear modem's buffer by reading data
    while(iCommSession.QueryReceiveBuffer() > 0)
	 {
	  iCommSession.Read (iRecstat, K_uS_ComWait ,iReceiveBuffer);    // read data from the serial port
      User::WaitForRequest (iRecstat);
	 }

	Silence();
}

/********************************************************************/
/*
TBool CFaxModem::CheckModemL (TFaxSettings * aFaxSettings)
   {
   TInt len = aFaxSettings->iDialStoreData.iModemInitString.Length ();
   for (TInt attempt = 0; attempt < 3; attempt++)
      {
      ExportL (_L8 ("ATH\r"));
      GetMatchL (_L8 ("OK"), 1);
      if (len)
         {
         ExportL (aFaxSettings->iDialStoreData.iModemInitString);
         ExportL (_L8 ("\r"));
         GetMatchL (_L8 ("OK"), 1);
         }
      iOurMessage.Format (_L8 ("ATE0X0Q0V1M%uL%uS0=0S8=%u\r"),
                          (TUint) aFaxSettings->iDialStoreData.iSpeaker,
                          (TUint) aFaxSettings->iDialStoreData.iSpeakerVolume,
                          aFaxSettings->iDialStoreData.iPauseTime);
      ExportL (iOurMessage);
      if (GetMatchL (_L8 ("OK"), 1) != 0)
         {
         Silence ();            // allow s-l-o-w modems to catch up with us
         return (ETrue);
         }
      }
   return (EFalse);
   }*/
/********************************************************************/


/**
*
* @return	number of microseconds elapsed since calling CFaxModem::ConstructL().
*			
*/
TInt CFaxModem::clock ()
   {
   TTime now;
   now.UniversalTime ();
   TInt64 runningTime = now.MicroSecondsFrom (iStartTime).Int64 ();
   return I64LOW(runningTime);
   }

/********************************************************************/

/**
*	Searches for a given substring in modem's answer.
*	also calls ProgressUpdate().
*
* @param	aMatstring	- sample string.
* @param	aMattime	- timeout in seconds.
*
* @see	ImportL function
*
* @return	0 -	given string (see param aMatstring) is found in modem answer.
*       	1 - sample not found or answer timeout.
*
* @leave	This function can leave
*/
TInt CFaxModem::GetMatchL (const TDesC8 & aMatstring, TInt aMattime)

   {
   TBuf8 < 64 > modemstring;
   TBuf8 < 64 > copymodemstring;
   modemstring.SetMax ();

   while (ImportL (modemstring, aMattime))
      {
      copymodemstring.Copy (modemstring);
      iOurMessage.Format (_L8 ("%S"), &copymodemstring);
      ProgressUpdateL ();

      if ((modemstring.FindF (aMatstring)) >= 0)
         return (1);


      }
   ProgressUpdateL ();
   return (0);
   }


/********************************************************************/
/**
*	Gets textual answer from the modem.
*	Calls SubImportL() and ProgressUpdateL().
*
* @param	aLine	-	ref. to the string descriptor that will accept data from modem if any.
						will contain string in upper case without terminating 0x0d, 0x0a. 
* @param	aTimeout -	Timeout in seconds.
*
* @see	SubImport function
*
* @return	0 - if timeout occured or wrong input descriptor. Otherwise -
*				number of characters in string descriptor.
*
* @note		This function can leave
*/
TInt CFaxModem::ImportL (TDes8 & aLine, TInt aTimeout)
{
	const	TInt SubTimeout = 3; //-- modem response 3 seconds timeout 
	TInt	linestate		= 0;
	TInt	CurrTimeout;
	
   while( aTimeout > 0 )
   {
   	 if (aTimeout <= SubTimeout)	CurrTimeout = aTimeout;
	 else							CurrTimeout = SubTimeout;

     linestate = SubImportL (aLine, CurrTimeout);
    
		if (linestate) 
		{//-- a response from modem received
            __FLOG_FAXSRV2(_L8("mdm imp[%d]: %S"), aTimeout, &aLine);
		
			//-- look for '+CREG' or '+CGREG' answers from modem.
			//-- if found, ignore them and continue waiting.
			//-- these unwanted answers represent network registration status (unsolicited result code)
			//-- and interfere with fax modem responses. For more information see mm.tsy and gprs.tsy
			if ((aLine.FindF(_L8("REG"))) >= 0)
			{
				aLine.FillZ();
				aLine.Zero ();
			}
			else break; 
	 
		}//if (linestate) 

		aTimeout -= SubTimeout;

		ProgressUpdateL ();
    }// while
	
	if (!linestate)
		{
        __FLOG_FAXSRV2(_L8("mdm imp[%d]: %S"), aTimeout, &aLine);
		}

   return (linestate);
}


/********************************************************************/

/**
*	Gets textual answer from the modem.
* 
* @param	aLine	- ref. to the string descriptor that will accept data from modem if any.
*						will contain string in upper case without terminating 0x0d, 0x0a
* @param	aTimeout -	Timeout in seconds.
*
* @see		RxcharWait function, iReadone
*
* @return	0 - if timeout occured or wrong input descriptor. Otherwise -
*				number of characters in string descriptor
*
* @note		This function can leave
*/
TInt CFaxModem::SubImportL (TDes8 & aLine, TInt aTimeout)

   {
   TInt i;
   TInt8 k;
   TInt t;

   if (aLine.MaxLength () == 0)
      return (0);
   aLine.FillZ ();
   aLine.Zero ();

   t = aTimeout * CLK_TCK;

   do
      {
      if ((RxcharWaitL (t)) == 0)
         return (0);
      k = iReadone[0];
      }


   while (k < 32);

   for (i = 0; i < aLine.MaxLength (); i++)
      {
      aLine.Append (k);
      aLine.UpperCase ();
      t = aTimeout * CLK_TCK;
      if ((RxcharWaitL (t)) == 0)
         return (0);
      k = iReadone[0];
      if (k < 32)
         break;
      }

   if (k == 0x0d)
      for (;;)
         {
         t = CLK_TCK;
         if ((RxcharWaitL (t)) == 0)
            break;
         if (iReadone[0] == 0x0a)
            break;
         }

   return (i);
   }
/********************************************************************/

/**
Sends a textual string to the modem with delay.

@param aLine - const. ref. to the string descriptor to be sent to modem.

@see SendL function

@return number of symbols sent to modem.
			
@note This function can leave
*/
TInt CFaxModem::ExportL(const TDesC8& aLine)
	{
	__FLOG_FAXSRV1(_L8("mdm exp: %S"), &aLine);

	TInt i = 0;

   // we need to guarantee a delay here before sending characters
   // lots of modems ignore commands that come in too quickly
   // we need to wait for at least twice the clock granularity to ensure
   // a decent wait interval as the clock may be just about to tick

   // example :

   // Silence ((iGranularity * 2) + 2);

   // BUT ...

   // on Protea, the clock ticks at 64 beats per second
   // that's every 15 ms or to be precise every 15625 microseconds
   // iGranularity actually does return this value of 15625
   // our maximum wait will be 30 ms (with a minumum of 15 ms)
   // there's also a maximum 35 ms overhead on any timer call or active
   // object or whatever - so we could have a delay here of up to 65 ms

   // with the time the modem takes to respond, this leads to too much
   // unreliability as we have a strict 75 ms +/- 20% T.30 requirement
   // for class 1 modems. (For class 2 and 2.0 modems this isn't a
   // problem so we happily delay for iGranularity*2).

   // this applies also to WINS on the PC ticks 10 times per second
   // this gives us a maximum delay of 200 ms, which is rather long
   // so we resort to a timing loop we calibrate on first entry

   for (;;)
      {
      i = Rxstat ();
      if (iTimingLoopDelay != EFalse)
         {
         for (TInt x = 0; x < iCalls; x++)
            clock ();
         }
      else
         {
         Silence ((iGranularity * 2) + 2);
         }
      if (i == Rxstat ())
         break;
      }

	if (aLine.Length())
		{
#ifdef _DEBUG
		TRAPD(modemUnplugged, SendL(aLine));
		__FLOG_FAXSRV1(_L8("CFaxModem::ExportL SendL leave error code: %d"), modemUnplugged);
#else
		TRAP_IGNORE(SendL(aLine));
#endif

		iOurMessage.Format(_L8("%S"), &aLine);
		i = iOurMessage.Length();
		if ((i != 0) && (iOurMessage[i - 1] == 0x0d))
			{
			iOurMessage.Delete(i - 1, 1);
			}
		ProgressUpdateL();
		}
	return i;
	}
/********************************************************************/

void CFaxModem::Dropdtr ()
   {
   
   __FLOG_FAXSRV( _L8("CFaxModem::Dropdtr"));
   
   LowerDTR ();
   Silence (CLK_TCK);
   RaiseDTR ();
   Silence (CLK_TCK);
   }
/********************************************************************/

   
/**
* Wait approximately 75 ms
*/
void CFaxModem::Silence ()
   {
   Silence (CLK_TCK / 13);      // wait approx 75 milliseconds
   }

/********************************************************************/

/**
* Wait given number of microseconds
*/
void CFaxModem::Silence (TInt ticks) const
   {
   User::After (ticks);         // wait however long
   }
/********************************************************************/

void CFaxModem::Speed (TBps aSpeed)
   {
   iRS232Settings ().iRate = aSpeed;
   iCommSession.SetConfig (iRS232Settings);
   }
/********************************************************************/

/**
* Turns on obeying XON, XOFF characters and CTS signal
*/
void CFaxModem::Xonon ()
   {
   iRS232Settings ().iHandshake = (KConfigObeyXoff | KConfigObeyCTS | KConfigWriteBufferedComplete);
   iCommSession.SetConfig (iRS232Settings);
   }
/********************************************************************/

/**
Turns off handshaking
*/
void CFaxModem::Xonoff()
	{
	iSendone.Zero(); // sending a null descriptor
#ifdef _DEBUG
	TRAPD(modemUnplugged, SendL(iSendone)); // waits till all pending transmits have gone
	__FLOG_FAXSRV1(_L8("CFaxModem::Xonoff SendL leave error code: %d"), modemUnplugged);
#else
	TRAP_IGNORE(SendL(iSendone)); // waits till all pending transmits have gone
#endif

	iSendone.SetMax(); // we send single characters via iSendone[0]

	iRS232Settings().iHandshake = 0;
	iCommSession.SetConfig(iRS232Settings);
	}
/********************************************************************/

void CFaxModem::LowerDTR()
   {
   iCommSession.SetSignals (0, KSignalDTR);     // SetSignals(SetMask,ClearMask) ;
   }
/********************************************************************/

void CFaxModem::RaiseDTR()
   {
   iCommSession.SetSignals (KSignalDTR, 0);     // SetSignals(SetMask,ClearMask) ;
   }
/********************************************************************/

/**
* @return	0 - if there are no data waiting in driver's input buffer and receiver's buffer to be read.
*        	otherwise - number of bytes to be read.
*/
TInt CFaxModem::Rxstat (void)
   {
   if (iReceiveBuffer.Length () != 0)
      return (1);
   else
      return (iCommSession.QueryReceiveBuffer ());
   }
/********************************************************************/

/**
* Does nothing. Just returns 0.
*/
TInt CFaxModem::Txstat()
   {
   return 0;
   }
/********************************************************************/

/**
* Sends 1 byte to modem. 
*
* @param	aChar	-Byte to be sent.
*
* @note		This function can leave.
*
* @see	SendL() function
*
* @return	none.
*/
void CFaxModem::TxcharL (TUint8 aChar)
   {
   iSendone[0] = aChar;
   SendL (iSendone);
   }
/********************************************************************/


// this routine amended July 1998 to offer infinite timeouts when sending data in fax ECM

void CFaxModem::SendL(const TDesC8 & astring)
    {
    TTimeIntervalMicroSeconds32 timeout = CLK_TCK * 10;
    if ((iProgress.iECM) && (iProgress.iPhase == EDataTransfer))
        {
        iCommSession.Write(iTranstat, timeout, astring);
        User::WaitForRequest(iTranstat);
        if (iTranstat == KErrTimedOut)
            {
            TInt ticks = 1;
            while (Rxstat() != 0)
                {
                RxcharWaitL(ticks);
                if (iReadone[0] == Kcan)
                    User::Leave(KFaxErrModemDisconnect);
                }
            }
        else if (iTranstat != KErrNone)
            iCommSession.ResetBuffers(KCommResetTx);
        }
    else
        {
        iCommSession.Write(iTranstat, timeout, astring);
        User::WaitForRequest(iTranstat);
        if (iTranstat != KErrNone)
            {
            iCommSession.ResetBuffers(KCommResetTx);
            if (iTranstat == KErrTimedOut)
                User::Leave(KFaxTransmitterStalled);
            }
        }
    }
/********************************************************************/

/**
*	Transmits iTransmitBuffer by calling SendL. After sending sets buffer's
*	length to zero.
*
* @note		This function can leave.
*
* @see	SendL() function
*/
void CFaxModem::CommitTransmitBufferL ()
   {
   SendL (iTransmitBuffer);
   iTransmitBuffer.Zero ();
   }
/********************************************************************/


/**
*  Reads 1 byte from serial port (or iReceiveBuffer) and puts it to iReadone buffer.
*
* @param	atimeout	-	timeout in microseconds.
*
* @return	0 - timeout, 
*       	1 -	byte read and put into iReadone
*
* @note		This function can leave. See comments in CFAXMODM.CPP 
*
*/

// this is our basic get char function with timeout

// Note that this function *can* leave, despite not being named as an L
// function.  This is because the leave is as a result of a user request
// to end the entire fax thread and could occur at any time.  This
// function (to read a character) is used at some point by virtually
// every part of the fax system and consequently, were the normal
// convention to be followed, every function would have to be an L
// function - this destroys the whole point of having a separate
// nomenclature.  Therefore, I declare that the possibility of a leave
// as a result of the user cancel request is henceforth regarded as an
// exception to the rule than a function which leaves must be called an
// L function.  The new rule is that any fax session must be run inside
// a trap harness - not simply because the modem and port need tidying up.

// Note to the note : The check for user cancel request has been moved to
// ProgressUpdate().  However, this function can still leave if the
// rx buffer overruns, in which case we use the standard iCancel flag to
// check if it is safe to leave and do so.  The original note still applies
// but to be honest, I'm less happy about it now.

TInt CFaxModem::RxcharWaitL (TInt & atimeout)
   {
   iReadone.Zero ();
   if (atimeout == 0)
      return (0);
   if (iReceiveBuffer.Length () == 0)			//iReceiveBuffer == Receive descriptor
      {
      for (;;)
         {
         TInt timeBegin = clock ();
         iCommSession.Read (iRecstat, atimeout, iReceiveBuffer, KBufSize);    // read data from the serial port
         User::WaitForRequest (iRecstat);
         if ((iRecstat == KErrCommsOverrun) && (iCancel == 0))  // check for overrun and user cancel request
            {
            iCancel = 2;
            iCommSession.ResetBuffers (KCommResetRx);			//Reset serial port buffers
            User::Leave (KErrCommsOverrun);
            }
            
		 if ((iReceiveBuffer.Length () == 0) && (iRecstat == KErrTimedOut))
			{
			if (iCommSession.QueryReceiveBuffer () != 0 )
				{
					iCommSession.ReadOneOrMore (iRecstat, iReceiveBuffer);
					User::WaitForRequest (iRecstat);
				}
			}	
		 atimeout -= (clock () - timeBegin);					//decrement timeout 
		 
		 if (iReceiveBuffer.Length () != 0)
            break;
         if (atimeout > 0)
            continue;
         atimeout = 0;
         return (0);
         }
      if (atimeout < 1)
         atimeout = 1;
      }
   iReadone.Append (iReceiveBuffer[0]);
   iReceiveBuffer.Delete (0, 1);
   return (1);
   }
/********************************************************************/

void CFaxModem::ProgressUpdateL ()
   {
// Commented out by AMC 13/3/00.  Pending complete deletion.  Not completely removed as removal of function from
// Base has not yet propagated down Device Drivers.  Until the device drivers support it there's a chance it
// will have to be re-introduced.

//	UserHal::ResetAutoSwitchOffTimer (); // required to stop powerdown
   if (iCancel == 1)
      {
      iCancel++;
      User::Leave (KFaxCancelRequested);
      }

   TTime now;
   now.UniversalTime ();

   iProgress.iLastUpdateTime = now;

   if (iVerbose)
      {
      if (iOurMessage.Length ())
         {
#ifdef _DEBUG
         //
         // Write the log message also to the main log file...
         //
         TBuf8<256> temp;

         temp.Copy(iOurMessage);
         __FLOG_FAXSRV1(_L8("ProgressUpdateL: %S"), &temp);
#endif
		 
         TDateTime dateTime;
         TBuf8 < 16 > datestamp;
         dateTime = now.DateTime ();
         datestamp.Format (_L8 ("%02d.%02d:%02d:%06d "), dateTime.Hour (), dateTime.Minute (), dateTime.Second (), dateTime.MicroSecond ());
         iOurMessage.Insert (0, datestamp);
         iOurMessage.Append (13);
         iOurMessage.Append (10);
		 
		 if (iFileOpen)
			iFile.Write (iOurMessage);

		 iOurMessage.Zero ();
         }
      }
   }
/*********************************************************************/
