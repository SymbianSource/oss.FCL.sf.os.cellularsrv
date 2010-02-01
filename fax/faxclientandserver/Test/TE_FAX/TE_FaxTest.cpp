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
// Telephony Fax Test main test code.
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
#include "etel.h"
#include "etelmm.h"
#include "faxhuff.h"

#include "TE_FaxBase.h"
#include "TE_FaxTest.h"


//
// Constants...
//
_LIT(KTEFaxTestStoreFileName, "C:\\TestStore.Fax");
_LIT(KTEFaxTestEncodeFileName, "C:\\TestEncode.Fax");
_LIT(KTEFaxTestHeaderFileName, "C:\\TestHeader.Fax");
_LIT(KTEFaxTSYName, "MM");
_LIT(KTEFaxPhoneName, "GsmPhone1");
_LIT(KTEFaxLineName, "Fax");


/**
 *  Each test step initialises it's own name
 */
CFaxTestFaxDefinition::CFaxTestFaxDefinition()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestFaxDefinition"));
	} // CFaxTestFaxDefinition::CFaxTestFaxDefinition


/**
 *  This tests the definition of a fax it's variables types.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestFaxDefinition::doTestStepL()
	{
#ifdef __EABI__
	//
	// Removed due to RVCT warnings!!!
	//
	INFO_PRINTF1(_L("Note: This test is not fully run on RVCT compiled platforms!"));
#else
	TFaxClass  faxClass = EClassAuto;
	faxClass = EClass1;

	TFaxPhase  faxPhase = ENotYetStarted;
	faxPhase = ECallEstablishment;

	TFaxResolution  faxResolution = EFaxNormal;
	faxResolution = EFaxFine;

	TFaxCompression  faxCompression = EModifiedHuffman;
	faxCompression = EModifiedRead;

	TInt  tmpValue;
	tmpValue = KFaxPixelsPerScanLine;
	tmpValue = KFaxBytesPerScanLine;
	tmpValue = KFaxCmPerScanLine;
	tmpValue = KFaxScanLinesPer10Cm;
	tmpValue = KFaxFineScanLinesPer10Cm;
	tmpValue = KFaxT4MaxDesLength;
	tmpValue = KFaxLinesPerBand;
#endif

	TFaxBufSenderId  faxBufSenderId;
	faxBufSenderId.Copy(_L8("Test"));

    TRawScanLine rawscanline;

	rawscanline.Zero();
	rawscanline.SetMax();
	rawscanline.FillZ();
	
	return TestStepResult();
	} // CFaxTestFaxDefinition::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestFaxStorage::CFaxTestFaxStorage()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestFaxStorage"));
	} // CFaxTestFaxStorage::CFaxTestFaxStorage


/**
 *  This tests the definition of a fax it's variables types.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestFaxStorage::doTestStepL()
	{
	INFO_PRINTF1(_L("Started fax store tests"));

    iWriteFaxFile = CWriteFaxFile::NewL();
	CleanupStack::PushL(iWriteFaxFile);
	iReadFaxFile = CReadFaxFile::NewL();
	CleanupStack::PushL(iReadFaxFile);

	iSenderId.Copy(_L("sender's number"));

	TBuf8<216>  scanlineBlack, scanlineWhite, scanlineAlt, scanlineCount;
	TInt  index;

	scanlineBlack.SetLength(216);
	scanlineBlack.Fill(TChar(0));
	scanlineWhite.SetLength(216);
	scanlineWhite.Fill(TChar(255));
	scanlineAlt.SetLength(216);
	scanlineAlt.Fill(TChar(255));
	
	for (index = 0;  index < 216; index += 2)
		{
		scanlineAlt[index] = 0;
		}
	scanlineCount.SetLength(216);

	for (index = 0;  index < 216;  index++)
		{
		scanlineCount[index] = TUint8(index);
		}

	TInt  numberLines, startnol(124), endnol(132);

	//
	// test 1
	//
	INFO_PRINTF3(_L("Test 1 - %d to %d repeated lines"), startnol, endnol);

	INFO_PRINTF1(_L("All black - Resolution Fine"));
	for (numberLines=startnol; numberLines<=endnol; numberLines++)
		{
		Test1L(numberLines, scanlineBlack, EFaxFine);
		}

	INFO_PRINTF1(_L("All white - Resolution Fine"));
	for (numberLines=startnol; numberLines<=endnol; numberLines++)
		{
		Test1L(numberLines, scanlineWhite, EFaxFine);
		}

	INFO_PRINTF1(_L("Black & White - Resolution Fine"));
	for (numberLines=startnol; numberLines<=endnol; numberLines++)
		{
		Test1L(numberLines, scanlineAlt, EFaxFine);
		}

	INFO_PRINTF1(_L("Counting - Resolution Fine"));
	for (numberLines=startnol; numberLines<=endnol; numberLines++)
		{
		Test1L(numberLines, scanlineCount, EFaxFine);
		}

	INFO_PRINTF1(_L("Black & White - Resolution Normal"));
	for (numberLines=startnol; numberLines<=endnol; numberLines++)
		{
		Test1L(numberLines, scanlineAlt, EFaxNormal);
		}

	INFO_PRINTF1(_L("Counting - Resolution Normal"));
	for (numberLines=startnol; numberLines<=endnol; numberLines++)
		{
		Test1L(numberLines, scanlineCount, EFaxNormal);
		}

	//
	// test 2
	//
	INFO_PRINTF3(_L("Test 2 - %d to %d alternated lines"), startnol, endnol);
	INFO_PRINTF1(_L("White and Black & White - Resolution Fine"));
	for (numberLines = startnol;  numberLines <= endnol;  numberLines++)
		{
		Test2L(numberLines, scanlineWhite, scanlineAlt, EFaxFine);
		}

	INFO_PRINTF1(_L("Black and Black & White - Resolution Fine"));
	for (numberLines = startnol;  numberLines <= endnol;  numberLines++)
		{
		Test2L(numberLines, scanlineBlack, scanlineAlt, EFaxFine);
		}

	INFO_PRINTF1(_L("Counting and Black & White - Resolution Fine"));
	for (numberLines = startnol;  numberLines <= endnol;  numberLines++)
		{
		Test2L(numberLines, scanlineCount, scanlineAlt, EFaxFine);
		}

	//
	// test 3
	//
	TInt  numberPages, maxNumberPages=5;

	INFO_PRINTF2(_L("Test 3 - 2 to %d pages"), maxNumberPages);
	for (numberPages = 2;  numberPages <= maxNumberPages;  numberPages++) 
		{
		INFO_PRINTF2(_L("%d pages"),numberPages);

		for (numberLines = startnol;  numberLines <= endnol;  numberLines++)
			{
			Test3L(numberLines, numberPages);
			}
		}

	//
	// test 4
	//
	INFO_PRINTF1(_L("Test 4 - seeking lines"));
	for (numberLines = startnol;  numberLines <= startnol;  numberLines++)
		{
		Test4L(numberLines);
		}

	//
	// test 5
	//
	TFaxBufSenderId  sd;

	INFO_PRINTF1(_L("Test 5 - writing and reading page info"));
	sd.Copy(_L8("+34345 4354"));
	Test5L(EFaxNormal,sd,EModifiedHuffman,TInt(2434));
	sd.Copy(_L8("+34345 4354"));
	Test5L(EFaxFine,sd,EModifiedHuffman,TInt(0));
	sd.Copy(_L8(""));
	Test5L(EFaxNormal,sd,EModifiedHuffman,TInt(1));
	sd.Copy(_L8(" + 4545 9"));
	Test5L(EFaxFine,sd,EModifiedHuffman,TInt(-1));

	//
	// Done!
	//
	CleanupStack::PopAndDestroy(iReadFaxFile);
	CleanupStack::PopAndDestroy(iWriteFaxFile);

	return TestStepResult();
	} // CFaxTestFaxStorage::doTestStepL


void CFaxTestFaxStorage::Test1L(TInt aNumberLines, TBuf8<216>& aScanLineWrite,
								TFaxResolution aRes)
	{
	TInt  row;
	TBuf8<216>  scanlineRead;

	iWriteFaxFile->OpenL(KTEFaxTestStoreFileName, 64);
	
	for (row = 0;  row < aNumberLines;  row++)
		{
		iWriteFaxFile->iWriteFaxPages->AddScanLineL(aScanLineWrite);
		}

	iWriteFaxFile->iWriteFaxPages->EndPageL(aRes, iSenderId);
	iWriteFaxFile->CommitL();
	iWriteFaxFile->Close();

	iReadFaxFile->OpenL(KTEFaxTestStoreFileName);

	for (row = 0;  row < aNumberLines;  row++)
		{
		iReadFaxFile->iReadFaxPages->GetScanLineL(scanlineRead);
		TESTCHECKSTR(scanlineRead, aScanLineWrite);
		}
	iReadFaxFile->Close();
	} // CFaxTestFaxStorage::Test1L


void CFaxTestFaxStorage::Test2L(TInt aNumberLines, TBuf8<216>& aScanLineWrite1,
								TBuf8<216>&  aScanLineWrite2, TFaxResolution aRes)
	{
	TInt  row;
	TBuf8<216>  scanlineRead;

	iWriteFaxFile->OpenL(KTEFaxTestStoreFileName, 64);
	for (row = 0; row < aNumberLines;  row++)
		{
		// line1
		iWriteFaxFile->iWriteFaxPages->AddScanLineL(aScanLineWrite1);
		row++;
		if (row >= aNumberLines)
			{
			break;
			}

		// line2
		iWriteFaxFile->iWriteFaxPages->AddScanLineL(aScanLineWrite2);
		}

	iWriteFaxFile->iWriteFaxPages->EndPageL(aRes, iSenderId);
	iWriteFaxFile->CommitL();
	iWriteFaxFile->Close();

	iReadFaxFile->OpenL(KTEFaxTestStoreFileName);
	for (row = 0;  row < aNumberLines;  row++)
		{
		// line1
		iReadFaxFile->iReadFaxPages->GetScanLineL(scanlineRead);
		TESTCHECKSTR(scanlineRead, aScanLineWrite1);
		row++;
		if (row >= aNumberLines)
			{
			break;
			}

		// line2
		iReadFaxFile->iReadFaxPages->GetScanLineL(scanlineRead);
		TESTCHECKSTR(scanlineRead, aScanLineWrite2);
		}
	iReadFaxFile->Close();
	} // CFaxTestFaxStorage::Test2L


void CFaxTestFaxStorage::Test3L(TInt aNumberLines, TInt aNumberPages)
	{
	TInt  row;
	TBuf8<216>  scanlineRead, scanlineWrite;

	scanlineWrite.SetLength(216);

	iWriteFaxFile->OpenL(KTEFaxTestStoreFileName, 64);
	TInt page;
	
	for (page = 0;  page < aNumberPages;  page++)
		{
		scanlineWrite.SetLength(216);
		scanlineWrite.Fill(TChar(255));
		
		for (TInt t=0; t<216; t+=2)
			{
			scanlineWrite[t] = 0;
			}
		
		scanlineWrite[0] = TUint8(page);
		
		for (row = 0;  row < aNumberLines;  row++)
			{
			iWriteFaxFile->iWriteFaxPages->AddScanLineL(scanlineWrite);
			}

		iWriteFaxFile->iWriteFaxPages->EndPageL(EFaxFine, iSenderId);
		iWriteFaxFile->CommitL();
		}
	iWriteFaxFile->Close();

	iReadFaxFile->OpenL(KTEFaxTestStoreFileName);

	for (page = 0;  page < aNumberPages;  page++)
		{
		iReadFaxFile->iReadFaxPages->SetPageL(page);

		for (row = 0;  row < aNumberLines;  row++)
			{
			iReadFaxFile->iReadFaxPages->GetScanLineL(scanlineRead);	
			TESTCHECK(scanlineRead[0], page);
			}
		}
	iReadFaxFile->Close();
	} // CFaxTestFaxStorage::Test3L


void CFaxTestFaxStorage::Test4L(TInt aNumberLines)
	{
	TInt  row;
	TBuf8<216>  scanlineRead, scanlineWrite;

	scanlineWrite.SetLength(216);

	iWriteFaxFile->OpenL(KTEFaxTestStoreFileName, 64);

	for (row = 0;  row < aNumberLines;  row++)
		{
		scanlineWrite[0] = TUint8(row);

		iWriteFaxFile->iWriteFaxPages->AddScanLineL(scanlineWrite);
		}

	iWriteFaxFile->iWriteFaxPages->EndPageL(EFaxFine, iSenderId);
	iWriteFaxFile->CommitL();
	iWriteFaxFile->Close();

	iReadFaxFile->OpenL(KTEFaxTestStoreFileName);

	for (row = 0;  row < aNumberLines;  row++)
		{
		iReadFaxFile->iReadFaxPages->SeekScanLineL(row);
		iReadFaxFile->iReadFaxPages->GetScanLineL(scanlineRead);
		TESTCHECK(scanlineRead[0], row);
		}

	for (row = aNumberLines - 1;  row >= 0;  row--)
		{
		iReadFaxFile->iReadFaxPages->SeekScanLineL(row);
		iReadFaxFile->iReadFaxPages->GetScanLineL(scanlineRead);
		TESTCHECK(scanlineRead[0], row);
		}

	iReadFaxFile->Close();
	} // CFaxTestFaxStorage::Test4L


void CFaxTestFaxStorage::Test5L(TFaxResolution aRes, TFaxBufSenderId& aSenderId,
								TFaxCompression aF1, TInt aF2)
{
	TInt  row;
	TBuf8<216>  scanLineWrite;

	scanLineWrite.SetLength(216);

	iWriteFaxFile->OpenL(KTEFaxTestStoreFileName, 64);

	for (row=0; row<100; row++)
		{
		iWriteFaxFile->iWriteFaxPages->AddScanLineL(scanLineWrite);
		}

	iWriteFaxFile->iWriteFaxPages->EndPageL(aRes, aSenderId, aF1, aF2);
	iWriteFaxFile->CommitL();
	iWriteFaxFile->Close();

	iReadFaxFile->OpenL(KTEFaxTestStoreFileName);

	TFaxPageInfo info = iReadFaxFile->iReadFaxPages->CurrentPageInfo();

	TESTCHECK(info.iNumScanLines, 100);
	TESTCHECK(info.iResolution, aRes);
	TESTCHECKSTR(info.iSenderId, aSenderId);
	TESTCHECK(info.iCompression, aF1);
	TESTCHECK(info.iReservedFlag2, aF2);

	iReadFaxFile->Close();
}


/**
 *  Each test step initialises it's own name
 */
