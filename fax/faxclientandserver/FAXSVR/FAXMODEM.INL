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

#ifndef __FAXMODEM_INL
#define __FAXMODEM_INL

/********************************************************************/

inline void CFaxModem::SendTransmitBufferL ()
   {
   if (iTransmitBuffer.Length () < KBufSize)
      return;
   CommitTransmitBufferL ();
   }
/********************************************************************/

#endif


