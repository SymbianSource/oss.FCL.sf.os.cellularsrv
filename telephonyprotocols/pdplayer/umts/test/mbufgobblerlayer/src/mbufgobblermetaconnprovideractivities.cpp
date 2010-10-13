// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Symbian Foundation License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
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

#include "mbufgobblermetaconnprovideractivities.h"
#include "mbufgobblerlog.h"

#include <comms-infras/coremcpractivities.h>


namespace MbufGobblerMCprActivities
{
DEFINE_ACTIVITY_MAP(MbufGobblerMCprActivitiesMap)
	//no activities defiend here yet. This define is to show where to put activities
	//so for now just inherit from core activities
ACTIVITY_MAP_END_BASE(MCprActivities, coreMCprActivities)
}
