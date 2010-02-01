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

const TInt KClass20HangupStatusTimer=20;	// < Time used to wait for final +FHS: report.  Was 5s, but typical GSM delays range between 4s and 10s.

// this module has two parts
// first receive routines rx
// second transmit routines tx

/********************************************************************/

CFaxModemDriver* CFaxClass20::NewLC(TFaxServerSessionSettings * aFaxServerSessionSettings, RFax::TProgress & aProgress)
	{
	CFaxModemDriver* self = new(ELeave) CFaxClass20;
	CleanupStack::PushL(self);
	self->ConstructL(aFaxServerSessionSettings, aProgress);
	return self;
	}

CFaxModemDriver* CFaxClass20::NewL(TFaxServerSessionSettings * aFaxServerSessionSettings, RFax::TProgress & aProgress)
	{
	CFaxModemDriver* self = NewLC(aFaxServerSessionSettings, aProgress);
	CleanupStack::Pop();
	return self;
	}
/********************************************************************/

TInt CFaxClass20::RxConnectL()
	{
	__FLOG_FAXSRV( _L8("CFaxClass20::RxConnectL entering"));

	CheckCadenceExportL (_L8 ("AT+FNR=1,1,1,0\r"));
	if ((iModem->GetMatchL (_L8 ("OK"), 5)) == 0)
		return (KFaxErrModemNotWorking);

	CheckCadenceExportL(_L8("AT+FCC=?\r"));
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

	if (iFaxServerSessionSettings->iPreferredECM == 0)
		iModem->iProgress.iECM = 0;

	iResults.Copy(_L8("AT+FCC=0,0,0,2,0,0,0,0\r"));

	iResults[7] = (TUint8) (iResults[7] + iModem->iProgress.iResolution);
	iResults[9] = (TUint8) (((iActualFaxSpeed / 2400) - 1) + '0');
	iResults[15] = (TUint8) (iResults[15] + iModem->iProgress.iCompression);
	iResults[17] = (TUint8) (iModem->iProgress.iECM + '0');
	CheckCadenceExportL(iResults);
	if ((iModem->GetMatchL(_L8("OK"), 3)) == 0)
		{
		return (KFaxErrModemNotWorking);
		}
	CheckCadenceExportL(_L8 ("AT+FAA=0\r"));
	if ((iModem->GetMatchL(_L8("OK"), 5)) == 0)
		{
		return (KFaxErrNoReceiveMode);
		}

	TBuf8<RCall::KFaxIdUserNameMaxSize> narrowBuf;
	narrowBuf.Copy(iFaxServerSessionSettings->iFaxId);	 // convert from unicode to narrow
	
	//-- By Dmitry Lyokhin. concerns PIA-58ELQK defect.
	if( narrowBuf.Length() < 1 )
		{
		narrowBuf.Append(' ');
		}

	if (iFaxServerSessionSettings->iMode & KFaxPoll)
		{
		iModem->ExportL(_L8 ("AT+FSP=1\r"));
		if ((iModem->GetMatchL(_L8("OK"), 5)) == 0)
			{
			return (KFaxPollingUnsupported);
			}
		iModem->ExportL(_L8("AT+FPI=\""));
		iModem->ExportL(narrowBuf);
		iModem->ExportL(_L8 ("\"\r"));
		if ((iModem->GetMatchL(_L8("OK"), 3)) == 0)
			{
			return (KFaxPollingUnsupported);
			}
		}
	else
		{
		CheckCadenceExportL(_L8("AT+FCR=1\r"));
		if ((iModem->GetMatchL(_L8("OK"), 5)) == 0)
			{
			return (KFaxErrNoReceiveMode);
			}
		CheckCadenceExportL(_L8("AT+FLI=\""));
		iModem->ExportL(narrowBuf);
		iModem->ExportL(_L8("\"\r"));
		if ((iModem->GetMatchL(_L8("OK"), 3)) == 0)
			{
			return (KFaxErrModemNotWorking);
			}
		}

	if (iFaxServerSessionSettings->iMode & KFaxWaitForRing)
		{
//		while ((iModem->GetMatchL(_L8("RING"), 3)) == 0);
//		iTimeOfLastRing.UniversalTime();
		}
	else
		{
		if ((iFaxServerSessionSettings->iMode & KFaxOffHook) == 0)
			{
			DialFaxOnDemandL();
			}
		}

	if ((iFaxServerSessionSettings->iMode & KFaxPoll) == 0)
		{
		CheckCadenceExportL(_L8("ATA\r"));
		}

	iModem->iProgress.iPhase = ECallEstablishment;

	TInt pollDocsAvailable = 0;

	for (;;)
		{
		if(!(iModem->ImportL(iResults, 35)))
			{
			return KFaxErrCannotAnswer;
			}

		iModem->iOurMessage.Format(_L8("%S"), &iResults);
		iModem->ProgressUpdateL();

		if ((iResults.FindF(_L8("NO DIALTONE"))) >= 0 ||
			iResults.FindF(_L8("NO DIAL TONE")) >= 0)
			return (KFaxErrNoDialTone);
		if ((iResults.FindF(_L8("BUSY"))) >= 0)
			return (KFaxErrBusy);
		if ((iResults.FindF(_L8("NO ANSWER"))) >= 0)
			return (KFaxErrNoAnswer);
		if ((iResults.FindF(_L8("NO CARRIER"))) >= 0)
			return (KFaxErrNoCarrier);
		if ((iResults.FindF(_L8("OK"))) >= 0)
			break;
		if ((iResults.FindF(_L8("FHS"))) >= 0)
			return (KFaxErrPrematureHangup);
		if ((iResults.FindF(_L8("FCO"))) >= 0)
			iModem->iProgress.iPhase = ESessionNegotiation;
		else if ((iResults.FindF(_L8("FPO"))) >= 0)
			pollDocsAvailable = 1;
		else if ((iResults.FindF(_L8("FTI"))) >= 0)
			ExtractAnswerback(iResults);
		else if ((iResults.FindF(_L8("FCS"))) >= 0)
			ParseResults(iResults);
		}

	if ((iFaxServerSessionSettings->iMode & KFaxPoll) && (pollDocsAvailable == 0))
		return (KFaxNothingToPoll);

	iModem->iOurMessage.Format(_L8 ("about to receive fax"));
	iModem->ProgressUpdateL();
	return RxPrePageL();
	}

