// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
// MbufGobbler selector. Used by the Tier Manager to select the access point below
//  (management plane)
//

/**
 @file
 @internalComponent
*/

#include "mbufgobblerproviderselector.h"
#include "mbufgobblerlog.h"

//extra includes 
#include <ss_select.h> //needed as declares MProviderSelector. Why is it in epoc32/ and not epoc32/comms-infras with all the other headers?
#include <metadatabase.h> //needed for CommsDat::CMDBSession
#include <ss_tiermanagerutils.h> //needed for ESock::TierManagerUtils
#include <commsdattypesv1_1_partner.h> //needed for CCDTierRecord (iTierRecord)

#include "mbufgobbler_panic.h" //needed for ASSERT_DEBUG
#include "mbufgobblertiermanagerfactory.h" //needed for CMbufGobblerTierManagerFactory::iUid
#include "mbufgobblermetaconnproviderfactory.h" //needed for CMbufGobblerMetaConnectionProviderFactory::iUid


ESock::MProviderSelector* CMbufGobblerProviderSelector::NewL(const Meta::SMetaData& aSelectionPreferences)
	{
	//LOG_STATIC_FUNC
	ASSERT_DEBUG(aSelectionPreferences.IsTypeOf(ESock::TSelectionPrefs::TypeId()), EUnexpectedTSelectionPrefsType);
	
	CMbufGobblerProviderSelector* self = NULL;
	self = new(ELeave) CMbufGobblerProviderSelector(aSelectionPreferences);
	CleanupStack::PushL(self);
	
	CommsDat::CMDBSession* dbs = CommsDat::CMDBSession::NewLC(KCDVersion1_2);
	
	self->iTierRecord = ESock::TierManagerUtils::LoadTierRecordL(TUid::Uid(CMbufGobblerTierManagerFactory::iUid), *dbs);
	
	ASSERT_DEBUG(static_cast<TUint32>(self->iTierRecord->iDefaultAccessPoint)!=0,EUnexpectedTSelectionPrefsType);
	
	CleanupStack::Pop(dbs);
	CleanupStack::Pop(self);
	ASSERT_DEBUG(self->iDbs == NULL, EInvalidNullPtr);
	self->iDbs = dbs;
	return self;
	}

CMbufGobblerProviderSelector::CMbufGobblerProviderSelector(const Meta::SMetaData& aSelectionPreferences)
: ASimpleSelectorBase(aSelectionPreferences)
	{
	}

CMbufGobblerProviderSelector::~CMbufGobblerProviderSelector()
	{
	}

void CMbufGobblerProviderSelector::Destroy()
	{
	delete this;
	}


/**
Find or create a Mbufgobbler Meta Connection Provider
*/
ESock::CMetaConnectionProviderBase* CMbufGobblerProviderSelector::FindOrCreateProviderL(TUint aAccessPoint)
	{
	return ASimpleSelectorBase::FindOrCreateProviderL(aAccessPoint); //use base ASimpleSelectorBase::FindOrCreateProviderL
	
	/*
	//Find factory
	TUid mcprUid(TUid::Uid(CMbufGobblerMetaConnectionProviderFactory::iUid));
	ESock::CMetaConnectionProviderFactoryBase* factory = static_cast<ESock::CMetaConnectionProviderFactoryBase*> (iMetaContainer.FindOrCreateFactoryL(mcprUid));
	
	// Create the provider.
	ESock::TProviderInfo providerInfo(TUid::Uid(CMbufGobblerTierManagerFactory::iUid), aAccessPoint);
	ESock::TMetaConnectionFactoryQuery query(providerInfo);
	
	ESock::CMetaConnectionProviderBase* provider = static_cast<ESock::CMetaConnectionProviderBase*> (factory->FindOrCreateObjectL(query));
	
	ASSERT_DEBUG(provider, EInvalidNullPtr);
	provider->IncrementBlockingDestroy();
	return provider;*/
	}


/**
Perform the selection of a Meta Connection Provider
*/
void CMbufGobblerProviderSelector::SelectL(ESock::ISelectionNotify& aSelectionNotify)
	{
	//LOG1(_L("CBtGenericProviderSelector %08x:\tSelectL()"), this);
	
	ASSERT_DEBUG(iDbs, EInvalidNullPtr);
	ASSERT_DEBUG(iTierRecord, EInvalidNullPtr);
	TUint32 defaultAccessPoint = iTierRecord->iDefaultAccessPoint;
	
	//Must be set
	User::LeaveIfError(defaultAccessPoint!=0 ? KErrNone : KErrCorrupt);
	
	// Send select complete with the provider, and the final select complete
	aSelectionNotify.SelectComplete(this, FindOrCreateProviderL(defaultAccessPoint));
	aSelectionNotify.SelectComplete(this, NULL);
	}
