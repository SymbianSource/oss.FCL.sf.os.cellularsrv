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
// Contents : for routine for saving/restoring fax header information
// amended 26/10/98  -  replaced sizeof(TRawScanLine) with KFaxBytesPerScanLine
// 
//

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

#include "CFAXIO.H"		//public header

_LIT(KHeaderFile, "FAXHEAD.DAT");
const TDriveNumber KDriveNumber = EDriveC;

/********************************************************************/

EXPORT_C CFaxHeaderLines *CFaxHeaderLines::NewLC ()
/** Constructs a CFaxHeaderLines object, which is used to read and write the fax 
header line data file.

As is usual in Symbian OS, the only difference between this function and NewL() 
is that this variant pushes the object to the cleanup stack.

As part of the construction process, the object opens a session with the file 
server.

@leave KErrNoMemory There is insufficient memory to perform the operation. 
@return Pointer to the newly created object. 
@capability None
*/
   {
   CFaxHeaderLines *self = new (ELeave) CFaxHeaderLines;
   CleanupStack::PushL (self);
   self->ConstructL ();
   return self;
   }
/********************************************************************/

EXPORT_C CFaxHeaderLines *CFaxHeaderLines::NewL ()
/** Constructs a CFaxHeaderLines object, which is used to read and write the fax 
header line data file.

As part of the construction process, the object opens a session with the file 
server.

@leave KErrNoMemory There is insufficient memory to perform the operation. 
@return A pointer to the newly created object. 
@capability None
*/
   {
   CFaxHeaderLines *self = NewLC ();
   CleanupStack::Pop ();
   return self;
   }
/********************************************************************/
/**
Default constructor
*/
CFaxHeaderLines::CFaxHeaderLines()
: iAdoptedHeaderFile(NULL), iUseAdpotedFileHandle(EFalse)
{
//Empty	
}

/**
Overloaded constructor
*/
CFaxHeaderLines::CFaxHeaderLines(RFile* aHeaderFile)
: iAdoptedHeaderFile(aHeaderFile), iUseAdpotedFileHandle(ETrue)
{
//Empty	
}

void CFaxHeaderLines::ConstructL ()
   {
   if(!iUseAdpotedFileHandle)   
	   	{	
	   	User::LeaveIfError (iFileSession.Connect ());
	   	}
   }
/********************************************************************/

CFaxHeaderLines::~CFaxHeaderLines ()
/** Closes the open header line data file and shuts down the file server session. */
   {
   if(!iUseAdpotedFileHandle)
   	{
   	iFile.Close ();
    iFileSession.Close ();
   	}
   
   }
/********************************************************************/

EXPORT_C void CFaxHeaderLines::WriteRawFontLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine)
/** Writes header line font bitmap scan lines to the header line data file. 

It should be called to add every scan line in the font bitmap.

@param alineNumber The line number of the current scan line. 
@param aUncompressedDataLine A reference to a raw font bitmap scan line to 
be added to the header line data file. 
@capability None
*/
   {
   iSeekpos=sizeof (TFaxHeaderInfo);
   iSeekpos+=KFaxBytesPerScanLine*iOurFaxHeaderInfoPckg().iHeaderFontHeightInLines;
   iSeekpos+=KFaxBytesPerScanLine*alineNumber;
   User::LeaveIfError (File().Seek (ESeekStart, iSeekpos));
   User::LeaveIfError (File().Write (aUncompressedDataLine, KFaxBytesPerScanLine));		
   }
/********************************************************************/

EXPORT_C void CFaxHeaderLines::WriteRawHeaderLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine)
/** Writes the header line template's scan lines to the header line data file. 
It should be called to add every scan line in the template.

@param alineNumber The line number of the current scan line. 
@param aUncompressedDataLine A reference to a raw header line template scan 
line to be added to the header line data file. 
@capability None
*/
   {
   iSeekpos=sizeof (TFaxHeaderInfo);
   iSeekpos+=KFaxBytesPerScanLine*alineNumber;
   User::LeaveIfError (File().Seek (ESeekStart, iSeekpos));
   User::LeaveIfError (File().Write (aUncompressedDataLine, KFaxBytesPerScanLine));
   }
/********************************************************************/

EXPORT_C void CFaxHeaderLines::WriteFaxHeaderInfoL (TFaxHeaderInfo & aFaxHeaderInfo)
/** Creates and opens the fax header data file, and then writes font and character 
offset information to it.

The font and character offset information is used by the fax server to determine 
at which position the font bitmap characters should be inserted in the header 
line template to create the send-time header line for a page.

Since this function creates and opens the file, it should be called before 
the other write functions.

@param aFaxHeaderInfo The fax header line information to be written to the 
file. 
@capability None
*/
   {
   iOurFaxHeaderInfoPckg() = aFaxHeaderInfo;
   iSeekpos=0;
   if(!iUseAdpotedFileHandle)
	   {
	   TFileName headerFileName;
   	   GenerateHeaderPathL(headerFileName);
	   User::LeaveIfError (iFile.Replace (iFileSession, headerFileName, EFileWrite));
	   }
   User::LeaveIfError (File().Seek (ESeekStart, iSeekpos));
   User::LeaveIfError (File().Write (iOurFaxHeaderInfoPckg, sizeof (TFaxHeaderInfo)));
   }
