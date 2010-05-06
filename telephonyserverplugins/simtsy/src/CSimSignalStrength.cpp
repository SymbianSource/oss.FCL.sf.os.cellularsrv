// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implements the functionality required to provide clients with
// signal strength information.
// 
//

/**
 @file
*/

#include <testconfigfileparser.h>
#include "CSimSignalStrength.h"
#include "CSimPhone.h"
#include "Simlog.h"
#include "simtsyglobalproperties.h"

const TInt KSignalStrengthGranularity=5;		// < Granularity for signal strength list array

CSimSignalStrength* CSimSignalStrength::NewL(CSimPhone* aPhone)
/**
 * Standard two-phase constructor.
 * @param aPhone				The parent phone object.
 * @return CSimSignalStrength	The new signal strength class.
 */
	{
	CSimSignalStrength* self=new(ELeave) CSimSignalStrength(aPhone);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CSimSignalStrength::CSimSignalStrength(CSimPhone* aPhone)
		: iPhone(aPhone)
/**
 * Trivial first phase construction.
 * @param aPhone				The parent phone object.
 */
	{
	}

void CSimSignalStrength::ConstructL()
/**
 * Second phase construction.  Create instances of the necessary heap-based
 * objects and read in the signal strength information from the configuration file.
 * Finally, if any signal strength tags have been read, the initial values
 * will be loaded and the timer started.
 *
 * Entries in the configuration file will take the following format:
 * "SignalStrengthLevel= <duration>, <dB Level>, <Bar Level>"
 * A number of these entries may be included to create a signal strength profile
 * for the duration of the test.
 */
	{
	iTimer=CSimTimer::NewL(iPhone);
	iSignalStrengthInfo=new(ELeave) CArrayFixFlat<TSignalStrengthInfo>(KSignalStrengthGranularity);

	LOGSS1("Starting to parse Signal Strength config parameters...");
	
	const CTestConfigItem* item=NULL;
	TInt ret=KErrNone;
	item=CfgFile()->Item(KSSNotifyCap);

	if (item != NULL)
		{
		TInt notifyCap;
		ret = CTestConfig::GetElement(item->Value(),KStdDelimiter,0,notifyCap);
		if(ret != KErrNone)
			{
			iNotifyCapability = 0;
			}
		else
			{
			iNotifyCapability = notifyCap;
			}
		}
	else
		{
		iNotifyCapability = 0;
		}
		
	TInt count=CfgFile()->ItemCount(KSSLevel);

	TInt i;
	for(i=0;i<count;i++)
		{
		item=CfgFile()->Item(KSSLevel,i);
		if(!item)
			break;

		TInt duration,ss,ssb,err;
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,0,duration);
		if(ret!=KErrNone)
			{
			LOGPARSERR("duration",ret,0,&KSSLevel);
			continue;
			}
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,1,ss);
		if(ret!=KErrNone)
			{
			LOGPARSERR("ss",ret,1,&KSSLevel);
			continue;
			}
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,2,ssb);
		if(ret!=KErrNone)
			{
			LOGPARSERR("ssb",ret,2,&KSSLevel);
			continue;
			}
		ret=CTestConfig::GetElement(item->Value(),KStdDelimiter,3,err);
		if (ret == KErrNotFound)
			{
			err = KErrNone;
			}
		else
			{
			LOGPARSERRANGE("ssb",ret,3,&KSSLevel,"0 or -1");
			}

		TSignalStrengthInfo ssInfo;
		ssInfo.iDuration=duration;
		ssInfo.iLevel=ss;
		ssInfo.iBar=(TInt8)ssb;
		ssInfo.iErr = err;
		iSignalStrengthInfo->AppendL(ssInfo);
		}
	
	LOGSS1("...Finished parsing Signal Strength config parameters...");

	if(iSignalStrengthInfo->Count()!=0)
		{
		iSSIndex=0;
		iCurrentLevel=iSignalStrengthInfo->At(0).iLevel;
		iCurrentBar=iSignalStrengthInfo->At(0).iBar;
		iCurrentError = iSignalStrengthInfo->At(0).iErr;
		iTimer->Start(iSignalStrengthInfo->At(0).iDuration,this);
		}
	//still have to listen on deprecated P&S uids
	iSignalStrengthChange = CSimPubSubChange::NewL(this, CSimPubSub::TPubSubProperty(KUidSimtsyCategoryLocal, KSimtsySignalStrengthChange, KSimtsySignalStrengthChangeType));
	iPSSignalStrengthChange = CSimPubSubChange::NewL(this, CSimPubSub::TPubSubProperty(KUidPSSimTsyCategory, KPSSimTsySignalStrengthChange, KPSSimTsySignalStrengthChangeKeyType));
	}

