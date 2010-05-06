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
// etel driver factory
// 
//

/**
 @file 
 @internalComponent
*/

#include <e32base.h>
#include <etelmm.h>

#include "cspudcontextelem.h"
#include "ceteldriverfactory.h"
#include "spudteldebuglogger.h"
#include "PDPFSM.h"
using namespace EtelDriver;

/**
@param aPdpFsmInterface - pdp fsm interface
*/
CEtelDriverFactory::CEtelDriverFactory (CPdpFsmInterface& aPdpFsmInterface)
: iPdpFsmInterface(aPdpFsmInterface),
  iServiceChangeNotifier(iPacketService, iPdpFsmInterface)
	{
	SPUDTELVERBOSE_INFO_LOG(_L("CEtelDriverFactory::CEtelDriverFactory()"));
	}

/**
@param aPdpFsmInterface - pdp fsm interface
*/
CEtelDriverFactory* CEtelDriverFactory::NewL (CPdpFsmInterface& aPdpFsmInterface)
	{
	SPUDTEL_INFO_LOG(_L("CEtelDriverFactory::NewL()"));
	
	return new (ELeave) CEtelDriverFactory (aPdpFsmInterface);
	}

CEtelDriverFactory::~CEtelDriverFactory()
	{
	SPUDTEL_FNLOG("CEtelDriverFactory::~CEtelDriverFactory()");
	SPUDTEL_INFO_LOG(_L("CEtelDriverFactory::~CEtelDriverFactory()"));

	// cancel all notificators
	iServiceChangeNotifier.Cancel();
	
	// cleanup array of CEtelDriverContext objs
	for (TInt i = 0; i < iContexts.Count(); i++ )
		{ 
		delete iContexts[i]; 
		}
	iContexts.Reset();

	// common data cleanup...
	// sync operations only
	iPacketService.Close();
	iPhone.Close();

	iTelServer.UnloadPhoneModule(iPdpFsmInterface.TsyName());

	iTelServer.Close();	// Phone module unloaded automatically
	
	iStrategies.Reset();
	}

/**
Function obtains the info on the phone object implemented in the TSY

Assumes aloadedTsyName has no ".TSY" appendage
Finds the phone information for the TSY just loaded. 
Assumes just one phone in that TSY - or that every phone in it is equally useful.

@param aTelServer - tel server
@param aLoadedTsyName - name of the loaded Tsy.
@param aInfo - information about the phone.
*/
static void GetPhoneInfoL(RTelServer& aTelServer, const TDesC& aLoadedTsyName, RTelServer::TPhoneInfo& aInfo)
	{
	// dev. note: leavescan reports an error in this method.
	// But there is no visible ground for it.
	SPUDTEL_FNLOG("GetPhoneInfoL()");
	TInt count;
	User::LeaveIfError(aTelServer.EnumeratePhones(count));
	if (count<=0)
		{
		User::Leave(KErrNotFound);
		}
		
	TBool found = EFalse;
	for (TInt i=0; i < count; i++)
		{
		TBuf<KCommsDbSvrMaxFieldLength> currentTsyName;
		User::LeaveIfError(aTelServer.GetTsyName(i,currentTsyName));
		
		TInt r=currentTsyName.Locate('.');
		if (r!=KErrNotFound)
			{
			currentTsyName.SetLength(r);
			}
		if (currentTsyName.CompareF(aLoadedTsyName)==KErrNone)
			{
			User::LeaveIfError(aTelServer.GetPhoneInfo(i,aInfo));
			found = ETrue;
			break;
			}
		}

	if (!found)
		{
		User::Leave(KErrNotFound);
		}

	}

