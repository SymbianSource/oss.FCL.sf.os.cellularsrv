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
#include "FAXMDRV.H"
#include "FAXMODEM.H"


// this module has three parts
// first receive routines rx
// second transmit routines tx
// third utilities

/********************************************************************/

CFaxModemDriver *CFaxClass1::NewLC (TFaxServerSessionSettings * aFaxServerSessionSettings, RFax::TProgress & aProgress)
{
	CFaxModemDriver *self = new (ELeave) CFaxClass1;
	CleanupStack::PushL (self);
	self->ConstructL (aFaxServerSessionSettings, aProgress);
	return self;
}

CFaxModemDriver *CFaxClass1::NewL (TFaxServerSessionSettings * aFaxServerSessionSettings, RFax::TProgress & aProgress)
{
	CFaxModemDriver *self = NewLC (aFaxServerSessionSettings, aProgress);
	CleanupStack::Pop ();
	return self;
}
/********************************************************************/

// here we set up a fax receive - Phase A
// this does require HDLC frames to be sent

TInt CFaxClass1::RxConnectL ()
{
	TInt faxIdFcf;               // CSI or CIG
	TInt capabilityFcf;          // DIS or DTC
	TBuf8 < 3 > faxIdTxt;        // CSI or CIG
	TBuf8 < 3 > capabilityTxt;   // DIS or DTC

	TInt i, x;
	iDisBytes = 3;
	iOldFrame.Zero ();

	// we query the modem to find out what its speed capabilities are

    CheckCadenceExportL (_L8 ("AT+FRM=?\r"));
    // coverity[check_return]
    iModem->ImportL (iResults, 2);
    iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
    iModem->ProgressUpdateL ();
    iModem->GetMatchL (_L8 ("OK"), 1);

	// the available speeds are stored in iResults
	// we set our proposed speed to the highest compatible with faxini settings

	if ((iResults.FindF (_L8 ("24"))) >= 0)
		iActualFaxSpeed = 24;
	else
		return (KFaxErrModemNotWorking);
	if ((iFaxServerSessionSettings->iMaxSpeed) > 2400)
		if ((iResults.FindF (_L8 ("48"))) >= 0)
			iActualFaxSpeed = 48;
	if ((iFaxServerSessionSettings->iMaxSpeed) > 4800)
		if ((iResults.FindF (_L8 ("96"))) >= 0)
			iActualFaxSpeed = 96;
	if ((iFaxServerSessionSettings->iMaxSpeed) > 9600)
		if ((iResults.FindF (_L8 ("145"))) >= 0)
			iActualFaxSpeed = 145;

	// we now prepare our DIS/DTC answer capabilities frame
	// the resolution and compression are taken from our settings

	for (x = 0; x < 5; x++)
		iDisFrame.byte[x] = 0;
	iDisFrame.bit.b09 = 0;
	iDisFrame.bit.b10 = 1;
	iDisFrame.bit.b20 = 1;
	iDisFrame.bit.b21 = 1;
	iDisFrame.bit.b22 = 1;
	iDisFrame.bit.b23 = 1;
	if (iFaxServerSessionSettings->iRxResolution == EFaxFine)
		iDisFrame.bit.b15 = 1;
	if (iFaxServerSessionSettings->iRxCompression == EModifiedRead)
		iDisFrame.bit.b16 = 1;

//	if (iFaxServerSessionSettings->iMode & KFaxWaitForRing)
//		{
	//	while ((iModem->GetMatchL (_L8 ("RING"), 3)) == 0);
	//	iTimeOfLastRing.UniversalTime();
//		}
//	else
//		{
		if (((iFaxServerSessionSettings->iMode & KFaxOffHook) == 0) && (!(iFaxServerSessionSettings->iMode & KFaxWaitForRing)))
//		if ((iFaxServerSessionSettings->iMode & KFaxOffHook) == 0)
			DialFaxOnDemandL ();
//		}

	// if we a trying to poll, we've dialled, so we wait for a DIS from the answerer
	// otherwise we do an answer ourselves

	if (iFaxServerSessionSettings->iMode & KFaxPoll)
		{
		faxIdFcf = KT30_CIG;
		faxIdTxt.Copy (_L8 ("CIG"));
		capabilityFcf = KT30_DTC;
		capabilityTxt.Copy (_L8 ("DTC"));
		iModem->iOurMessage.Format (_L8 ("about to poll fax"));

		}
	else
		{
		faxIdFcf = KT30_CSI;
		faxIdTxt.Copy (_L8 ("CSI"));
		capabilityFcf = KT30_DIS;
		capabilityTxt.Copy (_L8 ("DIS"));
		CheckCadenceExportL (_L8 ("ATA\r"));
		iModem->iOurMessage.Format (_L8 ("about to receive fax"));
		}

	iModem->iProgress.iPhase = ECallEstablishment;
	iModem->ProgressUpdateL ();

	for (;;)
		{
		if (!(iModem->ImportL (iResults, KT30_T1)))
			return (KFaxErrCannotConnect);
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
		if ((iResults.FindF (_L8 ("CONNECT"))) >= 0)
			break;
		}

	if (iFaxServerSessionSettings->iMode & KFaxPoll)
		User::LeaveIfError (RxPrePollL ());
	else
		{
		iModem->iOurMessage.Format (_L8 ("Fax call detected"));
		iModem->ProgressUpdateL ();
		}

	iModem->iOurMessage.Format (_L8 ("sending %S"), &faxIdTxt);
	iModem->ProgressUpdateL ();
	iFrame.Zero ();
	iFrame.Append (KT30_CTLNXT);
	iFrame.Append (faxIdFcf);
	for (i = 20; i > iFaxServerSessionSettings->iFaxId.Length (); i--)
		iFrame.Append (0x20);
	for (i = iFaxServerSessionSettings->iFaxId.Length (); i;)
	iFrame.Append (iFaxServerSessionSettings->iFaxId[--i]);
	if (SendframeL (iFrame) != 1)
		return (KFaxErrCSIorCIG);

	// we follow that with our DIS frame

	iModem->iOurMessage.Format (_L8 ("sending %S"), &capabilityTxt);
	iModem->iProgress.iPhase = ESessionNegotiation;
	iModem->ProgressUpdateL ();
	iDisFrame.byte[1] &= 0xc3;
	switch (iActualFaxSpeed)
		{
		case 48:
			iDisFrame.byte[1] |= 0x08;
			break;                 /* V.27 4800+2400            */
		case 96:
			iDisFrame.byte[1] |= 0x0c;
			break;                 /* & V.29 9600+7200            */
		case 145:
			iDisFrame.byte[1] |= 0x2c;
			break;                 /* & V.17 14400+1200+9600+7200 */
		default:
			iDisFrame.byte[1] |= 0x00;     /* V.27 fallback 2400 only     */
		}
	iFrame.Zero ();
	iFrame.Append (KT30_CTLLST);
	iFrame.Append (capabilityFcf);
	for (i = 0; i < iDisBytes; i++)
		iFrame.Append (iDisFrame.byte[i]);
	if (SendframeL (iFrame) != 1)
		return (KFaxErrDISorDTC);

	// and now we await the negotiation from the caller
	// note that we'll resend the last frame (DIS or DTC) if we get no reply
	// until we get a TSI or a DCS (which show the DIS or DTC was received)

	return (RxPrePageL ());
}

/********************************************************************/

// here we prepare for receiving via a poll
 // we have received a DIS in iResults, so check the polling bit
 // iResults[0] is the address
 // iResults[1] is the control
 // iResults[2] is the FCF
 // iResults[3] has bits 1-8 of the FIF
 // iResults[4] has bits 9-16 of the FIF
 // the polling bit is bit 9

