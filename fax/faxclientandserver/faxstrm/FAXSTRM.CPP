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

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

#include <s32file.h>
#include <s32mem.h>
#include "FAXSTPAN.H"

#include "FAXSTORE.H"
#include "faxpageinfo.h"


GLDEF_C void Panic (TFaxStorePanic aPanic)
// Panic the process with ETEXT as the category.
 //

{
	User::Panic (_L ("FaxStrm"), aPanic);
}

// END OF COPIED

EXPORT_C TFaxBandHeader::TFaxBandHeader ():
 iNumScanLines (0),
 iStreamId (KNullStreamId)
/**
@capability None
*/
{
}

EXPORT_C TFaxBandHeader::TFaxBandHeader (TStreamId aStreamId):
 iNumScanLines (0),
 iStreamId (aStreamId)
/**
@capability None
*/
{
}

EXPORT_C void TFaxBandHeader::InternalizeL (RReadStream & aStream)
/**
@capability None
*/
{
	iNumScanLines = aStream.ReadInt32L ();
	aStream >> iStreamId;
}

EXPORT_C void TFaxBandHeader::ExternalizeL (RWriteStream & aStream) const
/**
@capability None
*/
{
	aStream.WriteInt32L (iNumScanLines);
	aStream << iStreamId;
}

CFaxPageInfo::CFaxPageInfo ():
iResolution (EFaxNormal)
{
	__DECLARE_NAME (_S ("CFaxPageInfo"));
}

EXPORT_C CFaxPageInfo *CFaxPageInfo::NewL ()
/**
@capability None
*/
{
	CFaxPageInfo *pageinfo = new (ELeave) CFaxPageInfo ();
	CleanupStack::PushL (pageinfo);
	pageinfo->iBandHeaderList = new (ELeave) CArrayFixFlat < TFaxBandHeader > (8);
	CleanupStack::Pop ();
	return pageinfo;
}

EXPORT_C CFaxPageInfo::~CFaxPageInfo ()
{
	delete iBandHeaderList;
}

EXPORT_C void CFaxPageInfo::InternalizeL (RReadStream & aStream)
/**
@capability None
*/
{
	iResolution = (TFaxResolution) aStream.ReadInt8L ();
	aStream >> *iBandHeaderList;
	aStream >> iSenderId;
	iCompression = (TFaxCompression) aStream.ReadInt32L ();
	iReservedFlag2 = aStream.ReadInt32L ();
}

EXPORT_C void CFaxPageInfo::ExternalizeL (RWriteStream & aStream) const
/**
@capability None
*/
{
	aStream.WriteInt8L ((TInt8) iResolution);
	aStream << *iBandHeaderList;
	aStream << iSenderId;
	aStream.WriteInt32L (iCompression);
	aStream.WriteInt32L (iReservedFlag2);
}
/********************************************************************/

CFaxPages::CFaxPages ()
{
	__DECLARE_NAME (_S ("CFaxPages"));
}

EXPORT_C CFaxPages *CFaxPages::NewL ()
/**
@capability None
*/
{
	CFaxPages *faxpages = new (ELeave) CFaxPages ();
	CleanupStack::PushL (faxpages);
	faxpages->iPageStreamIdList = new (ELeave) CArrayFixFlat < TStreamId > (8);
	CleanupStack::Pop ();
	return faxpages;
}

EXPORT_C CFaxPages::~CFaxPages ()
{
	delete iPageStreamIdList;
}

EXPORT_C void CFaxPages::InternalizeL (RReadStream & aStream)
/**
@capability None
*/
{
	aStream >> *iPageStreamIdList;
}

EXPORT_C void CFaxPages::ExternalizeL (RWriteStream & aStream) const
/**
@capability None
*/
{
	aStream << *iPageStreamIdList;
}
/********************************************************************/

CWriteFaxPages::CWriteFaxPages (CStreamStore & aStore, TInt aMaxScanLinesInBand):
iMaxScanLinesInBand (aMaxScanLinesInBand ? aMaxScanLinesInBand : 64),
iStore (&aStore)
{
	__DECLARE_NAME (_S ("CWriteFaxPages"));
}

void CWriteFaxPages::ConstructL ()
{
	iFaxPages = CFaxPages::NewL ();
	iCurrentPage = CFaxPageInfo::NewL ();
	iFaxT4 = CFaxT4::NewL ();
}

