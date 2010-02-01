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

#ifndef _SIMTSYLOG_H_
#define _SIMTSYLOG_H_

/**
 *  @file
 *
 *  Logging macros for SimTSY. These use the Comms Debug Utility framework
 *  (aka Flogger V2).
 *
 *  To enable logging create a file called "comsdbg.ini" in the Symbian OS
 *  "C:\Logs\" directory which contains the following lines:
 *
 *    MEDIA FILE
 *    LOG SIMTSY Common
 *    LOG SIMTSY Config
 *    LOG SIMTSY Phone
 *    LOG SIMTSY Line
 *    LOG SIMTSY Call
 *    LOG SIMTSY Voice
 *    LOG SIMTSY Data
 *    LOG SIMTSY Network
 *    LOG SIMTSY PhBk
 *    LOG SIMTSY Misc
 *    LOG SIMTSY Sms
 *    LOG SIMTSY Packet
 *    LOG SIMTSY Ss
 *
 *  Each line relates to a particular part of the SIMTSY, enabling you to
 *  log only the parts of interest.
 *
 *  By default logging is only present on UDEB builds. To enable UREL logging
 *  on a device, you will need to rebuild your ROM with UDEB SIMTSY components.
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
#define _SIMTSY_LOGGING_ENABLED
#endif


#ifdef _SIMTSY_LOGGING_ENABLED

_LIT8(KSIMTSYSubSystem, "SIMTSY");

#define SIMTSYFLOG1(N,A)                 { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC0(KSIMTSYSubSystem,(N),KLogString); \
									  }
#define SIMTSYFLOG2(N,A,B)               { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC1(KSIMTSYSubSystem,(N),KLogString,(B)); \
									  }
#define SIMTSYFLOG3(N,A,B,C)             { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC2(KSIMTSYSubSystem,(N),KLogString,(B),(C)); \
									  }
#define SIMTSYFLOG4(N,A,B,C,D)           { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC3(KSIMTSYSubSystem,(N),KLogString,(B),(C),(D)); \
									  }
#define SIMTSYFLOG5(N,A,B,C,D,E)         { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC4(KSIMTSYSubSystem,(N),KLogString,(B),(C),(D),(E)); \
									  }
#define SIMTSYFLOG6(N,A,B,C,D,E,F)       { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC5(KSIMTSYSubSystem,(N),KLogString,(B),(C),(D),(E),(F)); \
									  }
#define SIMTSYFLOG7(N,A,B,C,D,E,F,G)     { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC6(KSIMTSYSubSystem,(N),KLogString,(B),(C),(D),(E),(F),(G)); \
									  }
#define SIMTSYFLOG8(N,A,B,C,D,E,F,G,H)   { \
									  _LIT8(KLogString, A); \
									  __FLOG_STATIC7(KSIMTSYSubSystem,(N),KLogString,(B),(C),(D),(E),(F),(G),(H)); \
									  }

_LIT8(KSIMTSYCommon,	"Common");
_LIT8(KSIMTSYConfig,	"Config");
_LIT8(KSIMTSYPhone,		"Phone");
_LIT8(KSIMTSYLine,		"Line");
_LIT8(KSIMTSYCall,		"Call");
_LIT8(KSIMTSYVoice,		"Voice");
_LIT8(KSIMTSYData,		"Data");
_LIT8(KSIMTSYNetwork,	"Network");
_LIT8(KSIMTSYPhBk,		"PhBk");
_LIT8(KSIMTSYMisc,		"Misc");
_LIT8(KSIMTSYSms,		"Sms");
_LIT8(KSIMTSYPacket,	"Packet");
_LIT8(KSIMTSYSs,		"Ss");


#define LOGSCOMMON1(A)                SIMTSYFLOG1(KSIMTSYCommon,A)
#define LOGSCOMMON2(A,B)              SIMTSYFLOG2(KSIMTSYCommon,A,(B))
#define LOGSCOMMON3(A,B,C)            SIMTSYFLOG3(KSIMTSYCommon,A,(B),(C))
#define LOGSCOMMON4(A,B,C,D)          SIMTSYFLOG4(KSIMTSYCommon,A,(B),(C),(D))
#define LOGSCOMMON5(A,B,C,D,E)        SIMTSYFLOG5(KSIMTSYCommon,A,(B),(C),(D),(E))
#define LOGSCOMMON6(A,B,C,D,E,F)      SIMTSYFLOG6(KSIMTSYCommon,A,(B),(C),(D),(E),(F))
#define LOGSCOMMON7(A,B,C,D,E,F,G)    SIMTSYFLOG7(KSIMTSYCommon,A,(B),(C),(D),(E),(F),(G))
#define LOGSCOMMON8(A,B,C,D,E,F,G,H)  SIMTSYFLOG8(KSIMTSYCommon,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGSCOMMONTIMESTAMP()		  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGSCOMMON5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGCONFIG1(A)                 SIMTSYFLOG1(KSIMTSYConfig,A)
#define LOGCONFIG2(A,B)               SIMTSYFLOG2(KSIMTSYConfig,A,(B))
#define LOGCONFIG3(A,B,C)             SIMTSYFLOG3(KSIMTSYConfig,A,(B),(C))
#define LOGCONFIG4(A,B,C,D)           SIMTSYFLOG4(KSIMTSYConfig,A,(B),(C),(D))
#define LOGCONFIG5(A,B,C,D,E)         SIMTSYFLOG5(KSIMTSYConfig,A,(B),(C),(D),(E))
#define LOGCONFIG6(A,B,C,D,E,F)       SIMTSYFLOG6(KSIMTSYConfig,A,(B),(C),(D),(E),(F))
#define LOGCONFIG7(A,B,C,D,E,F,G)     SIMTSYFLOG7(KSIMTSYConfig,A,(B),(C),(D),(E),(F),(G))
#define LOGCONFIG8(A,B,C,D,E,F,G,H)   SIMTSYFLOG8(KSIMTSYConfig,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGCONFIGTIMESTAMP()		  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGCONFIG5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }
#define LOGPARSERR(A,B,C,D)		  	  LOGCONFIG7("WARNING - CONFIGURATION FILE PARSING - %s at line: %d. Reading element no. %d (%s) from tag %S returned %d.",__FILE__,__LINE__,C,A,D,B)
#define LOGPARSERREXP(A,B,C,D,E)	  LOGCONFIG8("WARNING - CONFIGURATION FILE PARSING - %s at line: %d. Reading element no. %d (%s) from tag %S returned %d, expected %d.",__FILE__,__LINE__,C,A,D,B,E)
#define LOGPARSERRANGE(A,B,C,D,E)	  LOGCONFIG8("WARNING - CONFIGURATION FILE PARSING - %s at line: %d. Reading element no. %d (%s) from tag %S returned %d, expected %s.",__FILE__,__LINE__,C,A,D,B,E)

#define LOGPHONE1(A)                  SIMTSYFLOG1(KSIMTSYPhone,A)
#define LOGPHONE2(A,B)                SIMTSYFLOG2(KSIMTSYPhone,A,(B))
#define LOGPHONE3(A,B,C)              SIMTSYFLOG3(KSIMTSYPhone,A,(B),(C))
#define LOGPHONE4(A,B,C,D)            SIMTSYFLOG4(KSIMTSYPhone,A,(B),(C),(D))
#define LOGPHONE5(A,B,C,D,E)          SIMTSYFLOG5(KSIMTSYPhone,A,(B),(C),(D),(E))
#define LOGPHONE6(A,B,C,D,E,F)        SIMTSYFLOG6(KSIMTSYPhone,A,(B),(C),(D),(E),(F))
#define LOGPHONE7(A,B,C,D,E,F,G)      SIMTSYFLOG7(KSIMTSYPhone,A,(B),(C),(D),(E),(F),(G))
#define LOGPHONE8(A,B,C,D,E,F,G,H)    SIMTSYFLOG8(KSIMTSYPhone,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGPHONETIMESTAMP()		      { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGPHONE5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }


#define LOGLINE1(A)       			  SIMTSYFLOG1(KSIMTSYLine,A)
#define LOGLINE2(A,B)                 SIMTSYFLOG2(KSIMTSYLine,A,(B))
#define LOGLINE3(A,B,C)               SIMTSYFLOG3(KSIMTSYLine,A,(B),(C))
#define LOGLINE4(A,B,C,D)             SIMTSYFLOG4(KSIMTSYLine,A,(B),(C),(D))
#define LOGLINE5(A,B,C,D,E)           SIMTSYFLOG5(KSIMTSYLine,A,(B),(C),(D),(E))
#define LOGLINE6(A,B,C,D,E,F)         SIMTSYFLOG6(KSIMTSYLine,A,(B),(C),(D),(E),(F))
#define LOGLINE7(A,B,C,D,E,F,G)       SIMTSYFLOG7(KSIMTSYLine,A,(B),(C),(D),(E),(F),(G))
#define LOGLINE8(A,B,C,D,E,F,G,H)     SIMTSYFLOG8(KSIMTSYLine,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGLINETIMESTAMP()		      { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGLINE5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGCALL1(A)                   SIMTSYFLOG1(KSIMTSYCall,A)
#define LOGCALL2(A,B)                 SIMTSYFLOG2(KSIMTSYCall,A,(B))
#define LOGCALL3(A,B,C)               SIMTSYFLOG3(KSIMTSYCall,A,(B),(C))
#define LOGCALL4(A,B,C,D)             SIMTSYFLOG4(KSIMTSYCall,A,(B),(C),(D))
#define LOGCALL5(A,B,C,D,E)           SIMTSYFLOG5(KSIMTSYCall,A,(B),(C),(D),(E))
#define LOGCALL6(A,B,C,D,E,F)         SIMTSYFLOG6(KSIMTSYCall,A,(B),(C),(D),(E),(F))
#define LOGCALL7(A,B,C,D,E,F,G)       SIMTSYFLOG7(KSIMTSYCall,A,(B),(C),(D),(E),(F),(G))
#define LOGCALL8(A,B,C,D,E,F,G,H)     SIMTSYFLOG8(KSIMTSYCall,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGCALLTIMESTAMP()		  	  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGCALL5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGVOICE1(A)                  SIMTSYFLOG1(KSIMTSYVoice,A)
#define LOGVOICE2(A,B)                SIMTSYFLOG2(KSIMTSYVoice,A,(B))
#define LOGVOICE3(A,B,C)              SIMTSYFLOG3(KSIMTSYVoice,A,(B),(C))
#define LOGVOICE4(A,B,C,D)            SIMTSYFLOG4(KSIMTSYVoice,A,(B),(C),(D))
#define LOGVOICE5(A,B,C,D,E)          SIMTSYFLOG5(KSIMTSYVoice,A,(B),(C),(D),(E))
#define LOGVOICE6(A,B,C,D,E,F)        SIMTSYFLOG6(KSIMTSYVoice,A,(B),(C),(D),(E),(F))
#define LOGVOICE7(A,B,C,D,E,F,G)      SIMTSYFLOG7(KSIMTSYVoice,A,(B),(C),(D),(E),(F),(G))
#define LOGVOICE8(A,B,C,D,E,F,G,H)    SIMTSYFLOG8(KSIMTSYVoice,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGVOICETIMESTAMP()			  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGVOICE5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGDATA1(A)      			  SIMTSYFLOG1(KSIMTSYData,A)
#define LOGDATA2(A,B)                 SIMTSYFLOG2(KSIMTSYData,A,(B))
#define LOGDATA3(A,B,C)               SIMTSYFLOG3(KSIMTSYData,A,(B),(C))
#define LOGDATA4(A,B,C,D)             SIMTSYFLOG4(KSIMTSYData,A,(B),(C),(D))
#define LOGDATA5(A,B,C,D,E)           SIMTSYFLOG5(KSIMTSYData,A,(B),(C),(D),(E))
#define LOGDATA6(A,B,C,D,E,F)         SIMTSYFLOG6(KSIMTSYData,A,(B),(C),(D),(E),(F))
#define LOGDATA7(A,B,C,D,E,F,G)       SIMTSYFLOG7(KSIMTSYData,A,(B),(C),(D),(E),(F),(G))
#define LOGDATA8(A,B,C,D,E,F,G,H)     SIMTSYFLOG8(KSIMTSYData,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGDATATIMESTAMP()		  	  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGDATA5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGNETWORK1(A)                SIMTSYFLOG1(KSIMTSYNetwork,A)
#define LOGNETWORK2(A,B)              SIMTSYFLOG2(KSIMTSYNetwork,A,(B))
#define LOGNETWORK3(A,B,C)            SIMTSYFLOG3(KSIMTSYNetwork,A,(B),(C))
#define LOGNETWORK4(A,B,C,D)          SIMTSYFLOG4(KSIMTSYNetwork,A,(B),(C),(D))
#define LOGNETWORK5(A,B,C,D,E)        SIMTSYFLOG5(KSIMTSYNetwork,A,(B),(C),(D),(E))
#define LOGNETWORK6(A,B,C,D,E,F)      SIMTSYFLOG6(KSIMTSYNetwork,A,(B),(C),(D),(E),(F))
#define LOGNETWORK7(A,B,C,D,E,F,G)    SIMTSYFLOG7(KSIMTSYNetwork,A,(B),(C),(D),(E),(F),(G))
#define LOGNETWORK8(A,B,C,D,E,F,G,H)  SIMTSYFLOG8(KSIMTSYNetwork,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGNETWORKTIMESTAMP()		  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGNETWORK5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGPHBK1(A)                   SIMTSYFLOG1(KSIMTSYPhBk,A)
#define LOGPHBK2(A,B)                 SIMTSYFLOG2(KSIMTSYPhBk,A,(B))
#define LOGPHBK3(A,B,C)               SIMTSYFLOG3(KSIMTSYPhBk,A,(B),(C))
#define LOGPHBK4(A,B,C,D)             SIMTSYFLOG4(KSIMTSYPhBk,A,(B),(C),(D))
#define LOGPHBK5(A,B,C,D,E)           SIMTSYFLOG5(KSIMTSYPhBk,A,(B),(C),(D),(E))
#define LOGPHBK6(A,B,C,D,E,F)         SIMTSYFLOG6(KSIMTSYPhBk,A,(B),(C),(D),(E),(F))
#define LOGPHBK7(A,B,C,D,E,F,G)       SIMTSYFLOG7(KSIMTSYPhBk,A,(B),(C),(D),(E),(F),(G))
#define LOGPHBK8(A,B,C,D,E,F,G,H)     SIMTSYFLOG8(KSIMTSYPhBk,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGPHBKTIMESTAMP()		  	  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGPHBK5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGMISC1(A)                   SIMTSYFLOG1(KSIMTSYMisc,A)
#define LOGMISC2(A,B)                 SIMTSYFLOG2(KSIMTSYMisc,A,(B))
#define LOGMISC3(A,B,C)               SIMTSYFLOG3(KSIMTSYMisc,A,(B),(C))
#define LOGMISC4(A,B,C,D)             SIMTSYFLOG4(KSIMTSYMisc,A,(B),(C),(D))
#define LOGMISC5(A,B,C,D,E)           SIMTSYFLOG5(KSIMTSYMisc,A,(B),(C),(D),(E))
#define LOGMISC6(A,B,C,D,E,F)         SIMTSYFLOG6(KSIMTSYMisc,A,(B),(C),(D),(E),(F))
#define LOGMISC7(A,B,C,D,E,F,G)       SIMTSYFLOG7(KSIMTSYMisc,A,(B),(C),(D),(E),(F),(G))
#define LOGMISC8(A,B,C,D,E,F,G,H)	  SIMTSYFLOG8(KSIMTSYMisc,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGMISCTIMESTAMP()		  	  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGMISC5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGSMS1(A)                 	  SIMTSYFLOG1(KSIMTSYSms,A)
#define LOGSMS2(A,B)               	  SIMTSYFLOG2(KSIMTSYSms,A,(B))
#define LOGSMS3(A,B,C)          	  SIMTSYFLOG3(KSIMTSYSms,A,(B),(C))
#define LOGSMS4(A,B,C,D)          	  SIMTSYFLOG4(KSIMTSYSms,A,(B),(C),(D))
#define LOGSMS5(A,B,C,D,E)         	  SIMTSYFLOG5(KSIMTSYSms,A,(B),(C),(D),(E))
#define LOGSMS6(A,B,C,D,E,F)       	  SIMTSYFLOG6(KSIMTSYSms,A,(B),(C),(D),(E),(F))
#define LOGSMS7(A,B,C,D,E,F,G)     	  SIMTSYFLOG7(KSIMTSYSms,A,(B),(C),(D),(E),(F),(G))
#define LOGSMS8(A,B,C,D,E,F,G,H)   	  SIMTSYFLOG8(KSIMTSYSms,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGSMSTIMESTAMP()		  	  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGSMS5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGPACKET1(A)                 SIMTSYFLOG1(KSIMTSYPacket,A)
#define LOGPACKET2(A,B)               SIMTSYFLOG2(KSIMTSYPacket,A,(B))
#define LOGPACKET3(A,B,C)             SIMTSYFLOG3(KSIMTSYPacket,A,(B),(C))
#define LOGPACKET4(A,B,C,D)           SIMTSYFLOG4(KSIMTSYPacket,A,(B),(C),(D))
#define LOGPACKET5(A,B,C,D,E)         SIMTSYFLOG5(KSIMTSYPacket,A,(B),(C),(D),(E))
#define LOGPACKET6(A,B,C,D,E,F)       SIMTSYFLOG6(KSIMTSYPacket,A,(B),(C),(D),(E),(F))
#define LOGPACKET7(A,B,C,D,E,F,G)     SIMTSYFLOG7(KSIMTSYPacket,A,(B),(C),(D),(E),(F),(G))
#define LOGPACKET8(A,B,C,D,E,F,G,H)   SIMTSYFLOG8(KSIMTSYPacket,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGPACKETTIMESTAMP()		  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGPACKET5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }

#define LOGSS1(A)               	  SIMTSYFLOG1(KSIMTSYSs,A)
#define LOGSS2(A,B)             	  SIMTSYFLOG2(KSIMTSYSs,A,(B))
#define LOGSS3(A,B,C)           	  SIMTSYFLOG3(KSIMTSYSs,A,(B),(C))
#define LOGSS4(A,B,C,D)         	  SIMTSYFLOG4(KSIMTSYSs,A,(B),(C),(D))
#define LOGSS5(A,B,C,D,E)       	  SIMTSYFLOG5(KSIMTSYSs,A,(B),(C),(D),(E))
#define LOGSS6(A,B,C,D,E,F)     	  SIMTSYFLOG6(KSIMTSYSs,A,(B),(C),(D),(E),(F))
#define LOGSS7(A,B,C,D,E,F,G)   	  SIMTSYFLOG7(KSIMTSYSs,A,(B),(C),(D),(E),(F),(G))
#define LOGSS8(A,B,C,D,E,F,G,H) 	  SIMTSYFLOG8(KSIMTSYSs,A,(B),(C),(D),(E),(F),(G),(H))
#define LOGSSTIMESTAMP()		  	  { \
									  TTime  now; \
									  now.UniversalTime(); \
									  TDateTime dateTime(now.DateTime()); \
									  LOGSS5("TimeStamp=%02d:%02d:%02d.%06d", \
									  dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond()); \
									  }



#else  // _SIMTSY_LOGGING_ENABLED

#define LOGSCOMMON1(A)
#define LOGSCOMMON2(A,B)
#define LOGSCOMMON3(A,B,C)
#define LOGSCOMMON4(A,B,C,D)
#define LOGSCOMMON5(A,B,C,D,E)
#define LOGSCOMMON6(A,B,C,D,E,F)
#define LOGSCOMMON7(A,B,C,D,E,F,G)
#define LOGSCOMMON8(A,B,C,D,E,F,G,H)
#define LOGSCOMMONTIMESTAMP()

#define LOGCONFIG1(A)              
#define LOGCONFIG2(A,B)            
#define LOGCONFIG3(A,B,C)          
#define LOGCONFIG4(A,B,C,D)        
#define LOGCONFIG5(A,B,C,D,E)      
#define LOGCONFIG6(A,B,C,D,E,F)    
#define LOGCONFIG7(A,B,C,D,E,F,G)  
#define LOGCONFIG8(A,B,C,D,E,F,G,H)
#define LOGCONFIGTIMESTAMP()	

#define LOGPARSERR(A,B,C,D)
#define LOGPARSERREXP(A,B,C,D,E)
#define LOGPARSERRANGE(A,B,C,D,E)
								
#define LOGPHONE1(A)                  
#define LOGPHONE2(A,B)                
#define LOGPHONE3(A,B,C)              
#define LOGPHONE4(A,B,C,D)            
#define LOGPHONE5(A,B,C,D,E)          
#define LOGPHONE6(A,B,C,D,E,F)        
#define LOGPHONE7(A,B,C,D,E,F,G)      
#define LOGPHONE8(A,B,C,D,E,F,G,H)    
#define LOGPHONETIMESTAMP()		      

#define LOGLINE1(A)       			  
#define LOGLINE2(A,B)                 
#define LOGLINE3(A,B,C)               
#define LOGLINE4(A,B,C,D)             
#define LOGLINE5(A,B,C,D,E)           
#define LOGLINE6(A,B,C,D,E,F)         
#define LOGLINE7(A,B,C,D,E,F,G)       
#define LOGLINE8(A,B,C,D,E,F,G,H)     
#define LOGLINETIMESTAMP()		      

#define LOGCALL1(A)                   
#define LOGCALL2(A,B)                 
#define LOGCALL3(A,B,C)               
#define LOGCALL4(A,B,C,D)             
#define LOGCALL5(A,B,C,D,E)           
#define LOGCALL6(A,B,C,D,E,F)         
#define LOGCALL7(A,B,C,D,E,F,G)       
#define LOGCALL8(A,B,C,D,E,F,G,H)     
#define LOGCALLTIMESTAMP()		  	  
									  
#define LOGVOICE1(A)               
#define LOGVOICE2(A,B)             
#define LOGVOICE3(A,B,C)           
#define LOGVOICE4(A,B,C,D)         
#define LOGVOICE5(A,B,C,D,E)       
#define LOGVOICE6(A,B,C,D,E,F)     
#define LOGVOICE7(A,B,C,D,E,F,G)   
#define LOGVOICE8(A,B,C,D,E,F,G,H) 
#define LOGVOICETIMESTAMP()		

#define LOGDATA1(A)      		
#define LOGDATA2(A,B)              
#define LOGDATA3(A,B,C)            
#define LOGDATA4(A,B,C,D)          
#define LOGDATA5(A,B,C,D,E)        
#define LOGDATA6(A,B,C,D,E,F)      
#define LOGDATA7(A,B,C,D,E,F,G)    
#define LOGDATA8(A,B,C,D,E,F,G,H)  
#define LOGDATATIMESTAMP()		  
								
#define LOGNETWORK1(A)             
#define LOGNETWORK2(A,B)           
#define LOGNETWORK3(A,B,C)         
#define LOGNETWORK4(A,B,C,D)       
#define LOGNETWORK5(A,B,C,D,E)     
#define LOGNETWORK6(A,B,C,D,E,F)   
#define LOGNETWORK7(A,B,C,D,E,F,G) 
#define LOGNETWORK8(A,B,C,D,E,F,G,H)
#define LOGNETWORKTIMESTAMP()	

#define LOGPHBK1(A)                
#define LOGPHBK2(A,B)              
#define LOGPHBK3(A,B,C)            
#define LOGPHBK4(A,B,C,D)          
#define LOGPHBK5(A,B,C,D,E)        
#define LOGPHBK6(A,B,C,D,E,F)      
#define LOGPHBK7(A,B,C,D,E,F,G)    
#define LOGPHBK8(A,B,C,D,E,F,G,H)  
#define LOGPHBKTIMESTAMP()		  
								
#define LOGMISC1(A)                
#define LOGMISC2(A,B)              
#define LOGMISC3(A,B,C)            
#define LOGMISC4(A,B,C,D)          
#define LOGMISC5(A,B,C,D,E)        
#define LOGMISC6(A,B,C,D,E,F)      
#define LOGMISC7(A,B,C,D,E,F,G)    
#define LOGMISC8(A,B,C,D,E,F,G,H)
#define LOGMISCTIMESTAMP()		  

#define LOGSMS1(A)                 
#define LOGSMS2(A,B)               
#define LOGSMS3(A,B,C)             
#define LOGSMS4(A,B,C,D)           
#define LOGSMS5(A,B,C,D,E)         
#define LOGSMS6(A,B,C,D,E,F)       
#define LOGSMS7(A,B,C,D,E,F,G)     
#define LOGSMS8(A,B,C,D,E,F,G,H)   
#define LOGSMSTIMESTAMP()
								
#define LOGPACKET1(A)              
#define LOGPACKET2(A,B)            
#define LOGPACKET3(A,B,C)          
#define LOGPACKET4(A,B,C,D)        
#define LOGPACKET5(A,B,C,D,E)      
#define LOGPACKET6(A,B,C,D,E,F)    
#define LOGPACKET7(A,B,C,D,E,F,G)  
#define LOGPACKET8(A,B,C,D,E,F,G,H)
#define LOGPACKETTIMESTAMP()	

#define LOGSS1(A)               
#define LOGSS2(A,B)             
#define LOGSS3(A,B,C)           
#define LOGSS4(A,B,C,D)         
#define LOGSS5(A,B,C,D,E)       
#define LOGSS6(A,B,C,D,E,F)     
#define LOGSS7(A,B,C,D,E,F,G)   
#define LOGSS8(A,B,C,D,E,F,G,H) 
#define LOGSSTIMESTAMP()		  



#endif	// _SIMTSY_LOGGING_ENABLED

#endif  // _SIMTSYLOG_H_