TInt CFaxClass1::RxPrePollL ()
{
	TInt pollDocsAvailable = 0;
	TInt i;
	iResults.Copy (_L8 ("CALL JUST ANSWERED"));
	for (;;)
		{
		if (GetframeL (iResults) == 0)
			return (KFaxErrFrameFail);
		iModem->ProgressUpdateL ();

		// the third byte in the frame is the FCF (fax control field)

		switch ((TUint8) iResults[2])
			{
			case 0x20:            // this marks a non-standard frame, which we ignore
				iModem->iOurMessage.Format (_L8 ("NSF nonstandard facilities Frame"));
				iModem->ProgressUpdateL ();
				break;

			case 0x40:            // this marks the receiver ID
				iModem->iOurMessage.Format (_L8 ("CSI identity Frame"));
				iModem->ProgressUpdateL ();
				iModem->iProgress.iAnswerback.Zero ();
				for (i = 22; i > 2; i--)
					iModem->iProgress.iAnswerback.Append (iResults[i]);
				iModem->iOurMessage.Format (_L8 ("Remote fax ID is %S"), &iModem->iProgress.iAnswerback);
				iModem->ProgressUpdateL ();
				break;              // the capability frame should follows

			case 0x80:            // this marks the receiver capability frame
				iModem->iOurMessage.Format (_L8 ("DIS capability Frame"));
				iModem->ProgressUpdateL ();
				iFcfXbit = 1;       // we've received a DIS, so set the X bit
				pollDocsAvailable = iResults[4] & 0x01;     // and record the polling bit too
				break;

			case 0xfa:            // this means we were asked to disconnect
				RxDCNL ();
				return (KFaxErrRemoteDCN);

			default:;
			}

		// if a final frame we return
		// else we just issue AT+FRH=3 and continue

		if (iResults[1] & 0x10)
			break;
		iModem->ExportL (_L8 ("AT+FRH=3\r"));
		}

	if (pollDocsAvailable)
		{
		iModem->iOurMessage.Format (_L8 ("Polling bit set"));
		iModem->ProgressUpdateL ();
		return (KErrNone);
		}
	return (KFaxNothingToPoll);  // if the other machine isn't pollable we exit
}
/********************************************************************/

// here we negotiate a fax reception - Phase B
 // this function is always called after we have connected. However,
 // it can be called at other times if we have requested a renegotiation
 // or if the sender want to change the fax parameters

TInt CFaxClass1::RxPrePageL ()
{
	TInt x, z, i, nullCounter;
	TUint8 thisChar, lastChar;
	TInt ticks;

	for (;;)
		{
		iModem->ExportL (_L8 ("AT+FRH=3\r"));
		if (GetframeL (iResults) == 0)
			return (KFaxErrFrameFail);
		iModem->iOurMessage.Format (_L8 ("Response received"));
		iModem->ProgressUpdateL ();

		// analyse the possible responses

		switch ((TUint8) iResults[2])
			{
			case 0x42:            // this is the sender ID - their capability should follow

				iModem->iOurMessage.Format (_L8 ("TSI identity Frame"));
				iModem->ProgressUpdateL ();
				iModem->iProgress.iAnswerback.Zero ();
				for (i = 22; i > 2; i--)
					iModem->iProgress.iAnswerback.Append (iResults[i]);
				iModem->iOurMessage.Format (_L8 ("Remote fax ID is %S"), &iModem->iProgress.iAnswerback);
				iModem->ProgressUpdateL ();
				break;

			case 0x82:            // here's the sender capability frame - the most complex case

				iModem->iOurMessage.Format (_L8 ("DCS capability Frame"));
				iModem->ProgressUpdateL ();
				for (i = 0; i < 5; i++)
					iDcsFrame.byte[i] = (TUint8) iResults[i + 3];

				// we have the DCS saved - we analyse it for speed and resolution and compression

				if (iDcsFrame.bit.b24 == 0)
					iDcsFrame.byte[3] = 0;

				iModem->iProgress.iResolution = TFaxResolution (iDcsFrame.bit.b15);
				iModem->iProgress.iCompression = TFaxCompression (iDcsFrame.bit.b16);

				switch (iDcsFrame.byte[1] & 0x3c)
					{
				case 0x08:
					iActualFaxSpeed = 48;
					break;        /* 4800  V.27 */
				case 0x04:
					iActualFaxSpeed = 96;
					break;        /* 9600  V.29 */
				case 0x0c:
					iActualFaxSpeed = 72;
					break;        /* 7200  V.29 */
				case 0x24:
					iActualFaxSpeed = 97;
					break;        /* 9600  V.17 */
				case 0x2c:
					iActualFaxSpeed = 73;
					break;        /* 7200  V.17 */
				case 0x20:
					iActualFaxSpeed = 145;
					break;        /* 14400 V.17 */
				case 0x28:
					iActualFaxSpeed = 121;
					break;        /* 12000 V.17 */
				default:
					iActualFaxSpeed = 24; /* 2400  V.27 */
					}

				i = (iActualFaxSpeed & (~1));

				// now we prepare to recieve the training frame that follows the DCS
				// we try to get the carrier at this speed three times before giving up

				for (x = 0; x < 3; x++)
					{
					iModem->iOurMessage.Format (_L8 ("setting %d00"), i);
					iModem->iProgress.iSpeed = (i * 100);
					iModem->ProgressUpdateL ();

					iResults.Copy (_L8 ("AT+FRM="));
					iResults.AppendNum (iActualFaxSpeed);
					iResults.Append (_L8 ("\r"));
					iModem->ExportL (iResults);
					z = FramestatL ();
					if (z == 1)
						break;
				if (z != 0)
					{
					iModem->TxcharL (Kcan);
					if (FramestatL () < 0)
						iModem->TxcharL (Kreturn);
					ReceiveSilenceL ();
					iModem->iOurMessage.Format (_L8 ("sending FTT"));
					iModem->ProgressUpdateL ();
					iFrame.Append (KT30_FTT);
					if (SendframeL (iFrame) == 0)
						return (KFaxErrTrainFail);
					break;
					}
					}
				if (x == 3)
					return (KFaxErrAtNegotiatedSpeed);

				// once we have a carrier, we start receiving the training frame
				// we look for a clear 750 milliseconds of zeros ending in <dle><etx>
				// this is determined by calculating the number of number of null bytes
				// taken at any given speed

				iModem->iOurMessage.Format (_L8 ("training .... "));
				iModem->ProgressUpdateL ();

				ticks = (CLK_TCK * 165) / 100;	// bug fix - was originally "CLK_TICK * (165/100)"
												// This failed because 165/100 is rounded to 1 because
												// ticks is an integer and that made the fax server
												// training for 1 second instead of 1.5
				for (lastChar = 0, nullCounter = 0;;)
					{
					if (iModem->RxcharWaitL (ticks) == 0)
						{
						break;
						}
					thisChar = iModem->iReadone[0];
					if (nullCounter != (i * 75 / 8))
						{
						if (thisChar != 0)
							nullCounter = 0;
						else
							++nullCounter;
						}
					if ((thisChar == Ketx) && (lastChar == Kdle))
						break;
				lastChar = thisChar;
					}
				if (FramestatL () < 0)
					{
					iModem->TxcharL (Kcan);
					if (FramestatL () < 0)
						iModem->TxcharL (Kreturn);
					}

				// now we check the count of null bytes and either send FTT
				// (in which case the sender will send a new DCS and try again)
				// or else send CFR confirmation and wait for the first page

				iFrame.Zero ();
				iFrame.Append (KT30_CTLLST);
				if (nullCounter == (i * 75 / 8))
					{
					iModem->iOurMessage.Format (_L8 ("training OK"));
					iModem->ProgressUpdateL ();
					}
				else
					{
					ReceiveSilenceL ();
					iModem->iOurMessage.Format (_L8 ("sending FTT"));
					iModem->ProgressUpdateL ();
					iFrame.Append (KT30_FTT);
					if (SendframeL (iFrame) == 0)
						return (KFaxErrTrainFail);
					break;
					}

				iModem->iOurMessage.Format (_L8 ("sending CFR"));
				iModem->ProgressUpdateL ();

				iFrame.Append (KT30_CFR);
				if (SendframeL (iFrame) == 0)
					return (KFaxErrCFR);

				// after we send a CFR, we interpret a failure to
				// establish a high-speed carrier as an indication
				// that the sender didn't get our CFR, and will
				// act as if they received an FTT

				if (RxSetHighSpeedL () != KErrNone)
					break;
			return (KErrNone);

			// lastly, we cater for the sender disconnecting us,
			// either because we couldn't train or because our
			// capabilities were wrong, or because they were only trying
			// to hack our fax machine

			case 0xfa:
				RxDCNL ();
				return (KFaxErrRemoteDCN);

			default:;
			}
		}
}
/********************************************************************/

