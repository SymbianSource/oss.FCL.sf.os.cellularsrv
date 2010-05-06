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
// GsmuMsgAdditionalAttributes.cpp
// 
//

#include "gsmumsgadditionalattributes.h"
#include "smsstacklog.h"


CSmsMessageAdditionalAttributes* CSmsMessageAdditionalAttributes::NewL()
    {
	LOGGSMU1("CSmsMessageAdditionalAttributes::NewL()");

	CSmsMessageAdditionalAttributes* additionalAttributes=new(ELeave) CSmsMessageAdditionalAttributes();
	CleanupStack::PushL(additionalAttributes);
	additionalAttributes->ConstructL();
	CleanupStack::Pop();
	return additionalAttributes;
    } // CSmsMessageAdditionalAttributes::NewL


CSmsMessageAdditionalAttributes::CSmsMessageAdditionalAttributes()
    {
    // NOP
    } // CSmsMessageAdditionalAttributes::CSmsMessageAdditionalAttributes


CSmsMessageAdditionalAttributes::~CSmsMessageAdditionalAttributes()
    {
	LOGGSMU1("CSmsMessageAdditionalAttributes::~CSmsMessageAdditionalAttributes()");

	iSmsPDUArray.ResetAndDestroy();

    for (TInt category = 0; category < TSmsInformationElementCategories::ENumberOfCategories; category++)
        {
        if (iCollectionOfPointersToIEArrays[category] != NULL)
            {
            iCollectionOfPointersToIEArrays[category]->ResetAndDestroy();
            }

        delete iCollectionOfPointersToIEArrays[category];
        }

    delete iHyperLinkOperations;
    delete iReplyAddressOperations;
    delete iSpecialSMSMessageOperations;
    delete iEnhancedVoiceMailOperations;
    delete iSMSCCtrlParameterOperations;
    delete iTPSRROperations;
    delete iStatusReportScheme;
    delete iIncompleteClass0MessageInfo;
    } // CSmsMessageAdditionalAttributes::CSmsMessageAdditionalAttributes


void CSmsMessageAdditionalAttributes::ConstructL()
    {
	LOGGSMU1("CSmsMessageAdditionalAttributes::ConstructL()");

    iStatusReportScheme = new (ELeave) CSmsMessageAdditionalAttributes::CDefaultScheme();
    
    for (TInt category = 0; category < TSmsInformationElementCategories::ENumberOfCategories; category++)
        {
        iCollectionOfPointersToIEArrays[category] = new (ELeave) CArrayOfPointersToControlInformationElements(8);
        }
    } // CSmsMessageAdditionalAttributes::ConstructL


void CSmsMessageAdditionalAttributes::AddControlInformationElementL(CSmsInformationElement* aIE)
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::AddControlInformationElementL()");

    TSmsInformationElementCategories::TInformationElementCategory category;
    if (aIE != NULL)
        {
        if (TSmsInformationElementCategories::GetCategoryDefinition(aIE->Identifier(), category))
        	{
        	AddControlInformationElementL(category,aIE);
        	}
        else
            {
            LOGGSMU1("CSmsMessageAdditionalAttributes::AddControlInformationElementL, aIE->Identifier() is invalid");
            User::Leave(KErrArgument);
            }
        }
    else
        {
        LOGGSMU1("CSmsMessageAdditionalAttributes::AddControlInformationElementL, aIE == NULL");
        User::Leave(KErrArgument);
        }
    } // CSmsMessageAdditionalAttributes::AddControlInformationElementL


TBool CSmsMessageAdditionalAttributes::Find1stInstanceOfControlInformationElement(TInformationElementId aId, TUint& aIndex) const
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::Find1stInstanceOfControlInformationElement()");
    
    TBool rc = EFalse;
    TSmsInformationElementCategories::TInformationElementCategory category;
    
    if (TSmsInformationElementCategories::GetCategoryDefinition(aId, category))
    	{
		TInt count=iCollectionOfPointersToIEArrays[category]->Count();
		for (TInt i=0; i<count; i++)
		    {
		    if (iCollectionOfPointersToIEArrays[category]->operator[](i)->Identifier()==aId)
		        {
		        aIndex=i;
		        rc=ETrue;
		        break;
		        }
		    }
        }
    return rc;
    } // CSmsMessageAdditionalAttributes::Find1stInstanceOfControlInformationElement


