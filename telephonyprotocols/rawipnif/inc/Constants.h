// Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <e32base.h>
#include <etelqos.h>

const TUint KNifMajorVersionNumber=1;
const TUint KNifMinorVersionNumber=0;
const TUint KNifBuildVersionNumber=0;

// For CommDb usage
const TUint KSlashChar='\\';

// Constants for IPv4 protocol interface

_LIT8(KDescIp, "ip");
_LIT8(KDescIcmp, "icmp");

_LIT(KIPv4ProtocolIfName, "Bttint4");

// Constants for IPv6 protocol interface
_LIT8(KDescIp6, "ip6");
_LIT(KIPv6ProtocolIfName, "Bttint6");

_LIT(KNifName, "rawipnif");
enum TRawIPNifPanic
    {
	KNifUnknownInitState,
	KNifUnknownShutDownState,
	KNifProtocolInUse,
	/** Bca in unkonwn State */
	KBcaUnkownState,
	/** BCA not exist*/
	KBcaNotExist,
    /** BCA not exist*/
    KBcaAlreadyExists,
    /** invalid packet size on uplink */
    KFlowInvalidULPacketSize
    };


const TUint16 KIp4FrameType = 0x21;
const TUint16 KVanJacCompTCPIPFrameType = 0x2d;
const TUint16 KVanJacUnCompTCPIPFrameType = 0x2f;
const TUint16 KIp6FrameType = 0x57;

const TUint KDefaultSpeedMetric = 1;
// generally, the default MTU should be 1500 bytes but it has been 
// determined that for a number of real world use cases, 1500 bytes
// provides fairly poor performance, so we're limiting it 1400 here
// this can be overridden by patchable constant (iby file).
const TUint KDefaultMtu = 1400; 

const TUint KDataPacketMaxSize = 4096;

// BTT TSY contants
const TInt KTsyPhone = 0;

_LIT(KRawIPNifName, "RawIP");

// constants for diagnostic logging
_LIT8(KNifSubDir, "RawIp"); 
_LIT8(KRefFile, "RawIP"); 

// Max size of IP packet 
// generally, the default MTU should be 1500 bytes but it has been 
// determined that for a number of real world use cases, 1500 bytes
// provides fairly poor performance, so we're limiting it 1400 here
// this can be overridden by patchable constant (iby file).
const TUint KMaxIPPacket = 1400;
const TUint KIPTagHeaderLength = 2;
const TUint KMaxIPPacketAndHeader = KMaxIPPacket + KIPTagHeaderLength;

const TInt KIPTagHeaderByte1 = 0x00;
const TInt KIPTagHeaderByte2 = 0x21;

#endif	//CONSTANTS_H
