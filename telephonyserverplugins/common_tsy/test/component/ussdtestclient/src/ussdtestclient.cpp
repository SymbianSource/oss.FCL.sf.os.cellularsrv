// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ussdtestclient.h"
#include <ctsy/mmtsy_names.h>
#include <e32debug.h>
#include <e32math.h> 

CCTsyUssdMessagingTestClient::CCTsyUssdMessagingTestClient()
    {
    }  

/**
@SYMTestCaseID 
@SYMComponent  telephony_ctsy
@SYMTestCaseDesc Test support in CTSY for RMobileUssdMessaging::ReceiveMessage
@SYMTestPriority High
@SYMTestActions Invokes RMobileUssdMessaging::ReceiveMessage
@SYMTestExpectedResults Pass
@SYMTestType CT
*/
TInt CCTsyUssdMessagingTestClient::ReceiveMessageL(TBool aAcceptDialogue, TBool aReceiveTimeout, TReal aAfterTimePeriod, TBool aMO)
    {
    // Create a cleanup stack object
    CTrapCleanup* cleanup=CTrapCleanup::New();
    if (cleanup==NULL)
        return KErrNoMemory;    
    
    RTelServer server;
    RMobilePhone phone;

    TInt ret = server.Connect();
    if (ret != KErrNone)
        {
        server.Close();
        return ret; 
        }

    ret = phone.Open(server,KMmTsyPhoneName);
    if (ret != KErrNone)
        {
        server.Close();
        phone.Close();
        return ret; 
        }
    
    TRequestStatus requestStatus;   
    RMobileUssdMessaging::TGsmUssdMessageData name;
    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging ussdMessaging;
    ret = ussdMessaging.Open(phone);
    if (ret != KErrNone)
        {
        server.Close();
        phone.Close();
        return ret; 
        }
    
    //RDebug::Printf("%x >> ReceiveMessage", User::Identity());
    ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);
    //RDebug::Printf("%x << ReceiveMessage", User::Identity());
    
    if (aReceiveTimeout)
        {
        // This section of code and the aReceiveTimeout parameter apply
        // to the TestReceiveMessageWithTestClient0016L test 
        // (BA-CTSY-USSDM-URM-0020).
        // This test verifies that the second client does not recieve
        // a message. As part of the test spec we must call close on
        // the RMobileUssdMessaging object.
        // Rather than adding a whole ActiveScheduler framework to this
        // executable we've put in a wait and check loop. We can't use 
        // User::WaitForRequest as this would block. We don't want to
        // block as we are checking that an event which 
        // *does not occur* rather than checking for an event which
        // does occur.
        for (TInt i=0; i<50; ++i)
            {
            User::After(100000); // 0.1s
            if (requestStatus.Int() != KRequestPending)
                {
                break;
                }
            }

        if (requestStatus.Int() == KRequestPending)
            {
            ussdMessaging.Close();
            phone.Close();
            server.Close();
            
            delete cleanup;
            return KRequestPending;
            }
        }


    //RDebug::Printf("%x >> WaitForRequest", User::Identity());
    User::WaitForRequest(requestStatus);     
    //RDebug::Printf("%x << WaitForRequest", User::Identity());
    //RDebug::Printf("%x requestStatus = %d", User::Identity(), requestStatus.Int());
        
    if (aAfterTimePeriod)
        {
        //RDebug::Printf("%x Waiting for %f seconds", User::Identity(), aAfterTimePeriod);
        // Wait, mimic a bad client taking too long...
        User::After(aAfterTimePeriod * 1000000);
        }
    
    if(EFalse == aMO)
    	//network initiated dialog
        {
    	//RDebug::Printf("NI Dialog");
		if (aAcceptDialogue)
			{
			//RDebug::Printf("%x >> AcceptIncomingDialogue", User::Identity());
			// Client wants the dialogue
			ret = ussdMessaging.AcceptIncomingDialogue(); 
		   //RDebug::Printf("%x << AcceptIncomingDialogue", User::Identity());
			}
		else
			{
			// Client not interested
			//RDebug::Printf("%x >> RejectIncomingDialogue", User::Identity());
			ret = ussdMessaging.RejectIncomingDialogue();   
			//RDebug::Printf("%x << RejectIncomingDialogue", User::Identity());
			}
        }
    else
    	{
    	//RDebug::Printf("MO Dialogue");
    	ret = requestStatus.Int();    	
    	}
    //RDebug::Printf("%x Received Message: %S", User::Identity(), &name);
    User::After(2 * 1000000);

    ussdMessaging.Close();
    phone.Close();
    server.Close();
    
    delete cleanup;
    return ret;
	}