CFaxTestFaxCoding::CFaxTestFaxCoding()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestFaxCoding"));
	} // CFaxTestFaxCoding::CFaxTestFaxCoding


/**
 *  This tests the fax coding.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestFaxCoding::doTestStepL()
	{
	TInt  result, pos;
	TInt  blackrun;
	TBool  flag(EFalse);
	RFile  enFile;
	RFs  fs;
	TRawScanLine  rawScanLine;
	TBuf8<KFaxT4MaxDesLength>  encodedScanLine;
	TBuf8<50>  leftOvers;
	TRawScanLine  decodedScanLine;
	TTime  then;
	TTime  now;
	TInt64  timetaken;

	CFaxT4* faxT4 = CFaxT4::NewL();
	CleanupStack::PushL(faxT4);

	result = fs.Connect();
	CleanupClosePushL(fs);
	TESTCHECKL(result, KErrNone);
	result = enFile.Replace(fs, KTEFaxTestEncodeFileName, EFileShareAny);
	CleanupClosePushL(enFile);
	TESTCHECKL(result, KErrNone);

	INFO_PRINTF1(_L("Testing 1D coding methods"));

	faxT4->PageInitialize(EFaxFine, EModifiedHuffman);
	rawScanLine.Fill(KFaxWhite, KFaxBytesPerScanLine);
	for (blackrun = 0;  blackrun <= 1728;  blackrun++)
		{
		Mem::Fill(&rawScanLine[0], blackrun/8, KFaxBlack);
		if (blackrun%8)
			{
			rawScanLine[(blackrun/8)] = (TUint8)(KFaxWhite << blackrun%8);
			}
		faxT4->EncodeScanLine(rawScanLine, encodedScanLine);
		encodedScanLine.Append(0);
		encodedScanLine.Append(1);
		enFile.Write(encodedScanLine);
		}
	
	enFile.Flush();
	CleanupStack::PopAndDestroy(&enFile);

	result = enFile.Open(fs, KTEFaxTestEncodeFileName, EFileShareAny);
	CleanupClosePushL(enFile);
	TESTCHECKL(result, KErrNone);

	// now encoded.fax contains the encoded RawScanLines from the jerry.fax file
	// and now we are going to decode them again

	encodedScanLine.Zero();

	then.UniversalTime();
    faxT4->PageInitialize(EFaxFine,EModifiedHuffman);
	rawScanLine.Fill(KFaxWhite, KFaxBytesPerScanLine);
	for (blackrun = 0;  blackrun <= 1728;  blackrun++)
		{
		pos = FindEndOfLine(encodedScanLine);
		if (pos == KErrNotFound)
			{
			flag = ETrue;
			if (encodedScanLine.Length() != 0)
				{
				leftOvers.Copy(encodedScanLine);
				enFile.Read(encodedScanLine, (encodedScanLine.MaxLength()-leftOvers.Length()));

				if (FindEndOfLine(encodedScanLine) == KErrNotFound)
				{
					User::Leave(KErrNotFound);
				}
			
				encodedScanLine.Insert(0, leftOvers);
				leftOvers.Zero();
				pos = FindEndOfLine(encodedScanLine);
				flag = EFalse;
				}
			
			if (flag)
				{
				enFile.Read(encodedScanLine);
				pos = FindEndOfLine(encodedScanLine);
				}
			}

		result = faxT4->DecodeScanLine(decodedScanLine, encodedScanLine.Left(pos));
		TESTCHECKL(result, KErrNone);
		
		Mem::Fill(&rawScanLine[0], blackrun/8, KFaxBlack);
		if (blackrun%8)
			{
			rawScanLine[(blackrun/8)] = (TUint8)(KFaxWhite << blackrun%8);
			}

		TEST(!rawScanLine.Compare(decodedScanLine));
		encodedScanLine.Delete(0,pos+2);	//2 is the length of the delimiter character.
		}
	now.UniversalTime();
	timetaken = now.MicroSecondsFrom(then).Int64();
	INFO_PRINTF2(_L("%dms to 1D encode all possible run lengths"), I64LOW(timetaken)/1000);

	enFile.Flush();
	CleanupStack::PopAndDestroy(&enFile);

	//
	// Again, for this test, we'll instantiate our own CFaxT4 object and generate
	// the fax file using the raw functions  - this is purely done for testing
	// and in real life we'd do a 2D fax via the StartPage() function 
	// as used in test8L
	//
	result = enFile.Replace(fs, KTEFaxTestEncodeFileName, EFileShareAny);
	CleanupClosePushL(enFile);
	TESTCHECKL(result, KErrNone);

	INFO_PRINTF1(_L("Test 2D coding methods"));

	//writeFaxFile->OpenL(_L("c:\\test.fax"),64);
	
	faxT4->PageInitialize(EFaxFine, EModifiedRead);
	rawScanLine.Fill(KFaxBlack, KFaxBytesPerScanLine);      // prepare a black reference line
	faxT4->EncodeScanLine(rawScanLine, encodedScanLine);
	
	//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);
	encodedScanLine.Append(0);
	encodedScanLine.Append(1);
	enFile.Write(encodedScanLine);

	rawScanLine[0] = KFaxWhite ;                             // start with a white run
	rawScanLine[KFaxBytesPerScanLine-1] = KFaxWhite ;        // end with a white run
	rawScanLine[(KFaxBytesPerScanLine/2)-1] = KFaxWhite ;    // with a white run halfway through
	rawScanLine[(KFaxBytesPerScanLine/2)-10] = KFaxWhite ;    // and a white run 10 before the end
	faxT4->EncodeScanLine(rawScanLine, encodedScanLine);      // horizontal mode encoding


	//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);
	encodedScanLine.Append(0);
	encodedScanLine.Append(1);
	enFile.Write(encodedScanLine);

	rawScanLine[0] = KFaxWhite;                             // first run identical for v0
	rawScanLine[(KFaxBytesPerScanLine/2)-1] = 0x1c ;         // 00011100 test v+3 v-2 
	rawScanLine[(KFaxBytesPerScanLine/2)-10] = 0x3e ;        // 00111110 test v+2 v-1 
	rawScanLine[KFaxBytesPerScanLine-1] = 0x78 ;             // 01111000 test v+1 v-3
	faxT4->EncodeScanLine(rawScanLine, encodedScanLine);      // vertical mode encoding
	encodedScanLine.Append(0);
	encodedScanLine.Append(1);
	enFile.Write(encodedScanLine);
	
	
	//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);
	rawScanLine[0] = KFaxBlack ;                             // for pass mode
	faxT4->EncodeScanLine(rawScanLine, encodedScanLine);      // pass mode encoding
	encodedScanLine.Append(0);
	encodedScanLine.Append(1);
	enFile.Write(encodedScanLine);
	//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);

	// same again in opposite colours

	faxT4->PageInitialize(EFaxFine, EModifiedRead);
	rawScanLine.Fill(KFaxWhite, KFaxBytesPerScanLine);      // prepare a white reference line
	faxT4->EncodeScanLine(rawScanLine, encodedScanLine);
	encodedScanLine.Append(0);
	encodedScanLine.Append(1);
	enFile.Write(encodedScanLine);
	//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);
	
	rawScanLine[0] = KFaxBlack ;                             // start with a black run
	rawScanLine[KFaxBytesPerScanLine-1] = KFaxBlack ;        // end with a black run
	rawScanLine[(KFaxBytesPerScanLine/2)-1] = KFaxBlack ;    // with a black run halfway through
	rawScanLine[(KFaxBytesPerScanLine/2)-10] = KFaxBlack ;    // and a blacke run 10 before the end
	faxT4->EncodeScanLine(rawScanLine, encodedScanLine);      // horizontal mode encoding
	encodedScanLine.Append(0);
	encodedScanLine.Append(1);
	enFile.Write(encodedScanLine);

	//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);
	
	rawScanLine[0] = KFaxBlack ;                             // first run identical for v0
	rawScanLine[(KFaxBytesPerScanLine/2)-1] = 0xe3 ;         // 11100011 test v+3 v-2 
	rawScanLine[(KFaxBytesPerScanLine/2)-10] = 0xc1 ;        // 11000001 test v+2 v-1 
	rawScanLine[KFaxBytesPerScanLine-1] = 0x87 ;             // 10000111 test v+1 v-3
	faxT4->EncodeScanLine(rawScanLine, encodedScanLine);      // vertical mode encoding
	encodedScanLine.Append(0);
	encodedScanLine.Append(1);
	enFile.Write(encodedScanLine);
	//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);
	
	rawScanLine[0] = KFaxWhite ;                             // for pass mode
	faxT4->EncodeScanLine(rawScanLine, encodedScanLine);      // pass mode encoding
	//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);
	encodedScanLine.Append(0);
	encodedScanLine.Append(1);
	enFile.Write(encodedScanLine);

	then.UniversalTime();
	faxT4->PageInitialize (EFaxFine, EModifiedRead);         // now start again
	rawScanLine.Fill (KFaxWhite, KFaxBytesPerScanLine);
	for (blackrun=0; blackrun <= 1728 ; blackrun++)
		{
		Mem::Fill(&rawScanLine[0], blackrun/8, KFaxBlack);
		if (blackrun%8)
			{
			rawScanLine[(blackrun/8)] = (TUint8)(KFaxWhite << blackrun%8);
			}
		faxT4->EncodeScanLine(rawScanLine, encodedScanLine);
		//writeFaxFile->iWriteFaxPages->AddEncodedScanLineL(encodedScanLine);
		encodedScanLine.Append(0);
		encodedScanLine.Append(1);
		enFile.Write(encodedScanLine);
		}
	now.UniversalTime();
	timetaken = now.MicroSecondsFrom (then).Int64 ();
	INFO_PRINTF2(_L("%dms to 2D encode all possible run lengths"), I64LOW(timetaken)/1000);

	enFile.Flush();
	CleanupStack::PopAndDestroy(&enFile);

	/*
	writeFaxFile->iWriteFaxPages->EndPageL(EFaxFine,senderId, EModifiedRead);
	writeFaxFile->CommitL();
	writeFaxFile->Close();
	*/

	//readFaxFile->OpenL(_L("c:\\test.fax"));
	//readFaxFile->iReadFaxPages->SetPageL(0);
	//TFaxPageInfo info = readFaxFile->iReadFaxPages->CurrentPageInfo();

	INFO_PRINTF1(_L("Test 2D decoding methods"));

	result = enFile.Open(fs, KTEFaxTestEncodeFileName, EFileShareAny);
	CleanupClosePushL(enFile);
	TESTCHECKL(result, KErrNone);

	rawScanLine.Fill(KFaxBlack, KFaxBytesPerScanLine);      // prepare a black line as the reference line
	//readFaxFile->iReadFaxPages->GetEncodedScanLineL(encodedScanLine);

	enFile.Read(encodedScanLine);
	pos = FindEndOfLine(encodedScanLine);

	result = faxT4->DecodeScanLine(decodedScanLine, encodedScanLine.Left(pos));
	//err = faxT4->DecodeScanLine(decodedScanLine, encodedScanLine);
	TESTCHECKL(result, KErrNone);
	result = rawScanLine.Compare(decodedScanLine);
	TESTCHECKL(result, KErrNone);
	encodedScanLine.Delete(0,pos+2);

	rawScanLine[0] = KFaxWhite ;
	rawScanLine[KFaxBytesPerScanLine-1] = KFaxWhite ;
	rawScanLine[(KFaxBytesPerScanLine/2)-1] = KFaxWhite ;    // test horizontal mode codec
	rawScanLine[(KFaxBytesPerScanLine/2)-10] = KFaxWhite ;   // and a white run 10 before the end
	
	//enFile.Read(encodedScanLine);
	pos = FindEndOfLine(encodedScanLine);

	//err = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
	//readFaxFile->iReadFaxPages->GetEncodedScanLineL(encodedScanLine);
	
	result = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
	TESTCHECKL(result, KErrNone);
	result = rawScanLine.Compare(decodedScanLine);
	TESTCHECKL(result, KErrNone);
	encodedScanLine.Delete(0,pos+2);

	rawScanLine[(KFaxBytesPerScanLine/2)-1] = 0x1c ;         // test v+3 v-2
	rawScanLine[(KFaxBytesPerScanLine/2)-10] = 0x3e ;        // test v+2 v-1 coded 
	rawScanLine[KFaxBytesPerScanLine-1] = 0x78 ;             // test v+1 v-3 codec

	//readFaxFile->iReadFaxPages->GetEncodedScanLineL(encodedScanLine);	
	pos=FindEndOfLine(encodedScanLine);
	result = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
	TESTCHECKL(result, KErrNone);
	result = rawScanLine.Compare(decodedScanLine);
	TESTCHECKL(result, KErrNone);
	encodedScanLine.Delete(0,pos+2);
	
	rawScanLine[0] = KFaxBlack ;                      // test pass and v0 codec
	//readFaxFile->iReadFaxPages->GetEncodedScanLineL(encodedScanLine);	
	pos=FindEndOfLine(encodedScanLine);
	result = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
	TESTCHECKL(result, KErrNone);
	result = rawScanLine.Compare(decodedScanLine);
	TESTCHECKL(result, KErrNone);
	encodedScanLine.Delete(0,pos+2);
	// same again in opposite colours

	rawScanLine.Fill (KFaxWhite, KFaxBytesPerScanLine);      // prepare a white line as the reference line
	pos=FindEndOfLine(encodedScanLine);
	//readFaxFile->iReadFaxPages->GetEncodedScanLineL(encodedScanLine);	
	result = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
	TESTCHECKL(result, KErrNone);
	rawScanLine.Compare(decodedScanLine);
	TESTCHECKL(result, KErrNone);
	encodedScanLine.Delete(0,pos+2);


	rawScanLine[0] = KFaxBlack ;
	rawScanLine[KFaxBytesPerScanLine-1] = KFaxBlack ;
	rawScanLine[(KFaxBytesPerScanLine/2)-1] = KFaxBlack ;    // test horizontal mode codec
	rawScanLine[(KFaxBytesPerScanLine/2)-10] = KFaxBlack ;   // and a black run 10 before the end
	//readFaxFile->iReadFaxPages->GetEncodedScanLineL(encodedScanLine);
	pos=FindEndOfLine(encodedScanLine);
	result = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
	TESTCHECKL(result, KErrNone);
	result = rawScanLine.Compare(decodedScanLine);
	TESTCHECKL(result, KErrNone);
	encodedScanLine.Delete(0,pos+2);
	
	rawScanLine[(KFaxBytesPerScanLine/2)-1] = 0xe3 ;         // 11100011 test v+3 v-2 
	rawScanLine[(KFaxBytesPerScanLine/2)-10] = 0xc1 ;        // 11000001 test v+2 v-1 
	rawScanLine[KFaxBytesPerScanLine-1] = 0x87 ;             // 10000111 test v+1 v-3
	//readFaxFile->iReadFaxPages->GetEncodedScanLineL(encodedScanLine);	

	pos=FindEndOfLine(encodedScanLine);
	result = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
	TESTCHECKL(result, KErrNone);
	result = rawScanLine.Compare(decodedScanLine);
	TESTCHECKL(result, KErrNone);
	encodedScanLine.Delete(0,pos+2);

	rawScanLine[0] = KFaxWhite ;                             // test pass and v0 codec
	//readFaxFile->iReadFaxPages->GetEncodedScanLineL(encodedScanLine);	
	pos=FindEndOfLine(encodedScanLine);
	result = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
	TESTCHECKL(result, KErrNone);
	result = rawScanLine.Compare(decodedScanLine);
	TESTCHECKL(result, KErrNone);
	encodedScanLine.Delete(0,pos+2);
	
	// now do all run lengths

	then.UniversalTime();
	//faxT4->PageInitialize (info.iResolution, info.iCompression);
