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
#include "FONT8X16.DAT"
#include "ZEROS.DAT"
#include "FAXMDRV.H"
#include "FAXMODEM.H"

#include "FAXLOG.H"
#include <et_phone.h>

const TInt KLineReadTimeout=6;				// < The time-out (in secs) for a line read
const TInt KECMLineReadTimeout=132;			// < The time-out (in secs) for a line read when using Error Correction Mode
const TInt KSubsequentLineReadTimeout=10;	// < The time-out (in secs) for a subsequent line read
const TInt KSubsequentECMLineReadTimeout=140;	// < The time-out (in secs) for a subsequent line read when using Error Correction Mode

/********************************************************************/

// we need to keep a local pointer to the current TDialstorModemPreferences in order to
 // do whatever is required

void CFaxModemDriver::ConstructL (TFaxServerSessionSettings * iFaxServerSessionSettings, RFax::TProgress & aProgress)
{
	iModem = CFaxModem::NewL (iFaxServerSessionSettings, aProgress);
}
/********************************************************************/


CFaxModemDriver::CFaxModemDriver()
                :CBase(), iSharedHandles(NULL)
{
}

CFaxModemDriver::~CFaxModemDriver ()
{
	delete iModem;
}
/********************************************************************/

// function to check that we don't send commands out when there's less than half a second
// before the next RING comes in


/**
*	function to check that we don't send commands out when there's less than half a second
*	before the next RING comes in.
*	This function has been modified by Dmitry Lyokhin for the sake of PIA-586KGE defect fix
*
* @param	aCommand	-	contains the command to be sent to the modem.
*
* @see	ExportL function
*
* @return	number of symbols sent to the modem.
*			
*
*/
TInt CFaxModemDriver::CheckCadenceExportL(const TDesC8 & aCommand)
{
	
	
	const TInt RingTimeout_Sec	= 5;		//-- 5 sec. waiting for 'RING' timeout
	const TInt RingCadence_uSec	= 3000000;	//-- 3 sec. time span after 'RING' receiving during that commands 
											//-- can be sent to the modem

	const TInt CmdSendDelay_uSec= 100000;	//-- 100 ms delay between adjacent commands to the modem

	TTime CurrentTime;

    __FLOG_FAXSRV( _L8("-CFaxModemDriver::CheckCadenceExportL entering"));
  
  	
	//-- @note iCadence now is used like a flag. If its value is 0 (thai is set initially) we will
	//-- try to wait for 'RING' indication i.e synchronize with incoming rings. Otherwise - no.
	while( iCadence.Int64() == 0 )
	{
		if(iTimeOfLastRing.Int64() == 0) 
		{//-- we need to wait for 'RING' from the modem

            __FLOG_FAXSRV( _L8("-CFaxModemDriver::CheckCadenceExportL waiting for RING"));
	  
			//-- wait for 'RING' from modem
			if( iModem->GetMatchL (_L8 ("RING"), RingTimeout_Sec) == 0) 
			{	//User::Leave(KFaxErrReceiveTimeout); //-- 'RING' waiting timeout, leaving
				
				//-- There is no 'RING'indication, no we will not leave, instead of that
				//-- disable sync. with incoming rings and pass on to sending commands to the modem straightforward.
				iCadence = 1;
				break;
			}

			iTimeOfLastRing.UniversalTime(); //-- note the time
		}

		//-- get current time and check if we are in time to send a command to the modem
		CurrentTime.UniversalTime ();
	
		if( CurrentTime < iTimeOfLastRing + TTimeIntervalMicroSeconds32(RingCadence_uSec) )
			break; //-- send the command
		else
		{	//-- wait for the next 'RING'
            __FLOG_FAXSRV( _L8("-CFaxModemDriver::CheckCadenceExportL Resetting"));
			iTimeOfLastRing = 0;
		}
	}

	//-- I had to introduce this delay between sending adjacent commands to the modem because
	//-- some modems (e.g. Nokia9210) lose data.
	iModem->Silence(CmdSendDelay_uSec);

	//-- send the command
	return iModem->ExportL (aCommand);
}

/********************************************************************/

// if we have called FaxInL with a dial request, it calls this routine
 // to dial up a fax on demand service
 //
 // if not polling, we end with a colon to return to command mode and then
 // we delay for whatever time has been requested by the user
 // before returning, at which point FaxInL continues with ATA

