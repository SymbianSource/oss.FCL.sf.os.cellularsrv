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
//

#ifndef __TE_SMSMONDSK_H_
#define __TE_SMSMONDSK_H_

#include "Te_SmsTestSteps.h"

/**
 
 */
class CSetDiskMonitorLimits : public CSMSTestSteps
	{
public:
	virtual enum TVerdict doTestStepL();	
	};

/**
 
 */
class CSetDiskSpace : public CSMSTestSteps
	{
public:
	virtual enum TVerdict doTestStepL();	
	};
	
/**
 	
 */
class CFreeDiskSpace : public CSMSTestSteps
	{
public:
	virtual enum TVerdict doTestStepL();
	};


#endif // __TE_SMSMONDSK_H_
