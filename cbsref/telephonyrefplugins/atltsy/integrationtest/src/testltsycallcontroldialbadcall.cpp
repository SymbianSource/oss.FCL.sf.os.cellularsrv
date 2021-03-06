// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// 

//user include
#include "testltsycallcontroldialbadcall.h"

CTestLtsyCallControlDialBadCall::CTestLtsyCallControlDialBadCall(CTestLtsyModel& aModel)
	: CTestLtsyCallControlBase(aModel)
	{
	
	}

CTestLtsyCallControlDialBadCall::~CTestLtsyCallControlDialBadCall()
	{
	
	}

TVerdict CTestLtsyCallControlDialBadCall::doTestStepL()
	{
	INFO_PRINTF1(_L("starting CTestLtsyCallControlDialBadCall::doTestStepL()"));
	
	if(TestStepResult() == EPass)
		{
		//Open Call
		TRAPD(err,OpenCallOneL());
		if (err != KErrNone)
			{
			SetTestStepResult(EFail);
			INFO_PRINTF2(_L("The error was %d when opne call two"),err);
			return TestStepResult();
			}
				
		TRAP(err, CallOneDialBadNumber());
		if (err == KErrNone)
			{
			SetTestStepResult(EFail);
			INFO_PRINTF2(_L("The error was %d when dial bad telephone number"),err);
			return TestStepResult();			
			}
						
		//Close call
		CloseCallOne();
		}
	
	return TestStepResult();
	}

//End of file