//	enFile.Close();

//	err=enFile.Open(fs,_L("c:\\test.fax"),EFileShareAny);
//	if (err!=KErrNone)
//		User::Leave(err);

//	encodedScanLine.Zero();
	flag = EFalse;

    
	faxT4->PageInitialize(EFaxFine,EModifiedRead);
	rawScanLine.Fill(KFaxWhite, KFaxBytesPerScanLine);
	for (blackrun=0;  blackrun <= 1728;  blackrun++)
		{
		pos = FindEndOfLine(encodedScanLine);
		if (pos == KErrNotFound)
			{
			flag = ETrue;
			if (encodedScanLine.Length() != 0)
				{
				leftOvers.Copy(encodedScanLine);
				enFile.Read(encodedScanLine, (encodedScanLine.MaxLength()-leftOvers.Length()));
				if (FindEndOfLine(encodedScanLine) == KErrNotFound)
					{
					User::Leave(KErrNotFound);
					}
			
				encodedScanLine.Insert(0, leftOvers);
				leftOvers.Zero();
				pos = FindEndOfLine(encodedScanLine);
				flag = EFalse;
				}
			
			if (flag)
				{
				enFile.Read(encodedScanLine);
				pos = FindEndOfLine(encodedScanLine);
				}
			}

		result = faxT4->DecodeScanLine(decodedScanLine,encodedScanLine.Left(pos));
		TESTCHECKL(result, KErrNone);
		
		Mem::Fill(&rawScanLine[0], blackrun/8, KFaxBlack);
		if (blackrun%8)
			{
			rawScanLine[(blackrun/8)] = (TUint8)(KFaxWhite << blackrun%8);
			}
		result = rawScanLine.Compare(decodedScanLine);
		TESTCHECKL(result, KErrNone);
		encodedScanLine.Delete(0,pos+2);	//2 is the length of the delimiter character.
		}
	enFile.Flush();

	CleanupStack::PopAndDestroy(&enFile);
	CleanupStack::PopAndDestroy(&fs);
	CleanupStack::PopAndDestroy(faxT4);

	return TestStepResult();
	} // CFaxTestFaxCoding::doTestStepL


