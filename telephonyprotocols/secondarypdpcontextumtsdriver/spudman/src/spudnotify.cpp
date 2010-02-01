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
// NIFMAN notification proxy class
// 
//

/**
 @file
 @internalComponent
*/

#include "spudnotify.h"
#include "bindman.h"

CSpudNotify::CSpudNotify(CBindMan& aBindMan, MNifIfNotify* aNotify, TContextId aContextId)
    : iBindMan(aBindMan), iNotify(aNotify), iContextId(aContextId)
    {
    }


CSpudNotify::~CSpudNotify()
    {
    }

/**
 * Notification from the nif that the link layer has gone down.
 * @param aReason A Symbian OS error code indicating the reason for the link closing down
 * @param aAction The action that should be taken as a result of link layer down being signalled
 */
void CSpudNotify::LinkLayerDown(TInt aReason, TAction aAction)
    {
	iBindMan.SpudMan()->LinkLayerDown(iContextId, aReason, aAction);
    }

/**
 * Notification from the nif that the link layer is up
 */
void CSpudNotify::LinkLayerUp()
    {
	iBindMan.SpudMan()->LinkLayerUp(iContextId);
    }

/**
 * Indicate that link negotiation failed on this nif to allow nifman to take appropriate action
 * @param aIf The address of the CNifIfBase/Link object for which negotiation failed
 * @param aReason An error code denoting the reason for failure
 */
void CSpudNotify::NegotiationFailed(CNifIfBase* aIf, TInt aReason)
    {
    iNotify->NegotiationFailed(aIf, aReason);
    }

/**
 * Request to the agent to return the data required for authentication
 * @param aUsername On return, contains the username to be used for authentication
 * @param aPassword On return, contains the password to be used for authentication
 * @note The return values in aUsername and aPassword should not be considered valid until CNifIfLink::AuthenticateComplete() has been called
 */
TInt CSpudNotify::Authenticate(TDes& aUsername, TDes& aPassword)
    {
    return iNotify->Authenticate(aUsername, aPassword);
    }

/**
 * Request to the agent to cancel a pending authentication
 */
void CSpudNotify::CancelAuthenticate()
    {
    iNotify->CancelAuthenticate();
    }

/**
 * Fetch any excess data from the agent that it received during connection setup
 * @param aBuffer On return, the buffer contains the excess data from the agent
 * @return KErrNone, if successful; otherwise, one of the standard Symbian OS error codes
 */
TInt CSpudNotify::GetExcessData(TDes8& aBuffer)
    {
    return iNotify->GetExcessData(aBuffer);
    }


/**
 * Upcall from the nif indicating that a new progress state has been reached on this connection
 * @pre The stage value is within the range assigned to nifs i.e. KMinNifProgress <= aStage <= KMaxNifProgress
 * @param aStage The progress stage that has been reached
 * @param aError Any errors that have occurred
 */
void CSpudNotify::IfProgress(TInt aStage, TInt aError)
    {
	iBindMan.SpudMan()->IfProgress(iContextId, aStage, aError);
    }

/**
 * Upcall from the nif indicating that a new progress state has been reached on this subconnection
 * @pre The stage value is within the range assigned to nifs i.e. KMinNifProgress <= aStage <= KMaxNifProgress
 * @param aSubConnectionUniqueId The subconnection to which this notification refers
 * @param aStage The progress stage that has been reached
 * @param aError Any errors that have occurred
 */
void CSpudNotify::IfProgress(TSubConnectionUniqueId aSubConnectionUniqueId, TInt aStage, TInt aError)
    {
	iBindMan.SpudMan()->IfProgress(iContextId, aSubConnectionUniqueId, aStage, aError);
    }

/**
 * Increment the count of users of this nif
 */
void CSpudNotify::OpenRoute()
    {
    // TODO: check that this reference counting is correct
    iNotify->OpenRoute();
    }