TBool CWriteFaxPages::BandCompleted ()
{
	TInt count = iCurrentPage->iBandHeaderList->Count ();
	return (!count) || ((*iCurrentPage->iBandHeaderList)[count - 1].iNumScanLines == iMaxScanLinesInBand);
}

EXPORT_C CWriteFaxPages *CWriteFaxPages::NewL (CStreamStore & aStore, TInt aMaxScanLinesInBand)
/** Creates a CWriteFaxPages object, which offers the public API for writing fax 
pages to a stream store. 

This function is called by CWriteFaxFile as part of creating a fax file.

@param aStore The store to which fax pages are to be added. 
@param aMaxScanLinesInBand The maximum number of scan lines in a band. Faxes 
pages are stored in bands - to speed up display time. It is recommended 
that developers use 64 - the value used by CWriteFaxFile. 
@leave KErrNoMemory There is insufficient memory to perform the operation.
@return A pointer to the newly created object. 
@capability None
*/
{
	CWriteFaxPages *writefaxpages = new (ELeave) CWriteFaxPages (aStore, aMaxScanLinesInBand);
	CleanupStack::PushL (writefaxpages);
	writefaxpages->ConstructL ();
	CleanupStack::Pop ();
	return writefaxpages;
}

EXPORT_C CWriteFaxPages::~CWriteFaxPages ()
/** Destructor.

Closes the write stream, and frees all resources owned by the object, prior 
to its destruction. */
{
	iWriteStream.Close ();
	delete iFaxT4;
	delete iFaxPages;
	delete iCurrentPage;
}

EXPORT_C void CWriteFaxPages::StartPage (TFaxResolution aResolution, TFaxCompression aCompression, TInt aFlag2)
/**
Initialize fax page, set page parameters.

@param   aResolution     defines fax resolution
@param   aCompression    defines fax compression
@param   aFlag2          reserved flag.
@capability None
*/
{
	iFaxT4->PageInitialize (aResolution, aCompression, aFlag2);
}

EXPORT_C void CWriteFaxPages::EndPageL (TFaxResolution aResolution, TFaxBufSenderId & aSenderId, TFaxCompression aCompression, TInt aFlag2)
/**
Sets parameters for the current page, writes it to the stream and appends to the fax pages set.
   
@param   aResolution     defines fax resolution
@param   aSenderId       Sender Id.
@param   aCompression    defines fax compression
@param   aFlag2          reserved flag.
@capability None
*/
{
	if (!BandCompleted ())
		{
		iWriteStream.CommitL ();
		iWriteStream.Close ();
		}
	iCurrentPage->iResolution = aResolution;
	iCurrentPage->iSenderId = aSenderId;
	iCurrentPage->iCompression = aCompression;
	iCurrentPage->iReservedFlag2 = aFlag2;
	TStreamId streamid = iWriteStream.CreateL (*iStore);
	iWriteStream << *iCurrentPage;
	iWriteStream.CommitL ();
	iWriteStream.Close ();
	iFaxPages->iPageStreamIdList->AppendL (streamid);
	iCurrentPage->iBandHeaderList->Reset ();
}

EXPORT_C TStreamId CWriteFaxPages::CommitPageL ()
/**
Place FaxPages into write stream.

@return Stream Id.
@capability None
*/
{
	TStreamId streamid = iWriteStream.CreateL (*iStore);
	iWriteStream << *iFaxPages;
	iWriteStream.CommitL ();
	iWriteStream.Close ();
	iStore->CommitL ();
	return streamid;
}

EXPORT_C void CWriteFaxPages::AddScanLineL (const TDesC8 & aScanLine)
/** Adds raw scan lines to the write stream.
	
The function first encodes the scan line, using the format specified in the 
StartPage() function, and then adds it to the write stream.
	
@param aScanline The current raw scan line. 
@capability None
*/
{
	iFaxT4->EncodeScanLine (aScanLine, iEncodedScanLine);
	AddEncodedScanLineL (iEncodedScanLine);
}

