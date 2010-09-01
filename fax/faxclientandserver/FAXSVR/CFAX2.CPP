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

#include "FAXSERV.H"
#include "fax_reversebytes.h"
#include "FAXMODEM.H"
#include "FAXMDRV.H"

#include "FAXLOG.H"


const TInt KClass2HangupStatusTimer=20;	// < Time used to wait for final +FHNG: report.

// this module has three parts
// first receive routines rx
// second transmit routines tx
// third utilities

/********************************************************************/

CFaxModemDriver *CFaxClass2::NewLC (TFaxServerSessionSettings * aFaxServerSessionSettings, RFax::TProgress & aProgress)
{
	CFaxModemDriver *self = new (ELeave) CFaxClass2;
	CleanupStack::PushL (self);
	self->ConstructL (aFaxServerSessionSettings, aProgress);
	return self;
}

CFaxModemDriver *CFaxClass2::NewL (TFaxServerSessionSettings * aFaxServerSessionSettings, RFax::TProgress & aProgress)
{
	CFaxModemDriver *self = NewLC (aFaxServerSessionSettings, aProgress);
	CleanupStack::Pop ();
	return self;
}
/********************************************************************/

TInt CFaxClass2::RxConnectL ()
{
	__FLOG_FAXSRV( _L8("CFaxClass2::RxConnectL entering"));
	
	CheckCadenceExportL (_L8 ("AT+FDCC=?\r"));
	if (!(iModem->ImportL (iResults, 35)))
		return (KFaxErrModemNotWorking);

	iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
	iModem->ProgressUpdateL ();
	ParseResults (iResults);

	iModem->GetMatchL (_L8 ("OK"), 2);

	if (iFaxServerSessionSettings->iRxResolution == EFaxNormal)
		iModem->iProgress.iResolution = EFaxNormal;
	else
		iModem->iProgress.iResolution = EFaxFine;

	if (iFaxServerSessionSettings->iRxCompression == EModifiedRead)
		iModem->iProgress.iCompression = EModifiedRead;
	else
		iModem->iProgress.iCompression = EModifiedHuffman;
	iActualFaxSpeed = iModem->iProgress.iSpeed;
	if (iActualFaxSpeed > iFaxServerSessionSettings->iMaxSpeed)
		iActualFaxSpeed = iFaxServerSessionSettings->iMaxSpeed;

	iResults.Copy (_L8 ("AT+FDCC=0,0,0,2,0,0,0,0\r"));

	iResults[8] = (TUint8) (iResults[8] + iModem->iProgress.iResolution);
	iResults[10] = (TUint8) (((iActualFaxSpeed / 2400) - 1) + '0');
	iResults[16] = (TUint8) (iResults[16] + iModem->iProgress.iCompression);
	CheckCadenceExportL (iResults);
	if ((iModem->GetMatchL (_L8 ("OK"), 3)) == 0)
		return (KFaxErrModemNotWorking);
	CheckCadenceExportL (_L8 ("AT+FAA=0\r"));
	if ((iModem->GetMatchL (_L8 ("OK"), 5)) == 0)
		return (KFaxErrNoReceiveMode);
	
	TBuf8<RCall::KFaxIdUserNameMaxSize> narrowBuf;
	narrowBuf.Copy(iFaxServerSessionSettings->iFaxId);
	
	//-- By Dmitry Lyokhin. concerns PIA-58ELQK defect.
	if( narrowBuf.Length() < 1 ) narrowBuf.Append(' ');


	if (iFaxServerSessionSettings->iMode & KFaxPoll)
		{
		iModem->ExportL (_L8 ("AT+FSPL=1\r"));
		if ((iModem->GetMatchL (_L8 ("OK"), 5)) == 0)
			return (KFaxPollingUnsupported);
		iModem->ExportL (_L8 ("AT+FCIG=\""));
		iModem->ExportL (narrowBuf);
		iModem->ExportL (_L8 ("\"\r"));
		if ((iModem->GetMatchL (_L8 ("OK"), 3)) == 0)
			return (KFaxPollingUnsupported);
		}
	else
		{
		CheckCadenceExportL (_L8 ("AT+FCR=1\r"));
		if ((iModem->GetMatchL (_L8 ("OK"), 5)) == 0)
			return (KFaxErrNoReceiveMode);
		CheckCadenceExportL (_L8 ("AT+FLID=\""));
		iModem->ExportL (narrowBuf);
		iModem->ExportL (_L8 ("\"\r"));
		if ((iModem->GetMatchL (_L8 ("OK"), 3)) == 0)
			return (KFaxErrModemNotWorking);
		}

	if (iFaxServerSessionSettings->iMode & KFaxWaitForRing)
		{
//		while ((iModem->GetMatchL (_L8 ("RING"), 3)) == 0);
//		iTimeOfLastRing.UniversalTime();
		}
	else
		{
		if ((iFaxServerSessionSettings->iMode & KFaxOffHook) == 0)
			DialFaxOnDemandL ();
		}

	if ((iFaxServerSessionSettings->iMode & KFaxPoll) == 0)
		{
		iModem->Silence (CLK_TCK);
		CheckCadenceExportL (_L8 ("ATA\r"));
		}

	iModem->iProgress.iPhase = ECallEstablishment;

	TInt pollDocsAvailable = 0;

	for (;;)
		{
		if (!(iModem->ImportL (iResults, 35)))
			return (KFaxErrCannotAnswer);

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("NO DIALTONE"))) >= 0 ||
			iResults.FindF (_L8 ("NO DIAL TONE")) >= 0)
			return (KFaxErrNoDialTone);
		if ((iResults.FindF (_L8 ("BUSY"))) >= 0)
			return (KFaxErrBusy);
		if ((iResults.FindF (_L8 ("NO ANSWER"))) >= 0)
			return (KFaxErrNoAnswer);
		if ((iResults.FindF (_L8 ("NO CARRIER"))) >= 0)
			return (KFaxErrNoCarrier);
		if ((iResults.FindF (_L8 ("OK"))) >= 0)
			break;
		if ((iResults.FindF (_L8 ("FHNG"))) >= 0)
			return (KFaxErrPrematureHangup);
		if ((iResults.FindF (_L8 ("FCON"))) >= 0)
			iModem->iProgress.iPhase = ESessionNegotiation;
		else if ((iResults.FindF (_L8 ("FPOLL"))) >= 0)
			pollDocsAvailable = 1;
		else if ((iResults.FindF (_L8 ("FTSI"))) >= 0)
			ExtractAnswerback (iResults);
		else if ((iResults.FindF (_L8 ("FDCS"))) >= 0)
			ParseResults (iResults);
		}

	if ((iFaxServerSessionSettings->iMode & KFaxPoll) && (pollDocsAvailable == 0))
		return (KFaxNothingToPoll);

	iModem->iOurMessage.Format (_L8 ("about to receive fax"));
	iModem->ProgressUpdateL ();
	return (RxPrePageL ());
}

