// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// te_spudNetSideQos.h
// Defines the Network-side events & error conditions tests for SPUD.
// 
//

/**
 @file
 @internalComponent
*/

#ifndef TE_SPUD_NETSIDE_QOS_H
#define TE_SPUD_NETSIDE_QOS_H

#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
#include <networking/qoslib.h>


/** Encapsulates all Spud Network Side tests */
TPtrC QoSEventToText(TQoSEvent aEvent);
#endif // SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY

#endif 
