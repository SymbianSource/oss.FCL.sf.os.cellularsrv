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

#include <s32mem.h>
#include "CFAXIO.H"

#include "FAXSTPAN.H"
#include "FAXUHUFF.H"
#include "FAXHUFF.H"

// COPIED function from pdrutil

GLDEF_C void Panic (TFaxStorePanic aPanic)
// Panic the process with ETEXT as the category.
 //

{
	User::Panic (_L ("FaxStore"), aPanic);
}

// END OF COPIED

//#define KFaxFileName _L("c:\\temp.fax")

/********************************************************************/

#define RLE_MAKEUP(aTable,aRun) TUint8((aTable)+KRleMakeup+((aRun)>>6<<1))
#define RLE_RUN(aTable,aRun) TUint8((aTable)+((aRun)<<1))

#define WRITE_RLE(ptr,table,run) {if (run>63) {*ptr++=RLE_MAKEUP(table,run);run&=0x3f;} *ptr++=RLE_RUN(table,run);}
#define READ_RLE(ptr,pos) {TInt x=*ptr++;if (x>=KRleMakeup) {pos+=(x&KRleMakeupMask)<<5;x=*ptr++;}pos+=x>>1;}

LOCAL_C TUint8* RleEncoding(const TUint32 * aScan, TUint8* aRleEncoding)
//
// RLE encode a 1728 pixel scanline into the buffer, and return the end-of-rle data
// The edge detection algorithm is almost optimal for ARM
//
	{
	// the edge detection looks at multiple pixels at a time
	// tests show that for ARM, testing 8 is only 1-2% faster than testing 6
	// testing only 6 makes for a smaller lookup table

	// The FirstBit table is (5 - bitpos) of the least significant
	// bit that is set in the array index value

	const TInt KTestBitCount=6;
	const TInt KTestBitMask=(1u<<KTestBitCount)-1;
	const TInt KRunBias=KTestBitCount-2;
	static const TUint8 KFirstBit[64]=
		{
		0,5,4,5,3,5,4,5,2,5,4,5,3,5,4,5,1,5,4,5,3,5,4,5,2,5,4,5,3,5,4,5,
		0,5,4,5,3,5,4,5,2,5,4,5,3,5,4,5,1,5,4,5,3,5,4,5,2,5,4,5,3,5,4,5
		};

	const TUint32 *end = aScan + (KFaxPixelsPerScanLine >> 5);
	TUint32 color = ~0u;			// white at start
	TInt table=KRleWhite;
	TInt run = KRunBias;			// initialise run length
	const TUint8* lookup=KFirstBit;	// force the table to be in a register

	nextword:
	while (aScan < end)
		{
		run += 32;
		TUint32 pixels = *aScan++ ^ color;
		if (pixels)      // do no work if there is no edge
			{
			TInt bit = 31 + KTestBitCount;
			for (;;)
				{
				TUint pix;
				do
					{
					if ((bit-=KTestBitCount) < 0)
						goto nextword;	// finished processing the word
					// now examine the next 6 pixels
					// break out if we have found an edge
					pix=(pixels>>(31-bit))&KTestBitMask;
					} while (pix==0);
				// there is an edge, use the table to discover which pixel
				bit+=lookup[pix];
				// store the run-length
				run-=bit;
				WRITE_RLE(aRleEncoding,table,run);
				// flip color and look for the next edge
				color = ~color;
				table=KRleWhite-table;
				pixels=~pixels;
				run = bit;
				}
			}
		}
	// store the final run
	run-=KRunBias;
	WRITE_RLE(aRleEncoding,table,run);
	return aRleEncoding;
	}

LOCAL_C TUint8* RleEncoding (const TUint32 *aScanline,TInt aLength,TUint8* aRleEncoding)
//
// Justify the scanline into a full size buffer before encoding
//
	{
	__ASSERT_DEBUG(aLength < (KFaxPixelsPerScanLine >> 3),User::Invariant());
//
	TUint32 justified[KFaxPixelsPerScanLine/32];
//
	TInt margin = ((KFaxPixelsPerScanLine >> 3) - aLength) / 2;
	Mem::Fill (justified, sizeof(justified), 0xff);       // white fill
	Mem::Copy ((TUint8*)justified + margin, aScanline, aLength);
	return RleEncoding(justified,aRleEncoding);
	}