/********************************************************************/
TInt CFaxClass2::RxPrePageL ()
{
	__FLOG_FAXSRV( _L8("CFaxClass2::RxPrePageL entering"));

	iModem->ExportL (_L8 ("AT+FDR\r"));
	for (;;)
		{
		if (!(iModem->ImportL (iResults, 35)))
			return (KFaxErrCannotConnect);

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("ERROR"))) >= 0)
			return (KFaxErrNoReceiveMode);
		if ((iResults.FindF (_L8 ("FHNG"))) >= 0)
			return (KFaxErrPrematureHangup);
		if ((iResults.FindF (_L8 ("CONNECT"))) >= 0)
			break;
		if ((iResults.FindF (_L8 ("FDCS"))) >= 0)
			ParseResults (iResults);
		}

	iModem->TxcharL (Kdc2);
	return (RxStartPageL ());
}
/********************************************************************/
TInt CFaxClass2::RxPostPageL ()
{
	__FLOG_FAXSRV(_L8("CFaxClass2::RxPostPageL entering"));

	iModem->iProgress.iPhase = EPostPageStatus;
	iModem->ProgressUpdateL ();
	for (;;)
		{
		if (!(iModem->ImportL (iResults, 20)))
			return (KFaxErrModemResponse);

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("OK"))) >= 0)
			return (KFaxErrPrematureOK);
		if ((iResults.FindF (_L8 ("FHNG"))) >= 0)
			return (KFaxErrPrematureHangup);
		if ((iResults.FindF (_L8 ("FET"))) >= 0)
			break;
		}
	if (iModem->GetMatchL (_L8 ("OK"), 5) == 0)
		return (KFaxErrNoFinalOK);

	if ((iResults.FindF (_L8 ("0"))) >= 0)
		{
		iModem->iOurMessage.Format (_L8 ("page %u successfully received"), iModem->iProgress.iPage);
		iModem->ProgressUpdateL ();
		return (RxPrePageL ());
		}
	if ((iResults.FindF (_L8 ("1"))) >= 0)
		{
		iModem->iOurMessage.Format (_L8 ("page %u successfully received : end of document"), iModem->iProgress.iPage);
		iModem->ProgressUpdateL ();
		iModem->iOurMessage.Format (_L8 ("Next fax awaited"));
		iModem->ProgressUpdateL ();
		return (RxPrePageL ());
		}
	if ((iResults.FindF (_L8 ("2"))) >= 0)
		{
		iModem->iOurMessage.Format (_L8 ("Final page %u successfully received"), iModem->iProgress.iPage);
		iModem->ProgressUpdateL ();
		iModem->iProgress.iPhase = EDisconnection;
		iModem->ExportL (_L8 ("AT+FDR\r"));
		if (iModem->GetMatchL (_L8 ("FHNG"), KClass2HangupStatusTimer) == 0)
			return (KFaxErrNoHangup);
		if (iModem->GetMatchL (_L8 ("OK"), 5) == 0)
			return (KFaxErrNoFinalOK);
		return (KErrNone);
		}
	return (KFaxErrUnknownPageCode);
}
/********************************************************************/

