// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __WATCHERLOG_H__
#define __WATCHERLOG_H__

/**
 *  @file
 *
 *  Logging macros for the Telephony Watchers. These use the Comms Debug
 *  Utility framework (aka Flogger V2).
 *
 *  To enable logging create a file called "comsdbg.ini" in the Symbian OS
 *  "C:\Logs\" directory which contains the following lines:
 *
 *    MEDIA FILE
 *    LOG TelWatchers Common
 *    LOG TelWatchers Indicator
 *    LOG TelWatchers Signal
 *
 *  Each line relates to a particular part of the Telephony Watchers,
 *  enabling you to log only the interesting bits.
 *
 *  By default logging is only present on UDEB builds. To enable UREL logging
 *  on a device, you will need to rebuild your ROM with UDEB components.
 *
 *  To enable logging on a device, place the "comsdbg.ini" file in the ROM
 *  under the "Z:\Logs\" directory. The log file will be written in the
 *  "C:\Logs\" directory on the device.
 *
 *  To enable logging to the serial port change the "MEDIA FILE" line to
 *  "MEDIA SERIAL::1" or "MEDIA SERIAL::2". Note that this may no longer
 *  work due to a change of feature implementation in the Comms Debug
 *  Framework.
 *
 *  If you wish to have individual logs for each component, then use the
 *  extractlog.pl utility found in the %EPOCROOT%\Epoc32\Tools\ directory. E.g.
 *
 *    extractlog.pl \EPOC32\WINSCW\C\LOGS\LOG.TXT
 *
 *  @internalComponent
 */

#include <comms-infras/commsdebugutility.h>

#ifdef _DEBUG
#define _WATCHER_LOGGING_ENABLED
#endif


#ifdef _WATCHER_LOGGING_ENABLED

_LIT8(KTelWatcherSubSystem, "TelWatchers");

#define WATCHERFLOG1(N,A)                { \
									     _LIT8(KLogString, A); \
									     __FLOG_STATIC0(KTelWatcherSubSystem,(N),KLogString); \
									     }
#define WATCHERFLOG2(N,A,B)              { \
									     _LIT8(KLogString, A); \
									     __FLOG_STATIC1(KTelWatcherSubSystem,(N),KLogString,(B)); \
									     }
#define WATCHERFLOG3(N,A,B,C)            { \
									     _LIT8(KLogString, A); \
									     __FLOG_STATIC2(KTelWatcherSubSystem,(N),KLogString,(B),(C)); \
									     }
#define WATCHERFLOG4(N,A,B,C,D)          { \
									     _LIT8(KLogString, A); \
									     __FLOG_STATIC3(KTelWatcherSubSystem,(N),KLogString,(B),(C),(D)); \
									     }
#define WATCHERFLOG5(N,A,B,C,D,E)        { \
									     _LIT8(KLogString, A); \
									     __FLOG_STATIC4(KTelWatcherSubSystem,(N),KLogString,(B),(C),(D),(E)); \
									     }
#define WATCHERFLOG6(N,A,B,C,D,E,F)      { \
									     _LIT8(KLogString, A); \
									     __FLOG_STATIC5(KTelWatcherSubSystem,(N),KLogString,(B),(C),(D),(E),(F)); \
									     }
#define WATCHERFLOG7(N,A,B,C,D,E,F,G)    { \
									     _LIT8(KLogString, A); \
									     __FLOG_STATIC6(KTelWatcherSubSystem,(N),KLogString,(B),(C),(D),(E),(F),(G)); \
									     }
#define WATCHERFLOG8(N,A,B,C,D,E,F,G,H)  { \
									     _LIT8(KLogString, A); \
									     __FLOG_STATIC7(KTelWatcherSubSystem,(N),KLogString,(B),(C),(D),(E),(F),(G),(H)); \
									     }

_LIT8(KTelWatcherCommon, "Common");
_LIT8(KTelWatcherIndicator, "Indicator");
_LIT8(KTelWatcherSignal, "Signal");


#define LOGCOMMON1(A)                      WATCHERFLOG1(KTelWatcherCommon,A)
#define LOGCOMMON2(A,B)                    WATCHERFLOG2(KTelWatcherCommon,A,(B))
#define LOGCOMMON3(A,B,C)                  WATCHERFLOG3(KTelWatcherCommon,A,(B),(C))
#define LOGCOMMON4(A,B,C,D)                WATCHERFLOG4(KTelWatcherCommon,A,(B),(C),(D))
#define LOGCOMMON5(A,B,C,D,E)              WATCHERFLOG5(KTelWatcherCommon,A,(B),(C),(D),(E))
#define LOGCOMMON6(A,B,C,D,E,F)            WATCHERFLOG6(KTelWatcherCommon,A,(B),(C),(D),(E),(F))
#define LOGCOMMON7(A,B,C,D,E,F,G)          WATCHERFLOG7(KTelWatcherCommon,A,(B),(C),(D),(E),(F),(G))
#define LOGCOMMON8(A,B,C,D,E,F,G,H)        WATCHERFLOG8(KTelWatcherCommon,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGCOMMONTIMESTAMP()		       { \
									       TTime  now; \
									       now.UniversalTime(); \
									       TDateTime dateTime(now.DateTime()); \
									       LOGCOMMON5("TimeStamp=%02d:%02d:%02d.%06d", \
									       dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									       }

