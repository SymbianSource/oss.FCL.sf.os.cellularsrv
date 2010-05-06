// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// INCLUDE FILES
//

#include <featmgr/featurecontrol.h>
#include <featureuids.h>

#include <ctsy/mmtsy_names.h>
#include <ctsy/serviceapi/ctsysatmessagingbase.h>

#include "cmmmessagemanagerbase.h"
#include "cmmphonefactorytsy.h"
#include "cmmphonetsy.h"
#include "MmTsy_conf.h"

_LIT(KLtsyIniFile, "ltsydata.ini");

#ifdef USING_CTSY_DISPATCHER
_LIT(KDefaultLicenseeTsyDllName, "ctsydispatcher.dll");
const TInt KDefaultLicenseeTsyUID3 =  0x10285C38;
#else
_LIT(KDefaultLicenseeTsyDllName, "licenseetsy.dll");
const TInt KDefaultLicenseeTsyUID3 = 0x2000BEE4;
#endif //USING_CTSY_DISPATCHER

typedef MLtsyFactoryBase*(*TFactoryBaseNewL)();

// ======== MEMBER FUNCTIONS ========

CMmPhoneFactoryTsy::CMmPhoneFactoryTsy()
    {
    // Version numbers defined in ETEL.h
    iVersion = TVersion( KEtelMajorVersionNumber,
        KEtelMinorVersionNumber,
        KEtelBuildVersionNumber );
    }

CMmPhoneFactoryTsy* CMmPhoneFactoryTsy::NewL()
    {
    CMmPhoneFactoryTsy* phoneFactoryTsy = NULL;
    phoneFactoryTsy = new ( ELeave ) CMmPhoneFactoryTsy();
    CleanupStack::PushL( phoneFactoryTsy );
    phoneFactoryTsy->ConstructL();   
    CleanupStack::Pop();
    return phoneFactoryTsy;
    }    

void CMmPhoneFactoryTsy::ConstructL()
    {
    }

CMmPhoneFactoryTsy::~CMmPhoneFactoryTsy()
    {
    iLoadedLib.Close();
    }

// ---------------------------------------------------------------------------
// CMmPhoneFactoryTsy::NewPhoneL
// Creates a new phone object instance
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPhoneBase* CMmPhoneFactoryTsy::NewPhoneL( const TDesC& aName )
    {
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL - NEW LOG");

    iPhoneTsy = NULL;

    // In case of debug build, print flag info.
#ifdef TF_LOGGING_ENABLED
    
    RFeatureControl featureControl;
    TInt err = featureControl.Open();
    if (err != KErrNone)
        {
TFLOGSTRING("CMmPhoneFactoryTsy::NewPhoneL - failed to connect to FeatMgr");
        }
    else if (featureControl.FeatureSupported(NFeature::KCsVideoTelephony) == KFeatureSupported) 
        {
TFLOGSTRING("TSY: __CS_VIDEO_TELEPHONY -flag is on");
        }
    else if (featureControl.FeatureSupported(NFeature::KEmergencyCallsEnabledInOfflineMode) == KFeatureSupported)
        {
TFLOGSTRING("TSY: __COMMON_TSY__EMERGENCY_CALLS_ENABLED_IN_OFFLINE_MODE -flag is on");
        }

    featureControl.Close();
#endif
    
    // Check if the Phone Name is OK
    if ( KErrNone == aName.CompareF( KMmTsyPhoneName ) )
        {
        iMessageRouter = NULL;

        // Create Message Manager (the 'core' of the LTSY Plug-in API)
        CMmMessageManagerBase* messageManager = CMmMessageManagerBase::NewL();
        User::LeaveIfNull(messageManager);
        CleanupStack::PushL( messageManager );
        
        // LTSY Plug-in API successfully created
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL - Starting to open LicenceeTSY");
        
        // get TSY message manager callback object
        MmMessageManagerCallback* callBack = 
            messageManager->GetMessageManagerCallback();
        
        // Dynamic loading of LTSY
        MLtsyFactoryBase* ltsyFactory = LoadLibraryL();
        
        // Get message router from LTSY. All current interface versions
        // have the same GetMessageRouter defined in version 1.
        MLtsyFactoryV1* ptr_v1 = 
            static_cast<MLtsyFactoryV1*>( ltsyFactory );
                
        iMessageRouter = ptr_v1->GetMessageRouter( *callBack );
        User::LeaveIfNull( iMessageRouter );

        // Licencee Tsy successfully created
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL -  LicenceeTSY successfully opened");

        // set the pointer to the message router object
        messageManager->SetMessageRouter( iMessageRouter );

        // Ownership of messageManager to be passed to iPhoneTsy.
        CleanupStack::Pop( messageManager );
        
        // Create Phone Tsy (which creates the whole Common TSY)
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL - Starting to open CommonTSY");
        iPhoneTsy = CMmPhoneTsy::NewL(messageManager, this, ltsyFactory);

        if ( iPhoneTsy )
            {
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL - CommonTSY successfully opened");
            messageManager->SetPhoneTsy( iPhoneTsy );
            }
        else
            {
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL - CommonTSY not opened");
            }
        }

    return iPhoneTsy;
    }

