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
// Tier Manager Mesh Activities
//   (management plane)
//

/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERTIERMANAGERACTIVITIES_H
#define MBUFGOBBLERTIERMANAGERACTIVITIES_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <comms-infras/ss_corepractivities.h>


// CLASS DECLARATION

namespace MbufGobblerTMActivities
{


enum 
	{
	/*
	 * Add new actviity id's here. I think they must be offset from ECFActivityCustom.
	 */
	
	/*
	 * This is just an example activity
	 */
	EMbufGobblerTMSomeActivity = ESock::ECFActivityCustom + 1,
	};

DECLARE_ACTIVITY_MAP(MbufGobblerTMActivitiesMap)
}

#endif // MBUFGOBBLERTIERMANAGERACTIVITIES_H
