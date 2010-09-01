// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contents : for saving/restoring persistent fax settings
// 
//

/**
 @file
*/

#ifndef __FAXSET_H
#define __FAXSET_H

#include <cdblen.h> 
#include <cdbcols.h>

#include <s32strm.h>
#include <f32file.h>

/********************************************************************/

/**
Fax configuration information.

Contains persistent information applicable to all fax sessions. 

@publishedPartner
@released
*/
class TFaxSettings
	{
public:
	IMPORT_C void ExternalizeL (RWriteStream &) const;
	IMPORT_C void InternalizeL (RReadStream &);
	IMPORT_C TFaxSettings& operator=(const TFaxSettings& aFaxSettings);
	/** The local Fax ID string, which must be exactly 20 characters, with only 0-9, 
	space, and + permissible for exact conformance with ITU T.30. */
	TBuf8 < 20 > iFaxId ;
	/** Maximum speed of fax session in bps. */
	TInt iMaxSpeed;
	/** Minimum speed of fax session in bps. */
	TInt iMinSpeed;
	/** Preferred resolution. */
	TFaxResolution iPreferredResolution;
	/** Preferred compression. */
	TFaxCompression iPreferredCompression;
	/** If 1, use error correction mode (ECM), else 0. */
	TInt iPreferredECM;
	/** Switches fax logging on or off. This must be left on. */
	TInt iVerbose;
	/** The delay between dialling and receiving from a faxback service (seconds). */
	TInt iFaxOnDemandDelay ;
	/** The fax class. */
	TFaxClass iFaxClass;
	};

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <faxsettings.h>
#endif

#endif
