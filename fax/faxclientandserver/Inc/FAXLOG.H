// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent
*/

#ifndef __FAXLOG_H
#define __FAXLOG_H


#include <comms-infras/commsdebugutility.h>

//-------------------------------------------------------------------------------------------
//
//  some logging macros definition.
//  Unfortunately it is necessarily to use static logging functions because 
//  Fax client and server can not use the same logging object from their multiple threads
//
//-------------------------------------------------------------------------------------------

#ifdef _DEBUG

_LIT8(KFaxSubSystem,        "fax");      //-- Fax subsystem name for logging
_LIT8(KFaxCliComponent,     "FaxCli");   //-- Fax Client component name for logging
_LIT8(KFaxServerComponent,  "FaxSrv");   //-- Fax server component name for logging (CFaxSession class etc.)
_LIT8(KFaxStubComponent,    "FaxStub");  //-- Fax server stub component


//-- N.B. It's necessary to use static logging macros because FaxServer and FaxClient use 
//-- logging from different threads.

//-- logging macros for Fax Server
#define __FLOG_FAXSRV(AAA)           __FLOG_STATIC0(KFaxSubSystem,KFaxServerComponent,(AAA))
#define __FLOG_FAXSRV1(AAA,BBB)      __FLOG_STATIC1(KFaxSubSystem,KFaxServerComponent,(AAA),(BBB))
#define __FLOG_FAXSRV2(AAA,BBB,CCC)  __FLOG_STATIC2(KFaxSubSystem,KFaxServerComponent,(AAA),(BBB),(CCC))
                                     
//-- logging macros for Fax Client
#define __FLOG_FAXCLI(AAA)           __FLOG_STATIC0(KFaxSubSystem,KFaxCliComponent,(AAA))
#define __FLOG_FAXCLI1(AAA,BBB)      __FLOG_STATIC1(KFaxSubSystem,KFaxCliComponent,(AAA),(BBB))
#define __FLOG_FAXCLI2(AAA,BBB,CCC)  __FLOG_STATIC2(KFaxSubSystem,KFaxCliComponent,(AAA),(BBB),(CCC))

//-- logging macros for Fax Stub
#define __FLOG_FAXSTUB(AAA)          __FLOG_STATIC0(KFaxSubSystem,KFaxStubComponent,(AAA))
#define __FLOG_FAXSTUB1(AAA,BBB)     __FLOG_STATIC1(KFaxSubSystem,KFaxStubComponent,(AAA),(BBB))
#define __FLOG_FAXSTUB2(AAA,BBB,CCC) __FLOG_STATIC2(KFaxSubSystem,KFaxStubComponent,(AAA),(BBB),(CCC))

#else  //_DEBUG

#define __FLOG_FAXSRV(AAA)
#define __FLOG_FAXSRV1(AAA,BBB)
#define __FLOG_FAXSRV2(AAA,BBB,CCC)
/**                                     
logging macros for Fax Client
@internalComponent
*/
#define __FLOG_FAXCLI(AAA)
#define __FLOG_FAXCLI1(AAA,BBB)
#define __FLOG_FAXCLI2(AAA,BBB,CCC)

/**
logging macros for Fax Stub
@internalComponent
*/
#define __FLOG_FAXSTUB(AAA)
#define __FLOG_FAXSTUB1(AAA,BBB)
#define __FLOG_FAXSTUB2(AAA,BBB,CCC)


#endif	//_DEBUG

#endif  // __FAXLOG_H
