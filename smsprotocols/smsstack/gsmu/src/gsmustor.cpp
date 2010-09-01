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
// Implements the classes TSAREntry and CSmsSARStore
// 
//

/**
 @file
*/

#include <e32svr.h>

#include "gsmustor.h"
#include "Gsmumain.h"

const TDriveNumber KStoreDrive = EDriveC;
_LIT(KStoreSubDir, "sms\\");


/**
 *  Utility func for cleanup stack - close the filestore and set pointer to NULL
 */
void CSARStoreCloseObject(TAny* aObj)
    {
    LOGGSMU2("WARNING! Hey, CSARStoreCloseObject called by Untrapper! [0x%08x]", aObj);
    ((CSARStore*)aObj)->Revert();
    }

//
// implementation of TSAREntry
//


/**
 *  Constructor
 *  
 *  @capability None
 */
EXPORT_C TSAREntry::TSAREntry():
	iReference(0),
	iTotal(0),
	iCount(0),
	iData1(0),
	iData2(0),
	iData3(0),
	iData4(0),
	iDataStreamId(KNullStreamId),
	iFlags(0)
	{
	} // TSAREntry::TSAREntry


/**
 *  Internalises the object.
 *  
 *  @param aStream Stream to read from
 *  @capability None
 */
EXPORT_C void TSAREntry::InternalizeL(RReadStream& aStream)
	{
	iReference=aStream.ReadInt32L();
	iTotal=aStream.ReadInt32L();
	iCount=aStream.ReadInt32L();
	iData1=aStream.ReadInt32L();
	iData2=aStream.ReadInt32L();
	iData3=aStream.ReadInt32L();
	iData4=aStream.ReadInt32L();
	aStream >> iDescription1;
	aStream >> iDescription2;
	TInt64 time;
	aStream >> time;
	iTime=time;
	aStream >> iDataStreamId;
	} // TSAREntry::InternalizeL


/**
 *  Externalises the object.
 *  
 *  @param aStream Stream to write to
 *  @capability None
 */
EXPORT_C void TSAREntry::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(iReference);
	aStream.WriteInt32L(iTotal);
	aStream.WriteInt32L(iCount);
	aStream.WriteInt32L(iData1);
	aStream.WriteInt32L(iData2);
	aStream.WriteInt32L(iData3);
	aStream.WriteInt32L(iData4);
	aStream << iDescription1;
	aStream << iDescription2;
	aStream << Time().Int64();
	aStream << iDataStreamId;
	} // TSAREntry::ExternalizeL


//
// implementation of CSARStore
//


/**
 *  Opens a specified SAR store.
 *  
 *  A SAR filestore has has three uids, KPermanentFileStoreLayoutUid, KSARStoreUid
 *  (which designates that the file store is a SAR store) and a third UID, that
 *  identifies the particular implementation of CSARStore.
 *  
 *  @param aFullName Name for the segmentation and reassembly filestore
 *  @param aThirdUid Third UID for the filestore.
 *  @capability None
 */
EXPORT_C void CSARStore::OpenL(const TDesC& aFullName, const TUid& aThirdUid)
	{
#ifdef _SMS_LOGGING_ENABLED
	TBuf8<80> buf8;
	buf8.Copy(aFullName);
	LOGGSMU2("CSARStore::OpenL(): '%S'", &buf8);
#endif

	// sanity check
	__ASSERT_DEBUG(iFileStore==NULL,Panic(KGsmuPanicSARStoreAlreadyOpen));
	__ASSERT_DEBUG(!iInTransaction, Panic(KGsmuPanicSARStoreTransaction));
	__ASSERT_DEBUG(iEntryArray.Count()==0,Panic(KGsmuPanicSARStoreEntryArrayNotReset));
	iFullName.Set(aFullName);
	iThirdUid = aThirdUid;

	// first opening we have to Internalize
	DoOpenL();
 	// defect fix for EDNJJUN-4WYJGP
 	// Unable to send sms cause sms*.dat is corrupted
 	TRAPD(ret, InternalizeEntryArrayL());
	if (ret != KErrNone)
		{
		LOGGSMU2("WARNING: InteralizeEntryArrayL left with %d", ret);
		}

 	if(ret == KErrCorrupt || ret == KErrEof || ret == KErrNotFound)
	 	{
 		Close();             //because the file is in use
		User::LeaveIfError(iFs.Delete(iFullName));
		DoOpenL();
 		}
	else
		User::LeaveIfError(ret);
	} // CSARStore::OpenL