TInt CFaxTestFaxCoding::FindEndOfLine(TDes8& aEncodedDes)
	{
	TInt  maxlength;
	TUint  compareWord(0);

	maxlength = aEncodedDes.Length();
	if (aEncodedDes.Length() == 0)
		{
		return KErrNotFound;
		}

	TInt  index;

	for (index = 0;  index <= maxlength - 2;  index++)
		{
		compareWord = aEncodedDes[index];
		if (compareWord == 0)
			{
			compareWord = aEncodedDes[index + 1];
			if (compareWord == 1)
				{
				return index;
				}
			}
		}		
		
	return KErrNotFound;
	}

	
/**
 *  Each test step initialises it's own name
 */
CFaxTestFaxHeaderDefinition::CFaxTestFaxHeaderDefinition()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestFaxHeaderDefinition"));
	} // CFaxTestFaxHeaderDefinition::CFaxTestFaxHeaderDefinition


/**
 *  This tests the fax header definition...
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestFaxHeaderDefinition::doTestStepL()
	{
	//
    // this test is in two halves
	//
    // firstly we generate a header file - we don't do this via an Epoc print, but
    // directly using one of the test fonts in testfont.h.  However, the functions
    // we need are all exercised (writing font information and writing series of
    // TRawScanLines.  (Getting epoc printing to generate the correct data is not
    // part of this test.)  The test function here takes a pointer to the font
    // together with its dimensions as paramters, so that fonts 1 2 and 3 bytes
    // wide can be tested out all at te same time.  The fonts are in testfont.h
    //
    // secondly we use the header file we've just generate to create encoded scan lines
    // for a header, which we'll write to TEST.FAX as a fax file.  Naturally, in
    // real use, we'd send our encodes scan lines as part of the T.4 image at the start
    // of each pages, but generating a fax image (which is just a series of encoded scan
    // lines) is just as good a test.  So, we pass a pointer to the fax store as a parameters.
    // Again, we run this once for each font we test.
	//
	RFs  fs;
	TInt  result;
	RFile  file;

	result = fs.Connect();
	CleanupClosePushL(fs);
	TESTCHECKL(result, KErrNone);
	
	result = file.Replace(fs, KTEFaxTestHeaderFileName, EFileShareAny);
	CleanupClosePushL(file);
	TESTCHECKL(result, KErrNone);

	INFO_PRINTF1(_L("Generating header file using one byte wide font"));
    GenerateHeaderFileL(*font8x16, 1, 16);
	INFO_PRINTF1(_L("Writing fax header to TestHeader.Fax using current time and date")) ;
    GenerateFaxFileL(file);

	INFO_PRINTF1(_L("Generating header file using two bytes wide font"));
    GenerateHeaderFileL(*font16x16, 2, 16);
	INFO_PRINTF1(_L("Writing fax header to TestHeader.Fax using current time and date"));
	GenerateFaxFileL(file);

	INFO_PRINTF1(_L("Generating header file using three bytes wide font")) ;
    GenerateHeaderFileL(*font24x16, 3, 16);
	INFO_PRINTF1(_L("Writing fax header to TestHeader.Fax using current time and date")) ;
	GenerateFaxFileL(file);
	
	file.Flush();

	CleanupStack::PopAndDestroy(&file);
	CleanupStack::PopAndDestroy(&fs);

	return TestStepResult();
	} // CFaxTestFaxHeaderDefinition::doTestStepL


void CFaxTestFaxHeaderDefinition::GenerateFaxFileL(RFile& aFile)
   {
    // use the header to generate a *.FAX file
    TFaxHeaderInfo faxHeader;

	CFaxT4*  faxT4 = CFaxT4::NewL();
	CleanupStack::PushL(faxT4);
    CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	CleanupStack::PushL(testheader);

    testheader->ReadFaxHeaderInfoL(faxHeader);

    TRawScanLine  headline;
    TRawScanLine  fontline;
    TBuf8<KFaxT4MaxDesLength>  encodedHeadLine;

    TTime  timeOfTransmission;
    TBuf<12>  timeText;

	timeOfTransmission.HomeTime ();
    timeOfTransmission.FormatL(timeText, _L("%F%D%M%Y%H%T"));

	for (TInt index = 0;  index < 12;  index++)
		{
        timeText[index] -= '0';
		}

    // Forces 2 digit day - 2 digit month - 4 digit year - 2 digit hour - 2 digit minute

	for (TInt scanline = 0;  scanline < faxHeader.iHeaderFontHeightInLines;  scanline++)
		{
        testheader->ReadRawHeaderLineL(scanline, headline);
        testheader->ReadRawFontLineL(scanline, fontline);

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

            // put the page info in as 12/34

            headline[((faxHeader.iOffsetToCurrentPage) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(1 * faxHeader.iHeaderFontWidthInBytes) + fontByte];
            headline[((faxHeader.iOffsetToCurrentPage + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(2 * faxHeader.iHeaderFontWidthInBytes) + fontByte];
            headline[((faxHeader.iOffsetToTotalPages) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(3 * faxHeader.iHeaderFontWidthInBytes) + fontByte];
            headline[((faxHeader.iOffsetToTotalPages + 1) * faxHeader.iHeaderFontWidthInBytes) + fontByte] = fontline[(4 * faxHeader.iHeaderFontWidthInBytes) + fontByte];
			}

      faxT4->EncodeScanLine(headline, encodedHeadLine);
	  aFile.Write(encodedHeadLine);
      }

	CleanupStack::PopAndDestroy(testheader);
	CleanupStack::PopAndDestroy(faxT4);
	} // CFaxTestFaxHeaderDefinition::GenerateFaxFileL


void CFaxTestFaxHeaderDefinition::GenerateHeaderFileL(const TUint8* aFont,
													  TInt aWidth, TInt aHeight)
	{
    TFaxHeaderInfo  faxHeader;

    //
	// declare the font size
	//
    faxHeader.iHeaderFontWidthInBytes  = aWidth;
    faxHeader.iHeaderFontHeightInLines = aHeight;

    //
	// Work out the number of characters we can get in the scan line, so that we can
    // create a modifiable buffer to hold the header line and fill it with spaces
    //
	TInt  charactersPerLine = KFaxBytesPerScanLine / faxHeader.iHeaderFontWidthInBytes;
    HBufC8*  textBuff = HBufC8::NewL(charactersPerLine);
	CleanupStack::PushL(textBuff);

    TPtr8  textLine = textBuff->Des();
    textLine.Fill(0x20, charactersPerLine);

    //
	// Work out the margin we leave on each side of the page in characters
    // this is specified as 74 pixels in T.4
	//
	TInt  margin = (74 / (8 * faxHeader.iHeaderFontWidthInBytes));

	//
	// THE FOLLOWING LAYOUT IS PURELY FOR TEST PURPOSES AND SHOULDN'T
    // BE TAKEN TO BE A DEFINITIVE HEADER LAYOUT
	//
    // assume we have the date and time on the lefthand side as dd/mm/yyyy hh:mm
    // 0000000001111111
    // 1234567890123456
    // fill in the separators
	textLine.Replace(margin + 3, 1, _L8 ("/"));
    textLine.Replace(margin + 6, 1, _L8 ("/"));
    textLine.Replace(margin + 14, 1, _L8 (":"));

    //
	// set up the offsets to the data
	//
	faxHeader.iOffsetToDay = margin + 1;
    faxHeader.iOffsetToMonth = margin + 4;
    faxHeader.iOffsetToYear = margin + 7;
    faxHeader.iOffsetToHour = margin + 12;
    faxHeader.iOffsetToMinute = margin + 15;

    //
	// assume we have the page info on the righthand side as pp/tt
    // 54321
    // fill in the separator
	//
    textLine.Replace (charactersPerLine - margin - 3, 1, _L8 ("/"));

    //
	// set up the offsets to the data
	//
    faxHeader.iOffsetToTotalPages = charactersPerLine - margin - 2;
    faxHeader.iOffsetToCurrentPage = charactersPerLine - margin - 5;

    //
    // read in the fax settings, because we want the fax ID and the user ID
    //
    TFaxSettings  ourFaxSettings;
    TBuf8<20>  userName;
   
    ourFaxSettings.iFaxId.Copy(_L("+++01234 56789+++"));
    userName.Copy(_L("Symbian Test Header"));

    TInt  lenID = ourFaxSettings.iFaxId.Length ();
    TInt  lenUser = userName.Length ();

    //
    // Fill the header line with appropriate information
	//
    textLine.Replace (faxHeader.iOffsetToMinute + 4, lenID, ourFaxSettings.iFaxId);
    textLine.Replace (faxHeader.iOffsetToCurrentPage - lenUser -2, lenUser, userName);

    //
	// HEADER LAYOUT NOW FINISHED
	//
    // now we can write the fax header info to the header info file
    //
    CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	CleanupStack::PushL(testheader);
    testheader->WriteFaxHeaderInfoL(faxHeader);

	//
    // Now we generate the scan line and write the header line bitmap
    // via a loop for each rowInFont in the font
	//
    TInt rowInFont;

	for (rowInFont = 0;  rowInFont < faxHeader.iHeaderFontHeightInLines;  rowInFont++)
		{
		TRawScanLine  image;
        image.SetMax();
        image.FillZ();
        TInt  offsetInImage = 0;
 
        offsetInImage = 0;
        for (TInt characterPosition = 0;  characterPosition < charactersPerLine;  characterPosition++)
			{
            for (TInt fontByte = 0;  fontByte < faxHeader.iHeaderFontWidthInBytes;  fontByte++)
				{
				image[offsetInImage++] = (TUint8)~KReverseByte[*(aFont + ((textLine[characterPosition] - 32) * faxHeader.iHeaderFontHeightInLines * faxHeader.iHeaderFontWidthInBytes) + (rowInFont * faxHeader.iHeaderFontWidthInBytes) + fontByte)];
				}
			}
        testheader->WriteRawHeaderLineL (rowInFont, image);
		}

	//
    // now we replace the first ten characters in the header text line with digits
	//
    textLine.Replace(0, 10, _L8("0123456789"));

	//
    // and write out the font line
	//
	for (rowInFont = 0; rowInFont < faxHeader.iHeaderFontHeightInLines; rowInFont++)
		{
		TRawScanLine  image;
		image.SetMax();
		image.FillZ();
		TInt  offsetInImage = 0;

		offsetInImage = 0;
		for (TInt characterPosition = 0;  characterPosition < charactersPerLine;  characterPosition++)
			{
			for (TInt fontByte = 0;  fontByte < faxHeader.iHeaderFontWidthInBytes;  fontByte++)
				{
				image[offsetInImage++] = (TUint8)~KReverseByte[*(aFont + ((textLine[characterPosition] - 32) * faxHeader.iHeaderFontHeightInLines * faxHeader.iHeaderFontWidthInBytes) + (rowInFont * faxHeader.iHeaderFontWidthInBytes) + fontByte)];
				}
			}
		testheader->WriteRawFontLineL (rowInFont, image);
		// we can add the test code to generate the panic here
		}

	CleanupStack::PopAndDestroy(testheader);
	CleanupStack::PopAndDestroy(textBuff);
	} // CFaxTestFaxHeaderDefinition::GenerateHeaderFileL


/**
 *  Each test step initialises it's own name
 */