#define LOGINDICATOR1(A)                   WATCHERFLOG1(KTelWatcherIndicator,A)
#define LOGINDICATOR2(A,B)                 WATCHERFLOG2(KTelWatcherIndicator,A,(B))
#define LOGINDICATOR3(A,B,C)               WATCHERFLOG3(KTelWatcherIndicator,A,(B),(C))
#define LOGINDICATOR4(A,B,C,D)             WATCHERFLOG4(KTelWatcherIndicator,A,(B),(C),(D))
#define LOGINDICATOR5(A,B,C,D,E)           WATCHERFLOG5(KTelWatcherIndicator,A,(B),(C),(D),(E))
#define LOGINDICATOR6(A,B,C,D,E,F)         WATCHERFLOG6(KTelWatcherIndicator,A,(B),(C),(D),(E),(F))
#define LOGINDICATOR7(A,B,C,D,E,F,G)       WATCHERFLOG7(KTelWatcherIndicator,A,(B),(C),(D),(E),(F),(G))
#define LOGINDICATOR8(A,B,C,D,E,F,G,H)     WATCHERFLOG8(KTelWatcherIndicator,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGINDICATORTIMESTAMP()	           { \
									       TTime  now; \
									       now.UniversalTime(); \
									       TDateTime dateTime(now.DateTime()); \
									       LOGINDICATOR5("TimeStamp=%02d:%02d:%02d.%06d", \
									       dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									       }

#define LOGSIGNAL1(A)                      WATCHERFLOG1(KTelWatcherSignal,A)
#define LOGSIGNAL2(A,B)                    WATCHERFLOG2(KTelWatcherSignal,A,(B))
#define LOGSIGNAL3(A,B,C)                  WATCHERFLOG3(KTelWatcherSignal,A,(B),(C))
#define LOGSIGNAL4(A,B,C,D)                WATCHERFLOG4(KTelWatcherSignal,A,(B),(C),(D))
#define LOGSIGNAL5(A,B,C,D,E)              WATCHERFLOG5(KTelWatcherSignal,A,(B),(C),(D),(E))
#define LOGSIGNAL6(A,B,C,D,E,F)            WATCHERFLOG6(KTelWatcherSignal,A,(B),(C),(D),(E),(F))
#define LOGSIGNAL7(A,B,C,D,E,F,G)          WATCHERFLOG7(KTelWatcherSignal,A,(B),(C),(D),(E),(F),(G))
#define LOGSIGNAL8(A,B,C,D,E,F,G,H)        WATCHERFLOG8(KTelWatcherSignal,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGSIGNALTIMESTAMP()		       { \
									       TTime  now; \
									       now.UniversalTime(); \
									       TDateTime dateTime(now.DateTime()); \
									       LOGSIGNAL5("TimeStamp=%02d:%02d:%02d.%06d", \
									       dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									       }

#else  // _WATCHER_LOGGING_ENABLED

#define LOGCOMMON1(A)
#define LOGCOMMON2(A,B)
#define LOGCOMMON3(A,B,C)
#define LOGCOMMON4(A,B,C,D)
#define LOGCOMMON5(A,B,C,D,E)
#define LOGCOMMON6(A,B,C,D,E,F)
#define LOGCOMMON7(A,B,C,D,E,F,G)
#define LOGCOMMON8(A,B,C,D,E,F,G,H)
#define LOGCOMMONTIMESTAMP()

#define LOGINDICATOR1(A)
#define LOGINDICATOR2(A,B)
#define LOGINDICATOR3(A,B,C)
#define LOGINDICATOR4(A,B,C,D)
#define LOGINDICATOR5(A,B,C,D,E)
#define LOGINDICATOR6(A,B,C,D,E,F)
#define LOGINDICATOR7(A,B,C,D,E,F,G)
#define LOGINDICATOR8(A,B,C,D,E,F,G,H)
#define LOGINDICATORTIMESTAMP()

#define LOGSIGNAL1(A)
#define LOGSIGNAL2(A,B)
#define LOGSIGNAL3(A,B,C)
#define LOGSIGNAL4(A,B,C,D)
#define LOGSIGNAL5(A,B,C,D,E)
#define LOGSIGNAL6(A,B,C,D,E,F)
#define LOGSIGNAL7(A,B,C,D,E,F,G)
#define LOGSIGNAL8(A,B,C,D,E,F,G,H)
#define LOGSIGNALTIMESTAMP()

#endif	// _WATCHER_LOGGING_ENABLED

#endif // __WATCHERLOG_H__

