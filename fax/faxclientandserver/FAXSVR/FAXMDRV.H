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
// Contents :  fax s/r header file
// 
//

/**
 @file
 @internalComponent 
*/

#ifndef __FAXMDRV_H
#define __FAXMDRV_H

// class 1 T.30 frame stuff

#include "FRAMES.H"
/**
@internalComponent
*/
const TInt KDialTimeout (100);

/********************************************************************/

// now the CFaxModemDriver abstract class
class CFaxModem;
class CFaxModemDriver : public CBase
/**
@internalComponent
*/
   {

 public:
   static CFaxModemDriver * NewLC (TFaxServerSessionSettings *,RFax::TProgress& aProgress);
   static CFaxModemDriver * NewL (TFaxServerSessionSettings *,RFax::TProgress& aProgress);
   void ConstructL (TFaxServerSessionSettings *,RFax::TProgress& aProgress);
    CFaxModemDriver ();
   ~CFaxModemDriver ();

   void GetFaxDataL (TDes8 *);
   TInt GetLineL (TUint8 *);
   virtual TInt RxConnectL () = 0;
   virtual TInt RxPrePageL () = 0;
   virtual TInt RxPostPageL () = 0;

   TInt SendFaxDataL (const TDesC8 *);
   virtual TInt TxConnectL () = 0;
   virtual TInt TxPrePageL () = 0;
   virtual TInt TxPostPageL () = 0;

   inline void SetSharedFileHandles(CFaxSharedFileHandles* aSharedHandles) 
	   {
	   iSharedHandles = aSharedHandles;
	   }

 protected:
   inline TUint8 Invert (TUint8);
   void DialFaxOnDemandL ();
   void WriteFaxStoreL (TInt);
   TInt RxStartPageL();
   TInt TxStartPageL();
   void SendFaxHeaderL ();
   void padLineL (TInt);
   TInt CheckCadenceExportL(const TDesC8 &);

 public:
   CFaxModem * iModem;
   TFaxServerSessionSettings * iFaxServerSessionSettings;
   TInt iRepeatPage;
   TTimeIntervalMicroSeconds iCadence;
   TTime iTimeOfLastRing;

 protected:
   TInt iActualFaxSpeed;
   TInt iMinscan;
   TInt iMinlinelength;        // worked out from iMinscan and iActualFaxSpeed
   TBuf8 < 128 > iResults;
   CFaxSharedFileHandles* iSharedHandles;
   };

/********************************************************************/

// Here we have the derived classes for various types of fax modems
// class one is the only complicated one

/********************************************************************/

class CFaxClass1 : public CFaxModemDriver
/**
@internalComponent
*/
   {

 public:
   static CFaxModemDriver * NewL (TFaxServerSessionSettings *,RFax::TProgress& aProgress);
   static CFaxModemDriver * NewLC (TFaxServerSessionSettings *,RFax::TProgress& aProgress);

   TInt RxConnectL ();
   TInt RxPrePageL ();
   TInt RxPostPageL ();
   TInt TxConnectL ();
   TInt TxPrePageL ();
   TInt TxPostPageL ();

 private:
   TInt RxPrePollL ();
   TInt RxSetHighSpeedL ();
   void RxDCNL ();
   TInt TxSetHighSpeedL ();
   TInt TxDCNL ();

   TInt GetframeL (TDes8 &);
   TInt SendframeL (TDes8 &);
   TInt GetFramestatL ();
   TInt FramestatL ();
   TInt FramestatL (TInt);
   void ReceiveSilenceL ();
   TInt SendCRPL ();
   inline void AnalyseDISL ();

   union faxparms iDisFrame;
   union faxparms iDcsFrame;
   union faxparms iDtcFrame;
   TInt iDcsBytes ;
   TInt iDisBytes ;
   TInt iFcfXbit ;             // the fcf X bit, see T.30/5.3.6.1
   TBuf8 < 16 > iModemString;  // HDLC post-frame modem status
   TBuf8 < 36 > iOldFrame;     // last frame resend for class 1
   TBuf8 < 36 > iFrame;        // current frame data space
   };
/********************************************************************/

class CFaxClass2 : public CFaxModemDriver
/**
@internalComponent
*/
   {
 public:
   static CFaxModemDriver * NewL (TFaxServerSessionSettings *,RFax::TProgress& aProgress);
   static CFaxModemDriver * NewLC (TFaxServerSessionSettings *,RFax::TProgress& aProgress);

   TInt RxConnectL ();
   TInt RxPrePageL ();
   TInt RxPostPageL ();
   TInt TxConnectL ();
   TInt TxPrePageL ();
   TInt TxPostPageL ();

 protected:
   void ParseResults (TDesC8 &) ;
   void ExtractAnswerback (TDesC8 &);
   void AnalyseFHNG(void);

   };
/********************************************************************/

class CFaxClass20 : public CFaxClass2
/**
@internalComponent
*/
   {
 public:
   static CFaxModemDriver * NewL (TFaxServerSessionSettings *,RFax::TProgress& aProgress);
   static CFaxModemDriver * NewLC (TFaxServerSessionSettings *,RFax::TProgress& aProgress);

   TInt RxConnectL ();
   TInt RxPrePageL ();
   TInt RxPostPageL ();
   TInt TxConnectL ();
   TInt TxPrePageL ();
   TInt TxPostPageL ();
   };

/********************************************************************/

#include "FAXMDRV.INL"

#endif

