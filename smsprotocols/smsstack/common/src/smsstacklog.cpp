// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Logging macros code specific structures in SMSStack.
// 
//

/**
 @file
 @internalComponent
*/
 
#include "gsmupdu.h"
#include "smsstacklog.h"


//
// All functions in this file are available only if logging is enabled.
//
#ifdef _SMS_LOGGING_ENABLED


/**
 *  Number of characters per line during hex dump logging.
 */
const TInt  KHexDumpCharsPerLine = 32;


/**
 *  Converts a PDU from a buffer into a ASCII string and decodes it.
 *
 *  @param aPDU         PDU to log.
 *  @param aCommandPdu  Flag to say if this is a command PDU.
 */
void LogSmsIfPDU(const TDesC8& aText, const TDesC8& aPDU, TBool aCommandPdu)
	{
	TBuf8<128>  tmpBuf;

	//
	// Log the PDU as raw hex...
	//
	tmpBuf.Copy(aText);
	tmpBuf.Append(_L8("RAW:  "));

	LOGSMSIFHEXBUF(tmpBuf, aPDU);

	//
	// Log the first octet...
	//
	// bit	   7	6	 5	  4    3	2	 1	  0
	// submit  rp	udhi srr  vpf  vpf	rd	 0	  1  mti, rd,  vpf, rp, udhi, srr,
	// deliver rp	udhi sri			mms  0	  0  mti, mms, rp, udhi, sri
	// status		udhi srq			mms  1	  0  mti, udhi, mms, srq
	// command		udhi srr				 1    0  mti, udhi, srr
	//
	enum TSmsVp
		{
		EVpAbs,
		EVpEnh,
		EVpRel,
		EVpNone,
		};
	
	CSmsPDU::TSmsPDUType  pduType = (CSmsPDU::TSmsPDUType) -1;
	TInt  octetOffset = 0;
	TText8  firstOctet = aPDU[octetOffset++];
	TBool  udhiPresent = EFalse;
	TSmsVp  vpf = EVpRel;

	tmpBuf.Zero();
	
	if ((firstOctet & 0x03) == 0x00)
		{
		pduType = CSmsPDU::ESmsDeliver;
		tmpBuf.Append(_L8("DELIVER "));
		}
	else if ((firstOctet & 0x03) == 0x01)
		{
		pduType = CSmsPDU::ESmsSubmit;
		tmpBuf.Append(_L8("SUBMIT  "));
		}
	else if((firstOctet & 0x03) == 0x02)
		{
		if (aCommandPdu)
			{
			pduType = CSmsPDU::ESmsCommand;
			tmpBuf.Append(_L8("COMMAND "));
			}
		else
			{
			pduType = CSmsPDU::ESmsStatusReport;
			tmpBuf.Append(_L8("STATUS  "));
			}
		}
	else
		{
		tmpBuf.Append(_L8("UNKNOWN "));
		}

	if (pduType == CSmsPDU::ESmsSubmit  ||  pduType == CSmsPDU::ESmsDeliver)
		{
		if (firstOctet & 0x80)
			{
			tmpBuf.Append(_L8("    1"));
			}
		else
			{
			tmpBuf.Append(_L8("    0"));
			}
		}
	else
		{
		tmpBuf.Append(_L8("    -"));
		}

	if (firstOctet & 0x40)
		{
		tmpBuf.Append(_L8("    1"));
		udhiPresent = ETrue;
		}
	else
		{
		tmpBuf.Append(_L8("    0"));
		}

	if (firstOctet & 0x20)
		{
		tmpBuf.Append(_L8("    1"));
		}
	else
		{
		tmpBuf.Append(_L8("    0"));
		}

	if (pduType == CSmsPDU::ESmsDeliver  ||  pduType == CSmsPDU::ESmsStatusReport)
		{
		// MMS is Deliver or Status Report PDUs only...
		if (firstOctet & 0x04)
			{
			tmpBuf.Append(_L8("    1"));
			}
		else
			{
			tmpBuf.Append(_L8("    0"));
			}
		}
	else
		{
		tmpBuf.Append(_L8("    -"));
		}

	if (pduType == CSmsPDU::ESmsSubmit)
		{
		// Reject Duplicates and VP is for Sumbit PDUs only.
		if (firstOctet & 0x04)
			{
			tmpBuf.Append(_L8("    1"));
			}
		else
			{
			tmpBuf.Append(_L8("    0"));
			}

		if ((firstOctet & 0x18) == 0x18)
			{
			tmpBuf.Append(_L8("    ABS"));
			vpf = EVpAbs;
			}
		else if ((firstOctet & 0x18) == 0x10)
			{
			tmpBuf.Append(_L8("    REL"));
			vpf = EVpRel;
			}
		else if ((firstOctet & 0x18) == 0x08)
			{
			tmpBuf.Append(_L8("    ENH"));
			vpf = EVpEnh;
			}
		else
			{
			tmpBuf.Append(_L8("    NO "));
			vpf = EVpNone;
			}
		}
	else
		{
		tmpBuf.Append(_L8("    -      -"));
		}

	LOGSMSIF2("%S      HEX   MTI        RP UDHI  SRX  MMS   RD     VP", &aText);
	LOGSMSIF4("%SFO:   0x%02X  %S", &aText, firstOctet, &tmpBuf);

	if (pduType == -1)
		{
		// Not supported!
		return;
		}

	//
	//
	// Message reference (submit, status report and command only)...
	//
	if (pduType == CSmsPDU::ESmsSubmit  ||
	    pduType == CSmsPDU::ESmsStatusReport  ||
	    pduType == CSmsPDU::ESmsCommand)
		{
        LOGSMSIF3("%SMR:   0x%02X", &aText, aPDU[octetOffset]);
        octetOffset++;
		}

	//
	// Command's PID, CT and MN...
	//
	if (pduType == CSmsPDU::ESmsCommand)
		{
		LOGSMSIF3("%SPID:  0x%02X", &aText, aPDU[octetOffset]);
        octetOffset++;

		LOGSMSIF3("%SCT:   0x%02X", &aText, aPDU[octetOffset]);
        octetOffset++;

		LOGSMSIF3("%SMN:   0x%02X", &aText, aPDU[octetOffset]);
        octetOffset++;
		}

	//
	// Log the telephone number...
	//
	TInt  telLength = (TInt) aPDU[octetOffset];
	TInt  typeOfNumber = aPDU[octetOffset+1];
	
	octetOffset += 2;
		
	if (telLength >= 0  &&  telLength <= 20)
		{
		// Convert the number to ascii...
		tmpBuf.Zero();
		
		for (TInt  count = 0;  count < (telLength + 1) / 2;  count++)
			{
			tmpBuf.AppendFormat(_L8("%1X%1X"), aPDU[octetOffset] & 0x0f,
								(aPDU[octetOffset] >> 4) & 0x0f);
			octetOffset++;
			}
		
		tmpBuf.SetLength(telLength);

		LOGSMSIF4("%STEL:  0x%02X  %S", &aText, typeOfNumber, &tmpBuf);
		}
	else
		{
		LOGSMSIF3("%STEL:  Illegal length value (%d)!", &aText, telLength);
		return;
		}

	//
	// PID and the DCS (submit and deliver only)...
	//
	if (pduType == CSmsPDU::ESmsSubmit  ||  pduType == CSmsPDU::ESmsDeliver)
		{
		LOGSMSIF3("%SPID:  0x%02X", &aText, aPDU[octetOffset]);
		octetOffset++;

		tmpBuf.Zero();

		TInt  dcs = aPDU[octetOffset];
		octetOffset++;

		if ((dcs & 0xF0 != 0xF0)  &&
		    ((dcs & 0xD0) == 0x00  ||  (dcs & 0xC0) == 0xC0  ||
		     (dcs & 0xD0) == 0xD0  ||  (dcs & 0xE0) == 0xE0))
			{
			//
			// Classless messages:
			// 00x0xxxx GSM 7 bit default alphabet with no message class
			// 1100xxxx Message Waiting Indication Group: Discard Message
			// 1101xxxx Message Waiting Indication Group: Store Message
			// 1110xxxx Message Waiting Indication Group: Store Message (UCS2 character set)
			//
			tmpBuf.Append(_L8("NO        "));
			}
		else if ((dcs & 0xC0) == 0x00  ||  (dcs & 0x40) == 0x40  ||  (dcs & 0xF0) == 0xF0)
			{
			//
			// Message has a class:
			// 00xxxxxx General Data Coding indication
			// 01xxxxxx Message Marked for Automatic Deletion Group
			// 1111xxxx Data coding/message class
			//
			if ((dcs & 0x03) == 0x00)		//class 0 sms
				{
				tmpBuf.Append(_L8("0         "));
				}
			else if((dcs & 0x03) == 0x03)	//class 3 sms
				{
				tmpBuf.Append(_L8("3         "));
				}
			else if((dcs & 0x01) == 0x01)	//class 1 sms
				{
				tmpBuf.Append(_L8("1         "));
				}
			else if((dcs & 0x02) == 0x02)	//class 2 sms
				{
				tmpBuf.Append(_L8("2         "));
				}
			}
		else
			{
			//
			// 1000xxxx...1011xxxx Reserved coding groups
			//
			tmpBuf.Append(_L8("NO        "));
			}

		if ((dcs & 0xF0) == 0xF0)
			{
			// Data coding/message class
			if ((dcs & 0x04) == 0x00)
				{
				tmpBuf.Append(_L8(" 7 BIT   "));
				}
			else if ((dcs & 0x04) == 0x04)
				{
				tmpBuf.Append(_L8(" 8 BIT   "));
				}
			}
		else if ((dcs & 0xC0) == 0xC0  ||  (dcs & 0xD0) == 0xD0)
			{
			//Message Waiting Indication: Discard Message or Store Message
			tmpBuf.Append(_L8(" 7 BIT   "));
			}
		else if ((dcs & 0xE0) == 0xE0)
			{
			//Message Waiting Indication: Store Message (UCS2 character set)
			tmpBuf.Append(_L8("16 BIT   "));
			}
		else if ((dcs & 0xC0) == 0x00 ||  (dcs & 0x40) == 0x40)
			{
			// General Data Coding indication or Automatic Deletion Group
			if ((dcs & 0x0C) == 0x00)
				{
				tmpBuf.Append(_L8(" 7 BIT   "));
				}
			else if ((dcs & 0x0C) == 0x0C)
				{
				tmpBuf.Append(_L8("Reserved "));
				}
			else if ((dcs & 0x04) == 0x04)
				{
				tmpBuf.Append(_L8(" 8 BIT   "));
				}
			else if ((dcs & 0x08) == 0x08)
				{
				tmpBuf.Append(_L8("16 BIT   "));
				}
			}
		else
			{
			// Reserved coding groups
			tmpBuf.Append(_L("Reserved "));
			}

		if ((dcs & 0xF0 != 0xF0)  &&
		    ((dcs & 0xC0) == 0xC0  ||  (dcs & 0xD0) == 0xD0  ||  (dcs & 0xE0) == 0xE0))
			{
			if ((dcs & 0x03) == 0x00)
				{
				// Voicemail Message Waiting
				tmpBuf.Append(_L8("Voicemail Message Waiting"));
				}
			else if ((dcs & 0x03) == 0x03)
				{
				// Other Message Waiting
				tmpBuf.Append(_L8("Other Message Waiting"));
				}
			else if ((dcs & 0x01) == 0x01)
				{
				// Fax Message Waiting
				tmpBuf.Append(_L8("Fax Message Waiting"));
				}
			else if ((dcs & 0x02) == 0x02)
				{
				// Electronic Mail Message Waiting
				tmpBuf.Append(_L8("Electronic Mail Message Waiting"));
				}
			else
				{
				tmpBuf.Append(_L8("-"));
				}
			}
		else
			{
			tmpBuf.Append(_L8("-"));
			}

		LOGSMSIF2("%SDCS:  HEX   CLASS      DCS     INDICATION TYPE", &aText);
		LOGSMSIF4("%S      0x%02X  %S", &aText, dcs, &tmpBuf);
		}

	//
	// VP (submit only)...
	//
	if (pduType == CSmsPDU::ESmsSubmit)
		{
		if (vpf == EVpRel)
			{
			LOGSMSIF3("%SVP:   %d (Relative)", &aText, aPDU[octetOffset++]);
			}
		else if (vpf == EVpAbs)
			{
			// absolute format	  -  TODO to look for the right format
			tmpBuf.Zero();
			
			for (TInt index = 0;  index < 7;  index++)
				{
				tmpBuf.AppendFormat(_L8("%02X"), aPDU[octetOffset + index]);
				}
			
			LOGSMSIF3("%SVP:   %S (Absolute)", &aText, &tmpBuf);
			octetOffset += 7;
			}
		else if (vpf == EVpEnh)
			{
			// enhanced validity period   -  TODO to look for the right format
			tmpBuf.Zero();
			
			for (TInt index = 0;  index < 7;  index++)
				{
				tmpBuf.AppendFormat(_L8("%02X"), aPDU[octetOffset + index]);
				}
			
			LOGSMSIF3("%SVP:   %S (Enhanced)", &aText, &tmpBuf);
			octetOffset += 7;
			}
		}

	//
	// SCTS (deliver and status)  -  TODO to look for the right format
	//
	if (pduType == CSmsPDU::ESmsDeliver  ||  pduType == CSmsPDU::ESmsStatusReport)
		{
		tmpBuf.Zero();
		
		for (TInt index = 0;  index < 7;  index++)
			{
			tmpBuf.AppendFormat(_L8("%02X"), aPDU[octetOffset + index]);
			}
			
		LOGSMSIF3("%SSCTS: %S", &aText, &tmpBuf);
		octetOffset += 7;
		}
	
	//
	// Getting the DT and the status of the SR (SR only)
	// TODO	to look for some appended octet after the status, eg. carried ud
	//      and to analise the status
	//
	if (pduType == CSmsPDU::ESmsStatusReport)
		{
		tmpBuf.Zero();
		
		for (TInt index = 0;  index < 7;  index++)
			{
			tmpBuf.AppendFormat(_L8("%02X"), aPDU[octetOffset + index]);
			}
			
		LOGSMSIF3("%SDT:   %S", &aText, &tmpBuf);
		octetOffset += 7;

		LOGSMSIF3("%SST:   %02X", &aText, aPDU[octetOffset]);
		octetOffset++;
		}

	//
	// Getting the UDL   -    TODO to add UD later and to decode the UDH
	//
	if (pduType == CSmsPDU::ESmsDeliver  ||  pduType == CSmsPDU::ESmsSubmit)
		{
		if (udhiPresent)
			{
			TInt  udl  = aPDU[octetOffset++];
			TInt  udhl = aPDU[octetOffset++];
			
			if (udhl > 1)
				{
				tmpBuf.Zero();
			
				// getting iei_a
				TInt  iei_a = aPDU[octetOffset++];
	
				// getting ieidl_a
				TInt  ieidl_a = aPDU[octetOffset++];
	
				if (ieidl_a < udhl)
					{
					//
					// With EDS, IEIDL can  be almost the whole payload; need to limit
					// length of debug or increase udlbuf2 to around 300 bytes (which would
					// make stack frame around 700 bytes out of allowed 2kB; seems too much)
					// TODO: think whether we should explicitly show this truncation, eg with "..."
					//
					for (TInt index = 0;  index < ieidl_a;  index++)
						{
						tmpBuf.AppendFormat(_L8("%02X"), aPDU[octetOffset + index]);
						if (tmpBuf.Length() > tmpBuf.MaxLength() - 2  ||
							octetOffset + index == aPDU.Length() - 1)
							{
							break;
							}
						}
					}
				else
					{
					LOGSMSIF3("%SUDL:  Problems with the ieidl_a %d being less that UDHL",
							  &aText, ieidl_a);
					}
	
				if ((ieidl_a + 1) > udhl)
					{
					LOGSMSIF2("%SUDL:  Corrupted or implement decoding for second iei_b, iei_n!",
							  &aText);
					}
	
				LOGSMSIF2("%SUDL:  HEX    UDHL   IEI_A  IEIDL_A   ", &aText);
				LOGSMSIF7("%S      0x%02X   0x%02X   0x%02X   0x%02X  %S", &aText,
						  udl, udhl, iei_a, ieidl_a, &tmpBuf);
				}
			else
				{
				LOGSMSIF2("%SUDL:  Corrupted because TP-UDHP is TRUE and TP-UDHL is less than 1!", &aText);
				LOGSMSIF2("%SUDL:  HEX   UDHL   IEI_A", &aText);
				LOGSMSIF4("%S      0x%02X    0x%02X", &aText, udl, udhl);
				}
			}
		else
			{
			TInt  udl  = aPDU[octetOffset++];
			
			LOGSMSIF2("%SUDL:  HEX    UDHL", &aText);
			LOGSMSIF3("%S      0x%02X   -", &aText, udl);
			}
		}
	} // LogSmsIfPDU


