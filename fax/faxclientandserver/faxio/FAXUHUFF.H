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
// Header FAXUHUFF.H	 
// 
//

/**
 @file
 @internalComponent
*/

#if !defined(__FAXUHUFF_H__)
#define __FAXUHUFF_H__

/**
@internalComponent
*/
typedef TUint8 TNode[2];
typedef TUint TCode;

// 1728    = 11011000000
// 1728>>6 = 11011       = 27
/**
@internalComponent
*/
const TInt KEndCode=0x80;			// 0x080 = 128
const TInt KMaxRun=(1728>>6)+64;	// 0x05b = 91 = 27+64

/**
@internalComponent
*/
enum
	{
	KOurEol=KEndCode+KMaxRun+1,	// 0x0db = 219 = 128+90+1
	KBadRun,
	KStd1D,
	KTag1D=KStd1D,
	KTag2D,
	KHorzMode,
	KPassMode,
	KVtMode3n,
	KVtMode2n,
	KVtMode1n,
	KVtMode0, 
	KVtMode1p,
	KVtMode2p,
	KVtMode3p
	};

/**
@internalComponent
*/
#define NODE(left,right) {(left),(right)}
#define LEAF(val) (KEndCode|(val<64?val:0x40|(val>>6)))
#define CODE(tree,node,bit) (tree[node][bit?1:0])
#define ISBRANCH(code) (!(code&KEndCode))
#define ISERROR(code) (code>(KEndCode|KMaxRun))
#define CODEVALUE(code) (code&0x40 ? (code&0x3f)<<6 : code&0x3f)

/**
@internalComponent
*/
const TNode KWhiteTree [] =
	{
	NODE(1, 80),    // 0 
	NODE(40, 2),    // 1 
	NODE(21, 3),    // 2 
	NODE(4, LEAF(2)),    // 3 
	NODE(16, 5),    // 4 
	NODE(10, 6),    // 5 
	NODE(7, LEAF(256)),  // 6 
	NODE(9, 8),     // 7 
	NODE(LEAF(1344), LEAF(1408)),   // 8 
	NODE(LEAF(1216), LEAF(1280)),   // 9 
	NODE(11, 13),   // 10 
	NODE(LEAF(576), 12), // 11 
	NODE(LEAF(832), LEAF(896)),     // 12 
	NODE(14, 15),   // 13 
	NODE(LEAF(960), LEAF(1024)),    // 14 
	NODE(LEAF(1088), LEAF(1152)),   // 15 
	NODE(LEAF(1664), 17),// 16 
	NODE(18, 19),   // 17 
	NODE(LEAF(448), LEAF(512)),     // 18 
	NODE(20, LEAF(640)), // 19 
	NODE(LEAF(704), LEAF(768)),     // 20 
	NODE(32, 22),   // 21 
	NODE(23, 28),   // 22 
	NODE(24, 26),   // 23 
	NODE(LEAF(24), 25),  // 24 
	NODE(LEAF(49), LEAF(50)), // 25 
	NODE(27, LEAF(25)),  // 26 
	NODE(LEAF(51), LEAF(52)), // 27 
	NODE(29, LEAF(192)), // 28 
	NODE(30, 31),   // 29 
	NODE(LEAF(55), LEAF(56)), // 30 
	NODE(LEAF(57), LEAF(58)), // 31 
	NODE(LEAF(11), 33),  // 32 
	NODE(34, 36),   // 33 
	NODE(LEAF(27), 35),  // 34 
	NODE(LEAF(59), LEAF(60)), // 35 
	NODE(37, LEAF(18)),  // 36 
	NODE(38, 39),   // 37 
	NODE(LEAF(1472), LEAF(1536)),   // 38 
	NODE(LEAF(1600), LEAF(1728)),   // 39 
	NODE(59, 41),   // 40 
	NODE(49, 42),   // 41 
	NODE(43, LEAF(10)),  // 42 
	NODE(47, 44),   // 43 
	NODE(46, 45),   // 44 
	NODE(LEAF(320), LEAF(384)),     // 45 
	NODE(LEAF(63), LEAF(0)),   // 46 
	NODE(LEAF(28), 48),  // 47 
	NODE(LEAF(61), LEAF(62)), // 48 
	NODE(56, 50),   // 49 
	NODE(53, 51),   // 50 
	NODE(52, LEAF(21)),  // 51 
	NODE(LEAF(43), LEAF(44)), // 52 
	NODE(55, 54),   // 53 
	NODE(LEAF(41), LEAF(42)), // 54 
	NODE(LEAF(39), LEAF(40)), // 55 
	NODE(LEAF(12), 57),  // 56 
	NODE(58, LEAF(26)),  // 57 
	NODE(LEAF(53), LEAF(54)), // 58 
	NODE(70, 60),   // 59 
	NODE(64, 61),   // 60 
	NODE(62, LEAF(1)),   // 61 
	NODE(LEAF(19), 63),  // 62 
	NODE(LEAF(31), LEAF(32)), // 63 
	NODE(68, 65),   // 64 
	NODE(67, 66),   // 65 
	NODE(LEAF(37), LEAF(38)), // 66 
	NODE(LEAF(35), LEAF(36)), // 67 
	NODE(LEAF(20), 69),  // 68 
	NODE(LEAF(33), LEAF(34)), // 69 
	NODE(74, 71),   // 70 
	NODE(72, LEAF(13)),  // 71 
	NODE(LEAF(23), 73),  // 72 
	NODE(LEAF(47), LEAF(48)), // 73 
	NODE(77, 75),   // 74 
	NODE(76, LEAF(22)),  // 75 
	NODE(LEAF(45), LEAF(46)), // 76 
	NODE(79, 78),   // 77 
	NODE(LEAF(29), LEAF(30)), // 78 
	NODE(KOurEol, KBadRun), // 79 
	NODE(86, 81),   // 80 
	NODE(83, 82),   // 81 
	NODE(LEAF(6), LEAF(7)),   // 82 
	NODE(LEAF(5), 84),   // 83 
	NODE(85, LEAF(64)),  // 84 
	NODE(LEAF(14), LEAF(15)), // 85 
	NODE(90, 87),   // 86 
	NODE(88, LEAF(4)),   // 87 
	NODE(LEAF(9), 89),   // 88 
	NODE(LEAF(16), LEAF(17)), // 89 
	NODE(LEAF(3), 91),   // 90 
	NODE(LEAF(128), LEAF(8))  // 91 
	} ;

