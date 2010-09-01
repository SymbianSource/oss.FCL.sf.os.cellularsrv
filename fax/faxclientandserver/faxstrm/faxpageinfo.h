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
 @internalTechnology
 @released
*/

#if !defined(__FAXPAGEINFO_H__)
#define __FAXPAGEINFO_H__

/********************************************************************/

/**
@internalTechnology
*/
class TFaxBandHeader
	{
public:
	IMPORT_C TFaxBandHeader();
	IMPORT_C TFaxBandHeader(TStreamId aStreamId);
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;

public:
	TInt iNumScanLines;
	TInt iNumBytes;
	TStreamId iStreamId;
	};

/**
@internalTechnology
*/
class CFaxPageInfo : public CBase
	{
protected:
	CFaxPageInfo();
public:
	IMPORT_C static CFaxPageInfo* NewL();
	IMPORT_C ~CFaxPageInfo();
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
public:
	TFaxResolution iResolution;
	TFaxCompression iCompression; // was TInt iReservedFlag1 up to faxstore 013
	TInt iReservedFlag2;
	TFaxBufSenderId iSenderId;
	CArrayFixFlat<TFaxBandHeader>* iBandHeaderList;
	};

/**
@internalTechnology
*/
class CFaxPages	: public CBase
	{
protected:
	CFaxPages();
public:
	IMPORT_C static CFaxPages* NewL();
	IMPORT_C ~CFaxPages();
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
public:
	CArrayFixFlat<TStreamId>* iPageStreamIdList;
	};

/********************************************************************/
#endif // __FAXPAGEINFO_H__
