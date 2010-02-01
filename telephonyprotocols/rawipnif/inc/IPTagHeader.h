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
// Defines the Class that adds and extracts specific header tags.
// 
//

/**
 @file
*/

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS

#ifndef IPTAGHEADER_H__
#define IPTAGHEADER_H__

#include "bttlog.h"
#include "Constants.h"
#include <nifmbuf.h>

class CBttLogger;

class CIPTagHeader : public CBase
	{
public:
	CIPTagHeader(CBttLogger* aTheLogger);
	~CIPTagHeader();
	void SetType(TUint16 aType);	
	void AddHeader(TDes8& aDes);
	TUint16 RemoveHeader(RMBufChain& aPdu);

private: // Unowned data.
	CBttLogger* iTheLogger;

	//CLASSNAMEDECL(CIPTagHeader);

	TBuf8<KIPTagHeaderLength> iHeaderByte; // Owned
	};

#endif // IPTAGHEADER_H__

#endif // RAWIP_HEADER_APPENDED_TO_PACKETS