/**
 * Decrement the count of users of this nif
 */
void CSpudNotify::CloseRoute()
    {
    // TODO: check that this reference counting is correct
    iNotify->CloseRoute();
    }

/**
 * Notification of an event from the nif
 * @param aEvent The type of event that occurred
 * @param aInfo Any data associated with the event
 * @return KErrNone if successful, otherwise one of the system-wide error codes
 */
TInt CSpudNotify::Notification(TNifToAgentEventType aEvent, void * aInfo)
    {
	return iBindMan.SpudMan()->Notification(iContextId, aEvent, aInfo);
    }

/**
 * Notification from the nif that a binder layer has gone down
 * @param aBinderIf An identifier for the binder layer that went down
 * @param aReason The reason for the binder layer failure
 * @param aAction The action to be taken as a result, eg. disconnect, reconnect
 */
void CSpudNotify::BinderLayerDown(CNifIfBase* aBinderIf, TInt aReason, TAction aAction)
    {
    iNotify->BinderLayerDown(aBinderIf, aReason, aAction);
    }

/**
 * Indication from network layer that a packet has been transmitted or received
 *
 * @param aDirection Whether the packet was transmitted or received
 * @param aBytes The size of the packet
 * @param aResetTimer Whether the idle timer should be reset
 * @return KErrNone if successful, otherwise one of the system-wide error codes
 */
TInt CSpudNotify::PacketActivity(TDataTransferDirection aDirection, TUint aBytes, TBool aResetTimer)
    {
    return iNotify->PacketActivity(aDirection, aBytes, aResetTimer);
    }

/**
 * Indication that the sent bytes threshold for a subconnection has been met or exceeded in the nif
 *
 * @param aSubConnectionUniqueId The subconnection to which this notification refers
 * @param aUplinkVolume The total number of bytes sent by this subconnection
 */
void CSpudNotify::NotifyDataSent(TSubConnectionUniqueId aSubConnectionUniqueId, TUint aUplinkVolume)
    {
    iNotify->NotifyDataSent(aSubConnectionUniqueId, aUplinkVolume);
    }

/**
 * Indication that the received bytes threshold for a subconnection has been met or exceeded in the nif
 *
 * @param aSubConnectionUniqueId The subconnection to which this notification refers
 * @param aDownlinkVolume The total number of bytes sent by this subconnection
 */
void CSpudNotify::NotifyDataReceived(TSubConnectionUniqueId aSubConnectionUniqueId, TUint aDownlinkVolume)
    {
    iNotify->NotifyDataReceived(aSubConnectionUniqueId, aDownlinkVolume);
    }

/**
 * Indication that some parameter relating to a subconnection has changed in the nif eg. subconnection closed, opened, QoS changed
 *
 * @param aEventType The type of event which has occurred - used to direct the event notification to the appropriate component(s)
 * @param aEvent The event which has occurred
 * @param aEventData Any arguments or additional data to do with the event; the interpretation of this depends on the event
 * @param aSource An appropriate identifier of the source of the event - this will be used when one receiver is receiving events from many producers
 */
void CSpudNotify::NifEvent(TNetworkAdaptorEventType aEventType, TUint aEvent, const TDesC8& aEventData, TAny* aSource)
    {
    iNotify->NifEvent(aEventType, aEvent, aEventData, aSource);
    }




/**
 * Read an integer field from the connection settings provider
 * @param aField The name of the field to read
 * @param aValue On return, contains the value of the field read
 * @param aMessage for capability checking
 * @return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
 */
TInt CSpudNotify::DoReadInt(const TDesC& aField, TUint32& aValue,const RMessagePtr2* aMessage)
    {
	//Spudman does eventually call iNotify->ReadInt but it does some necessary translations for some special cases therefore we
	//need to defer to Spudman and cannot call iNotify->ReadInt ourselves
    // The lower NIF will never have any capabilities to give us
	__ASSERT_ALWAYS(aMessage == NULL, iBindMan.SpudMan()->Panic());
	return iBindMan.SpudMan()->ReadInt(iContextId, aField, aValue);
    }