void LogSmsIfHexBuf(const TDesC8& aText, const TDesC8& aHexBuf)
    {
	//
	// Print the PDU in hex in rows of upto KHexDumpCharsPerLine bytes...
	//
	TBuf8<KHexDumpCharsPerLine * 2>  hexLine;
    TInt  length = aHexBuf.Length();
	TInt  position;

	for (position = 0;  position < length;  position += KHexDumpCharsPerLine)
		{
		TInt  bytesToLog = length - position;
		TInt  byteIndex;
		
		if (bytesToLog > KHexDumpCharsPerLine)
			{
			bytesToLog = KHexDumpCharsPerLine;
			}

		hexLine.Zero();

		for (byteIndex = 0;  byteIndex < bytesToLog;  byteIndex++)
			{
			hexLine.AppendFormat(_L8("%02X"), aHexBuf[position + byteIndex]);
			}

		LOGSMSIF3("%S%S", &aText, &hexLine);
		}
    } // LogSmsIfHexBuf


/**
 *	Logs a Type of Number enum.
 *
 *  @param aTON  Enum to log.
 */
void LogSmsIfTypeOfNumber(const TDesC8& aText, RMobilePhone::TMobileTON aTON)
	{
	switch (aTON)
		{
		case RMobilePhone::EUnknownNumber:
			{
			LOGSMSIF2("%SEUnknownNumber", &aText);
			}
			break;

		case RMobilePhone::EInternationalNumber:
			{
			LOGSMSIF2("%SEInternationalNumber", &aText);
			}
			break;

		case RMobilePhone::ENationalNumber:
			{
			LOGSMSIF2("%SENationalNumber", &aText);
			}
			break;

		case RMobilePhone::ENetworkSpecificNumber:
			{
			LOGSMSIF2("%SENetworkSpecificNumber", &aText);
			}
			break;

		case RMobilePhone::ESubscriberNumber:
			{
			LOGSMSIF2("%SESubscriberNumber", &aText);
			}
			break;

		case RMobilePhone::EAlphanumericNumber:
			{
			LOGSMSIF2("%SEAlphanumericNumber", &aText);
			}
			break;

		case RMobilePhone::EAbbreviatedNumber:
			{
			LOGSMSIF2("%SEAbbreviatedNumber", &aText);
			}
			break;

		default:
			{
			LOGSMSIF3("%S<unknown enum %d>", &aText, aTON);
			}
			break;
		}
	} // LogSmsIfTypeOfNumber