EXPORT_C void CWriteFaxPages::AddEncodedScanLineL (const TDesC8 & anEncodedScanLine)
/** Adds encoded scan lines to the write stream.

@param anEncodedScanLine The encoded scan line (MH or MR). 
@capability None
*/
{
	if (BandCompleted ())
		{
		TStreamId streamid = iWriteStream.CreateL (*iStore);
		iCurrentPage->iBandHeaderList->AppendL (TFaxBandHeader (streamid));
		}

	TInt count = iCurrentPage->iBandHeaderList->Count ();
	TFaxBandHeader *bandheader = &(*iCurrentPage->iBandHeaderList)[count - 1];
	iWriteStream << anEncodedScanLine;
	bandheader->iNumScanLines++;
	bandheader->iNumBytes += anEncodedScanLine.Length ();

	if (BandCompleted ())
		{
		iWriteStream.CommitL ();
		iWriteStream.Close ();
		}
}
 /********************************************************************/

CWriteFaxFile::CWriteFaxFile ()
{
	__DECLARE_NAME (_S ("CWriteFaxFile"));
}

void CWriteFaxFile::ConstructL ()
{
	User::LeaveIfError (iFs.Connect ());
}

const TUid KUidPsiFaxApp = {268435908};

void CWriteFaxFile::DoOpenL (TInt aMaxScanLinesInBand)
{
	iFileStore = CDirectFileStore::ReplaceL (iFs, iFileName, EFileWrite);
	TUidType type (KDirectFileStoreLayoutUid, TUid::Uid (KFaxFileStoreUidVal), KUidPsiFaxApp);
	iFileStore->SetTypeL (type);
	iWriteFaxPages = CWriteFaxPages::NewL (*iFileStore, aMaxScanLinesInBand);
}

EXPORT_C CWriteFaxFile *CWriteFaxFile::NewL ()
/** Constructs a CWriteFaxFile object, which offers the public API for creating 
a fax store file.

As part of the construction process, the object opens a session with the file 
server.

@return A pointer to the newly created object. 
@capability None
*/
{
	CWriteFaxFile *writefaxfile = new (ELeave) CWriteFaxFile;
	CleanupStack::PushL (writefaxfile);
	writefaxfile->ConstructL ();
	CleanupStack::Pop ();
	return writefaxfile;
}

EXPORT_C CWriteFaxFile::~CWriteFaxFile ()
/** Destructor.

Closes the session with the file server, and frees all resources owned by 
the object, prior to its destruction. */
{
	Close ();
	iFs.Close ();
}

LOCAL_C void DoAbort (TAny * aPtr)
{
	((CWriteFaxFile *) aPtr)->AbortWrite ();
}

EXPORT_C void CWriteFaxFile::OpenL (const TDesC & aFileName, TInt aMaxScanLinesInBand)
/** Creates and opens a fax file for writing.

The function also allocates memory to create a CWriteFaxPages object which 
is pointed to by the class data member. This object defines the API for writing 
pages to the file store.

Fax files which have been opened should be paired with a Close() function.

@param aFileName The name of the new fax file. 
@param aMaxScanLinesInBand The maximum number of scan lines in a band. Fax 
pages are stored in bands for quick retrieval/display. If zero is passed then 
it will default to 64 otherwise the passed value will take effect. 
@capability None
*/
{

	iFileName = aFileName;
	CleanupStack::PushL (TCleanupItem (DoAbort, this));
	DoOpenL (aMaxScanLinesInBand);
	CleanupStack::Pop ();
}


EXPORT_C void CWriteFaxFile::CommitL ()
/** Commits the current fax page created using the CWriteFaxPages API  to 
the fax file.

Committing a page writes it to the fax file, rather than storing it in temporary 
memory. The function should be called after each page is added, to ensure 
that only one fax page can be lost if there is an out of memory error. 
@capability None
*/
{
	TStreamId streamid = iWriteFaxPages->CommitPageL ();
	iFileStore->SetRootL (streamid);
	iFileStore->CommitL ();
}


EXPORT_C void CWriteFaxFile::Close ()
/** Closes the fax file, and deletes the resources owned by the object. 

Although this function is called in the destructor, it is good programming 
practice to invoke it manually to pair previous OpenL() calls. 
@capability None
*/
{
	delete iWriteFaxPages;
	iWriteFaxPages = NULL;
	delete iFileStore;
	iFileStore = NULL;
}

EXPORT_C void CWriteFaxFile::AbortWrite ()
/** Aborts the creation of the fax store file, and then deletes the file. 

It can be called by developer programs, and is called automatically if OpenL() 
leaves. 
@capability None
*/
{
	Close ();
	iFs.Delete (iFileName);
}
/********************************************************************/

