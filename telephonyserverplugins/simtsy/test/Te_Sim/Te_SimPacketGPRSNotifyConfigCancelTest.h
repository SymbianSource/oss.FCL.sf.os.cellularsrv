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

#ifndef __TE_SIM_PACKET_GPRSNOTIFYCONFIGCANCEL_TEST__
#define __TE_SIM_PACKET_GPRSNOTIFYCONFIGCANCEL_TEST__

#include "Te_SimTestStepBase.h"
#include "Te_SimConstants.h"

class CSimPacketGPRSNotifyConfigCancelTest : public CPhBkAndPacketTestsTestStepBase
	{
public:
	CSimPacketGPRSNotifyConfigCancelTest();
 	virtual TVerdict doTestStepL();

private:
	RPacketContext iGPRSPacketContext;
	RPacketService iGPRSPacketService;
	};

#endif // __TE_SIM_PACKET_GPRSNOTIFYCONFIGCANCEL_TEST__
