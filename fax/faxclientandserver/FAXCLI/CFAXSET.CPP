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

#include "CFAX32.H"
#include "faxsettings.h"

 #include <commsdattypesv1_1.h>
 
 using namespace CommsDat;

// amended August 1998 as follows :

// 1. TFaxSettings is defined, with its internalize and externalize functions,
 // in this module solely for use by the apps that use faxtrans.  They should
 // decide where the setting are stored and get or set them using the
 // functions here. A reference to the settings to use are passed to
 // faxtrans as parameters on instantiation of the CFaxTransfer object - eg
 //
 // CFaxTransfer * session =  CFaxTransfer::NewL (aFaxSettings);
 //
 // 2. A number of items previous part of TFaxSettings are now kept as part of
 // the modem definitions by Dialstor - these include both ModemInitString
 // and FaxInitString, together with the CSY module name and the comm port
 // name and number.  Most importantly, the fax modem class is now part of the
 // Dialstor data. A TDialstorModemPreferences object with this information in it is now
 // part of the TFaxSettings, but this need NOT be initialized in the
 // reference passed to CFaxTransfer on instantiation.  If the modem class
 // does happen to be unknown, then a special CFaxModemDriver is used which
 // does autodetection and nothing else.
 //
 // 3. As a conseqence, CFaxSettings no longer has Get or Set functions
 // for TFaxSettings.  Instead, we have a ValidateAndGetClass function to
 // validate the passed values in TFaxSettings (in case they are
 // nonsense) and get the values from DialStore.
 //

/********************************************************************/

EXPORT_C TFaxSettings& TFaxSettings::operator=(const TFaxSettings& aFaxSettings)
/** Assignment operator.

Copies the content of one TFaxSettings into another, replacing the existing 
content.

@param aFaxSettings A reference to the TFaxSettings to be copied. 
@return A reference to this TFaxSettings. 
@capability None
*/
	{
	iFaxId = aFaxSettings.iFaxId;
	iMaxSpeed = aFaxSettings.iMaxSpeed;
	iMinSpeed = aFaxSettings.iMinSpeed;
	iPreferredResolution = aFaxSettings.iPreferredResolution;
	iPreferredCompression = aFaxSettings.iPreferredCompression;
	iPreferredECM = aFaxSettings.iPreferredECM;
	iVerbose = aFaxSettings.iVerbose;
	iFaxOnDemandDelay = aFaxSettings.iFaxOnDemandDelay;
	iFaxClass = aFaxSettings.iFaxClass;
	return (*this);
	}

/********************************************************************/

EXPORT_C void TFaxSettings::ExternalizeL (RWriteStream & inifile) const
/** Externalises the TFaxSettings object to a write stream. The presence of this
function means that the standard templated operator<<() (defined in s32strm.h) is available
to externalise objects of this class.

@param inifile  The stream to which the object should be externalised. 
@capability None
*/
   {
   inifile << iFaxId;
   inifile.WriteInt32L (iMaxSpeed);
   inifile.WriteInt32L (iMinSpeed);
   inifile.WriteInt32L (iPreferredResolution);
   inifile.WriteInt32L (iPreferredCompression);
   inifile.WriteInt32L (iPreferredECM);
   inifile.WriteInt32L (iVerbose);
   inifile.WriteInt32L (iFaxOnDemandDelay);
   inifile.WriteInt32L (iFaxClass);
   }
/********************************************************************/

EXPORT_C void TFaxSettings::InternalizeL (RReadStream & inifile)
/** Internalises the TFaxSettings object from a read stream. The presence of this
function means that the standard templated operator>>() (defined in s32strm.h) is 
available to internalise objects of this class. This function may leave if there 
is a problem reading from the stream, or if internalisation causes an out of memory 
error. 
@param inifile  The stream from which the object should be internalised. 
@capability None
*/
   {
   inifile >> iFaxId;
   iMaxSpeed = inifile.ReadInt32L ();
   iMinSpeed = inifile.ReadInt32L ();
   iPreferredResolution = (TFaxResolution) inifile.ReadInt32L ();
   iPreferredCompression = (TFaxCompression) inifile.ReadInt32L ();
   iPreferredECM = inifile.ReadInt32L ();
   iVerbose = inifile.ReadInt32L ();
   iFaxOnDemandDelay = inifile.ReadInt32L ();
   iFaxClass = (TFaxClass) inifile.ReadInt32L ();
   }
