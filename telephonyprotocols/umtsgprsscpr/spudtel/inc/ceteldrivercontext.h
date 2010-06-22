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
// Header for etel driver context class
// 
//

/**
 @file 
 @internalComponent
*/

#ifndef CETELDRIVERCONTEXT_H
#define CETELDRIVERCONTEXT_H

#include <e32base.h>
#include <etelmm.h>
#include <etelqos.h>
#include <networking/umtsnifcontrolif.h>

#include "eteldrivernmspace.h"
#include "PDPFSM.h"
#include "meteldriverstrategy.h"
#include "pdpfsmnmspace.h"


class CEtelDriverFactory;

//using namespace EtelDriver;
/** 
 A data storage for pdp context 

 @internalComponent
*/
class CEtelDriverContext : public CActive
	{
public:
	CEtelDriverContext (TContextId aId, CEtelDriverFactory& aFactory);
	virtual ~CEtelDriverContext();

	TInt Input (EtelDriver::TEtelInput aOperation);
	
	void StartNotifications() const;
	void StopNotifications() const;

    void PdpStatusChangeNotifierCancel();
    void QoSChangeNotifierCancel();
    void PdpConfigChangeNotifierCancel();
    
	// CActive methods
	
	/** accessors, basic */
	inline TContextId Id() const;
	inline TName& Name();
	inline TRequestStatus& CompletionStatus();
	MEtelDriverStrategy::TSequenceStep StrategyStep() const;
	inline void SetStrategyStep (MEtelDriverStrategy::TSequenceStep aStep);

		
	/** accessors, eTel objects */
	inline RPacketQoS& PacketQoS();
	inline RPacketContext&	PacketContext();
	RPhone& Phone() const;
	RPacketService& PacketService() const;
	CPdpFsmInterface& PdpFsmInterface() const;
	const TName& ExistingContextName() const;

	/** context data accessors */
	
	inline TRequestedProfileBuffer& QosRequested();
	inline TNegotiatedProfileBuffer& QosNegotiated();
	
	inline RPacketContext::TContextConfigGPRS&	ContextConfigGPRS();

	inline TRequestedProfileBufferPckg& QosRequestedPckg();
	inline TNegotiatedProfileBufferPckg& QosNegotiatedPckg();
	inline EtelDriver::TContextConfigGPRSPckg& ContextConfigGPRSPckg();
	inline RPacketContext::TDataChannelV2& DataChannelV2();
	inline RPacketContext::TDataChannelV2Pckg& DataChannelV2Pckg();
	inline TTFTInfo& TftInfo();
	inline RPacketContext::TPacketFilterV2& FilterV2();
	inline RPacketContext::TPacketFilterV2Pckg& FilterV2Pckg();
	inline TUint8 TftRequested() const;
	inline TInt FirstFilterV2();
	inline TInt NextFilterV2();
	inline TTFTOperationCode& TftOperationCode();
	
#ifdef _DEBUG
    void DumpReqProfileParameters ();
    void DumpNegProfileParameters ();
#endif
		
protected:
	MEtelDriverStrategy& Strategy(EtelDriver::TEtelDriverStrategy aId) const;

	// CActive methods
	virtual void RunL();
	virtual void DoCancel();

protected:
	/** ID */
	TContextId				iId;
	/** tsy name of a context */
	TName					iName;
	/** request status */
	TRequestStatus			iCompletionStatus;
	/** active strategy step */
	MEtelDriverStrategy::TSequenceStep iStrategyStep;
	/** id of active strategy */
	EtelDriver::TEtelDriverStrategy 	iStrategyId;

	/** packet QoS */
	RPacketQoS				iPacketQoS;
	/** packet context */
	RPacketContext			iPacketContext;

	// factory data:
	/** etel driver factory */
	CEtelDriverFactory& 	iFactory;

	// data from pdp fsm
	// all members below contain valid data for limited time only, 
	//    i.e. for the duration of a strategy OR a duration of a single call to etel
	/** Requested QoS */
    TRequestedProfileBuffer iQosRequested;
    TRequestedProfileBufferPckg iQosRequestedPckg;

	/** Negotiated QoS */
    TNegotiatedProfileBuffer iQosNegotiated;
    TNegotiatedProfileBufferPckg iQosNegotiatedPckg;
    

	/** context config */
	RPacketContext::TContextConfigGPRS	iContextConfigGPRS;
	/** packaged context config */
	EtelDriver::TContextConfigGPRSPckg				iContextConfigGPRSPckg;
	/** tft info */
	TTFTInfo							iTftInfo;
	/** data channel V2 */
	RPacketContext::TDataChannelV2		iDataChannelV2;
	/** packaged data channel V2 */
	RPacketContext::TDataChannelV2Pckg	iDataChannelV2Pckg;
	
private:
	// TFT data
	/** local var for tft count */
	TUint8								iTftRequested;
	/** local V2 filter */
	RPacketContext::TPacketFilterV2		iFilterV2;
	/** local packaged V2 filter */
	EtelDriver::TPacketFilterV2Pckg					iFilterV2Pckg;
	/** TFT Operation Code */
	TTFTOperationCode					iTftOperationCode;
	};