CFaxTestTransmitFax01::CFaxTestTransmitFax01()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestTransmitFax01"));
	} // CFaxTestTransmitFax01::CFaxTestTransmitFax01


/**
 *  This tests the transmission of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestTransmitFax01::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass             = EClassAuto;
	faxSettings.iPreferredECM         = 0;
	faxSettings.iMaxSpeed             = 14400;
	faxSettings.iMinSpeed             = 2400;
	faxSettings.iPreferredResolution  = EFaxNormal;
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);

	faxSession->SetMode(EDialAndTransmit);

	//
	// Generate a Fax header for transmitting...
	//
	TFaxHeaderInfo  faxHeader;

	faxHeader.iHeaderFontWidthInBytes = 0;
	
	CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	TRAP_IGNORE(testheader->ReadFaxHeaderInfoL(faxHeader));
	delete testheader;

	GenerateHeaderFileL(faxSettings, *font8x16, 1, 16);

	//
	// Add the source file to the session...
	//
	faxSession->AddSourceL(_L("C:\\TEST.FAX"), Prefer1D);

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}
	
	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax transmission of TEST.FAX a simple test fax..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax transmission completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestTransmitFax01::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestTransmitFax02::CFaxTestTransmitFax02()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestTransmitFax02"));
	} // CFaxTestTransmitFax02::CFaxTestTransmitFax02


/**
 *  This tests the transmission of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestTransmitFax02::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass             = EClassAuto;
	faxSettings.iPreferredECM         = 0;
	faxSettings.iMaxSpeed             = 9600;
	faxSettings.iMinSpeed             = 2400;
	faxSettings.iPreferredResolution  = EFaxFine;
	faxSettings.iPreferredCompression = EModifiedRead;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);

	faxSession->SetMode(EDialAndTransmit); // Or EImmediateTransmit

	//
	// Generate a Fax header for transmitting...
	//
	TFaxHeaderInfo  faxHeader;

	faxHeader.iHeaderFontWidthInBytes = 0;
	
	CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	TRAP_IGNORE(testheader->ReadFaxHeaderInfoL(faxHeader));
	delete testheader;

	GenerateHeaderFileL(faxSettings, *font16x16, 2, 16);

	//
	// Add the source file to the session. "CHART4.FAX" is an ITU test chart.
	//
	faxSession->AddSourceL(_L("C:\\CHART4.FAX"), Prefer2D);

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}
	
	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax transmission of CHART4.FAX an ITU test chart..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax transmission completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestTransmitFax02::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestTransmitFax03::CFaxTestTransmitFax03()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestTransmitFax03"));
	} // CFaxTestTransmitFax03::CFaxTestTransmitFax03


/**
 *  This tests the transmission of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestTransmitFax03::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass             = EClass2;
	faxSettings.iMaxSpeed             = 9600;
	faxSettings.iMinSpeed             = 14400;
	faxSettings.iPreferredResolution  = EFaxNormal;
	faxSettings.iPreferredCompression = EModifiedRead;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);

	faxSession->SetMode(EDialAndTransmit); // Or EImmediateTransmit

	//
	// Generate a Fax header for transmitting...
	//
	TFaxHeaderInfo  faxHeader;

	faxHeader.iHeaderFontWidthInBytes = 0;
	
	CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	TRAP_IGNORE(testheader->ReadFaxHeaderInfoL(faxHeader));
	delete testheader;

	GenerateHeaderFileL(faxSettings, *font24x16, 3, 16);

	//
	// Add the source file to the session. The "BLANK.FAX" all-white short run
	// length file.
	//
	faxSession->AddSourceL(_L("C:\\BLANK.FAX"), Prefer2D);

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}
	
	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax transmission of BLANK.FAX an all-white short run length file..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax transmission completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestTransmitFax03::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestTransmitFax04::CFaxTestTransmitFax04()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestTransmitFax04"));
	} // CFaxTestTransmitFax04::CFaxTestTransmitFax04


/**
 *  This tests the transmission of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestTransmitFax04::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass             = EClass2point0;
	faxSettings.iPreferredECM         = 1;
	faxSettings.iMaxSpeed             = 2400;
	faxSettings.iMinSpeed             = 2400;
	faxSettings.iPreferredResolution  = EFaxNormal;
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);

	faxSession->SetMode(EDialAndTransmit); // Or EImmediateTransmit

	//
	// Generate a Fax header for transmitting...
	//
	TFaxHeaderInfo  faxHeader;

	faxHeader.iHeaderFontWidthInBytes = 0;
	
	CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	TRAP_IGNORE(testheader->ReadFaxHeaderInfoL(faxHeader));
	delete testheader;

	GenerateHeaderFileL(faxSettings, *font8x16, 1, 16);

	//
	// Add the source file to the session...
	//
	faxSession->AddSourceL(_L("C:\\TWO.FAX"), Prefer2D);

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}

	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax transmission of TWO.FAX a two page fax..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax transmission completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestTransmitFax04::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestTransmitFax05::CFaxTestTransmitFax05()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestTransmitFax05"));
	} // CFaxTestTransmitFax05::CFaxTestTransmitFax05


/**
 *  This tests the transmission of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestTransmitFax05::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass             = EClassAuto;
	faxSettings.iPreferredECM         = 1;
	faxSettings.iMaxSpeed             = 14400;
	faxSettings.iMinSpeed             = 2400;
	faxSettings.iPreferredResolution  = EFaxNormal;
	faxSettings.iPreferredCompression = EModifiedRead;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);

	faxSession->SetMode(EDialAndTransmit); // Or EImmediateTransmit

	//
	// Generate a Fax header for transmitting...
	//
	TFaxHeaderInfo  faxHeader;

	faxHeader.iHeaderFontWidthInBytes = 0;
	
	CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	TRAP_IGNORE(testheader->ReadFaxHeaderInfoL(faxHeader));
	delete testheader;

	GenerateHeaderFileL(faxSettings, *font24x16, 3, 16);

	//
	// Add the source files to the session. This will test out the multi page
	// sends and also the overall sending of large complex fax images
	// by sending whole of CHART4.FAX, pages 3 to 5 from SIX.FAX, followed by
	// TWO.FAX starting from page 2 and followed by page 1 of BLANK.FAX.
	//
	faxSession->AddSourceL(_L("C:\\chart4.fax"), Prefer2D);
	faxSession->AddSourceL(_L("C:\\SIX.FAX"), 3, 5);
	faxSession->AddSourceL(_L("C:\\TWO.FAX"), 2);
	faxSession->AddSourceL(_L("C:\\BLANK.FAX"), 1, 1);

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}
	
	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax transmission of a large complex fax set..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax transmission completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestTransmitFax05::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestTransmitFax06::CFaxTestTransmitFax06()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestTransmitFax06"));
	} // CFaxTestTransmitFax06::CFaxTestTransmitFax06


/**
 *  This tests the transmission of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestTransmitFax06::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass             = EClassAuto;
	faxSettings.iPreferredECM         = 0;
	faxSettings.iMaxSpeed             = 14400;
	faxSettings.iMinSpeed             = 2400;
	faxSettings.iPreferredResolution  = EFaxFine;
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);

	faxSession->SetMode(EDialAndTransmit); // Or EImmediateTransmit

	//
	// Generate a Fax header for transmitting...
	//
	TFaxHeaderInfo  faxHeader;

	faxHeader.iHeaderFontWidthInBytes = 0;
	
	CFaxHeaderLines*  testheader = CFaxHeaderLines::NewL();
	TRAP_IGNORE(testheader->ReadFaxHeaderInfoL(faxHeader));
	delete testheader;

	GenerateHeaderFileL(faxSettings, *font24x16, 3, 16);

	//
	// Add the source files to the session. This tests a bad page fax.
	//
	faxSession->AddSourceL(_L("C:\\BADPAGE.FAX"), Prefer1D);

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}
	
	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax transmission of a bad page fax..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax transmission completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestTransmitFax06::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestReceiveFax01::CFaxTestReceiveFax01()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestReceiveFax01"));
	} // CFaxTestReceiveFax01::CFaxTestReceiveFax01


/**
 *  This tests the reception of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestReceiveFax01::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass = EClassAuto;  // Or EClass1, EClass2, EClass2point0
	if (faxSettings.iFaxClass == EClass2point0  ||
		faxSettings.iFaxClass == EClassAuto)
		{
		faxSettings.iPreferredECM = 0; // Or 1
		}

	faxSettings.iMaxSpeed = 14400; // 2400 to 14400 % 2400
	faxSettings.iMinSpeed = 2400; // 2400 to 14400 % 2400
	faxSettings.iPreferredResolution = EFaxNormal; // Or EFaxFine
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);

	faxSession->SetMode(EWaitForRingAndReceive); // Or EDialAndReceiveFaxBack, EDialAndReceivePoll, EImmediateReceive
	faxSession->SetReceiveFileName(_L("C:\\RECEIVE01.FAX"));

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}

	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax reception..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax reception completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestReceiveFax01::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestReceiveFax02::CFaxTestReceiveFax02()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestTransmitFax02"));
	} // CFaxTestReceiveFax02::CFaxTestReceiveFax02


/**
 *  This tests the reception of a fax, specifying the call object name first.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestReceiveFax02::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass = EClassAuto;  // Or EClass1, EClass2, EClass2point0
	if (faxSettings.iFaxClass == EClass2point0  ||
		faxSettings.iFaxClass == EClassAuto)
		{
		faxSettings.iPreferredECM = 0; // Or 1
		}

	faxSettings.iMaxSpeed = 14400; // 2400 to 14400 % 2400
	faxSettings.iMinSpeed = 2400; // 2400 to 14400 % 2400
	faxSettings.iPreferredResolution = EFaxNormal; // Or EFaxFine
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);


	faxSession->SetMode(EWaitForRingAndReceive); // Or EDialAndReceiveFaxBack, EDialAndReceivePoll, EImmediateReceive
	faxSession->SetReceiveFileName(_L("C:\\RECEIVE02.FAX"));

	//
	// For this test, find the name of the incoming call and request the fax
	// session to open that call...
	//
	RTelServer  telServer;
	RMobilePhone  phone;
	RLine  line;
	RCall  call;
	TName  incomingCallName;
	RCall::TStatus  callStatus;

	TInt  result = telServer.Connect();
	TESTCHECKL(result, KErrNone);
	CleanupClosePushL(telServer);

	result = telServer.LoadPhoneModule(KTEFaxTSYName);
	TESTCHECKL(result, KErrNone);

	result = phone.Open(telServer, KTEFaxPhoneName);
	TESTCHECKL(result, KErrNone);
	CleanupClosePushL(phone);

	result = line.Open(phone, KTEFaxLineName);
	TESTCHECKL(result, KErrNone);
	CleanupClosePushL(line);

	TRequestStatus  status;

	line.NotifyIncomingCall(status, incomingCallName);
	User::WaitForRequest(status);
	TESTCHECKL(status.Int(), KErrNone);
	INFO_PRINTF2(_L("Incoming fax call is \"%S\"..."), &incomingCallName);

	faxSession->SetCallObjectName(incomingCallName);

	//
	// Open the incoming call and request a notification of state change. If
	// the Fax Server were to open a different RCall object then this would
	// not work - thus it is a good test of SetCallObjectName()...
	//
	result = call.OpenExistingCall(line, incomingCallName);
	TESTCHECKL(result, KErrNone);
	CleanupClosePushL(call);

	callStatus = RCall::EStatusUnknown;
	call.NotifyStatusChange(status, callStatus);
	TESTCHECK(status.Int(), KRequestPending);

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}
	
	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax reception..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax reception completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Check that the notify operation returned as the fax server answered the
	// call...
	//
	User::WaitForRequest(status);
	TESTCHECK(status.Int(), KErrNone);
	TESTCHECK(callStatus, RCall::EStatusAnswering);

	//
	// Clean up Etel handles...
	//
	CleanupStack::PopAndDestroy(&call);
	CleanupStack::PopAndDestroy(&line);
	CleanupStack::PopAndDestroy(&phone);
	CleanupStack::PopAndDestroy(&telServer);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestReceiveFax02::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestReceiveFax03::CFaxTestReceiveFax03()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestReceiveFax03"));
	} // CFaxTestReceiveFax03::CFaxTestReceiveFax03


/**
 *  This tests the reception of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestReceiveFax03::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass = EClassAuto;  // Or EClass1, EClass2, EClass2point0
	if (faxSettings.iFaxClass == EClass2point0  ||
		faxSettings.iFaxClass == EClassAuto)
		{
		faxSettings.iPreferredECM = 0; // Or 1
		}

	faxSettings.iMaxSpeed = 14400; // 2400 to 14400 % 2400
	faxSettings.iMinSpeed = 2400; // 2400 to 14400 % 2400
	faxSettings.iPreferredResolution = EFaxNormal; // Or EFaxFine
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);

	faxSession->SetMode(EWaitForRingAndReceive); // Or EDialAndReceiveFaxBack, EDialAndReceivePoll, EImmediateReceive
	faxSession->SetReceiveFileName(_L("C:\\RECEIVE03.FAX"));

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}
	
	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax reception..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax reception completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestReceiveFax03::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestReceiveFax04::CFaxTestReceiveFax04()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestReceiveFax04"));
	} // CFaxTestReceiveFax04::CFaxTestReceiveFax04


/**
 *  This tests the reception of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestReceiveFax04::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass = EClassAuto;  // Or EClass1, EClass2, EClass2point0
	if (faxSettings.iFaxClass == EClass2point0  ||
		faxSettings.iFaxClass == EClassAuto)
		{
		faxSettings.iPreferredECM = 0; // Or 1
		}

	faxSettings.iMaxSpeed = 14400; // 2400 to 14400 % 2400
	faxSettings.iMinSpeed = 2400; // 2400 to 14400 % 2400
	faxSettings.iPreferredResolution = EFaxNormal; // Or EFaxFine
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);


	faxSession->SetMode(EWaitForRingAndReceive); // Or EDialAndReceiveFaxBack, EDialAndReceivePoll, EImmediateReceive
	faxSession->SetReceiveFileName(_L("C:\\RECEIVE04.FAX"));

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}
	
	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax reception..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax reception completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestReceiveFax04::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestReceiveFax05::CFaxTestReceiveFax05()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestReceiveFax05"));
	} // CFaxTestReceiveFax05::CFaxTestReceiveFax05


/**
 *  This tests the reception of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestReceiveFax05::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass = EClassAuto;  // Or EClass1, EClass2, EClass2point0
	if (faxSettings.iFaxClass == EClass2point0  ||
		faxSettings.iFaxClass == EClassAuto)
		{
		faxSettings.iPreferredECM = 0; // Or 1
		}

	faxSettings.iMaxSpeed = 14400; // 2400 to 14400 % 2400
	faxSettings.iMinSpeed = 2400; // 2400 to 14400 % 2400
	faxSettings.iPreferredResolution = EFaxNormal; // Or EFaxFine
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);


	faxSession->SetMode(EWaitForRingAndReceive); // Or EDialAndReceiveFaxBack, EDialAndReceivePoll, EImmediateReceive
	faxSession->SetReceiveFileName(_L("C:\\RECEIVE05.FAX"));

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}

	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax reception..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax reception completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestReceiveFax05::doTestStepL


/**
 *  Each test step initialises it's own name
 */
