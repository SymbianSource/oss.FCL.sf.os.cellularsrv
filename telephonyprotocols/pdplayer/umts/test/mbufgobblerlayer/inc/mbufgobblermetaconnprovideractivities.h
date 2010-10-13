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
// Meta Connection Provider Mesh Activities
//  (management plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERMETACONNPROVIDERACTIVITIES_H
#define MBUFGOBBLERMETACONNPROVIDERACTIVITIES_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <comms-infras/ss_activities.h>
#include <elements/mm_activities.h>

// CLASS DECLARATION

namespace MbufGobblerMCprActivities
{
enum 
	{
	/*
	 * Add new actviity id's here. I think they must be offset from ECFActivityCustom.
	 */
	
	/*
	 * This is just an example activity
	 */
	EMbufGobblerMCprSomeActivity = ESock::ECFActivityCustom + 1,
	};

DECLARE_ACTIVITY_MAP(MbufGobblerMCprActivitiesMap)
}

#endif // MBUFGOBBLERMETACONNPROVIDERACTIVITIES_H
