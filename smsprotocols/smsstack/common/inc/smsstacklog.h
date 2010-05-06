// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _SMSSTACKLOG_H_
#define _SMSSTACKLOG_H_

/**
 *  @file
 *
 *  Logging macros for SMSStack. These use the Comms Debug Utility framework
 *  (aka Flogger V2).
 *
 *  To enable logging create a file called "comsdbg.ini" in the Symbian OS
 *  "C:\Logs\" directory which contains the following lines:
 *
 *    MEDIA FILE
 *    LOG SMSStack Common
 *    LOG SMSStack GSMU
 *    LOG SMSStack SMSPROT
 *    LOG SMSStack SMSU
 *    LOG SMSStack SMSIF
 *    LOG SMSStack WAPPROT
 *
 *  Each line relates to a particular part of the SMS Stack, enabling you to
 *  log only the interesting bits.
 *
 *  By default logging is only present on UDEB builds. To enable UREL logging
 *  on a device, you will need to rebuild your ROM with UDEB SMS components.
 *
 *  To enable logging on a device, place the "comsdbg.ini" file in the ROM
 *  under the "Z:\Logs\" directory. The log file will be written in the
 *  "C:\Logs\" directory on the device.
 *
 *  To enable logging to the serial port change the "MEDIA FILE" line to
 *  "MEDIA SERIAL::1" or "MEDIA SERIAL::2". Note that this may no longer
 *  work due to a defect in the Comms Debug Framework.
 *
 *  If you wish to have individual logs for each component, then use the
 *  extractlog.pl utility found in the %EPOCROOT%\Epoc32\Tools\ directory. E.g.
 *
 *    extractlog.pl \EPOC32\WINSCW\C\LOGS\LOG.TXT
 *
 *  @internalComponent
 */

#include <comms-infras/commsdebugutility.h>
#include <etelmm.h>

#ifdef _DEBUG
#define _SMS_LOGGING_ENABLED
#endif


#ifdef _SMS_LOGGING_ENABLED

_LIT8(KSMSStackSubSystem, "SMSStack");

#define SMSFLOG1(N,A)                 { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC0(KSMSStackSubSystem,(N),KLogString); \
									  }
#define SMSFLOG2(N,A,B)               { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC1(KSMSStackSubSystem,(N),KLogString,(B)); \
									  }
#define SMSFLOG3(N,A,B,C)             { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC2(KSMSStackSubSystem,(N),KLogString,(B),(C)); \
									  }
#define SMSFLOG4(N,A,B,C,D)           { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC3(KSMSStackSubSystem,(N),KLogString,(B),(C),(D)); \
									  }
#define SMSFLOG5(N,A,B,C,D,E)         { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC4(KSMSStackSubSystem,(N),KLogString,(B),(C),(D),(E)); \
									  }
#define SMSFLOG6(N,A,B,C,D,E,F)       { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC5(KSMSStackSubSystem,(N),KLogString,(B),(C),(D),(E),(F)); \
									  }
#define SMSFLOG7(N,A,B,C,D,E,F,G)     { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC6(KSMSStackSubSystem,(N),KLogString,(B),(C),(D),(E),(F),(G)); \
									  }
#define SMSFLOG8(N,A,B,C,D,E,F,G,H)   { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC7(KSMSStackSubSystem,(N),KLogString,(B),(C),(D),(E),(F),(G),(H)); \
									  }

_LIT8(KSMSStackCommon,  "Common");
_LIT8(KSMSStackGSMU,    "GSMU");
_LIT8(KSMSStackSMSPROT, "SMSPROT");
_LIT8(KSMSStackSMSU,    "SMSU");
_LIT8(KSMSStackSMSIF,   "SMSIF");
_LIT8(KSMSStackWAPPROT, "WAPPROT");


