/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef CMMPHONEFACTORYTSY_H
#define CMMPHONEFACTORYTSY_H

#include <et_phone.h>
#include <etel.h>
#include <ctsy/mltsyfactorybase.h>
#include <ctsy/mltsyfactory.h>

// FORWARD DECLARATIONS
class CTsySatMessagingBase;
class CMmPhoneTsy;
class MLtsyFactoryBase;

// CLASS DECLARATION

/**
 * CMmPhoneFactoryTsy creates a new multimode phone object.
 */
NONSHARABLE_CLASS( CMmPhoneFactoryTsy ) : public CPhoneFactoryBase
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * return created phone object 
         */
        static CMmPhoneFactoryTsy* NewL();
        
        /**
         * C++ default constructor. LibEntry requires that this is public
         */
        CMmPhoneFactoryTsy();   
        
        /**
         * By default Symbian 2nd phase constructor.
         */
        void ConstructL();

        /**
         * Returns Phone instance
         */
        CPhoneBase* NewPhoneL( const TDesC& aName );

        /**
         * Returns SimAtk TSY instance
         */
        virtual CTsySatMessagingBase* NewSimAtk( const TDesC& aName );

        /**
         * Destructor.
         */
        ~CMmPhoneFactoryTsy();  

        // New functions

        /**
         * Returns maximum number of supported phone instances.
         * 
         *
         * @return Maximum number of supported phone instances.
         */
        TInt EnumeratePhones(); 

        /**
         * Returns phone information
         * 
         *
         * @param aIndex index number of the phone
         * @param aPhoneFactoryBaseInfo info copied here
         * @return Return value to the ETel Server
         */
        TInt GetPhoneInfo( 
            const TInt aIndex, 
            RTelServer::TPhoneInfo& aPhoneFactoryBaseInfo );

        /**
         * Provides information about the supported functional units of 
         * the TSY
         * 
         *
         * @param aFunctionalUnit is this functional unit supported.
         * @return TBool ETrue if supported, EFalse if not
         */
        TBool IsSupported( const TInt aFunctionalUnit );    
#ifdef USING_CTSY_DISPATCHER     
    protected:
    	
    	TBool UsingCtsyDispatcher();
    	MLtsyFactoryBase* LoadLibraryL();
#endif // #endif USING_CTSY_DISPATCHER
    private:    // Data
    
        /**
         * Pointer to PhoneTSY
         * Not owned.
         */
        CMmPhoneTsy* iPhoneTsy;

        /**
         * Pointer to messagerouter 
         * Not owned.
         */
        MMessageRouter* iMessageRouter;
                
        /**
         * Pointer to LTSY factory, used to retrieve LTSY and SIM ATK TSY
         * Not owned.
         */
        MLtsyFactoryBase* iLtsyFactory;
#ifdef USING_CTSY_DISPATCHER        
        RLibrary iLoadedLib; //can either be LicenseeTsy or CtsyDispatcher dlls
#endif // #ifdef USING_CTSY_DISPATCHER
    };

#endif // CMMPHONEFACTORYTSY_H

// End of File