/**
 *  Commits a transaction to the filestore.
 *  @capability None
 */
EXPORT_C void CSARStore::CommitTransactionL()
// This function does the real work of updating the filestore
	{
    LOGGSMU4("CSARStore::CommitTransactionL(): this=0x%08X iInTransaction=%d iFileStore=0x%08X",
    		 this, iInTransaction, iFileStore);

	__ASSERT_DEBUG(iFileStore!=NULL, Panic(KGsmuPanicSARStoreNotOpen));
	__ASSERT_DEBUG(iInTransaction, Panic(KGsmuPanicSARStoreTransaction));

#ifdef _SMS_LOGGING_ENABLED
	TRAPD(err, DoCommitAndCompactL());
	if (err != KErrNone)
		{
		LOGGSMU2("WARNING! could not CommitL/CompactL due to %d", err);
		User::Leave(err);
		}
#else
	DoCommitAndCompactL();
#endif

	CleanupStack::Pop(this);
	iInTransaction = EFalse;

	RemoveDeletedEntries();
	} // CSARStore::CommitTransactionL


/**
 *  Closes the file store.
 *  
 *  The function deletes the file store object and resets the entry array. It
 *  should be called once when the protocol module is closing down.
 *  @capability None
 */
EXPORT_C void CSARStore::Close()
	{
#ifdef _SMS_LOGGING_ENABLED
	TBuf8<80> buf8;
	buf8.Copy(iFullName);
	LOGGSMU2("CSARStore::Close(): '%S'", &buf8);
#endif

	__ASSERT_DEBUG(!iInTransaction, Panic(KGsmuPanicSARStoreTransaction));

	delete iFileStore;
	iFileStore=NULL;
	iEntryArray.Reset();
	} // CSARStore::Close


/**
 *  Gets a const reference to the entry array.
 *  
 *  @return Entry array
 *  @capability None
 */
EXPORT_C const CArrayFix<TSAREntry>& CSARStore::Entries() const
	{
	return iEntryArray;
	} // CSARStore::Entries


/**
 *  Purges the reassembly file store.
 *  
 *  After a multipart message, it delete all the old entries.
 *  
 *  Entries will be purged when: 1) The complete message is received; 2) After
 *  aTimerintervalMinutes, if aPurgeIncompletely is false.
 *  
 *  PurgeL() will be called after the booting of the device or when a message
 *  has been received.
 *  
 *  This function opens and closes the file automatically.
 *  
 *  Note: overload function for segmentation store.
 *  
 *  @param aTimeIntervalMinutes Purge time
 *  @param aPurgeIncompleteOnly Purge complete messages flag
 *  @capability None
 */
EXPORT_C void CSARStore::PurgeL(const TTimeIntervalMinutes& aTimeIntervalMinutes,
								TBool aPurgeIncompleteOnly)