TBool CSmsMessageAdditionalAttributes::FindNextInstanceOfControlInformationElement(TInformationElementId aId, TUint aStartIndex, TUint& aIndex) const
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::FindNextInstanceOfControlInformationElement()");
    
    TBool rc = EFalse;
    TSmsInformationElementCategories::TInformationElementCategory category;

	if (TSmsInformationElementCategories::GetCategoryDefinition(aId, category))
		{
	    TInt count=iCollectionOfPointersToIEArrays[category]->Count();
		for (TInt i =aStartIndex + 1; i<count; i++)
		    {
		    if (iCollectionOfPointersToIEArrays[category]->operator[](i)->Identifier()==aId)
		        {
				aIndex=i;
				rc=ETrue;
	            break;
				}
		    }
		}

    return rc;
    } // CSmsMessageAdditionalAttributes::FindNextInstanceOfControlInformationElement


CSmsInformationElement& CSmsMessageAdditionalAttributes::GetControlInformationElementL(TInformationElementId aId, TUint aIndex) const
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::GetControlInformationElementL()");
    

    TSmsInformationElementCategories::TInformationElementCategory category;

    if (TSmsInformationElementCategories::GetCategoryDefinition(aId, category) == EFalse)
    	{
    	User::Leave(KErrArgument);
    	}

    if (aIndex >= iCollectionOfPointersToIEArrays[category]->Count())
        {
        LOGGSMU3("CSmsMessageAdditionalAttributes::GetControlInformationElementL  aId = %d , aIndex = %d", aId, aIndex);
        LOGGSMU2("CSmsMessageAdditionalAttributes::GetControlInformationElementL  Count = %d", iCollectionOfPointersToIEArrays[category]->Count());
        User::Leave(KErrArgument);
        }

    if (iCollectionOfPointersToIEArrays[category]->operator[](aIndex) == NULL)
        {
        LOGGSMU3("CSmsMessageAdditionalAttributes::GetControlInformationElementL  aId = %d , aIndex = %d", aId, aIndex);
        User::Leave(KErrArgument);
        }

    if (iCollectionOfPointersToIEArrays[category]->operator[](aIndex)->Identifier()!=aId)
        {
        LOGGSMU3("CSmsMessageAdditionalAttributes::GetControlInformationElementL aId = %d, aIndex = %d", aId, aIndex);
        LOGGSMU2("CSmsMessageAdditionalAttributes::GetControlInformationElementL  id = %d", iCollectionOfPointersToIEArrays[category]->operator[](aIndex)->Identifier());
        User::Leave(KErrArgument);
        }

    CSmsInformationElement* ie = iCollectionOfPointersToIEArrays[category]->operator[](aIndex);
    return *ie;
    } // CSmsMessageAdditionalAttributes::GetControlInformationElementL


TBool CSmsMessageAdditionalAttributes::RemoveControlInformationElement(TInformationElementId aId, TUint aIndex, CSmsInformationElementPtr& aIE)
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::RemoveControlInformationElement()");
    
    TBool rc = EFalse;
    TSmsInformationElementCategories::TInformationElementCategory category;

	if (TSmsInformationElementCategories::GetCategoryDefinition(aId, category))
		{
		if ((iCollectionOfPointersToIEArrays[category]->Count() > aIndex) &&
		    (iCollectionOfPointersToIEArrays[category]->operator[](aIndex)->Identifier()==aId))
		        {
		        aIE = iCollectionOfPointersToIEArrays[category]->operator[](aIndex);
		        iCollectionOfPointersToIEArrays[category]->Delete(aIndex);
		        rc = ETrue;
		        }
		}
		
    return rc;
    } // CSmsMessageAdditionalAttributes::RemoveControlInformationElement