CReadFaxPages::CReadFaxPages (CStreamStore & aStore):
iStore (&aStore)
{
	__DECLARE_NAME (_S ("CReadFaxPages"));
}

void CReadFaxPages::ConstructL (TStreamId aStreamId)
{
	iFaxPages = CFaxPages::NewL ();
	iCurrentPage = CFaxPageInfo::NewL ();
	iFaxT4 = CFaxT4::NewL ();
	iReadStream.OpenL (*iStore, aStreamId);
	iReadStream >> *iFaxPages;
	iReadStream.Close ();
	SetPageL (0);
}

EXPORT_C CReadFaxPages *CReadFaxPages::NewL (CStreamStore & aStore, TStreamId aStreamId)
/** Creates a CReadFaxPages object, which offers the public API for reading fax 
pages from a stream store. 

This function is called by CReadFaxFile when opening a fax file.

@param aStore The store from which fax pages are to be read. 
@param aStreamId The root ID of the stream store. 
@leave KErrNoMemory There is insufficient memory to perform the operation. 
@return A pointer to the newly created object. 
@capability None
*/
{
	CReadFaxPages *readfaxpages = new (ELeave) CReadFaxPages (aStore);
	CleanupStack::PushL (readfaxpages);
	readfaxpages->ConstructL (aStreamId);
	CleanupStack::Pop ();
	return readfaxpages;
}

EXPORT_C CReadFaxPages::~CReadFaxPages ()
/** Destructor.

Closes the stream, and frees all resources owned by the object, prior to its 
destruction. */
{
	iReadStream.Close ();
	delete iFaxT4;
	delete iFaxPages;
	delete iCurrentPage;
}

EXPORT_C TInt CReadFaxPages::NumPages () const
/** Gets the number of pages in the store.

@return The number of fax pages in the store. 
@capability None
*/
{
	return iFaxPages->iPageStreamIdList->Count ();
}

EXPORT_C void CReadFaxPages::SetPageL (TInt aNum)
/** Sets a selected page to be the current page, and resets the current scan line 
to the first scan line in the page.

@param aNum The number of the new page. 
@capability None
*/
{
	__ASSERT_DEBUG ((aNum >= 0) && (aNum < iFaxPages->iPageStreamIdList->Count ()), Panic (EFaxPageIndexOutOfRange));
	iReadStream.Close ();
	iReadStream.OpenL (*iStore, (*iFaxPages->iPageStreamIdList)[aNum]);
	iReadStream >> *iCurrentPage;
	iReadStream.Close ();
	iFaxT4->PageInitialize (iCurrentPage->iResolution, iCurrentPage->iCompression, iCurrentPage->iReservedFlag2);
	SeekScanLineL (0);
}

EXPORT_C TFaxPageInfo CReadFaxPages::CurrentPageInfo () const
/** Gets the information for the current fax page, where the current page was set 
in a previous call to the SetPageL() function.

@return The information for the current fax page 
@capability None
*/
{
	TFaxPageInfo info;
	info.iResolution = iCurrentPage->iResolution;
	info.iSenderId = iCurrentPage->iSenderId;
	info.iCompression = iCurrentPage->iCompression;
	info.iReservedFlag2 = iCurrentPage->iReservedFlag2;
	info.iNumScanLines = 0;
	TInt count = iCurrentPage->iBandHeaderList->Count ();
	for (TInt i = 0; i < count; i++)
		info.iNumScanLines += (*iCurrentPage->iBandHeaderList)[i].iNumScanLines;
	return info;
}

EXPORT_C void CReadFaxPages::SeekScanLineL (TInt anIndex)
/** Sets the specified scan line as the current scan line. 

@param anIndex The index of the scan line. 
@capability None
*/
{
	__ASSERT_DEBUG ((anIndex >= 0) && (anIndex < CurrentPageInfo ().iNumScanLines), Panic (EFaxScanLineIndexOutOfRange));
	TInt numscanlines = 0;
	TInt count = iCurrentPage->iBandHeaderList->Count ();
	for (iBandIndex = 0; (iBandIndex < count) && ((numscanlines + (*iCurrentPage->iBandHeaderList)[iBandIndex].iNumScanLines) <= anIndex); iBandIndex++)
		numscanlines += (*iCurrentPage->iBandHeaderList)[iBandIndex].iNumScanLines;

	SetBandL (iBandIndex);
	TBuf8 < KFaxT4MaxDesLength > encodedscanline;
	for (iScanLineOffset = 0; iScanLineOffset < (anIndex - numscanlines);)
	GetEncodedScanLineL (encodedscanline);
}