// this is a small function to set a class 1 fax modem to phase C
 // reception speed (found in iActualFaxSpeed) in preparation for
 // receiving data.  This is called before the first page and also
 // between pages.  If the modem can't find a high speed carrier, we
 // leave the caller to decide what action to take - if we'd just sent a
 // page confirmation we should try resending our last negotiating frame
 // in case it was lost, but if we have just sent a CFR, we wait for the
 // sender to retrain.

// If the protocol is out of sync and we get a low speed carrier
 // then we'll get a +FCERROR response (same as ERROR)

TInt CFaxClass1::RxSetHighSpeedL ()
{
	TInt x, portSpeed;
	switch (iActualFaxSpeed)
		{
		case 145:
			x = 144;
			portSpeed = 146;
			break;
		case 121:
			x = 120;
			portSpeed = 122;
			break;
		case 97:
			x = 96;
			portSpeed = 98;
			break;
		case 73:
			x = 72;
			portSpeed = 74;
			break;
		default:
			x = portSpeed = iActualFaxSpeed;
		}

	iModem->iOurMessage.Format (_L8 ("setting %d00"), x);
	iModem->iProgress.iSpeed = (x * 100);
	iModem->ProgressUpdateL ();

	iResults.Copy (_L8 ("AT+FRM="));
	iResults.AppendNum (portSpeed);
	iResults.Append (_L8 ("\r"));
	iModem->ExportL (iResults);

	x = FramestatL (KT30_T2);     // always wait 6 seconds before a timeout
	if (x == 1)
		{
		return (RxStartPageL ());
		}
	if (x != 0)
		{
		iModem->TxcharL (Kcan);
		if (FramestatL () < 0)
			iModem->TxcharL (Kreturn);
		}
	return (KFaxErrAtNegotiatedSpeed);
}
/********************************************************************/

// after page data has been received, we go back to 300 bps negotiation
// for the post-page message which the transmitter sends 75ms after the
// end of the data

TInt CFaxClass1::RxPostPageL ()
{
	TInt i, x = 0;
	iOldFrame.Zero ();
	iModem->iProgress.iPhase = EPostPageStatus;
	iModem->ProgressUpdateL ();

	// wait for the modem to react to the end of fax data before proceeding

	if ((iModem->GetMatchL (_L8 ("NO CARRIER"), 5)) == 0)
		return (KFaxErrCannotEndData);

	for (;;)
		{

		// we start by requesting a frame

		iModem->ExportL (_L8 ("AT+FRH=3\r"));
		if (GetframeL (iResults) == 0)
			{
			if (x++ == 3)
				return (KFaxErrFrameFail);
			continue;
			}

		iModem->iOurMessage.Format (_L8 ("Response received"));
		iModem->ProgressUpdateL ();

		// now we work out what it is

		switch ((TUint8) iResults[2])
			{

			// the first case is where the last page was the end of the fax

			case 0x3e:            // we recognize PRI-Q frames but treat them like non-PRI-Q variants
				iModem->iOurMessage.Format (_L8 ("PRI-Q bit set"));
				iModem->ProgressUpdateL ();
				// fallthrough
			case 0x2e:
				iModem->iOurMessage.Format (_L8 ("EOP end of page %u and transmission"), iModem->iProgress.iPage);
				iModem->ProgressUpdateL ();
				iModem->iOurMessage.Format (_L8 ("sending MCF"));
				iModem->ProgressUpdateL ();
				iFrame.Zero ();
				iFrame.Append (KT30_CTLLST);
				iFrame.Append (KT30_MCF);
				if (SendframeL (iFrame) == 0)
					return (KFaxErrMCF);

				// now it isn't an error if get a DCN and hang up
				// so, loop and wait for it

				iModem->iProgress.iPhase = EDisconnection;
				continue;

				// the second case is where the sender demands a renegotiation

			case 0x9e:            // we recognize PRI-Q frames but treat them like non-PRI-Q variants
				iModem->iOurMessage.Format (_L8 ("PRI-Q bit set"));
				iModem->ProgressUpdateL ();
				// fallthrough
			case 0x8e:
				iModem->iOurMessage.Format (_L8 ("EOM end of page %u and document"), iModem->iProgress.iPage);
				iModem->ProgressUpdateL ();
				iModem->iOurMessage.Format (_L8 ("sending MCF"));
				iModem->ProgressUpdateL ();
				iFrame.Zero ();
				iFrame.Append (KT30_CTLLST);
				iFrame.Append (KT30_MCF);
				if (SendframeL (iFrame) == 0)
					return (KFaxErrMCF);
				iModem->iProgress.iPhase = ESessionNegotiation;
				iOldFrame.Zero ();
				iOldFrame.Append (KT30_CTLLST);
				iOldFrame.Append (KT30_DIS);
				for (i = 0; i < iDisBytes; i++)
					iOldFrame.Append (iDisFrame.byte[i]);
				return (RxPrePageL ());

				// the third case is where another page is going to follow

			case 0x5e:            // we recognize PRI-Q frames but treat them like non-PRI-Q variants

				iModem->iOurMessage.Format (_L8 ("PRI-Q bit set"));
				iModem->ProgressUpdateL ();
				// fallthrough
			case 0x4e:
				iModem->iOurMessage.Format (_L8 ("MPS end of page %u"), iModem->iProgress.iPage);
				iModem->ProgressUpdateL ();

				iModem->iOurMessage.Format (_L8 ("sending MCF"));
				iModem->ProgressUpdateL ();

				iFrame.Zero ();
				iFrame.Append (KT30_CTLLST);
				iFrame.Append (KT30_MCF);
				if (SendframeL (iFrame) == 0)
					return (KFaxErrMCF);

				for (x = 0; x < 3; x++)
					{
				if (RxSetHighSpeedL () == KErrNone)
					return (KErrNone);
				iModem->iOurMessage.Format (_L8 ("Resending last response .... "));
				iModem->ProgressUpdateL ();
				if (SendframeL (iOldFrame) == 0)
					return (KFaxErrMCF);
					}
				return (KFaxErrMCF);

				// the fourth case is where we are told to disconnect
				// it's an error if we hadn't been expecting it

			case 0xfa:
				if (iModem->iProgress.iPhase == EDisconnection)
					{
					RxDCNL ();
					return (KErrNone);
					}

				RxDCNL ();
				return (KFaxErrRemoteDCN);

				// the fifth case is where we see a negotiation frame
				// the sixth case is where we see a negotiation frame
				// our supposed page may have been a mistake
				// just go back to phase B and try to recover that way

			case 0x42:            // TSI frame
			case 0x82:            // DCS frame
				if ((TUint8) iResults[2] == 0x42)
					iModem->iOurMessage.Format (_L8 ("TSI identity"));
				else
					iModem->iOurMessage.Format (_L8 ("DCS capability"));
				iModem->iOurMessage.Append (_L8 (" Frame - renegotiating session parameters"));
				iModem->ProgressUpdateL ();
				iModem->iProgress.iPhase = ESessionNegotiation;
				return (RxPrePageL ());

				// the last case is where we see an unsupported frame
				// if it is a final frame we ask for a repeat via CRP

			default:
				if (SendCRPL () != KErrNone)
					return (KFaxErrCRP);
			}
		}
}
/********************************************************************/

