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
 @released
*/
inline void CPhoneFactoryBase::Inc()
	{
	CObject::Inc();
	}

inline TVersion CPhoneFactoryBase::TsyVersionNumber() const
	{
	return iVersion;
	}

inline void CSubSessionExtBase::Inc()
	{
	CObject::Inc();
	}

inline TBool CBuffer::OverFlow() const
	{
	return iOverFlow;
	}

inline TBool CBuffer::BufferFull() const
	{
	return iBufferFull;
	}


inline void CReqEntry::operator delete(TAny* aPtr, TLeave )
//
// overloaded delete - to prevent warning
//
    {
    operator delete(aPtr);
    }

inline void CReqEntry::operator delete(TAny* aPtr, RHeap *)
//
// overloaded delete - to prevent warning
//
    {
    operator delete(aPtr);
    }

inline void CReqEntry::operator delete(TAny* aPtr, TUint )
//
// overloaded delete - to prevent warning
//
    {
    operator delete(aPtr);
    }


inline void CBuffer::operator delete(TAny* aPtr, TLeave )
//
// overloaded delete - to prevent warning
//
    {
    operator delete(aPtr);
    }

inline void CBuffer::operator delete(TAny* aPtr, RHeap *)
//
// overloaded delete - to prevent warning
//
    {
    operator delete(aPtr);
    }

inline void CBuffer::operator delete(TAny* aPtr, TUint )
//
// overloaded delete - to prevent warning
//
    {
    operator delete(aPtr);
    }

inline TPhoneClientId::TPhoneClientId() : iSessionId(0), iSubSessionId(0)
	{
	}

inline TPhoneClientId::TPhoneClientId(TInt aSessionId, TInt aSubSessionId)
	: iSessionId(aSessionId), iSubSessionId(aSubSessionId)
	{
	}
		
inline TBool TPhoneClientId::operator == (const TPhoneClientId& aClientId) const
	{
	return (iSessionId == aClientId.iSessionId &&
			iSubSessionId == aClientId.iSubSessionId);
	}

inline TBool TPhoneClientId::operator != (const TPhoneClientId& aClientId) const
	{
	return !(*this == aClientId);
	}

inline TPhoneClientId& TPhoneClientId::operator = (const TPhoneClientId& aClientId)
	{
	if (this != &aClientId)
		{
		iSessionId = aClientId.iSessionId;
		iSubSessionId = aClientId.iSubSessionId;
		}
	return *this;
	}