void CSmsMessageAdditionalAttributes::AddControlInformationElementL(TCategory aCategory, CSmsInformationElementPtr aIE)
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::AddControlInformationElementL()");
    
    if ((aIE != NULL) &&
        (aCategory < TSmsInformationElementCategories::ENumberOfCategories))
        {
        iCollectionOfPointersToIEArrays[aCategory]->AppendL(aIE);
        }
    else
        {
        User::Leave(KErrArgument);
        }
    } // CSmsMessageAdditionalAttributes::AddControlInformationElementL


TBool CSmsMessageAdditionalAttributes::RemoveNextControlInformationElement(TCategory aCategory, CSmsInformationElementPtr& aIEPtr)
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::RemoveNextControlInformationElement()");

    TBool rc = EFalse;
    if (aCategory < TSmsInformationElementCategories::ENumberOfCategories)
        {
        TInt tailIndex;
        if ((tailIndex = (iCollectionOfPointersToIEArrays[aCategory]->Count() - 1)) >= 0)
            {
            aIEPtr = (*iCollectionOfPointersToIEArrays[aCategory])[tailIndex];
            iCollectionOfPointersToIEArrays[aCategory]->Delete(tailIndex);
            rc = ETrue;
            }
        }
    return rc;
    } // CSmsMessageAdditionalAttributes::RemoveNextControlInformationElement


CSmsInformationElement& CSmsMessageAdditionalAttributes::GetControlInformationElementL(TCategory aCategory, TUint aIndex) const
    {
    LOGGSMU3("CSmsMessageAdditionalAttributes::GetControlInformationElementL(): aCategory=%d, aIndex=%d",
    		 aCategory, aIndex);

    if ((aCategory >= TSmsInformationElementCategories::ENumberOfCategories) ||
        (aIndex    >= iCollectionOfPointersToIEArrays[aCategory]->Count()))
        {
        LOGGSMU1("CSmsMessageAdditionalAttributes::GetControlInformationElementL: KErrArgument");
        User::Leave(KErrArgument);
        }

    if  ((iCollectionOfPointersToIEArrays[aCategory])->operator[](aIndex) == NULL)
        {
        LOGGSMU1("CSmsMessageAdditionalAttributes::GetControlInformationElementL: KErrCorrupt");
        User::Leave(KErrCorrupt);
        }

    return *(iCollectionOfPointersToIEArrays[aCategory]->operator[](aIndex));
    } // CSmsMessageAdditionalAttributes::GetControlInformationElementL


void CSmsMessageAdditionalAttributes::DeleteControlInformationElement(TCategory aCategory, TUint8 aIndex)
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::DeleteControlInformationElement()");
    
    if (aCategory < TSmsInformationElementCategories::ENumberOfCategories)
        {
        TInt count = iCollectionOfPointersToIEArrays[aCategory]->Count();
        if (aIndex < count)
            {
            CSmsInformationElement* iE = iCollectionOfPointersToIEArrays[aCategory]->operator[](aIndex);
            iCollectionOfPointersToIEArrays[aCategory]->Delete(aIndex);
			// The element being deleted has been removed from iCollectionOfPointersToIEArrays. So, next time it cannot be deleted again.
			// coverity[double_free]
            delete iE;
            }
        }
    } // CSmsMessageAdditionalAttributes::DeleteControlInformationElement


TUint CSmsMessageAdditionalAttributes::NumberOfControlInformationElements(TCategory aCategory) const
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::NumberOfControlInformationElements1()");

    TUint count = 0;

    if (aCategory < TSmsInformationElementCategories::ENumberOfCategories)
    {
      count = iCollectionOfPointersToIEArrays[aCategory]->Count();
    }

    return count;
    } // CSmsMessageAdditionalAttributes::NumberOfControlInformationElements


