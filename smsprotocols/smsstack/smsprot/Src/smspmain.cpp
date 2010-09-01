// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implements various functions
// 
//

/**
 @file
*/

#include <e32std.h>
#include "Gsmuelem.h"
#include <exterror.h>
#include "smspmain.h"
#include "smsstacklog.h"


/**
 *  convert an internal EPOC error to an RP specific error
 *  
 *  @param aError EPOC specific error
 *  @return TInt RP specific error
 */
GLREF_C TInt ErrorToRPError(TInt aError)
	{
	TInt rpError;
	//  ETEL extended errors mapping function according to GSM 04.11 also see exterror.h.
	//  Internal EPOC errors - currently map only memory related errors are mapped to
	//  KErrGsmSMSMemoryCapacityExceeded. All other errors are mapped to
	//  KErrGsmSMSUnspecifiedProtocolError.
	//  In addition all correct, MT related RL errors, are retained.
	switch (aError)
		{
		case KErrDiskFull:
		case KErrNoMemory:
			rpError = KErrGsmSMSMemoryCapacityExceeded;
			break;

		case KErrGsmSMSMemoryCapacityExceeded:
		case KErrGsmSMSInvalidShortMessageTransferReferenceValue:
		case KErrGsmSMSUnspecifiedInvalidMessage:
		case KErrGsmSMSInvalidMandatoryInformation:
		case KErrGsmSMSNonExistentMessageType:
		case KErrGsmSMSIncompatibleMessageWithSmsProtocolState:
		case KErrGsmSMSInformationElementNotImplemented:
		case KErrGsmSMSUnspecifiedProtocolError:
		    rpError = aError;
		    break;

		default:
			rpError = KErrGsmSMSUnspecifiedProtocolError;
			break;
		}
	LOGSMSPROT3("ErrorToRPError (%d->%d)", aError, rpError);

	return rpError;
	} // ErrorToRPError


/**
 *  convert an internal EPOC error to an TP specific error
 *  
 *  @param aError EPOC specific error
 *  @return TInt RP specific error
 */
GLREF_C TInt ErrorToTPError(TInt aError)
    {
    TInt tpError;
    //  ETEL errors
    if( (aError<=KErrGsmSmsBase) && (aError>=KErrGsmSMSUnspecifiedErrorCause) )
        {
        tpError = TSmsFailureCause::ESmsErrorErrorInMS;
        }
    else
        {
        switch (aError)
            {
            case KErrDiskFull:
            case KErrNoMemory:
                {
                tpError = TSmsFailureCause::ESmsErrorMemoryCapacityExceded;
                } break;
            case KErrNotFound:
            default:
                {
                tpError = TSmsFailureCause::ESmsErrorErrorInMS;
                } break;
            }
        }
    LOGSMSPROT3("ErrorToTPError (%d->%d)", aError, tpError);
    return tpError;
	}

GLREF_C TInt FailureCauseToError(TInt aFailureCause)
    {
    TInt error=KErrGsmSmsBase-aFailureCause;
    return error;
    }

/**
 *  panic in the SmsProt
 */
GLDEF_C void SmspPanic(TSmspPanic aPanic)
    {
    // Ignore in code coverage  - panic method
    BULLSEYE_OFF
    LOGSMSPROT2("SMSPPANIC %d", aPanic);
    _LIT(KSmsprotPanic, "SMSP");
    User::Panic(KSmsprotPanic,aPanic);
    BULLSEYE_RESTORE
    }
