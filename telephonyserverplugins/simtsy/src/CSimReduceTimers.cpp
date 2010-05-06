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
// A subject for oberving requests to reduce the timers currently
// running in the simulator.
// 
//

/**
 @file
*/


#include "Simlog.h"
#include "CSimReduceTimers.h"


/**
Factory function for creating a CSimReduceTimers object.

@return A pointer to the object created.
*/
CSimReduceTimers* CSimReduceTimers::NewL()
	{
	CSimReduceTimers* self = new(ELeave) CSimReduceTimers;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Second phase constructor.  Ensures that the required Publish &
Subscribe property is defined and starts listening to any changes (or
Set() calls) made to this property.
*/
void CSimReduceTimers::ConstructL()
	{
	TInt dummy;
	TInt ret = RProperty::Get(KUidPSSimTsyCategory, KPSSimTsyTimersReduceTime, dummy);

	if (ret == KErrNone)
		{
		iPSProperty = CSimPubSub::TPubSubProperty(KUidPSSimTsyCategory,KPSSimTsyTimersReduceTime,
												KPSSimTsyTimersReduceTimeSignalKeyType);
		iSimPubSub = CSimPubSub::NewL(this,iPSProperty);
		iSimPubSub->Start();
		}
	else
		{
		User::Leave(ret);
		}
	}

/**
Default, first-phase, constructor.
*/
CSimReduceTimers::CSimReduceTimers()
	{
	// NOP
	}

/**
Default destrucor for the reduce timers subject.
*/
CSimReduceTimers::~CSimReduceTimers()
	{
	delete iSimPubSub;
	iObservers.Close();
	}

void CSimReduceTimers::SimPSEvent(const CSimPubSub::TPubSubProperty aProperty, TInt aStatus)
	{
	if (aProperty == iPSProperty && // Condition for correct property
		(aStatus == ETimerIdAllTimers)) // and correct value for signal
		{
		LOGMISC1(">>CSimReduceTimers.cpp: Reduce Timer event fired for all timers");
		Notify();
		}
	else if(aProperty == iPSProperty && aStatus > 0)
		{
		LOGMISC2(">>CSimReduceTimers.cpp: Reduce Timer event fired for event ID %d", aStatus);
		Notify(aStatus);
		}
	}

void CSimReduceTimers::AttachL(MSimTimerUpdateObserver* aObserver)
	{
	LOGMISC1("CSimReduceTimers::AttachL");
	User::LeaveIfError(iObservers.Append(aObserver));
	}

void CSimReduceTimers::DetachL(MSimTimerUpdateObserver* aObserver)
	{
	TInt pos = iObservers.Find(aObserver);
	LOGMISC2("CSimReduceTimers::DetachL, from position %d", pos);
	User::LeaveIfError(pos);
	iObservers.Remove(pos);
	}

void CSimReduceTimers::Notify()
	{
	TInt totalItems = iObservers.Count();
	LOGMISC2("CSimReduceTimers::Notify, %d observers registered", totalItems);
	if (totalItems <= 0)
		{
		LOGMISC1("CSimReduceTimers::Notify, No timers to reduce.");
		return;
		}

	TInt minTimeReduce = KMaxTInt;
	TInt num;
	for (num = 0; num < totalItems; num++)
		{
		TInt temp = iObservers[num]->GetRemainingSeconds();
		LOGMISC3("CSimReduceTimers::Notify, iObservers[%d] remaining time = %d", num, temp);

		// Ensure no error is returned and check if the current itteration gives a lower time remaining.
		if (temp > 0 && temp < minTimeReduce)
			{
			minTimeReduce = temp;
			}
		}

	if (minTimeReduce == KMaxTInt)
		{
		LOGMISC1("CSimReduceTimers::Notify, No running timers.");
		return;
		}

	LOGMISC2("CSimReduceTimers::Notify, Min time to reduce all timers: %d", minTimeReduce);
	// Ensure that reducing timers by minTimeReduce does not reduce any timer to less than KTimerDelayOnReduceTimeSignal.
	minTimeReduce -= KTimerDelayOnReduceTimeSignal;
	if (minTimeReduce <= 0)
		{
		LOGMISC1("CSimReduceTimers::Notify, Min time to reduce <= KTimerDelayOnReduceTimeSignal, no change to timers.");
		return;
		}

	for (num = 0; num < totalItems; num++)
		{
		iObservers[num]->Update(minTimeReduce);
		}
	}
	
void CSimReduceTimers::Notify(TInt aTimerEventId)
	{
	TInt totalItems = iObservers.Count();
	LOGMISC2("CSimReduceTimers::Notify, %d observers registered", totalItems);
	if (totalItems <= 0)
		{
		LOGMISC1("CSimReduceTimers::Notify, No timers to reduce.");
		return;
		}
	
	//Get the index of the observer with with event id aTimerEventId
	//and the lowest remaining timer 
	TInt lowestTime = KMaxTInt;
	TInt indexOfLowest = KErrNotFound;
	
	TInt index;
	for(index = 0; index < totalItems; index++)
		{
		if((iObservers[index]->GetTimerEventInfo() == aTimerEventId) &&
			(iObservers[index]->GetRemainingSeconds() > 0) &&
			(iObservers[index]->GetRemainingSeconds() < lowestTime))
			{
			indexOfLowest = index;
			lowestTime = iObservers[index]->GetRemainingSeconds();
			}
		}
		
	if(indexOfLowest == KErrNotFound)
		{
		LOGMISC2(">>CSimReduceTimers::Notify, No observers with event ID %d found", 
					aTimerEventId);
		return;
		}

	//Check that reducing the timer of the selected observer is acceptable
	TInt reduceBy = lowestTime - KTimerDelayOnReduceTimeSignal;
	if(reduceBy >= lowestTime)
		{
		LOGMISC3("CSimReduceTimers::Notify, Lowest timer for event ID %d already less than %d sec. No timer reduced.",
				KTimerDelayOnReduceTimeSignal, aTimerEventId);
		}
	else
		{
		LOGMISC3(">>CSimReduceTimers::Notify, Timer for observer with event ID %d, reduced by %d sec", 
					aTimerEventId, reduceBy);
		iObservers[indexOfLowest]->Update(reduceBy);
		}
	}