void CSmsMessageAdditionalAttributes::ExternalizeL(RWriteStream& aStream, TInt aVersion) const
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::ExternalizeL()");

	if(aVersion == CSmsMessage::ESmsIncompleteClass0MessageV)
		{
		iIncompleteClass0MessageInfo->ExternalizeL(aStream);
		return;
		}

    for (TInt category = 0; category < TSmsInformationElementCategories::ENumberOfCategories; category++)
        {
        TUint32 count = iCollectionOfPointersToIEArrays[category]->Count();
		
		aStream.WriteUint32L(count);
        for (TUint32 j = 0; j < count; j++)
            {
            iCollectionOfPointersToIEArrays[category]->operator[](j)->ExternalizeL(aStream);
            }
        }
        
    if (aVersion == CSmsMessage::ESmsMessageV2  ||
    	aVersion == CSmsMessage::ESmsMessageV3  ||
    	aVersion == CSmsMessage::ESmsMessageV4)	// Status Report Scheme
		{
		aStream.WriteInt16L(iStatusReportScheme->Id());			//The Scheme
		
		if(iStatusReportScheme->Id() == ETPSRRScheme)
			{
			CTPSRRScheme* scheme = (CTPSRRScheme*)iStatusReportScheme;
			TUint32 count = (scheme->iTPSRRStatusReport).Count();	// TPSRR Array Count
			aStream << count;
			for(TInt ii=0; ii<count; ii++)
				{
				aStream.WriteUint16L((scheme->iTPSRRStatusReport[ii]).iSegmentSequenceNum);	// Segment number
				aStream.WriteInt16L((scheme->iTPSRRStatusReport[ii]).iTPSRRStatus);			// TPSRR Bit
				}
			aStream.WriteInt16L(scheme->iDefaultStatusReport);		// Default
			aStream.WriteInt16L(scheme->iNumOfPDUs);
			}
			
		if(iStatusReportScheme->Id() == EControlParametersScheme)
			{
			CControlParametersScheme* scheme = (CControlParametersScheme*)iStatusReportScheme;
			TUint32 count = (scheme->iControlParametersStatusReport).Count();	// Control Params Array Count
			aStream << count;
			for(TInt ii=0; ii<count; ii++)
				{
				aStream.WriteUint16L((scheme->iControlParametersStatusReport[ii]).iSegmentSequenceNum);	// Segment number
				aStream.WriteInt8L((scheme->iControlParametersStatusReport[ii]).iSelectiveStatus);			// Selective Status
				}
			aStream.WriteInt8L(scheme->iDefaultStatusReport);		// Default
			aStream.WriteInt16L(scheme->iNumOfPDUs);
			}	
		}
    
    // CSmsMessage::ESmsMessageV3 is the same as CSmsMessage::ESmsIncompleteClass0MessageV!

    if (aVersion == CSmsMessage::ESmsMessageV4)	// National language 7bit encoding support
		{
		aStream.WriteInt32L(iAlternative7bitEncoding);
		}
	}