CFaxTestReceiveFax06::CFaxTestReceiveFax06()
	{
	//
	// Store the name of this test case.  This is the name that is used by
	// the script file.
	//
	SetTestStepName(_L("TestReceiveFax06"));
	} // CFaxTestReceiveFax06::CFaxTestReceiveFax06


/**
 *  This tests the reception of a straight forward test fax.
 *
 *  @return Test verdict for this test.
 */	
enum TVerdict CFaxTestReceiveFax06::doTestStepL()
	{
	TFaxSettings  faxSettings;

	//
	// Before instantiating CFaxTransfer we have to have a valid set of
	// TFaxSettings to pass in to it - so our first step must always be
	// to read the fax settings or set up suitable defaults.
	//
	TRAPD(leaveCode, ReadFaxCommDBSettingsL(faxSettings));
	if (leaveCode != KErrNone)
		{
		INFO_PRINTF2(_L("ReadFaxCommDBSettingsL() error %d!"), leaveCode);
		}
	TESTCHECKL(leaveCode, KErrNone);

	//
	// Set the Fax parameters...
	//
	faxSettings.iFaxClass = EClassAuto;  // Or EClass1, EClass2, EClass2point0
	if (faxSettings.iFaxClass == EClass2point0  ||
		faxSettings.iFaxClass == EClassAuto)
		{
		faxSettings.iPreferredECM = 0; // Or 1
		}

	faxSettings.iMaxSpeed = 14400; // 2400 to 14400 % 2400
	faxSettings.iMinSpeed = 2400; // 2400 to 14400 % 2400
	faxSettings.iPreferredResolution = EFaxNormal; // Or EFaxFine
	faxSettings.iPreferredCompression = EModifiedHuffman;
	faxSettings.iFaxId.Copy(_L8("+441632960000"));

	//
	// Start the Fax session...
	//
	CFaxTransfer*  faxSession = NULL;

	TRAPD(startSessionResult, faxSession = CFaxTransfer::NewL(faxSettings));
	TESTCHECKL(startSessionResult, KErrNone);
	TESTL(faxSession != NULL);


	faxSession->SetMode(EWaitForRingAndReceive); // Or EDialAndReceiveFaxBack, EDialAndReceivePoll, EImmediateReceive
	faxSession->SetReceiveFileName(_L("C:\\RECEIVE06.FAX"));

	//
	// Set the destination phone number if needed...
	//
	if ((faxSession->iMode & KFaxNoDial) == 0)
		{
		TBuf8<32>  phoneNumber;

		ReadDestPhoneNumberFromIniFileL(phoneNumber);
		faxSession->SetPhoneNumberL(phoneNumber);
		}

	//
	// Begin transfering the fax...
	//
	INFO_PRINTF1(_L("Starting fax reception..."));
	TInt  transferResult = BeginFaxTransfer(faxSession);
	INFO_PRINTF2(_L("Fax reception completed with return code %d"), transferResult);
	TESTCHECK(transferResult, KErrNone);

	//
	// Clean up the Fax session...
	//
	faxSession->RemoveAllSources();
	delete faxSession;
	faxSession = NULL;

	return TestStepResult();
	} // CFaxTestReceiveFax06::doTestStepL

