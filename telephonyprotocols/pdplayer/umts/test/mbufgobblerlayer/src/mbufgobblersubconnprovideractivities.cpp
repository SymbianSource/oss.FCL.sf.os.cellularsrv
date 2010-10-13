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
// Sub Connection Provider Mesh Activities
//  (control plane)
//

/**
 @file
 @internalComponent
*/

#include "mbufgobblersubconnprovideractivities.h"
#include "mbufgobblerlog.h"

//#include <comms-infras/ss_corepractivities.h>
#include <comms-infras/corescpractivities.h>



namespace MbufGobblerSCprActivities
{
DEFINE_ACTIVITY_MAP(MbufGobblerSCprActivitiesMap)
	//no activities defiend here yet. This define is to show where to put activities
	//so for now just inherit from core activities
ACTIVITY_MAP_END_BASE(SCprActivities, coreSCprActivities)
}