LOCAL_C TUint8* RleEncoding (const TDesC8& aScanLine, TUint8* aRleEncoding)
//
// Build the RLE encoding for aScanline, handling wrong-sized scanlines
//
	{
	TInt len = aScanLine.Length ();
	const TUint32 *scan = (const TUint32 *) aScanLine.Ptr ();
	__ASSERT_DEBUG ((TUint (scan) & 3) == 0, Panic (EFaxEncodeScanlineAlignment));
	if (len >= (KFaxPixelsPerScanLine >> 3))
		return RleEncoding(scan + ((len - (KFaxPixelsPerScanLine >> 3)) >> 3),aRleEncoding);  // margin in words
	else
		return RleEncoding(scan,len,aRleEncoding);
	}

LOCAL_C void EncodeHuffman(TDes8 & anEncodedScanLine,TInt aTagCode,const TUint8* aRleEncoding,const TUint8* aRleEnd)
//
// Starting with the tag code, encode all codes in the rle data using the single huffman table
//
	{
	TUint8 *t4 = (TUint8 *) anEncodedScanLine.Ptr ();
	TUint8 *const e4 = t4;
	// start with tag code
	TCodeDef huff=KCodes[aTagCode];
	TUint code=HUFFBITS(huff);
	TInt bits=HUFFLEN(huff)-16;
	while (aRleEncoding<aRleEnd)
		{
		__ASSERT_DEBUG (bits < 0, User::Invariant ());
		TUint8 c=*aRleEncoding++;
		TCodeDef huff=KCodes[c];
		code|=HUFFBITS(huff)>>(bits+16);
		bits+=HUFFLEN(huff);
		if (bits<0)
			continue;
		*t4++=TUint8(code>>24);
		*t4++=TUint8(code>>16);
		code<<=16;
		bits-=16;
		}
	if (bits>-16)
		{	// flush out the remaining bits
		*t4++=TUint8(code>>24);
		if (bits>-8)
			*t4++=TUint8(code>>16);
		}
	anEncodedScanLine.SetLength (t4 - e4);
	}

/********************************************************************/

inline CFaxT4::CFaxT4 ()
	{PageInitialize(EFaxNormal,EModifiedHuffman);}

EXPORT_C CFaxT4 *CFaxT4::NewLC ()
/** Constructs a CFaxT4 object, which provides utility functions to encode and 
decode fax scan lines. 

As is usual in Symbian OS, the only difference between this function and NewL() 
is that this variant pushes the object to the cleanup stack.

The new object is constructed with the default compression and resolution: 
EModifiedHuffman and EFaxNormal respectively.

@leave KErrNoMemory There is insufficient memory to perform the operation.
@return A pointer to the newly created object. 
@capability None
*/
	{
	CFaxT4 *self = NewL ();
	CleanupStack::PushL (self);
	return self;
	}

EXPORT_C CFaxT4 *CFaxT4::NewL ()
/** Constructs a CFaxT4 object, which provides utility functions to encode and 
decode fax scan lines. 

The function is exactly the same as NewLC() except that the new object is 
popped from the cleanup stack.

The new object is constructed with the default compression and resolution: 
EModifiedHuffman and EFaxNormal respectively.

@leave KErrNoMemory There is insufficient memory to perform the operation. 
@return A pointer to the newly created object. 
@capability None
*/
	{
	return new (ELeave) CFaxT4;
	}

EXPORT_C void CFaxT4::PageInitialize (TFaxResolution aResolution, TFaxCompression aCompression, TInt aFlag2)
/**
Initialize fax page, set page parameters.

@param   aResolution     defines fax resolution
@param   aCompression    defines fax compression
@param   aFlag2          reserved flag.
@capability None
*/
	{
	__ASSERT_ALWAYS (((aCompression == EModifiedHuffman) || (aCompression == EModifiedRead)), Panic (EFaxUnsupportedCompression));
	iCompression = aCompression;
	iResolution = aResolution;
	iReservedFlag2 = aFlag2;
	iK = iResolution == EFaxFine ? 4 : 2;
	iLineCount = 1;
	// an all-white reference line
	iRef[0]=RLE_MAKEUP(KRleWhite,KFaxBytesPerScanLine);
	iRef[1]=RLE_RUN(KRleWhite,0);
	iEndRef=iRef+2;
	}