EXPORT_C TInt CReadFaxPages::GetScanLineL (TDes8 & aScanLine)
/** Gets the raw scan line specified in a previous call to the SeekScanLineL() 
function. 

The scan line is stored in compressed form, but is retrieved by this function 
in raw form for viewing.

@param aScanLine On return, contains the scan line. 
@return KErrNone if successful, otherwise another of the system-wide error 
codes. 
@capability None
*/
{
	GetEncodedScanLineL (iEncodedScanLine);
	return iFaxT4->DecodeScanLine (aScanLine, iEncodedScanLine);
}

EXPORT_C void CReadFaxPages::GetEncodedScanLineL (TDes8 & anEncodedScanLine)
/** Gets the encoded scan line specified in a previous call to the SeekScanLineL() 
function.

@param anEncodedScanLine On return, contains the encoded scan line. 
@capability None
*/
{
	if (iScanLineOffset == (*iCurrentPage->iBandHeaderList)[iBandIndex].iNumScanLines)
		SetBandL (iBandIndex + 1);
	iReadStream >> anEncodedScanLine;
	iScanLineOffset++;
}

void CReadFaxPages::SetBandL (TInt anIndex)
{
	__ASSERT_DEBUG ((anIndex >= 0) && (anIndex < iCurrentPage->iBandHeaderList->Count ()), Panic (EFaxBandIndexOutOfRange));
	iBandIndex = anIndex;
	iReadStream.Close ();
	iReadStream.OpenL (*iStore, (*iCurrentPage->iBandHeaderList)[iBandIndex].iStreamId);
	iScanLineOffset = 0;
}
 /********************************************************************/

CReadFaxFile::CReadFaxFile ()
{
	__DECLARE_NAME (_S ("CReadFaxFile"));
}

void CReadFaxFile::ConstructL ()
{
	User::LeaveIfError (iFs.Connect ());
}

void CReadFaxFile::DoOpenL (const TDesC & aFileName)
{
	iFileStore = CDirectFileStore::OpenL (iFs, aFileName, EFileStream | EFileRead | EFileShareReadersOnly);
	if (iFileStore->Type ()[1] != TUid::Uid (KFaxFileStoreUidVal))
		User::Leave (KErrNotSupported);
	TStreamId streamid = iFileStore->Root ();
	iReadFaxPages = CReadFaxPages::NewL (*iFileStore, streamid);
}

EXPORT_C CReadFaxFile *CReadFaxFile::NewL ()
/** Constructs a CReadFaxFile object, which offers the public API for opening a 
fax file for reading. 

As part of the construction process, the object starts a session with the 
file server.

@return A pointer to the newly created object. 
@capability None
*/
{
	CReadFaxFile *readfaxfile = new (ELeave) CReadFaxFile;
	CleanupStack::PushL (readfaxfile);
	readfaxfile->ConstructL ();
	CleanupStack::Pop ();
	return readfaxfile;
}

EXPORT_C CReadFaxFile::~CReadFaxFile ()
/** Destructor.

Closes the session with the file server, and frees all resources owned by 
the object, prior to its destruction. */
{
	Close ();
	iFs.Close ();
}

EXPORT_C void CReadFaxFile::OpenL (const TDesC & aFileName)
/** Opens a fax file for reading. 

The function also allocates memory to create a CReadFaxPages object - which 
is pointed to by the iReadFaxPages class data member. This class defines the 
API for reading individual pages from the file store.

Fax files which have been opened should be paired with a Close() function.

@param aFileName The name of the fax file. 
@capability None
*/
{
	CleanupClosePushL (*this);
	DoOpenL (aFileName);
	CleanupStack::Pop ();
}

EXPORT_C void CReadFaxFile::Close ()
/** Closes the fax file, and deletes the resources owned by the object. 

Although this function is called in the destructor, it is good programming 
practice to invoke it manually to pair previous OpenL() calls. 
@capability None
*/
	{
	delete iReadFaxPages;
	iReadFaxPages = NULL;
	delete iFileStore;
	iFileStore = NULL;
	}
