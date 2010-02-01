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

/**
 @file
 @publishedPartner
*/

#if !defined(__FAXSTORE_H__)
#define __FAXSTORE_H__

#include <faxdefn.h>
#include <cfaxio.h>

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <faxpageinfo.h>
#endif

class TFaxBandHeader;
class CFaxPageInfo;
class CFaxPages;

/**
Adds pages to a fax file.

The class is used by first calling StartPage() to define the compression and 
resolution of the fax page, and then calling AddEncodedScanLineL() or AddScanLineL() 
to add each of the page's encoded or raw scan lines. When the page is complete, 
the EndPage() function adds the scan line compression, scan line resolution, 
and sender Id information to the store. 

In pre-v5.1 releases of Symbian OS, the import library was faxst2.lib. 

@publishedPartner
@released
*/
class CWriteFaxPages : public CBase
	{
protected:
	CWriteFaxPages(CStreamStore& aStore,TInt aMaxScanLinesInBand);
	void ConstructL();
	TBool BandCompleted();
public:
	IMPORT_C static CWriteFaxPages* NewL(CStreamStore& aStore,TInt aMaxScanLinesInBand=0);
	IMPORT_C ~CWriteFaxPages();
	IMPORT_C void StartPage(TFaxResolution aResolution, TFaxCompression aCompression, TInt aFlag2 = 0);
	IMPORT_C void EndPageL(TFaxResolution aResolution, TFaxBufSenderId& aSenderId, TFaxCompression aCompression = EModifiedHuffman, TInt aFlag2 = 0);
	IMPORT_C void AddScanLineL(const TDesC8& aScanline);
	IMPORT_C void AddEncodedScanLineL(const TDesC8& anEncodedScanLine);
	IMPORT_C TStreamId CommitPageL();
private:
	TInt iMaxScanLinesInBand;
	CFaxPages* iFaxPages;
	CFaxPageInfo* iCurrentPage;
	CStreamStore* iStore;
	CFaxT4* iFaxT4;
	RStoreWriteStream iWriteStream;
	TBuf8<KFaxT4MaxDesLength> iEncodedScanLine;
	};

/**
Creates and open a fax file for writing.

The class provides the functionality to create a fax file, to commit pages 
to the fax file, to abort and delete the file, and to close the file. The 
ability to add individual fax pages to the store is provided through the CWriteFaxPages 
data member, which is created when the file is opened.

In pre-v5.1 releases of Symbian OS, the import library was faxst2.lib. 

@publishedPartner
@released
*/
class CWriteFaxFile : public CBase
	{
protected:
	CWriteFaxFile();
	void ConstructL();
	void DoOpenL(TInt aMaxScanLinesInBand);
public:
	IMPORT_C static CWriteFaxFile* NewL();  
	IMPORT_C ~CWriteFaxFile();
	IMPORT_C void OpenL(const TDesC& aFileName,TInt aMaxScanLinesInBand);
	IMPORT_C void CommitL();	
	IMPORT_C void Close();	
	IMPORT_C void AbortWrite();	 // deletes file, called by OpenL() and CloseL() if they leave
private:
	TFileName iFileName;
	RFs iFs;
	CDirectFileStore* iFileStore;
public:
	/** A pointer to the object which can be used to add pages to the file. Memory 
	is allocated for the object when the file is opened. */
	CWriteFaxPages* iWriteFaxPages;
	};

/**
Fax page information read from a file.

Stores fax page information which is read from a fax file - see CReadFaxPages::CurrentPageInfo().

@publishedPartner
@released 
*/
class TFaxPageInfo
	{
public:
	/** The number of scan lines. */
	TInt iNumScanLines;
	/** The fax page resolution. */
	TFaxResolution iResolution;
	/** The sender Id. */
	TFaxBufSenderId iSenderId;
	/** The fax page encoding. */
    TFaxCompression iCompression; // was TInt iReservedFlag1 up to faxstore 013
	TInt iReservedFlag2;
	};

