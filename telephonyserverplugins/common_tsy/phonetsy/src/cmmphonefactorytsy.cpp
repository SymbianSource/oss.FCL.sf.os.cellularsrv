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

#include "cmmphonefactorytsy.h"
#include "cmmphonetsy.h"
#include <ctsy/mmtsy_names.h>
#include "MmTsy_conf.h"
#include "cmmmessagemanagerbase.h"
#include <ctsy/serviceapi/ctsysatmessagingbase.h>

#ifdef USING_CTSY_DISPATCHER
_LIT(KLicenseeTsyDllName, "licenseetsy.dll");
_LIT(KCtsyDispatcherDllName, "ctsydispatcher.dll");

const TInt KLicenseeTsyUID3 = 0x2000BEE4;
const TInt KCtsyDispatcherUID3 =  0x10285C38;

typedef MLtsyFactoryBase*(*TFactoryBaseNewL)();

#endif //USING_CTSY_DISPATCHER
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
#ifdef USING_CTSY_DISPATCHER
    iLoadedLib.Close();
#endif //USING_CTSY_DISPATCHER
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
        if ( messageManager )
            {
	        CleanupStack::PushL( messageManager );
        
            // LTSY Plug-in API successfully created
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL - Starting to open LicenceeTSY");
            
            // get TSY message manager callback object
            MmMessageManagerCallback* callBack = 
                messageManager->GetMessageManagerCallback();
            
#ifdef USING_CTSY_DISPATCHER
            iLtsyFactory = LoadLibraryL();
#else
            // library entry for LTSY, get base factory object
            iLtsyFactory = LTsyFactoryL();
            
#endif //USING_CTSY_DISPATCHER

            if( iLtsyFactory )
                {
                // Get message router from LTSY. All current interface versions
                // have the same GetMessageRouter defined in version 1.
                MLtsyFactoryV1* ptr_v1 = 
                	static_cast<MLtsyFactoryV1*>( iLtsyFactory );
                        
                iMessageRouter = ptr_v1->GetMessageRouter( *callBack );
		        User::LeaveIfNull( iMessageRouter );
        
		        // Licencee Tsy successfully created
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL -  LicenceeTSY successfully opened");

	            // set the pointer to the message router object
	            messageManager->SetMessageRouter( iMessageRouter );

	            // Ownership of messageManager passed to iPhoneTsy.
                CleanupStack::Pop( messageManager );
	            // Create Phone Tsy (which creates the whole Common TSY)
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL - Starting to open CommonTSY");
	            iPhoneTsy = CMmPhoneTsy::NewL( 
	                messageManager, this, iLtsyFactory );

	            if ( iPhoneTsy )
	                {
TFLOGSTRING("TSY: CMmPhoneFactoryTsy::NewL - CommonTSY successfully opened");
	                messageManager->SetPhoneTsy( iPhoneTsy );
	                }
                }
            else
            	{
    	        CleanupStack::PopAndDestroy( messageManager );
            	}
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

        if( iLtsyFactory )
            {
            // Get SIM ATK TSY messaging service from LicenseeTsy
            MLtsyFactoryV1* ptr_v1 = 
            	static_cast<MLtsyFactoryV1*>( iLtsyFactory );
     
#ifdef USING_CTSY_DISPATCHER     	
           	satMessaging = ptr_v1->GetSatMessagingService( 
            	iPhoneTsy->SmsService() );
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

MLtsyFactoryBase* CMmPhoneFactoryTsy::LoadLibraryL()
	{
	//load library.  This can either be LicenseeTsy or CtsyDispatcher dlls
	TPtrC dllname;
	TInt uid3_int;
	if(UsingCtsyDispatcher())
		{
		dllname.Set(KCtsyDispatcherDllName);
		uid3_int = KCtsyDispatcherUID3;
		}
	else
		{
		dllname.Set(KLicenseeTsyDllName);
		uid3_int = KLicenseeTsyUID3;
		}
	
	TFLOGSTRING3("TSY: CMmPhoneFactoryTsy::LoadLibraryL - Loading Dll=%S, UID3=0x%x", &dllname, uid3_int);
	
	TUidType uid(KNullUid, KNullUid, TUid::Uid(uid3_int));
	
	User::LeaveIfError(iLoadedLib.Load(dllname));
	
	// Check the Uid3 is as expected
	if(iLoadedLib.Type()[2]!=TUid::Uid(uid3_int))
		User::Leave(KErrBadLibraryEntryPoint);
	
	TFactoryBaseNewL libEntry=(TFactoryBaseNewL)iLoadedLib.Lookup(1);	
	MLtsyFactoryBase* factory=(*libEntry)();	// libEntry may leave.
	
	return factory;
	}
#endif //USING_CTSY_DISPATCHER

//  End of File