/********************************************************************/
TInt CFaxClass20::RxPrePageL()
	{
	__FLOG_FAXSRV( _L8("CFaxClass20::RxPrePageL entering"));

	iModem->ExportL(_L8 ("AT+FDR\r"));
	for (;;)
		{
		if (!(iModem->ImportL (iResults, 35)))
			return (KFaxErrCannotConnect);

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("ERROR"))) >= 0)
			return (KFaxErrNoReceiveMode);
		if ((iResults.FindF (_L8 ("FHS"))) >= 0)
			return (KFaxErrPrematureHangup);
		if ((iResults.FindF (_L8 ("CONNECT"))) >= 0)
			break;
		if ((iResults.FindF (_L8 ("FCS"))) >= 0)
			ParseResults (iResults);
		}

	iModem->TxcharL (Kdc2);
	return (RxStartPageL ());
}
/********************************************************************/
TInt CFaxClass20::RxPostPageL ()
{
    __FLOG_FAXSRV( _L8("CFaxClass20::RxPostPageL entering"));

	iModem->iProgress.iPhase = EPostPageStatus;
	iModem->ProgressUpdateL ();
	for (;;)
		{
		if (iModem->iProgress.iECM == 0)
			{
			if (!(iModem->ImportL (iResults, 20)))
				return (KFaxErrModemResponse);
			}
        else
            {
            // coverity[check_return]
            iModem->ImportL (iResults, 0x7fff);     // no timeout if ECM enabled
            }

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("OK"))) >= 0)
			return (KFaxErrPrematureOK);
		if ((iResults.FindF (_L8 ("FHS"))) >= 0)
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
		if (iModem->GetMatchL (_L8 ("FHS"), KClass20HangupStatusTimer) == 0)
			return (KFaxErrNoHangup);
		if (iModem->GetMatchL (_L8 ("OK"), 5) == 0)
			return (KFaxErrNoFinalOK);
		return (KErrNone);
		}
	return (KFaxErrUnknownPageCode);
}
/********************************************************************/