TInt CCTsyUssdMessagingTestClient::SendMessageL()
    {
    // Create a cleanup stack object
    CTrapCleanup* cleanup=CTrapCleanup::New();
    if (cleanup==NULL)
        return KErrNoMemory;    
    
    RTelServer server;
    RMobilePhone phone;

    TInt ret = server.Connect();
    if (ret != KErrNone)
        {
        server.Close();
        return ret; 
        }

    ret = phone.Open(server,KMmTsyPhoneName);
    if (ret != KErrNone)
        {
        server.Close();
        phone.Close();
        return ret; 
        }
    
    TRequestStatus requestStatus;

    TUint32 flags = 100;
    RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
    RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
    TUint8 dcs = 200;

    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    attributes.iFlags  = flags;
    attributes.iFormat = format;
    attributes.iType   = type;
    attributes.iDcs   = dcs;
    
    TName name = _L("Name1");
    TPckg<TName> msgData(name);
    
    RMobileUssdMessaging ussdMessaging;
    ret = ussdMessaging.Open(phone);
    if (ret != KErrNone)
        {
        server.Close();
        phone.Close();
        return ret; 
        }
        
    ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes);
    User::WaitForRequest(requestStatus);
    ret = requestStatus.Int();
    
    User::After(2 * 1000000);

    ussdMessaging.Close();
    phone.Close();
    server.Close();
    
    delete cleanup;
    return KErrNone;
    }

TInt CCTsyUssdMessagingTestClient::SendMessageDefaultHandlerL()
    {
    // Create a cleanup stack object
    CTrapCleanup* cleanup=CTrapCleanup::New();
    if (cleanup==NULL)
        return KErrNoMemory;    
    
    RTelServer server;
    RMobilePhone phone;

   TInt ret = server.Connect();
   if (ret != KErrNone)
	   {
	   server.Close();
	   return ret; 
	   }

   RDebug::Printf(">> server.Connect() SUCCESS!");
       ret = phone.Open(server,KMmTsyPhoneName);
       if (ret != KErrNone)
           {
           server.Close();
           phone.Close();
           return ret; 
           }
   RDebug::Printf(">> phone.Open() SUCCESS!"); 
   TRequestStatus requestStatus;

   TUint32 flags = 100;
   RMobileUssdMessaging::TMobileUssdDataFormat format = RMobileUssdMessaging::EFormatUnspecified;
   RMobileUssdMessaging::TMobileUssdMessageType type  = RMobileUssdMessaging::EUssdMORequest;
   TUint8 dcs = 200;

   RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
   TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
	
	attributes.iFlags  = flags;
	attributes.iFormat = format;
	attributes.iType   = type;
	attributes.iDcs   = dcs;
    
    TName name = _L("Name1");
    TPckg<TName> msgData(name);
    
    RMobileUssdMessaging ussdMessaging;
    ret = ussdMessaging.Open(phone);
    if (ret != KErrNone)
        {
        server.Close();
        phone.Close();
        return ret; 
        }
    RDebug::Printf(">> ussdMessaging.Open() SUCCESS!"); 
            
    ussdMessaging.SendMessage(requestStatus, msgData, msgAttributes, RMobileUssdMessaging::ETransferToDefaultHandler);    
    User::WaitForRequest(requestStatus);
    ret = requestStatus.Int();
    RDebug::Printf(">> ret = %d", ret);
    
    User::After(2 * 1000000);

    ussdMessaging.Close();
    phone.Close();
    server.Close();
    
    delete cleanup;
    return ret;
    }