CSimSignalStrength::~CSimSignalStrength()
/**
 * Standard destructor.  Destroy the heap-based object owned by this object.
 */
	{
	delete iTimer;
	delete iSignalStrengthInfo;
	delete iSignalStrengthChange;
	delete iPSSignalStrengthChange;
	}

TInt CSimSignalStrength::GetSignalCaps(TTsyReqHandle aReqHandle,TDes8* aPckg1)
/**
 * Retrieve signal strength capability information.  This function completes the
 * client's request synchronously.  If the configuration file contains any signal
 * strength profile information, then it indicates support for signal strength
 * requests, otherwise it does not.
 *
 * @param aReqHandle	The request handle associated with this request.
 * @param aPckg1		The first parameter package.  This will be populated with the
 *						capability data to be returned.
 * @return TInt			Standard error value.
 */
	{
	TPckg<TUint32>* signalCapsPckg=(TPckg<TUint32>*)aPckg1;
	TUint32& signalCaps=(*signalCapsPckg)();

	if (iNotifyCapability != 0)
		{
		signalCaps= RMobilePhone::KCapsGetSignalStrength |
					RMobilePhone::KCapsNotifySignalStrengthChange;
		}
	else if(iSignalStrengthInfo->Count()==0)
		{
		signalCaps=0;
		}
	else if((iSignalStrengthInfo->Count()-1)<=iSSIndex)
		{
		signalCaps= RMobilePhone::KCapsGetSignalStrength;
		}
	else
		{
		signalCaps= RMobilePhone::KCapsGetSignalStrength |
					RMobilePhone::KCapsNotifySignalStrengthChange;
		}

	iPhone->ReqCompleted(aReqHandle,KErrNone);
	return KErrNone;
	}

TInt CSimSignalStrength::GetSignalStrength(TTsyReqHandle aReqHandle,TDes8* aPckg1, TDes8* aPckg2)
/**
 * Return the current signal strength information.  This function completes synchronously.
 * If the configuration file contains any signal strength profile information, the
 * request completes successfully, otherwise it completes with KErrNotSupported.
 *
 * @param aReqHandle	The request handle associated with this request.
 * @param aPckg1		The first parameter package.  This is populated with the signal
 *						strength dB level.
 * @param aPckg2		The second parameter package.  This is populated with the signal
 *						strength bar level.
 * @return TInt			Standard error value.
 */
	{
	LOGSS1(">>CSimSignalStrength::GetSignalStrength");
	TPckg<TInt32>* ssPckg=(TPckg<TInt32>*)aPckg1;
	TInt32& ss=(*ssPckg)();
	TPckg<TInt8>* ssbPckg=(TPckg<TInt8>*)aPckg2;
	TInt8& ssb=(*ssbPckg)();

	if(iSignalStrengthInfo->Count()==0 && iNotifyCapability == 0)
		{
		iPhone->ReqCompleted(aReqHandle,KErrNotSupported);
		return KErrNone;
		}

	ss=iCurrentLevel;
	ssb=iCurrentBar;
	LOGSS3("<<CSimSignalStrength::GetSignalStrength with level=%d dB and bar=%d",iCurrentLevel,iCurrentBar);
	iPhone->ReqCompleted(aReqHandle,iCurrentError);
	return KErrNone;
	}