EXPORT_C void CFaxT4::EncodeScanLine (const TDesC8 & aScanLine, TDes8 & anEncodedScanLine)
/** Encodes a scan line using either one dimensional Modified Huffman (MH) or two 
dimensional Modified Read (MR) encoding. 

The type of encoding used depends on the compression type specified when the 
object was initialised - using PageInitialize(). If the object was not initialised, 
then the default compression is MH.

@param aScanLine The raw scan line to be encoded. 
@param anEncodedScanLine On return, contains the encoded scan line. 
@capability None
*/
	{
	if (iCompression == EModifiedRead)
		EncodeScanLine2D (aScanLine, anEncodedScanLine);
	else
		EncodeScanLine1D(aScanLine,anEncodedScanLine);
	}

EXPORT_C void CFaxT4::EncodeScanLine1D (const TDesC8 & aScanLine, TDes8 & anEncodedScanLine)
/** Encodes a scan line using Modified Huffman compression.

@param aScanLine The scan line to be encoded. 
@param anEncodedScanLine On return, contains the MH encoded scan line. 
@capability None
*/
	{
	iEndRef=RleEncoding(aScanLine,iRef);
	EncodeHuffman(anEncodedScanLine,iCompression == EModifiedHuffman ? KRleStd1D : KRleTag1D,iRef,iEndRef);
	}

EXPORT_C void CFaxT4::EncodeScanLine2D (const TDesC8 & aScanLine, TDes8 & anEncodedScanLine)
/** Encodes a scan line using Modified Read compression.

@param aScanLine The scan line to be encoded. 
@param anEncodedScanLine On return, contains the MR encoded scan line. 
@capability None
*/
	{
	// initialize our own scan line
	TInt lc=iLineCount-1;
	if (lc==0)
		{	// 1D reference line
		iLineCount=iK;
		EncodeScanLine1D(aScanLine,anEncodedScanLine);
		}
	else
		{	// 2D line
		iLineCount=lc;
		DoEncodeScanLine2D(aScanLine,anEncodedScanLine);
		}
	}

void CFaxT4::DoEncodeScanLine2D (const TDesC8 & aScanLine, TDes8 & aEncodedScanLine)
	{
	TUint8 rlebuf[KFaxPixelsPerScanLine*3/2 + 16]; // for output + reference line

	// the buffer is big enough that the 2d coding output into the buffer will not
	// catch the reference coding before it is used

	// copy the reference line into the end of the stack buffer

	TInt len=iEndRef-iRef;
	TUint8* ref=rlebuf+sizeof(rlebuf)-len;
	Mem::Copy(ref,iRef,len);

	// Do the standard RLE encoding of the current line
	iEndRef=RleEncoding(aScanLine,iRef);
	const TUint8* cur=iRef;

	TUint8* rle=rlebuf;
	TInt a0=-1;			// previous edge on current line
	TInt a1=0;			// current edge on current line
	TInt b0;			// previous edge on reference line
	TInt b1=0;			// current edge on reference line
	TInt b2=0;			// look-ahead edge on reference line
	TInt color=KRleWhite;	// color at a0 (initially white)

	// the reference color is not tracked. Instead the number of reference edges
	// traversed is monitored (modulo 2) to ensure that edge b1 is of the same
	// color to a1 at "gotB2"

	READ_RLE(cur,a1);		// find the first edge

	for (;;)
		{
		do
			{	// find the new current and next edges on reference line
			b0=b1;
			b1=b2;
			if (b1==KFaxPixelsPerScanLine)
				break;			// end of line
			READ_RLE(ref,b2);
refMove1:	//	find just the look-ahead edge on the reference line
			b0=b1;
			b1=b2;
			if (b1==KFaxPixelsPerScanLine)
				break;
			READ_RLE(ref,b2);
			} while(b1<=a0);		// ensure that we have the right reference edge

gotB2:	if (b2 < a1)
			{	// pass mode detected
			*rle++=KRlePassMode;
			a0=b2;	// move along by 2 edges
			continue;
			}

		if (TUint(b1-a1+3)<=6u)
			{	// vertical mode
			*rle++=TUint8(KRleVertMode0 + (b1 - a1));
			if (a1==KFaxPixelsPerScanLine)
				break;		// complete
			if (b0>a1)
				{
				// special case of vertical mode edge "cross-over"
				// the next edge may match an earlier reference edge than this!
				// rewind the reference line by 2 edges
				// we know that [b0,b1] is small, and so only uses 1 byte in the rle
				// we check for [b1,b2] requiring a makeup byte as well
				ref-=2;
				if (b2-b1>=64)
					--ref;
				b2=b0;
				b1=0;	// no longer know b0, but this cannot happen again without traversing 2 edges
				}
			a0 = a1;	// traverse a single edge
			READ_RLE(cur,a1);
			color=KRleWhite-color;
			goto refMove1;
			}

		// we must be in horizontal mode - write out the RLE codes for remainder
		// and copy RLE codes for next edge from current coding

		*rle++=KRleHorzMode;
		a0=Max(0,a0);			// deal with start-effects (a0==-1)
		TInt run=a1-a0;
		WRITE_RLE(rle,color,run);
		// copy the next run
		if (a1==KFaxPixelsPerScanLine)
			{	// complete, need a zero-length, other-color, run to finish
			*rle++=RLE_RUN(KRleWhite-color,0);
			break;
			}
		// copy the next RLE code directly from the current line
		TInt x=*cur++;
		__ASSERT_DEBUG((x&KRleWhite)==KRleWhite-color,User::Invariant());
		if (x>=KRleMakeup)
			{
			*rle++=TUint8(x);
			a1+=(x&KRleMakeupMask)<<5;
			x=*cur++;
			}
		*rle++=TUint8(x);
		a1+=x>>1;
		if (a1==KFaxPixelsPerScanLine)
			break;	// complete
		a0=a1;
		READ_RLE(cur,a1);	// traverse another edge
		if (b1>a0)
			goto gotB2;
		}
	EncodeHuffman(aEncodedScanLine,KRleTag2D,rlebuf,rle);
	}