void CSmsMessageAdditionalAttributes::InternalizeL(RReadStream& aStream, TInt aVersion)
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::InternalizeL()");

	if(aVersion == CSmsMessage::ESmsIncompleteClass0MessageV)
		{
		iIncompleteClass0MessageInfo->InternalizeL(aStream);
		return;
		}

    for (TInt category = 0; category < TSmsInformationElementCategories::ENumberOfCategories; category++)
        {
        TUint32 count = aStream.ReadInt32L();
        iCollectionOfPointersToIEArrays[category]->ResetAndDestroy();

        for (TInt32 j = 0; j < count; j++)
            {
            CSmsInformationElement* informationElement = CSmsInformationElement::NewL();
            CleanupStack::PushL(informationElement);
            aStream >> *informationElement;
            iCollectionOfPointersToIEArrays[category]->AppendL(informationElement);
            CleanupStack::Pop();
            }	
    	}
    	
    if (aVersion == CSmsMessage::ESmsMessageV2  ||
    	aVersion == CSmsMessage::ESmsMessageV3  ||
    	aVersion == CSmsMessage::ESmsMessageV4)	// Status Report Scheme
		{
		TSmsStatusReportScheme schemeId = static_cast<TSmsStatusReportScheme>(aStream.ReadInt16L());
		
		if(schemeId == ETPSRRScheme)
			{
			SetStatusReportSchemeL(ETPSRRScheme);
			CTPSRRScheme* scheme = (CTPSRRScheme*)iStatusReportScheme;
			
			TUint32 count = aStream.ReadInt32L();
			
			CSmsMessageAdditionalAttributes::CTPSRRScheme::TSmsTPSRRStatus tpsrrStatus;
			for(TInt ii=0; ii<count; ii++)
				{
				tpsrrStatus.iSegmentSequenceNum = aStream.ReadUint16L();	// Segment number
				tpsrrStatus.iTPSRRStatus = static_cast<TSmsFirstOctet::TSmsStatusReportRequest>(aStream.ReadInt16L());	// TPSRR Bit
				(scheme->iTPSRRStatusReport).AppendL(tpsrrStatus);
				}
			scheme->iDefaultStatusReport = static_cast<TSmsFirstOctet::TSmsStatusReportRequest>(aStream.ReadInt16L());
			scheme->iNumOfPDUs = aStream.ReadInt16L();
			}
		if(schemeId == EControlParametersScheme)
			{
			SetStatusReportSchemeL(EControlParametersScheme);
			CControlParametersScheme* scheme = (CControlParametersScheme*)iStatusReportScheme;
			
			TUint32 count = aStream.ReadInt32L();
			
			CSmsMessageAdditionalAttributes::CControlParametersScheme::TSmsSMSCCtrlParameterStatus smscCtrlParameterStatus;
			for(TInt ii=0; ii<count; ii++)
				{
				smscCtrlParameterStatus.iSegmentSequenceNum = aStream.ReadUint16L();	// Segment number
				smscCtrlParameterStatus.iSelectiveStatus = aStream.ReadInt8L();			// Selective Status	
				(scheme->iControlParametersStatusReport).AppendL(smscCtrlParameterStatus);
				}
			scheme->iDefaultStatusReport = aStream.ReadInt8L();
			scheme->iNumOfPDUs = aStream.ReadInt16L();
			}
		}
	else
		{
		SetStatusReportSchemeL(EDefaultScheme);
		}
    
    // CSmsMessage::ESmsMessageV3 is the same as CSmsMessage::ESmsIncompleteClass0MessageV!

    if (aVersion == CSmsMessage::ESmsMessageV4)	// National language 7bit encoding support
		{
		iAlternative7bitEncoding = (TSmsEncoding) aStream.ReadInt32L();
		}
    } // CSmsMessageAdditionalAttributes::InternalizeL


void CSmsMessageAdditionalAttributes::ResetAttributesL()
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::ResetAttributesL()");
    
    for (TInt category = 0; category < TSmsInformationElementCategories::ENumberOfCategories; category++)
        {
        iCollectionOfPointersToIEArrays[category]->ResetAndDestroy();
        }
        
    if(iStatusReportScheme != NULL)
    	{
    	delete iStatusReportScheme;
    	iStatusReportScheme = NULL;
    	}
    	
    iStatusReportScheme = new (ELeave) CSmsMessageAdditionalAttributes::CDefaultScheme();
    
    iAlternative7bitEncoding = ESmsEncodingNone;
    } // CSmsMessageAdditionalAttributes::ResetAttributesL


CSmsIEOperation& CSmsMessageAdditionalAttributes::GetIEOperationL(TInformationElementId aId) const
    {
    LOGGSMU1("CSmsMessageAdditionalAttributes::GetIEOperation()");

    CSmsIEOperation* operation = NULL;

    switch(aId)
        {
        case CSmsInformationElement::ESmsHyperLinkFormat:
            operation = iHyperLinkOperations;
            break;
        case CSmsInformationElement::ESmsReplyAddressFormat:
            operation = iReplyAddressOperations;
            break;
        case CSmsInformationElement::ESmsEnhanceVoiceMailInformation:
            operation = iEnhancedVoiceMailOperations;
            break;
        case CSmsInformationElement::ESmsIEISpecialSMSMessageIndication:
            operation = iSpecialSMSMessageOperations;
            break;
        case CSmsInformationElement::ESmsIEISMSCControlParameters:
            operation = iSMSCCtrlParameterOperations;
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }

    return *operation;
    } // CSmsMessageAdditionalAttributes::GetIEOperationL