#define LOGCOMMON1(A)                 SMSFLOG1(KSMSStackCommon,A)
#define LOGCOMMON2(A,B)               SMSFLOG2(KSMSStackCommon,A,(B))
#define LOGCOMMON3(A,B,C)             SMSFLOG3(KSMSStackCommon,A,(B),(C))
#define LOGCOMMON4(A,B,C,D)           SMSFLOG4(KSMSStackCommon,A,(B),(C),(D))
#define LOGCOMMON5(A,B,C,D,E)         SMSFLOG5(KSMSStackCommon,A,(B),(C),(D),(E))
#define LOGCOMMON6(A,B,C,D,E,F)       SMSFLOG6(KSMSStackCommon,A,(B),(C),(D),(E),(F))
#define LOGCOMMON7(A,B,C,D,E,F,G)     SMSFLOG7(KSMSStackCommon,A,(B),(C),(D),(E),(F),(G))
#define LOGCOMMON8(A,B,C,D,E,F,G,H)   SMSFLOG8(KSMSStackCommon,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGCOMMONTIMESTAMP()		  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGCOMMON5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGGSMU1(A)                   SMSFLOG1(KSMSStackGSMU,A)
#define LOGGSMU2(A,B)                 SMSFLOG2(KSMSStackGSMU,A,(B))
#define LOGGSMU3(A,B,C)               SMSFLOG3(KSMSStackGSMU,A,(B),(C))
#define LOGGSMU4(A,B,C,D)             SMSFLOG4(KSMSStackGSMU,A,(B),(C),(D))
#define LOGGSMU5(A,B,C,D,E)           SMSFLOG5(KSMSStackGSMU,A,(B),(C),(D),(E))
#define LOGGSMU6(A,B,C,D,E,F)         SMSFLOG6(KSMSStackGSMU,A,(B),(C),(D),(E),(F))
#define LOGGSMU7(A,B,C,D,E,F,G)       SMSFLOG7(KSMSStackGSMU,A,(B),(C),(D),(E),(F),(G))
#define LOGGSMU8(A,B,C,D,E,F,G,H)     SMSFLOG8(KSMSStackGSMU,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGGSMUTIMESTAMP()	          { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGGSMU5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGSMSPROT1(A)                SMSFLOG1(KSMSStackSMSPROT,A)
#define LOGSMSPROT2(A,B)              SMSFLOG2(KSMSStackSMSPROT,A,(B))
#define LOGSMSPROT3(A,B,C)            SMSFLOG3(KSMSStackSMSPROT,A,(B),(C))
#define LOGSMSPROT4(A,B,C,D)          SMSFLOG4(KSMSStackSMSPROT,A,(B),(C),(D))
#define LOGSMSPROT5(A,B,C,D,E)        SMSFLOG5(KSMSStackSMSPROT,A,(B),(C),(D),(E))
#define LOGSMSPROT6(A,B,C,D,E,F)      SMSFLOG6(KSMSStackSMSPROT,A,(B),(C),(D),(E),(F))
#define LOGSMSPROT7(A,B,C,D,E,F,G)    SMSFLOG7(KSMSStackSMSPROT,A,(B),(C),(D),(E),(F),(G))
#define LOGSMSPROT8(A,B,C,D,E,F,G,H)  SMSFLOG8(KSMSStackSMSPROT,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGSMSPROTTIMESTAMP()		  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGSMSPROT5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGSMSU1(A)                   SMSFLOG1(KSMSStackSMSU,A)
#define LOGSMSU2(A,B)                 SMSFLOG2(KSMSStackSMSU,A,(B))
#define LOGSMSU3(A,B,C)               SMSFLOG3(KSMSStackSMSU,A,(B),(C))
#define LOGSMSU4(A,B,C,D)             SMSFLOG4(KSMSStackSMSU,A,(B),(C),(D))
#define LOGSMSU5(A,B,C,D,E)           SMSFLOG5(KSMSStackSMSU,A,(B),(C),(D),(E))
#define LOGSMSU6(A,B,C,D,E,F)         SMSFLOG6(KSMSStackSMSU,A,(B),(C),(D),(E),(F))
#define LOGSMSU7(A,B,C,D,E,F,G)       SMSFLOG7(KSMSStackSMSU,A,(B),(C),(D),(E),(F),(G))
#define LOGSMSU8(A,B,C,D,E,F,G,H)     SMSFLOG8(KSMSStackSMSU,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGSMSUTIMESTAMP()		      { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGSMSU5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGSMSIF1(A)                  SMSFLOG1(KSMSStackSMSIF,A)
#define LOGSMSIF2(A,B)                SMSFLOG2(KSMSStackSMSIF,A,(B))
#define LOGSMSIF3(A,B,C)              SMSFLOG3(KSMSStackSMSIF,A,(B),(C))
#define LOGSMSIF4(A,B,C,D)            SMSFLOG4(KSMSStackSMSIF,A,(B),(C),(D))
#define LOGSMSIF5(A,B,C,D,E)          SMSFLOG5(KSMSStackSMSIF,A,(B),(C),(D),(E))
#define LOGSMSIF6(A,B,C,D,E,F)        SMSFLOG6(KSMSStackSMSIF,A,(B),(C),(D),(E),(F))
#define LOGSMSIF7(A,B,C,D,E,F,G)      SMSFLOG7(KSMSStackSMSIF,A,(B),(C),(D),(E),(F),(G))
#define LOGSMSIF8(A,B,C,D,E,F,G,H)    SMSFLOG8(KSMSStackSMSIF,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGSMSIFTIMESTAMP()		      { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGSMSIF5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }
#define LOGSMSIFPDU(A,B,C)            LogSmsIfPDU((A),(B),(C))
#define LOGSMSIFHEXBUF(A,B)           LogSmsIfHexBuf((A),(B))
#define LOGSMSIFTYPEOFNUMBER(A,B)     LogSmsIfTypeOfNumber((A),(B))
#define LOGSMSIFNUMBERINGPLAN(A,B)    LogSmsIfNumberingPlan((A),(B))
#define LOGSMSIFSMSENTRY(A,B)         LogSmsIfSmsEntry((A),(B))
#define LOGSMSIFSENDATTRIBUTES(A,B)   LogSmsIfSendAttributes((A),(B))