TInt CFaxClass2::TxConnectL ()
{
	// we always use direct bit order when sending
	// we always use reversed bit order when receiving
	// this is in accordance with the rockwell bug.
	// if a modem doesn't have the rockwell bug then
	// the fax init string AT+FBOR=1 will force the
	// use of reversed bit ordering on reception.
	// since we have here a hardwired default AT+FBOR=0
	// on transmission this will override the init string
	// which will therefore only be used (as needed) for
	// reception, and everything will work again

    __FLOG_FAXSRV( _L8("CFaxClass2::TxConnectL entering"));

	iModem->ExportL (_L8 ("AT+FBOR=0\r"));
	iModem->GetMatchL (_L8 ("OK"), 3);

	TBuf8<RCall::KFaxIdUserNameMaxSize> narrowBuf;
	narrowBuf.Copy(iFaxServerSessionSettings->iFaxId);
	
	//-- if the fax local id string is empty, insert blank
	//-- PIA-58ELQK defect fix by Dmitry Lyokhin 26.03.2001
	if( narrowBuf.Length() < 1 ) narrowBuf.Append(' ');


	iModem->ExportL (_L8 ("AT+FLID=\""));
	iModem->ExportL (narrowBuf);
	iModem->ExportL (_L8 ("\"\r"));
	if ((iModem->GetMatchL (_L8 ("OK"), 3)) == 0)
		return (KFaxErrModemNotWorking);

	iModem->ExportL (_L8 ("AT+FDCC=?\r"));
	if (!(iModem->ImportL (iResults, 35)))
		return (KFaxErrModemNotWorking);

	iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
	iModem->ProgressUpdateL ();
	ParseResults (iResults);

	iModem->GetMatchL (_L8 ("OK"), 2);

	iActualFaxSpeed = iModem->iProgress.iSpeed;
	if (iActualFaxSpeed > iFaxServerSessionSettings->iMaxSpeed)
		iActualFaxSpeed = iFaxServerSessionSettings->iMaxSpeed;

	iResults.Copy (_L8 ("AT+FDCC=0,0,0,2,0,0,0,0\r"));

	iResults[8] = (TUint8) (iResults[8] + iFaxServerSessionSettings->iTxResolution);
	iResults[10] = (TUint8) (((iActualFaxSpeed / 2400) - 1) + '0');


	// added to support 2Dfaxing using class 2
	if (iModem->iProgress.iCompression==EModifiedRead)
		iResults[16] = (TUint8) (iResults[16] + iFaxServerSessionSettings->iTxCompression);
	else
		iResults[16] = (TUint8) (iResults[16] + EModifiedHuffman);

	iModem->ExportL (iResults);
	if ((iModem->GetMatchL (_L8 ("OK"), 3)) == 0)
		return (KFaxErrModemNotWorking);

	iModem->ExportL (_L8 ("ATD"));
	if ((iFaxServerSessionSettings->iMode & KFaxOffHook) == 0)
		iModem->ExportL (iFaxServerSessionSettings->iPhoneNumber);
	iModem->TxcharL (Kreturn);

	iModem->iProgress.iPhase = ECallEstablishment;
	for (;;)
		{
		if (!(iModem->ImportL (iResults, KDialTimeout)))
			return (KFaxErrNoDial);

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("NO DIALTONE"))) >= 0 ||
			iResults.FindF (_L8 ("NO DIAL TONE")) >= 0)
			return (KFaxErrNoDialTone);
		if ((iResults.FindF (_L8 ("BUSY"))) >= 0)
			return (KFaxErrBusy);
		if ((iResults.FindF (_L8 ("NO ANSWER"))) >= 0)
			return (KFaxErrNoAnswer);
		if ((iResults.FindF (_L8 ("NO CARRIER"))) >= 0)
			return (KFaxErrNoCarrier);
		if ((iResults.FindF (_L8 ("FHNG"))) >= 0)
			return (KFaxErrNoNegotiate);
		if ((iResults.FindF (_L8 ("OK"))) >= 0)
			break;
		if ((iResults.FindF (_L8 ("FCON"))) >= 0)
			iModem->iProgress.iPhase = ESessionNegotiation;
		else if ((iResults.FindF (_L8 ("FCSI"))) >= 0)
			ExtractAnswerback (iResults);
		}
	return (TxPrePageL ());
}
/********************************************************************/
TInt CFaxClass2::TxPrePageL ()
{
    __FLOG_FAXSRV( _L8("CFaxClass2::TxPrePageL entering"));

	iModem->ExportL (_L8 ("AT+FDT\r"));
	for (;;)
		{
		if (!(iModem->ImportL (iResults, 30)))
			return (KFaxErrNoNegotiate);

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("FHNG"))) >= 0)
			return (KFaxErrNoNegotiate);
		if ((iResults.FindF (_L8 ("CONNECT"))) >= 0)
			break;
		if ((iResults.FindF (_L8 ("FDCS"))) >= 0)
			ParseResults (iResults);
		}

	for (;;)
		{
		
		//TInt ticks = CLK_TCK * 10;
		//-- PIA-58ELQK defect fix by Dmitry Lyokhin 26.03.2001
		TInt XonTimeoutSec = CLK_TCK * 3; 
		
		if ((iModem->RxcharWaitL (XonTimeoutSec)) == 0)
		{
			//-- XON character sending timeout. Ignore. 
			//-- PIA-58ELQK defect fix by Dmitry Lyokhin 26.03.2001
			//return (KFaxNoClass2Xon);
			break;
		}

		if (iModem->iReadone[0] == Kxon)
			break;
		}
	return (TxStartPageL ());
}
/********************************************************************/