/********************************************************************/

EXPORT_C void CFaxHeaderLines::ReadRawFontLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine)
/** Reads the font bitmap's scan lines from the header line data file. 

It should be called to read every scan line in the bitmap.

In normal operation the function is called by the fax server prior to sending 
a page.

@param alineNumber The line number to be read. 
@param aUncompressedDataLine On return, contains a reference to the raw scan 
line. 
@capability None
*/
   {
   iSeekpos=sizeof (TFaxHeaderInfo);
   iSeekpos+=KFaxBytesPerScanLine*iOurFaxHeaderInfoPckg().iHeaderFontHeightInLines;
   iSeekpos+=KFaxBytesPerScanLine*alineNumber;
   User::LeaveIfError (File().Seek (ESeekStart, iSeekpos));
   User::LeaveIfError (File().Read (aUncompressedDataLine, KFaxBytesPerScanLine));
} 
/********************************************************************/

EXPORT_C void CFaxHeaderLines::ReadRawHeaderLineL (const TInt alineNumber,TRawScanLine & aUncompressedDataLine)
/** Reads the header line template's scan lines from the header line data file. 
It should be called to read every scan line in the template.

In normal operation the function is called by the fax server prior to sending 
a page.

@param alineNumber The line number of the scan line to be read. 
@param aUncompressedDataLine On return, contains the scan line. 
@capability None
*/
   {
   iSeekpos=sizeof (TFaxHeaderInfo);
   iSeekpos+=KFaxBytesPerScanLine*alineNumber;
   User::LeaveIfError (File().Seek (ESeekStart, iSeekpos));
   User::LeaveIfError (File().Read (aUncompressedDataLine, KFaxBytesPerScanLine));
   }
/********************************************************************/

EXPORT_C void CFaxHeaderLines::ReadFaxHeaderInfoL (TFaxHeaderInfo & aFaxHeaderInfo)
/** Opens the fax header data file, and then reads font and character offset information 
from it.

The font and character offset information is used by the fax server to determine 
at which position the font bitmap characters should be inserted in the header 
line template to create the send time header line for a page.

Since this function opens the file, it should be called before the other read 
functions.

@param aFaxHeaderInfo On return, contains header line information from the 
header data file. 
@capability None
*/
   {
   iSeekpos=0;
   if(!iUseAdpotedFileHandle)
	   {
	   TFileName headerFileName;
   	   GenerateHeaderPathL(headerFileName);
   	   User::LeaveIfError (iFile.Open (iFileSession, headerFileName, EFileRead));
	   }
   User::LeaveIfError (File().Seek (ESeekStart, iSeekpos));
   User::LeaveIfError (File().Read (iOurFaxHeaderInfoPckg, sizeof (TFaxHeaderInfo)));
   aFaxHeaderInfo = iOurFaxHeaderInfoPckg();
   }
   
/**
Constructs a CFaxHeaderLines object, which is used to read and write the fax 
header line data file.

This overload allows an already open file handle to be passed in which is used to access the file.  
This function is not intended for public use.

@internalTechnology
@param aHeaderFile Pointer to file handle.
@return Pointer to the newly created object.
@capability None
@released
*/
EXPORT_C CFaxHeaderLines* CFaxHeaderLines::NewLC (RFile* aHeaderFile)
	{
	CFaxHeaderLines *self = new (ELeave) CFaxHeaderLines(aHeaderFile);
    CleanupStack::PushL (self);
    self->ConstructL ();
    return self;
	}
		
/**
Generates the header path to place the faxhead.dat file.  
If platform security is enforced this path will be the private path of the client process.  As this directory structure may not exist 
This function will generate the directory structure if it does not exist.
If platform security is not enforced this path will be c:\system\...


@internalTechnology
@param aHeaderPath contains the generated private path
@capability None
@released
*/
EXPORT_C void CFaxHeaderLines::GeneratePathForHeaderFileL(TDes& aHeaderPath)
	{
	GenerateHeaderPathL(aHeaderPath);
	}

/**
Return reference to open file handle.
*/
inline RFile& CFaxHeaderLines::File()
	{
	if(iUseAdpotedFileHandle)
		{
		return *iAdoptedHeaderFile;
		}
	else
		{
		return iFile;
		}
	}

/**
Generates the header path to place the faxhead.dat file.
*/
void CFaxHeaderLines::GenerateHeaderPathL(TDes& aHeaderPath)
	{	
	TDriveUnit driveUnit(KDriveNumber);
	TDriveName drive=driveUnit.Name();
	aHeaderPath.Insert(0, drive);
	TPath headerPath;
	//append private path	
	RFs rfs;
	User::LeaveIfError(rfs.Connect());
	CleanupClosePushL(rfs);	
	rfs.PrivatePath(headerPath);		
	//generate directory structure.
	TInt ret = rfs.CreatePrivatePath(driveUnit);
	if(ret != KErrNone && ret!=KErrAlreadyExists)
		{
		User::Leave(ret);
		}
	CleanupStack::PopAndDestroy();	//rfs

	aHeaderPath.Append(headerPath);
	aHeaderPath.Append(KHeaderFile);
	}

/********************************************************************/