/**
Reads individual pages from a fax file.

The class provides functions to get the number of fax pages in a fax store, 
the number of scan lines per page, information about the scan line compression 
and resolution, and the sender Id. It is then possible to iterate through 
the store by page and by scan line to obtain the fax page.

In pre-v5.1 releases of Symbian OS, the import library was faxst2.lib. 

@publishedPartner
@released
*/
class CReadFaxPages : public CBase
	{
protected:
	CReadFaxPages(CStreamStore& aStore);
	void ConstructL(TStreamId aStreamId);
public:
	IMPORT_C static CReadFaxPages* NewL(CStreamStore& aStore,TStreamId aStreamId);
	IMPORT_C ~CReadFaxPages();
	IMPORT_C TInt NumPages() const;
	IMPORT_C void SetPageL(TInt aNum);
	IMPORT_C TFaxPageInfo CurrentPageInfo() const;
	IMPORT_C void SeekScanLineL(TInt anIndex);
	IMPORT_C TInt GetScanLineL(TDes8& aScanLine);
	IMPORT_C void GetEncodedScanLineL(TDes8& anEncodedScanLine);
protected:
	void SetBandL(TInt anIndex);
private:
	TInt iBandIndex;
	TInt iScanLineOffset;
	CFaxPages* iFaxPages;
	CFaxPageInfo* iCurrentPage;
	CStreamStore* iStore;
	CFaxT4* iFaxT4;
	RStoreReadStream iReadStream;
	TBuf8<KFaxT4MaxDesLength> iEncodedScanLine;
	};

/**
Opens a fax file for reading.

The class provides the functionality to open and close a fax file. The ability 
to read individual fax pages is provided by the CReadFaxPages data member, 
which is created when the file is opened.

In pre-v5.1 versions of Symbian OS, the import library was faxst2.lib. 

@publishedPartner
@released
*/
class CReadFaxFile : public CBase
	{
protected:
	CReadFaxFile();
	void ConstructL();
	void DoOpenL(const TDesC& aFileName);
public:
	IMPORT_C static CReadFaxFile* NewL();
	IMPORT_C ~CReadFaxFile();
	IMPORT_C void OpenL(const TDesC& aFileName);
	IMPORT_C void Close();
private:
	RFs iFs;
	CDirectFileStore* iFileStore;
public:
	/** A pointer to the object which can be used to access the fax pages stored in 
	the file. Memory is allocated for the object when the fax file is opened. */
	CReadFaxPages* iReadFaxPages;
	};

// now the TFaxHeaderInfo and CFaxHeaderLines classes
// for saving/restoring fax headers
// used by fax client and fax server
/*
class TFaxHeaderInfo
	{
public:
	TInt iHeaderFontWidthInBytes;	// width of font in bytes
	TInt iHeaderFontHeightInLines;// height of font in lines
	TInt iOffsetToDay;            // offset to two digit day of month
	TInt iOffsetToMonth;          // offset to two digits month of year
	TInt iOffsetToYear;           // offset to four digits year
	TInt iOffsetToHour;           // offset to two digits hour (24 hour clock) 
	TInt iOffsetToMinute;         // offset to two digits minute
	TInt iOffsetToTotalPages;     // offset to two digits for total pages
	TInt iOffsetToCurrentPage;    // offset to two digits for current page
	};

typedef TPckgBuf < TFaxHeaderInfo > TFaxHeaderInfoPckg;

class CFaxHeaderLines : public CBase
	{
public:
	IMPORT_C static CFaxHeaderLines * NewL();
	IMPORT_C static CFaxHeaderLines * NewLC();
	~CFaxHeaderLines ();

	IMPORT_C void WriteRawFontLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine);
	IMPORT_C void WriteRawHeaderLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine);
	IMPORT_C void WriteFaxHeaderInfoL (TFaxHeaderInfo & aFaxHeaderInfo);
	IMPORT_C void ReadRawFontLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine);
	IMPORT_C void ReadRawHeaderLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine);
	IMPORT_C void ReadFaxHeaderInfoL (TFaxHeaderInfo & aFaxHeaderInfo);

protected:
	void ConstructL();

private:
	RFs iFileSession;
	RFile iFile;
	TInt iSeekpos;
	TInt iLineNumber;

public:
	TFaxHeaderInfoPckg iOurFaxHeaderInfoPckg;
	};*/

#endif