/**
 *  Gets the operation being used but for a non Information Element Type. 
 *  
 *  @note In the present case it is the TPSRR operation as that's the only one which exists as for now.
 *  
 *  @param aId The Identifier for the operation.
 */
CSmsNonIEOperation& CSmsMessageAdditionalAttributes::GetNonIEOperationL(TSmsNonIEIdentifier aId) const
	{
	LOGGSMU1("CSmsMessageAdditionalAttributes::GetNonIEOperationL");
    
    CSmsNonIEOperation* operation = NULL;
    
    if(aId == ESmsTPSRRParameter)
    	{
    	operation = iTPSRROperations;	
    	}
	else if (aId == ESmsIncompleteClass0MessageParameter)
		{
		operation = iIncompleteClass0MessageInfo;
		}
    else
    	{
    	User::Leave(KErrArgument);	
    	}
    
    return *operation;
    } // CSmsMessageAdditionalAttributes::GetIEOperationL


void CSmsMessageAdditionalAttributes::SetIEOperationL(CSmsIEOperation* aOperation)
	{
	LOGGSMU1("CSmsMessageAdditionalAttributes::SetIEOperationL()");

    if (aOperation == NULL)
        {
        User::Leave(KErrArgument);
        }

    LOGGSMU2("CSmsMessageAdditionalAttributes::AddIEOperation, operation=%d", aOperation->Id());

    switch(aOperation->Id())
        {
        case CSmsInformationElement::ESmsHyperLinkFormat:
            iHyperLinkOperations = (CSmsHyperLinkOperations*) aOperation;
            break;
        case CSmsInformationElement::ESmsReplyAddressFormat:
            iReplyAddressOperations = (CSmsReplyAddressOperations*)aOperation;
            break;
        case CSmsInformationElement::ESmsEnhanceVoiceMailInformation:
            iEnhancedVoiceMailOperations = (CSmsEnhancedVoiceMailOperations*) aOperation;
            break;
        case CSmsInformationElement::ESmsIEISpecialSMSMessageIndication:
            iSpecialSMSMessageOperations = (CSmsSpecialSMSMessageOperations*) aOperation;
            break;
        case CSmsInformationElement::ESmsIEISMSCControlParameters:
            iSMSCCtrlParameterOperations = (CSmsSMSCCtrlParameterOperations*) aOperation; 
            break;
        default:
            (void) aOperation;
            User::Leave(KErrArgument);
            break;
        }
	} // CSmsMessageAdditionalAttributes::SetIEOperationL


/**
 *  Sets the operation but for a non Information Element Type. 
 *  
 *  @note In the present case it is the TPSRR operation as that's the only one which exists as for now.
 */
void CSmsMessageAdditionalAttributes::SetNonIEOperationL(CSmsNonIEOperation* aOperation) 
	{
    if (aOperation == NULL)
        {
        User::Leave(KErrArgument);        
        }
    
    LOGGSMU2("CSmsMessageAdditionalAttributes::SetNonIEOperationL, operation = %d", aOperation->Id());
    
    if (aOperation->Id() == ESmsTPSRRParameter)
        {
        iTPSRROperations = (CSmsTPSRROperations*) aOperation;
        }
	else if (aOperation->Id() == ESmsIncompleteClass0MessageParameter)
		{
		iIncompleteClass0MessageInfo = (CIncompleteClass0MessageInfo*) aOperation;
		}
    else
    	{
    	User::Leave(KErrArgument);	
    	}
    } // CSmsMessageAdditionalAttributes::SetNonIEOperationL

    
/**
 *  Gets the pointer to the scheme being used for Status reporting. This is called in operations class and
 *  in Segmentation Store.
 *  
 *  @return Pointer to the scheme being used.
 */
CSmsMessageAdditionalAttributes::CSmsStatusReportScheme& CSmsMessageAdditionalAttributes::GetStatusReportScheme() const
	{
	return *iStatusReportScheme;
	} // CSmsMessageAdditionalAttributes::GetStatusReportScheme


