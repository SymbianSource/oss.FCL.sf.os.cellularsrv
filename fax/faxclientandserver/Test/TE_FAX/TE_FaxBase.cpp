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
// Telephony Fax Test base test code.
// 
//

/**
 @file
 @internalComponent
*/

#include "faxdefn.h"
#include "fax_reversebytes.h"
#include "FAXSET.H"
#include "CFAX32.H"

#include "TE_FaxBase.h"

 #include <commsdattypesv1_1.h>

 using namespace CommsDat;


//
// Constants...
//
const TInt KFaxTestLogPeriod = 5*1000000;
_LIT(KTEFaxINIFileName, "Z:\\TestData\\Configs\\TE_Fax.INI");
_LIT8(KTEFaxINIDestNumber, "DestPhoneNumber");


/**
 *  Default constructor.
 */	
CFaxTestStep::CFaxTestStep()
	{
	// NOP
	} // CFaxTestStep::CFaxTestStep


/**
 *  Default destructor.
 */	
CFaxTestStep::~CFaxTestStep()
	{
	// NOP
	} // CFaxTestStep::~CFaxTestStep


/**
 *  Pre-amble function called before all tests.
 *
 *  @return Test verdict for this test.
 */	
TVerdict CFaxTestStep::doTestStepPreambleL()
	{
	//
	// Mark for memory leaks!!!
	//
	__UHEAP_MARK;

	//
	// Create an Active Scheduler...
	//
	iScheduler = new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);

	return TestStepResult();
	} // CFaxTestStep::doTestStepPreambleL


/**
 *  Post-amble function called before all tests.
 *
 *  @return Test verdict for this test.
 */	
TVerdict CFaxTestStep::doTestStepPostambleL()
	{
	delete iScheduler;
	iScheduler = NULL;
	
	//
	// Check the heap for memory leaks...
	//
	__UHEAP_MARKEND;

	return TestStepResult();
	} // CFaxTestStep::doTestStepPostambleL


/**
 *  Initialise the Fax settings based on default values and CommDB settings.
 *
 *  @param aFaxSettings  Fax Settings stucture to initialise.
 */	
void CFaxTestStep::ReadDestPhoneNumberFromIniFileL(TDes8& aPhoneNumber)
	{
	//
	// Open the INI file...
	//
	RFs  fs;
	RFile  file;
	TInt  result;

	result = fs.Connect();
	TESTCHECKL(result, KErrNone);
	CleanupClosePushL(fs);

	result = file.Open(fs, KTEFaxINIFileName, EFileShareAny | EFileRead);
	if (result != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot load TE_Fax.ini file! (Error %d)"), result);
		TESTCHECKL(result, KErrNone);
		}
	CleanupClosePushL(file);

	//
	// Get the file size and and then load the file into memory...
	//
	TInt  fileSize;

	result = file.Size(fileSize);
	TESTCHECKL(result, KErrNone);
	TESTL(fileSize > 0);

	HBufC8*  fileContents = HBufC8::NewLC(fileSize);
	TPtr8  fileContentsPtr(fileContents->Des());

	result = file.Read(0, fileContentsPtr);
	TESTCHECKL(result, KErrNone);

	//
	// Parse the contents of the file...
	//
	TLex8  lexInput(fileContentsPtr);

	while (!lexInput.Eos())
		{
		TPtrC8  token(lexInput.NextToken());
		lexInput.SkipSpace();

		if (token.Find(KTEFaxINIDestNumber) == KErrNone) 
			{
			lexInput.SkipSpaceAndMark();

			TPtrC8  phoneNumberPtr(lexInput.NextToken());
			TBuf<32>  phoneNumber;

			phoneNumber.Copy(phoneNumberPtr);
			INFO_PRINTF2(_L("Phone number read from INI file as \"%S\"."), &phoneNumber);

			aPhoneNumber.Copy(phoneNumber);
			}
		}

	//
	// Done, clean up...
	//
	CleanupStack::PopAndDestroy(fileContents);
	CleanupStack::PopAndDestroy(&file);
	CleanupStack::PopAndDestroy(&fs);
	} // CFaxTestStep::ReadDestPhoneNumberFromIniFileL


/**
 *  Initialise the Fax settings based on default values and CommDB settings.
 *
 *  @param aFaxSettings  Fax Settings stucture to initialise.
 */	
