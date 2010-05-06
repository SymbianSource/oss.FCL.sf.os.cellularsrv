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

#ifndef MMMMESSHANDLERBASE_H
#define MMMMESSHANDLERBASE_H

//  INCLUDES
#include <e32base.h>
#include <et_tsy.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION
 
/**
 *  If the message is mode-independent it  will be handled here. 
 *  Otherwise, the message will be sent to message handler of active mode.
 */
class MMmMessHandlerBase
    {
    public:
    
        /**
         * ExtFuncL
         *         
         *
         * @param aIpc Ipc number of the request
         * @param aDataPackage Datapackage
         * @return Error value
         */
        virtual TInt  ExtFuncL( TInt aIpc, VA_LIST& aList ) = 0;
    };
    

#endif // MMMMESSHANDLERBASE_H

//  End of file
