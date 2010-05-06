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
// Implements the adding and removal of tag headers.
// 
//

/**
 @file
*/

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS

#include "IPTagHeader.h"
#include "Constants.h"

/**
Default constructor. 
*/
CIPTagHeader::CIPTagHeader(CBttLogger* aTheLogger)
	: iTheLogger(aTheLogger)
	{
	iHeaderByte.SetMax();
	iHeaderByte.FillZ();
	}

/**
Default destructor.
*/
CIPTagHeader::~CIPTagHeader()
	{
	}

/**
This method will set the type of the IP header.

@param aType IP header type
*/
void CIPTagHeader::SetType(TUint16 aType)
 	{
 	_LOG_L1C1(_L8("CIPTagHeader::SetType"));
 	
 	iHeaderByte[0] = (TUint8)(aType>>8); 
 	iHeaderByte[1] = (TUint8)(aType&0xff);	
	}

/**
This method will add a two-byte header to the send buffer detailing the
protocol the aDes data packet is encoded with; in this case an IPv4
header tag is appended.

@param aPdu The data packet to be sent
*/
void CIPTagHeader::AddHeader(RCommsBufChain& aPdu)
	{
	_LOG_L1C1(_L8("CIPTagHeader::AddHeader"));

	// Add the protocol code as a header to the buffer
	aPdu.Write(iHeaderByte,0);	
	}

/**
This method will remove the two-byte header from the received data,
which specifies whether it is an IPv4/IPv6 packet.  This information
will be stored in protocolCode and returned to the caller.

@param aPdu The received data packet
@return The protocol header tag stripped from aPdu
*/
TUint16 CIPTagHeader::RemoveHeader(RCommsBufChain& aPdu)
	{
	_LOG_L1C1(_L8("CIPTagHeader::RemoveHeader"));

	if (aPdu.Length() > KIPTagHeaderLength)
		{
		//Strip off the first part of the header
		aPdu.TrimStart(1);
		//Read the remaining part of the header as this contains the protocol code
		TUint8* payloadPtr = aPdu.First()->Ptr();
		TUint16 protocolCode = static_cast<TUint16>(*payloadPtr);
		//Strip off the remaining protocol code part of the header

		aPdu.TrimStart(1);

		return protocolCode;
		}

	return 0;
	}

#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