/**
 *	Logs a Numbering Plan enum.
 *
 *  @param aNPI  Enum to log.
 */
void LogSmsIfNumberingPlan(const TDesC8& aText, RMobilePhone::TMobileNPI aNPI)
	{
	switch (aNPI)
		{
		case RMobilePhone::EUnknownNumberingPlan:
			{
			LOGSMSIF2("%SEUnknownNumberingPlan", &aText);
			}
			break;

		case RMobilePhone::EIsdnNumberPlan:
			{
			LOGSMSIF2("%SEIsdnNumberPlan", &aText);
			}
			break;

		case RMobilePhone::EDataNumberPlan:
			{
			LOGSMSIF2("%SEDataNumberPlan", &aText);
			}
			break;

		case RMobilePhone::ETelexNumberPlan:
			{
			LOGSMSIF2("%SETelexNumberPlan", &aText);
			}
			break;

		case RMobilePhone::EServiceCentreSpecificPlan1:
			{
			LOGSMSIF2("%SEServiceCentreSpecificPlan1", &aText);
			}
			break;

		case RMobilePhone::EServiceCentreSpecificPlan2:
			{
			LOGSMSIF2("%SEServiceCentreSpecificPlan2", &aText);
			}
			break;

		case RMobilePhone::ENationalNumberPlan:
			{
			LOGSMSIF2("%SENationalNumberPlan", &aText);
			}
			break;

		case RMobilePhone::EPrivateNumberPlan:
			{
			LOGSMSIF2("%SEPrivateNumberPlan", &aText);
			}
			break;

		case RMobilePhone::EERMESNumberPlan:
			{
			LOGSMSIF2("%SEERMESNumberPlan", &aText);
			}
			break;

		default:
			{
			LOGSMSIF3("%S<unknown enum %d>", &aText, aNPI);
			}
			break;
		}
	} // LogSmsIfNumberingPlan