void CFaxModemDriver::DialFaxOnDemandL ()
{
	iModem->ExportL (_L8 ("ATD"));
	iModem->ExportL (iFaxServerSessionSettings->iPhoneNumber);

	if (iFaxServerSessionSettings->iMode & KFaxPoll)
		iModem->TxcharL (Kreturn);
	else
		{
		iModem->ExportL (_L8 (";"));
		iModem->TxcharL (Kreturn);
		if ((iModem->GetMatchL (_L8 ("OK"), KDialTimeout)) == 0)
			User::Leave (KFaxErrNoDial);
		iModem->Silence (CLK_TCK * iFaxServerSessionSettings->iFaxOnDemandDelay);
		}
}
/********************************************************************/

// now the routines to add a header line to the top of each fax page

/********************************************************************/

// we send four blank scan lines
 // we create a line of text containing time and date,
 // the Fax ID and Username,
 // the page number and total number of pages,
 // we digitize that, and send it out
 // our font height is 16 so that means 20 scan lines
 // are added to the top of each sent fax

void CFaxModemDriver::SendFaxHeaderL ()
{
	TFaxHeaderInfo faxHeader;
	TRawScanLine headline;
	TRawScanLine fontline;
	TBuf8 < KFaxT4MaxDesLength > encodedHeadLine;
	TTime timeOfTransmission;
	TBuf < 12 > timeText;

	timeOfTransmission.HomeTime();
	timeOfTransmission.FormatL (timeText, (_L ("%F%D%M%Y%H%T")));

	iModem->iOurMessage.Format (_L8 ("Sending page header"));
	iModem->ProgressUpdateL ();

	for (TInt r = iModem->iProgress.iResolution ; r >= 0; r--)
		{
		for (TInt x = 0, y = 4 ; x < 4; x++)
			{
			iModem->iTransmitBuffer.Append (0x00);
			if (iModem->iProgress.iCompression)
				{
				iModem->iTransmitBuffer.Append (Invert (0x00));
				iModem->iTransmitBuffer.Append (Invert (0x34));
				y = 5;
				}
			else
				iModem->iTransmitBuffer.Append (Invert (0x14));
			iModem->iTransmitBuffer.Append (Invert (0xD9));
			iModem->iTransmitBuffer.Append (Invert (0xA8));
			padLineL (y);
			}
		}

	CFaxT4 * faxT4 =CFaxT4::NewLC();
	faxT4->PageInitialize (iModem->iProgress.iResolution, iModem->iProgress.iCompression);
	if(!iSharedHandles)
		{
		User::Leave(KErrBadHandle);
		}
	CFaxHeaderLines * faxheader = CFaxHeaderLines::NewLC (&iSharedHandles->File());	

	faxheader->ReadFaxHeaderInfoL (faxHeader);
	for (TInt n = 0; n < 12; n++)
		timeText[n] -= '0';

	// Forces 2 digit day - 2 digit month - 4 digit year - 2 digit hour - 2 digit minute

	for (TInt scanline = 0; scanline < faxHeader.iHeaderFontHeightInLines; scanline++)
		{
		faxheader->ReadRawHeaderLineL (scanline, headline);
		faxheader->ReadRawFontLineL (scanline, fontline);
		for (TInt fontByte = 0; fontByte < faxHeader.iHeaderFontWidthInBytes; fontByte++)
			{
			headline[((faxHeader.iOffsetToDay) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[0] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToDay + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[1] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToMonth) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[2] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToMonth + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[3] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToYear) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[4] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToYear + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[5] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToYear + 2) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[6] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToYear + 3) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[7] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToHour) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[8] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToHour + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[9] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToMinute) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[10] * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToMinute + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(timeText[11] * faxHeader.iHeaderFontWidthInBytes) + fontByte];

			// put the page info in


			if (iModem->iProgress.iPage > 9)
				headline[((faxHeader.iOffsetToCurrentPage) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[((iModem->iProgress.iPage / 10) * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToCurrentPage + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[((iModem->iProgress.iPage % 10) * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			if (iFaxServerSessionSettings->iTxPages > 9)
				headline[((faxHeader.iOffsetToTotalPages) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[((iFaxServerSessionSettings->iTxPages / 10) * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			headline[((faxHeader.iOffsetToTotalPages + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[((iFaxServerSessionSettings->iTxPages % 10) * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			}

		// we send the line once in normal resolution but twice in fine resolution

		for (TInt r = iModem->iProgress.iResolution ; r >= 0; r--)
			{
			faxT4->EncodeScanLine (headline, encodedHeadLine);

			// always add the extra null at the start for the initial eol

			iModem->iTransmitBuffer.Append (Knul);

			// and then invert and finally send the encoded line

			const TUint8 *px = CONST_CAST (TUint8 *, encodedHeadLine.Ptr ());
			const TUint8 *ex = px + encodedHeadLine.Length ();;
			TUint8 thisChar;
			TInt bytesSent = 0;
			while (px < ex)
				{
				thisChar = Invert (*px++);
				iModem->iTransmitBuffer.Append (thisChar);
				if (thisChar == Kdle)
					iModem->iTransmitBuffer.Append (Kdle);
				iModem->SendTransmitBufferL ();
				bytesSent++;
				}
			padLineL (bytesSent);
			}
		}

	CleanupStack::PopAndDestroy(faxheader);
    CleanupStack::PopAndDestroy(faxT4);
}

/********************************************************************/

// this function handles line padding out for minimum scan line times
 // this feature should really only be needed on class 1 modems ...
 // it takes a single integer parameter, which is the number of bytes sent

void CFaxModemDriver::padLineL (TInt aByteCount)
{
	if (iFaxServerSessionSettings->iFaxClass == EClass1)
		{
		while (iMinlinelength > aByteCount)
			{
			aByteCount++;
			iModem->iTransmitBuffer.Append (Knul);
			iModem->SendTransmitBufferL ();
			}
		}
}
/********************************************************************/

// rewritten to avoid using any AT commands August 1997 Andrew Margolis
 // pointer arithmetic courtesy of Andrew Thoelke
 // Though this doesn't use any AT command but it does need to know
 // about the various modem classes ... GetFaxDataL is in the same boat

// the client has buffered up lines to minimize interaction
 // aData starts with a TInt containing the number of lines
 // Each line follows, preceded with a TInt containing its length
 // which must be copied as it might not be aligned on a 4-byte
 // boundary

TInt CFaxModemDriver::SendFaxDataL (const TDesC8 * aData)
{
	TUint8 thisChar;
	TInt ticks;
	TInt numberOfLines;
	TInt lengthOfLine;

	TUint8 *thisLine = CONST_CAST (TUint8 *, (*aData).Ptr ());
	Mem::Copy (&numberOfLines, thisLine, sizeof (TInt));
	thisLine += sizeof (TInt);

	// buffering debug message

	iModem->iOurMessage.Format (_L8 ("%u lines in buffer taking up %u bytes"), numberOfLines, (*aData).Length ());
	iModem->ProgressUpdateL ();

	while (numberOfLines--)
		{
		Mem::Copy (&lengthOfLine, thisLine, sizeof (TInt));
		thisLine += sizeof (TInt);
		iModem->iProgress.iLines++;

		TInt bytesSent = 0;
		const TUint8 *px = thisLine;
		const TUint8 *ex = px + lengthOfLine;
		while (px < ex)
			{
			thisChar = Invert (*px++);
			iModem->iTransmitBuffer.Append (thisChar);
			if (thisChar == Kdle)
				iModem->iTransmitBuffer.Append (Kdle);
			iModem->SendTransmitBufferL ();
			bytesSent++;
			}
		padLineL (bytesSent);

		// we must check for cancel commands from the modem in class 2.0 transmission

		if (iFaxServerSessionSettings->iFaxClass == EClass2point0)
			{
			while (iModem->Rxstat () != 0)
				{
				ticks = CLK_TCK;
				iModem->RxcharWaitL (ticks);
				if (iModem->iReadone[0] == Kcan)
					{
					iModem->TxcharL (Kdle);
					iModem->TxcharL (Ketx);
					iModem->CommitTransmitBufferL ();
					iModem->Xonoff ();
					iModem->GetMatchL (_L8 ("OK"), 5);
					return (KFaxErrModemDisconnect);
					}
				}
			}
		thisLine += lengthOfLine;
		}
	return (KErrNone);
}
/********************************************************************/
 // This function takes a pointer to a binary data buffer.
 // We are guaranteed that the buffer is big enough to take
 // two entire scan lines up to KMaxT4Des in size.

 // We call GetLineL to fill the buffer up with scan lines, and
 // we keep a count of the number of lines received as an integer
 // at the start of the buffer.  We return when either we haven't
 // enough room to guarantee another line, or when we have received
 // a line of zero length, which means an end of page. The descriptor
 // is set to the correct length on return

 // So on return, aData starts with a TInt containing the number of lines
 // Each line follows, preceded with a TInt containing its length
 // which must be copied as it might not be aligned on a 4-byte
 // boundary - a line of zero length indicates we have reached
 // the end of the page

void CFaxModemDriver::GetFaxDataL (TDes8 * aData)
{
	TUint8 *startData;
	TUint8 *lineData;
	const TUint8 *maxData;
	TInt lineLength;
	TInt numberOfLines = 0;

	lineData = startData = CONST_CAST (TUint8 *, (*aData).Ptr ());
	maxData = startData + (*aData).MaxLength () - KMaxT4Des - sizeof (TInt);

	(*aData).SetMax ();
	lineData += sizeof (TInt);

	for (;;)
		{
		numberOfLines++;
		lineLength = GetLineL (lineData);
		Mem::Copy (lineData, &lineLength, sizeof (TInt));
		lineData += sizeof (TInt);
		lineData += lineLength;
		if (lineData > maxData)
			break;
		if (lineLength == 0)
			break;
		}
	Mem::Copy (startData, &numberOfLines, sizeof (TInt));
	(*aData).SetLength (lineData - startData);

	// buffering debug message

	iModem->iOurMessage.Format (_L8 ("%u lines in buffer taking up %u bytes"), numberOfLines, (*aData).Length ());
	iModem->ProgressUpdateL ();
}
/********************************************************************/
// This function takes a pointer to a binary data buffer.
 // We are guaranteed that the buffer is big enough to take
 // an entire scan line up to KMaxT4Des in size.
 // We receive the scan line, with a leave if we timeout.
 // We return with the length of the scan line and the buffer has
 // a space for this to be placed as an integer by the caller,
 // followed by the scan line data.  If the length of the scan line
 // is zero, we have reached the end of the page

TInt CFaxModemDriver::GetLineL (TUint8 * aFaxData)
{
	TUint8 *lineStart;
	TUint8 *lineEnd;
	TUint8 *currentByte;

	lineStart = currentByte = (aFaxData + sizeof (TInt));
	lineEnd = lineStart + KMaxT4Des;

	TUint8 thisChar = 0;
	TUint8 leading0s = 0;
	TUint8 trailing0s = 0;
	TInt nullcount = 0;

	TInt ticks = CLK_TCK * KLineReadTimeout;
	if (iModem->iProgress.iECM != 0)
		ticks = CLK_TCK * KECMLineReadTimeout;

	iModem->iProgress.iLines++;

	TInt bol = 1;
	TUint8 lastChar = 0xff;

	// lastChar set to 0xff flags the entry to the function
	// during iterations lastChar must be either 0 or 1
	// we always come here just after detecting an EOL, and the character
	// which contains the EOL bit is guaranteed to be re-readable
	// lastChar set to 0xff indicates that's is not been read,
	// so we re-read it, set its trailing0s, and put it in the buffer
	// without bothering to do any dle checking (we already know it's ok)

	for (;;)
		{
		if (lastChar == 0xff)
			{
			lastChar = 0;
			thisChar = iModem->iReadone[0];
			}
		else
			{
			if ((iModem->RxcharWaitL (ticks)) == 0)
				User::Leave (KFaxErrReceiveTimeout);
			thisChar = iModem->iReadone[0];

			// check if we have the character after a leading dle
			// if we have unmark the last character as being a dle -
			// dle etx is end of data
			// dle dle is a shielded dle
			// dle sub is two times dle for class 2.0 only
			// dle and anything else we ignore

			if (lastChar == Kdle)
				{
				lastChar = 0;

				if (thisChar == Ketx)
					{
					iModem->iOurMessage.Format (_L8 ("<dle><etx> detected after %u lines"), iModem->iProgress.iLines);
					iModem->ProgressUpdateL ();
					return (0);
					}

				if (iFaxServerSessionSettings->iFaxClass == EClass2point0)
					{
					if (thisChar == 0x1a)
						{
						thisChar = Kdle;
						*currentByte++ = Invert (thisChar);   // invert class 2.0
						if (currentByte == lineEnd)
							{
							return (KMaxT4Des);
							}
						trailing0s = 4;
						}
					}

				if (thisChar != Kdle)
					continue;
				}                   // drop through only with a data dle

			// if not a trailing dle
			// check if this character is itself a leading dle
			// drop through only if it isn't

			else if (thisChar == Kdle)
				{
				lastChar = Kdle;
				continue;
				}
			}

		// if we've received six EOL codes already, ignore everything
		// till dle etx arrives

		if (bol == 6)
			continue;

		// have we a null ? if yes we ignore nulls if they come in
		// anything more than pairs - if no, we zero nullcount and
		// invert the byte back the right way for non-class 2 modems
		// THIS LAST IS IMPORTANT

		if (thisChar == Knul)
			{
			if (nullcount == 2)
				continue;
			else
				nullcount++;
			}
		else
			{
			nullcount = 0;
			if (iFaxServerSessionSettings->iFaxClass != EClass2)
				thisChar = Invert (thisChar);
			}

		// count the leading zeros in this byte

		leading0s = zerotable[thisChar][0];

		// if the leading zeros in this byte and the trailing zeros in the
		// previous byte total 11 or more we have ourselves an EOL
		// so we write the data we have so far as an entire line
		// we are guaranteed than an eol will span at least two bytes
		// so the data we have must include the end of the last line
		// if this is a nul we don't write anything yet as we haven't
		// detected a proper eol code
		// we don't write anything for consecutibe eols

		if (((trailing0s + leading0s) > 10) && (thisChar != Knul))
			{
			bol++;
			if ((bol == 1) && (currentByte != lineStart))
				{
				return (currentByte - lineStart);
				}
			if (iModem->iProgress.iECM == 0)
				ticks = CLK_TCK * KSubsequentLineReadTimeout;
			else
				ticks = CLK_TCK * KSubsequentECMLineReadTimeout;				// 11/1/01 AMC: ECM requires longer time-outs due to retries
			}

		// else if we had received an eol and this character is not nul
		// we have ourselves a new line start

		else
			{
			if (bol)
				if (thisChar != Knul)
					bol = 0;
			}

		// if we have a nul, add 8 to our trailing zero bits
		// else count them by hand

		if (thisChar == Knul)
			trailing0s += 8;
		else
			trailing0s = zerotable[thisChar][1];

		// ignore multiple eols

		if (bol > 1)
			continue;

		// save everything else - we've already inverted the data if needed

		*currentByte++ = thisChar;
		if (currentByte == lineEnd)
			{
			return (KMaxT4Des);
			}
		}
}
/********************************************************************/

TInt CFaxModemDriver::TxStartPageL ()
{
	iModem->iProgress.iPhase = EDataTransfer;
	iModem->iProgress.iLines = 0;
	iModem->iOurMessage.Format (_L8 ("About to send page %u"), ++iModem->iProgress.iPage);
	iModem->ProgressUpdateL ();
	iModem->Xonon ();

	// for class 1 modems we start each page with a short burst of binary 1s

	if (iFaxServerSessionSettings->iFaxClass == EClass1)
		{
		for (TInt x = (iActualFaxSpeed * 20 / 8); x; x--)
			{
			iModem->iTransmitBuffer.Append (0xff);
			iModem->SendTransmitBufferL ();
			}
		iModem->CommitTransmitBufferL ();
		}

	// we're now in phase C so we start the page by sending the fax header

	SendFaxHeaderL ();
	return (KErrNone);
}
/********************************************************************/

TInt CFaxModemDriver::RxStartPageL ()
{
	TUint8 thisChar, leading0s, trailing0s = 0;
	TInt ticks = CLK_TCK * KLineReadTimeout;
	if (iModem->iProgress.iECM != 0)
		ticks = CLK_TCK * KECMLineReadTimeout;  // ECM mode requires longer time-outs due to retries

	iModem->iProgress.iPhase = EDataTransfer;
	iModem->iProgress.iLines = 0;
	iModem->iOurMessage.Format (_L8 ("Awaiting page %u"), ++iModem->iProgress.iPage);
	iModem->ProgressUpdateL ();
	trailing0s = 0;

	// this function looks for the start of the received fax
	// this is the first EOL code - we invert bytes for non-class 2 modems

	for (;;)
		{
		if ((iModem->RxcharWaitL (ticks)) == 0)
			return (KFaxErrReceiveTimeout);
		thisChar = iModem->iReadone[0];
		if (iFaxServerSessionSettings->iFaxClass != EClass2)
			thisChar = Invert (thisChar);
		leading0s = zerotable[thisChar][0];
		if (((trailing0s + leading0s) > 10) && (thisChar != 0))
			break;
		if (thisChar == Knul)
			trailing0s += 8;
		else
			trailing0s = zerotable[thisChar][1];
		}

	// we've found the first EOL - it's left in iModem->iReadone[0]

	iModem->iOurMessage.Format (_L8 ("Receiving data .... "));
	iModem->ProgressUpdateL ();
	return (KErrNone);
}
/********************************************************************/

TInt CFaxModemDriver::RxConnectL ()
{
	return (KFaxErrWrongModemType);
}
TInt CFaxModemDriver::RxPrePageL ()
{
	return (KFaxErrWrongModemType);
}
TInt CFaxModemDriver::RxPostPageL ()
{
	return (KFaxErrWrongModemType);
}
TInt CFaxModemDriver::TxConnectL ()
{
	return (KFaxErrWrongModemType);
}
TInt CFaxModemDriver::TxPrePageL ()
{
	return (KFaxErrWrongModemType);
}
TInt CFaxModemDriver::TxPostPageL ()
{
	return (KFaxErrWrongModemType);
}
/********************************************************************/