#define LOGWAPPROT1(A)                SMSFLOG1(KSMSStackWAPPROT,A)
#define LOGWAPPROT2(A,B)              SMSFLOG2(KSMSStackWAPPROT,A,(B))
#define LOGWAPPROT3(A,B,C)            SMSFLOG3(KSMSStackWAPPROT,A,(B),(C))
#define LOGWAPPROT4(A,B,C,D)          SMSFLOG4(KSMSStackWAPPROT,A,(B),(C),(D))
#define LOGWAPPROT5(A,B,C,D,E)        SMSFLOG5(KSMSStackWAPPROT,A,(B),(C),(D),(E))
#define LOGWAPPROT6(A,B,C,D,E,F)      SMSFLOG6(KSMSStackWAPPROT,A,(B),(C),(D),(E),(F))
#define LOGWAPPROT7(A,B,C,D,E,F,G)    SMSFLOG7(KSMSStackWAPPROT,A,(B),(C),(D),(E),(F),(G))
#define LOGWAPPROT8(A,B,C,D,E,F,G,H)  SMSFLOG8(KSMSStackWAPPROT,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGWAPPROTTIMESTAMP()		  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGWAPPROT5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#else  // _SMS_LOGGING_ENABLED

#define LOGCOMMON1(A)
#define LOGCOMMON2(A,B)
#define LOGCOMMON3(A,B,C)
#define LOGCOMMON4(A,B,C,D)
#define LOGCOMMON5(A,B,C,D,E)
#define LOGCOMMON6(A,B,C,D,E,F)
#define LOGCOMMON7(A,B,C,D,E,F,G)
#define LOGCOMMON8(A,B,C,D,E,F,G,H)
#define LOGCOMMONTIMESTAMP()

