// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @publishedPartner
 @released
*/

#if defined(__VC32__) && _MSC_VER==1100
// Disable MSVC++ 5.0 aggressive warnings about non-expansion of inline functions. 
#pragma warning(disable : 4710)	// function '...' not expanded
#endif

#if !defined(__CFAXIO_H__)
#define __CFAXIO_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif // __E32STD_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif // __E32BASE_H__

#if !defined(__F32FILE_H__)
#include <f32file.h>
#endif // __F32FILE_H__

#if !defined(__FAXDEFN_H__)
#include <faxdefn.h>
#endif // __FAXDEFN_H__

/**
@internalComponent
*/
typedef TUint32 TCodeDef;

/**
@publishedPartner
@released
*/
const TInt KFaxFileStoreUidVal(268435712);

class CFileWriteStream;
class CFileReadStream;


/**
Fax line coding/decoding.

Provides utility functions for encoding and decoding fax scan lines. The lines 
can be encoded/decoded as 1 dimensional modified Huffman or 2 dimensional 
modified Read.

Users must first create a CFaxT4 object using NewL() or NewLC(). Specific 
functions are provided to encode/decode scan lines using the two coding schemes. 
In addition, general functions are provided which determine the coding type 
from the values specified when the object is initialised - using PageInitialise().

In pre-v5.1 releases of Symbian OS, this class was defined in faxstore.h and 
its import library was faxst2.lib.

@publishedPartner
@released 
*/
class CFaxT4 : public CBase // replaces FaxT4 class used up to faxstore 013
	{
public:
	IMPORT_C static CFaxT4 * NewL ();
	IMPORT_C static CFaxT4 * NewLC ();
	IMPORT_C void EncodeScanLine(const TDesC8& aScanLine,TDes8& anEncodedScanLine);
	IMPORT_C TInt DecodeScanLine(TDes8& aScanLine,const TDesC8& anEncodedScanLine);
	IMPORT_C void EncodeScanLine1D(const TDesC8& aScanLine,TDes8& anEncodedScanLine);
	IMPORT_C TInt DecodeScanLine1D(TDes8& aScanLine,const TDesC8& anEncodedScanLine);
	IMPORT_C void EncodeScanLine2D(const TDesC8& aScanLine,TDes8& anEncodedScanLine);
	IMPORT_C TInt DecodeScanLine2D(TDes8& aScanLine,const TDesC8& anEncodedScanLine);
	IMPORT_C void PageInitialize (TFaxResolution aResolution, TFaxCompression aCompression, TInt aFlag2 = 0);

private:
	inline CFaxT4();

	void DoEncodeScanLine2D (const TDesC8 & aScanLine, TDes8 & anEncodedScanLine);
	void DecodeHuffman(const TDesC8 & aEncodedScanLine);

private:
	TFaxResolution iResolution;
	TFaxCompression iCompression;
	TInt iLineCount;
	TInt iK;
	TInt iReservedFlag2;
	const TUint8* iEndRef;
	TUint8 iRef[KFaxPixelsPerScanLine+4];
	};

/**
Fax header line information.

Contains the information needed to generate a fax header line from a font 
bitmap line and a header line template. For a detailed discussion of how this 
class interacts with others to generate the fax header line.

In pre-v5.1 releases of Symbian OS, this class was defined in faxstore.h.

The iOffset members specify an offset in a TRawScanLine. In other words, the 
offsets are specified in bytes rather than in characters or bits. 

@publishedPartner
@released
*/
class TFaxHeaderInfo
	{
public:
	/** Width of the font in bytes. */
	TInt iHeaderFontWidthInBytes;	
	/** Height of the font in lines. */
	TInt iHeaderFontHeightInLines;
	/** Offset to two digit day of month. */
	TInt iOffsetToDay;           
	/** Offset to two digits month of year. */
	TInt iOffsetToMonth;         
	/** Offset to four digits year. */
	TInt iOffsetToYear;         
	/** Offset to two digits hour (24 hour clock). */
	TInt iOffsetToHour;       
	/** Offset to two digits minute. */
	TInt iOffsetToMinute;        
	/** Offset to two digits for total pages. */
	TInt iOffsetToTotalPages;   
	/** Offset to two digits for current page. */
	TInt iOffsetToCurrentPage;   
	};

/**
Packages fax header information for transferring across the client-server boundary. 

@internalComponent
*/
typedef TPckgBuf < TFaxHeaderInfo > TFaxHeaderInfoPckg;


/**
Read/write fax header line data

Allows applications to read and write information from the fax header line 
data file: including the header line template, a font bitmap, and character 
offset information. This data can be used to generate a fax header line -
which contains send-time information - in real time. 

This class is not intended for user derivation.

In pre-v5.1 releases of Symbian OS, this class was defined in faxstore.h and 
had an import library faxst2.lib. 

@publishedPartner
@released
*/
class CFaxHeaderLines : public CBase
	{
public:
	IMPORT_C static CFaxHeaderLines * NewL ();
	IMPORT_C static CFaxHeaderLines * NewLC ();
	~CFaxHeaderLines ();

	IMPORT_C void WriteRawFontLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine);
	IMPORT_C void WriteRawHeaderLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine);
	IMPORT_C void WriteFaxHeaderInfoL (TFaxHeaderInfo & aFaxHeaderInfo);
	IMPORT_C void ReadRawFontLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine);
	IMPORT_C void ReadRawHeaderLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine);
	IMPORT_C void ReadFaxHeaderInfoL (TFaxHeaderInfo & aFaxHeaderInfo);
	IMPORT_C static CFaxHeaderLines * NewLC (RFile* aHeaderFile);
	IMPORT_C static void GeneratePathForHeaderFileL(TDes& aPrivatePath);

protected:
	CFaxHeaderLines(RFile* aHeaderFile);
	CFaxHeaderLines();
	void ConstructL ();
	static void GenerateHeaderPathL(TDes& aPrivatePath);
	inline RFile& File();

private:
	RFs iFileSession;
	RFile iFile;
	TInt iSeekpos;
	TInt iLineNumber;
	RFile* iAdoptedHeaderFile;
	TBool iUseAdpotedFileHandle;

public:
	/** The fax header information package. */
	TFaxHeaderInfoPckg iOurFaxHeaderInfoPckg;
	};

#endif // __CFAXIO_H__