//
// TODO ahe - performance filestore / sms
// In many cases the files will be opened and closed but no entry will be deleted.
// Conclusion:
// 1) only open, close, externalize and compact the file when an entry
//    really shall be deleted
// 2) Compact the file after purging, only compact it here
// 3) Only purge the segmentation store if the send queue is empty
//    and only purge the reassembly store when the receiv queue is full
//
// TODO ahe - for last release -
// message can be lost during crash: this issue has to be redesigned but
// after the reboot of the device the information in the stores could be lost
// due to purging or compacting - a single pdu always gets lost due to not
// storing it in the reassembly store.
//
	{
	LOGGSMU3("CSARStore::PurgeL(): aTimeIntervalMinutes=%d, aPurgeIncompleteOnly=%d",
			 aTimeIntervalMinutes.Int(), aPurgeIncompleteOnly);

	// TODO - flag
	// we could also save the call of the method from the consruction of the smsprot
	if( aPurgeIncompleteOnly )
		return;

	TInt count=iEntryArray.Count();
	LOGGSMU2("CSARStore::PurgeL(): count=%d", count);

	TTime time;
	time.UniversalTime();

	// we open the file outside the loop
	// to save some CPU
	BeginTransactionLC();
	for (TInt i=count-1; i>=0; i--)
		{
		TSAREntry entry=iEntryArray[i];
		if (((time>(entry.Time()+aTimeIntervalMinutes)) && !aPurgeIncompleteOnly ))
			// TODO - flag
			// check the logic o the aPurgeIncompleteOnly flg
			// don't purge the store if the entry is complete
			// entry.IsComplete()  )
			{
			DoDeleteEntryL(i);
			}
		}
	ExternalizeEntryArrayL();
	CommitTransactionL();
	} // CSARStore::PurgeL


/**
 *  Purges the segmentation store.
 *  
 *  This purges the filestore after a multipart message, deleting all the old
 *  entries.
 *  
 *  This function opens and closes the file automatically.
 *  
 *  @param aKSegmentationLifetimeMultiplier The coefficient for calculating whether
 *  a message should be purged
 *  @param aPurgeIncompleteOnly Set to false to purge all entries. Set to true
 *  to purge incomplete messages only.
 *  @capability None
 */
EXPORT_C void CSARStore::PurgeL(TInt aKSegmentationLifetimeMultiplier,TBool aPurgeIncompleteOnly)
	{
	LOGGSMU3("CSARStore::PurgeL(): aKSegmentationLifetimeMultiplier=%d, aPurgeIncompleteOnly=%d",
			 aKSegmentationLifetimeMultiplier, aPurgeIncompleteOnly);

	TInt count=Entries().Count();
	LOGGSMU2("CSARStore::PurgeL(): count=%d", count);
	TTime time;
	time.UniversalTime();

	// we open the file outside the loop
	// to save some CPU
	BeginTransactionLC();
	TInt numDeletedEntry = 0;
	
	for (TInt i=count-1; i>=0; i--)
		{
		const TSmsSegmentationEntry& entry = (const TSmsSegmentationEntry&)Entries()[i];
		const TTimeIntervalMinutes lifetime((entry.ValidityPeriod()*aKSegmentationLifetimeMultiplier)/1000);
		//
		// AEH: Defect fix for EDNPAHN-4WADW3 'Unreliable logging'
		//
		//      This should solve the problem with agressive PurgeL algorithm
		//      deleting PDUs which are waiting for Status Reports.
		//      Extra bit in TSAREntry indicating Status Reports.
		//      if we don't expect a status report, fake the
		//      number of delivered and failed to be equal
		//      to Total.
		//

		const TBool have_sr = entry.ExpectStatusReport();

		const TInt deliv_n_failed = have_sr
		  ? entry.Delivered() + entry.Failed() // total acked PDUs
		  : entry.Total();                     // fake

		if (
			(entry.IsComplete() && (deliv_n_failed >= entry.Total()))
			||
			((time>(entry.Time()+lifetime)) && !aPurgeIncompleteOnly )
			)
			{
			DoDeleteEntryL(i);
			++numDeletedEntry;
			}
		}
	
	TInt numOldestEntry = 0;
	if(iMaxmumNumberOfMessagesInSegmentationStore > 0)
		{
		while((count - numDeletedEntry) > iMaxmumNumberOfMessagesInSegmentationStore)
			{ // The number of messages in the store is greater than the allowed number. Find&Delete the oldest one.
			  // If the maximum number of messages is set to o, we don't delete any messages.
			while (Entries()[numOldestEntry].IsDeleted())
				{ // Find oldest entry that has not been deleted already
				++numOldestEntry;
				}
			DoDeleteEntryL(numOldestEntry++);
			++numDeletedEntry;
			}
		}
	    
	ExternalizeEntryArrayL();
	CommitTransactionL();
	LOGGSMU1("CSmsSegmentationStore::PurgeL End");
	} // CSARStore::PurgeL


