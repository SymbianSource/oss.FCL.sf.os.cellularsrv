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
// The bytes are all inverted as the T.30 spec indicates that all frame 
// data should be transmitted from MSB -> LSB ( i.e. left to right just 
// as printed).   However, asynchronous data is always send LSB -> MSB  
// which would be the wrong way round.   Note that in the faxparms.bit  
// structure the fields are declared in MSB -> LSB order, which is OK.  
// ID strings are an exception : they are always transmitted completely 
// backwards (LSB of last byte -> MSB of first byte.        
// Where the MSB is listed as an x this should be set to 1 if we    
// received KT30_DIS or left at to 0 if we received a response to our KT30_DIS
// We have a mask on incoming data to knock this bit off
// 
//

/**
 @file
 @internalComponent 
*/

#ifndef __FRAMES_H
#define __FRAMES_H
/**
@internalComponent
*/
const TUint8 KT30_DISMASK (0xfe);

const TUint8 KT30_TCF     (0x00);
const TUint8 KT30_FILL    (0x00);

const TUint8 KT30_T1        (35);  // this is the T1 35 second timeout
const TUint8 KT30_T2         (6);  // this is the T2 6 second timeout
const TUint8 KT30_T4         (3);  // this is the T4 3 second timeout (5.2E/T.30)

const TUint8 KT30_ADDR    (0xff);  // HDLC address is palindromic 
const TUint8 KT30_CTLNXT  (0x03);  // intermediate frame control field 11000000 
const TUint8 KT30_CTLLST  (0x13);  // final control field 11001000 

const TUint8 KT30_DIS     (0x80);  // answering capability follows 00000001 
const TUint8 KT30_CSI     (0x40);  // answering station identity follows 00000010 
const TUint8 KT30_NSF     (0x20);  // answering nonstandard facilities 00000100 

const TUint8 KT30_DTC     (0x81);  // polling capability follows 10000001 
const TUint8 KT30_CIG     (0x41);  // polling station identity follows 10000010 
const TUint8 KT30_NSC     (0x21);  // polling nonstandard facilities 10000100 

const TUint8 KT30_DCS     (0x82);  // transmitter capability follows x1000001 
const TUint8 KT30_TSI     (0x42);  // transmitter station identity follows x1000010 
const TUint8 KT30_NSS     (0x22);  // transmitter nonstandard facilities x1000100 

const TUint8 KT30_CFR     (0x84);  // confirmation to received x0100001 
const TUint8 KT30_FTT     (0x44);  // failure to train x0100010 

const TUint8 KT30_EOM     (0x8e);  // end of document x1110001 
const TUint8 KT30_MPS     (0x4e);  // end of page x1110010 
const TUint8 KT30_EOP     (0x2e);  // end of transmission x1110100 
const TUint8 KT30_PRIEOM  (0x9e);  // end of document - operator pls x1111001 
const TUint8 KT30_PRIMPS  (0x5e);  // end of page - operator pls x1111010 
const TUint8 KT30_PRIEOP  (0x3e);  // end of transmission - operator pls x1111100 

const TUint8 KT30_MCF     (0x8c);  // message confirmation x0110001 
const TUint8 KT30_RTP     (0xcc);  // message confirmation with retrain x0110011 
const TUint8 KT30_PIP     (0xac);  // message confirmation - operator pls x0110101 
const TUint8 KT30_RTN     (0x4c);  // message not received with retrain x0110010 
const TUint8 KT30_PIN     (0x2c);  // message not received - operator pls x0110100 

const TUint8 KT30_DCN     (0xfa);  // disconnect x1011111 
const TUint8 KT30_CRP     (0x1a);  // command repeat x1011000 
/**
@internalComponent
*/
union faxparms
   {
   TUint8 byte[5] ;
   struct
      {
      unsigned b01: 1 ; // transmitter T2 
      unsigned b02: 1 ; // receiver T2 
      unsigned b03: 1 ; // T2 IOC 
      unsigned b04: 1 ; // transmitter T3 
      unsigned b05: 1 ; // receiver T3 
      unsigned b06: 1 ; // reserved 
      unsigned b07: 1 ; // reserved 
      unsigned b08: 1 ; // reserved 

      unsigned b09: 1 ; // transmitter T4 
      unsigned b10: 1 ; // receiver T4 
      unsigned b11: 1 ; // data signalling rate b 1 
      unsigned b12: 1 ; // data signalling rate b 2 
      unsigned b13: 1 ; // data signalling rate b 3 
      unsigned b14: 1 ; // data signalling rate b 4 
      unsigned b15: 1 ; // fine vertical resolution 
      unsigned b16: 1 ; // two dimensional coding 

      unsigned b17: 1 ; // fax width b 1 
      unsigned b18: 1 ; // fax width b 2 
      unsigned b19: 1 ; // fax length b 1 
      unsigned b20: 1 ; // fax length b 2 
      unsigned b21: 1 ; // minimum scan line line b 1 
      unsigned b22: 1 ; // minimum scan line line b 2 
      unsigned b23: 1 ; // minimum scan line line b 3 
      unsigned b24: 1 ; // extend field 

      unsigned b25: 1 ; // 2400 bps handshake 
      unsigned b26: 1 ; // uncompressed mode 
      unsigned b27: 1 ; // error correction 
      unsigned b28: 1 ; // frame size 64 octets 
      unsigned b29: 1 ; // error limiting mode 
      unsigned b30: 1 ; // G4 
      unsigned b31: 1 ; // T6 coding 
      unsigned b32: 1 ; // extend field 

      unsigned b33: 1 ; // validity of width bs 17,18 
      unsigned b34: 1 ; // width 1216 pels/151mm 
      unsigned b35: 1 ; // width 864 pels/107mm 
      unsigned b36: 1 ; // width 1728 pels/151mm 
      unsigned b37: 1 ; // width 1728 pels/107mm 
      unsigned b38: 1 ; // reserved 
      unsigned b39: 1 ; // reserved 
      unsigned b40: 1 ; // extend 
      } bit ;
   } ;

#endif