/**
@internalComponent
*/
const TNode KBlackTree [] =
	{
	NODE(2, 1),     // 0 
	NODE(LEAF(3), LEAF(2)),   // 1 
	NODE(4, 3),     // 2 
	NODE(LEAF(1), LEAF(4)),   // 3 
	NODE(6, 5),     // 4 
	NODE(LEAF(6), LEAF(5)),   // 5 
	NODE(9, 7),     // 6 
	NODE(8, LEAF(7)),    // 7 
	NODE(LEAF(9), LEAF(8)),   // 8 
	NODE(31, 10),   // 9 
	NODE(30, 11),   // 10 
	NODE(12, LEAF(12)),  // 11 
	NODE(23, 13),   // 12 
	NODE(17, 14),   // 13 
	NODE(15, LEAF(0)),    // 14 
	NODE(LEAF(21), 16),  // 15 
	NODE(LEAF(42), LEAF(43)), // 16 
	NODE(21, 18),   // 17 
	NODE(20, 19),   // 18 
	NODE(LEAF(38), LEAF(39)), // 19 
	NODE(LEAF(36), LEAF(37)), // 20 
	NODE(LEAF(20), 22),  // 21 
	NODE(LEAF(34), LEAF(35)), // 22 
	NODE(LEAF(15), 24),  // 23 
	NODE(27, 25),   // 24 
	NODE(26, LEAF(19)),  // 25 
	NODE(LEAF(28), LEAF(29)), // 26 
	NODE(29, 28),   // 27 
	NODE(LEAF(26), LEAF(27)), // 28 
	NODE(LEAF(128), LEAF(192)),     // 29 
	NODE(LEAF(10), LEAF(11)), // 30 
	NODE(57, 32),   // 31 
	NODE(45, 33),   // 32 
	NODE(34, LEAF(14)),  // 33 
	NODE(41, 35),   // 34 
	NODE(38, 36),   // 35 
	NODE(37, LEAF(22)),  // 36 
	NODE(LEAF(40), LEAF(41)), // 37 
	NODE(40, 39),   // 38 
	NODE(LEAF(32), LEAF(33)), // 39 
	NODE(LEAF(30), LEAF(31)), // 40 
	NODE(LEAF(17), 42),  // 41 
	NODE(44, 43),   // 42 
	NODE(LEAF(62), LEAF(63)), // 43 
	NODE(LEAF(48), LEAF(49)), // 44 
	NODE(LEAF(13), 46),  // 45 
	NODE(51, 47),   // 46 
	NODE(48, LEAF(16)),  // 47 
	NODE(50, 49),   // 48 
	NODE(LEAF(61), LEAF(256)),// 49 
	NODE(LEAF(57), LEAF(58)), // 50 
	NODE(55, 52),   // 51 
	NODE(54, 53),   // 52 
	NODE(LEAF(46), LEAF(47)), // 53 
	NODE(LEAF(44), LEAF(45)), // 54 
	NODE(LEAF(23), 56),  // 55 
	NODE(LEAF(50), LEAF(51)), // 56 
	NODE(KOurEol, 58),     // 57 
	NODE(75, 59),   // 58 
	NODE(67, 60),   // 59 
	NODE(61, LEAF(64)),  // 60 
	NODE(65, 62),   // 61 
	NODE(64, 63),   // 62 
	NODE(LEAF(1152), LEAF(1216)),   // 63 
	NODE(LEAF(1024), LEAF(1088)),   // 64 
	NODE(LEAF(54), 66),  // 65 
	NODE(LEAF(896), LEAF(960)),     // 66 
	NODE(72, 68),   // 67 
	NODE(71, 69),   // 68 
	NODE(70, LEAF(53)),  // 69 
	NODE(LEAF(512), LEAF(576)),     // 70 
	NODE(LEAF(384), LEAF(448)),     // 71 
	NODE(LEAF(25), 73),  // 72 
	NODE(74, LEAF(320)), // 73 
	NODE(LEAF(1664), LEAF(1728)),   // 74 
	NODE(85, 76),   // 75 
	NODE(80, 77),   // 76 
	NODE(78, LEAF(24)),  // 77 
	NODE(LEAF(60), 79),  // 78 
	NODE(LEAF(1536), LEAF(1600)),   // 79 
	NODE(83, 81),   // 80 
	NODE(82, LEAF(59)),  // 81 
	NODE(LEAF(1408), LEAF(1472)),   // 82 
	NODE(LEAF(56), 84),  // 83 
	NODE(LEAF(1280), LEAF(1344)),   // 84 
	NODE(LEAF(18), 86),  // 85 
	NODE(89, 87),   // 86 
	NODE(88, LEAF(55)),  // 87 
	NODE(LEAF(768), LEAF(832)),     // 88 
	NODE(LEAF(52), 90),  // 89 
	NODE(LEAF(640), LEAF(704))// 90 
	};

/**
@internalComponent
*/
const TNode KTwoTree [] =
    {
    NODE(1, KVtMode0),         // 0
    NODE(2, 3),                // 1
    NODE(4, KHorzMode),        // 2
    NODE(KVtMode1n, KVtMode1p),// 3
    NODE(5, KPassMode),        // 4
    NODE(6, 7),                // 5
    NODE(8, 9),                // 6
    NODE(KVtMode2n, KVtMode2p),// 7
    NODE(KOurEol, KBadRun),    // 8
    NODE(KVtMode3n, KVtMode3p) // 9
    };

/**
@internalComponent
*/
const TNode KSynchTree[]=
	{
	NODE(0,KStd1D)		// any number of 0's followed by a 1
	};

/**
@internalComponent
*/
const TNode KTagTree[]=
	{
	NODE(0,1),
	NODE(KTag2D,KTag1D)
	};

#endif // __FAXUHUFF_H__