void CFaxTestStep::ReadFaxCommDBSettingsL(TFaxSettings& aFaxSettings)
	{
	INFO_PRINTF1(_L("Reading Fax settings from the CommDB."));

	//
	// Initialise the default values...
	//
	aFaxSettings.iFaxId.Zero();
	aFaxSettings.iMaxSpeed             = 14400;
	aFaxSettings.iMinSpeed             = 2400;
	aFaxSettings.iPreferredResolution  = EFaxFine;
	aFaxSettings.iPreferredCompression = EModifiedHuffman;
	aFaxSettings.iPreferredECM         = EFalse;
	aFaxSettings.iFaxOnDemandDelay     = 20;
	aFaxSettings.iVerbose              = 1;

	//
	// Now we now read the Comm DB modem settings - we must have
	// these to run...
	//
#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_2);
#else
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
#endif
	CleanupStack::PushL(db);

	//
	// Read the currently selected connection preference and find preferred IAP...
	//
	TInt prefRank = 1;
	
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

	//
	// Get bearer number from IAP table, it is assumed bearer type will be a modem...
	//
	CMDBField<TUint32>* bearerField = new(ELeave) CMDBField<TUint32>(KCDTIdIAPBearer);
	CleanupStack::PushL(bearerField);
	bearerField->SetRecordId(iapId);
	bearerField->LoadL(*db);
	TUint32 modemId = *bearerField;
	CleanupStack::PopAndDestroy(bearerField);
	
	//
	// Get modem parameters...
	//
	CCDModemBearerRecord *modemRecord =
		static_cast<CCDModemBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
	CleanupStack::PushL(modemRecord);
	modemRecord->SetRecordId(modemId);
	modemRecord->LoadL(*db);
	iPortDriverName = modemRecord->iCsyName;
	iCommPortName = modemRecord->iPortName;
	iFaxInitString = modemRecord->iFaxInitString;
	iModemInitString = modemRecord->iModemInitString;
	
	TUint32 faxClass;
	faxClass = modemRecord->iFaxClassPref;
	aFaxSettings.iFaxClass = (TFaxClass) faxClass;

	CleanupStack::PopAndDestroy(3); // db, connectionPrefs, modemId
	
	} // CFaxTestStep::ReadFaxCommDBSettingsL


/**
 *  Begin Fax Transfer.  We only launch it once the elements of
 *  session->iFaxSessionParameters are all set up.  We call it with a
 *  TRequestStatus and while waiting for the fax to end, we also display
 *  the status of the fax every second.
 *
 *  @param aFaxSession   Fax Session already in use.
 */	
TInt CFaxTestStep::BeginFaxTransfer(CFaxTransfer* aFaxSession)
	{
	TRequestStatus  threadStatus;
	TRequestStatus  timerStatus;
	RTimer  timer;
	TFaxPhase  lastPhase = ENotYetStarted;
	TInt  result;

	result = aFaxSession->Start(threadStatus);
	if (result == KErrNone)
		{
		timer.CreateLocal ();
		timer.After(timerStatus, KFaxTestLogPeriod);

		for (;;)
			{
			User::WaitForRequest(timerStatus, threadStatus);
			if (timerStatus != KRequestPending)
				{
				if (aFaxSession->Progress())
					{
					INFO_PRINTF1(_L("Cancelling - no progress!"));
					aFaxSession->Cancel();
					}
				else
					{
					lastPhase = aFaxSession->iProgress.iPhase;
					if (lastPhase != ENotYetStarted)
						{
						INFO_PRINTF6(_L("Phase %c Speed %d Resolution %d Compression %dD ECM %d"),
									 aFaxSession->iProgress.iPhase,
									 aFaxSession->iProgress.iSpeed,
									 aFaxSession->iProgress.iResolution,
									 aFaxSession->iProgress.iCompression+1,
									 aFaxSession->iProgress.iECM);
						INFO_PRINTF4(_L("Line %d Page %d Connected to \"%S\""),
									 aFaxSession->iProgress.iLines,
									 aFaxSession->iProgress.iPage,
									 &aFaxSession->iProgress.iAnswerback);
						}
					timer.After(timerStatus, KFaxTestLogPeriod);
					}
				}
			else if (threadStatus != KRequestPending)
				{
				aFaxSession->Stop();

				timer.Cancel();
				User::WaitForRequest(timerStatus);
				timer.Close ();

				result = threadStatus.Int();
				break;
				}
			}
		}

	if (result == KFaxErrNoHangup)
		{
		//
		// KFaxErrNoHangup is a phase E error and can be safely ignored.
		//
		result = KErrNone;
		}

	return (result);
	} // CFaxTestStep::BeginFaxTransfer()