// here we have detected a disconnection frame so we hang up the modem

void CFaxClass1::RxDCNL ()
{
	iModem->iOurMessage.Format (_L8 ("DCN disconnect Frame"));
	iModem->iProgress.iPhase = EDisconnection;
	iModem->ProgressUpdateL ();
}
/********************************************************************/

// here we send a DCN disconnect frame and then hang up the modem

TInt CFaxClass1::TxDCNL ()
{
	iModem->iOurMessage.Format (_L8 ("sending DCN"));
	iModem->iProgress.iPhase = EDisconnection;
	iModem->ProgressUpdateL ();
	ReceiveSilenceL ();
	iFrame.Zero ();
	iFrame.Append (KT30_CTLLST);
	iFrame.Append (KT30_DCN);
	if (SendframeL (iFrame) == 0)
		return (KFaxErrDCN);
	return (KErrNone);
}
/********************************************************************/

// here we set up a fax transmit - Phase A
 // there's no HDLC stuff here

TInt CFaxClass1::TxConnectL ()
{

	// we query the modem to find out what its speed capabilities are
    iModem->ExportL (_L8 ("AT+FTM=?\r"));
    // coverity[check_return]
    iModem->ImportL (iResults, 2);
    iModem->iOurMessage.Format (_L8 ("%S"), &iResults);
    iModem->ProgressUpdateL ();
    iModem->GetMatchL (_L8 ("OK"), 1);
    
	// the available speeds are stored in iResults
	// we set our proposed speed to the highest compatible with faxini settings

	if ((iResults.FindF (_L8 ("24"))) >= 0)
		iActualFaxSpeed = 24;
	else
		return (KFaxErrModemNotWorking);
	if ((iFaxServerSessionSettings->iMaxSpeed) > 2400)
		if ((iResults.FindF (_L8 ("48"))) >= 0)
			iActualFaxSpeed = 48;
	if ((iFaxServerSessionSettings->iMaxSpeed) > 4800)
		if ((iResults.FindF (_L8 ("96"))) >= 0)
			iActualFaxSpeed = 96;
	if ((iFaxServerSessionSettings->iMaxSpeed) > 9600)
		if ((iResults.FindF (_L8 ("145"))) >= 0)
			iActualFaxSpeed = 145;

	// we now issue our ATD command, and if we aren't in immediate
	// transmit mode (already off hook) then we dial a number

	iModem->ExportL (_L8 ("ATD"));
	if ((iFaxServerSessionSettings->iMode & KFaxOffHook) == 0)
		iModem->ExportL (iFaxServerSessionSettings->iPhoneNumber);
	iModem->TxcharL (Kreturn);
	iModem->iOurMessage.Format (_L8 ("Call has been dialled"));
	iModem->iProgress.iPhase = ECallEstablishment;
	iModem->ProgressUpdateL ();

	// now we wait up to KDialTimeout seconds for the modem to connect

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
		if ((iResults.FindF (_L8 ("CONNECT"))) >= 0)
			break;
		}

	// we can now go on to phase B

	iResults.Copy (_L8 ("CALL JUST ANSWERED"));
	return (TxPrePageL ());
}
/********************************************************************/

// here we negotiate a fax transmission, or a polled reception - Phase B
// this function is always called after we have connected. However,
// it can be called at other times if the receiver has requested a
// renegotiation or if we want to change the fax parameters, in which
// case we would enter with iModem->iProgress.iPhase == RFax::EPostPageStatus