EXPORT_C TInt CFaxT4::DecodeScanLine (TDes8 & aScanLine, const TDesC8 & anEncodedScanLine)
/** Decodes a scan line. 

The decoding method depends on the compression type specified when the object 
was initialised - using PageInitialize(). If the object was not initialised, 
then the scan line is decoded as Modified Huffman.

The fax client can determine the type of compression used in a fax from its 
header, and can hence use PageInitialize() to set the correct decoding method. 
KErrUnderflow is returned if the wrong type of compression is specified.

@param aScanLine On return, contains the decoded scan line. 
@param anEncodedScanLine The encoded scan line to be decoded. 
@return KErrNone if successful, otherwise another of the system-wide error 
codes. 
@capability None
*/
	{
	if (iCompression == EModifiedHuffman)
		return (DecodeScanLine1D (aScanLine, anEncodedScanLine));
	else
		return (DecodeScanLine2D (aScanLine, anEncodedScanLine));
	}

void CFaxT4::DecodeHuffman(const TDesC8 & aEncodedScanLine)
	{
	// If all goes wrong then the reference line is unchanged and will be
	// used for the current line

	const TUint8* t4=aEncodedScanLine.Ptr();
	const TUint8* endt4=t4+aEncodedScanLine.Length();
	TUint bits=0;

	// store the basic RLE data locally, and copy to the member data if the decode
	// is successful

	TUint8 rlebuf[KFaxPixelsPerScanLine+4];
	TUint8* rle=rlebuf;

	// Keep track of where we have got to on the reference (previous) line
	const TUint8* ref=iRef;
	TInt b1=0;		// pixel position on the reference line
	TInt a0=0;		// previous position on the current line
	TInt a1=0;		// current position on the current line

	// start decoding using the tag-tree, which finds the first 1-bit
	// and then determines the encoding (1D or 2D) based on the next bit
	const TNode* tree=KTagTree;

	// "color" stores the current line color (in bit 0), the reference line
	// color (in bit 1), and the number of white/black codes to expect (x4)
	// if color<0, then we are in 2d-tree mode
	// initially unused until the encoding type is known

	TInt color=0;
	TInt code;

	for (;;)
		{
		// the structure of the following code maxmises the speed of the
		// huffman decoder. Don't change it.
		code = 0;		// start at the root of the tree
nextBit2d:
		if (((bits <<= 1) & 0x80000000)==0)
			goto nextByte2d;		// run out of bits in the current byte
decode2d:
		code = CODE (tree, code, bits & 0x80);
		if (ISBRANCH (code))
			goto nextBit2d;			// a branch code

		// We have the huffman code

		if (code<KOurEol)
			{	// the code was a white/black length code
			__ASSERT_DEBUG(color>=0,User::Invariant());
			TInt v=CODEVALUE(code);
			a1+=v;
			if (a1>KFaxPixelsPerScanLine)
				return;		// overflow
			if (v < 64)
				{	// a run code (as opposed to make-up code). Emit the RLE
				a0=a1-a0;
				WRITE_RLE(rle,(color&1),a0);
				a0=a1;
				color^=KRleWhite;	// switch color
				color-=4;			// one less white/black code
				tree=color>=0 ? color&KRleWhite ? KWhiteTree : KBlackTree : KTwoTree;
				}
			continue;
			}
		if (code<KPassMode)
			{
			if (code == KHorzMode)
				{	// expect two white/black codes to follow
				color+=8;
				tree = color&KRleWhite ? KWhiteTree : KBlackTree;
				continue;
				}
			if (code==KTag1D)
				{	// 1d decoding: set color to maximum positive value
					// as all codes are standard white/black code.
				color=KMaxTInt;		// current and reference color both 1 (white)
				tree=KWhiteTree;
				continue;
				}
			if (code==KTag2D)
				{	// 2d decoding: set color negative to indicate 2d-tree
				color=-1;			// current and reference color both 1 (white)
				tree=KTwoTree;
				continue;
				}
			if (code==KOurEol)
				goto eol2d;
			__ASSERT_DEBUG(code == KBadRun,User::Invariant());
			return;			// bad run, give up
			}

		// The remaining 2D possibilities all require that we know the various 2D determinants
		// so we proceed as follows :

		// b0 tracks the previous "edge" in the reference line
		TInt b0=0;
		// find the next "edge" on the reference line after a1
		// if we've just started decoding (rle==rlebuf), b1 boundary at 0 is correct
		if (rle!=rlebuf)
			{
			while (b1<=a1)
				{
				color^=KRleWhite<<1;
				b0=b1;
				if (b1!=KFaxPixelsPerScanLine)
					READ_RLE(ref,b1);
				}
			}
		// the b1 "edge" must match the colors of a1, so move one more edge if
		// the a0 color (color&1) is the same as the b1 color (color&2)
		if (((color^(color>>1))&1)==0)
			{
			b0=b1;
			if (b1!=KFaxPixelsPerScanLine)
				READ_RLE(ref,b1);
			color^=KRleWhite<<1;
			}

		// If the code is below PASSMODE then it is one of the vertical code words
		// which are pretty easy to decipher as we have all the data. Vertical mode
		// flips the colour and then continues

		if (code==KPassMode)
			{
			// we need to identify the next reference "edge"
			if (b1==KFaxPixelsPerScanLine)
				return;		// overflow
			READ_RLE(ref,b1);
			if (b1==KFaxPixelsPerScanLine)
				return;		// overflow
			color^=KRleWhite<<1;
			a1=b1;
			continue;
			}

		__ASSERT_DEBUG(code>=KVtMode3n && code<=KVtMode3p,User::Invariant());
		// vertical mode
		a1=b1+(code-(KVtMode0));
		if (a1>KFaxPixelsPerScanLine)
			return;		// overflow
		if (b0>a1)
			{
			// special case of vertical mode cross-over
			// rewind the reference line to the previous "edge"
			b1=b0;
			--ref;
			color^=KRleWhite<<1;
			}
		a0=a1-a0;
		WRITE_RLE(rle,(color&1),a0);
		a0=a1;
		color^=KRleWhite;
		}
nextByte2d:
	if (t4 < endt4)
		{
		bits = 0xff000000u | *t4++;
		goto decode2d;
		}
eol2d:
	if (a0==KFaxPixelsPerScanLine)
		iEndRef=Mem::Copy(iRef,rlebuf,rle-rlebuf);
	}