/**
 *	Logs contents of TMobileGsmSmsEntryV1 object.
 *
 *  @param aSmsGsmEntryV1  Entry to log.
 */
void LogSmsIfSmsEntry(const TDesC8& aText,
					  const RMobileSmsStore::TMobileGsmSmsEntryV1& aSmsGsmEntryV1)
	{
	//
	// Header and index...
	//
	LOGSMSIF2("%SRMobileSmsStore::TMobileGsmSmsEntryV1:", &aText);
	LOGSMSIF4("%S  iIndex=%d (0x%08x)", &aText, aSmsGsmEntryV1.iIndex,
			  aSmsGsmEntryV1.iIndex);
	
	//
	// Message status...
	//
	switch (aSmsGsmEntryV1.iMsgStatus)
		{
		case RMobileSmsStore::EStoredMessageUnknownStatus:
			{
			LOGSMSIF2("%S  iStoreStats=EStoredMessageUnknownStatus", &aText);
			}
			break;

		case RMobileSmsStore::EStoredMessageUnread:
			{
			LOGSMSIF2("%S  iStoreStats=EStoredMessageUnread", &aText);
			}
			break;

		case RMobileSmsStore::EStoredMessageRead:
			{
			LOGSMSIF2("%S  iStoreStats=EStoredMessageRead", &aText);
			}
			break;

		case RMobileSmsStore::EStoredMessageUnsent:
			{
			LOGSMSIF2("%S  iStoreStats=EStoredMessageUnsent", &aText);
			}
			break;

		case RMobileSmsStore::EStoredMessageSent:
			{
			LOGSMSIF2("%S  iStoreStats=EStoredMessageSent", &aText);
			}
			break;

		case RMobileSmsStore::EStoredMessageDelivered:
			{
			LOGSMSIF2("%S  iStoreStats=EStoredMessageDelivered", &aText);
			}
			break;

		default:
			{
			LOGSMSIF3("%S  iStoreStats=<unknown enum %d>", &aText,
					  aSmsGsmEntryV1.iMsgStatus);
			}
			break;
		}

	//
	// Service centre number...
	//
	TBuf8<RMobilePhone::KMaxMobileTelNumberSize>  numberIn8bit;

	numberIn8bit.Copy(aSmsGsmEntryV1.iServiceCentre.iTelNumber);
	LOGSMSIF3("%S  SRC Address=\"%S\"", &aText, &numberIn8bit);

	//
	// Service Centre Type Of Number...
	//
	TBuf8<128>  tmpBuf;
	
	tmpBuf.Copy(aText);
	tmpBuf.Append(_L8("  SRC AddrTON="));

	LOGSMSIFTYPEOFNUMBER(tmpBuf, aSmsGsmEntryV1.iServiceCentre.iTypeOfNumber);

	//
	// Number Plan...
	//
	tmpBuf.Copy(aText);
	tmpBuf.Append(_L8("  SRC AddrNPI="));

	LOGSMSIFNUMBERINGPLAN(tmpBuf, aSmsGsmEntryV1.iServiceCentre.iNumberPlan);

	//
	// PDU...
	//
	tmpBuf.Copy(aText);
	tmpBuf.Append(_L8("  PDU: "));

	LOGSMSIFPDU(tmpBuf, aSmsGsmEntryV1.iMsgData, EFalse);
	} // LogSmsIfSmsEntry


