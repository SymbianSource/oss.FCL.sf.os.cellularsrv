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

#include "mbufgobblertiermanageractivities.h"
#include "mbufgobblerlog.h"

#include <comms-infras/ss_activities.h>
#include <elements/mm_activities.h>

//#include <comms-infras/ss_corepractivities.h>
#include <comms-infras/coretiermanageractivities.h>



namespace MbufGobblerTMActivities
{
DEFINE_ACTIVITY_MAP(MbufGobblerTMActivitiesMap)
	//no activities defiend here yet. This define is to show where to put activities
	//so for now just inherit from core activities
ACTIVITY_MAP_END_BASE(TMActivities, coreTMActivities)
}