TInt CFaxClass1::TxPrePageL ()
{
	TInt i;
	//TInt x;
	TInt successiveErrors = 0;
	TInt trainingAttempts = 0;
	iDcsBytes = 3;
	iOldFrame.Zero ();
	TInt ticks;
	TInt trainbytes;

	// this routine is one big frame waiting loop - note that on first entry
	// here we have set the length of the last frame stored in iOldFrame to
	// zero, so we don't resend any last frame on first entry here.
	// Subsequent iterations of our receive loop will resend the iOldFrame
	// if nothing is received, in an attempt at error recovery, unless of
	// course we have deliberately reset iOldFrame to zero again

	//x = 0;
	for (;;)
		{
		if (iModem->iProgress.iPhase != EPostPageStatus)
			{
			if (GetframeL (iResults) == 0)
				{
				if (successiveErrors++ > 3)
					return (KFaxErrFrameFail);

				 if (iModem->iProgress.iPhase == ECallEstablishment)
				 // no point in carrying on without any capability frame
				{
				iModem->ExportL (_L8 ("AT+FRH=3\r"));
				continue;
				}
		//		if (iModem->iProgress.iPhase == ECallEstablishment)
		//			{
		//			if (x++ == 3)
		//				return (KFaxErrFrameFail);
		//			
		//			iModem->ExportL (_L8 ("AT+FRH=3\r"));
		//			continue;
		//			}

				// no point in carrying on without any capability frame
				// else

				iResults[1] = 0x10;
				iResults[2] = 0xff;

				// this is a nonexistent response
				// the effect of this is to force a retransmission of the TSI and DCS
				// with (hopefully) a subsequent retrain in an attempt to resync
				}
			else 
			    successiveErrors = 0;


			
			if (iResults[2] != 0xff)
			{
			
			iModem->iOurMessage.Format (_L8 ("Response received"));
			iModem->ProgressUpdateL ();

			// the third byte in the frame is the FCF (fax control field)

			switch ((TUint8) iResults[2])
				{
				case 0xff:         // this is our dummy octet to force a restart
					break;

				case 0x40:         // this marks the receiver ID
					iModem->iOurMessage.Format (_L8 ("CSI identity Frame"));
					iModem->ProgressUpdateL ();
					iModem->iProgress.iAnswerback.Zero ();
					for (i = 22; i > 2; i--)
						iModem->iProgress.iAnswerback.Append (iResults[i]);
					iModem->iOurMessage.Format (_L8 ("Remote fax ID is %S"), &iModem->iProgress.iAnswerback);
					iModem->ProgressUpdateL ();
					break;           // the capability frame should follows

				case 0x80:         // this marks the receiver capability frame
					iFcfXbit = 1;    // we've received a DIS, so set the X bit
					iModem->iOurMessage.Format (_L8 ("DIS capability Frame"));
					iModem->ProgressUpdateL ();
					AnalyseDISL ();   // analyse the DIS and compose a DCS
					if (iDisFrame.bit.b10 != 1)
						return (KFaxErrRemoteCannotReceive);  // if the other machine can't receive we exit
					break;

				case 0x84:         // this marks a good train and is the normal exit from this loop
					iModem->iOurMessage.Format (_L8 ("CFR confirmation Frame"));
					iModem->ProgressUpdateL ();
					return (TxSetHighSpeedL ());

				case 0x44:         // this marks a failed train so we drop the speed
					iModem->iOurMessage.Format (_L8 ("FTT failure to train Frame"));
					iModem->ProgressUpdateL ();
					if (++trainingAttempts & 1)      // train down on failures 2 4 6 8
						break;
				if (iActualFaxSpeed == 73)
					iActualFaxSpeed = 96;
				else
					iActualFaxSpeed -= 24;
				if (iActualFaxSpeed < (iFaxServerSessionSettings->iMinSpeed / 100))
					{
				TxDCNL ();
				return (KFaxBelowMinSpeed);
					}
				break;

				case 0xfa:         // this means we were asked to disconnect
					RxDCNL ();
					return (KFaxErrRemoteDCN);

				case 0x20:         // this marks a non-standard frame, which we ignore
					iModem->iOurMessage.Format (_L8 ("NSF nonstandard facilities Frame"));
					iModem->ProgressUpdateL ();
					break;

				// the last case is where we see an unsupported frame
				// if it is a final frame we ask for a repeat via CRP

				default:
					if (SendCRPL () != KErrNone)
						return (KFaxErrCRP);
				}

			// if not a final frame we just issue AT+FRH=3 and continue

			if (!(iResults[1] & 0x10))
				{
				iModem->ExportL (_L8 ("AT+FRH=3\r"));
				continue;
				}
			// otherwise we send our proposals, starting with our own ID

			iModem->iOurMessage.Format (_L8 ("Final frame received"));
			iModem->ProgressUpdateL ();
			}
		}// if (iResults[2] !=0) statement
		if (iModem->iProgress.iPhase == ECallEstablishment)
			{
			iModem->iOurMessage.Format (_L8 ("sending TSI"));
			iModem->ProgressUpdateL ();
			iFrame.Zero ();
			iFrame.Append (KT30_CTLNXT);
			iFrame.Append (KT30_TSI);
			for (i = 20; i > iFaxServerSessionSettings->iFaxId.Length (); i--)
				iFrame.Append (0x20);
			for (i = iFaxServerSessionSettings->iFaxId.Length (); i;)
			iFrame.Append (iFaxServerSessionSettings->iFaxId[--i]);
			if (SendframeL (iFrame) != 1)
				return (KFaxErrHDLC);
			}
		else
			{
			iModem->ExportL (_L8 ("AT+FTH=3\r"));
			if (FramestatL () != 1)
				return (KFaxErrHDLC);
			}

		iModem->iProgress.iPhase = ESessionNegotiation;

		// before sending our DCS frame we ensure the speeds bits match what we want

		iDcsFrame.byte[1] &= 0xc3;
		switch (iActualFaxSpeed)
			{
			case 48:
				iDcsFrame.byte[1] |= 0x08;
				break;						/* 4800 */
			case 96:
				iDcsFrame.byte[1] |= 0x04;
				break;						/* 9600 V.29 */
			case 97:
				iDcsFrame.byte[1] |= 0x24;
				break;						/* 9600 V.17 */
			case 72:
				iDcsFrame.byte[1] |= 0x0c;
				break;						/* 7200 V.29 */
			case 73:
				iDcsFrame.byte[1] |= 0x2c;
				break;						/* 7200 V.17 */
			case 145:
				iDcsFrame.byte[1] |= 0x20;
				break;						/* 14400 */
			case 121:
				iDcsFrame.byte[1] |= 0x28;
				break;						/* 12000 */
			default:
				iDcsFrame.byte[1] |= 0x00;  /* 2400 */
			}
		iModem->iOurMessage.Format (_L8 ("sending DCS "));
		iModem->ProgressUpdateL ();
		iFrame.Zero ();
		iFrame.Append (KT30_CTLLST);
		iFrame.Append (KT30_DCS);
		//x = 3;
		for (i = 0; i < iDcsBytes; i++)
			iFrame.Append (iDcsFrame.byte[i]);
		if (SendframeL (iFrame) != 1)
			return (KFaxErrHDLC);

		// after sending our DCS frame we wait fot 75 ms before training
		//
		// Note on the 75 millisecond delays
		// =================================
		// At this point we need to introduce a 75 ms delay (+-20%).
		// this is usually done with the AT+FTS=8 command and the code
		// would normally run as follows :
		//
		// iModem->ExportL (_L8 ("AT+FTS=8\r"));
		// if ((iModem->GetMatchL (_L8 ("OK"), 3)) == 0)
		// return (KFaxErrStopAndWait);
		//
		// or, alternatively, we could use our own routines to delay
		// for this amount of time using
		//
		// iModem->Silence (75000);
		//
		// However, the innards of the comms driver in EPOC32 introduces
		// 2-tick delays on timed reads and writes - in other words, there
		// was a 2-tick delay before the OK from our last frame was received
		// and there will also be a 2-tick delay before the next command
		// reaches the modem. Note that a 2-tick delay could be from 15
		// to 30 ms - we want a delay from 60ms to 90 ms (75 ms +- 20%) -
		// which must be between 4 and 6 ticks.  All the delays we use
		// here are empirically arrived at via Faxlab testing rather
		// than being worked out in advance.

		// NOTE : these delays are really applicable ONLY to Protea/ARM7100
		// the machine dependency is unavoidable under the
		// circumstances. Protea ticks @ 64 Hz, which gives
		// us 15.625 ms per tick.  WINS ticks @ 10 Hz,
		// which gives us 100 microseconds per tick - clearly a
		// significant difference - the delta timers in the comms
		// kernel are therefore also very different

		iModem->iOurMessage.Format (_L8 ("delaying for 75 ms"));
		iModem->ProgressUpdateL ();
		
	//	iModem->Silence ((iModem->iGranularity - 125) * 4);       // 4 ticks pre-TCF - below iModem->iGranularity x 1
		
		TInt delay=iModem->iCalls*37;		//iCalls for 2ms *37 to get an approximatelly 75ms delay
		TInt k=0;

		for (k=0;k<delay;k++)		// this loop will  generate the 75ms delay
		iModem->clock();
		
		i = (iActualFaxSpeed & 0xfe);
		iModem->iOurMessage.Format (_L8 ("setting %d00"), i);
		iModem->iProgress.iSpeed = (i * 100);
		iModem->ProgressUpdateL ();
		iResults.Copy (_L8 ("AT+FTM="));
		iResults.AppendNum (iActualFaxSpeed);
		iResults.Append (_L8 ("\r"));
		iModem->ExportL (iResults);

		// say how many bytes in the 1.5 second TCF
		// prepare a null filled transmit buffer length and get length to maxnulls
		// calculate minimum scan line times
		// wait for the modem CONNECT

		trainbytes = (i * 150 / 8);
		iModem->iOurMessage.Format (_L8 ("training sequence for %d bytes"), trainbytes);

		iModem->iTransmitBuffer.SetMax ();
		iModem->iTransmitBuffer.FillZ ();
		TInt maxnulls = iModem->iTransmitBuffer.Length ();
		iModem->ProgressUpdateL ();
		if (iMinscan == 0)
			iMinlinelength = 0;
		else
			iMinlinelength = ((iModem->iProgress.iSpeed / (1000 / iMinscan)) / 8) + 1;
		if ((iModem->GetMatchL (_L8 ("CONNECT"), 5)) == 0)
			return (KFaxErrAtNegotiatedSpeed);

		// now we send our TCF with flow control

		iModem->Xonon ();
		
		iModem->iOurMessage.Format (_L8 ("Entering training loop"));
		iModem->ProgressUpdateL ();
		
		while (trainbytes != 0)
			{
			if (maxnulls > trainbytes)
				{
				iModem->iTransmitBuffer.SetLength (trainbytes);
				trainbytes = 0;
				}
			else
				{
				iModem->iTransmitBuffer.SetMax ();
				trainbytes -= maxnulls;
				}
			iModem->CommitTransmitBufferL ();
			}

		iModem->iTransmitBuffer.Append (Kdle);
		iModem->iTransmitBuffer.Append (Ketx);
		iModem->CommitTransmitBufferL ();


		iModem->iOurMessage.Format (_L8 ("Train complete"));
		iModem->ProgressUpdateL ();

		// now wait for the modem to return to command mode

		while (iModem->Rxstat () != 0)
			{
			ticks = CLK_TCK;
			iModem->RxcharWaitL (ticks);
			}
		if ((iModem->GetMatchL (_L8 ("OK"), 5)) == 0)
			return (KFaxErrTrainStop);

		iModem->Xonoff ();
		// there's no frame to resend, so we prepare a CRP in case of
		// any errors before looping for the response

		iOldFrame.Zero ();
		iModem->ExportL (_L8 ("AT+FRH=3\r"));
		}
}
/********************************************************************/