/**
 *  Deletes an entry from the entry array, and externalizes it.
 *  
 *  The file store must be open before calling this function.
 *  
 *  @param aIndex Entry in the SAR store to delete
 *  @capability None
 */
EXPORT_C void CSARStore::DeleteEntryL(TInt aIndex)
	{
	LOGGSMU2("CSARStore::DeleteEntryL(): aIndex=%d", aIndex);

	DoDeleteEntryL(aIndex);
	ExternalizeEntryArrayL();
	} // CSARStore::DeleteEntryL


/**
 *  Constructor, initialises class members only.
 *  
 *  @param aFs File system handle
 *  @capability None
 */
EXPORT_C CSARStore::CSARStore(RFs& aFs)
    :iFs(aFs)
    ,iEntryArray(8)
    {
    } // CSARStore::CSARStore


/**
 *  Destructor. Frees resource.
 *  @capability None
 */
EXPORT_C CSARStore::~CSARStore()
    {
	delete iFileStore;
    } // CSARStore::CSARStore


/**
 *  Gets a reference to the filestore.
 *  
 *  @return The filestore
 *  @capability None
 */
EXPORT_C CFileStore& CSARStore::FileStore()
	{
	LOGGSMU1("CSARStore::FileStore()");

	__ASSERT_DEBUG(iFileStore!=NULL,Panic(KGsmuPanicSARStoreNotOpen));
	return *iFileStore;
	} // CSARStore::FileStore


/**
 *  Gets a (const) reference to the filestore.
 *  
 *  @return The filestore
 *  @capability None
 */
EXPORT_C const CFileStore& CSARStore::FileStore() const
    {
    // Ignore in code coverage - not used in SMS stack.
    BULLSEYE_OFF
    LOGGSMU1("CSARStore::FileStore()");
    __ASSERT_DEBUG(iFileStore!=NULL,Panic(KGsmuPanicSARStoreNotOpen));
    return *iFileStore;
    BULLSEYE_RESTORE
    }

/**
 *  Adds a new entry to the file store.
 *  
 *  The function appends the new entry to the internal entry array, and then externalises
 *  it.
 *  
 *  The file store must be open before calling this function.
 *  
 *  @param aEntry SAR entry to add
 *  @capability None
 */
EXPORT_C void CSARStore::AddEntryL(const TSAREntry& aEntry)
	{
	LOGGSMU1("CSARStore::AddEntryL()");

	__ASSERT_DEBUG(iFileStore!=NULL,Panic(KGsmuPanicSARStoreNotOpen));
	__ASSERT_DEBUG(aEntry.DataStreamId()!=KNullStreamId,Panic(KGsmuPanicSAREntryDataStreamIdNotSet));
	iEntryArray.AppendL(aEntry);
	iEntryArray[iEntryArray.Count()-1].SetIsAdded(ETrue);
	ExternalizeEntryArrayL();
	} // CSARStore::AddEntryL


/**
 *  Changes an entry in the file store - changes it first in the internal
 *  entry array, and then externalizes it.
 *  
 *  The file store must be open before calling this function.
 *  
 *  @param aIndex The index into the SAR store to be changed
 *  @param aNewEntry The new SAR store entry
 *  @capability None
 */
EXPORT_C void CSARStore::ChangeEntryL(TInt aIndex,const TSAREntry& aNewEntry)
	{
	LOGGSMU2("CSARStore::ChangeEntryL(): aIndex=%d", aIndex);

	__ASSERT_DEBUG(iFileStore!=NULL,Panic(KGsmuPanicSARStoreNotOpen));
	__ASSERT_DEBUG(iEntryArray[aIndex].DataStreamId()==aNewEntry.DataStreamId(),Panic(KGsmuPanicSAREntryDataStreamIdChanged));

	iEntryArray[aIndex].SetIsDeleted(ETrue);
	iEntryArray.InsertL(aIndex,aNewEntry);
	iEntryArray[aIndex].SetIsAdded(ETrue);

	ExternalizeEntryArrayL();
	} // CSARStore::ChangeEntryL