// ---------------------------------------------------------------------------
// CMmPhoneFactoryTsy::EnumeratePhones
// Returns the maximum number of supported phone instances
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmPhoneFactoryTsy::EnumeratePhones()
    {
    return KMmTsyNumOfSupportedPhones;
    }

// ---------------------------------------------------------------------------
// CMmPhoneFactoryTsy::GetPhoneInfo
// Index of the requested phone is passed to the client
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMmPhoneFactoryTsy::GetPhoneInfo(
    const TInt aIndex,
    RTelServer::TPhoneInfo& aInfo )
    {
    TInt ret( KErrNotFound );
    
    if( aIndex == 0 )
        {
        aInfo.iName.Copy( TPtrC( KMmTsyPhoneName ) );
        aInfo.iNetworkType = KMmTsyNetworkType;
        aInfo.iNumberOfLines = KMmTsyNumOfSupportedLines;
        //(TAN) Symbian's comment to iExtension parameter:
        //The best solution is not to worry about this iExtensions field too
        //much and to just return one API extension value, and that being the
        //main API supported by the TSY. If the TSY also supports e.g. GPRS
        //and/or SAT then the client can find this out using the
        //RTelServer::IsSupportedByModule method.
        aInfo.iExtensions = KETelExtMultimodeV1;

        // Phone found. Change ret value.
        ret = KErrNone;
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// CMmPhoneFactoryTsy::IsSupported
// Provides information about the supported functional units of
// the TSY. This method directs the request to all supported protocol
// extensions. If functional unit is supported by any of those protocol
// extension the result will be ETrue. Otherwise the method will 
// return KErrNone
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CMmPhoneFactoryTsy::IsSupported( const TInt aFunctionalUnit )
    {
    TBool ret = EFalse;

    switch ( aFunctionalUnit )
        {
        //Supported functional units of mmtsy
        case KETelFuncMobileIccAccess:
        case KETelFuncMobileNetwork:
        case KETelFuncMobileIdentity:
        case KETelFuncMobilePower:
        case KETelFuncMobileSignal:
        case KETelFuncMobileDTMF:
        case KETelFuncMobileUserNetworkAccess:
        case KETelFuncMobileIdentityService:
        case KETelFuncMobileCallForwarding:
        case KETelFuncMobileCallBarring:
        case KETelFuncMobileCallWaiting:
        case KETelFuncMobileCallCompletion:
        case KETelFuncMobileSecurity:
        case KETelFuncMobileAlternateLineService:
        case KETelFuncMobileFixedDiallingNumbers:
        case KETelFuncMobileDataCall:
        case KETelFuncMobileEmergencyCall:
        case KETelFuncMobileSmsMessaging:
        case KETelFuncMobileBroadcastMessaging:
        case KETelFuncMobileUssdMessaging:
        case KETelFuncMobileConferenceCall:
        case KETelFuncMobilePhonebook:
        case KETelFuncMobileSmsStore:
        case KETelFuncMobileOwnNumberStore:
        case KETelFuncMobileEmergencyNumberStore:
        case KETelFuncMobilePrivacy:
        case KETelExtMultimodeV1:
        case KETelExtMultimodeV2:
        case KETelExtMultimodeV3:
        case KEtelFuncMobileAPNControlList:
        case KEtelExtMultimodeV9:
        case KEtelFuncCellInfo:
            ret = ETrue;
            break;

        //Unsupported functional units
        //case KETelFuncMobileAlternatingCall:
        //case KETelFuncMobileMessageWaiting:
        //case KETelFuncMobileNamStore:
        //case KETelFuncMobileMultimediaCall:
        //case KEtelFuncMobileMultimediaCallSettings:
        //case KETelFuncMobileCost:

        default:
            ret = EFalse;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CMmPhoneFactoryTsy::NewSimAtk
// Creates CTsySatMessaging object and returns it
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CTsySatMessagingBase* CMmPhoneFactoryTsy::NewSimAtk( const TDesC& aName )
    {
    CTsySatMessagingBase* satMessaging = NULL;
    
    if ( KErrNone == aName.CompareF( KSatToolkit ) )
        {
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewSimAtk - Starting to open SimAtkTSY");

        // Dynamic loading of LTSY
        MLtsyFactoryBase* ltsyFactory = NULL;
        TRAPD(ret, ltsyFactory = LoadLibraryL());
        if (ret == KErrNone)
            {
            // Get SIM ATK TSY messaging service from LicenseeTsy
            MLtsyFactoryV1* ptr_v1 = static_cast<MLtsyFactoryV1*>( ltsyFactory );
     
#ifdef USING_CTSY_DISPATCHER        
            satMessaging = ptr_v1->GetSatMessagingService(iPhoneTsy->SmsService());
#else
            satMessaging = ptr_v1->GetSatMessagingService( );
#endif
            }        
        }

    return satMessaging;
    }

// ---------------------------------------------------------------------------
// extern "C"
// The LibEntry function is implemented in another programming
// language (ASM?). extern "C" is required to port from other language
// ---------------------------------------------------------------------------
//
extern "C"
    {
    IMPORT_C CPhoneFactoryBase* LibEntry(); // Force "Proper Name" export
    }

// ---------------------------------------------------------------------------
// LibEntry
// A Windows Entry Procedure. The LibEntry() procedure in the
// DLL C run-time libraries performs all the necessary initialization
// required by the WEPs (Windows Entry Point) they contain. This code is
// copied from Symbian's example
// Returns: CPhoneFactoryBase*: Pointer to CMmPhoneFactoryTsy class
// ---------------------------------------------------------------------------
//
EXPORT_C CPhoneFactoryBase* LibEntry()
    {
TFLOGSTRING("TSY: CPhoneFactoryBase::LibEntry()...");
    // return NULL if instantiation fails
    CMmPhoneFactoryTsy* factory(NULL);
    TRAP_IGNORE(factory = CMmPhoneFactoryTsy::NewL()); 
    return factory; 
    }

#ifdef USING_CTSY_DISPATCHER
TBool CMmPhoneFactoryTsy::UsingCtsyDispatcher()
    {
    //function to determine whether the setup is using the CTSY Dispatcher layer.
#ifdef USING_CTSY_DISPATCHER 
    return ETrue; 
#else
    return EFalse;
#endif
    }
#endif //USING_CTSY_DISPATCHER

// ---------------------------------------------------------------------------
// CMmPhoneFactoryTsy::LoadLibraryL
// Dynamically loads a LTSY.
// ---------------------------------------------------------------------------
//
MLtsyFactoryBase* CMmPhoneFactoryTsy::LoadLibraryL()
    {
    TPtrC ltsyDllName(0, NULL);
    TUint ltsyDllUid = 0;
    
#ifdef USING_CTSY_DISPATCHER 
    ltsyDllName.Set(KDefaultLicenseeTsyDllName);
    ltsyDllUid = KDefaultLicenseeTsyUID3; 
#else
    RBuf configData;
    CleanupClosePushL(configData);
    ReadConfigFileL(&configData);
    
    TRAPD(ret,
          {
          ltsyDllName.Set(ReadDllNameFromConfigL(configData));
          ltsyDllUid = ReadDllUidFromConfigL(configData);
          });
    
    if (ret != KErrNone)
        {
        ltsyDllName.Set(KDefaultLicenseeTsyDllName);
        ltsyDllUid = KDefaultLicenseeTsyUID3;
        TFLOGSTRING2("TSY: Failed to load LTSY dll details from ini (error: %d), using defaults", ret);
        }
#endif
    
    TFLOGSTRING3("TSY: CMmPhoneFactoryTsy::LoadLibraryL - Loading Dll=%S, UID3=0x%x", &ltsyDllName, ltsyDllUid);
    User::LeaveIfError(iLoadedLib.Load(ltsyDllName));
    
#ifndef USING_CTSY_DISPATCHER
    CleanupStack::PopAndDestroy();  // configData
#endif
    
    // Check the Uid3 is as expected
    if (iLoadedLib.Type()[2] != TUid::Uid(ltsyDllUid))
        {
        User::Leave(KErrBadLibraryEntryPoint);
        }
    
    TFactoryBaseNewL libEntry = REINTERPRET_CAST(TFactoryBaseNewL, iLoadedLib.Lookup(1));   
    MLtsyFactoryBase* factory = (*libEntry)();    // libEntry may leave.
    
    User::LeaveIfNull(factory);
    TFLOGSTRING("TSY: Loaded LTSY");
    return factory;
    }

TPtrC CMmPhoneFactoryTsy::ReadDllNameFromConfigL(const TDesC& aConfigData)
    {
    _LIT(KLicenseeTsyDllKeyName, "LicenseeTsyDllName");
    TPtrC dllName = GetValueForKeyL(aConfigData, KLicenseeTsyDllKeyName);
    TFLOGSTRING2("TSY: Config file: LTSY DLL name = %S", &dllName);
    return dllName;
    }

TUint CMmPhoneFactoryTsy::ReadDllUidFromConfigL(const TDesC& aConfigData)
    {
    _LIT(KLicenseeTsyUid3, "LicenseeTsyUid3");

    const TDesC& rawUid = GetValueForKeyL(aConfigData, KLicenseeTsyUid3);
    // only take the right 8 characters (ie discard the "0x")
    ASSERT(rawUid.Length() >= 8);
    TLex lex(rawUid.Right(8));   
    TUint uid;
    User::LeaveIfError(lex.Val(uid, EHex));
    TFLOGSTRING2("TSY: Config file: LTSY UID3 = 0x%x", uid);
    return uid;
    }

/*static*/
void CMmPhoneFactoryTsy::ReadConfigFileL(RBuf* aConfigData)
/**
 * Reads config file from system drive or, if not present, from ROM
 *
 * @param aConfigData This buffer gets created and has the ini file contents loaded into it
 */
    {
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    fs.SetSessionToPrivate(fs.GetSystemDrive());

    RFile configFile;
    TFLOGSTRING2("TSY: Reading ini file %S", &KLtsyIniFile);
    TInt ret = configFile.Open(fs, KLtsyIniFile, EFileShareExclusive);
    if (ret == KErrNotFound)
        {
        User::LeaveIfError(fs.SetSessionToPrivate(EDriveZ));
        User::LeaveIfError(configFile.Open(fs, KLtsyIniFile, EFileShareExclusive));
        }
    CleanupClosePushL(configFile);

    TInt configFileSize;
    User::LeaveIfError(configFile.Size(configFileSize));

    RBuf8 fileBuf;
    fileBuf.Create(configFileSize);
    CleanupClosePushL(fileBuf);
    User::LeaveIfError(configFile.Read(fileBuf));

    aConfigData->CreateL(configFileSize);
    aConfigData->Copy(fileBuf);

    CleanupStack::PopAndDestroy(3, &fs); // fs, configFile, fileBuf
    }

/*static*/
TPtrC CMmPhoneFactoryTsy::GetValueForKeyL(const TDesC& aKeysValues, const TDesC& aKey)
/**
 * Gets the value for a specified key. The expected format is
 * <pre>
 * key1 value1
 * key2 value2
 * </pre>
 * 
 * However the parsing is flexible enough to allow "key1 =value1" as it allows an equals separator and extra whitespace
 * The value cannot include whitespace, even if quoted.
 */
    {
    // this matches the bracket expression in this regexp: \w+\s*=?\s*(\w+)[\s$]
    TInt startOfKey = aKeysValues.Find(aKey);
    TLex valueExtractor(aKeysValues);
    valueExtractor.Inc(startOfKey + aKey.Length());
    valueExtractor.SkipSpaceAndMark();
    // allow equals separating key and value
    if (valueExtractor.Peek() == '=')
        {
        valueExtractor.Inc();
        valueExtractor.SkipSpaceAndMark();
        }
    
    TPtrC value = valueExtractor.NextToken();
    __ASSERT_ALWAYS(value.Length() > 0, User::Leave(KErrCorrupt));
    return value;
    }