TInt CFaxClass20::TxConnectL ()
{
    __FLOG_FAXSRV( _L8("CFaxClass20::TxConnectL entering"));

	iModem->ExportL (_L8 ("AT+FNR=1,1,1,0\r"));
	if ((iModem->GetMatchL (_L8 ("OK"), 5)) == 0)
		return (KFaxErrModemNotWorking);
	TBuf8<RCall::KFaxIdUserNameMaxSize> narrowBuf;
	narrowBuf.Copy(iFaxServerSessionSettings->iFaxId);	 // convert from unicode to narrow
	
	//-- By Dmitry Lyokhin. concerns PIA-58ELQK defect.
	if( narrowBuf.Length() < 1 ) narrowBuf.Append(' ');

	
	iModem->ExportL (_L8 ("AT+FLI=\""));
	iModem->ExportL (narrowBuf);
	iModem->ExportL (_L8 ("\"\r"));
	if ((iModem->GetMatchL (_L8 ("OK"), 3)) == 0)
		return (KFaxErrModemNotWorking);

	iModem->ExportL (_L8 ("AT+FCC=?\r"));
	if (!(iModem->ImportL (iResults, 35)))
		return (KFaxErrModemNotWorking);

	iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
	iModem->ProgressUpdateL ();
	ParseResults (iResults);

	iModem->GetMatchL (_L8 ("OK"), 2);

	iActualFaxSpeed = iModem->iProgress.iSpeed;
	if (iActualFaxSpeed > iFaxServerSessionSettings->iMaxSpeed)
		iActualFaxSpeed = iFaxServerSessionSettings->iMaxSpeed;

	if (iFaxServerSessionSettings->iPreferredECM == 0)
		iModem->iProgress.iECM = 0;

	iResults.Copy (_L8 ("AT+FCC=0,0,0,2,0,0,0,0\r"));

	iResults[7] = (TUint8) (iResults[7] + iFaxServerSessionSettings->iTxResolution);
	iResults[9] = (TUint8) (((iActualFaxSpeed / 2400) - 1) + '0');

	
	// added to support 2Dfaxing using class 2.0
	if (iModem->iProgress.iCompression)
		iResults[15] = (TUint8) (iResults[15] + iFaxServerSessionSettings->iTxCompression);
	else
		iResults[15] = (TUint8) (iResults[15] + EModifiedHuffman);
	
	//iResults[15] = (TUint8) (iResults[15] + iFaxServerSessionSettings->iTxCompression);
	
		
	iResults[17] = (TUint8) (iModem->iProgress.iECM + '0');
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
		if ((iResults.FindF (_L8 ("FHS"))) >= 0)
			return (KFaxErrNoNegotiate);
		if ((iResults.FindF (_L8 ("OK"))) >= 0)
			break;
		if ((iResults.FindF (_L8 ("FCO"))) >= 0)
			iModem->iProgress.iPhase = ESessionNegotiation;
		else if ((iResults.FindF (_L8 ("FCI"))) >= 0)
			ExtractAnswerback (iResults);
		}
	return (TxPrePageL ());
}
/********************************************************************/
TInt CFaxClass20::TxPrePageL ()
{
    __FLOG_FAXSRV(_L8("CFaxClass20::TxPrePageL entering"));

	iModem->ExportL (_L8 ("AT+FDT\r"));
	for (;;)
		{
		if (!(iModem->ImportL (iResults, 30)))
			return (KFaxErrNoNegotiate);

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("FHS"))) >= 0)
			return (KFaxErrNoNegotiate);
		if ((iResults.FindF (_L8 ("CONNECT"))) >= 0)
			break;
		if ((iResults.FindF (_L8 ("FCS"))) >= 0)
			ParseResults (iResults);
		}

	for (;;)
		{
		TInt XonTimeoutSec = CLK_TCK * 3; 
		
		if ((iModem->RxcharWaitL(XonTimeoutSec)) == 0)
			{
			break;
			}

		if (iModem->iReadone[0] == Kxon)
			{
			break;
			}
		}
	return (TxStartPageL());
}
/********************************************************************/

