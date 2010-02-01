// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Common header for pdp FSM
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef PDPFSMNMSPACE_H
#define PDPFSMNMSPACE_H

namespace PdpFsm
{

/** completion and notification signals */
enum TEtelSignal
	{
	EPhoneOpened,
	EPhoneOpenedFailed,
	
	EQoSSet,
	EQoSSetFailed,
	ETftSet,
	ETftSetFailed,
	ETftChanged,
	ETftChangedFailed,
	
	E1ryPdpContextCreated,
	E1ryPdpContextCreatedFailed,
	E2ryPdpContextCreated,
	E2ryPdpContextCreatedFailed,
	
	EPdpContextModified,
	EPdpContextModifiedFailed,
	EPdpActivated,
	EPdpActivatedFailed,
	EContextDeleted,
	EContextDeletedFailed,
	// add new signals here
	
	// network signals
	EQoSProfileChangeNetwork,
	EConfigGPRSChangeNetwork,
	EContextStatusChangeNetwork,
	EServiceStatusChangeNetwork,
	EPacketStatusChangeNetwork,
	// add new signals here
	
	ESentinelSignal
	};

} // namespace PdpFsm

	
#endif // PDPFSMNMSPACE_H