/********************************************************************/

CFaxSettings *CFaxSettings::NewLC ()
   {
   CFaxSettings *self = new (ELeave) CFaxSettings;
   CleanupStack::PushL (self);
   self->ConstructL ();
   return self;
   }
/********************************************************************/

CFaxSettings *CFaxSettings::NewL ()
   {
   CFaxSettings *self = NewLC ();
   CleanupStack::Pop ();
   return self;
   }
/********************************************************************/

void CFaxSettings::ConstructL()
	{

	}
/********************************************************************/

CFaxSettings::~CFaxSettings ()
   {
   }
/********************************************************************/

void CFaxSettings::ValidateAndSetClassL (TFaxSettings * aFaxSettings)
	{
	TInt badspeed;

	TFaxClass passedFaxClass = aFaxSettings->iFaxClass;
	// first we check that the settings we are making are within range

	badspeed = aFaxSettings->iMaxSpeed % 2400;
	if (badspeed)
		aFaxSettings->iMaxSpeed -= badspeed;
	if ((aFaxSettings->iMaxSpeed > 14400) || (aFaxSettings->iMaxSpeed < 2400))
		aFaxSettings->iMaxSpeed = 9600;

	badspeed = aFaxSettings->iMinSpeed % 2400;
	if (badspeed)
		aFaxSettings->iMinSpeed -= badspeed;
	if ((aFaxSettings->iMinSpeed > 14400) || (aFaxSettings->iMinSpeed < 2400))
		aFaxSettings->iMinSpeed = 2400;

	if (aFaxSettings->iMinSpeed > aFaxSettings->iMaxSpeed)
		aFaxSettings->iMinSpeed = aFaxSettings->iMaxSpeed;

	if (aFaxSettings->iPreferredResolution != EFaxNormal)
		aFaxSettings->iPreferredResolution = (TFaxResolution)EFaxFine;

	if (aFaxSettings->iPreferredCompression != (TFaxCompression)EModifiedHuffman)
		aFaxSettings->iPreferredCompression = (TFaxCompression)EModifiedRead;

	// second we read the current modem class

	// AnnW, 9/8/99 - This all assumes that we are taking the modem settings from the 
	// dial out IAP, which is fince for now, but may not be in the future?  This may also 
	// need modifying for Linda?

	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
	CleanupStack::PushL(db);
	
	// Read the currently selected connection preference and find preferred IAP
	TInt prefRank  = 1;
	
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
	
	CMDBField<TUint32>* bearerField = new(ELeave) CMDBField<TUint32>(KCDTIdIAPBearer);
	CleanupStack::PushL(bearerField);
	bearerField->SetRecordId(iapId);
	bearerField->LoadL(*db);
	TUint32 modemId = *bearerField;
	CleanupStack::PopAndDestroy(bearerField);
		
	CMDBField<TUint32>* faxField = new(ELeave) CMDBField<TUint32>(KCDTIdFaxClassPref);
	CleanupStack::PushL(faxField);
	faxField->SetRecordId(modemId);
	faxField->LoadL(*db);
	TUint32 faxClass;
	faxClass = *faxField;
	aFaxSettings->iFaxClass = static_cast<TFaxClass>(faxClass);
		
	// finally we validate the modem class - if not class 1 or 2 or 2.0 we
	// write back the class we came in with, which is assumed to be correct
	// - this is how we update the Comms database  - other settings are ignored
	
	if ((aFaxSettings->iFaxClass != EClass1)
		&& (aFaxSettings->iFaxClass != EClass2)
		&& (aFaxSettings->iFaxClass != EClass2point0))
		{
		aFaxSettings->iFaxClass = passedFaxClass;
		db->OpenTransactionL();
		*faxField = aFaxSettings->iFaxClass;
		faxField->ModifyL(*db);
		db->CommitTransactionL();
		}
	CleanupStack::PopAndDestroy(faxField);
		
	CleanupStack::PopAndDestroy(2);		// db, connectionPrefs 
		
	}

/********************************************************************/