/**
 *  Gets the extra stream ID.
 *  
 *  @return Extra stream ID
 *  @capability None
 */
EXPORT_C TStreamId CSARStore::ExtraStreamId() const
	{
	LOGGSMU1("CSARStore::ExtraStreamId");
	return iExtraStreamId;
	} // CSARStore::ExtraStreamId


/**
 *  Sets the extra stream ID - changes it first in the internal
 *  entry array, and then externalizes it.
 *  
 *  You must call Close() when the function returns.
 *  
 *  @param aExtraStreamId Extra stream ID
 *  @capability None
 */
EXPORT_C void CSARStore::SetExtraStreamIdL(const TStreamId& aExtraStreamId)
	{
	LOGGSMU2("CSARStore::SetExtraStreamIdL(): id=%d", aExtraStreamId.Value());

	__ASSERT_DEBUG(iFileStore!=NULL,Panic(KGsmuPanicSARStoreNotOpen));
	TStreamId streamid=iExtraStreamId;
	iExtraStreamId=aExtraStreamId;
	TRAPD(ret, ExternalizeEntryArrayL());
	if (ret!=KErrNone)
		{
		LOGGSMU2("WARNING! CSARStore::DoExternalizeEntryArrayL left with %d", ret);
		iExtraStreamId=streamid;	//  Roll back
		User::Leave(ret);			//  re-leave to allow caller to also roll back
		}
	} // CSARStore::SetExtraStreamIdL


/**
 *  Compacts the file store.
 *  
 *  This is done on every reboot and for every eighth SMS message sent, to keep
 *  the size of the file as low as possible.
 *  
 *  This function opens and closes the file automatically.
 *  @capability None
 */
EXPORT_C void CSARStore::CompactL()
	{
    LOGGSMU1("CSARStore::CompactL Start");
	__ASSERT_DEBUG(iFileStore!=NULL, Panic(KGsmuPanicSARStoreNotOpen));
	__ASSERT_DEBUG(iInTransaction, Panic(KGsmuPanicSARStoreTransaction));

	TInt space = iFileStore->CompactL();
	iFileStore->CommitL();

    LOGGSMU2("CSARStore::CompactL End [space=%d]", space);
	(void)space;
	} // CSARStore::CompactL


/**
 *  Sets the SAR store as in-transaction.
 *  
 *  The function checks the validity of the call and leaves KErrAccessDenied if
 *  invalid.
 *  @capability None
 */
EXPORT_C void CSARStore::BeginTransactionLC()
	{
    LOGGSMU4("CSARStore::BeginTransactionLC [this=0x%08X iInTransaction=%d iFileStore=0x%08X]", this, iInTransaction, iFileStore);

	if (iFileStore == NULL || iInTransaction)
		{
	    LOGGSMU1("WARNING CSARStore::BeginTransactionLC leaving with KErrAccessDenied");
		User::Leave(KErrAccessDenied);
		}

	TCleanupItem sarClose(CSARStoreCloseObject, this);
	CleanupStack::PushL(sarClose);
	iInTransaction = ETrue;
	} // CSARStore::BeginTransactionLC

void CSARStore::Revert()
	{
    LOGGSMU3("CSARStore::Revert(): this=0x%08X, iInTransaction=%d",
    		 this, iInTransaction);

	__ASSERT_DEBUG(iInTransaction, Panic(KGsmuPanicSARStoreTransaction));

	iFileStore->Revert();
	iInTransaction = EFalse;
	ReinstateDeletedEntries();
	} // CSARStore::Revert


/**
 *  Opens a filestore.
 *  
 *  This is called by OpenL(), and does the work of really opening the file. If
 *  the file is not there, it creates a new one.
 *  @capability None
 */