void CSmsMessageAdditionalAttributes::SetStatusReportSchemeL(TSmsStatusReportScheme aId)
	{
	if(iStatusReportScheme != NULL)
		{
		delete iStatusReportScheme;
		iStatusReportScheme = NULL;	
		}
	
	if (aId == EControlParametersScheme)
		{
		iStatusReportScheme = new (ELeave) CSmsMessageAdditionalAttributes::CControlParametersScheme();	
		}
	if (aId == ETPSRRScheme)
		{
		iStatusReportScheme = new (ELeave) CSmsMessageAdditionalAttributes::CTPSRRScheme();	
		}
	if (aId == EDefaultScheme)
		{
		iStatusReportScheme = new (ELeave) CSmsMessageAdditionalAttributes::CDefaultScheme();	
		}
	} // CSmsMessageAdditionalAttributes::SetStatusReportSchemeL


/**
 *  Get the current encoding value.
 * 
 *  @return Encoding value.
 */
TSmsEncoding CSmsMessageAdditionalAttributes::Alternative7bitEncoding() const
	{
	return iAlternative7bitEncoding;
	} // CSmsMessageAdditionalAttributes::Alternative7bitEncoding


/**
 *  Set the encoding value.
 * 
 *  @param aEncoding  Encoding value.
 */
void CSmsMessageAdditionalAttributes::SetAlternative7bitEncoding(TSmsEncoding aEncoding)
	{
	iAlternative7bitEncoding = aEncoding;
	} // CSmsMessageAdditionalAttributes::SetAlternative7bitEncoding


CSmsMessageAdditionalAttributes::CControlParametersScheme::CControlParametersScheme()
: iControlParametersStatusReport(), iNumOfPDUs(0)
	{
	} // CSmsMessageAdditionalAttributes::CControlParametersScheme::CControlParametersScheme


/**
 *  Returns the ID of the current scheme.
 */
TSmsStatusReportScheme CSmsMessageAdditionalAttributes::CControlParametersScheme::Id()
	{
	return 	EControlParametersScheme;
	} // CSmsMessageAdditionalAttributes::CControlParametersScheme::Id

	
CSmsMessageAdditionalAttributes::CControlParametersScheme::~CControlParametersScheme()
	{
	iControlParametersStatusReport.Reset();
	iControlParametersStatusReport.Close();
	} // CSmsMessageAdditionalAttributes::CControlParametersScheme::~CControlParametersScheme

	
CSmsMessageAdditionalAttributes::CTPSRRScheme::CTPSRRScheme()
: iTPSRRStatusReport(), iNumOfPDUs(0)
	{
		
	} // CSmsMessageAdditionalAttributes::CTPSRRScheme::CTPSRRScheme


/**
 *  Returns the ID of the current scheme.
 */
TSmsStatusReportScheme CSmsMessageAdditionalAttributes::CTPSRRScheme::Id()
	{
	return ETPSRRScheme;
	} // CSmsMessageAdditionalAttributes::CTPSRRScheme::Id

	
CSmsMessageAdditionalAttributes::CTPSRRScheme::~CTPSRRScheme()
	{
	iTPSRRStatusReport.Reset();
	iTPSRRStatusReport.Close();
	} // CSmsMessageAdditionalAttributes::CTPSRRScheme::~CTPSRRScheme

	
CSmsMessageAdditionalAttributes::CDefaultScheme::CDefaultScheme()
	{
		
	} // CSmsMessageAdditionalAttributes::CDefaultScheme::CDefaultScheme


/**
 *  Returns the ID of the current scheme.
 */
TSmsStatusReportScheme CSmsMessageAdditionalAttributes::CDefaultScheme::Id()
	{
	return EDefaultScheme;
	} // CSmsMessageAdditionalAttributes::CDefaultScheme::Id

	
CSmsMessageAdditionalAttributes::CDefaultScheme::~CDefaultScheme()
	{

	} // CSmsMessageAdditionalAttributes::CDefaultScheme::~CDefaultScheme