/** initializes factory objects */
void CEtelDriverFactory::InitL()
	{
	SPUDTEL_FNLOG("CEtelDriverFactory::InitL()");
	SPUDTEL_INFO_LOG(_L("Initializing Etel driver"));
	
	iStrategies[EOpenPhoneStrategy] 			= &iOpenStrategy;
	iStrategies[ESetQoSStrategy] 				= &iSetQoSStrategy;
	iStrategies[ESetTftStrategy] 				= &iSetTftStrategy;
	iStrategies[EChangeTftStrategy]				= &iChangeTftStrategy;
	
	iStrategies[ECreate1ryPdpContextStrategy] 	= &iCreate1ryPdpContextStrategy;
	iStrategies[ECreate2ryPdpContextStrategy] 	= &iCreate2ryPdpContextStrategy;
	iStrategies[EActivatePdpStrategy] 			= &iActivatePdpStrategy;
	iStrategies[EGetNegQoSStrategy] 			= &iGetNegQoSStrategy;
	iStrategies[EModifyActiveStrategy] 			= &iModifyActivePdpStrategy;
	iStrategies[EContextDeleteStrategy] 		= &iContextDeleteStrategy;
	iStrategies[ECreateMbmsPdpContextStrategy] = &iCreateMbmsPdpContextStrategy;
	iStrategies[EActivateMbmsPdpStrategy]		= &iActivateMbmsPdpStrategy;
	iStrategies[ESessionUpdateStrategy]         = &iMbmsSessionUpdateStragegy;

	// initialization of operator to strategy id map
	iInputToStrategyMap[EOpenPhone]				= EOpenPhoneStrategy;
	iInputToStrategyMap[ESetQoS] 				= ESetQoSStrategy;
	iInputToStrategyMap[ECreate1ryPdpContext] 	= ECreate1ryPdpContextStrategy;
	iInputToStrategyMap[ECreate2ryPdpContext] 	= ECreate2ryPdpContextStrategy;
	iInputToStrategyMap[ESetTft] 				= ESetTftStrategy;
	iInputToStrategyMap[EChangeTft] 			= EChangeTftStrategy;
	iInputToStrategyMap[EActivatePdp] 			= EActivatePdpStrategy;
	iInputToStrategyMap[ENegQoSGet] 			= EGetNegQoSStrategy;
	iInputToStrategyMap[EModifyActive] 			= EModifyActiveStrategy;
	iInputToStrategyMap[EContextDelete]			= EContextDeleteStrategy;
	iInputToStrategyMap[ECreateMbmsPdpContext]  = ECreateMbmsPdpContextStrategy;
	iInputToStrategyMap[EActivateMbmsPdp]		= EActivateMbmsPdpStrategy;
	iInputToStrategyMap[ESessionUpdate]			= ESessionUpdateStrategy;
	User::LeaveIfError (iTelServer.Connect());
	User::LeaveIfError(iTelServer.LoadPhoneModule(iPdpFsmInterface.TsyName()));

	User::LeaveIfError(iTelServer.SetExtendedErrorGranularity(RTelServer::EErrorExtended));

	RTelServer::TPhoneInfo phoneInfo;
	GetPhoneInfoL(iTelServer, iPdpFsmInterface.TsyName(), phoneInfo);
	User::LeaveIfError (iPhone.Open (iTelServer, phoneInfo.iName) );
	User::LeaveIfError (iPacketService.Open (iPhone) );

	// remaining async calls to phone are postponed to later async Input request
	
	iServiceChangeNotifier.Start();
	}
	
/** creates pdp by id
	dev. note: pdp context will NOT be opened during this call.

@param aPdpId - id of a pdp context
*/
void CEtelDriverFactory::CreatePdpL (TContextId aPdpId,SpudMan::TPdpContextType aContextType)
	{
	SPUDTEL_FNLOG("CEtelDriverFactory::CreatePdpL()");
	SPUDTELVERBOSE_INFO_LOG1(_L("pdp id : %d"), aPdpId);
	__ASSERT_ALWAYS((NULL == iContexts[aPdpId]), User::Panic(KTxtSpudTel, KErrArgument));

	if (aContextType == SpudMan::EMbms)
		{
		iContexts[aPdpId]=CMbmsContextElem::NewL(aPdpId, *this, iPdpFsmInterface);
		}
	else
		{
		iContexts[aPdpId]=CSpudContextElem::NewL(aPdpId, *this, iPdpFsmInterface);
		}
	}


/** frees pdp context by id 

@param aPdpId - id of a pdp context
*/
void CEtelDriverFactory::FreePdp(TContextId aPdpId)
	{

	SPUDTELVERBOSE_INFO_LOG1(_L("Free pdp id : %d"), aPdpId);
    // In an OOM situation, this object may be cleaned up prior to establishment. 
    if (iContexts[aPdpId] != NULL) 
        { 
        // the context has to be closed before a call to FreePdp 
        __ASSERT_ALWAYS(!Context(aPdpId).PacketContext().SubSessionHandle(), User::Panic(KTxtSpudTel, KErrInUse)); 
        delete iContexts[aPdpId];
        iContexts[aPdpId] = NULL;
        } 
	}


/** starts etel notifications for pdp context

@param aPdpId - id of a pdp context
*/
void CEtelDriverFactory::StartPdpNotifications (TContextId aPdpId)
	{
	SPUDTELVERBOSE_INFO_LOG1(_L("StartPdpNotifications for pdp id : %d"), aPdpId);
	// sanity check
	ASSERT(iContexts[aPdpId]);
	
	iContexts[aPdpId]->Start();
	}

/** cancels etel notifications for aPdpId context 

@param aPdpId - id of a pdp context
*/
void CEtelDriverFactory::CancelPdpNotifications (TContextId aPdpId)
	{
	SPUDTELVERBOSE_INFO_LOG1(_L("CancelPdpNotifications for pdp id : %d"), aPdpId);
	// sanity check
	ASSERT(iContexts[aPdpId]);
	
	iContexts[aPdpId]->Cancel();
	}

/** cancels notifications for all pdp contexts */
void CEtelDriverFactory::CancelAllPdpNotifications()
	{
	SPUDTEL_INFO_LOG(_L("CancelAllPdpNotifications"));
	for(TContextId i = 0; i < static_cast<TContextId>(ContextCount()); i++)
		{ 
		CancelPdpNotifications (i); 
		}
	}


TBool CEtelDriverFactory::HasContext(TContextId aId) const
    {
    return (iContexts[aId] != NULL);
    }


/** context accessor

@param aPdpId - id of a pdp context
@return etel driver context
*/
CEtelDriverContext& CEtelDriverFactory::Context(TContextId aId) 
	{ 
	// sanity check
	__ASSERT_ALWAYS(iContexts[aId], User::Panic(KTxtSpudTel, KErrBadHandle));
	return *iContexts[aId]->iContext; 
	}