TInt CSimSignalStrength::NotifySignalStrengthChange(TTsyReqHandle aReqHandle,TDes8* aPckg1, TDes8* aPckg2)
/**
 * Register a client's interest in being notified when the signal strength changes.
 * This function records the request's parameters and awaits a change in signal
 * strength before completing.
 *
 * @param aPckg1		The first parameter package.  This is populated with the signal
 *						strength dB level.
 * @param aPckg2		The second parameter package.  This is populated with the signal
 *						strength bar level.
 * @return TInt			Standard error value.
 */
	{
	LOGSS1(">>CSimSignalStrength::NotifySignalStrengthChange");
	TPckg<TInt32>* ssPckg=(TPckg<TInt32>*)aPckg1;
	TInt32& ss=(*ssPckg)();
	TPckg<TInt8>* ssbPckg=(TPckg<TInt8>*)aPckg2;
	TInt8& ssb=(*ssbPckg)();

	if(iSignalStrengthInfo->Count()==0 && iNotifyCapability == 0)
		{
		iPhone->ReqCompleted(aReqHandle,KErrNotSupported);
		return KErrNone;
		}

	__ASSERT_ALWAYS(!iSSChangeNotificationPending,SimPanic(ENotificationReqAlreadyOutstanding));
	iSSChangeNotificationPending=ETrue;
	iSSChangeNotificationReqHandle=aReqHandle;
	iSSChangeNotificationSSLevel=&ss;
	iSSChangeNotificationSSBarLevel=&ssb;
	return KErrNone;
	}

void CSimSignalStrength::NotifySignalStrengthChangeCancel()
/**
 * Cancel a previous request to be notified of a change in signal strength.
 */
	{
	if(iSSChangeNotificationPending)
		{
		LOGSS1("CSimSignalStrength::NotifySignalStrengthChange has been cancelled");
		iSSChangeNotificationPending=EFalse;
		iPhone->ReqCompleted(iSSChangeNotificationReqHandle,KErrCancel);
		}
	else 
		LOGSS1("CSimSignalStrength::NotifySignalStrengthChange was not outstanding and hasn't been cancelled");
	}

void CSimSignalStrength::TimerCallBack(TInt /*aId*/)
/**
 * The timer callback function.  This function will be called when the timer
 * completes.  It indicates a change in signal strength settings.  So, the new
 * signal strength settings must be loaded into the member variables representing
 * the current settings and any pending signal strength notification requests must
 * be completed.  Finally, the next timer is started.
 *
 * @param aId	This parameter is unused.  It is only required for CSimXxx classes
 *				that have more than one timer instance and need to identify which
 *				timer has expired.
 */
	{
	iSSIndex++;
	if(iSignalStrengthInfo->Count()<=iSSIndex)
		return;

	iCurrentLevel=iSignalStrengthInfo->At(iSSIndex).iLevel;
	iCurrentBar=iSignalStrengthInfo->At(iSSIndex).iBar;
	iCurrentError = iSignalStrengthInfo->At(iSSIndex).iErr;
	if(iSSChangeNotificationPending)
		{
		iSSChangeNotificationPending=EFalse;
		*iSSChangeNotificationSSLevel=iCurrentLevel;
		*iSSChangeNotificationSSBarLevel=iCurrentBar;
		LOGSS3("<<CSimSignalStrength::NotifySignalStrengthChange with level=%d dB and bar=%d",iCurrentLevel,iCurrentBar);
		iPhone->ReqCompleted(iSSChangeNotificationReqHandle,iCurrentError);
		}
	iTimer->Start(iSignalStrengthInfo->At(iSSIndex).iDuration,this);
	}

const CTestConfigSection* CSimSignalStrength::CfgFile()
/**
 * Returns a pointer to the current configuration file section.
 *
 * @return CTestConfigSection	A pointer to the current configuration file section.
 */
	{
	return iPhone->CfgFile();
	}

/**
 * Called by CSimSignalStrengthChange when a signal strength request has been received.
 *
 * @param aNewVal The new signal strength value (number of bars)
 */
void CSimSignalStrength::PubSubCallback(TInt aNewVal)
	{
	LOGSS2("CSimSignalStrength::SignalStrengthChangeCallBack newVal=%d", aNewVal);
	if (iCurrentBar == aNewVal)
		{
		return;
		}
	iCurrentBar = (TInt8) aNewVal;
	if(iSSChangeNotificationPending)
		{
		iSSChangeNotificationPending=EFalse;
		*iSSChangeNotificationSSLevel=iCurrentLevel;
		*iSSChangeNotificationSSBarLevel=iCurrentBar;
		LOGSS3("<<CSimSignalStrength::NotifySignalStrengthChange with level=%d dB and bar=%d",iCurrentLevel,iCurrentBar);
		iPhone->ReqCompleted(iSSChangeNotificationReqHandle,iCurrentError);
		}
	}
