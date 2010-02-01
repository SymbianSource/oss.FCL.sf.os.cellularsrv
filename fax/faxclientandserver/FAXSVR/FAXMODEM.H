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
// Contents :  fax s/r header file
// 
//

/**
 @file
 @internalComponent 
*/

#ifndef __FAXMODEM_H
#define __FAXMODEM_H

#include "FAXLOG.H"
// high level modem control functions

class CFaxModem : public CBase
/**
@internalComponent
*/
	{
public:
	static CFaxModem * NewL (TFaxServerSessionSettings *,RFax::TProgress& aProgress);
	static CFaxModem * NewLC (TFaxServerSessionSettings *,RFax::TProgress& aProgress);
	CFaxModem (RFax::TProgress& aProgress);	// added by MattS
	~CFaxModem ();

private:
	void ConstructL (TFaxServerSessionSettings *);
	TInt SubImportL (TDes8 &, TInt);	// chop up long timeouts
	void Dropdtr (void);				// drop DTR for a second
	void LowerDTR (void);				// do this to hang up modem
	void RaiseDTR (void);				// normal port state
	void Speed (TBps);					// set port speed
	void SendL (const TDesC8 &);		// Send a descriptor
	void HangupProcedureL();			// Destructor Hangup procedure
	void SetModemCommandModeL(void);	// Switch modem into command mode and clear its buffer.

public:
	TInt GetMatchL (const TDesC8 &, TInt);
	TInt ImportL (TDes8 &, TInt);
	TInt ExportL (const TDesC8 &);
	TInt clock (void);					// get ticks since start
	void Silence (void);
	void Silence (TInt) const;
	void Xonon (void);					// set Kxon/Kxoff
	void Xonoff (void);					// no handshaking, normal port state
	TInt Rxstat (void);					// Any character waiting to be read ?
	TInt Txstat (void);					// Any characters waiting to be sent ?
	void TxcharL (TUint8);				// this sends a byte
	inline void SendTransmitBufferL (void);
	void CommitTransmitBufferL (void);
	TInt RxcharWaitL (TInt &);
	void ProgressUpdateL ();

	TInt iVerbose;
	TInt iCancel;
	RFax::TProgress& iProgress;			// was TFaxTransferProgress. MattS.
	TBuf8 < 256 > iOurMessage;			// for feedback

	TInt iGranularity;					// for looking up timer granularity
	TInt iCalls;						// call calibration for high granularity
	TBuf8 < 1 > iReadone;				// for single character reads
	TBuf8 < KBufSize+16 > iTransmitBuffer;	// send data i/o buffer
	TBuf8 < KBufSize > iReceiveBuffer;		// read data i/o buffer

protected:									// all these will default to EFalse
	TBool iFileSessionOpen;
	TBool iFileOpen;
	TBool iPortOpen;
	TBool iModemSet;
	TBool iTimingLoopDelay;

	TCommConfig iRS232Settings;

	TRequestStatus iRecstat;				// for reads
	TRequestStatus iTranstat;				// for sends
	TBuf8 < 1 > iSendone;					// for single character sends
	TTime iStartTime;						// time we started at

	RComm iCommSession;
	RFs iFileSession;
	RFile iFile;
	};

/********************************************************************/

#include "FAXMODEM.INL"

#endif