void CFaxTestStep::GenerateHeaderFileL(TFaxSettings& aFaxSettings, const TUint8* aFont,
									   TInt aWidth, TInt aHeight)
{
	TFaxHeaderInfo  faxHeader;

	//
	// Declare the font size...
	//
	faxHeader.iHeaderFontWidthInBytes  = aWidth;
	faxHeader.iHeaderFontHeightInLines = aHeight;

	//
	// Work out the number of characters we can get in the scan line, so that we can
	// create a modifiable buffer to hold the header line and fill it with spaces
	//
	TInt  charactersPerLine = KFaxBytesPerScanLine / faxHeader.iHeaderFontWidthInBytes;
	HBufC8*  textBuff = HBufC8::New(charactersPerLine);
	CleanupStack::PushL(textBuff);

	if (!textBuff)
		{
		User::Leave (KErrNoMemory);
		}

	TPtr8  textLine = textBuff->Des();
	textLine.Fill(' ', charactersPerLine);

	//
	// Work out the margin we leave on each side of the page in characters
	// this is specified as 74 pixels in T.4.
	//
	TInt  margin = (74 / (8 * faxHeader.iHeaderFontWidthInBytes));

	//
	// THE FOLLOWING LAYOUT IS PURELY FOR TEST PURPOSES AND SHOULDN'T
	// BE TAKEN TO BE A DEFINITIVE HEADER LAYOUT
	//
	//
	// assume we have the date and time on the lefthand side as dd/mm/yyyy hh:mm
	// 0000000001111111
	// 1234567890123456
	// fill in the separators
	//
	textLine.Replace(margin + 3, 1, _L8 ("/"));
	textLine.Replace(margin + 6, 1, _L8 ("/"));
	textLine.Replace(margin + 14, 1, _L8 (":"));

	//
	// Set up the offsets to the data...
	//
	faxHeader.iOffsetToDay    = margin + 1;
	faxHeader.iOffsetToMonth  = margin + 4;
	faxHeader.iOffsetToYear   = margin + 7;
	faxHeader.iOffsetToHour   = margin + 12;
	faxHeader.iOffsetToMinute = margin + 15;

	//
	// Assume we have the page info on the righthand side as pp/tt
	// 54321
	// fill in the separator
	//
	textLine.Replace(charactersPerLine - margin - 3, 1, _L8 ("/"));

	//
	// Set up the offsets to the data
	//
	faxHeader.iOffsetToTotalPages  = charactersPerLine - margin - 2;
	faxHeader.iOffsetToCurrentPage = charactersPerLine - margin - 5;

	//
	// Fill the header line with appropriate information
	//
	textLine.Replace(faxHeader.iOffsetToMinute + 4, aFaxSettings.iFaxId.Length (),
					 aFaxSettings.iFaxId);

	//
	// HEADER LAYOUT NOW FINISHED
	//
	// Now we can write the fax header info to the header info file
	//
	CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	CleanupStack::PushL(testheader);

	testheader->WriteFaxHeaderInfoL(faxHeader);

	//
	// Now we generate the scan line and write the header line bitmap
	// via a loop for each rowInFont in the font.
	//
	TInt rowInFont;

	for (rowInFont = 0;  rowInFont < faxHeader.iHeaderFontHeightInLines;  rowInFont++)
		{
		TRawScanLine  image;

		image.SetMax ();
		image.FillZ ();
		
		TInt offsetInImage = 0;

		for (TInt characterPosition = 0;  characterPosition < charactersPerLine;  characterPosition++)
			{
			for (TInt fontByte = 0;  fontByte < faxHeader.iHeaderFontWidthInBytes;  fontByte++)
				{
				image[offsetInImage++] = (TUint8) ~ KReverseByte[*(aFont + ((textLine[characterPosition] - 32) * faxHeader.iHeaderFontHeightInLines * faxHeader.iHeaderFontWidthInBytes) + (rowInFont * faxHeader.iHeaderFontWidthInBytes) + fontByte)];
				}
			}
		testheader->WriteRawHeaderLineL(rowInFont, image);
		}

	//
	// Now we replace the first ten characters in the header text line with digits
	// and write out the font line...
	//
	textLine.Replace(0, 10, _L8 ("0123456789"));	

	for (rowInFont = 0;  rowInFont < faxHeader.iHeaderFontHeightInLines;  rowInFont++)
		{
		TRawScanLine image;

		image.SetMax ();
		image.FillZ ();

		TInt offsetInImage = 0;

		for (TInt characterPosition = 0;  characterPosition < charactersPerLine;  characterPosition++)
			{
			for (TInt fontByte = 0;  fontByte < faxHeader.iHeaderFontWidthInBytes;  fontByte++)
				{
				image[offsetInImage++] = (TUint8) ~ KReverseByte[*(aFont + ((textLine[characterPosition] - 32) * faxHeader.iHeaderFontHeightInLines * faxHeader.iHeaderFontWidthInBytes) + (rowInFont * faxHeader.iHeaderFontWidthInBytes) + fontByte)];
				}
			}
		testheader->WriteRawFontLineL(rowInFont, image);
		}

	CleanupStack::PopAndDestroy(testheader);
	CleanupStack::PopAndDestroy(textBuff);
	} // CFaxTestStep::GenerateHeaderFileL

