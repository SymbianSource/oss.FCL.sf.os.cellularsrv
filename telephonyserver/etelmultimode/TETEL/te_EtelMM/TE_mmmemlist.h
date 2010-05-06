// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalComponent 
*/

#ifndef _TE_MMMEMLIST_H_
#define _TE_MMMEMLIST_H_

#include <etelmm.h>
#include <mmlist.h>


class CTestMemMMLists : public CEtelMMTestStepBase
	{
public:
	TInt iTestCount;
	CTestMemMMLists() ;
	~CTestMemMMLists(){} ;
	virtual enum TVerdict doTestStepL();
	};

#endif
