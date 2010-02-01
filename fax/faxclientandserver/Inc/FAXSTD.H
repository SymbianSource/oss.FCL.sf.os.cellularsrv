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
 @publishedPartner
*/

#if !defined(__FaxStd_H__)
#define __FaxStd_H__

// we start off with data constants

// error codes

/**
Could not connect to the fax machine. The fax modem has failed. 
@publishedPartner
@released
*/
const TInt KFaxErrModemNotWorking = -101;    

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrCannotConnect = -102;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrCSIorCIG = -103;            

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrDISorDTC = -104;            

/**
The connection to the fax machine has been broken. 
@publishedPartner
@released
*/
const TInt KFaxErrFrameFail = -105;           

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrAtNegotiatedSpeed = -106;   

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrCFR = -107;                 

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrMCF = -108;                 

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrCRP = -109;                 

/**
Could not connect to the fax machine. Could not dial fax number. 
@publishedPartner
@released
*/
const TInt KFaxErrNoDial = -110;              

/**
Could not connect to the fax machine. No dial tone was detected. 
@publishedPartner
@released
*/
const TInt KFaxErrNoDialTone = -111;          

/**
Could not connect to the fax machine. The line is busy. 
@publishedPartner
@released
*/
const TInt KFaxErrBusy = -112;                

/**
Could not connect to the fax machine. The call was not answered. 
@publishedPartner
@released
*/
const TInt KFaxErrNoAnswer = -113;            

/**
Could not connect to the fax machine. The call was not answered by a fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrNoCarrier = -114;           

/**
Could not send to the fax machine. The fax machine is unable to receive faxes. 
@publishedPartner
@released
*/
const TInt KFaxErrRemoteCannotReceive = -115; 

/**
Panic. 
@publishedPartner
@released
*/
const TInt KFaxErrCompression = -116;         

/**
Panic. 
@publishedPartner
@released
*/
const TInt KFaxErrPageTooLong = -117;         

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrDCN = -118;                 

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrRemoteDCN = -119;           

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrHDLC = -120;                

/**
Panic. 
@publishedPartner
@released
*/
const TInt KFaxErrStopAndWait = -121;         

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrTrainStop = -122;           

/**
Could not send to the fax machine. The connection has timed out. 
@publishedPartner
@released
*/
const TInt KFaxErrReceiveTimeout = -123;      

/**
The connection to the fax machine has been broken. 
@publishedPartner
@released
*/
const TInt KFaxErrCannotEndData = -124;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrEOP = -125;                 

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrMPS = -126;                 

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrNoReceiveMode = -127;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrCannotAnswer = -128;        

/**
Could not receive fax. The fax machine ended the call. 
@publishedPartner
@released
*/
const TInt KFaxErrPrematureHangup = -129;     

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrModemResponse = -130;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrPrematureOK = -131;         

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrNoFinalOK = -132;           

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrUnknownPageCode = -133;     

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrNoHangup = -134;            

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrNoNegotiate = -135;         

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrModemDisconnect = -136;     

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrWrongModemType = -137;      

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxErrTrainFail = -138;           

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxTransmitterStalled = -139;     

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxReceiverOverflow = -140;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxCannotOpenPort = -141;         

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxCannotCloseStore = -142;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxCannotOpenStore = -143;        

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxThreadError = -144;            

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxFileSessionError = -145;       

/**
Could not connect to the fax machine.  Was KFaxFileCreateError till 7/10/97.
@publishedPartner
@released
*/
const TInt KFaxLogCreateError = -146;         

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxFileSeekError = -147;          

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxPDDError = -148;               

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxLDDError = -149;               

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxC32Error = -150;               

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxCommsServerError = -151;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxCommsPortError = -152;         

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxCancelRequested = -153;        

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxNoClass2Xon = -154;            

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxConfigError = -155;            

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxCannotAutodetect = -156;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KCannotFindFirstPage = -157;       

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxBadInitialization = -158;      

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxOnlyJustAutoDetected = -159;   

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxBelowMinSpeed = -160;          

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxPollingUnsupported = -161;     

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxNothingToPoll = -162;          

/**
Could not connect to the fax machine. 
@publishedPartner
@released
*/
const TInt KFaxEtelServerError = -163;        
					
/**
standard communication bytes
*/
const TUint8 Knul = 0x00;
const TUint8 Kdle = 0x10;
const TUint8 Ketx = 0x03;
const TUint8 Kbackspace = 0x08;
const TUint8 Kreturn = 0x0d;
const TUint8 Klinefeed = 0x0a;
const TUint8 Kxon = 0x11;
const TUint8 Kxoff = 0x13;
const TUint8 Kdc2 = 0x12;
const TUint8 Kcan = 0x18;

/**
apparent clock granularity
*/
const TInt CLK_TCK = 1000000;

// maximum size of a descriptor for reading and writing T4 scan lines

// the longest possible encoded scan line is alternating black and white dots
// that's 8 bits, as single white dot = 00111 single black dot = 010
// there's a maximum of 864 such pairs in a 1728 dot scan line
// let's be generous and allow for eols and padding - 896
// this is sufficient for faxes we generate ourselves

// On the other hand, a scan line can take up to 5 seconds to transmit
// At 14400 bps, it could contain 72000 bits = 9000 bytes
// There could be internal line padding = alternate run lengths of zero)
// So this bigger figure is needed for faxes which we receive via the line

// we also use this when sending phase C data to the modem
// we also use this when receiving phase C data from the modem
const TInt KMaxT4Des = 896;
const TInt KMaxFaxBuf ((KMaxT4Des + sizeof (TInt)) * 2);

// base for sizing buffers
//-- LYN-593JT3 defect fix by Dmitry Lyokhin. 16.04.02
const TInt KBufSize = 2048; 

/**
Major version number.
A version must be specified when creating a session with the server.
*/
const TUint KEtelFaxMajorVersionNumber = 1;
/** Minor version number.
A version must be specified when creating a session with the server.
@publishedPartner
*/
const TUint KEtelFaxMinorVersionNumber = 0;
/** Build version number. 
A version must be specified when creating a session with the server.
*/
const TUint KEtelFaxVersionNumber = 601;

IMPORT_C TInt StartThread ();


/********************************************************************/

// fax structures
/********************************************************************/ 

//The next enumerations are bitmapped, giving useful TFaxMode masks.
const TUint KFaxReceive = 0x01;
const TUint KFaxOffHook = 0x02;
const TUint KFaxWaitForRing = 0x04;
const TUint KFaxNoDial = 0x06;
const TUint KFaxPoll = 0x08;

/**
Defines the seven fax session modes supported by the fax client.

The enumerations do not progress from 0 to 6, as might be expected, but rather 
as 0, 1, 2, 3, 4, 5, 9. 
*/
enum TFaxMode
	{
	/** Normal fax machine type send mode. */
	EDialAndTransmit=0,				// this is a normal fax machine type send
	/** Dial and receive fax back mode calling a fax on demand service. */
	EDialAndReceiveFaxBack=1,  		// this dials a fax on demand service
	/** Immediate transmit mode  for switching from voice to send. */
	EImmediateTransmit=2,	  		// for switching from voice to send
	/** Immediate receive mode for switching from voice to receive. */
	EImmediateReceive=3,				// for switching from voice to receive
	/** Ring and transmit mode for providing fax on demand. */
	EWaitForRingAndTransmit=4, 		// this make us into a fax on demand 
	/** Normal fax machine type receive mode. */
	EWaitForRingAndReceive=5,		// this is a normal fax machine type receive
	/** Fax polling mode */
	EDialAndReceivePoll=9			// this is for fax polling
	};

#endif