EXPORT_C void CSARStore::DoOpenL()
	{
#ifdef _SMS_LOGGING_ENABLED
	TBuf8<80> buf8;
	buf8.Copy(iFullName);
	LOGGSMU3("CSARStore::DoOpenL(): '%S' this=0x%08X", &buf8, this);
#endif

	TUidType uidtype(KPermanentFileStoreLayoutUid,KSARStoreUid,iThirdUid);
	TEntry entry;
	TInt ret=iFs.Entry(iFullName,entry);  //  Check file exists
	if (ret==KErrNone)  //  File found
		{
		if (uidtype!=entry.iType)
			ret=KErrNotFound;
		}
	if (ret==KErrNone)  //  Filestore found
		{
		// using OpenLC puts it on the cleanup stack,
		// so that the trap handler will close the file
		// automatically
		TRAP(ret,(iFileStore=CPermanentFileStore::OpenL(iFs,iFullName,EFileShareExclusive|EFileStream|EFileRead|EFileWrite)));
		if(ret != KErrNone)
			{
			LOGGSMU2("WARNING! CPermanentFileStore::OpenLC left with %d", ret);
			}
		}

	if (ret==KErrNoMemory)  //  Filestore not corrupted
		{
		User::Leave(ret);
		}
	else if (ret != KErrNone)  //  The filestore was corrupted or not found, so create a new one
		{
		// create a new file and push the close function on the cleanup stack,
		// so that the trap handler will close the file automatically
#ifdef _SMS_LOGGING_ENABLED
		TBuf8<80> buf8;
		buf8.Copy(iFullName);
		LOGGSMU2("CSARStore::DoOpenL(): New file created '%S'", &buf8);
#endif
		TInt kerr(iFs.MkDirAll(iFullName)); //the directory may not exist, So create one.
		if(kerr != KErrAlreadyExists)
			{
			User::LeaveIfError(kerr);
			}
		iFileStore=CPermanentFileStore::ReplaceL(iFs, iFullName,EFileShareExclusive|EFileStream|EFileRead|EFileWrite);
		iFileStore->SetTypeL(uidtype);
		// the file got lost so we externalize the internal
		// representation of the data before we continue
		BeginTransactionLC();
		ExternalizeEntryArrayL();
		CommitTransactionL();
		}

	__ASSERT_DEBUG(iFileStore!=NULL, Panic(KGsmuPanicSARStoreNotOpen));
	} // CSARStore::DoOpenL


/**
 *  Actually delete an entry in the entry array
 */
void CSARStore::DoDeleteEntryL(TInt aIndex)
	{
#ifdef _SMS_LOGGING_ENABLED
	const TSmsSegmentationEntry& entry = (const TSmsSegmentationEntry&)iEntryArray[aIndex];

	LOGGSMU3("CSARStore::DoDeleteEntryL [aIndex=%d Count=%d]",
			 aIndex, iEntryArray.Count());
	LOGGSMU4("CSARStore::DoDeleteEntryL [aIndex=%d Delivered=%d Failed=%d]",
			 aIndex, entry.Delivered(), entry.Failed());
	LOGGSMU4("CSARStore::DoDeleteEntryL [aIndex=%d Count=%d Total=%d]",
			 aIndex, entry.Count(), entry.Total());
	LOGGSMU4("CSARStore::DoDeleteEntryL [aIndex=%d logId=%d StreamId=%d]",
			 aIndex, entry.LogServerId(), entry.DataStreamId().Value());
#endif // _SMS_LOGGING_ENABLED

	__ASSERT_DEBUG(iFileStore!=NULL,Panic(KGsmuPanicSARStoreNotOpen));
	TRAPD(err, iFileStore->DeleteL(iEntryArray[aIndex].DataStreamId()));
	if(err == KErrNone)
	    {
        iEntryArray[aIndex].SetIsDeleted(ETrue);
  	    }
	} // CSARStore::DoDeleteEntryL


/**
 *  internalize - read from the file store into RAM
 */