// this is a small function to set a class 1 fax modem to phase C
 // transmission speed (found in iActualFaxSpeed) in preparation for
 // sending data.  This function is called before the first page
 // and also between pages. Class 1 modems require that we delay for
 // 75 ms before going to phase C transmission - see the note
 // earlier on method of achieving a 75 ms delay here

TInt CFaxClass1::TxSetHighSpeedL ()
{
	TInt x, portSpeed;
	iFrame.SetMax ();
	iModem->iOurMessage.Format (_L8 ("delaying for 75 ms"));
	iModem->ProgressUpdateL ();
//	iModem->Silence ((iModem->iGranularity - 125) * 4);  // 4 ticks pre-T4 - below iModem->iGranularity x 3

	TInt delay=iModem->iCalls*37;		//iCalls for 2ms *37 to get an approximatelly 75ms delay
	TInt k=0;

	for (k=0;k<delay;k++)		// this loop will  generate the 75ms delay
		iModem->clock();

	switch (iActualFaxSpeed)
		{
		case 145:
			x = 144;
			portSpeed = 146;
			break;
		case 121:
			x = 120;
			portSpeed = 122;
			break;
		case 97:
			x = 96;
			portSpeed = 98;
			break;
		case 73:
			x = 72;
			portSpeed = 74;
			break;
		default:
			x = portSpeed = iActualFaxSpeed;
		}
	iModem->iOurMessage.Format (_L8 ("setting %d00"), x);
	iModem->iProgress.iSpeed = (x * 100);
	iModem->ProgressUpdateL ();
	iResults.Copy (_L8 ("AT+FTM="));
	iResults.AppendNum (portSpeed);
	iResults.Append (_L8 ("\r"));
	iModem->ExportL (iResults);
	if (FramestatL () != 1)
		return (KFaxErrAtNegotiatedSpeed);
	return (TxStartPageL ());
}
/********************************************************************/

// here's where we wait after sending a page and the postpage
// message to see what the receiver thought - there are five responses
// MCF RTP PIP = good page         RTN PIN = bad page
//
// TxPostPage should return either with
//
// a) an error code and iPhase set to RFax::EPostPageStatus, in which case the send returns with the error
// b) KErrNone and iPhase set to RFax::EDataTransfer, in which case we send the next page
// c) KErrNone and iPhase set to RFax::EDisconnection, in which case the send returns with KErrNone
//

TInt CFaxClass1::TxPostPageL ()
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
	iModem->iTransmitBuffer.Append (Ketx);
	iModem->CommitTransmitBufferL ();

	iModem->iOurMessage.Format (_L8 ("<dle><etx> transmitted after %d lines"), iModem->iProgress.iLines);
	iModem->iProgress.iPhase = EPostPageStatus;
	iModem->ProgressUpdateL ();
	while (iModem->Txstat () != 0)
		;

	// we've just ended phase C data, so we need to wait for the modem to respond with OK

	if (iModem->GetMatchL (_L8 ("OK"), (32 * 1024) / (iModem->iProgress.iSpeed / 10)) == 0)
		return (KFaxErrCannotEndData);

	iModem->Xonoff ();

	iModem->iOurMessage.Format (_L8 ("delaying for 75 ms"));
	iModem->ProgressUpdateL ();


/*************************************          NOTE       *******************************************************	
// see the note earlier on reason for the lack of an explicit 75 ms delay here


//	iModem->Silence ((iModem->iGranularity - 125) * 4);   4 ticks post-T4 - below iModem->iGranularity x 1
//  The above line of code was removed because the Silence function calls User::After which is fairly inaccurate
//  Faxlab revealed that we were actually waiting for 432 ms !!! instead of the recomended 75ms +- 20%
//  The delay is now generated using the timing callibration loop
//  This is more CPU intensive than the User::After call because essentially is a aoftware delay loop but 
//  will enhance the reliability of fax class 1 especially over GSM
*****************************************************************************************************************/
	TInt delay=iModem->iCalls*30;		//iCalls for 2ms *37 to get an approximatelly 75ms delay
	TInt k=0;

	for (k=0;k<delay;k++)		// this loop will  generate the 75ms delay
		iModem->clock();

	iOldFrame.Zero ();
	iFrame.Zero ();
	iFrame.Append (KT30_CTLLST);


	if ((iFaxServerSessionSettings->iTxPages) == iModem->iProgress.iPage)
		{
		iModem->iOurMessage.Format (_L8 ("sending EOP"));
		iModem->ProgressUpdateL ();

		iFrame.Append ((KT30_EOP));
		if (SendframeL (iFrame) == 0)
			return (KFaxErrEOP);

		iModem->iOurMessage.Format (_L8 ("End of document transmitted"));
		iModem->ProgressUpdateL ();
		}
	else
		{
		iModem->iOurMessage.Format (_L8 ("sending MPS"));
		iModem->ProgressUpdateL ();

		iFrame.Append ((KT30_MPS));
		if (SendframeL (iFrame) == 0)
			return (KFaxErrMPS);

		iModem->iOurMessage.Format (_L8 ("End of page %u transmitted"), iModem->iProgress.iPage);
		iModem->ProgressUpdateL ();
		}

	// now we await the post-page response from the receiver
	// we loop here because we need a final frame

	for (;;)
		{
		iModem->ExportL (_L8 ("AT+FRH=3\r"));
		if (GetframeL (iResults) == 0)
			return (KFaxErrFrameFail);
		iModem->iOurMessage.Format (_L8 ("Response received"));
		iModem->ProgressUpdateL ();

		// the third byte in the frame is the FCF (fax control field)
		// for those we recognize straight off, we say so
		// for any others, we either loop immediately or after a CRP request

		switch ((TUint8) iResults[2])
			{
			case 0x8c:
				iModem->iOurMessage.Format (_L8 ("MCF"));
				break;
			case 0xcc:
				iModem->iOurMessage.Format (_L8 ("RTP"));
				break;
			case 0xac:
				iModem->iOurMessage.Format (_L8 ("PIP"));
				break;
			case 0x4c:
				iModem->iOurMessage.Format (_L8 ("RTN"));
				break;
			case 0x2c:
				iModem->iOurMessage.Format (_L8 ("PIN"));
				break;

			// the last case is where we see an unsupported frame
			// if it is a final frame we ask for a repeat via CRP

			default:
				if (SendCRPL () != KErrNone)
					return (KFaxErrCRP);
				continue;
			}

		// now back to look at the FCF some more
		switch ((TUint8) iResults[2])
			{
			case 0x8c:
			case 0xcc:            // for good pages we say confirmed
			case 0xac:
				iModem->iOurMessage.Append (_L8 (" message confirmation"));
				iModem->ProgressUpdateL ();
				break;

			case 0x4c:            // for bad pages we say no good
			case 0x2c:            // if we haven't done so, resent last page
				iModem->iOurMessage.Append (_L8 (" : page not confirmed"));
				iModem->ProgressUpdateL ();
				if (iRepeatPage == 0)
					{
				iRepeatPage++;
				iModem->iProgress.iPage--;
				return (TxPrePageL ());
					}

			default:;
			}

		// we aren't going to resend the last page now

		iRepeatPage = 0;

		// if we've reached the end, we just quit

		if ((iFaxServerSessionSettings->iTxPages) == iModem->iProgress.iPage)
			{
			return (TxDCNL ());
			}

		// if we've received an MCF we carry on with phase C

		if (iResults[2] == KT30_MCF)      // carry on with phase C only if MCF

			{
			return (TxSetHighSpeedL ());
			}

		// we renegotiate if PIP or RTP, or PIN or RTN with no resend

		iModem->iOurMessage.Format (_L8 ("Renegotiating session parameters"));
		iModem->ProgressUpdateL ();
		return (TxPrePageL ());
		}
}
/********************************************************************/

