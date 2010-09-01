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
*/

#ifndef __cfax32_INL
#define __cfax32_INL

/********************************************************************/

/** Sets the mode of operation during the fax session. 
There are three possible transmission modes: 
A standard fax machine style dial and transmit.
An immediate transmit mode which is used if the phone is already off-hook on a voice call. 
A polling mode, in which the fax is set up as an answering machine. It sends the fax on request when
dialled by a remote modem.
There are four possible reception modes:
A standard fax machine style wait for a ring and receive.
An immediate receive which is used if the phone is already off-hook on a voice call.
A dial and receive mode in which the modem dials up in voice mode and then switches to immediate receive. 
It is intended for use with faxback services which require users to dial manually and then press the start 
button on their fax machine. The number of seconds between the voice mode dial and the switch to fax receive 
mode is determined by TFaxSettings::iFaxOnDemandDelay.
An automatic polling mode, in which the fax dials a remote answering machine, and requests it to send a fax.

Notes:
Some GSM networks do not allow switching between voice and fax or data modes, and hence cannot support the 
immediate transmit and immediate receive modes. This includes all networks in the UK. 

@param aMode  Type of fax operation for the current session. 

@publishedPartner
@released
*/
inline void CFaxTransfer::SetMode (TFaxMode aMode)
	{
	iMode = aMode;
	}

/** Sets the telephone number to be dialled when sending a fax. 
This function does not check the validity of the number, which is simply sent to the modem 
as part of an ATD dial command in its raw state.
The alternative function SetPhoneNumberL() should be used to resolve the phone number according to
the current location and country. One of these functions must be called before CFaxTransfer::Start()
when a fax is to be sent. 

@param aNumber  Descriptor containing the phone number to be dialled. 
@publishedPartner
@released
*/
inline void CFaxTransfer::SetPhoneNumber (TDesC8 & aNumber)
	{
	iPhoneNumber.Copy (aNumber);
	}

/**
Sets the RCall object name to use when transfering a fax.  This method allows
a client to use other ETel functionality to discover the incoming call name and
then open it for its own use.  If this function is not used, the fax server will
open a new RCall object to send or receive the Fax.

@see RLine::NotifyIncomingCall()

@param aName  Descriptor containing the RCall object name.
@publishedPartner
@released
*/
inline void CFaxTransfer::SetCallObjectName(TDesC& aName)
	{
	iCallName.Copy(aName);
	}

/**
Specifies the name of the fax store file in which to save incoming pages.
If the file already exists the previous contents are lost.

@param aName  The name of a file in which to save received faxes.
@publishedPartner
@released 
*/
inline void CFaxTransfer::SetReceiveFileName (const TFileName & aName)
	{
	iReceiveFileName.Copy (aName);
	}

/********************************************************************/

#endif
