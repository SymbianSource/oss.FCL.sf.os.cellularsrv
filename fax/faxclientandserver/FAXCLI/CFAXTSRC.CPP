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
*/


#include "CFAX32.H"

/********************************************************************/

CFaxTransferSource *CFaxTransferSource::NewLC()
	{
	CFaxTransferSource *self = new(ELeave) CFaxTransferSource;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
/********************************************************************/

CFaxTransferSource *CFaxTransferSource::NewL()
	{
	CFaxTransferSource *self = NewLC();
	CleanupStack::Pop();
	return self;
	}
/********************************************************************/

void CFaxTransferSource::ConstructL ()
   {
   iSources = CBufSeg::NewL (256);
   }
/********************************************************************/

CFaxTransferSource::~CFaxTransferSource ()
   {
   delete iSources;
   iSources = NULL;
   }
/********************************************************************/

// we come here with a document and a page or range of pages
//EXPORT_C void CFaxTransferSource::AddSourceL (const TFileName & aFaxPageStore, const TFileName & aFaxPageStore2)
//	{
//	AddSourceL(aFaxPageStore,1);
//	AddSourceL(aFaxPageStore2,1);
//	iFaxPages=iFaxPages/2;
//	}

EXPORT_C void CFaxTransferSource::AddSourceL (const TFileName & aFaxPageStore, TFaxPreferredCompression aPreferredCompression)
/**
@capability WriteUserData
*/
   {
   AddSourceL (aFaxPageStore, 1, aPreferredCompression);
   }

EXPORT_C void CFaxTransferSource::AddSourceL (const TFileName & aFaxPageStore, TInt aStartPage, TFaxPreferredCompression aPreferredCompression)
/**
@capability WriteUserData
*/
   {
   if (aStartPage < 1)
      User::Leave (KErrUnderflow);
   TInt lastPage = OpenFaxOutL (aFaxPageStore);
   CloseFaxStore ();
   AddSourceL (aFaxPageStore, aStartPage, lastPage, aPreferredCompression);
   }

EXPORT_C void CFaxTransferSource::AddSourceL (const TFileName & aFaxPageStore, TInt aStartPage, TInt aEndPage, TFaxPreferredCompression aPreferredCompression)
/**
@capability WriteUserData
*/
   {
   iPage = 0;
   if (aStartPage < 1)
      User::Leave (KErrUnderflow);
   else
      {
      if (aStartPage > aEndPage)
         User::Leave (KErrOverflow);
      else
         {
         iOurFaxEntry.iFaxPageStore = aFaxPageStore;
         iOurFaxEntry.iStartPage = aStartPage;
         iOurFaxEntry.iPageCount = (aEndPage - aStartPage + 1);

         iOutstream.Append (*iSources);
         iOutstream << iOurFaxEntry;
         iOutstream.CommitL ();

         iFaxListEntries++;
         iFaxPages += iOurFaxEntry.iPageCount;
		 iOurPreferredCompression=aPreferredCompression;
         }
      }
   }
/********************************************************************/

EXPORT_C void CFaxTransferSource::RemoveAllSources ()
/**
@capability WriteUserData
*/
   {
   iFaxPages = 0;
   iFaxListEntries = 0;
   iSources->Reset ();
   }
/********************************************************************/

// this gets the next page of the fax
// and makes it all ready to be sent

// the iInstream is opened already, and
// we never call this if iFaxListEntries
// and iOurFaxEntry.iPageCount are both zero

void CFaxTransferSource::GetNextPageReadyL ()
   {
   // if no pages in a current open document we need to get the next doc

   if (iOurFaxEntry.iPageCount == 0)
      {

      // if page count is 0 we haven't started yet
      // otherwise close the finished document
      // then read the next entry in
      // mark one less entry to deal with
      // and open the next document

      if (iPage != 0)
         CloseFaxStore ();

      iInstream >> iOurFaxEntry;

      --iFaxListEntries;
      OpenFaxOutL (iOurFaxEntry.iFaxPageStore);

      // skip pages if we don't send from page 1

      for (--iOurFaxEntry.iStartPage; iOurFaxEntry.iStartPage; --iOurFaxEntry.iStartPage)
         ReadPageParmsL ();
      }
   ReadPageParmsL ();
   iPage++;
   --iOurFaxEntry.iPageCount;
   }
/********************************************************************/

void CFaxTransferSource::OpenFaxInL (const TDesC & aFileName)
   {
   if (iWriteFaxFile == NULL)
      iWriteFaxFile = CWriteFaxFile::NewL ();
   iWriteFaxFile->OpenL (aFileName, 0);
   }

void CFaxTransferSource::PagePrepare ()
   {
   iLines = 0;
   ++iPage;
   }

void CFaxTransferSource::WriteFaxDataL (const TDesC8 & aBuffer)
   {
   iWriteFaxFile->iWriteFaxPages->AddEncodedScanLineL (aBuffer);
   if (aBuffer.Length () != KMaxT4Des)
      iLines++;
   }

void CFaxTransferSource::WritePageParmsL (TFaxBufSenderId & aSenderId)
   {
   iWriteFaxFile->iWriteFaxPages->EndPageL (TFaxResolution (iResolu), aSenderId, TFaxCompression (iCompression));
   iWriteFaxFile->CommitL ();
   }

void CFaxTransferSource::CloseFaxInStore ()
   {
   iWriteFaxFile->Close ();     // was CloseL till 6/3/97
   delete iWriteFaxFile;
   iWriteFaxFile = NULL;
   }
/*******************************************************************/

TInt CFaxTransferSource::OpenFaxOutL (const TDesC & aFileName)
   {
   CReadFaxFile *fax = iReadFaxFile;
   if (fax == NULL)
      iReadFaxFile = fax = CReadFaxFile::NewL ();
   fax->OpenL (aFileName);
   iPageInCurrentFile = 0;
   return (fax->iReadFaxPages->NumPages ());
   }

TInt CFaxTransferSource::NextPageFind ()
   {
   return (iReadFaxFile->iReadFaxPages->NumPages ()) - iPageInCurrentFile;
   }

void CFaxTransferSource::ReadPageParmsL ()
   {
   CReadFaxPages & pages = *iReadFaxFile->iReadFaxPages;
   pages.SetPageL (iPageInCurrentFile++);
   const TFaxPageInfo & info = pages.CurrentPageInfo ();
   iLines = info.iNumScanLines;
   iResolu = info.iResolution;
   iCompression = info.iCompression;
   }

void CFaxTransferSource::ReadFaxData(TDes8& aBuffer)
	{
	// Discard the following error
	TRAP_IGNORE(iReadFaxFile->iReadFaxPages->GetEncodedScanLineL(aBuffer));
	}

void CFaxTransferSource::PreviousPageFindL()
	{
	--iPageInCurrentFile;
	ReadPageParmsL();
	}

void CFaxTransferSource::CloseFaxStore ()
   {
   if (iReadFaxFile != NULL)
      {
      iReadFaxFile->Close ();
      delete iReadFaxFile;
      iReadFaxFile = NULL;
      }
   }


/*******************************************************************/

// we've also got a small subclass to handle the details of pages in the fax

void TFaxEntry::ExternalizeL (RWriteStream & aStream) const
   {
   aStream << iFaxPageStore;
   aStream.WriteUint8L (iStartPage);
   aStream.WriteUint8L (iPageCount);
   }

void TFaxEntry::InternalizeL (RReadStream & aStream)
   {
   aStream >> iFaxPageStore;
   iStartPage = aStream.ReadUint8L ();
   iPageCount = aStream.ReadUint8L ();
   }
/********************************************************************/