// the analysis of the DIS frame and composition of the DCS frame
 // has been moved here for readability

inline void CFaxClass1::AnalyseDISL ()
{
	TInt i;

	// we copy iResults to our iDisFrame and compose our reply in iDcsFrame

	for (i = 0; i < 5; i++)
		iDisFrame.byte[i] = (TUint8) iResults[i + 3];

	for (i = 0; i < 5; i++)
		iDcsFrame.byte[i] = 0;

	// we always set T.4

	iDcsFrame.bit.b10 = 1;

	// we check the speed capability next and reset our iActualFaxSpeed

	switch (iDisFrame.byte[1] & 0x3c)
		{
		case 0x08:
			i = 48;
			break;                 /* V.27 ter 4800 2400 */
		case 0x0c:
			i = 96;
			break;                 /* V.29 9600 7200 + V.27 */
		case 0x2c:
			i = 145;
			break;                 /* V.17 14400 + V.29 + V.27 */
		default:
			i = 24;                /* V.27 fallback 2400 only */
		}
	if (i < (iActualFaxSpeed))
		iActualFaxSpeed = i;

	// we set our resolution to that of the fax we want to send
	// but if the receiver can only understand normal resolution
	// then we send all our faxes as normal and resign ourselves
	// to stretching them to double length

	iDcsFrame.bit.b15 = iFaxServerSessionSettings->iTxResolution;
	if (iDisFrame.bit.b15 == 0)
		iDcsFrame.bit.b15 = 0;
	iModem->iProgress.iResolution = TFaxResolution (iDcsFrame.bit.b15);

	// we set our compression to that of the fax we want to send
	// unless the receiver can only understand 1D compression - in
	// which case the sender should be able to compensate from the 
	// progress settings

	iDcsFrame.bit.b16 = iFaxServerSessionSettings->iTxCompression;
	if (iDisFrame.bit.b16 == 0)
		{
		iDcsFrame.bit.b16 = 0;
		}
	
	if ((iDisFrame.bit.b16==1) && ((iFaxServerSessionSettings->iTxCompression==EModifiedRead)))
		{
		iDcsFrame.bit.b16 = 1;
		}

	iModem->iProgress.iCompression = TFaxCompression (iDcsFrame.bit.b16);

	if (iModem->iProgress.iCompression==0)
		{
		iModem->iOurMessage.Format (_L8("DCS frame set to 1D"));
		iModem->ProgressUpdateL();
		}
	else 
		{
		iModem->iOurMessage.Format (_L8("DCS frame set to 2D"));
		iModem->ProgressUpdateL();
		}


	// we set the minumum scan line time to that of the receiver

	iDcsFrame.byte[2] &= 0x8f;
	switch (iDisFrame.byte[2] & 0x70)
		{
		case 0x70:
                                        			iMinscan = 0;
			iDcsFrame.byte[2] |= 0x70;
			break;                 /* b21=1 b22=1 b23=1 */
		case 0x50:
			if (iDcsFrame.bit.b15 == 0)
				{
			iMinscan = 40;
			iDcsFrame.byte[2] |= 0x40;
				}
			else
				{
				iMinscan = 20;
				}
			break;                 /* b21=1 b22=0 b23=1 - for fine res, /by 2 */
		case 0x30:
			if (iDcsFrame.bit.b15 == 0)
				{
				iMinscan = 20;
				}
			else
				{
				iMinscan = 10;
				iDcsFrame.byte[2] |= 0x20;
				}
			break;                 /* b21=1 b22=1 b23=0 - for fine res, /by 2 */
		case 0x60:
			if (iDcsFrame.bit.b15 == 0)
				{
				iMinscan = 10;
				iDcsFrame.byte[2] |= 0x20;
				}
			else
				{
				iMinscan = 5;
				iDcsFrame.byte[2] |= 0x10;
				}
			break;                 /* b21=0 b22=1 b23=1 - for fine res, /by 2 */
		case 0x10:
			iMinscan = 5;
			iDcsFrame.byte[2] |= 0x10;
			break;                 /* b21=1 b22=0 b23=0 */
		case 0x20:
			iMinscan = 10;
			iDcsFrame.byte[2] |= 0x20;
			break;                 /* b21=0 b22=1 b23=0 */
		case 0x40:
			iMinscan = 40;
			iDcsFrame.byte[2] |= 0x40;
			break;                 /* b21=0 b22=0 b23=1 */
		default:
			iMinscan = 20;         /* b21=0 b22=0 b23=0 */
		}

	// lastly, we always match our page length to the receiver page length

	iDcsFrame.bit.b19 = iDisFrame.bit.b19;
	iDcsFrame.bit.b20 = iDisFrame.bit.b20;
}
/********************************************************************/

// this is where we wait for modem responses

 // this function can be called either with a timeout in seconds
 // or with nothing, in which case we use a default as follows :

 // when we wait for the frame data we use a 6 second timeout
 // as specified in T.30 as timer T2 if we are waiting for
 // a command or a 3 second timer as specified in timer T4
 // if we are waiting for a response, with the presence of
 // a resendable frame in iOldFrame being the test

 // notice that importL() takes a timeout specified in seconds

 // normally OK and CONNECT are good with ERROR or NO CARRIER being bad
 // we save the actual result for inspection as ERROR codes after
 // frame reception with AT+FRH needs special handling via GETFRAMESTAT

TInt CFaxClass1::FramestatL ()
{
	TInt ticks;
	if (iOldFrame.Length () == 0)
		ticks = KT30_T2;
	else
		ticks = KT30_T4;
	return (FramestatL (ticks));
}
/********************************************************************/

TInt CFaxClass1::FramestatL (TInt aTicks)
{
	for (;;)
		{
		iModemString.SetMax ();
		if (iModem->ImportL (iModemString, aTicks) == 0)
			return (-1);;

		iModem->iOurMessage.Format (_L8 ("%S"), &iModemString);
		iModem->ProgressUpdateL ();

		if ((iModemString.FindF (_L8 ("OK"))) >= 0)
			return (1);
		if ((iModemString.FindF (_L8 ("CONNECT"))) >= 0)
			return (1);
		if ((iModemString.FindF (_L8 ("ERROR"))) >= 0)
			return (0);
		if ((iModemString.FindF (_L8 ("NO CARRIER"))) >= 0)
			return (0);
		}
}
/********************************************************************/

// if we have an ERROR result on receiving a frame it means that
 // the CRC was incorrect - the action is to wait until 200 ms of
 // silence have elapsed before resending the last frame - we use
 // the ReceiveSilenceL function for this

TInt CFaxClass1::GetFramestatL ()
{
	TInt code = FramestatL ();
	if ((iModemString.FindF (_L8 ("ERROR"))) >= 0)
		ReceiveSilenceL ();
	return (code);
}
/********************************************************************/

// this is the HDLC frame reception handler after AT+FRH commands