TInt CCTsyUssdMessagingTestClient::RandomLoopL()
    {
    // Create a cleanup stack object
    CTrapCleanup* cleanup=CTrapCleanup::New();
    if (cleanup==NULL)
        return KErrNoMemory;    
    
    RTelServer server;
    RMobilePhone phone;

    TInt ret = server.Connect();
    if (ret != KErrNone)
        {
        server.Close();
        return ret; 
        }

    ret = phone.Open(server,KMmTsyPhoneName);
    if (ret != KErrNone)
        {
        server.Close();
        phone.Close();
        return ret; 
        }
    
    TRequestStatus requestStatus;   
    RMobileUssdMessaging::TGsmUssdMessageData name;
    RMobileUssdMessaging::TMobileUssdAttributesV1 attributes;
    TPckg<RMobileUssdMessaging::TMobileUssdAttributesV1> msgAttributes(attributes);
    
    RMobileUssdMessaging ussdMessaging;
    
    FOREVER
        {
        ret = ussdMessaging.Open(phone);
        if (ret != KErrNone)
            {
            server.Close();
            phone.Close();
            return ret; 
            }
        
        //RDebug::Printf("%x >> ReceiveMessage", User::Identity());
        ussdMessaging.ReceiveMessage(requestStatus, name, msgAttributes);
        //RDebug::Printf("%x << ReceiveMessage", User::Identity());
    
        if (ProbabilityPercent(50))
            {
            for (TInt i=0; i<10; ++i)
                {
                User::After(100000); // 0.1s
                if (requestStatus.Int() != KRequestPending)
                    {
                    break;
                    }
                }
            }
        else
            {
            User::WaitForRequest(requestStatus);  
            }
        
        if (requestStatus.Int() == KErrNone)
            {
            if (ProbabilityPercent(50))
                {
                TInt timeout = RandomNumber(1000000);
                User::After(timeout);
                }
            
            if (ProbabilityPercent(50))
                {
                ret = ussdMessaging.AcceptIncomingDialogue();
                }
            else
                {
                ret = ussdMessaging.RejectIncomingDialogue();
                }
            }
        else
            {
            ussdMessaging.CancelAsyncRequest(EMobileUssdMessagingReceiveMessage);
            }
    
        ussdMessaging.Close();
        }
    
    phone.Close();
    server.Close();
    
    delete cleanup;
    return ret;
    }

TBool CCTsyUssdMessagingTestClient::ProbabilityPercent(TInt aPercentTrue)
    {
    TInt random = RandomNumber(100);
    return aPercentTrue < random;
    }

// Not properly random but good enough...
TInt CCTsyUssdMessagingTestClient::RandomNumber(TInt aMax)
    {
    TUint32 rand = Math::Random() % aMax;
    return rand;
    }

_LIT(KReceiveOption,"-R");
_LIT(KReceiveTimeoutOption,"-RTimeout");
_LIT(KSendOption,"-S");
_LIT(KAcceptOption,"-A");
_LIT(KTimerOption,"-T");
_LIT(KRandomOption,"-Random");
_LIT(KDefaultOption,"-D");
_LIT(KMOOption,"-M");
TInt ParseCommandLine(TReal& aAfterTimePeriod)
    {
    TBuf<256> c;
    User::CommandLine(c);

    TLex l(c);
    
    // Get options
    TInt option = 0;
    aAfterTimePeriod = 0;
    TPtrC nextToken;
    for (;;)
        {
        nextToken.Set(l.NextToken());
        if (nextToken == KReceiveOption)
            {               
            option |= EOptionReceive;
            }
        else if (nextToken == KReceiveTimeoutOption)
            { 
            option |= EOptionReceiveTimeout;
            }
        else if (nextToken == KSendOption)
            {               
            option |= EOptionSend;
            }
        else if (nextToken == KAcceptOption)
            {               
            option |= EOptionAccept;
            }
        else if (nextToken == KDefaultOption)
			{               
			option |= EOptionDefault;
			}
        else if (nextToken == KMOOption)
			{               
			option |= EOptionMO;
			}
        else if (nextToken == KTimerOption)
            {               
            nextToken.Set(l.NextToken());
            if (nextToken != KNullDesC)
                {
                TLex sl(nextToken);
                if (sl.Val(aAfterTimePeriod)!=KErrNone)
                    aAfterTimePeriod = 0.0;
                }
            }
        else if (nextToken == KRandomOption)
            {               
            option |= EOptionRandom;
            }
        else
            {
            break;
            }
        }
    return option;
    }

TInt E32Main()
/** 
   Executable entrypoint calls test functions within heap check.
 */
    {
    //__UHEAP_MARK;
    
    CTrapCleanup* cleanup=CTrapCleanup::New();
    
    TReal afterTimePeriod;
    TInt option = ParseCommandLine(afterTimePeriod);
    TInt err = KErrNone;
    TInt ret = KErrNone;
    CCTsyUssdMessagingTestClient ussdClient;
    if (option & EOptionReceive)
        {
        TRAPD(err, ret = ussdClient.ReceiveMessageL((option & EOptionAccept), (option & EOptionReceiveTimeout), afterTimePeriod, (option & EOptionMO)));
        }
    
    if (option & EOptionSend)
        {
        TRAPD(err, ret = ussdClient.SendMessageL());
        }
    
    if (option & EOptionDefault)
		{
		TRAPD(err, ret = ussdClient.SendMessageDefaultHandlerL());
		}
    
    if (option & EOptionRandom)
        {
        TRAPD(err, ret = ussdClient.RandomLoopL());
        }
    
    delete cleanup;
    
    //__UHEAP_MARKEND;
    
    return err ? err : ret;
    }


