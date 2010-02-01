// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// PDP Default SubConnection Provider class definition.
// 
//

/**
 @file
 @internalComponent
*/

#ifndef SYMBIAN_PDPDEFTSCPR_H
#define SYMBIAN_PDPDEFTSCPR_H

#include <comms-infras/ss_subconnprov.h>
#include <comms-infras/corescpr.h>
#include <comms-infras/corescprstates.h>
#include "PDPSCPR.h"
#include "PDPSCPRStates.h"

namespace PDPSCprStates
    {
    class TCreatePrimaryPDPCtx;
	class TRetrieveSipAddr;
    }

enum PDPSCprCustomActivities
	{
	ECFActivityIoctl = ESock::ECFActivityCustom
	};
    
class CPDPSubConnectionProviderFactory;    
class CPDPDefaultSubConnectionProvider : public CPDPSubConnectionProvider
/** Default PDP subconnection provider

@internalTechnology
@released Since 9.4 */
    {
    friend class CPDPSubConnectionProviderFactory;
    friend class PDPSCprStates::TCreatePrimaryPDPCtx;
	friend class PDPSCprStates::TRetrieveSipAddr;
public:
    typedef CPDPSubConnectionProviderFactory FactoryType;
	virtual ~CPDPDefaultSubConnectionProvider();
	
	//PdpFsm Callbacks
    void PdpFsmAllContextEvent(TInt aNotification, TInt aParam);
    
    virtual void LinkUp();
    virtual void LinkDown(TInt aCause);

	void SendDataClientIdleIfNoSubconnsAndNoClientsL();
	
protected:
    //-====================================
    //Construction methods bundle - accessible thru the factory only
    //-====================================
    CPDPDefaultSubConnectionProvider(ESock::CSubConnectionProviderFactoryBase& aFactory);
	static CPDPDefaultSubConnectionProvider* NewL(CPDPSubConnectionProviderFactory& aFactory);
	void ConstructL();

    TInt iLinkDowns;
    };

    
#endif //SYMBIAN_PDPDEFTSCPR_H