//
 // TxPostPageL should return either with
 //
 // a) an error code and iPhase set to RFax::EPostPageStatus, in which case the send returns with the error
 // b) KErrNone and iPhase set to RFax::EDataTransfer, in which case we send the next page
 // c) KErrNone and iPhase set to RFax::EDisconnection, in which case the send returns with KErrNone
 //

TInt CFaxClass20::TxPostPageL ()
{
	if (iModem->iProgress.iCompression == EModifiedRead)
		{
		iModem->iTransmitBuffer.Append (0x00);
		iModem->iTransmitBuffer.Append (0x60);
		iModem->iTransmitBuffer.Append (0x00);
		iModem->iTransmitBuffer.Append (0x0C);
		iModem->iTransmitBuffer.Append (0x80);
		iModem->iTransmitBuffer.Append (0x01);
		iModem->iTransmitBuffer.Append (0x30);
		iModem->iTransmitBuffer.Append (0x00);
		iModem->iTransmitBuffer.Append (0x06);
		iModem->iTransmitBuffer.Append (0xC0);
		}
	else
		{
		for (TInt x = 3; x; x--)
			{
			iModem->iTransmitBuffer.Append (0x0);
			iModem->iTransmitBuffer.Append (0x08);
			iModem->iTransmitBuffer.Append (0x80);
			}
		}

	iModem->iTransmitBuffer.Append (Kdle);
	iModem->iOurMessage.Format (_L8 ("RTC transmitted after %d lines"), iModem->iProgress.iLines);
	iModem->ProgressUpdateL ();
	if (iFaxServerSessionSettings->iTxPages == iModem->iProgress.iPage)
		{
		iModem->iTransmitBuffer.Append (0x2e);
		iModem->iOurMessage.Format (_L8 ("End of document transmitted <dle><eop>"));
		}
	else
		{
		iModem->iTransmitBuffer.Append (0x2c);
		iModem->iOurMessage.Format (_L8 ("End of page %u transmitted <dle><mps>"), iModem->iProgress.iPage);
		}
    __FLOG_FAXSRV( iModem->iOurMessage);

	iModem->CommitTransmitBufferL ();

	iModem->iProgress.iPhase = EPostPageStatus;
	iModem->ProgressUpdateL ();
	while (iModem->Txstat () != 0)
		;
	iModem->Xonoff ();

	// we've just ended phase C data, so we need to wait for the modem to respond with OK or ERROR

	for (;;)
		{
		if (iModem->iProgress.iECM == 0)
			{
			if (!(iModem->ImportL (iResults, (32 * 1024) / (iModem->iProgress.iSpeed / 10))))
				return (KFaxErrCannotEndData);
			}
        else
            {
            // coverity[check_return]
            iModem->ImportL (iResults, 0x7fff);     // no timeout if ECM enabled, but NO CARRIER possible from some mobile phones
            if ((iResults.FindF (_L8 ("NO CARRIER"))) >= 0)
                return (KFaxErrCannotEndData);
            }

		iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
		iModem->ProgressUpdateL ();

		if ((iResults.FindF (_L8 ("ERROR"))) >= 0)
			break;
		if ((iResults.FindF (_L8 ("OK"))) >= 0)
			break;
		}
	if (((iResults.FindF (_L8 ("ERROR"))) >= 0) && (iRepeatPage == 0))
		{
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