TInt CFaxClass1::GetframeL (TDes8 & aResult)
{
	TInt i, retries = 0, frameStatus = 1;
	TInt ticks;
	TUint8 thisChar, lastChar;
	TBuf8 < 4 > iHexchar;

	iModem->iOurMessage.Format (_L8 ("Waiting for Frame"));
	iModem->ProgressUpdateL ();

	// framestat is set to 1 for no error before entry to the loop

	for (;;)
		{

		// we only try three times before giving up

		retries++;
		if (retries > 3)
			return (0);

		// if we timed out, cancel the frame before proceeding

		if (frameStatus < 0)
			{
			iModem->TxcharL (Kcan);
			iModem->iOurMessage.Format (_L8 ("Frame timed out"));
			iModem->ProgressUpdateL ();
			GetFramestatL ();
			}

		// on any error, we resend the last frame if possible before
		// waiting for another go

		if (frameStatus != 1)
			{
			if (SendframeL (iOldFrame) == 0)
				{
				iModem->iOurMessage.Format (_L8 ("Cannot resend Frame"));
				iModem->ProgressUpdateL ();
				return (0);
				}
			iModem->iOurMessage.Format (_L8 ("Frame has been resent"));
			iModem->ProgressUpdateL ();
			iModem->ExportL (_L8 ("AT+FRH=3\r"));
			}

		// if we've just answered the phone, we don't wait for a result
		// otherwise we must have an OK or CONNECT before proceeding

		if ((aResult.Compare (_L8 ("CALL JUST ANSWERED"))) == 0)
			{
			frameStatus = 1;
			}
		else
			{
			frameStatus = GetFramestatL ();
			}

		aResult.FillZ ();
		aResult.SetMax ();

		// if we got NO CARRIER or ERROR then we have to try again

		if (frameStatus != 1)
			continue;

		// when we wait for the frame data we use a 6 second timeout
		// as specified in T.30 as timer T2 if we are waiting for
		// a command or a 3 second timer as specified in timer T4
		// if we are waiting for a response, with the presence of
		// a resendable frame in iOldFrame being the test

		// note that RxcharWaitL () takes a timeout in microseconds

		if (iOldFrame.Length () == 0)
			ticks = CLK_TCK * KT30_T2;
		else
			ticks = CLK_TCK * KT30_T4;

		// we expect data with dle shielding, ending with dle etx,
		// and with a hex dump for our session log

		// there is a 3 second maximum length to a frame but
		// the modem will detect bad HDLC frames for us and
		// flag with ERROR

		for (i = 0, lastChar = 0; i < 64;)
			{
			if ((iModem->RxcharWaitL (ticks)) == 0)
				{
				frameStatus = (-1);
				break;
				}
			thisChar = iModem->iReadone[0];
			if (lastChar == Kdle)
				{
				if (thisChar == Ketx)
					break;
				lastChar = 0;
				if (thisChar != Kdle)
					continue;
				}
			else if (thisChar == Kdle)
				{
				lastChar = Kdle;
				continue;
				}
			aResult[i++] = thisChar;

			iHexchar.Format (_L8 ("%x "), thisChar);
			if ((iModem->iOurMessage.Length () + iHexchar.Length ()) + 18 < iModem->iOurMessage.MaxLength ())
				iModem->iOurMessage.Append (iHexchar);
			}

		iModem->ProgressUpdateL ();

		// if we timed out during the wait, then go round again and handle it

		if (frameStatus != 1)
			continue;

		// otherwise we wait for the result code following the dle etx
		// and handle any errors from that

		frameStatus = GetFramestatL ();
		if (frameStatus != 1)
			continue;

		// the third byte in the frame is the FCF (fax control field)
		// we don't care whether we originated or answered the call
		// so we always knock off the T.30 X bit (LSB)

		aResult[2] &= 0xfe;

		// if we have been asked for a frame repeat we do that in here

		if (aResult[2] == 0x1a)
			{
			iModem->iOurMessage.Format (_L8 ("CRP command repeat Frame"));
			iModem->ProgressUpdateL ();
			ReceiveSilenceL ();
			frameStatus = 0;       // treat as a timeout and resend
			continue;
			}

		// otherwise we can now return with success

		return (1);
		}
}
/********************************************************************/

// this is the send HDLC frame handler following AT+FTH commands

TInt CFaxClass1::SendframeL (TDes8 & newframe)
{
	TUint8 i;
	TInt frameSize;
	TBuf8 < 4 > iHexchar;

	// we take a copy of the frame we've been passed in case we want
	// to resend for error recovery during GetFrame - it's the copy
	// we work with

	if (&newframe != &iOldFrame)
		iOldFrame.Copy (newframe);
	frameSize = iOldFrame.Length ();
	if (frameSize < 2)
		return (0);

	// all frames must be at least three characters
	// note that calling SendframeL with an empty frame disables
	// resends - iOldFrame.Zero() is rather quicker though

	// we don't need to tell the modem that we're going to send a frame
	// if we're sending DCS or CSI or DIS/DTC for the first time after a CONNECT response
	// (not a resend) - in all other cases we need to send AT+FTH

	iOldFrame[1] |= iFcfXbit;    // combine the FCF with the X bit
	i = iOldFrame[1];

	if (!((&newframe != &iOldFrame) && (i == (KT30_DCS|iFcfXbit)) || (i == KT30_CSI) || (i == (KT30_DIS|iFcfXbit))))
		{
		iModem->ExportL (_L8 ("AT+FTH=3\r"));
		if (FramestatL () != 1)
			return (0);
		}

	// we have a short delay before sending data here after any modem response

	iModem->ExportL (_L8 (""));

	// we now send the frame, starting with the fixed address followed
	// by the data we have been passed.  We use dle shielding and end
	// with dle etx and a hex dump before returning with the modem
	// response code

	iModem->TxcharL (KT30_ADDR);
	iModem->iOurMessage.Format (_L8 ("%x "), KT30_ADDR);
	for (i = 0; i < frameSize; i++)
		{
		iModem->TxcharL (iOldFrame[i]);
		iHexchar.Format (_L8 ("%x "), iOldFrame[i]);
		if ((iModem->iOurMessage.Length () + iHexchar.Length ()) + 18 < iModem->iOurMessage.MaxLength ())
			iModem->iOurMessage.Append (iHexchar);
		if (iOldFrame[i] == Kdle)
			iModem->TxcharL (Kdle);
		}
	iModem->TxcharL (Kdle);
	iModem->TxcharL (Ketx);
	iModem->ProgressUpdateL ();
	return (FramestatL ());
}
/********************************************************************/

// this is short routine to request a resend of a frame

TInt CFaxClass1::SendCRPL ()
{
	iModem->iOurMessage.Format (_L8 ("Inappropriate frame %x"), (TUint8) iResults[2]);
	iModem->ProgressUpdateL ();
	if (iResults[1] & 0x10)
		{
		iModem->iOurMessage.Format (_L8 ("sending CRP"));
		iModem->ProgressUpdateL ();
		ReceiveSilenceL ();
		iFrame.Zero ();
		iFrame.Append (KT30_CTLLST);
		iFrame.Append (KT30_CRP);
		if (SendframeL (iFrame) == 0)
			return (KFaxErrCRP);
		iResults[1] = 0x0;
		}
	return (KErrNone);
}
/********************************************************************/

// this utility waits for 200 ms of silence before proceeding. We allow
// a three second timeout here in case we are waiting for a train to
// finish.  we used the modem AT+FRS command - if it fails we'll have
// waited for three seconds in any case, so why bother with an error ? -
// this code is used mostly for error recovery purposes but note that
// not all modems support the +FRS command properly for example the
// Megahertz PCMCIA sportster

void CFaxClass1::ReceiveSilenceL ()
{
	iModem->ExportL (_L8 ("AT+FRS=20\r"));
	if ((iModem->GetMatchL (_L8 ("OK"), 3)) == 0)
		{
		iModem->iOurMessage.Format (_L8 ("Timeout waiting for silence"));
		iModem->ProgressUpdateL ();
		iModem->TxcharL (Kcan);
		if (iModem->GetMatchL (_L8 ("OK"), 1) == 0)
			iModem->TxcharL (Kreturn);
		}
}
/********************************************************************/

