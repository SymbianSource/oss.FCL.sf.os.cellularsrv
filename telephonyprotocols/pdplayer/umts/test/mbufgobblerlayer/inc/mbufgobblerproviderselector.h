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

#ifndef MBUFGOBBLERPROVIDERSELECTOR_H
#define MBUFGOBBLERPROVIDERSELECTOR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <comms-infras/simpleselectorbase.h>

// CLASS DECLARATION

/**
 *  CMbufGobblerProviderSelector
 *  
 *  This is basically copied from other selector examples I culd find. Specifically I have used the
 *  CBtGenericProviderSelector as a template.
 * 
 */
class CMbufGobblerProviderSelector : public CBase, private ASimpleSelectorBase
	{
public:
	static ESock::MProviderSelector* NewL(const Meta::SMetaData& aSelectionPreferences);
	
private:
	CMbufGobblerProviderSelector(const Meta::SMetaData& aSelectionPreferences);
	~CMbufGobblerProviderSelector();
	
	// From MProviderSelector
	virtual void SelectL(ESock::ISelectionNotify& aSelectionNotify);
	virtual void Destroy();
	
	ESock::CMetaConnectionProviderBase* FindOrCreateProviderL(TUint aAccessPoint);
	
private:
	ESock::TSelectionPrefs iSelectionPreferences;
	TUint32 iTier;
	};


#endif // MBUFGOBBLERPROVIDERSELECTOR_H
