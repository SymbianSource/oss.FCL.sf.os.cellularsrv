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
// Defines the series of requests which a ESOCK client can send to the MBuf Gobbler using
// the publish and subscribe attribute EMBufGobbler.
//
// A test ESOCK client can deplete all the MBufs from the MBuf pool as follows:
//
//    TInt result = RProperty::Set(TUid::Uid(EMBufGobbler ), EAdjustNumberOfMBufsRemainingInPool , EGobbleAllMBufs);
//
// Issuing this command should cause a socket read or write request to fail due to MBuf pool exhaustion.
//
// Subsequently a test ESOCK client can return all the MBufs it has taken from the pool as follows:
//
//   result = RProperty::Set(TUid::Uid(EMBufGobbler), EAdjustNumberOfMBufsRemainingInPool, EReleaseAllMBufs);  
//
// After taking all the MBufs from the pool, an ESOCK test client can also return a single MBuf back to the pool as follows:
//
//   result = RProperty::Set(TUid::Uid(EMBufGobbler), EAdjustNumberOfMBufsRemainingInPool, EReleaseASingleMBuf);
//
/**
 @file
 @internalComponent
*/

#ifndef MBUFGOBBLERPUBSUB_H
#define MBUFGOBBLERPUBSUB_H




enum
{
EMBufGobbler = 0x10272F97
};

enum
{
EAdjustNumberOfMBufsRemainingInPool = 1
};    
    
enum
{
EGobbleAllMBufs,
EReleaseAllMBufs,
EReleaseASingleMBuf
};

#endif