//
 // TxPostPage should return either with
 //
 // a) an error code and iPhase set to RFax::EPostPageStatus, in which case the send returns with the error
 // b) KErrNone and iPhase set to RFax::EDataTransfer, in which case we send the next page
 // c) KErrNone and iPhase set to RFax::EDisconnection, in which case the send returns with KErrNone
 //

TInt CFaxClass2::TxPostPageL ()
{
	
	iModem->iTransmitBuffer.Append (Kdle);
	iModem->iTransmitBuffer.Append (Ketx);
	iModem->CommitTransmitBufferL ();

	iModem->iOurMessage.Format (_L8 ("<dle><etx> transmitted after %d lines"), iModem->iProgress.iLines);
	
    __FLOG_FAXSRV(iModem->iOurMessage);

	iModem->iProgress.iPhase = EPostPageStatus;
	iModem->ProgressUpdateL ();
	while (iModem->Txstat () != 0)
		;

	// we've just ended phase C data, so we need to wait for the modem to respond with OK



	if (iModem->GetMatchL (_L8 ("OK"), (32 * 1024) / (iModem->iProgress.iSpeed / 10)) == 0)
		return (KFaxErrCannotEndData);

	iModem->Xonoff ();

	if (iFaxServerSessionSettings->iTxPages == iModem->iProgress.iPage)
		{
		iModem->ExportL (_L8 ("AT+FET=2\r"));
		iModem->iOurMessage.Format (_L8 ("End of document transmitted EOP frame"));
		}
	else
		{
		iModem->ExportL (_L8 ("AT+FET=0\r"));
		iModem->iOurMessage.Format (_L8 ("End of page %u transmitted MPS frame"), iModem->iProgress.iPage);
		}
	iModem->ProgressUpdateL ();

	for (;;)
		{
		
//#if defined (__WINS__)
//		RDebug::Print(_L("NOW"));
//#endif


		if (!(iModem->ImportL (iResults, 30)))
		{
			return (KFaxErrModemResponse);
		}

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("FHNG"))) >= 0)
			{
			if (iResults[7] == '0')
				break;
			return (KFaxErrNoNegotiate);
			}

		if ((iResults.FindF (_L8 ("FPTS"))) >= 0)
			{
				
				if (iResults.FindF (_L8("1"))==6)		// 6 is the position of the error code in descriptor
					{
						iModem->iOurMessage.Append (_L8("FPTS:1  MCF frame"));
						iModem->ProgressUpdateL ();	
						break;
					}
				else if (iResults.FindF(_L8("2"))==6)
					{
						iModem->iOurMessage.Append (_L8("FPTS:2  RTN frame"));
						iModem->ProgressUpdateL ();	
						break;
					}
				else if (iResults.FindF(_L8("3"))==6)
					{
						iModem->iOurMessage.Append (_L8("FPTS:3  RTP frame"));
						iModem->ProgressUpdateL ();	
						break;
					}
				else if (iResults.FindF(_L8("4"))==6)
					{
						iModem->iOurMessage.Append (_L8("FPTS:4  PIN frame"));
						iModem->ProgressUpdateL ();	
						break;
					}
				else if (iResults.FindF(_L8("5"))==6)
					{
						iModem->iOurMessage.Append (_L8("FPTS:5  PIP frame"));
						iModem->ProgressUpdateL ();	
						break;
					}  
				break;
	
			}
	} // for(;;) ends here

	if (iModem->GetMatchL (_L8 ("OK"), 5) == 0)
		return (KFaxErrNoFinalOK);

	if ((((iResults.FindF (_L8 ("2"))) >= 0) || (iResults.FindF (_L8 ("4")) >= 0)) && (iRepeatPage <= 3))  //RTN and 
	{																								       //PIN  responses
		iModem->iProgress.iPhase = ESessionNegotiation;
		iRepeatPage++;
		iModem->iProgress.iPage--;
		return (TxPrePageL ());
		}
	if (iFaxServerSessionSettings->iTxPages != iModem->iProgress.iPage)
		{
		iRepeatPage = 0;
		return (TxPrePageL ());
		}
	iModem->iProgress.iPhase = EDisconnection;
	return (KErrNone);
}
/********************************************************************/