/**
 * Write an integer field to the connection settings provider
 * @param aField The name of the field to which to write
 * @param aValue The value to be written to the field
 * @param aMessage for capability checking
 * @return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
 */
TInt CSpudNotify::DoWriteInt(const TDesC& aField, TUint32 aValue,const RMessagePtr2* aMessage)
    {
    return iNotify->WriteInt(aField, aValue, aMessage);
    }

/**
 * Read a 8-bit descriptor field from the connection settings provider
 * @param aField The name of the field to read
 * @param aValue On return, contains the value of the field read
 * @param aMessage for capability checking
 * @return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
 */
TInt CSpudNotify::DoReadDes(const TDesC& aField, TDes8& aValue,const RMessagePtr2* aMessage)
    {
	//Spudman does eventually call iNotify->ReadDes but it does some necessary translations for some special cases therefore we
	//need to defer to Spudman and cannot call iNotify->ReadDes ourselves
    // The lower NIF will never have any capabilities to give us
	__ASSERT_ALWAYS(aMessage == NULL, iBindMan.SpudMan()->Panic());
	return iBindMan.SpudMan()->ReadDes(iContextId, aField, aValue);
    }

/**
 * Read a 16-bit descriptor field from the connection settings provider
 * @param aField The name of the field to read
 * @param aValue On return, contains the value of the field read
 * @param aMessage for capability checking
 * @return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
 */
TInt CSpudNotify::DoReadDes(const TDesC& aField, TDes16& aValue,const RMessagePtr2* aMessage)
    {
	//Spudman does eventually call iNotify->ReadDes but it does some necessary translations for some special cases therefore we
	//need to defer to Spudman and cannot call iNotify->ReadDes ourselves
    // The lower NIF will never have any capabilities to give us
	__ASSERT_ALWAYS(aMessage == NULL, iBindMan.SpudMan()->Panic());
	return iBindMan.SpudMan()->ReadDes(iContextId, aField, aValue);
    }

/**
 * Write an 8-bit descriptor field to the connection settings provider
 * @param aField The name of the field to which to write
 * @param aValue The value to be written to the field
 * @param aMessage for capability checking
 * @return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
 */
TInt CSpudNotify::DoWriteDes(const TDesC& aField, const TDesC8& aValue,const RMessagePtr2* aMessage)
    {
    return iNotify->WriteDes(aField, aValue, aMessage);
    }

/**
 * Write an 16-bit descriptor field to the connection settings provider
 * @param aField The name of the field to which to write
 * @param aValue The value to be written to the field
 * @param aMessage for capability checking
 * @return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
 */
TInt CSpudNotify::DoWriteDes(const TDesC& aField, const TDesC16& aValue,const RMessagePtr2* aMessage)
    {
    return iNotify->WriteDes(aField, aValue, aMessage);
    }

/**
 * Read an boolean field from the connection settings provider
 * @param aField The name of the field to read
 * @param aValue On return, contains the value of the field read
 * @param aMessage for capability checking
 * @return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
 */
TInt CSpudNotify::DoReadBool(const TDesC& aField, TBool& aValue,const RMessagePtr2* aMessage)
    {
    return iNotify->ReadBool(aField, aValue, aMessage);
    }

/**
 * Write an boolean field to the connection settings provider
 * @param aField The name of the field to which to write
 * @param aValue The value to be written to the field
 * @param aMessage for capability checking
 * @return KErrNone, if successful; otherwise one of the standard Symbian OS error codes
 */
TInt CSpudNotify::DoWriteBool(const TDesC& aField, TBool aValue,const RMessagePtr2* aMessage)
    {
    return iNotify->WriteBool(aField, aValue, aMessage);
    }


