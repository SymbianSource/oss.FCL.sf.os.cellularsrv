// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Helper class supplying useful logging functions to the providers in the layer
//

/**
 @file
 @internalComponent
*/

#include "mbufgobblerhelper.h"
#include "mbufgobblerlog.h"
#include <elements/mm_node.h>
#include <comms-infras/ss_nodeinterfaces.h>


void MbufGobblerHelper::LogCreateDestroy(const TDesC8& aSubTag, const TDesC8& aNodeName, TAny* aNodePtr, TBool aIsCreate)
	{
	// to eradicate warnings on builds for which the LOG macro does nothing (e.g. urel)
	(void)(aSubTag); (void)(aNodeName); (void)(aNodePtr); (void)(aIsCreate);
	
	if(aIsCreate)
		{
		LOG(aSubTag,_L8("%S (0x%x) Created"), &aNodeName, aNodePtr);
		}		
	else
		{
		LOG(aSubTag,_L8("%S (0x%x) Destroyed"), &aNodeName, aNodePtr);
		}		
	}

void MbufGobblerHelper::LogMessage(const TDesC8& aSubTag, const TDesC8& aNodeName, TAny* aNodePtr, const Messages::TRuntimeCtxId& aSender, const Messages::TNodeId& aRecipient, Messages::TSignatureBase& aMessage)
	{
	// to eradicate warnings on builds for which the LOG macro does nothing (e.g. urel)
	(void)(aSubTag); (void)(aNodeName); (void)(aNodePtr);

	TInt msgId = aMessage.MessageId().MessageId();
	TInt realm = aMessage.MessageId().Realm();
	
	TUint32 threadid_of_sender = aSender.Thread();
	Messages::TNodeId nid = Messages::address_cast<Messages::TNodeId>(aSender);
	TAny* ptr = nid.Ptr(); //this seems to be the only hting that can be used to print out something to address the node
	TAny* rptr = aRecipient.Ptr();
	__CFLOG_VAR((KMbufGobblerComponentTag, aSubTag,_L8("Node %S(0x%x) message received (msgid=%d, realm=0x%x), sender(0x%x threadID=%d)"), &aNodeName, rptr, msgId, realm, ptr, threadid_of_sender));
	}

void MbufGobblerHelper::PrintClientNodes(const TDesC8& aSubTag, const TDesC8& aNodeName, MeshMachine::AMMNodeBase& aNode, TInt& aClientCount)
	{
	// to eradicate warnings on builds for which the LOG macro does nothing (e.g. urel)
	(void)(aSubTag); (void)(aNodeName);

	TUint type = ESock::TCFClientType::EData | ESock::TCFClientType::ECtrl | ESock::TCFClientType::EServProvider | ESock::TCFClientType::ECtrlProvider;
	TInt totclients =  	aNode.CountClients<Messages::TDefaultClientMatchPolicy>(type);
	//only print if count different than previous
	if(aClientCount != totclients)
		{
		aClientCount = 	totclients;
		LOG(aSubTag,_L8("Printing Node %S(0x%x) info..."), &aNodeName, aNode.Id().Ptr());
		LOG(aSubTag, _L8("\tTotal Num clients = %d"), totclients);
		
		//enumerate through clients	
		Messages::TClientIter<Messages::TDefaultClientMatchPolicy> clientIter(aNode.GetClientIter<Messages::TDefaultClientMatchPolicy>(type));
		Messages::RNodeInterface* client = clientIter++;
		while (client)
			{
			
			Messages::TNodeId id = client->RecipientId();
			//look up node type
			TBuf8<20> typlu;
			switch(client->Type())
				{
				case ESock::TCFClientType::EUnknown:
					typlu = _L8("EUnknown");
					break;
				case ESock::TCFClientType::EData:
					typlu = _L8("EData");
					break;
				case ESock::TCFClientType::ECtrl:
					typlu = _L8("ECtrl");
					break;
				case ESock::TCFClientType::EServProvider:
					typlu = _L8("EServProvider");
					break;
				case ESock::TCFClientType::ECtrlProvider:
					typlu = _L8("ECtrlProvider");
					break;
				default:
					typlu = _L8("Unknown");
					break;
				}
			LOG(aSubTag, _L8("\t\tClientNode(0x%x) type=%d(%S), flags=%d"), id.Ptr(), client->Type(), &typlu, client->Flags());
			client = clientIter++;
			}
		}
		
	}