// this routine parses both class 2 and class 2.0
 // T.30 session subparameter strings, from both interrogatories
 // and reports - for interrogatories, we assume that later capabilities
 // (inside the brackets) are preferable to earlier ones

// amended July 1998 to support ECM for class 2.0

void CFaxClass2::ParseResults (TDesC8 & aResults)
{
	TInt x, field, inbracket;
	static const TInt minscans[][2] =
		{
			{0, 0
			},
			{5, 5
			},
			{10, 5
			},
			{10, 10
			},
			{20, 10
			},
			{20, 20
			},
			{40, 20
			},
			{40, 40
			}
		};

	for ( x = 0, field = 0, inbracket = 0; x < aResults.Length (); x++)
		{
		if ((aResults[x] > '0' - 1) && (aResults[x] < '9' + 1))
			{
			if (field == 0)
				iModem->iProgress.iResolution = TFaxResolution (aResults[x] & 1);
			else if (field == 1)
				iModem->iProgress.iSpeed = (aResults[x] - '0' + 1) * 2400;
			else if (field == 4)
				iModem->iProgress.iCompression = TFaxCompression (aResults[x] & 1);
			else if (field == 5)
				iModem->iProgress.iECM = (aResults[x] - '0');
			else if (field == 7)
				{
				iMinscan = minscans[aResults[x] - '0'][iModem->iProgress.iResolution];
				if (iMinscan == 0)
					iMinlinelength = 0;
				else
					iMinlinelength = ((iModem->iProgress.iSpeed / (1000 / iMinscan)) / 8) + 1;
				}
			}
		if (aResults[x] == 0)
			break;
		if (aResults[x] == 0x28)
			inbracket = 1;
		if (aResults[x] == 0x29)
			inbracket = 0;
		if ((aResults[x] == 0x2c) && (inbracket == 0))
			field++;
		if (field == 8)
			break;
		}
	if (iFaxServerSessionSettings->iFaxClass == EClass2)
		iModem->iProgress.iECM = 0;       // no ECM using class 2
	if (iModem->iProgress.iECM)
		iModem->iProgress.iECM = 1;       // and treat EC as a boolean
}
/********************************************************************/

// this routine extract an ID string from a result code

void CFaxClass2::ExtractAnswerback (TDesC8 & aResults)
{
	TInt startID = aResults.Locate (0x22);
	if (startID != KErrNotFound)
		{
		startID++;
		TInt maxIDlength = (aResults.Length () - startID);
		if (maxIDlength > 20)
			maxIDlength = 20;
		iModem->iProgress.iAnswerback.Copy (aResults.Mid (startID, maxIDlength));
		}
}
/********************************************************************/