#define LOGGSMU1(A)
#define LOGGSMU2(A,B)
#define LOGGSMU3(A,B,C)
#define LOGGSMU4(A,B,C,D)
#define LOGGSMU5(A,B,C,D,E)
#define LOGGSMU6(A,B,C,D,E,F)
#define LOGGSMU7(A,B,C,D,E,F,G)
#define LOGGSMU8(A,B,C,D,E,F,G,H)
#define LOGGSMUTIMESTAMP()

#define LOGSMSPROT1(A)
#define LOGSMSPROT2(A,B)
#define LOGSMSPROT3(A,B,C)
#define LOGSMSPROT4(A,B,C,D)
#define LOGSMSPROT5(A,B,C,D,E)
#define LOGSMSPROT6(A,B,C,D,E,F)
#define LOGSMSPROT7(A,B,C,D,E,F,G)
#define LOGSMSPROT8(A,B,C,D,E,F,G,H)
#define LOGSMSPROTTIMESTAMP()

#define LOGSMSU1(A)
#define LOGSMSU2(A,B)
#define LOGSMSU3(A,B,C)
#define LOGSMSU4(A,B,C,D)
#define LOGSMSU5(A,B,C,D,E)
#define LOGSMSU6(A,B,C,D,E,F)
#define LOGSMSU7(A,B,C,D,E,F,G)
#define LOGSMSU8(A,B,C,D,E,F,G,H)
#define LOGSMSUTIMESTAMP()

#define LOGSMSIF1(A)
#define LOGSMSIF2(A,B)
#define LOGSMSIF3(A,B,C)
#define LOGSMSIF4(A,B,C,D)
#define LOGSMSIF5(A,B,C,D,E)
#define LOGSMSIF6(A,B,C,D,E,F)
#define LOGSMSIF7(A,B,C,D,E,F,G)
#define LOGSMSIF8(A,B,C,D,E,F,G,H)
#define LOGSMSIFTIMESTAMP()
#define LOGSMSIFPDU(A,B,C)
#define LOGSMSIFHEXBUF(A,B)
#define LOGSMSIFTYPEOFNUMBER(A,B)
#define LOGSMSIFNUMBERINGPLAN(A,B)
#define LOGSMSIFSMSENTRY(A,B)
#define LOGSMSIFSENDATTRIBUTES(A,B)

#define LOGWAPPROT1(A)
#define LOGWAPPROT2(A,B)
#define LOGWAPPROT3(A,B,C)
#define LOGWAPPROT4(A,B,C,D)
#define LOGWAPPROT5(A,B,C,D,E)
#define LOGWAPPROT6(A,B,C,D,E,F)
#define LOGWAPPROT7(A,B,C,D,E,F,G)
#define LOGWAPPROT8(A,B,C,D,E,F,G,H)
#define LOGWAPPROTTIMESTAMP()

#endif	// _SMS_LOGGING_ENABLED


//
// Functions to perform specific logging where necessary.
//
#ifdef _SMS_LOGGING_ENABLED
    void LogSmsIfPDU(const TDesC8& aText, const TDesC8& aPDU, TBool aCommandPdu);
    void LogSmsIfHexBuf(const TDesC8& aText, const TDesC8& aHexBuf);
    void LogSmsIfTypeOfNumber(const TDesC8& aText, RMobilePhone::TMobileTON aTON);
    void LogSmsIfNumberingPlan(const TDesC8& aText, RMobilePhone::TMobileNPI aNPI);
    void LogSmsIfSmsEntry(const TDesC8& aText,
    					  const RMobileSmsStore::TMobileGsmSmsEntryV1& aSmsGsmEntryV1);
    void LogSmsIfSendAttributes(const TDesC8& aText,
    							const RMobileSmsMessaging::TMobileSmsSendAttributesV1& aAttrib);
#endif

// Define macros to disable and restore Code Coverage
    
#ifndef BULLSEYE_OFF
#ifdef _BullseyeCoverage
#define BULLSEYE_OFF "BullseyeCoverage save off";
#define BULLSEYE_RESTORE "BullseyeCoverage restore";
#else
#define BULLSEYE_OFF 
#define BULLSEYE_RESTORE 
#endif
#endif

#endif  // _SMSSTACKLOG_H_
