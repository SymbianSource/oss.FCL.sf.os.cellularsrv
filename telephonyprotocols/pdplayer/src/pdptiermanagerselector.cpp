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
//

/**
 @file
 @internalComponent
*/

#include "pdptiermanagerselector.h"
#include "pdptiermanagerfactory.h"

#include <comms-infras/ss_log.h>
#include <commsdattypesv1_1.h>
#include <commsdattypesv1_1_partner.h>
#include <comms-infras/ss_tiermanagerutils.h>
#include <es_connpref.h>	//TConnIdList
#include <es_enum_internal.h>	

#ifdef __CFLOG_ACTIVE
#define KPdpTierMgrTag KESockTierTag
_LIT8(KPdpTierMgrSubTag, "pdptiermgr");
#endif // __CFLOG_ACTIVE

using namespace ESock;
using namespace CommsDat;


//Panic codes
_LIT(KPdpSelectorPanic, "PdpSelector");
enum TPdpSelectorPanic
	{
	EExpectedAccessPointAwareSystem = 0,     
	EUnExpectedSelectionPreferences = 1
    };

//
//CPdpProviderSelector
class CPdpProviderSelector : public CBase, public ASimpleSelectorBase
/** Link connection selector.
	Simple selector object for the link layer.

@internalComponent
@prototype
*/
	{
public:
	explicit CPdpProviderSelector(const Meta::SMetaData& aSelectionPreferences)
	:	ASimpleSelectorBase(aSelectionPreferences)
		{
		}

private:
	virtual void SelectL(ESock::ISelectionNotify& aSelectionNotify);
	};

void CPdpProviderSelector::SelectL(ISelectionNotify& aSelectionNotify)
	{
	ASSERT(iDbs);
	ASSERT(iTierRecord);
	TUint32 defaultAccessPoint = iTierRecord->iDefaultAccessPoint;

	//This selector deals only with the AccessPoint aware system!
	__ASSERT_DEBUG(defaultAccessPoint!=0,User::Panic(KPdpSelectorPanic,EExpectedAccessPointAwareSystem));

	if (iSelectionPrefs.IsEmpty())
		{
    	//Implicit case on the new setup
		__CFLOG_VAR((KPdpTierMgrTag, KPdpTierMgrSubTag, _L8("CPdpProviderSelector %08x::\tSelectL() Using Default AP:%d"),this,defaultAccessPoint));
		aSelectionNotify.SelectComplete(this,FindOrCreateProviderL(defaultAccessPoint));
		aSelectionNotify.SelectComplete(this,NULL);
		return;
		}

	const TConnPref& prefs = iSelectionPrefs.Prefs();
	if (prefs.ExtensionId() == TConnPref::EConnPrefProviderInfo)
		{
    	ASSERT(iSelectionPrefs.Scope() & TSelectionPrefs::ESelectFromExisting); //This is always attach
		const TConnProviderInfo& connProvInfo = static_cast<const TConnProviderInfoPref&>(prefs).Info();
		__CFLOG_VAR((KPdpTierMgrTag, KPdpTierMgrSubTag, _L8("CPdpProviderSelector %08x::\tSelectL() Using TConnProviderInfoPref, AP:%d"),this,connProvInfo.iInfo[1]));
		aSelectionNotify.SelectComplete(this,FindProviderL(connProvInfo.iInfo[1],(TAny*)connProvInfo.iInfo[2]));
		aSelectionNotify.SelectComplete(this,NULL);
		return;
		}

	if (prefs.ExtensionId() == TConnPref::EConnPrefSnap)
		{
		TUint accessPoint = static_cast<const TConnSnapPref&>(prefs).Snap();
		__CFLOG_VAR((KPdpTierMgrTag, KPdpTierMgrSubTag, _L8("CPdpProviderSelector %08x::\tSelectL() Using TConnPrefSnap, AP:%d"),this,accessPoint));
		aSelectionNotify.SelectComplete(this,FindOrCreateProviderL(accessPoint));
		aSelectionNotify.SelectComplete(this,NULL);
		return;
		}

	if (prefs.ExtensionId() == TConnPref::EConnPrefIdList)
		{
		__CFLOG_VAR((KPdpTierMgrTag, KPdpTierMgrSubTag, _L8("CPdpProviderSelector %08x::\tSelectL() Using TConnIdList"),this));
		const TConnIdList& list = static_cast<const TConnIdList&>(prefs);
		TInt count = list.Count();
		for (TInt i = 0; i < count; i++)
			{
			aSelectionNotify.SelectComplete(this,FindOrCreateProviderL(list.Get(i)));
			}
		aSelectionNotify.SelectComplete(this,NULL);
		return;
		}

	//In this selector we _must_ have the new preferences, otherwise it means that
	//a critical, non-recoverable mitsake has occured before when this selector has been picked.
	__CFLOG_VAR((KPdpTierMgrTag, KPdpTierMgrSubTag, _L8("ERROR: CPdpProviderSelector %08x::\tSelectL() Unexpected selection preferences"),this));
	User::Panic(KPdpSelectorPanic,EUnExpectedSelectionPreferences);
	}

//
// TPdpSelectorFactory::NewSelectorL - This fn matches a selector
MProviderSelector* TPdpSelectorFactory::NewSelectorL(const Meta::SMetaData& aSelectionPreferences)
	{
	__CFLOG_VAR((KPdpTierMgrTag, KPdpTierMgrSubTag, _L8("CPdpMetaCprSelectorBase::\tNewL()")));
	ASSERT(aSelectionPreferences.IsTypeOf(TSelectionPrefs::TypeId()));
	CMDBSession* dbs = CMDBSession::NewLC(KCDVersion1_2);
	CPdpProviderSelector* self = new (ELeave) CPdpProviderSelector(aSelectionPreferences);
	CleanupStack::PushL(self);
	ASSERT(self->iTierRecord==NULL);
	self->iTierRecord = TierManagerUtils::LoadTierRecordL(TUid::Uid(CPDPTierManagerFactory::EUid),*dbs);
	CleanupStack::Pop(self);
	CleanupStack::Pop(dbs);
	ASSERT(self->iDbs==NULL);
	self->iDbs = dbs;
	return self;
	}