// inlines
/** accessor 

@return id of first V2 filter 
*/
inline TInt CEtelDriverContext::FirstFilterV2()
	{ 
	iTftInfo.SetToFirst();
	iTftRequested = 0;
	return iTftInfo.NextPacketFilter(iFilterV2);
	}

/** accessor 

@return id of next V2 filter
*/
inline TInt CEtelDriverContext::NextFilterV2()
	{
	TInt err = iTftInfo.NextPacketFilter(iFilterV2);
	if (err == KErrNone)
		{
		++iTftRequested;
		}
	return err;
	}

/** accessor 

@return context id 
*/
inline TContextId CEtelDriverContext::Id() const
	{ 
	return iId; 
	}

/** accessor 

@return reference to name of pdp context
*/
inline TName& CEtelDriverContext::Name()
	{ 
	return iName; 
	}

/** accessor 

@return reference to request status
*/
inline TRequestStatus& CEtelDriverContext::CompletionStatus()
	{ 
	return iCompletionStatus; 
	}

/** accessor 

@return strategy sequence step 
*/
inline MEtelDriverStrategy::TSequenceStep CEtelDriverContext::StrategyStep() const
	{ 
	return iStrategyStep; 
	}

/** sets strategy sequence step 

@param aStep - strategy sequence step 
*/
inline void CEtelDriverContext::SetStrategyStep(MEtelDriverStrategy::TSequenceStep aStep)
	{ 
	iStrategyStep = aStep; 
	}
	
/** accessor

@return reference to etel packet QoS
*/
inline RPacketQoS& CEtelDriverContext::PacketQoS() 
	{ 
	return iPacketQoS; 
	}

/** accessor 

@return reference to etel packet context
*/
inline RPacketContext&	CEtelDriverContext::PacketContext()
	{ 
	return iPacketContext; 
	}


// context data accessors
/**
accessor 
@return reference to QoSR5Requested
*/

inline TRequestedProfileBuffer& CEtelDriverContext::QosRequested()
	{ 
	return iQosRequested; 
	}


inline TNegotiatedProfileBuffer& CEtelDriverContext::QosNegotiated()
	{ 
	return iQosNegotiated;
	}




	

/** accessor 

@return reference to TRequestedProfileBufferPckg
*/
inline TRequestedProfileBufferPckg& CEtelDriverContext::QosRequestedPckg()
	{ 
	return iQosRequestedPckg; 
	}

/**
accessor 
@return reference to TNegotiatedProfileBufferPckg
*/
inline TNegotiatedProfileBufferPckg& CEtelDriverContext::QosNegotiatedPckg()
	{ 
	return iQosNegotiatedPckg; 
	}


/**
accessor 
@return reference to ContextConfigGPRS
*/
inline RPacketContext::TContextConfigGPRS&	CEtelDriverContext::ContextConfigGPRS()
	{ 
	return iContextConfigGPRS; 
	}

/** accessor 

@return reference to ContextConfigGPRSPckg
*/
inline EtelDriver::TContextConfigGPRSPckg& CEtelDriverContext::ContextConfigGPRSPckg()
	{ 
	return iContextConfigGPRSPckg; 
	}

/** accessor 

@return reference to packet context' DataChannelV2
*/
inline RPacketContext::TDataChannelV2& CEtelDriverContext::DataChannelV2()
	{ 
	return iDataChannelV2; 
	}

/** accessor 

@return reference to packet context' DataChannelV2Pckg
*/
inline RPacketContext::TDataChannelV2Pckg& CEtelDriverContext::DataChannelV2Pckg()
	{ 
	return iDataChannelV2Pckg; 
	}

/** accessor 

@return reference to TFT info
*/
inline TTFTInfo& CEtelDriverContext::TftInfo()
	{ 
	return iTftInfo; 
	}

/** accessor 

@return reference to packet context' PacketFilterV2
*/
inline RPacketContext::TPacketFilterV2& CEtelDriverContext::FilterV2()
	{ 
	return iFilterV2; 
	}

/** accessor 

@return reference to packet context' PacketFilterV2Pckg
*/
inline RPacketContext::TPacketFilterV2Pckg& CEtelDriverContext::FilterV2Pckg()
	{ 
	return iFilterV2Pckg; 
	}

/** accessor 

@return requested TFT
*/
inline TUint8 CEtelDriverContext::TftRequested() const
	{ 
	return iTftRequested; 
	}

/** accessor 

@return reference to TFT operation code
*/
inline TTFTOperationCode& CEtelDriverContext::TftOperationCode()
	{
	return iTftOperationCode;
	}


#endif // CETELDRIVERCONTEXT_H