void CSARStore::InternalizeEntryArrayL()
	{
	LOGGSMU1("CSARStore::InternalizeEntryArrayL()");

	__ASSERT_DEBUG(iFileStore!=NULL, Panic(KGsmuPanicSARStoreNotOpen));

	TStreamId headerid=iFileStore->Root();
	RStoreReadStream stream;
	stream.OpenLC(*iFileStore,headerid);
	TInt count=stream.ReadInt32L();
	for (TInt i=0; i<count; i++)
		{
		TSAREntry sarentry;
		stream >> sarentry;
		iEntryArray.AppendL(sarentry);
		}
	stream >> iExtraStreamId;
	CleanupStack::PopAndDestroy();  //  stream
	} // CSARStore::InternalizeEntryArrayL


void CSARStore::RemoveDeletedEntries()
	{
	LOGGSMU1("CSARStore::RemoveDeletedEntries()");

	TInt count=iEntryArray.Count();
	while (count--)
		{
		TSAREntry& entry = iEntryArray[count];

		if (entry.IsDeleted())
			iEntryArray.Delete(count);
		else
			entry.SetIsAdded(EFalse);
		}
	} // CSARStore::RemoveDeletedEntries


void CSARStore::ReinstateDeletedEntries()
	{
	LOGGSMU1("CSARStore::ReinstateDeletedEntries()");

	TInt count=iEntryArray.Count();
	while (count--)
		{
		TSAREntry& entry = iEntryArray[count];

		if (entry.IsAdded())
			iEntryArray.Delete(count);
		else
			entry.SetIsDeleted(EFalse);
		}
	} // CSARStore::ReinstateDeletedEntries


/**
 *  externalize - read from RAM to the file store
 */
void CSARStore::ExternalizeEntryArrayL()
	{
	__ASSERT_DEBUG(iFileStore!=NULL, Panic(KGsmuPanicSARStoreNotOpen));
	__ASSERT_DEBUG(iInTransaction, Panic(KGsmuPanicSARStoreTransaction));

	LOGGSMU4("CSARStore::ExternalizeEntryArrayL(): this=0x%08X count=%d headerid=%d]",
			 this, iEntryArray.Count(), iFileStore->Root().Value());

	TStreamId headerid=iFileStore->Root();
	RStoreWriteStream stream;
	if (headerid==KNullStreamId)
		{
		headerid=stream.CreateLC(*iFileStore);
		iFileStore->SetRootL(headerid);
		}
	else
		{
		stream.ReplaceLC(*iFileStore,headerid);
		}

	TInt count1=iEntryArray.Count();
	TInt count2=0;
	TInt i=0;

	for (; i<count1; i++)
		{
		if (!iEntryArray[i].IsDeleted())
			{
			count2++;
			}
		}
	stream.WriteInt32L(count2);
	for (i=0; i<count1; i++)
		{
		if (!iEntryArray[i].IsDeleted())
			{
			stream << iEntryArray[i];
			}
		}

	stream << iExtraStreamId;
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);
	} // CSARStore::ExternalizeEntryArrayL


/**
 *  called by CloseFile - always do a CommitL
 *  and if the store is of a certains size then
 *  call CompactL also
 */
void CSARStore::DoCommitAndCompactL()
	{
	LOGGSMU1("CSARStore::DoCommitAndCompactL()");

	LOGGSMUTIMESTAMP();
	iFileStore->CommitL();
	LOGGSMUTIMESTAMP();

	iCommitCount--;
	if (iCommitCount < 0)
		{
		iCommitCount = KNumStoreCommitsBeforeCompaction;
		iFileStore->CompactL();
		iFileStore->CommitL();
		}
	} // CSARStore::DoCommitAndCompactL


/**
 *  Returns the private path used to open a store.
 *  
 *  @param aPath The private path of a store.
 *  @capability None
 */
EXPORT_C void CSARStore::PrivatePath(TDes& aPath)
	{
	LOGGSMU1("CSARStore::PrivatePath()");

	TDriveUnit driveUnit(KStoreDrive);
	TDriveName drive=driveUnit.Name();
	aPath.Insert(0, drive);
	//append private path
	TPath privatePath;
	iFs.PrivatePath(privatePath);
	aPath.Append(privatePath);
	aPath.Append(KStoreSubDir);
	} // CSARStore::PrivatePath