EXPORT_C TInt CFaxT4::DecodeScanLine2D (TDes8 & aScanLine, const TDesC8 & aEncodedScanLine)
/** Decodes a Modified Read encoded scan line.
	
@param aScanLine On return, contains the decoded scan line. 
@param anEncodedScanLine The 2D encoded scan line to be decoded. 
@return KErrNone if successful, KErrUnderflow if the scan line is encoded as 
MR, otherwise another of the system-wide error codes. 
@capability None
*/
	{
	DecodeHuffman(aEncodedScanLine);
//
//	decode the RLE into the scanline
//
	aScanLine.SetLength (KFaxPixelsPerScanLine / 8);
	TUint32 *scan = (TUint32 *) aScanLine.Ptr ();
	__ASSERT_DEBUG ((TUint (scan) & 3) == 0, Panic (EFaxDecodeScanlineAlignment));

	const TUint8* rle=iRef;
	const TUint8* const end=iEndRef;

	TUint color = ~0u;		// start white
	TUint out = 0;			// output pixels in accumulation
	TInt bits = 0;          // this is the number of used bits in out

	while (rle<end)
		{
		TInt run=*rle++;
		__ASSERT_DEBUG(TUint((run&1))==(color&1),User::Invariant());	// rle-data correct
		if (run<KRleMakeup)
			{	// run length code (x2)
			out += color << bits;		// complete the current 32-bits
			bits += run >> 1;			// add the run length
			if (bits < 32)				// hasn't completed the word
				out -= color << bits;	// remove the trailing bits
			else
				{
				*scan++ = out;			// write the 32-bits
				bits -= 64;
				if (bits >= 0)
					*scan++ = color;	// + another 32 bits
				else
					bits += 32;			// no extra
				out = color - (color<<bits);	// bits remaining
				}
			color = ~color;				// swap color
			}
		else
			{
			// make-up code. (run-KRleMakeup)>>1 is the multiple of 64 bits
			out += color << bits;		// complete the current 32-bits
			*scan++ = out;				// output
			*scan++ = color;			// +32 bits of color
			for (run -= KRleMakeup+4;run >= 0;run -= 2)
				{	// extra multiples of 64 bits
				*scan++ = color;
				*scan++ = color;
				}
			out = color - (color<<bits);	// remainder bits
			}
		}
	return KErrNone;
	}

