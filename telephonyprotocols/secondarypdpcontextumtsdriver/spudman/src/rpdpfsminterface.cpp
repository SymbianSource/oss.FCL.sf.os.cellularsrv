// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Proxy methods for RPdpFsmInterface class
// 
//

/**
 @file 
 @internalComponent
*/


#include "rpdpfsminterface.h"


RPdpFsmInterface::RPdpFsmInterface()
   : iFsmImpl(NULL)
{
}

RPdpFsmInterface::~RPdpFsmInterface()
{
}
	
void RPdpFsmInterface::OpenL(MSpudManInterface* aSpudManInterface, TName& aName)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrNotReady ));
   iFsmImpl->OpenL (aSpudManInterface, aName);
}

void RPdpFsmInterface::Close()
{
   if (iFsmImpl)
      {
      // Clear the member first to avoid ping-ponging Close() messages
      // between the PdpFsmInterface and SpudManInterface
      MPdpFsmInterface* fsmImpl = iFsmImpl;
      iFsmImpl = NULL;
      fsmImpl->Close ();
      }
}
	

TInt RPdpFsmInterface::Input (TContextId aPdpId, const TInt aOperation, const TInt aParam /* =KErrNone */)
{
   if (iFsmImpl)
      {
      return iFsmImpl->Input (aPdpId, aOperation, aParam);
      }
   
   // let whoever know we can't do what they ask
   return KErrNotReady;
}


#ifdef SYMBIAN_NETWORKING_UMTSR5

void RPdpFsmInterface::SetIMCNSignalling(TBool aImCnSignalling)
/**
Set the status of IM CN Signalling Flag in UmtsGprsScpr

@param aImCnSignalling IMCN signalling Status
*/
{
__ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KDescNullPdpFsm, KErrNotReady ));	
 iFsmImpl->SetIMCNSignalling(aImCnSignalling);	
}

TInt RPdpFsmInterface::Set(TContextId aPdpId, const RPacketQoS::TQoSR5Requested& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KDescNullPdpFsm, KErrNotReady ));
   return iFsmImpl->Set (aPdpId, aParam);
}

TInt RPdpFsmInterface::Set(TContextId aPdpId, const RPacketQoS::TQoSR5Negotiated& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KDescNullPdpFsm, KErrNotReady ));
   return iFsmImpl->Set (aPdpId, aParam);
}

#else
// !SYMBIAN_NETWORKING_UMTSR5 

TInt RPdpFsmInterface::Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Requested& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Set (aPdpId, aParam);
}

TInt RPdpFsmInterface::Set(TContextId aPdpId, const RPacketQoS::TQoSR99_R4Negotiated& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Set (aPdpId, aParam);
}

#endif 
// SYMBIAN_NETWORKING_UMTSR5 



TInt RPdpFsmInterface::Set(TContextId aPdpId, const TTFTInfo& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Set (aPdpId, aParam);
}

TInt RPdpFsmInterface::Set(TContextId aPdpId, const TTFTOperationCode& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Set (aPdpId, aParam);
}

TInt RPdpFsmInterface::Set(TContextId aPdpId, const RPacketContext::TDataChannelV2& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Set (aPdpId, aParam);
}

TInt RPdpFsmInterface::Set(TContextId aPdpId, const RPacketContext::TContextConfigGPRS& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Set (aPdpId, aParam);
}

TInt RPdpFsmInterface::Set(TContextId aPdpId, const RPacketContext::TContextStatus& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Set (aPdpId, aParam);
}


#ifdef SYMBIAN_NETWORKING_UMTSR5
TInt RPdpFsmInterface::Get(TContextId aPdpId, RPacketQoS::TQoSR5Requested& aParam) const
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KDescNullPdpFsm, KErrNotReady ));
   return iFsmImpl->Get (aPdpId, aParam);
}

TInt RPdpFsmInterface::Get(TContextId aPdpId, RPacketQoS::TQoSR5Negotiated& aParam) const
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KDescNullPdpFsm, KErrNotReady ));
   return iFsmImpl->Get (aPdpId, aParam);
}

#else
// !SYMBIAN_NETWORKING_UMTSR5 

TInt RPdpFsmInterface::Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Requested& aParam) const
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Get (aPdpId, aParam);
}

TInt RPdpFsmInterface::Get(TContextId aPdpId, RPacketQoS::TQoSR99_R4Negotiated& aParam) const
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Get (aPdpId, aParam);
}

#endif 
// SYMBIAN_NETWORKING_UMTSR5 


TInt RPdpFsmInterface::Get(TContextId aPdpId, TTFTInfo& aParam) const
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Get (aPdpId, aParam);
}

TInt RPdpFsmInterface::Get(TContextId aPdpId, TTFTOperationCode& aParam) const
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Get (aPdpId, aParam);
}

TInt RPdpFsmInterface::Get(TContextId aPdpId, RPacketContext::TDataChannelV2& aParam) const
{
   if (iFsmImpl == NULL) 
      {
      // Since the RPdpFsmInterface implementation hasn't been initialised yet we need to provide
      // a response. This is only going to happen as lower nifs are being created and before the
      // UmtsGprsSCPR has had a chance to initialise our iFsmImpl pointer, and only if they do a
      // ReadDes() for ModemBearer\CSYName, or ModemBearer\PortName. Luckily for us at this
      // early stage Etel always response with an empty TDataChannelV2 structure.
      aParam.iChannelId = KNullDesC;
      aParam.iCsy = KNullDesC;
      aParam.iPort = KNullDesC;
      return KErrNone;
      }
   else
      {
      return iFsmImpl->Get (aPdpId, aParam);
      }
}

TInt RPdpFsmInterface::Get(TContextId aPdpId, RPacketContext::TContextConfigGPRS& aParam) const
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->Get (aPdpId, aParam);
}

TInt RPdpFsmInterface::Get(TContextId aPdpId, RPacketContext::TContextStatus& aParam) const
{
   if (iFsmImpl) 
      {
      return iFsmImpl->Get (aPdpId, aParam);
      }
      
   return KErrNotReady;
}

void RPdpFsmInterface::Set(const RPacketService::TStatus aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   iFsmImpl->Set (aParam);
}

void RPdpFsmInterface::Get(RPacketService::TStatus& aParam)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   iFsmImpl->Get (aParam);
}

TInt RPdpFsmInterface::GetLastErrorCause(TContextId aPdpId, TInt& aLastErrorCause) const
{
   if (iFsmImpl) 
      {
      return iFsmImpl->GetLastErrorCause (aPdpId, aLastErrorCause);
      }
      
   return KErrNone;
}

const TName& RPdpFsmInterface::TsyName(void)
{
   __ASSERT_DEBUG (iFsmImpl != NULL, User::Panic( KNullDesC, KErrGeneral ));
   return iFsmImpl->TsyName ();
}
	
void RPdpFsmInterface::SetContextTerminationErrorAndStop(TContextId aPdpId, TInt aErrorCode)
{
   if (iFsmImpl) 
      {
      iFsmImpl->SetContextTerminationErrorAndStop (aPdpId, aErrorCode);
      }
}


void RPdpFsmInterface::Init (const MPdpFsmInterface* aSpudMan)
{
   // This should only be getting set the once, but so long as its the same instance we'll let it slip
   ASSERT (iFsmImpl == NULL || aSpudMan == iFsmImpl);
   iFsmImpl = const_cast<MPdpFsmInterface*>(aSpudMan);
}