/**
 *	Logs contents of TMobileSmsSendAttributesV1 object.
 *
 *  @param aAttrib  Attributes to log.
 */
void LogSmsIfSendAttributes(const TDesC8& aText,
							const RMobileSmsMessaging::TMobileSmsSendAttributesV1& aAttrib)
	{
	LOGSMSIF2("%SRMobileSmsMessaging::TMobileSmsSendAttributesV1:", &aText);
	LOGSMSIF3("%S  iFlags=0x%08x", &aText, (TInt)(aAttrib.iFlags));

	if (aAttrib.iFlags & RMobileSmsMessaging::KGsmServiceCentre)
		{
		TBuf8<128 + RMobilePhone::KMaxMobileTelNumberSize>  tmpBuf;
		
		tmpBuf.Copy(aAttrib.iGsmServiceCentre.iTelNumber);
		LOGSMSIF3("%S  SRC Address=%S", &aText, &tmpBuf);

		tmpBuf.Copy(aText);
		tmpBuf.Append(_L8("  SRC AddrTON="));

		LOGSMSIFTYPEOFNUMBER(tmpBuf, aAttrib.iGsmServiceCentre.iTypeOfNumber);

		tmpBuf.Copy(aText);
		tmpBuf.Append(_L8("  SRC AddrNPI="));

		LOGSMSIFNUMBERINGPLAN(tmpBuf, aAttrib.iGsmServiceCentre.iNumberPlan);
		}

	if (aAttrib.iFlags & RMobileSmsMessaging::KSmsDataFormat)
		{
		switch (aAttrib.iDataFormat)
			{
			case RMobileSmsMessaging::EFormatUnspecified:
				{
				LOGSMSIF2("%S  iDataFormat=EFormatUnspecified", &aText);
				}
				break;

			case RMobileSmsMessaging::EFormatGsmTpdu:
				{
				LOGSMSIF2("%S  iDataFormat=EFormatGsmTpdu", &aText);
				}
				break;

			case RMobileSmsMessaging::EFormatCdmaTpdu:
				{
				LOGSMSIF2("%S  iDataFormat=EFormatCdmaTpdu", &aText);
				}
				break;

			default:
				{
				LOGSMSIF3("%S  iDataFormat=<unknown enum %d>", &aText,
						  aAttrib.iDataFormat);
				}
				break;
			}
		}

	if (aAttrib.iFlags & RMobileSmsMessaging::KCdmaTeleservice)
		{
		LOGSMSIF3("%S  iCdmaTeles=0x08X", &aText, aAttrib.iCdmaTeleservice);
		}

	if (aAttrib.iFlags & RMobileSmsMessaging::KCdmaServiceCategory)
		{
		LOGSMSIF3("%S  iCdmaServ=0x08X", &aText, aAttrib.iCdmaServiceCategory);
		}

	if (aAttrib.iFlags & RMobileSmsMessaging::KRemotePartyInfo)
		{
		TBuf8<RMobilePhone::KMaxMobileTelNumberSize>  tmpBuf;
		
		tmpBuf.Copy(aAttrib.iDestination.iTelNumber);
		LOGSMSIF3("%S  DST Address=%S", &aText, &tmpBuf);

		tmpBuf.Copy(aText);
		tmpBuf.Append(_L8("  DST AddrTON="));

		LOGSMSIFTYPEOFNUMBER(tmpBuf, aAttrib.iDestination.iTypeOfNumber);

		tmpBuf.Copy(aText);
		tmpBuf.Append(_L8("  DST AddrNPI="));

		LOGSMSIFNUMBERINGPLAN(tmpBuf, aAttrib.iDestination.iNumberPlan);
		}

	if (aAttrib.iFlags & RMobileSmsMessaging::KMoreToSend)
		{
		if (aAttrib.iMore)
			{
			LOGSMSIF2("%S  iMore=ETrue", &aText);
			}
		else
			{
			LOGSMSIF2("%S  iMore=EFalse", &aText);
			}
		}

	if (aAttrib.iFlags & RMobileSmsMessaging::KMessageReference)
		{
		LOGSMSIF3("%S  iMsgRef=0x08X", &aText, aAttrib.iMsgRef);
		}

	if(aAttrib.iFlags & RMobileSmsMessaging::KGsmSubmitReport)
		{
		TBuf8<RMobilePhone::KMaxMobileTelNumberSize>  tmpBuf;
		
		tmpBuf.Copy(aText);
		tmpBuf.Append(_L8("  iSubmitReport PDU: "));

		LOGSMSIFPDU(tmpBuf, aAttrib.iSubmitReport, EFalse);
		}
	} // LogSmsIfSendAttributes

#endif // _SMS_LOGGING_ENABLED