EXPORT_C TInt CFaxT4::DecodeScanLine1D (TDes8 & aScanLine, const TDesC8 & anEncodedScanLine)
//
// This could be done through DecodeScanLine2D, but this is an optimized version for 1D
// The intermediate rle encoding is skipped
//
/** Decodes a Modified Huffman encoded scan line.

@param aScanLine On return, contains the decoded scan line. 
@param anEncodedScanLine The MH encoded scan line to be decoded. 
@return KErrNone if successful, KErrUnderflow if the scan line is encoded as 
MR, otherwise another of the system-wide error codes. 
@capability None
*/
	{
	const TUint8 *t4 = anEncodedScanLine.Ptr ();
	const TUint8 *const endt4 = t4 + anEncodedScanLine.Length ();
	//
	aScanLine.SetLength (KFaxPixelsPerScanLine / 8);
	TUint32 *scan = (TUint32 *) aScanLine.Ptr ();
	__ASSERT_DEBUG ((TUint (scan) & 3) == 0, Panic (EFaxDecodeScanlineAlignment));
	TUint out = 0;
	TInt bits = 0;               // this is the number ofused bits
	//
	TInt dotsleft = KFaxPixelsPerScanLine;
	const TNode *tree = KSynchTree;
	TUint octet=0;
	TUint color = 0xffffffffu;

nextCode1d:
	TInt code = 0;
nextBit1d:
	if (((octet <<= 1) & 0x80000000)==0)
		goto nextByte1d;
decode1d:
	code = CODE (tree, code, octet & 0x80);
	if (ISBRANCH (code))
		goto nextBit1d;

	// end of huffman code

	if (code<KEndCode+0x40)
		{
		code-=KEndCode;
		if ((dotsleft -= code) < 0)
			return KErrOverflow;
		out += color<<bits;
		bits+=code;
		if (bits<32)
			out -= color<<bits;	// output word not full
		else
			{	// out is full
			*scan++=out;
			bits-=64;
			if (bits>=0)
				*scan++=color;
			else
				bits+=32;
			out=color-(color<<bits);
			}
		color = ~color;
		if (color)
			tree=KWhiteTree;
		else
			tree=KBlackTree;
		goto nextCode1d;
		}
	else if (code<=KEndCode+KMaxRun)
		{	// makeup (multiple of 64)
		code-=KEndCode+0x40;
		if ((dotsleft -= code<<6) < 0)
			return KErrOverflow;
		out += color << bits;
		*scan++=out;
		*scan++=color;
		for (code-=2;code>=0;--code)
			{
			*scan++=color;
			*scan++=color;
			}
		out = color-(color<<bits);
		goto nextCode1d;
		}
	else if (code == KOurEol)
		goto eol1d;
	else if (code == KStd1D)
		{
		tree=KWhiteTree;
		goto nextCode1d;
		}
	else
		{
		__ASSERT_DEBUG (code == KBadRun, User::Invariant ());
		return KErrCorrupt;
		}
nextByte1d:
	if (t4 < endt4)
		{
		octet = 0xff000000u | *t4++;
		goto decode1d;
		}
eol1d:
	__ASSERT_DEBUG (dotsleft || (TUint8 *) scan - aScanLine.Ptr () == KFaxPixelsPerScanLine / 8, User::Invariant ());
	return dotsleft ? KErrUnderflow : KErrNone;
	}

