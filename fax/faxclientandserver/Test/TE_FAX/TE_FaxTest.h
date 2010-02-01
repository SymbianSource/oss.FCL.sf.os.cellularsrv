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
// Telephony Fax Test header file.
// 
//

/**
 @file
 @internalComponent
*/

#ifndef __TE_FAX_H__
#define __TE_FAX_H__


class CFaxTestFaxDefinition : public CFaxTestStep
	{
public:
	CFaxTestFaxDefinition();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestFaxStorage : public CFaxTestStep
	{
public:
	CFaxTestFaxStorage();
	virtual enum TVerdict doTestStepL();
 
	void Test1L(TInt aNumberLines, TBuf8<216>& aScanLineWrite, TFaxResolution aRes);
	void Test2L(TInt aNumberLines, TBuf8<216>& aScanLineWrite1,
				TBuf8<216>&  aScanLineWrite2, TFaxResolution aRes);
	void Test3L(TInt aNumberLines, TInt aNumberPages);
	void Test4L(TInt aNumberLines);
	void Test5L(TFaxResolution aRes, TFaxBufSenderId& aSenderId,
				TFaxCompression aF1, TInt aF2);

private:
    CWriteFaxFile*  iWriteFaxFile;
	CReadFaxFile*  iReadFaxFile;
	TFaxBufSenderId  iSenderId;
	};


class CFaxTestFaxCoding : public CFaxTestStep
	{
public:
	CFaxTestFaxCoding();
	virtual enum TVerdict doTestStepL();

	TInt FindEndOfLine(TDes8& aEncodedDes);
	};


class CFaxTestFaxHeaderDefinition : public CFaxTestStep
	{
public:
	CFaxTestFaxHeaderDefinition();
	virtual enum TVerdict doTestStepL();

	void GenerateFaxFileL(RFile& aFile);
	void GenerateHeaderFileL(const TUint8* aFont, TInt aWidth, TInt aHeight);
	};


class CFaxTestTransmitFax01 : public CFaxTestStep
	{
public:
	CFaxTestTransmitFax01();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestTransmitFax02 : public CFaxTestStep
	{
public:
	CFaxTestTransmitFax02();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestTransmitFax03 : public CFaxTestStep
	{
public:
	CFaxTestTransmitFax03();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestTransmitFax04 : public CFaxTestStep
	{
public:
	CFaxTestTransmitFax04();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestTransmitFax05 : public CFaxTestStep
	{
public:
	CFaxTestTransmitFax05();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestTransmitFax06 : public CFaxTestStep
	{
public:
	CFaxTestTransmitFax06();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestReceiveFax01 : public CFaxTestStep
	{
public:
	CFaxTestReceiveFax01();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestReceiveFax02 : public CFaxTestStep
	{
public:
	CFaxTestReceiveFax02();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestReceiveFax03 : public CFaxTestStep
	{
public:
	CFaxTestReceiveFax03();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestReceiveFax04 : public CFaxTestStep
	{
public:
	CFaxTestReceiveFax04();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestReceiveFax05 : public CFaxTestStep
	{
public:
	CFaxTestReceiveFax05();
	virtual enum TVerdict doTestStepL();
	};


class CFaxTestReceiveFax06 : public CFaxTestStep
	{
public:
	CFaxTestReceiveFax06();
	virtual enum TVerdict doTestStepL();
	};


#endif // __TE_FAXTEST_H__

