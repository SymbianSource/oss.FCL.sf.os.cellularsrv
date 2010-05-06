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
// Provides test functions to generate strings based on types of character sets.
// 
//

/**
 @file
*/

#include "TE_smsprt_alphabet.h"


/**
 *  Standard 7bit ASCII text that is in the GSM default table.
 */
const TMappedChar  gStandard7bitGSMChars[] =
	{
	{0x0040, 0x0040, 1}, /* COMMERCIAL AT */
	{0x00A3, 0x00A3, 1}, /* POUND SIGN */
	{0x0024, 0x0024, 1}, /* DOLLAR SIGN */
	{0x00A5, 0x00A5, 1}, /* YEN SIGN */
	{0x00E8, 0x00E8, 1}, /* LATIN SMALL LETTER E WITH GRAVE */
	{0x00E9, 0x00E9, 1}, /* LATIN SMALL LETTER E WITH ACUTE */
	{0x00F9, 0x00F9, 1}, /* LATIN SMALL LETTER U WITH GRAVE */
	{0x00EC, 0x00EC, 1}, /* LATIN SMALL LETTER I WITH GRAVE */
	{0x00F2, 0x00F2, 1}, /* LATIN SMALL LETTER O WITH GRAVE */
	{0x00E7, 0x00E7, 1}, /* LATIN SMALL LETTER C WITH CEDILLA */ /* Techically should be capital! */
	{0x000A, 0x000A, 1}, /* LINE FEED (LF) */
	{0x00D8, 0x00D8, 1}, /* LATIN CAPITAL LETTER O WITH STROKE */
	{0x00F8, 0x00F8, 1}, /* LATIN SMALL LETTER O WITH STROKE */
	{0x000D, 0x000D, 1}, /* Carriage return */
	{0x00C5, 0x00C5, 1}, /* LATIN CAPITAL LETTER A WITH RING ABOVE */
	{0x00E5, 0x00E5, 1}, /* LATIN SMALL LETTER A WITH RING ABOVE */
	{0x0394, 0x0394, 1}, /* GREEK CAPITAL LETTER DELTA */
	{0x005F, 0x005F, 1}, /* LOW LINE */
	{0x03A6, 0x03A6, 1}, /* GREEK CAPITAL LETTER PHI */
	{0x0393, 0x0393, 1}, /* GREEK CAPITAL LETTER GAMMA */
	{0x039B, 0x039B, 1}, /* GREEK CAPITAL LETTER LAMDA */
	{0x03A9, 0x03A9, 1}, /* GREEK CAPITAL LETTER OMEGA */
	{0x03A0, 0x03A0, 1}, /* GREEK CAPITAL LETTER PI */
	{0x03A8, 0x03A8, 1}, /* GREEK CAPITAL LETTER PSI */
	{0x03A3, 0x03A3, 1}, /* GREEK CAPITAL LETTER SIGMA */
	{0x0398, 0x0398, 1}, /* GREEK CAPITAL LETTER THETA */
	{0x039E, 0x039E, 1}, /* GREEK CAPITAL LETTER XI */
	// Escape code
	{0x00C6, 0x00C6, 1}, /* LATIN CAPITAL LETTER AE */
	{0x00E6, 0x00E6, 1}, /* LATIN SMALL LETTER AE */
	{0x00DF, 0x00DF, 1}, /* LATIN SMALL LETTER SHARP S */
	{0x00C9, 0x00C9, 1}, /* LATIN CAPITAL LETTER E WITH ACUTE */
	{0x0020, 0x0020, 1}, /* SPACE */
	{0x0021, 0x0021, 1}, /* EXCLAMATION MARK */
	{0x0022, 0x0022, 1}, /* QUOTATION MARK */
	{0x0023, 0x0023, 1}, /* NUMBER SIGN */
	{0x00A4, 0x00A4, 1}, /* CURRENCY SIGN */
	{0x0025, 0x0025, 1}, /* PERCENT SIGN */
	{0x0026, 0x0026, 1}, /* AMPERSAND */
	{0x0027, 0x0027, 1}, /* APOSTROPHE */
	{0x0028, 0x0028, 1}, /* LEFT PARENTHESIS */
	{0x0029, 0x0029, 1}, /* RIGHT PARENTHESIS */
	{0x002A, 0x002A, 1}, /* ASTERISK */
	{0x002B, 0x002B, 1}, /* PLUS SIGN */
	{0x002C, 0x002C, 1}, /* COMMA */
	{0x002D, 0x002D, 1}, /* Hyphen/Minus sign */
	{0x002E, 0x002E, 1}, /* FULL STOP */
	{0x002F, 0x002F, 1}, /* SOLIDUS */
	{0x0030, 0x0030, 1}, /* Digit Zero */
	{0x0031, 0x0031, 1}, /* Digit One */
	{0x0032, 0x0032, 1}, /* Digit Two */
	{0x0033, 0x0033, 1}, /* Digit Three */
	{0x0034, 0x0034, 1}, /* Digit Four */
	{0x0035, 0x0035, 1}, /* Digit Five */
	{0x0036, 0x0036, 1}, /* Digit Six */
	{0x0037, 0x0037, 1}, /* Digit Seven */
	{0x0038, 0x0038, 1}, /* Digit Eight */
	{0x0039, 0x0039, 1}, /* Digit Nine */
	{0x003A, 0x003A, 1}, /* COLON */
	{0x003B, 0x003B, 1}, /* SEMICOLON */
	{0x003C, 0x003C, 1}, /* LESS-THAN SIGN */
	{0x003D, 0x003D, 1}, /* EQUALS SIGN */
	{0x003E, 0x003E, 1}, /* GREATER-THAN SIGN */
	{0x003F, 0x003F, 1}, /* QUESTION MARK */
	{0x00A1, 0x00A1, 1}, /* INVERTED EXCLAMATION MARK */
	{0x0041, 0x0041, 1}, /* Latin Capital letter A */
	{0x0042, 0x0042, 1}, /* Latin Capital letter B */
	{0x0043, 0x0043, 1}, /* Latin Capital letter C */
	{0x0044, 0x0044, 1}, /* Latin Capital letter D */
	{0x0045, 0x0045, 1}, /* Latin Capital letter E */
	{0x0046, 0x0046, 1}, /* Latin Capital letter F */
	{0x0047, 0x0047, 1}, /* Latin Capital letter G */
	{0x0048, 0x0048, 1}, /* Latin Capital letter H */
	{0x0049, 0x0049, 1}, /* Latin Capital letter I */
	{0x004A, 0x004A, 1}, /* Latin Capital letter J */
	{0x004B, 0x004B, 1}, /* Latin Capital letter K */
	{0x004C, 0x004C, 1}, /* Latin Capital letter L */
	{0x004D, 0x004D, 1}, /* Latin Capital letter M */
	{0x004E, 0x004E, 1}, /* Latin Capital letter N */
	{0x004F, 0x004F, 1}, /* Latin Capital letter O */
	{0x0050, 0x0050, 1}, /* Latin Capital letter P */
	{0x0051, 0x0051, 1}, /* Latin Capital letter Q */
	{0x0052, 0x0052, 1}, /* Latin Capital letter R */
	{0x0053, 0x0053, 1}, /* Latin Capital letter S */
	{0x0054, 0x0054, 1}, /* Latin Capital letter T */
	{0x0055, 0x0055, 1}, /* Latin Capital letter U */
	{0x0056, 0x0056, 1}, /* Latin Capital letter V */
	{0x0057, 0x0057, 1}, /* Latin Capital letter W */
	{0x0058, 0x0058, 1}, /* Latin Capital letter X */
	{0x0059, 0x0059, 1}, /* Latin Capital letter Y */
	{0x005A, 0x005A, 1}, /* Latin Capital letter Z */
	{0x00C4, 0x00C4, 1}, /* LATIN CAPITAL LETTER A WITH DIAERESIS */
	{0x00D6, 0x00D6, 1}, /* LATIN CAPITAL LETTER O WITH DIAERESIS */
	{0x00D1, 0x00D1, 1}, /* LATIN CAPITAL LETTER N WITH TILDE */
	{0x00DC, 0x00DC, 1}, /* LATIN CAPITAL LETTER U WITH DIAERESIS */
	{0x00A7, 0x00A7, 1}, /* SECTION SIGN */
	{0x00BF, 0x00BF, 1}, /* INVERTED QUESTION MARK */
	{0x0061, 0x0061, 1}, /* Latin Small Letter A */
	{0x0062, 0x0062, 1}, /* Latin Small Letter B */
	{0x0063, 0x0063, 1}, /* Latin Small Letter C */
	{0x0064, 0x0064, 1}, /* Latin Small Letter D */
	{0x0065, 0x0065, 1}, /* Latin Small Letter E */
	{0x0066, 0x0066, 1}, /* Latin Small Letter F */
	{0x0067, 0x0067, 1}, /* Latin Small Letter G */
	{0x0068, 0x0068, 1}, /* Latin Small Letter H */
	{0x0069, 0x0069, 1}, /* Latin Small Letter I */
	{0x006A, 0x006A, 1}, /* LATIN SMALL LETTER J */
	{0x006B, 0x006B, 1}, /* Latin Small Letter K */
	{0x006C, 0x006C, 1}, /* Latin Small Letter L */
	{0x006D, 0x006D, 1}, /* Latin Small Letter M */
	{0x006E, 0x006E, 1}, /* Latin Small Letter N */
	{0x006F, 0x006F, 1}, /* Latin Small Letter O */
	{0x0070, 0x0070, 1}, /* Latin Small Letter P */
	{0x0071, 0x0071, 1}, /* Latin Small Letter Q */
	{0x0072, 0x0072, 1}, /* Latin Small Letter R */
	{0x0073, 0x0073, 1}, /* Latin Small Letter S */
	{0x0074, 0x0074, 1}, /* Latin Small Letter T */
	{0x0075, 0x0075, 1}, /* Latin Small Letter U */
	{0x0076, 0x0076, 1}, /* Latin Small Letter V */
	{0x0077, 0x0077, 1}, /* Latin Small Letter W */
	{0x0078, 0x0078, 1}, /* Latin Small Letter X */
	{0x0079, 0x0079, 1}, /* Latin Small Letter Y */
	{0x007A, 0x007A, 1}, /* Latin Small Letter Z */
	{0x00E4, 0x00E4, 1}, /* LATIN SMALL LETTER A WITH DIAERESIS */
	{0x00F6, 0x00F6, 1}, /* LATIN SMALL LETTER O WITH DIAERESIS */
	{0x00F1, 0x00F1, 1}, /* LATIN SMALL LETTER N WITH TILDE */
	{0x00FC, 0x00FC, 1}, /* LATIN SMALL LETTER U WITH DIAERESIS */
	{0x00E0, 0x00E0, 1}, /* LATIN SMALL LETTER A WITH GRAVE */
	{0xFFFF, 0xFFFF, 0}
	};


/**
 *  Standard 7bit ASCII text that is in the GSM shift table.
 */
const TMappedChar  gStandard7bitGSMShiftChars[] =
	{
	{0x005E, 0x005E, 2}, /* CIRCUMFLEX ACCENT */
	{0x007B, 0x007B, 2}, /* LEFT CURLY BRACKET */
	{0x007D, 0x007D, 2}, /* RIGHT CURLY BRACKET */
	{0x005C, 0x005C, 2}, /* REVERSE SOLIDUS */
	{0x005B, 0x005B, 2}, /* LEFT SQUARE BRACKET */
	{0x007E, 0x007E, 2}, /* Tilde */
	{0x005D, 0x005D, 2}, /* RIGHT SQUARE BRACKET */
	{0x007C, 0x007C, 2}, /* VERTICAL LINE */
	{0x20AC, 0x20AC, 2}, /* EURO SIGN */
	{0xFFFF, 0xFFFF, 0}
	};


/**
 *  Standard 8bit ASCII text used for GSM (Code Page 1252).
 */
const TMappedChar  gStandard8bitGSMChars[] =
	{
	{0x0000, 0x0000, 1}, /* NUL */
	{0x0001, 0x0001, 1}, /* SOH */
	{0x0002, 0x0002, 1}, /* STX */
	{0x0003, 0x0003, 1}, /* ETX */
	{0x0004, 0x0004, 1}, /* EOT */
	{0x0005, 0x0005, 1}, /* ENQ */
	{0x0006, 0x0006, 1}, /* ACK */
	{0x0007, 0x0007, 1}, /* BEL */
	{0x0008, 0x0008, 1}, /* BS */
	{0x0009, 0x0009, 1}, /* CHARACTER TABULATION */
	{0x000A, 0x000A, 1}, /* LINE FEED (LF) */
	{0x000B, 0x000B, 1}, /* VT */
	{0x000C, 0x000C, 1}, /* FORM FEED (FF) */
	{0x000D, 0x000D, 1}, /* CARRIDGE RETURN (CR) */
	{0x000E, 0x000E, 1}, /* SO */
	{0x000F, 0x000F, 1}, /* SI */
	{0x0010, 0x0010, 1}, /* DLE */
	{0x0011, 0x0011, 1}, /* DC1 */
	{0x0012, 0x0012, 1}, /* DC2 */
	{0x0013, 0x0013, 1}, /* DC3 */
	{0x0014, 0x0014, 1}, /* DC4 */
	{0x0015, 0x0015, 1}, /* NAK */
	{0x0016, 0x0016, 1}, /* SYN */
	{0x0017, 0x0017, 1}, /* ETB */
	{0x0018, 0x0018, 1}, /* CAN */
	{0x0019, 0x0019, 1}, /* EM */
	{0x001A, 0x001A, 1}, /* SUB */
	{0x001B, 0x001B, 1}, /* ESC */
	{0x001C, 0x001C, 1}, /* FS */
	{0x001D, 0x001D, 1}, /* GS */
	{0x001E, 0x001E, 1}, /* RS */
	{0x001F, 0x001F, 1}, /* US */
	{0x0020, 0x0020, 1}, /* SPACE */
	{0x0021, 0x0021, 1}, /* EXCLAMATION MARK */
	{0x0022, 0x0022, 1}, /* QUOTATION MARK */
	{0x0023, 0x0023, 1}, /* NUMBER SIGN */
	{0x0024, 0x0024, 1}, /* DOLLAR SIGN */
	{0x0025, 0x0025, 1}, /* PERCENT SIGN */
	{0x0026, 0x0026, 1}, /* AMPERSAND */
	{0x0027, 0x0027, 1}, /* APOSTROPHE */
	{0x0028, 0x0028, 1}, /* LEFT PARENTHESIS */
	{0x0029, 0x0029, 1}, /* RIGHT PARENTHESIS */
	{0x002A, 0x002A, 1}, /* ASTERISK */
	{0x002B, 0x002B, 1}, /* PLUS SIGN */
	{0x002C, 0x002C, 1}, /* COMMA */
	{0x002D, 0x002D, 1}, /* Hyphen/Minus sign */
	{0x002E, 0x002E, 1}, /* FULL STOP */
	{0x002F, 0x002F, 1}, /* SOLIDUS */
	{0x0030, 0x0030, 1}, /* Digit Zero */
	{0x0031, 0x0031, 1}, /* Digit One */
	{0x0032, 0x0032, 1}, /* Digit Two */
	{0x0033, 0x0033, 1}, /* Digit Three */
	{0x0034, 0x0034, 1}, /* Digit Four */
	{0x0035, 0x0035, 1}, /* Digit Five */
	{0x0036, 0x0036, 1}, /* Digit Six */
	{0x0037, 0x0037, 1}, /* Digit Seven */
	{0x0038, 0x0038, 1}, /* Digit Eight */
	{0x0039, 0x0039, 1}, /* Digit Nine */
	{0x003A, 0x003A, 1}, /* COLON */
	{0x003B, 0x003B, 1}, /* SEMICOLON */
	{0x003C, 0x003C, 1}, /* LESS-THAN SIGN */
	{0x003D, 0x003D, 1}, /* EQUALS SIGN */
	{0x003E, 0x003E, 1}, /* GREATER-THAN SIGN */
	{0x003F, 0x003F, 1}, /* QUESTION MARK */
	{0x0040, 0x0040, 1}, /* COMMERCIAL AT */
	{0x0041, 0x0041, 1}, /* Latin Capital letter A */
	{0x0042, 0x0042, 1}, /* Latin Capital letter B */
	{0x0043, 0x0043, 1}, /* Latin Capital letter C */
	{0x0044, 0x0044, 1}, /* Latin Capital letter D */
	{0x0045, 0x0045, 1}, /* Latin Capital letter E */
	{0x0046, 0x0046, 1}, /* Latin Capital letter F */
	{0x0047, 0x0047, 1}, /* Latin Capital letter G */
	{0x0048, 0x0048, 1}, /* Latin Capital letter H */
	{0x0049, 0x0049, 1}, /* Latin Capital letter I */
	{0x004A, 0x004A, 1}, /* Latin Capital letter J */
	{0x004B, 0x004B, 1}, /* Latin Capital letter K */
	{0x004C, 0x004C, 1}, /* Latin Capital letter L */
	{0x004D, 0x004D, 1}, /* Latin Capital letter M */
	{0x004E, 0x004E, 1}, /* Latin Capital letter N */
	{0x004F, 0x004F, 1}, /* Latin Capital letter O */
	{0x0050, 0x0050, 1}, /* Latin Capital letter P */
	{0x0051, 0x0051, 1}, /* Latin Capital letter Q */
	{0x0052, 0x0052, 1}, /* Latin Capital letter R */
	{0x0053, 0x0053, 1}, /* Latin Capital letter S */
	{0x0054, 0x0054, 1}, /* Latin Capital letter T */
	{0x0055, 0x0055, 1}, /* Latin Capital letter U */
	{0x0056, 0x0056, 1}, /* Latin Capital letter V */
	{0x0057, 0x0057, 1}, /* Latin Capital letter W */
	{0x0058, 0x0058, 1}, /* Latin Capital letter X */
	{0x0059, 0x0059, 1}, /* Latin Capital letter Y */
	{0x005A, 0x005A, 1}, /* Latin Capital letter Z */
	{0x005B, 0x005B, 1}, /* LEFT SQUARE BRACKET */
	{0x005C, 0x005C, 1}, /* REVERSE SOLIDUS */
	{0x005D, 0x005D, 1}, /* RIGHT SQUARE BRACKET */
	{0x005E, 0x005E, 1}, /* CIRCUMFLEX ACCENT */
	{0x005F, 0x005F, 1}, /* LOW LINE */
	{0x0060, 0x0060, 1}, /* GRAVE ACCENT */
	{0x0061, 0x0061, 1}, /* Latin Small Letter A */
	{0x0062, 0x0062, 1}, /* Latin Small Letter B */
	{0x0063, 0x0063, 1}, /* Latin Small Letter C */
	{0x0064, 0x0064, 1}, /* Latin Small Letter D */
	{0x0065, 0x0065, 1}, /* Latin Small Letter E */
	{0x0066, 0x0066, 1}, /* Latin Small Letter F */
	{0x0067, 0x0067, 1}, /* Latin Small Letter G */
	{0x0068, 0x0068, 1}, /* Latin Small Letter H */
	{0x0069, 0x0069, 1}, /* Latin Small Letter I */
	{0x006A, 0x006A, 1}, /* LATIN SMALL LETTER J */
	{0x006B, 0x006B, 1}, /* Latin Small Letter K */
	{0x006C, 0x006C, 1}, /* Latin Small Letter L */
	{0x006D, 0x006D, 1}, /* Latin Small Letter M */
	{0x006E, 0x006E, 1}, /* Latin Small Letter N */
	{0x006F, 0x006F, 1}, /* Latin Small Letter O */
	{0x0070, 0x0070, 1}, /* Latin Small Letter P */
	{0x0071, 0x0071, 1}, /* Latin Small Letter Q */
	{0x0072, 0x0072, 1}, /* Latin Small Letter R */
	{0x0073, 0x0073, 1}, /* Latin Small Letter S */
	{0x0074, 0x0074, 1}, /* Latin Small Letter T */
	{0x0075, 0x0075, 1}, /* Latin Small Letter U */
	{0x0076, 0x0076, 1}, /* Latin Small Letter V */
	{0x0077, 0x0077, 1}, /* Latin Small Letter W */
	{0x0078, 0x0078, 1}, /* Latin Small Letter X */
	{0x0079, 0x0079, 1}, /* Latin Small Letter Y */
	{0x007A, 0x007A, 1}, /* Latin Small Letter Z */
	{0x007B, 0x007B, 1}, /* LEFT CURLY BRACKET */
	{0x007C, 0x007C, 1}, /* VERTICAL LINE */
	{0x007D, 0x007D, 1}, /* RIGHT CURLY BRACKET */
	{0x007E, 0x007E, 1}, /* Tilde */
	{0x007F, 0x007F, 1}, /* DEL */

	//
	// Range of 128 to 159 are not supported by the 1252 CharConv but it does
	// have a 1-to-1 mapping there. However such characters are marked as
	// unconvertible.
	//
	//{0x20AC, 0x20AC, 1}, /* EURO SIGN */
	// 129 not used
	//{0x201A, 0x201A, 1}, /* SINGLE LOW-9 QUOTATION MARK */
	//{0x0192, 0x0192, 1}, /* LATIN SMALL LETTER F WITH HOOK */
	//{0x201E, 0x201E, 1}, /* DOUBLE LOW-9 QUOTATION MARK */
	//{0x2026, 0x2026, 1}, /* HORIZONTAL ELLIPSIS */
	//{0x2020, 0x2020, 1}, /* DAGGER */
	//{0x2021, 0x2021, 1}, /* DOUBLE DAGGER */
	//{0x02C6, 0x02C6, 1}, /* MODIFIER LETTER CIRCUMFLEX ACCENT */
	//{0x2030, 0x2030, 1}, /* PER MILLE SIGN */
	//{0x0160, 0x0160, 1}, /* LATIN CAPITAL LETTER S WITH CARON */
	//{0x2039, 0x2039, 1}, /* SINGLE LEFT-POINTING ANGLE QUOTATION MARK */
	//{0x0152, 0x0152, 1}, /* LATIN CAPITAL LIGATURE OE */
	// 141 not used
	//{0x017D, 0x017D, 1}, /* LATIN CAPITAL LETTER Z WITH CARON */
	// 143 not used
	// 144 not used
	//{0x2018, 0x2018, 1}, /* LEFT SINGLE QUOTATION MARK */
	//{0x2019, 0x2019, 1}, /* RIGHT SINGLE QUOTATION MARK */
	//{0x201C, 0x201C, 1}, /* LEFT DOUBLE QUOTATION MARK */
	//{0x201D, 0x201D, 1}, /* RIGHT DOUBLE QUOTATION MARK */
	//{0x2022, 0x2022, 1}, /* BULLET */
	//{0x2013, 0x2013, 1}, /* EN DASH */
	//{0x2014, 0x2014, 1}, /* EM DASH */
	//{0x02DC, 0x02DC, 1}, /* SMALL TILDE */
	//{0x2122, 0x2122, 1}, /* TRADE MARK SIGN */
	//{0x0161, 0x0161, 1}, /* LATIN SMALL LETTER S WITH CARON */
	//{0x203A, 0x203A, 1}, /* SINGLE RIGHT-POINTING ANGLE QUOTATION MARK */
	//{0x0153, 0x0153, 1}, /* LATIN SMALL LIGATURE OE */
	// 157 not used
	//{0x017E, 0x017E, 1}, /* LATIN SMALL LETTER Z WITH CARON */
	//{0x0178, 0x0178, 1}, /* LATIN CAPITAL LETTER Y WITH DIAERESIS */
	
	{0x00A0, 0x00A0, 1}, /* NO-BREAK SPACE */
	{0x00A1, 0x00A1, 1}, /* INVERTED EXCLAMATION MARK */
	{0x00A2, 0x00A2, 1}, /* CENT SIGN */
	{0x00A3, 0x00A3, 1}, /* POUND SIGN */
	{0x00A4, 0x00A4, 1}, /* CURRENCY SIGN */
	{0x00A5, 0x00A5, 1}, /* YEN SIGN */
	{0x00A6, 0x00A6, 1}, /* BROKEN BAR */
	{0x00A7, 0x00A7, 1}, /* SECTION SIGN */
	{0x00A8, 0x00A8, 1}, /* DIAERESIS */
	{0x00A9, 0x00A9, 1}, /* COPYRIGHT SIGN */
	{0x00AA, 0x00AA, 1}, /* FEMININE ORDINAL INDICATOR */
	{0x00AB, 0x00AB, 1}, /* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{0x00AC, 0x00AC, 1}, /* NOT SIGN */
	{0x00AD, 0x00AD, 1}, /* SOFT HYPHEN */
	{0x00AE, 0x00AE, 1}, /* REGISTERED SIGN */
	{0x00AF, 0x00AF, 1}, /* MACRON */
	{0x00B0, 0x00B0, 1}, /* DEGREE SIGN */
	{0x00B1, 0x00B1, 1}, /* PLUS-MINUS SIGN */
	{0x00B2, 0x00B2, 1}, /* SUPERSCRIPT TWO */
	{0x00B3, 0x00B3, 1}, /* SUPERSCRIPT THREE */
	{0x00B4, 0x00B4, 1}, /* ACUTE ACCENT */
	{0x00B5, 0x00B5, 1}, /* MICRO SIGN */
	{0x00B6, 0x00B6, 1}, /* PILCROW SIGN */
	{0x00B7, 0x00B7, 1}, /* MIDDLE DOT */
	{0x00B8, 0x00B8, 1}, /* CEDILLA */
	{0x00B9, 0x00B9, 1}, /* SUPERSCRIPT ONE */
	{0x00BA, 0x00BA, 1}, /* MASCULINE ORDINAL INDICATOR */
	{0x00BB, 0x00BB, 1}, /* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{0x00BC, 0x00BC, 1}, /* VULGAR FRACTION ONE QUARTER */
	{0x00BD, 0x00BD, 1}, /* VULGAR FRACTION ONE HALF */
	{0x00BE, 0x00BE, 1}, /* VULGAR FRACTION THREE QUARTERS */
	{0x00BF, 0x00BF, 1}, /* INVERTED QUESTION MARK */
	{0x00C0, 0x00C0, 1}, /* LATIN CAPITAL LETTER A WITH GRAVE */
	{0x00C1, 0x00C1, 1}, /* LATIN CAPITAL LETTER A WITH ACUTE */
	{0x00C2, 0x00C2, 1}, /* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
	{0x00C3, 0x00C3, 1}, /* LATIN CAPITAL LETTER A WITH TILDE */
	{0x00C4, 0x00C4, 1}, /* LATIN CAPITAL LETTER A WITH DIAERESIS */
	{0x00C5, 0x00C5, 1}, /* LATIN CAPITAL LETTER A WITH RING ABOVE */
	{0x00C6, 0x00C6, 1}, /* LATIN CAPITAL LETTER AE */
	{0x00C7, 0x00C7, 1}, /* LATIN CAPITAL LETTER C WITH CEDILLA */
	{0x00C8, 0x00C8, 1}, /* LATIN CAPITAL LETTER E WITH GRAVE */
	{0x00C9, 0x00C9, 1}, /* LATIN CAPITAL LETTER E WITH ACUTE */
	{0x00CA, 0x00CA, 1}, /* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
	{0x00CB, 0x00CB, 1}, /* LATIN CAPITAL LETTER E WITH DIAERESIS */
	{0x00CC, 0x00CC, 1}, /* LATIN CAPITAL LETTER I WITH GRAVE */
	{0x00CD, 0x00CD, 1}, /* LATIN CAPITAL LETTER I WITH ACUTE */
	{0x00CE, 0x00CE, 1}, /* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
	{0x00CF, 0x00CF, 1}, /* LATIN CAPITAL LETTER I WITH DIAERESIS */
	{0x00D0, 0x00D0, 1}, /* LATIN CAPITAL LETTER ETH */
	{0x00D1, 0x00D1, 1}, /* LATIN CAPITAL LETTER N WITH TILDE */
	{0x00D2, 0x00D2, 1}, /* LATIN CAPITAL LETTER O WITH GRAVE */
	{0x00D3, 0x00D3, 1}, /* LATIN CAPITAL LETTER O WITH ACUTE */
	{0x00D4, 0x00D4, 1}, /* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
	{0x00D5, 0x00D5, 1}, /* LATIN CAPITAL LETTER O WITH TILDE */
	{0x00D6, 0x00D6, 1}, /* LATIN CAPITAL LETTER O WITH DIAERESIS */
	{0x00D7, 0x00D7, 1}, /* MULTIPLICATION SIGN */
	{0x00D8, 0x00D8, 1}, /* LATIN CAPITAL LETTER O WITH STROKE */
	{0x00D9, 0x00D9, 1}, /* LATIN CAPITAL LETTER U WITH GRAVE */
	{0x00DA, 0x00DA, 1}, /* LATIN CAPITAL LETTER U WITH ACUTE */
	{0x00DB, 0x00DB, 1}, /* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
	{0x00DC, 0x00DC, 1}, /* LATIN CAPITAL LETTER U WITH DIAERESIS */
	{0x00DD, 0x00DD, 1}, /* LATIN CAPITAL LETTER Y WITH ACUTE */
	{0x00DE, 0x00DE, 1}, /* LATIN CAPITAL LETTER THORN */
	{0x00DF, 0x00DF, 1}, /* LATIN SMALL LETTER SHARP S */
	{0x00E0, 0x00E0, 1}, /* LATIN SMALL LETTER A WITH GRAVE */
	{0x00E1, 0x00E1, 1}, /* LATIN SMALL LETTER A WITH ACUTE */
	{0x00E2, 0x00E2, 1}, /* LATIN SMALL LETTER A WITH CIRCUMFLEX */
	{0x00E3, 0x00E3, 1}, /* LATIN SMALL LETTER A WITH TILDE */
	{0x00E4, 0x00E4, 1}, /* LATIN SMALL LETTER A WITH DIAERESIS */
	{0x00E5, 0x00E5, 1}, /* LATIN SMALL LETTER A WITH RING ABOVE */
	{0x00E6, 0x00E6, 1}, /* LATIN SMALL LETTER AE */
	{0x00E7, 0x00E7, 1}, /* LATIN SMALL LETTER C WITH CEDILLA */
	{0x00E8, 0x00E8, 1}, /* LATIN SMALL LETTER E WITH GRAVE */
	{0x00E9, 0x00E9, 1}, /* LATIN SMALL LETTER E WITH ACUTE */
	{0x00EA, 0x00EA, 1}, /* LATIN SMALL LETTER E WITH CIRCUMFLEX */
	{0x00EB, 0x00EB, 1}, /* LATIN SMALL LETTER E WITH DIAERESIS */
	{0x00EC, 0x00EC, 1}, /* LATIN SMALL LETTER I WITH GRAVE */
	{0x00ED, 0x00ED, 1}, /* LATIN SMALL LETTER I WITH ACUTE */
	{0x00EE, 0x00EE, 1}, /* LATIN SMALL LETTER I WITH CIRCUMFLEX */
	{0x00EF, 0x00EF, 1}, /* LATIN SMALL LETTER I WITH DIAERESIS */
	{0x00F0, 0x00F0, 1}, /* LATIN SMALL LETTER ETH */
	{0x00F1, 0x00F1, 1}, /* LATIN SMALL LETTER N WITH TILDE */
	{0x00F2, 0x00F2, 1}, /* LATIN SMALL LETTER O WITH GRAVE */
	{0x00F3, 0x00F3, 1}, /* LATIN SMALL LETTER O WITH ACUTE */
	{0x00F4, 0x00F4, 1}, /* LATIN SMALL LETTER O WITH CIRCUMFLEX */
	{0x00F5, 0x00F5, 1}, /* LATIN SMALL LETTER O WITH TILDE */
	{0x00F6, 0x00F6, 1}, /* LATIN SMALL LETTER O WITH DIAERESIS */
	{0x00F7, 0x00F7, 1}, /* DIVISION SIGN */
	{0x00F8, 0x00F8, 1}, /* LATIN SMALL LETTER O WITH STROKE */
	{0x00F9, 0x00F9, 1}, /* LATIN SMALL LETTER U WITH GRAVE */
	{0x00FA, 0x00FA, 1}, /* LATIN SMALL LETTER U WITH ACUTE */
	{0x00FB, 0x00FB, 1}, /* LATIN SMALL LETTER U WITH CIRCUMFLEX */
	{0x00FC, 0x00FC, 1}, /* LATIN SMALL LETTER U WITH DIAERESIS */
	{0x00FD, 0x00FD, 1}, /* LATIN SMALL LETTER Y WITH ACUTE */
	{0x00FE, 0x00FE, 1}, /* LATIN SMALL LETTER THORN */
	{0x00FF, 0x00FF, 1}, /* LATIN SMALL LETTER Y WITH DIAERESIS */
	{0xFFFF, 0xFFFF}
	};


/**
 *  Various UCS2 characters (obviously not the complete set).
 */
const TMappedChar  gVariousUCS2Chars[] =
	{
	{0x0000, 0x0000, 2}, /* NUL */
	{0x0001, 0x0001, 2}, /* SOH */
	{0x0002, 0x0002, 2}, /* STX */
	{0x0003, 0x0003, 2}, /* ETX */
	{0x0004, 0x0004, 2}, /* EOT */
	{0x0005, 0x0005, 2}, /* ENQ */
	{0x0006, 0x0006, 2}, /* ACK */
	{0x0007, 0x0007, 2}, /* BEL */
	{0x0008, 0x0008, 2}, /* BS */
	{0x0009, 0x0009, 2}, /* CHARACTER TABULATION */
	{0x000A, 0x000A, 2}, /* LINE FEED (LF) */
	{0x000B, 0x000B, 2}, /* VT */
	{0x000C, 0x000C, 2}, /* FORM FEED (FF) */
	{0x000D, 0x000D, 2}, /* CARRIDGE RETURN (CR) */
	{0x000E, 0x000E, 2}, /* SO */
	{0x000F, 0x000F, 2}, /* SI */
	{0x0010, 0x0010, 2}, /* DLE */
	{0x0011, 0x0011, 2}, /* DC1 */
	{0x0012, 0x0012, 2}, /* DC2 */
	{0x0013, 0x0013, 2}, /* DC3 */
	{0x0014, 0x0014, 2}, /* DC4 */
	{0x0015, 0x0015, 2}, /* NAK */
	{0x0016, 0x0016, 2}, /* SYN */
	{0x0017, 0x0017, 2}, /* ETB */
	{0x0018, 0x0018, 2}, /* CAN */
	{0x0019, 0x0019, 2}, /* EM */
	{0x001A, 0x001A, 2}, /* SUB */
	{0x001B, 0x001B, 2}, /* ESC */
	{0x001C, 0x001C, 2}, /* FS */
	{0x001D, 0x001D, 2}, /* GS */
	{0x001E, 0x001E, 2}, /* RS */
	{0x001F, 0x001F, 2}, /* US */
	{0x0020, 0x0020, 2}, /* SPACE */
	{0x0021, 0x0021, 2}, /* EXCLAMATION MARK */
	{0x0022, 0x0022, 2}, /* QUOTATION MARK */
	{0x0023, 0x0023, 2}, /* NUMBER SIGN */
	{0x0024, 0x0024, 2}, /* DOLLAR SIGN */
	{0x0025, 0x0025, 2}, /* PERCENT SIGN */
	{0x0026, 0x0026, 2}, /* AMPERSAND */
	{0x0027, 0x0027, 2}, /* APOSTROPHE */
	{0x0028, 0x0028, 2}, /* LEFT PARENTHESIS */
	{0x0029, 0x0029, 2}, /* RIGHT PARENTHESIS */
	{0x002A, 0x002A, 2}, /* ASTERISK */
	{0x002B, 0x002B, 2}, /* PLUS SIGN */
	{0x002C, 0x002C, 2}, /* COMMA */
	{0x002D, 0x002D, 2}, /* Hyphen/Minus sign */
	{0x002E, 0x002E, 2}, /* FULL STOP */
	{0x002F, 0x002F, 2}, /* SOLIDUS */
	{0x0030, 0x0030, 2}, /* Digit Zero */
	{0x0031, 0x0031, 2}, /* Digit One */
	{0x0032, 0x0032, 2}, /* Digit Two */
	{0x0033, 0x0033, 2}, /* Digit Three */
	{0x0034, 0x0034, 2}, /* Digit Four */
	{0x0035, 0x0035, 2}, /* Digit Five */
	{0x0036, 0x0036, 2}, /* Digit Six */
	{0x0037, 0x0037, 2}, /* Digit Seven */
	{0x0038, 0x0038, 2}, /* Digit Eight */
	{0x0039, 0x0039, 2}, /* Digit Nine */
	{0x003A, 0x003A, 2}, /* COLON */
	{0x003B, 0x003B, 2}, /* SEMICOLON */
	{0x003C, 0x003C, 2}, /* LESS-THAN SIGN */
	{0x003D, 0x003D, 2}, /* EQUALS SIGN */
	{0x003E, 0x003E, 2}, /* GREATER-THAN SIGN */
	{0x003F, 0x003F, 2}, /* QUESTION MARK */
	{0x0040, 0x0040, 2}, /* COMMERCIAL AT */
	{0x0041, 0x0041, 2}, /* Latin Capital letter A */
	{0x0042, 0x0042, 2}, /* Latin Capital letter B */
	{0x0043, 0x0043, 2}, /* Latin Capital letter C */
	{0x0044, 0x0044, 2}, /* Latin Capital letter D */
	{0x0045, 0x0045, 2}, /* Latin Capital letter E */
	{0x0046, 0x0046, 2}, /* Latin Capital letter F */
	{0x0047, 0x0047, 2}, /* Latin Capital letter G */
	{0x0048, 0x0048, 2}, /* Latin Capital letter H */
	{0x0049, 0x0049, 2}, /* Latin Capital letter I */
	{0x004A, 0x004A, 2}, /* Latin Capital letter J */
	{0x004B, 0x004B, 2}, /* Latin Capital letter K */
	{0x004C, 0x004C, 2}, /* Latin Capital letter L */
	{0x004D, 0x004D, 2}, /* Latin Capital letter M */
	{0x004E, 0x004E, 2}, /* Latin Capital letter N */
	{0x004F, 0x004F, 2}, /* Latin Capital letter O */
	{0x0050, 0x0050, 2}, /* Latin Capital letter P */
	{0x0051, 0x0051, 2}, /* Latin Capital letter Q */
	{0x0052, 0x0052, 2}, /* Latin Capital letter R */
	{0x0053, 0x0053, 2}, /* Latin Capital letter S */
	{0x0054, 0x0054, 2}, /* Latin Capital letter T */
	{0x0055, 0x0055, 2}, /* Latin Capital letter U */
	{0x0056, 0x0056, 2}, /* Latin Capital letter V */
	{0x0057, 0x0057, 2}, /* Latin Capital letter W */
	{0x0058, 0x0058, 2}, /* Latin Capital letter X */
	{0x0059, 0x0059, 2}, /* Latin Capital letter Y */
	{0x005A, 0x005A, 2}, /* Latin Capital letter Z */
	{0x005B, 0x005B, 2}, /* LEFT SQUARE BRACKET */
	{0x005C, 0x005C, 2}, /* REVERSE SOLIDUS */
	{0x005D, 0x005D, 2}, /* RIGHT SQUARE BRACKET */
	{0x005E, 0x005E, 2}, /* CIRCUMFLEX ACCENT */
	{0x005F, 0x005F, 2}, /* LOW LINE */
	{0x0060, 0x0060, 2}, /* GRAVE ACCENT */
	{0x0061, 0x0061, 2}, /* Latin Small Letter A */
	{0x0062, 0x0062, 2}, /* Latin Small Letter B */
	{0x0063, 0x0063, 2}, /* Latin Small Letter C */
	{0x0064, 0x0064, 2}, /* Latin Small Letter D */
	{0x0065, 0x0065, 2}, /* Latin Small Letter E */
	{0x0066, 0x0066, 2}, /* Latin Small Letter F */
	{0x0067, 0x0067, 2}, /* Latin Small Letter G */
	{0x0068, 0x0068, 2}, /* Latin Small Letter H */
	{0x0069, 0x0069, 2}, /* Latin Small Letter I */
	{0x006A, 0x006A, 2}, /* LATIN SMALL LETTER J */
	{0x006B, 0x006B, 2}, /* Latin Small Letter K */
	{0x006C, 0x006C, 2}, /* Latin Small Letter L */
	{0x006D, 0x006D, 2}, /* Latin Small Letter M */
	{0x006E, 0x006E, 2}, /* Latin Small Letter N */
	{0x006F, 0x006F, 2}, /* Latin Small Letter O */
	{0x0070, 0x0070, 2}, /* Latin Small Letter P */
	{0x0071, 0x0071, 2}, /* Latin Small Letter Q */
	{0x0072, 0x0072, 2}, /* Latin Small Letter R */
	{0x0073, 0x0073, 2}, /* Latin Small Letter S */
	{0x0074, 0x0074, 2}, /* Latin Small Letter T */
	{0x0075, 0x0075, 2}, /* Latin Small Letter U */
	{0x0076, 0x0076, 2}, /* Latin Small Letter V */
	{0x0077, 0x0077, 2}, /* Latin Small Letter W */
	{0x0078, 0x0078, 2}, /* Latin Small Letter X */
	{0x0079, 0x0079, 2}, /* Latin Small Letter Y */
	{0x007A, 0x007A, 2}, /* Latin Small Letter Z */
	{0x007B, 0x007B, 2}, /* LEFT CURLY BRACKET */
	{0x007C, 0x007C, 2}, /* VERTICAL LINE */
	{0x007D, 0x007D, 2}, /* RIGHT CURLY BRACKET */
	{0x007E, 0x007E, 2}, /* Tilde */
	{0x007F, 0x007F, 2}, /* DEL */
	{0x00A0, 0x00A0, 2}, /* NO-BREAK SPACE */
	{0x00A1, 0x00A1, 2}, /* INVERTED EXCLAMATION MARK */
	{0x00A2, 0x00A2, 2}, /* CENT SIGN */
	{0x00A3, 0x00A3, 2}, /* POUND SIGN */
	{0x00A4, 0x00A4, 2}, /* CURRENCY SIGN */
	{0x00A5, 0x00A5, 2}, /* YEN SIGN */
	{0x00A6, 0x00A6, 2}, /* BROKEN BAR */
	{0x00A7, 0x00A7, 2}, /* SECTION SIGN */
	{0x00A8, 0x00A8, 2}, /* DIAERESIS */
	{0x00A9, 0x00A9, 2}, /* COPYRIGHT SIGN */
	{0x00AA, 0x00AA, 2}, /* FEMININE ORDINAL INDICATOR */
	{0x00AB, 0x00AB, 2}, /* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{0x00AC, 0x00AC, 2}, /* NOT SIGN */
	{0x00AD, 0x00AD, 2}, /* SOFT HYPHEN */
	{0x00AE, 0x00AE, 2}, /* REGISTERED SIGN */
	{0x00AF, 0x00AF, 2}, /* MACRON */
	{0x00B0, 0x00B0, 2}, /* DEGREE SIGN */
	{0x00B1, 0x00B1, 2}, /* PLUS-MINUS SIGN */
	{0x00B2, 0x00B2, 2}, /* SUPERSCRIPT TWO */
	{0x00B3, 0x00B3, 2}, /* SUPERSCRIPT THREE */
	{0x00B4, 0x00B4, 2}, /* ACUTE ACCENT */
	{0x00B5, 0x00B5, 2}, /* MICRO SIGN */
	{0x00B6, 0x00B6, 2}, /* PILCROW SIGN */
	{0x00B7, 0x00B7, 2}, /* MIDDLE DOT */
	{0x00B8, 0x00B8, 2}, /* CEDILLA */
	{0x00B9, 0x00B9, 2}, /* SUPERSCRIPT ONE */
	{0x00BA, 0x00BA, 2}, /* MASCULINE ORDINAL INDICATOR */
	{0x00BB, 0x00BB, 2}, /* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{0x00BC, 0x00BC, 2}, /* VULGAR FRACTION ONE QUARTER */
	{0x00BD, 0x00BD, 2}, /* VULGAR FRACTION ONE HALF */
	{0x00BE, 0x00BE, 2}, /* VULGAR FRACTION THREE QUARTERS */
	{0x00BF, 0x00BF, 2}, /* INVERTED QUESTION MARK */
	{0x00C0, 0x00C0, 2}, /* LATIN CAPITAL LETTER A WITH GRAVE */
	{0x00C1, 0x00C1, 2}, /* LATIN CAPITAL LETTER A WITH ACUTE */
	{0x00C2, 0x00C2, 2}, /* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
	{0x00C3, 0x00C3, 2}, /* LATIN CAPITAL LETTER A WITH TILDE */
	{0x00C4, 0x00C4, 2}, /* LATIN CAPITAL LETTER A WITH DIAERESIS */
	{0x00C5, 0x00C5, 2}, /* LATIN CAPITAL LETTER A WITH RING ABOVE */
	{0x00C6, 0x00C6, 2}, /* LATIN CAPITAL LETTER AE */
	{0x00C7, 0x00C7, 2}, /* LATIN CAPITAL LETTER C WITH CEDILLA */
	{0x00C8, 0x00C8, 2}, /* LATIN CAPITAL LETTER E WITH GRAVE */
	{0x00C9, 0x00C9, 2}, /* LATIN CAPITAL LETTER E WITH ACUTE */
	{0x00CA, 0x00CA, 2}, /* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
	{0x00CB, 0x00CB, 2}, /* LATIN CAPITAL LETTER E WITH DIAERESIS */
	{0x00CC, 0x00CC, 2}, /* LATIN CAPITAL LETTER I WITH GRAVE */
	{0x00CD, 0x00CD, 2}, /* LATIN CAPITAL LETTER I WITH ACUTE */
	{0x00CE, 0x00CE, 2}, /* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
	{0x00CF, 0x00CF, 2}, /* LATIN CAPITAL LETTER I WITH DIAERESIS */
	{0x00D0, 0x00D0, 2}, /* LATIN CAPITAL LETTER ETH */
	{0x00D1, 0x00D1, 2}, /* LATIN CAPITAL LETTER N WITH TILDE */
	{0x00D2, 0x00D2, 2}, /* LATIN CAPITAL LETTER O WITH GRAVE */
	{0x00D3, 0x00D3, 2}, /* LATIN CAPITAL LETTER O WITH ACUTE */
	{0x00D4, 0x00D4, 2}, /* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
	{0x00D5, 0x00D5, 2}, /* LATIN CAPITAL LETTER O WITH TILDE */
	{0x00D6, 0x00D6, 2}, /* LATIN CAPITAL LETTER O WITH DIAERESIS */
	{0x00D7, 0x00D7, 2}, /* MULTIPLICATION SIGN */
	{0x00D8, 0x00D8, 2}, /* LATIN CAPITAL LETTER O WITH STROKE */
	{0x00D9, 0x00D9, 2}, /* LATIN CAPITAL LETTER U WITH GRAVE */
	{0x00DA, 0x00DA, 2}, /* LATIN CAPITAL LETTER U WITH ACUTE */
	{0x00DB, 0x00DB, 2}, /* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
	{0x00DC, 0x00DC, 2}, /* LATIN CAPITAL LETTER U WITH DIAERESIS */
	{0x00DD, 0x00DD, 2}, /* LATIN CAPITAL LETTER Y WITH ACUTE */
	{0x00DE, 0x00DE, 2}, /* LATIN CAPITAL LETTER THORN */
	{0x00DF, 0x00DF, 2}, /* LATIN SMALL LETTER SHARP S */
	{0x00E0, 0x00E0, 2}, /* LATIN SMALL LETTER A WITH GRAVE */
	{0x00E1, 0x00E1, 2}, /* LATIN SMALL LETTER A WITH ACUTE */
	{0x00E2, 0x00E2, 2}, /* LATIN SMALL LETTER A WITH CIRCUMFLEX */
	{0x00E3, 0x00E3, 2}, /* LATIN SMALL LETTER A WITH TILDE */
	{0x00E4, 0x00E4, 2}, /* LATIN SMALL LETTER A WITH DIAERESIS */
	{0x00E5, 0x00E5, 2}, /* LATIN SMALL LETTER A WITH RING ABOVE */
	{0x00E6, 0x00E6, 2}, /* LATIN SMALL LETTER AE */
	{0x00E7, 0x00E7, 2}, /* LATIN SMALL LETTER C WITH CEDILLA */
	{0x00E8, 0x00E8, 2}, /* LATIN SMALL LETTER E WITH GRAVE */
	{0x00E9, 0x00E9, 2}, /* LATIN SMALL LETTER E WITH ACUTE */
	{0x00EA, 0x00EA, 2}, /* LATIN SMALL LETTER E WITH CIRCUMFLEX */
	{0x00EB, 0x00EB, 2}, /* LATIN SMALL LETTER E WITH DIAERESIS */
	{0x00EC, 0x00EC, 2}, /* LATIN SMALL LETTER I WITH GRAVE */
	{0x00ED, 0x00ED, 2}, /* LATIN SMALL LETTER I WITH ACUTE */
	{0x00EE, 0x00EE, 2}, /* LATIN SMALL LETTER I WITH CIRCUMFLEX */
	{0x00EF, 0x00EF, 2}, /* LATIN SMALL LETTER I WITH DIAERESIS */
	{0x00F0, 0x00F0, 2}, /* LATIN SMALL LETTER ETH */
	{0x00F1, 0x00F1, 2}, /* LATIN SMALL LETTER N WITH TILDE */
	{0x00F2, 0x00F2, 2}, /* LATIN SMALL LETTER O WITH GRAVE */
	{0x00F3, 0x00F3, 2}, /* LATIN SMALL LETTER O WITH ACUTE */
	{0x00F4, 0x00F4, 2}, /* LATIN SMALL LETTER O WITH CIRCUMFLEX */
	{0x00F5, 0x00F5, 2}, /* LATIN SMALL LETTER O WITH TILDE */
	{0x00F6, 0x00F6, 2}, /* LATIN SMALL LETTER O WITH DIAERESIS */
	{0x00F7, 0x00F7, 2}, /* DIVISION SIGN */
	{0x00F8, 0x00F8, 2}, /* LATIN SMALL LETTER O WITH STROKE */
	{0x00F9, 0x00F9, 2}, /* LATIN SMALL LETTER U WITH GRAVE */
	{0x00FA, 0x00FA, 2}, /* LATIN SMALL LETTER U WITH ACUTE */
	{0x00FB, 0x00FB, 2}, /* LATIN SMALL LETTER U WITH CIRCUMFLEX */
	{0x00FC, 0x00FC, 2}, /* LATIN SMALL LETTER U WITH DIAERESIS */
	{0x00FD, 0x00FD, 2}, /* LATIN SMALL LETTER Y WITH ACUTE */
	{0x00FE, 0x00FE, 2}, /* LATIN SMALL LETTER THORN */
	{0x00FF, 0x00FF, 2}, /* LATIN SMALL LETTER Y WITH DIAERESIS */
	{0x0100, 0x0100, 2}, /* LATIN CAPITAL LETTER A WITH MACRON */
	{0x0101, 0x0101, 2}, /* LATIN SMALL LETTER A WITH MACRON */
	{0x0102, 0x0102, 2}, /* LATIN CAPITAL LETTER A WITH BREVE */
	{0x0103, 0x0103, 2}, /* LATIN SMALL LETTER A WITH BREVE */
	{0x0104, 0x0104, 2}, /* LATIN CAPITAL LETTER A WITH OGONEK */
	{0x0105, 0x0105, 2}, /* LATIN SMALL LETTER A WITH OGONEK */
	{0x0106, 0x0106, 2}, /* LATIN CAPITAL LETTER C WITH ACUTE */
	{0x0107, 0x0107, 2}, /* LATIN SMALL LETTER C WITH ACUTE */
	{0x0108, 0x0108, 2}, /* LATIN CAPITAL LETTER C WITH CIRCUMFLEX */
	{0x0109, 0x0109, 2}, /* LATIN SMALL LETTER C WITH CIRCUMFLEX */
	{0x010A, 0x010A, 2}, /* LATIN CAPITAL LETTER C WITH DOT ABOVE */
	{0x010B, 0x010B, 2}, /* LATIN SMALL LETTER C WITH DOT ABOVE */
	{0x010C, 0x010C, 2}, /* LATIN CAPITAL LETTER C WITH CARON */
	{0x010D, 0x010D, 2}, /* LATIN SMALL LETTER C WITH CARON */
	{0x010E, 0x010E, 2}, /* LATIN CAPITAL LETTER D WITH CARON */
	{0x010F, 0x010F, 2}, /* LATIN SMALL LETTER D WITH CARON */
	{0x0110, 0x0110, 2}, /* LATIN CAPITAL LETTER D WITH STROKE */
	{0x0111, 0x0111, 2}, /* LATIN SMALL LETTER D WITH STROKE */
	{0x0112, 0x0112, 2}, /* LATIN CAPITAL LETTER E WITH MACRON */
	{0x0113, 0x0113, 2}, /* LATIN SMALL LETTER E WITH MACRON */
	{0x0116, 0x0116, 2}, /* LATIN CAPITAL LETTER E WITH DOT ABOVE */
	{0x0117, 0x0117, 2}, /* LATIN SMALL LETTER E WITH DOT ABOVE */
	{0x0118, 0x0118, 2}, /* LATIN CAPITAL LETTER E WITH OGONEK */
	{0x0119, 0x0119, 2}, /* LATIN SMALL LETTER E WITH OGONEK */
	{0x011A, 0x011A, 2}, /* LATIN CAPITAL LETTER E WITH CARON */
	{0x011B, 0x011B, 2}, /* LATIN SMALL LETTER E WITH CARON */
	{0x011C, 0x011C, 2}, /* LATIN CAPITAL LETTER G WITH CIRCUMFLEX */
	{0x011D, 0x011D, 2}, /* LATIN SMALL LETTER G WITH CIRCUMFLEX */
	{0x011E, 0x011E, 2}, /* LATIN CAPITAL LETTER G WITH BREVE */
	{0x011F, 0x011F, 2}, /* LATIN SMALL LETTER G WITH BREVE */
	{0x0120, 0x0120, 2}, /* LATIN CAPITAL LETTER G WITH DOT ABOVE */
	{0x0121, 0x0121, 2}, /* LATIN SMALL LETTER G WITH DOT ABOVE */
	{0x0122, 0x0122, 2}, /* LATIN CAPITAL LETTER G WITH CEDILLA */
	{0x0124, 0x0124, 2}, /* LATIN CAPITAL LETTER H WITH CIRCUMFLEX */
	{0x0125, 0x0125, 2}, /* LATIN SMALL LETTER H WITH CIRCUMFLEX */
	{0x0126, 0x0126, 2}, /* LATIN CAPITAL LETTER H WITH STROKE */
	{0x0127, 0x0127, 2}, /* LATIN SMALL LETTER H WITH STROKE */
	{0x0128, 0x0128, 2}, /* LATIN CAPITAL LETTER I WITH TILDE */
	{0x0129, 0x0129, 2}, /* LATIN SMALL LETTER I WITH TILDE */
	{0x012A, 0x012A, 2}, /* LATIN CAPITAL LETTER I WITH MACRON */
	{0x012B, 0x012B, 2}, /* LATIN SMALL LETTER I WITH MACRON */
	{0x012E, 0x012E, 2}, /* LATIN CAPITAL LETTER I WITH OGONEK */
	{0x012F, 0x012F, 2}, /* LATIN SMALL LETTER I WITH OGONEK */
	{0x0130, 0x0130, 2}, /* LATIN CAPITAL LETTER I WITH DOT ABOVE */
	{0x0131, 0x0131, 2}, /* LATIN SMALL LETTER DOTLESS I */
	{0x0132, 0x0132, 2}, /* LATIN CAPITAL LIGATURE IJ */
	{0x0133, 0x0133, 2}, /* LATIN SMALL LIGATURE IJ */
	{0x0134, 0x0134, 2}, /* LATIN CAPITAL LETTER J WITH CIRCUMFLEX */
	{0x0135, 0x0135, 2}, /* LATIN SMALL LETTER J WITH CIRCUMFLEX */
	{0x0136, 0x0136, 2}, /* LATIN CAPITAL LETTER K WITH CEDILLA */
	{0x0137, 0x0137, 2}, /* LATIN SMALL LETTER K WITH CEDILLA */
	{0x0138, 0x0138, 2}, /* LATIN SMALL LETTER KRA */
	{0x0139, 0x0139, 2}, /* LATIN CAPITAL LETTER L WITH ACUTE */
	{0x013A, 0x013A, 2}, /* LATIN SMALL LETTER L WITH ACUTE */
	{0x013B, 0x013B, 2}, /* LATIN CAPITAL LETTER L WITH CEDILLA */
	{0x013C, 0x013C, 2}, /* LATIN SMALL LETTER L WITH CEDILLA */
	{0x013D, 0x013D, 2}, /* LATIN CAPITAL LETTER L WITH CARON */
	{0x013E, 0x013E, 2}, /* LATIN SMALL LETTER L WITH CARON */
	{0x013F, 0x013F, 2}, /* LATIN CAPITAL LETTER L WITH MIDDLE DOT */
	{0x0140, 0x0140, 2}, /* LATIN SMALL LETTER L WITH MIDDLE DOT */
	{0x0141, 0x0141, 2}, /* LATIN CAPITAL LETTER L WITH STROKE */
	{0x0142, 0x0142, 2}, /* LATIN SMALL LETTER L WITH STROKE */
	{0x0143, 0x0143, 2}, /* LATIN CAPITAL LETTER N WITH ACUTE */
	{0x0144, 0x0144, 2}, /* LATIN SMALL LETTER N WITH ACUTE */
	{0x0145, 0x0145, 2}, /* LATIN CAPITAL LETTER N WITH CEDILLA */
	{0x0146, 0x0146, 2}, /* LATIN SMALL LETTER N WITH CEDILLA */
	{0x0147, 0x0147, 2}, /* LATIN CAPITAL LETTER N WITH CARON */
	{0x0148, 0x0148, 2}, /* LATIN SMALL LETTER N WITH CARON */
	{0x0149, 0x0149, 2}, /* LATIN SMALL LETTER N PRECEDED BY APOSTROPHE */
	{0x014A, 0x014A, 2}, /* LATIN CAPITAL LETTER ENG */
	{0x014B, 0x014B, 2}, /* LATIN SMALL LETTER ENG */
	{0x014C, 0x014C, 2}, /* LATIN CAPITAL LETTER O WITH MACRON */
	{0x014D, 0x014D, 2}, /* LATIN SMALL LETTER O WITH MACRON */
	{0x0150, 0x0150, 2}, /* LATIN CAPITAL LETTER O WITH DOUBLE ACUTE */
	{0x0151, 0x0151, 2}, /* LATIN SMALL LETTER O WITH DOUBLE ACUTE */
	{0x0152, 0x0152, 2}, /* LATIN CAPITAL LIGATURE OE */
	{0x0153, 0x0153, 2}, /* LATIN SMALL LIGATURE OE */
	{0x0154, 0x0154, 2}, /* LATIN CAPITAL LETTER R WITH ACUTE */
	{0x0155, 0x0155, 2}, /* LATIN SMALL LETTER R WITH ACUTE */
	{0x0156, 0x0156, 2}, /* LATIN CAPITAL LETTER R WITH CEDILLA */
	{0x0157, 0x0157, 2}, /* LATIN SMALL LETTER R WITH CEDILLA */
	{0x0158, 0x0158, 2}, /* LATIN CAPITAL LETTER R WITH CARON */
	{0x0159, 0x0159, 2}, /* LATIN SMALL LETTER R WITH CARON */
	{0x015A, 0x015A, 2}, /* LATIN CAPITAL LETTER S WITH ACUTE */
	{0x015B, 0x015B, 2}, /* LATIN SMALL LETTER S WITH ACUTE */
	{0x015C, 0x015C, 2}, /* LATIN CAPITAL LETTER S WITH CIRCUMFLEX */
	{0x015D, 0x015D, 2}, /* LATIN SMALL LETTER S WITH CIRCUMFLEX */
	{0x015E, 0x015E, 2}, /* LATIN CAPITAL LETTER S WITH CEDILLA */
	{0x015F, 0x015F, 2}, /* LATIN SMALL LETTER S WITH CEDILLA */
	{0x0160, 0x0160, 2}, /* LATIN CAPITAL LETTER S WITH CARON */
	{0x0161, 0x0161, 2}, /* LATIN SMALL LETTER S WITH CARON */
	{0x0162, 0x0162, 2}, /* LATIN CAPITAL LETTER T WITH CEDILLA */
	{0x0163, 0x0163, 2}, /* LATIN SMALL LETTER T WITH CEDILLA */
	{0x0164, 0x0164, 2}, /* LATIN CAPITAL LETTER T WITH CARON */
	{0x0165, 0x0165, 2}, /* LATIN SMALL LETTER T WITH CARON */
	{0x0166, 0x0166, 2}, /* LATIN CAPITAL LETTER T WITH STROKE */
	{0x0167, 0x0167, 2}, /* LATIN SMALL LETTER T WITH STROKE */
	{0x0168, 0x0168, 2}, /* LATIN CAPITAL LETTER U WITH TILDE */
	{0x0169, 0x0169, 2}, /* LATIN SMALL LETTER U WITH TILDE */
	{0x016A, 0x016A, 2}, /* LATIN CAPITAL LETTER U WITH MACRON */
	{0x016B, 0x016B, 2}, /* LATIN SMALL LETTER U WITH MACRON */
	{0x016C, 0x016C, 2}, /* LATIN CAPITAL LETTER U WITH BREVE */
	{0x016D, 0x016D, 2}, /* LATIN SMALL LETTER U WITH BREVE */
	{0x016E, 0x016E, 2}, /* LATIN CAPITAL LETTER U WITH RING ABOVE */
	{0x016F, 0x016F, 2}, /* LATIN SMALL LETTER U WITH RING ABOVE */
	{0x0170, 0x0170, 2}, /* LATIN CAPITAL LETTER U WITH DOUBLE ACUTE */
	{0x0171, 0x0171, 2}, /* LATIN SMALL LETTER U WITH DOUBLE ACUTE */
	{0x0172, 0x0172, 2}, /* LATIN CAPITAL LETTER U WITH OGONEK */
	{0x0173, 0x0173, 2}, /* LATIN SMALL LETTER U WITH OGONEK */
	{0x0174, 0x0174, 2}, /* LATIN CAPITAL LETTER W WITH CIRCUMFLEX */
	{0x0175, 0x0175, 2}, /* LATIN SMALL LETTER W WITH CIRCUMFLEX */
	{0x0176, 0x0176, 2}, /* LATIN CAPITAL LETTER Y WITH CIRCUMFLEX */
	{0x0177, 0x0177, 2}, /* LATIN SMALL LETTER Y WITH CIRCUMFLEX */
	{0x0178, 0x0178, 2}, /* LATIN CAPITAL LETTER Y WITH DIAERESIS */
	{0x0179, 0x0179, 2}, /* LATIN CAPITAL LETTER Z WITH ACUTE */
	{0x017A, 0x017A, 2}, /* LATIN SMALL LETTER Z WITH ACUTE */
	{0x017B, 0x017B, 2}, /* LATIN CAPITAL LETTER Z WITH DOT ABOVE */
	{0x017C, 0x017C, 2}, /* LATIN SMALL LETTER Z WITH DOT ABOVE */
	{0x017D, 0x017D, 2}, /* LATIN CAPITAL LETTER Z WITH CARON */
	{0x017E, 0x017E, 2}, /* LATIN SMALL LETTER Z WITH CARON */
	{0x0192, 0x0192, 2}, /* LATIN SMALL LETTER F WITH HOOK */
	{0x01B5, 0x01B5, 2}, /* LATIN CAPITAL LETTER Z WITH STROKE */
	{0x01F5, 0x01F5, 2}, /* LATIN SMALL LETTER G WITH ACUTE */
	{0x02C6, 0x02C6, 2}, /* MODIFIER LETTER CIRCUMFLEX ACCENT */
	{0x02C7, 0x02C7, 2}, /* CARON */
	{0x02D8, 0x02D8, 2}, /* BREVE */
	{0x02D9, 0x02D9, 2}, /* DOT ABOVE */
	{0x02DA, 0x02DA, 2}, /* RING ABOVE */
	{0x02DB, 0x02DB, 2}, /* OGONEK */
	{0x02DC, 0x02DC, 2}, /* SMALL TILDE */
	{0x02DD, 0x02DD, 2}, /* DOUBLE ACUTE ACCENT */
	{0x0311, 0x0311, 2}, /* COMBINING INVERTED BREVE */
	{0x0332, 0x0332, 2}, /* COMBINING LOW LINE */
	{0x0393, 0x0393, 2}, /* GREEK CAPITAL LETTER GAMMA */
	{0x0394, 0x0394, 2}, /* GREEK CAPITAL LETTER DELTA */
	{0x0398, 0x0398, 2}, /* GREEK CAPITAL LETTER THETA */
	{0x039B, 0x039B, 2}, /* GREEK CAPITAL LETTER LAMDA */
	{0x039E, 0x039E, 2}, /* GREEK CAPITAL LETTER XI */
	{0x03A0, 0x03A0, 2}, /* GREEK CAPITAL LETTER PI */
	{0x03A3, 0x03A3, 2}, /* GREEK CAPITAL LETTER SIGMA */
	{0x03A5, 0x03A5, 2}, /* GREEK CAPITAL LETTER UPSILON */
	{0x03A6, 0x03A6, 2}, /* GREEK CAPITAL LETTER PHI */
	{0x03A8, 0x03A8, 2}, /* GREEK CAPITAL LETTER PSI */
	{0x03A9, 0x03A9, 2}, /* GREEK CAPITAL LETTER OMEGA */
	{0x03B1, 0x03B1, 2}, /* GREEK SMALL LETTER ALPHA */
	{0x03B2, 0x03B2, 2}, /* GREEK SMALL LETTER BETA */
	{0x03B3, 0x03B3, 2}, /* GREEK SMALL LETTER GAMMA */
	{0x03B4, 0x03B4, 2}, /* GREEK SMALL LETTER DELTA */
	{0x03B5, 0x03B5, 2}, /* GREEK SMALL LETTER EPSILON */
	{0x03B6, 0x03B6, 2}, /* GREEK SMALL LETTER ZETA */
	{0x03B7, 0x03B7, 2}, /* GREEK SMALL LETTER ETA */
	{0x03B8, 0x03B8, 2}, /* GREEK SMALL LETTER THETA */
	{0x03B9, 0x03B9, 2}, /* GREEK SMALL LETTER IOTA */
	{0x03BA, 0x03BA, 2}, /* GREEK SMALL LETTER KAPPA */
	{0x03BB, 0x03BB, 2}, /* GREEK SMALL LETTER LAMDA */
	{0x03BC, 0x03BC, 2}, /* GREEK SMALL LETTER MU */
	{0x03BD, 0x03BD, 2}, /* GREEK SMALL LETTER NU */
	{0x03BE, 0x03BE, 2}, /* GREEK SMALL LETTER XI */
	{0x03C0, 0x03C0, 2}, /* GREEK SMALL LETTER PI */
	{0x03C1, 0x03C1, 2}, /* GREEK SMALL LETTER RHO */
	{0x03C2, 0x03C2, 2}, /* GREEK SMALL LETTER FINAL SIGMA */
	{0x03C3, 0x03C3, 2}, /* GREEK SMALL LETTER SIGMA */
	{0x03C4, 0x03C4, 2}, /* GREEK SMALL LETTER TAU */
	{0x03C5, 0x03C5, 2}, /* GREEK SMALL LETTER UPSILON */
	{0x03C6, 0x03C6, 2}, /* GREEK SMALL LETTER PHI */
	{0x03C7, 0x03C7, 2}, /* GREEK SMALL LETTER CHI */
	{0x03C8, 0x03C8, 2}, /* GREEK SMALL LETTER PSI */
	{0x03C9, 0x03C9, 2}, /* GREEK SMALL LETTER OMEGA */
	{0x03D1, 0x03D1, 2}, /* GREEK THETA SYMBOL */
	{0x03D2, 0x03D2, 2}, /* GREEK UPSILON WITH HOOK SYMBOL */
	{0x03D5, 0x03D5, 2}, /* GREEK PHI SYMBOL */
	{0x03D6, 0x03D6, 2}, /* GREEK PI SYMBOL */
	{0x03DC, 0x03DC, 2}, /* GREEK LETTER DIGAMMA */
	{0x03DD, 0x03DD, 2}, /* GREEK SMALL LETTER DIGAMMA */
	{0x03F0, 0x03F0, 2}, /* GREEK KAPPA SYMBOL */
	{0x03F1, 0x03F1, 2}, /* GREEK RHO SYMBOL */
	{0x03F5, 0x03F5, 2}, /* GREEK LUNATE EPSILON SYMBOL */
	{0x03F6, 0x03F6, 2}, /* GREEK REVERSED LUNATE EPSILON SYMBOL */
	{0x0401, 0x0401, 2}, /* CYRILLIC CAPITAL LETTER IO */
	{0x0402, 0x0402, 2}, /* CYRILLIC CAPITAL LETTER DJE */
	{0x0403, 0x0403, 2}, /* CYRILLIC CAPITAL LETTER GJE */
	{0x0404, 0x0404, 2}, /* CYRILLIC CAPITAL LETTER UKRAINIAN IE */
	{0x0405, 0x0405, 2}, /* CYRILLIC CAPITAL LETTER DZE */
	{0x0406, 0x0406, 2}, /* CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I */
	{0x0407, 0x0407, 2}, /* CYRILLIC CAPITAL LETTER YI */
	{0x0408, 0x0408, 2}, /* CYRILLIC CAPITAL LETTER JE */
	{0x0409, 0x0409, 2}, /* CYRILLIC CAPITAL LETTER LJE */
	{0x040A, 0x040A, 2}, /* CYRILLIC CAPITAL LETTER NJE */
	{0x040B, 0x040B, 2}, /* CYRILLIC CAPITAL LETTER TSHE */
	{0x040C, 0x040C, 2}, /* CYRILLIC CAPITAL LETTER KJE */
	{0x040E, 0x040E, 2}, /* CYRILLIC CAPITAL LETTER SHORT U */
	{0x040F, 0x040F, 2}, /* CYRILLIC CAPITAL LETTER DZHE */
	{0x0410, 0x0410, 2}, /* CYRILLIC CAPITAL LETTER A */
	{0x0411, 0x0411, 2}, /* CYRILLIC CAPITAL LETTER BE */
	{0x0412, 0x0412, 2}, /* CYRILLIC CAPITAL LETTER VE */
	{0x0413, 0x0413, 2}, /* CYRILLIC CAPITAL LETTER GHE */
	{0x0414, 0x0414, 2}, /* CYRILLIC CAPITAL LETTER DE */
	{0x0415, 0x0415, 2}, /* CYRILLIC CAPITAL LETTER IE */
	{0x0416, 0x0416, 2}, /* CYRILLIC CAPITAL LETTER ZHE */
	{0x0417, 0x0417, 2}, /* CYRILLIC CAPITAL LETTER ZE */
	{0x0418, 0x0418, 2}, /* CYRILLIC CAPITAL LETTER I */
	{0x0419, 0x0419, 2}, /* CYRILLIC CAPITAL LETTER SHORT I */
	{0x041A, 0x041A, 2}, /* CYRILLIC CAPITAL LETTER KA */
	{0x041B, 0x041B, 2}, /* CYRILLIC CAPITAL LETTER EL */
	{0x041C, 0x041C, 2}, /* CYRILLIC CAPITAL LETTER EM */
	{0x041D, 0x041D, 2}, /* CYRILLIC CAPITAL LETTER EN */
	{0x041E, 0x041E, 2}, /* CYRILLIC CAPITAL LETTER O */
	{0x041F, 0x041F, 2}, /* CYRILLIC CAPITAL LETTER PE */
	{0x0420, 0x0420, 2}, /* CYRILLIC CAPITAL LETTER ER */
	{0x0421, 0x0421, 2}, /* CYRILLIC CAPITAL LETTER ES */
	{0x0422, 0x0422, 2}, /* CYRILLIC CAPITAL LETTER TE */
	{0x0423, 0x0423, 2}, /* CYRILLIC CAPITAL LETTER U */
	{0x0424, 0x0424, 2}, /* CYRILLIC CAPITAL LETTER EF */
	{0x0425, 0x0425, 2}, /* CYRILLIC CAPITAL LETTER HA */
	{0x0426, 0x0426, 2}, /* CYRILLIC CAPITAL LETTER TSE */
	{0x0427, 0x0427, 2}, /* CYRILLIC CAPITAL LETTER CHE */
	{0x0428, 0x0428, 2}, /* CYRILLIC CAPITAL LETTER SHA */
	{0x0429, 0x0429, 2}, /* CYRILLIC CAPITAL LETTER SHCHA */
	{0x042A, 0x042A, 2}, /* CYRILLIC CAPITAL LETTER HARD SIGN */
	{0x042B, 0x042B, 2}, /* CYRILLIC CAPITAL LETTER YERU */
	{0x042C, 0x042C, 2}, /* CYRILLIC CAPITAL LETTER SOFT SIGN */
	{0x042D, 0x042D, 2}, /* CYRILLIC CAPITAL LETTER E */
	{0x042E, 0x042E, 2}, /* CYRILLIC CAPITAL LETTER YU */
	{0x042F, 0x042F, 2}, /* CYRILLIC CAPITAL LETTER YA */
	{0x0430, 0x0430, 2}, /* CYRILLIC SMALL LETTER A */
	{0x0431, 0x0431, 2}, /* CYRILLIC SMALL LETTER BE */
	{0x0432, 0x0432, 2}, /* CYRILLIC SMALL LETTER VE */
	{0x0433, 0x0433, 2}, /* CYRILLIC SMALL LETTER GHE */
	{0x0434, 0x0434, 2}, /* CYRILLIC SMALL LETTER DE */
	{0x0435, 0x0435, 2}, /* CYRILLIC SMALL LETTER IE */
	{0x0436, 0x0436, 2}, /* CYRILLIC SMALL LETTER ZHE */
	{0x0437, 0x0437, 2}, /* CYRILLIC SMALL LETTER ZE */
	{0x0438, 0x0438, 2}, /* CYRILLIC SMALL LETTER I */
	{0x0439, 0x0439, 2}, /* CYRILLIC SMALL LETTER SHORT I */
	{0x043A, 0x043A, 2}, /* CYRILLIC SMALL LETTER KA */
	{0x043B, 0x043B, 2}, /* CYRILLIC SMALL LETTER EL */
	{0x043C, 0x043C, 2}, /* CYRILLIC SMALL LETTER EM */
	{0x043D, 0x043D, 2}, /* CYRILLIC SMALL LETTER EN */
	{0x043E, 0x043E, 2}, /* CYRILLIC SMALL LETTER O */
	{0x043F, 0x043F, 2}, /* CYRILLIC SMALL LETTER PE */
	{0x0440, 0x0440, 2}, /* CYRILLIC SMALL LETTER ER */
	{0x0441, 0x0441, 2}, /* CYRILLIC SMALL LETTER ES */
	{0x0442, 0x0442, 2}, /* CYRILLIC SMALL LETTER TE */
	{0x0443, 0x0443, 2}, /* CYRILLIC SMALL LETTER U */
	{0x0444, 0x0444, 2}, /* CYRILLIC SMALL LETTER EF */
	{0x0445, 0x0445, 2}, /* CYRILLIC SMALL LETTER HA */
	{0x0446, 0x0446, 2}, /* CYRILLIC SMALL LETTER TSE */
	{0x0447, 0x0447, 2}, /* CYRILLIC SMALL LETTER CHE */
	{0x0448, 0x0448, 2}, /* CYRILLIC SMALL LETTER SHA */
	{0x0449, 0x0449, 2}, /* CYRILLIC SMALL LETTER SHCHA */
	{0x044A, 0x044A, 2}, /* CYRILLIC SMALL LETTER HARD SIGN */
	{0x044B, 0x044B, 2}, /* CYRILLIC SMALL LETTER YERU */
	{0x044C, 0x044C, 2}, /* CYRILLIC SMALL LETTER SOFT SIGN */
	{0x044D, 0x044D, 2}, /* CYRILLIC SMALL LETTER E */
	{0x044E, 0x044E, 2}, /* CYRILLIC SMALL LETTER YU */
	{0x044F, 0x044F, 2}, /* CYRILLIC SMALL LETTER YA */
	{0x0451, 0x0451, 2}, /* CYRILLIC SMALL LETTER IO */
	{0x0452, 0x0452, 2}, /* CYRILLIC SMALL LETTER DJE */
	{0x0453, 0x0453, 2}, /* CYRILLIC SMALL LETTER GJE */
	{0x0454, 0x0454, 2}, /* CYRILLIC SMALL LETTER UKRAINIAN IE */
	{0x0455, 0x0455, 2}, /* CYRILLIC SMALL LETTER DZE */
	{0x0456, 0x0456, 2}, /* CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I */
	{0x0457, 0x0457, 2}, /* CYRILLIC SMALL LETTER YI */
	{0x0458, 0x0458, 2}, /* CYRILLIC SMALL LETTER JE */
	{0x0459, 0x0459, 2}, /* CYRILLIC SMALL LETTER LJE */
	{0x045A, 0x045A, 2}, /* CYRILLIC SMALL LETTER NJE */
	{0x045B, 0x045B, 2}, /* CYRILLIC SMALL LETTER TSHE */
	{0x045C, 0x045C, 2}, /* CYRILLIC SMALL LETTER KJE */
	{0x045E, 0x045E, 2}, /* CYRILLIC SMALL LETTER SHORT U */
	{0x045F, 0x045F, 2}, /* CYRILLIC SMALL LETTER DZHE */
	{0x2002, 0x2002, 2}, /* EN SPACE */
	{0x2003, 0x2003, 2}, /* EM SPACE */
	{0x2004, 0x2004, 2}, /* THREE-PER-EM SPACE */
	{0x2005, 0x2005, 2}, /* FOUR-PER-EM SPACE */
	{0x2007, 0x2007, 2}, /* FIGURE SPACE */
	{0x2008, 0x2008, 2}, /* PUNCTUATION SPACE */
	{0x2009, 0x2009, 2}, /* THIN SPACE */
	{0x200A, 0x200A, 2}, /* HAIR SPACE */
	{0x200B, 0x200B, 2}, /* ZERO WIDTH SPACE */
	{0x2010, 0x2010, 2}, /* HYPHEN */
	{0x2013, 0x2013, 2}, /* EN DASH */
	{0x2014, 0x2014, 2}, /* EM DASH */
	{0x2015, 0x2015, 2}, /* HORIZONTAL BAR */
	{0x2016, 0x2016, 2}, /* DOUBLE VERTICAL LINE */
	{0x2018, 0x2018, 2}, /* LEFT SINGLE QUOTATION MARK */
	{0x2019, 0x2019, 2}, /* RIGHT SINGLE QUOTATION MARK */
	{0x201A, 0x201A, 2}, /* SINGLE LOW-9 QUOTATION MARK */
	{0x201C, 0x201C, 2}, /* LEFT DOUBLE QUOTATION MARK */
	{0x201D, 0x201D, 2}, /* RIGHT DOUBLE QUOTATION MARK */
	{0x201E, 0x201E, 2}, /* DOUBLE LOW-9 QUOTATION MARK */
	{0x2020, 0x2020, 2}, /* DAGGER */
	{0x2021, 0x2021, 2}, /* DOUBLE DAGGER */
	{0x2022, 0x2022, 2}, /* BULLET */
	{0x2025, 0x2025, 2}, /* TWO DOT LEADER */
	{0x2026, 0x2026, 2}, /* HORIZONTAL ELLIPSIS */
	{0x2030, 0x2030, 2}, /* PER MILLE SIGN */
	{0x2031, 0x2031, 2}, /* PER TEN THOUSAND SIGN */
	{0x2032, 0x2032, 2}, /* PRIME */
	{0x2033, 0x2033, 2}, /* DOUBLE PRIME */
	{0x2034, 0x2034, 2}, /* TRIPLE PRIME */
	{0x2035, 0x2035, 2}, /* REVERSED PRIME */
	{0x2039, 0x2039, 2}, /* SINGLE LEFT-POINTING ANGLE QUOTATION MARK */
	{0x203A, 0x203A, 2}, /* SINGLE RIGHT-POINTING ANGLE QUOTATION MARK */
	{0x2041, 0x2041, 2}, /* CARET INSERTION POINT */
	{0x2043, 0x2043, 2}, /* HYPHEN BULLET */
	{0x204F, 0x204F, 2}, /* REVERSED SEMICOLON */
	{0x2057, 0x2057, 2}, /* QUADRUPLE PRIME */
	{0x205F, 0x205F, 2}, /* MEDIUM MATHEMATICAL SPACE */
	{0x2060, 0x2060, 2}, /* WORD JOINER */
	{0x2061, 0x2061, 2}, /* FUNCTION APPLICATION */
	{0x2062, 0x2062, 2}, /* INVISIBLE TIMES */
	{0x2063, 0x2063, 2}, /* INVISIBLE SEPARATOR */
	{0x20AC, 0x20AC, 2}, /* EURO SIGN */
	{0x20DB, 0x20DB, 2}, /* COMBINING THREE DOTS ABOVE */
	{0x20DC, 0x20DC, 2}, /* COMBINING FOUR DOTS ABOVE */
	{0x2102, 0x2102, 2}, /* DOUBLE-STRUCK CAPITAL C */
	{0x2105, 0x2105, 2}, /* CARE OF */
	{0x210A, 0x210A, 2}, /* SCRIPT SMALL G */
	{0x210B, 0x210B, 2}, /* SCRIPT CAPITAL H */
	{0x210C, 0x210C, 2}, /* BLACK-LETTER CAPITAL H */
	{0x210D, 0x210D, 2}, /* DOUBLE-STRUCK CAPITAL H */
	{0x210E, 0x210E, 2}, /* PLANCK CONSTANT */
	{0x210F, 0x210F, 2}, /* PLANCK CONSTANT OVER TWO PI */
	{0x2110, 0x2110, 2}, /* SCRIPT CAPITAL I */
	{0x2111, 0x2111, 2}, /* BLACK-LETTER CAPITAL I */
	{0x2112, 0x2112, 2}, /* SCRIPT CAPITAL L */
	{0x2113, 0x2113, 2}, /* SCRIPT SMALL L */
	{0x2115, 0x2115, 2}, /* DOUBLE-STRUCK CAPITAL N */
	{0x2116, 0x2116, 2}, /* NUMERO SIGN */
	{0x2117, 0x2117, 2}, /* SOUND RECORDING COPYRIGHT */
	{0x2118, 0x2118, 2}, /* SCRIPT CAPITAL P */
	{0x2119, 0x2119, 2}, /* DOUBLE-STRUCK CAPITAL P */
	{0x211A, 0x211A, 2}, /* DOUBLE-STRUCK CAPITAL Q */
	{0x211B, 0x211B, 2}, /* SCRIPT CAPITAL R */
	{0x211C, 0x211C, 2}, /* BLACK-LETTER CAPITAL R */
	{0x211D, 0x211D, 2}, /* DOUBLE-STRUCK CAPITAL R */
	{0x211E, 0x211E, 2}, /* PRESCRIPTION TAKE */
	{0x2122, 0x2122, 2}, /* TRADE MARK SIGN */
	{0x2124, 0x2124, 2}, /* DOUBLE-STRUCK CAPITAL Z */
	{0x2126, 0x2126, 2}, /* OHM SIGN */
	{0x2127, 0x2127, 2}, /* INVERTED OHM SIGN */
	{0x2128, 0x2128, 2}, /* BLACK-LETTER CAPITAL Z */
	{0x2129, 0x2129, 2}, /* TURNED GREEK SMALL LETTER IOTA */
	{0x212B, 0x212B, 2}, /* ANGSTROM SIGN */
	{0x212C, 0x212C, 2}, /* SCRIPT CAPITAL B */
	{0x212D, 0x212D, 2}, /* BLACK-LETTER CAPITAL C */
	{0x212F, 0x212F, 2}, /* SCRIPT SMALL E */
	{0x2130, 0x2130, 2}, /* SCRIPT CAPITAL E */
	{0x2131, 0x2131, 2}, /* SCRIPT CAPITAL F */
	{0x2133, 0x2133, 2}, /* SCRIPT CAPITAL M */
	{0x2134, 0x2134, 2}, /* SCRIPT SMALL O */
	{0x2135, 0x2135, 2}, /* ALEF SYMBOL */
	{0x2136, 0x2136, 2}, /* BET SYMBOL */
	{0x2137, 0x2137, 2}, /* GIMEL SYMBOL */
	{0x2138, 0x2138, 2}, /* DALET SYMBOL */
	{0x2145, 0x2145, 2}, /* DOUBLE-STRUCK ITALIC CAPITAL D */
	{0x2146, 0x2146, 2}, /* DOUBLE-STRUCK ITALIC SMALL D */
	{0x2147, 0x2147, 2}, /* DOUBLE-STRUCK ITALIC SMALL E */
	{0x2148, 0x2148, 2}, /* DOUBLE-STRUCK ITALIC SMALL I */
	{0x2153, 0x2153, 2}, /* VULGAR FRACTION ONE THIRD */
	{0x2154, 0x2154, 2}, /* VULGAR FRACTION TWO THIRDS */
	{0x2155, 0x2155, 2}, /* VULGAR FRACTION ONE FIFTH */
	{0x2156, 0x2156, 2}, /* VULGAR FRACTION TWO FIFTHS */
	{0x2157, 0x2157, 2}, /* VULGAR FRACTION THREE FIFTHS */
	{0x2158, 0x2158, 2}, /* VULGAR FRACTION FOUR FIFTHS */
	{0x2159, 0x2159, 2}, /* VULGAR FRACTION ONE SIXTH */
	{0x215A, 0x215A, 2}, /* VULGAR FRACTION FIVE SIXTHS */
	{0x215B, 0x215B, 2}, /* VULGAR FRACTION ONE EIGHTH */
	{0x215C, 0x215C, 2}, /* VULGAR FRACTION THREE EIGHTHS */
	{0x215D, 0x215D, 2}, /* VULGAR FRACTION FIVE EIGHTHS */
	{0x215E, 0x215E, 2}, /* VULGAR FRACTION SEVEN EIGHTHS */
	{0x2190, 0x2190, 2}, /* LEFTWARDS ARROW */
	{0x2191, 0x2191, 2}, /* UPWARDS ARROW */
	{0x2192, 0x2192, 2}, /* RIGHTWARDS ARROW */
	{0x2193, 0x2193, 2}, /* DOWNWARDS ARROW */
	{0x2194, 0x2194, 2}, /* LEFT RIGHT ARROW */
	{0x2195, 0x2195, 2}, /* UP DOWN ARROW */
	{0x2196, 0x2196, 2}, /* NORTH WEST ARROW */
	{0x2197, 0x2197, 2}, /* NORTH EAST ARROW */
	{0x2198, 0x2198, 2}, /* SOUTH EAST ARROW */
	{0x2199, 0x2199, 2}, /* SOUTH WEST ARROW */
	{0x219A, 0x219A, 2}, /* LEFTWARDS ARROW WITH STROKE */
	{0x219B, 0x219B, 2}, /* RIGHTWARDS ARROW WITH STROKE */
	{0x219D, 0x219D, 2}, /* RIGHTWARDS WAVE ARROW */
	{0x219E, 0x219E, 2}, /* LEFTWARDS TWO HEADED ARROW */
	{0x219F, 0x219F, 2}, /* UPWARDS TWO HEADED ARROW */
	{0x21A0, 0x21A0, 2}, /* RIGHTWARDS TWO HEADED ARROW */
	{0x21A1, 0x21A1, 2}, /* DOWNWARDS TWO HEADED ARROW */
	{0x21A2, 0x21A2, 2}, /* LEFTWARDS ARROW WITH TAIL */
	{0x21A3, 0x21A3, 2}, /* RIGHTWARDS ARROW WITH TAIL */
	{0x21A4, 0x21A4, 2}, /* LEFTWARDS ARROW FROM BAR */
	{0x21A5, 0x21A5, 2}, /* UPWARDS ARROW FROM BAR */
	{0x21A6, 0x21A6, 2}, /* RIGHTWARDS ARROW FROM BAR */
	{0x21A7, 0x21A7, 2}, /* DOWNWARDS ARROW FROM BAR */
	{0x21A9, 0x21A9, 2}, /* LEFTWARDS ARROW WITH HOOK */
	{0x21AA, 0x21AA, 2}, /* RIGHTWARDS ARROW WITH HOOK */
	{0x21AB, 0x21AB, 2}, /* LEFTWARDS ARROW WITH LOOP */
	{0x21AC, 0x21AC, 2}, /* RIGHTWARDS ARROW WITH LOOP */
	{0x21AD, 0x21AD, 2}, /* LEFT RIGHT WAVE ARROW */
	{0x21AE, 0x21AE, 2}, /* LEFT RIGHT ARROW WITH STROKE */
	{0x21B0, 0x21B0, 2}, /* UPWARDS ARROW WITH TIP LEFTWARDS */
	{0x21B1, 0x21B1, 2}, /* UPWARDS ARROW WITH TIP RIGHTWARDS */
	{0x21B2, 0x21B2, 2}, /* DOWNWARDS ARROW WITH TIP LEFTWARDS */
	{0x21B3, 0x21B3, 2}, /* DOWNWARDS ARROW WITH TIP RIGHTWARDS */
	{0x21B6, 0x21B6, 2}, /* ANTICLOCKWISE TOP SEMICIRCLE ARROW */
	{0x21B7, 0x21B7, 2}, /* CLOCKWISE TOP SEMICIRCLE ARROW */
	{0x21BA, 0x21BA, 2}, /* ANTICLOCKWISE OPEN CIRCLE ARROW */
	{0x21BB, 0x21BB, 2}, /* CLOCKWISE OPEN CIRCLE ARROW */
	{0x21BC, 0x21BC, 2}, /* LEFTWARDS HARPOON WITH BARB UPWARDS */
	{0x21BD, 0x21BD, 2}, /* LEFTWARDS HARPOON WITH BARB DOWNWARDS */
	{0x21BE, 0x21BE, 2}, /* UPWARDS HARPOON WITH BARB RIGHTWARDS */
	{0x21BF, 0x21BF, 2}, /* UPWARDS HARPOON WITH BARB LEFTWARDS */
	{0x21C0, 0x21C0, 2}, /* RIGHTWARDS HARPOON WITH BARB UPWARDS */
	{0x21C1, 0x21C1, 2}, /* RIGHTWARDS HARPOON WITH BARB DOWNWARDS */
	{0x21C2, 0x21C2, 2}, /* DOWNWARDS HARPOON WITH BARB RIGHTWARDS */
	{0x21C3, 0x21C3, 2}, /* DOWNWARDS HARPOON WITH BARB LEFTWARDS */
	{0x21C4, 0x21C4, 2}, /* RIGHTWARDS ARROW OVER LEFTWARDS ARROW */
	{0x21C5, 0x21C5, 2}, /* UPWARDS ARROW LEFTWARDS OF DOWNWARDS ARROW */
	{0x21C6, 0x21C6, 2}, /* LEFTWARDS ARROW OVER RIGHTWARDS ARROW */
	{0x21C7, 0x21C7, 2}, /* LEFTWARDS PAIRED ARROWS */
	{0x21C8, 0x21C8, 2}, /* UPWARDS PAIRED ARROWS */
	{0x21C9, 0x21C9, 2}, /* RIGHTWARDS PAIRED ARROWS */
	{0x21CA, 0x21CA, 2}, /* DOWNWARDS PAIRED ARROWS */
	{0x21CB, 0x21CB, 2}, /* LEFTWARDS HARPOON OVER RIGHTWARDS HARPOON */
	{0x21CC, 0x21CC, 2}, /* RIGHTWARDS HARPOON OVER LEFTWARDS HARPOON */
	{0x21CD, 0x21CD, 2}, /* LEFTWARDS DOUBLE ARROW WITH STROKE */
	{0x21CE, 0x21CE, 2}, /* LEFT RIGHT DOUBLE ARROW WITH STROKE */
	{0x21CF, 0x21CF, 2}, /* RIGHTWARDS DOUBLE ARROW WITH STROKE */
	{0x21D0, 0x21D0, 2}, /* LEFTWARDS DOUBLE ARROW */
	{0x21D1, 0x21D1, 2}, /* UPWARDS DOUBLE ARROW */
	{0x21D2, 0x21D2, 2}, /* RIGHTWARDS DOUBLE ARROW */
	{0x21D3, 0x21D3, 2}, /* DOWNWARDS DOUBLE ARROW */
	{0x21D4, 0x21D4, 2}, /* LEFT RIGHT DOUBLE ARROW */
	{0x21D5, 0x21D5, 2}, /* UP DOWN DOUBLE ARROW */
	{0x21D6, 0x21D6, 2}, /* NORTH WEST DOUBLE ARROW */
	{0x21D7, 0x21D7, 2}, /* NORTH EAST DOUBLE ARROW */
	{0x21D8, 0x21D8, 2}, /* SOUTH EAST DOUBLE ARROW */
	{0x21D9, 0x21D9, 2}, /* SOUTH WEST DOUBLE ARROW */
	{0x21DA, 0x21DA, 2}, /* LEFTWARDS TRIPLE ARROW */
	{0x21DB, 0x21DB, 2}, /* RIGHTWARDS TRIPLE ARROW */
	{0x21DD, 0x21DD, 2}, /* RIGHTWARDS SQUIGGLE ARROW */
	{0x21E4, 0x21E4, 2}, /* LEFTWARDS ARROW TO BAR */
	{0x21E5, 0x21E5, 2}, /* RIGHTWARDS ARROW TO BAR */
	{0x21F5, 0x21F5, 2}, /* DOWNWARDS ARROW LEFTWARDS OF UPWARDS ARROW */
	{0x21FD, 0x21FD, 2}, /* LEFTWARDS OPEN-HEADED ARROW */
	{0x21FE, 0x21FE, 2}, /* RIGHTWARDS OPEN-HEADED ARROW */
	{0x21FF, 0x21FF, 2}, /* LEFT RIGHT OPEN-HEADED ARROW */
	{0x2200, 0x2200, 2}, /* FOR ALL */
	{0x2201, 0x2201, 2}, /* COMPLEMENT */
	{0x2202, 0x2202, 2}, /* PARTIAL DIFFERENTIAL */
	{0x2203, 0x2203, 2}, /* THERE EXISTS */
	{0x2204, 0x2204, 2}, /* THERE DOES NOT EXIST */
	{0x2205, 0x2205, 2}, /* EMPTY SET */
	{0x2207, 0x2207, 2}, /* NABLA */
	{0x2208, 0x2208, 2}, /* ELEMENT OF */
	{0x2209, 0x2209, 2}, /* NOT AN ELEMENT OF */
	{0x220B, 0x220B, 2}, /* CONTAINS AS MEMBER */
	{0x220C, 0x220C, 2}, /* DOES NOT CONTAIN AS MEMBER */
	{0x220F, 0x220F, 2}, /* N-ARY PRODUCT */
	{0x2210, 0x2210, 2}, /* N-ARY COPRODUCT */
	{0x2211, 0x2211, 2}, /* N-ARY SUMMATION */
	{0x2212, 0x2212, 2}, /* MINUS SIGN */
	{0x2213, 0x2213, 2}, /* MINUS-OR-PLUS SIGN */
	{0x2214, 0x2214, 2}, /* DOT PLUS */
	{0x2216, 0x2216, 2}, /* SET MINUS */
	{0x2217, 0x2217, 2}, /* ASTERISK OPERATOR */
	{0x2218, 0x2218, 2}, /* RING OPERATOR */
	{0x221A, 0x221A, 2}, /* SQUARE ROOT */
	{0x221D, 0x221D, 2}, /* PROPORTIONAL TO */
	{0x221E, 0x221E, 2}, /* INFINITY */
	{0x221F, 0x221F, 2}, /* RIGHT ANGLE */
	{0x2220, 0x2220, 2}, /* ANGLE */
	{0x2221, 0x2221, 2}, /* MEASURED ANGLE */
	{0x2222, 0x2222, 2}, /* SPHERICAL ANGLE */
	{0x2223, 0x2223, 2}, /* DIVIDES */
	{0x2224, 0x2224, 2}, /* DOES NOT DIVIDE */
	{0x2225, 0x2225, 2}, /* PARALLEL TO */
	{0x2226, 0x2226, 2}, /* NOT PARALLEL TO */
	{0x2227, 0x2227, 2}, /* LOGICAL AND */
	{0x2228, 0x2228, 2}, /* LOGICAL OR */
	{0x2229, 0x2229, 2}, /* INTERSECTION */
	{0x222A, 0x222A, 2}, /* UNION */
	{0x222B, 0x222B, 2}, /* INTEGRAL */
	{0x222C, 0x222C, 2}, /* DOUBLE INTEGRAL */
	{0x222D, 0x222D, 2}, /* TRIPLE INTEGRAL */
	{0x222E, 0x222E, 2}, /* CONTOUR INTEGRAL */
	{0x222F, 0x222F, 2}, /* SURFACE INTEGRAL */
	{0x2230, 0x2230, 2}, /* VOLUME INTEGRAL */
	{0x2231, 0x2231, 2}, /* CLOCKWISE INTEGRAL */
	{0x2232, 0x2232, 2}, /* CLOCKWISE CONTOUR INTEGRAL */
	{0x2233, 0x2233, 2}, /* ANTICLOCKWISE CONTOUR INTEGRAL */
	{0x2234, 0x2234, 2}, /* THEREFORE */
	{0x2235, 0x2235, 2}, /* BECAUSE */
	{0x2236, 0x2236, 2}, /* RATIO */
	{0x2237, 0x2237, 2}, /* PROPORTION */
	{0x2238, 0x2238, 2}, /* DOT MINUS */
	{0x223A, 0x223A, 2}, /* GEOMETRIC PROPORTION */
	{0x223B, 0x223B, 2}, /* HOMOTHETIC */
	{0x223C, 0x223C, 2}, /* TILDE OPERATOR */
	{0x223D, 0x223D, 2}, /* REVERSED TILDE */
	{0x223E, 0x223E, 2}, /* INVERTED LAZY S */
	{0x223F, 0x223F, 2}, /* SINE WAVE */
	{0x2240, 0x2240, 2}, /* WREATH PRODUCT */
	{0x2241, 0x2241, 2}, /* NOT TILDE */
	{0x2242, 0x2242, 2}, /* MINUS TILDE */
	{0x2243, 0x2243, 2}, /* ASYMPTOTICALLY EQUAL TO */
	{0x2244, 0x2244, 2}, /* NOT ASYMPTOTICALLY EQUAL TO */
	{0x2245, 0x2245, 2}, /* APPROXIMATELY EQUAL TO */
	{0x2246, 0x2246, 2}, /* APPROXIMATELY BUT NOT ACTUALLY EQUAL TO */
	{0x2247, 0x2247, 2}, /* NEITHER APPROXIMATELY NOR ACTUALLY EQUAL TO */
	{0x2248, 0x2248, 2}, /* ALMOST EQUAL TO */
	{0x2249, 0x2249, 2}, /* NOT ALMOST EQUAL TO */
	{0x224A, 0x224A, 2}, /* ALMOST EQUAL OR EQUAL TO */
	{0x224B, 0x224B, 2}, /* TRIPLE TILDE */
	{0x224C, 0x224C, 2}, /* ALL EQUAL TO */
	{0x224D, 0x224D, 2}, /* EQUIVALENT TO */
	{0x224E, 0x224E, 2}, /* GEOMETRICALLY EQUIVALENT TO */
	{0x224F, 0x224F, 2}, /* DIFFERENCE BETWEEN */
	{0x2250, 0x2250, 2}, /* APPROACHES THE LIMIT */
	{0x2251, 0x2251, 2}, /* GEOMETRICALLY EQUAL TO */
	{0x2252, 0x2252, 2}, /* APPROXIMATELY EQUAL TO OR THE IMAGE OF */
	{0x2253, 0x2253, 2}, /* IMAGE OF OR APPROXIMATELY EQUAL TO */
	{0x2254, 0x2254, 2}, /* COLON EQUALS */
	{0x2255, 0x2255, 2}, /* EQUALS COLON */
	{0x2256, 0x2256, 2}, /* RING IN EQUAL TO */
	{0x2257, 0x2257, 2}, /* RING EQUAL TO */
	{0x2259, 0x2259, 2}, /* ESTIMATES */
	{0x225A, 0x225A, 2}, /* EQUIANGULAR TO */
	{0x225C, 0x225C, 2}, /* DELTA EQUAL TO */
	{0x225F, 0x225F, 2}, /* QUESTIONED EQUAL TO */
	{0x2260, 0x2260, 2}, /* NOT EQUAL TO */
	{0x2261, 0x2261, 2}, /* IDENTICAL TO */
	{0x2262, 0x2262, 2}, /* NOT IDENTICAL TO */
	{0x2264, 0x2264, 2}, /* LESS-THAN OR EQUAL TO */
	{0x2265, 0x2265, 2}, /* GREATER-THAN OR EQUAL TO */
	{0x2266, 0x2266, 2}, /* LESS-THAN OVER EQUAL TO */
	{0x2267, 0x2267, 2}, /* GREATER-THAN OVER EQUAL TO */
	{0x2268, 0x2268, 2}, /* LESS-THAN BUT NOT EQUAL TO */
	{0x2269, 0x2269, 2}, /* GREATER-THAN BUT NOT EQUAL TO */
	{0x226A, 0x226A, 2}, /* MUCH LESS-THAN */
	{0x226B, 0x226B, 2}, /* MUCH GREATER-THAN */
	{0x226C, 0x226C, 2}, /* BETWEEN */
	{0x226D, 0x226D, 2}, /* NOT EQUIVALENT TO */
	{0x226E, 0x226E, 2}, /* NOT LESS-THAN */
	{0x226F, 0x226F, 2}, /* NOT GREATER-THAN */
	{0x2270, 0x2270, 2}, /* NEITHER LESS-THAN NOR EQUAL TO */
	{0x2271, 0x2271, 2}, /* NEITHER GREATER-THAN NOR EQUAL TO */
	{0x2272, 0x2272, 2}, /* LESS-THAN OR EQUIVALENT TO */
	{0x2273, 0x2273, 2}, /* GREATER-THAN OR EQUIVALENT TO */
	{0x2274, 0x2274, 2}, /* NEITHER LESS-THAN NOR EQUIVALENT TO */
	{0x2275, 0x2275, 2}, /* NEITHER GREATER-THAN NOR EQUIVALENT TO */
	{0x2276, 0x2276, 2}, /* LESS-THAN OR GREATER-THAN */
	{0x2277, 0x2277, 2}, /* GREATER-THAN OR LESS-THAN */
	{0x2278, 0x2278, 2}, /* NEITHER LESS-THAN NOR GREATER-THAN */
	{0x2279, 0x2279, 2}, /* NEITHER GREATER-THAN NOR LESS-THAN */
	{0x227A, 0x227A, 2}, /* PRECEDES */
	{0x227B, 0x227B, 2}, /* SUCCEEDS */
	{0x227C, 0x227C, 2}, /* PRECEDES OR EQUAL TO */
	{0x227D, 0x227D, 2}, /* SUCCEEDS OR EQUAL TO */
	{0x227E, 0x227E, 2}, /* PRECEDES OR EQUIVALENT TO */
	{0x227F, 0x227F, 2}, /* SUCCEEDS OR EQUIVALENT TO */
	{0x2280, 0x2280, 2}, /* DOES NOT PRECEDE */
	{0x2281, 0x2281, 2}, /* DOES NOT SUCCEED */
	{0x2282, 0x2282, 2}, /* SUBSET OF */
	{0x2283, 0x2283, 2}, /* SUPERSET OF */
	{0x2284, 0x2284, 2}, /* NOT A SUBSET OF */
	{0x2285, 0x2285, 2}, /* NOT A SUPERSET OF */
	{0x2286, 0x2286, 2}, /* SUBSET OF OR EQUAL TO */
	{0x2287, 0x2287, 2}, /* SUPERSET OF OR EQUAL TO */
	{0x2288, 0x2288, 2}, /* NEITHER A SUBSET OF NOR EQUAL TO */
	{0x2289, 0x2289, 2}, /* NEITHER A SUPERSET OF NOR EQUAL TO */
	{0x228A, 0x228A, 2}, /* SUBSET OF WITH NOT EQUAL TO */
	{0x228B, 0x228B, 2}, /* SUPERSET OF WITH NOT EQUAL TO */
	{0x228D, 0x228D, 2}, /* MULTISET MULTIPLICATION */
	{0x228E, 0x228E, 2}, /* MULTISET UNION */
	{0x228F, 0x228F, 2}, /* SQUARE IMAGE OF */
	{0x2290, 0x2290, 2}, /* SQUARE ORIGINAL OF */
	{0x2291, 0x2291, 2}, /* SQUARE IMAGE OF OR EQUAL TO */
	{0x2292, 0x2292, 2}, /* SQUARE ORIGINAL OF OR EQUAL TO */
	{0x2293, 0x2293, 2}, /* SQUARE CAP */
	{0x2294, 0x2294, 2}, /* SQUARE CUP */
	{0x2295, 0x2295, 2}, /* CIRCLED PLUS */
	{0x2296, 0x2296, 2}, /* CIRCLED MINUS */
	{0x2297, 0x2297, 2}, /* CIRCLED TIMES */
	{0x2298, 0x2298, 2}, /* CIRCLED DIVISION SLASH */
	{0x2299, 0x2299, 2}, /* CIRCLED DOT OPERATOR */
	{0x229A, 0x229A, 2}, /* CIRCLED RING OPERATOR */
	{0x229B, 0x229B, 2}, /* CIRCLED ASTERISK OPERATOR */
	{0x229D, 0x229D, 2}, /* CIRCLED DASH */
	{0x229E, 0x229E, 2}, /* SQUARED PLUS */
	{0x229F, 0x229F, 2}, /* SQUARED MINUS */
	{0x22A0, 0x22A0, 2}, /* SQUARED TIMES */
	{0x22A1, 0x22A1, 2}, /* SQUARED DOT OPERATOR */
	{0x22A2, 0x22A2, 2}, /* RIGHT TACK */
	{0x22A3, 0x22A3, 2}, /* LEFT TACK */
	{0x22A4, 0x22A4, 2}, /* DOWN TACK */
	{0x22A5, 0x22A5, 2}, /* UP TACK */
	{0x22A7, 0x22A7, 2}, /* MODELS */
	{0x22A8, 0x22A8, 2}, /* TRUE */
	{0x22A9, 0x22A9, 2}, /* FORCES */
	{0x22AA, 0x22AA, 2}, /* TRIPLE VERTICAL BAR RIGHT TURNSTILE */
	{0x22AB, 0x22AB, 2}, /* DOUBLE VERTICAL BAR DOUBLE RIGHT TURNSTILE */
	{0x22AC, 0x22AC, 2}, /* DOES NOT PROVE */
	{0x22AD, 0x22AD, 2}, /* NOT TRUE */
	{0x22AE, 0x22AE, 2}, /* DOES NOT FORCE */
	{0x22AF, 0x22AF, 2}, /* NEGATED DOUBLE VERTICAL BAR DOUBLE RIGHT TURNSTILE */
	{0x22B0, 0x22B0, 2}, /* PRECEDES UNDER RELATION */
	{0x22B2, 0x22B2, 2}, /* NORMAL SUBGROUP OF */
	{0x22B3, 0x22B3, 2}, /* CONTAINS AS NORMAL SUBGROUP */
	{0x22B4, 0x22B4, 2}, /* NORMAL SUBGROUP OF OR EQUAL TO */
	{0x22B5, 0x22B5, 2}, /* CONTAINS AS NORMAL SUBGROUP OR EQUAL TO */
	{0x22B6, 0x22B6, 2}, /* ORIGINAL OF */
	{0x22B7, 0x22B7, 2}, /* IMAGE OF */
	{0x22B8, 0x22B8, 2}, /* MULTIMAP */
	{0x22B9, 0x22B9, 2}, /* HERMITIAN CONJUGATE MATRIX */
	{0x22BA, 0x22BA, 2}, /* INTERCALATE */
	{0x22BB, 0x22BB, 2}, /* XOR */
	{0x22BD, 0x22BD, 2}, /* NOR */
	{0x22BE, 0x22BE, 2}, /* RIGHT ANGLE WITH ARC */
	{0x22BF, 0x22BF, 2}, /* RIGHT TRIANGLE */
	{0x22C0, 0x22C0, 2}, /* N-ARY LOGICAL AND */
	{0x22C1, 0x22C1, 2}, /* N-ARY LOGICAL OR */
	{0x22C2, 0x22C2, 2}, /* N-ARY INTERSECTION */
	{0x22C3, 0x22C3, 2}, /* N-ARY UNION */
	{0x22C4, 0x22C4, 2}, /* DIAMOND OPERATOR */
	{0x22C5, 0x22C5, 2}, /* DOT OPERATOR */
	{0x22C6, 0x22C6, 2}, /* STAR OPERATOR */
	{0x22C7, 0x22C7, 2}, /* DIVISION TIMES */
	{0x22C8, 0x22C8, 2}, /* BOWTIE */
	{0x22C9, 0x22C9, 2}, /* LEFT NORMAL FACTOR SEMIDIRECT PRODUCT */
	{0x22CA, 0x22CA, 2}, /* RIGHT NORMAL FACTOR SEMIDIRECT PRODUCT */
	{0x22CB, 0x22CB, 2}, /* LEFT SEMIDIRECT PRODUCT */
	{0x22CC, 0x22CC, 2}, /* RIGHT SEMIDIRECT PRODUCT */
	{0x22CD, 0x22CD, 2}, /* REVERSED TILDE EQUALS */
	{0x22CE, 0x22CE, 2}, /* CURLY LOGICAL OR */
	{0x22CF, 0x22CF, 2}, /* CURLY LOGICAL AND */
	{0x22D0, 0x22D0, 2}, /* DOUBLE SUBSET */
	{0x22D1, 0x22D1, 2}, /* DOUBLE SUPERSET */
	{0x22D2, 0x22D2, 2}, /* DOUBLE INTERSECTION */
	{0x22D3, 0x22D3, 2}, /* DOUBLE UNION */
	{0x22D4, 0x22D4, 2}, /* PITCHFORK */
	{0x22D5, 0x22D5, 2}, /* EQUAL AND PARALLEL TO */
	{0x22D6, 0x22D6, 2}, /* LESS-THAN WITH DOT */
	{0x22D7, 0x22D7, 2}, /* GREATER-THAN WITH DOT */
	{0x22D8, 0x22D8, 2}, /* VERY MUCH LESS-THAN */
	{0x22D9, 0x22D9, 2}, /* VERY MUCH GREATER-THAN */
	{0x22DA, 0x22DA, 2}, /* LESS-THAN EQUAL TO OR GREATER-THAN */
	{0x22DB, 0x22DB, 2}, /* GREATER-THAN EQUAL TO OR LESS-THAN */
	{0x22DE, 0x22DE, 2}, /* EQUAL TO OR PRECEDES */
	{0x22DF, 0x22DF, 2}, /* EQUAL TO OR SUCCEEDS */
	{0x22E0, 0x22E0, 2}, /* DOES NOT PRECEDE OR EQUAL */
	{0x22E1, 0x22E1, 2}, /* DOES NOT SUCCEED OR EQUAL */
	{0x22E2, 0x22E2, 2}, /* NOT SQUARE IMAGE OF OR EQUAL TO */
	{0x22E3, 0x22E3, 2}, /* NOT SQUARE ORIGINAL OF OR EQUAL TO */
	{0x22E6, 0x22E6, 2}, /* LESS-THAN BUT NOT EQUIVALENT TO */
	{0x22E7, 0x22E7, 2}, /* GREATER-THAN BUT NOT EQUIVALENT TO */
	{0x22E8, 0x22E8, 2}, /* PRECEDES BUT NOT EQUIVALENT TO */
	{0x22E9, 0x22E9, 2}, /* SUCCEEDS BUT NOT EQUIVALENT TO */
	{0x22EA, 0x22EA, 2}, /* NOT NORMAL SUBGROUP OF */
	{0x22EB, 0x22EB, 2}, /* DOES NOT CONTAIN AS NORMAL SUBGROUP */
	{0x22EC, 0x22EC, 2}, /* NOT NORMAL SUBGROUP OF OR EQUAL TO */
	{0x22ED, 0x22ED, 2}, /* DOES NOT CONTAIN AS NORMAL SUBGROUP OR EQUAL */
	{0x22EE, 0x22EE, 2}, /* VERTICAL ELLIPSIS */
	{0x22EF, 0x22EF, 2}, /* MIDLINE HORIZONTAL ELLIPSIS */
	{0x22F0, 0x22F0, 2}, /* UP RIGHT DIAGONAL ELLIPSIS */
	{0x22F1, 0x22F1, 2}, /* DOWN RIGHT DIAGONAL ELLIPSIS */
	{0x22F2, 0x22F2, 2}, /* ELEMENT OF WITH LONG HORIZONTAL STROKE */
	{0x22F3, 0x22F3, 2}, /* ELEMENT OF WITH VERTICAL BAR AT END OF HORIZONTAL STROKE */
	{0x22F4, 0x22F4, 2}, /* SMALL ELEMENT OF WITH VERTICAL BAR AT END OF HORIZONTAL STROKE */
	{0x22F5, 0x22F5, 2}, /* ELEMENT OF WITH DOT ABOVE */
	{0x22F6, 0x22F6, 2}, /* ELEMENT OF WITH OVERBAR */
	{0x22F7, 0x22F7, 2}, /* SMALL ELEMENT OF WITH OVERBAR */
	{0x22F9, 0x22F9, 2}, /* ELEMENT OF WITH TWO HORIZONTAL STROKES */
	{0x22FA, 0x22FA, 2}, /* CONTAINS WITH LONG HORIZONTAL STROKE */
	{0x22FB, 0x22FB, 2}, /* CONTAINS WITH VERTICAL BAR AT END OF HORIZONTAL STROKE */
	{0x22FC, 0x22FC, 2}, /* SMALL CONTAINS WITH VERTICAL BAR AT END OF HORIZONTAL STROKE */
	{0x22FD, 0x22FD, 2}, /* CONTAINS WITH OVERBAR */
	{0x22FE, 0x22FE, 2}, /* SMALL CONTAINS WITH OVERBAR */
	{0x2305, 0x2305, 2}, /* PROJECTIVE */
	{0x2306, 0x2306, 2}, /* PERSPECTIVE */
	{0x2308, 0x2308, 2}, /* LEFT CEILING */
	{0x2309, 0x2309, 2}, /* RIGHT CEILING */
	{0x230A, 0x230A, 2}, /* LEFT FLOOR */
	{0x230B, 0x230B, 2}, /* RIGHT FLOOR */
	{0x230C, 0x230C, 2}, /* BOTTOM RIGHT CROP */
	{0x230D, 0x230D, 2}, /* BOTTOM LEFT CROP */
	{0x230E, 0x230E, 2}, /* TOP RIGHT CROP */
	{0x230F, 0x230F, 2}, /* TOP LEFT CROP */
	{0x2310, 0x2310, 2}, /* REVERSED NOT SIGN */
	{0x2312, 0x2312, 2}, /* ARC */
	{0x2313, 0x2313, 2}, /* SEGMENT */
	{0x2315, 0x2315, 2}, /* TELEPHONE RECORDER */
	{0x2316, 0x2316, 2}, /* POSITION INDICATOR */
	{0x231C, 0x231C, 2}, /* TOP LEFT CORNER */
	{0x231D, 0x231D, 2}, /* TOP RIGHT CORNER */
	{0x231E, 0x231E, 2}, /* BOTTOM LEFT CORNER */
	{0x231F, 0x231F, 2}, /* BOTTOM RIGHT CORNER */
	{0x2322, 0x2322, 2}, /* FROWN */
	{0x2323, 0x2323, 2}, /* SMILE */
	{0x2329, 0x2329, 2}, /* LEFT-POINTING ANGLE BRACKET */
	{0x232A, 0x232A, 2}, /* RIGHT-POINTING ANGLE BRACKET */
	{0x232D, 0x232D, 2}, /* CYLINDRICITY */
	{0x232E, 0x232E, 2}, /* ALL AROUND-PROFILE */
	{0x2336, 0x2336, 2}, /* APL FUNCTIONAL SYMBOL I-BEAM */
	{0x233D, 0x233D, 2}, /* APL FUNCTIONAL SYMBOL CIRCLE STILE */
	{0x233F, 0x233F, 2}, /* APL FUNCTIONAL SYMBOL SLASH BAR */
	{0x237C, 0x237C, 2}, /* RIGHT ANGLE WITH DOWNWARDS ZIGZAG ARROW */
	{0x23B0, 0x23B0, 2}, /* UPPER LEFT OR LOWER RIGHT CURLY BRACKET SECTION */
	{0x23B1, 0x23B1, 2}, /* UPPER RIGHT OR LOWER LEFT CURLY BRACKET SECTION */
	{0x23B4, 0x23B4, 2}, /* TOP SQUARE BRACKET */
	{0x23B5, 0x23B5, 2}, /* BOTTOM SQUARE BRACKET */
	{0x23B6, 0x23B6, 2}, /* BOTTOM SQUARE BRACKET OVER TOP SQUARE BRACKET */
	{0x2423, 0x2423, 2}, /* OPEN BOX */
	{0x24C8, 0x24C8, 2}, /* CIRCLED LATIN CAPITAL LETTER S */
	{0x2500, 0x2500, 2}, /* BOX DRAWINGS LIGHT HORIZONTAL */
	{0x2502, 0x2502, 2}, /* BOX DRAWINGS LIGHT VERTICAL */
	{0x250C, 0x250C, 2}, /* BOX DRAWINGS LIGHT DOWN AND RIGHT */
	{0x2510, 0x2510, 2}, /* BOX DRAWINGS LIGHT DOWN AND LEFT */
	{0x2514, 0x2514, 2}, /* BOX DRAWINGS LIGHT UP AND RIGHT */
	{0x2518, 0x2518, 2}, /* BOX DRAWINGS LIGHT UP AND LEFT */
	{0x251C, 0x251C, 2}, /* BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
	{0x2524, 0x2524, 2}, /* BOX DRAWINGS LIGHT VERTICAL AND LEFT */
	{0x252C, 0x252C, 2}, /* BOX DRAWINGS LIGHT DOWN AND HORIZONTAL */
	{0x2534, 0x2534, 2}, /* BOX DRAWINGS LIGHT UP AND HORIZONTAL */
	{0x253C, 0x253C, 2}, /* BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL */
	{0x2550, 0x2550, 2}, /* BOX DRAWINGS DOUBLE HORIZONTAL */
	{0x2551, 0x2551, 2}, /* BOX DRAWINGS DOUBLE VERTICAL */
	{0x2552, 0x2552, 2}, /* BOX DRAWINGS DOWN SINGLE AND RIGHT DOUBLE */
	{0x2553, 0x2553, 2}, /* BOX DRAWINGS DOWN DOUBLE AND RIGHT SINGLE */
	{0x2554, 0x2554, 2}, /* BOX DRAWINGS DOUBLE DOWN AND RIGHT */
	{0x2555, 0x2555, 2}, /* BOX DRAWINGS DOWN SINGLE AND LEFT DOUBLE */
	{0x2556, 0x2556, 2}, /* BOX DRAWINGS DOWN DOUBLE AND LEFT SINGLE */
	{0x2557, 0x2557, 2}, /* BOX DRAWINGS DOUBLE DOWN AND LEFT */
	{0x2558, 0x2558, 2}, /* BOX DRAWINGS UP SINGLE AND RIGHT DOUBLE */
	{0x2559, 0x2559, 2}, /* BOX DRAWINGS UP DOUBLE AND RIGHT SINGLE */
	{0x255A, 0x255A, 2}, /* BOX DRAWINGS DOUBLE UP AND RIGHT */
	{0x255B, 0x255B, 2}, /* BOX DRAWINGS UP SINGLE AND LEFT DOUBLE */
	{0x255C, 0x255C, 2}, /* BOX DRAWINGS UP DOUBLE AND LEFT SINGLE */
	{0x255D, 0x255D, 2}, /* BOX DRAWINGS DOUBLE UP AND LEFT */
	{0x255E, 0x255E, 2}, /* BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE */
	{0x255F, 0x255F, 2}, /* BOX DRAWINGS VERTICAL DOUBLE AND RIGHT SINGLE */
	{0x2560, 0x2560, 2}, /* BOX DRAWINGS DOUBLE VERTICAL AND RIGHT */
	{0x2561, 0x2561, 2}, /* BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE */
	{0x2562, 0x2562, 2}, /* BOX DRAWINGS VERTICAL DOUBLE AND LEFT SINGLE */
	{0x2563, 0x2563, 2}, /* BOX DRAWINGS DOUBLE VERTICAL AND LEFT */
	{0x2564, 0x2564, 2}, /* BOX DRAWINGS DOWN SINGLE AND HORIZONTAL DOUBLE */
	{0x2565, 0x2565, 2}, /* BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE */
	{0x2566, 0x2566, 2}, /* BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL */
	{0x2567, 0x2567, 2}, /* BOX DRAWINGS UP SINGLE AND HORIZONTAL DOUBLE */
	{0x2568, 0x2568, 2}, /* BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE */
	{0x2569, 0x2569, 2}, /* BOX DRAWINGS DOUBLE UP AND HORIZONTAL */
	{0x256A, 0x256A, 2}, /* BOX DRAWINGS VERTICAL SINGLE AND HORIZONTAL DOUBLE */
	{0x256B, 0x256B, 2}, /* BOX DRAWINGS VERTICAL DOUBLE AND HORIZONTAL SINGLE */
	{0x256C, 0x256C, 2}, /* BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL */
	{0x2580, 0x2580, 2}, /* UPPER HALF BLOCK */
	{0x2584, 0x2584, 2}, /* LOWER HALF BLOCK */
	{0x2588, 0x2588, 2}, /* FULL BLOCK */
	{0x2591, 0x2591, 2}, /* LIGHT SHADE */
	{0x2592, 0x2592, 2}, /* MEDIUM SHADE */
	{0x2593, 0x2593, 2}, /* DARK SHADE */
	{0x25A1, 0x25A1, 2}, /* WHITE SQUARE */
	{0x25AA, 0x25AA, 2}, /* BLACK SMALL SQUARE */
	{0x25AB, 0x25AB, 2}, /* WHITE SMALL SQUARE */
	{0x25AD, 0x25AD, 2}, /* WHITE RECTANGLE */
	{0x25AE, 0x25AE, 2}, /* BLACK VERTICAL RECTANGLE */
	{0x25B1, 0x25B1, 2}, /* WHITE PARALLELOGRAM */
	{0x25B3, 0x25B3, 2}, /* WHITE UP-POINTING TRIANGLE */
	{0x25B4, 0x25B4, 2}, /* BLACK UP-POINTING SMALL TRIANGLE */
	{0x25B5, 0x25B5, 2}, /* WHITE UP-POINTING SMALL TRIANGLE */
	{0x25B8, 0x25B8, 2}, /* BLACK RIGHT-POINTING SMALL TRIANGLE */
	{0x25B9, 0x25B9, 2}, /* WHITE RIGHT-POINTING SMALL TRIANGLE */
	{0x25BD, 0x25BD, 2}, /* WHITE DOWN-POINTING TRIANGLE */
	{0x25BE, 0x25BE, 2}, /* BLACK DOWN-POINTING SMALL TRIANGLE */
	{0x25BF, 0x25BF, 2}, /* WHITE DOWN-POINTING SMALL TRIANGLE */
	{0x25C2, 0x25C2, 2}, /* BLACK LEFT-POINTING SMALL TRIANGLE */
	{0x25C3, 0x25C3, 2}, /* WHITE LEFT-POINTING SMALL TRIANGLE */
	{0x25CA, 0x25CA, 2}, /* LOZENGE */
	{0x25CB, 0x25CB, 2}, /* WHITE CIRCLE */
	{0x25EC, 0x25EC, 2}, /* WHITE UP-POINTING TRIANGLE WITH DOT */
	{0x25EF, 0x25EF, 2}, /* LARGE CIRCLE */
	{0x25F8, 0x25F8, 2}, /* UPPER LEFT TRIANGLE */
	{0x25F9, 0x25F9, 2}, /* UPPER RIGHT TRIANGLE */
	{0x25FA, 0x25FA, 2}, /* LOWER LEFT TRIANGLE */
	{0x25FB, 0x25FB, 2}, /* WHITE MEDIUM SQUARE */
	{0x25FC, 0x25FC, 2}, /* BLACK MEDIUM SQUARE */
	{0x2605, 0x2605, 2}, /* BLACK STAR */
	{0x2606, 0x2606, 2}, /* WHITE STAR */
	{0x260E, 0x260E, 2}, /* BLACK TELEPHONE */
	{0x2640, 0x2640, 2}, /* FEMALE SIGN */
	{0x2642, 0x2642, 2}, /* MALE SIGN */
	{0x2660, 0x2660, 2}, /* BLACK SPADE SUIT */
	{0x2663, 0x2663, 2}, /* BLACK CLUB SUIT */
	{0x2665, 0x2665, 2}, /* BLACK HEART SUIT */
	{0x2666, 0x2666, 2}, /* BLACK DIAMOND SUIT */
	{0x266A, 0x266A, 2}, /* EIGHTH NOTE */
	{0x266D, 0x266D, 2}, /* MUSIC FLAT SIGN */
	{0x266E, 0x266E, 2}, /* MUSIC NATURAL SIGN */
	{0x266F, 0x266F, 2}, /* MUSIC SHARP SIGN */
	{0x2713, 0x2713, 2}, /* CHECK MARK */
	{0x2717, 0x2717, 2}, /* BALLOT X */
	{0x2720, 0x2720, 2}, /* MALTESE CROSS */
	{0x2736, 0x2736, 2}, /* SIX POINTED BLACK STAR */
	{0x2758, 0x2758, 2}, /* LIGHT VERTICAL BAR */
	{0x27F5, 0x27F5, 2}, /* LONG LEFTWARDS ARROW */
	{0x27F6, 0x27F6, 2}, /* LONG RIGHTWARDS ARROW */
	{0x27F7, 0x27F7, 2}, /* LONG LEFT RIGHT ARROW */
	{0x27F8, 0x27F8, 2}, /* LONG LEFTWARDS DOUBLE ARROW */
	{0x27F9, 0x27F9, 2}, /* LONG RIGHTWARDS DOUBLE ARROW */
	{0x27FA, 0x27FA, 2}, /* LONG LEFT RIGHT DOUBLE ARROW */
	{0x27FC, 0x27FC, 2}, /* LONG RIGHTWARDS ARROW FROM BAR */
	{0x27FF, 0x27FF, 2}, /* LONG RIGHTWARDS SQUIGGLE ARROW */
	{0x2902, 0x2902, 2}, /* LEFTWARDS DOUBLE ARROW WITH VERTICAL STROKE */
	{0x2903, 0x2903, 2}, /* RIGHTWARDS DOUBLE ARROW WITH VERTICAL STROKE */
	{0x2904, 0x2904, 2}, /* LEFT RIGHT DOUBLE ARROW WITH VERTICAL STROKE */
	{0x2905, 0x2905, 2}, /* RIGHTWARDS TWO-HEADED ARROW FROM BAR */
	{0x290C, 0x290C, 2}, /* LEFTWARDS DOUBLE DASH ARROW */
	{0x290D, 0x290D, 2}, /* RIGHTWARDS DOUBLE DASH ARROW */
	{0x290E, 0x290E, 2}, /* LEFTWARDS TRIPLE DASH ARROW */
	{0x290F, 0x290F, 2}, /* RIGHTWARDS TRIPLE DASH ARROW */
	{0x2910, 0x2910, 2}, /* RIGHTWARDS TWO-HEADED TRIPLE DASH ARROW */
	{0x2911, 0x2911, 2}, /* RIGHTWARDS ARROW WITH DOTTED STEM */
	{0x2912, 0x2912, 2}, /* UPWARDS ARROW TO BAR */
	{0x2913, 0x2913, 2}, /* DOWNWARDS ARROW TO BAR */
	{0x2916, 0x2916, 2}, /* RIGHTWARDS TWO-HEADED ARROW WITH TAIL */
	{0x2919, 0x2919, 2}, /* LEFTWARDS ARROW-TAIL */
	{0x291A, 0x291A, 2}, /* RIGHTWARDS ARROW-TAIL */
	{0x291B, 0x291B, 2}, /* LEFTWARDS DOUBLE ARROW-TAIL */
	{0x291C, 0x291C, 2}, /* RIGHTWARDS DOUBLE ARROW-TAIL */
	{0x291D, 0x291D, 2}, /* LEFTWARDS ARROW TO BLACK DIAMOND */
	{0x291E, 0x291E, 2}, /* RIGHTWARDS ARROW TO BLACK DIAMOND */
	{0x291F, 0x291F, 2}, /* LEFTWARDS ARROW FROM BAR TO BLACK DIAMOND */
	{0x2920, 0x2920, 2}, /* RIGHTWARDS ARROW FROM BAR TO BLACK DIAMOND */
	{0x2923, 0x2923, 2}, /* NORTH WEST ARROW WITH HOOK */
	{0x2924, 0x2924, 2}, /* NORTH EAST ARROW WITH HOOK */
	{0x2925, 0x2925, 2}, /* SOUTH EAST ARROW WITH HOOK */
	{0x2926, 0x2926, 2}, /* SOUTH WEST ARROW WITH HOOK */
	{0x2927, 0x2927, 2}, /* NORTH WEST ARROW AND NORTH EAST ARROW */
	{0x2928, 0x2928, 2}, /* NORTH EAST ARROW AND SOUTH EAST ARROW */
	{0x2929, 0x2929, 2}, /* SOUTH EAST ARROW AND SOUTH WEST ARROW */
	{0x292A, 0x292A, 2}, /* SOUTH WEST ARROW AND NORTH WEST ARROW */
	{0x2933, 0x2933, 2}, /* WAVE ARROW POINTING DIRECTLY RIGHT */
	{0x2935, 0x2935, 2}, /* ARROW POINTING RIGHTWARDS THEN CURVING DOWNWARDS */
	{0x2936, 0x2936, 2}, /* ARROW POINTING DOWNWARDS THEN CURVING LEFTWARDS */
	{0x2937, 0x2937, 2}, /* ARROW POINTING DOWNWARDS THEN CURVING RIGHTWARDS */
	{0x2938, 0x2938, 2}, /* RIGHT-SIDE ARC CLOCKWISE ARROW */
	{0x2939, 0x2939, 2}, /* LEFT-SIDE ARC ANTICLOCKWISE ARROW */
	{0x293C, 0x293C, 2}, /* TOP ARC CLOCKWISE ARROW WITH MINUS */
	{0x293D, 0x293D, 2}, /* TOP ARC ANTICLOCKWISE ARROW WITH PLUS */
	{0x2945, 0x2945, 2}, /* RIGHTWARDS ARROW WITH PLUS BELOW */
	{0x2948, 0x2948, 2}, /* LEFT RIGHT ARROW THROUGH SMALL CIRCLE */
	{0x2949, 0x2949, 2}, /* UPWARDS TWO-HEADED ARROW FROM SMALL CIRCLE */
	{0x294A, 0x294A, 2}, /* LEFT BARB UP RIGHT BARB DOWN HARPOON */
	{0x294B, 0x294B, 2}, /* LEFT BARB DOWN RIGHT BARB UP HARPOON */
	{0x294E, 0x294E, 2}, /* LEFT BARB UP RIGHT BARB UP HARPOON */
	{0x294F, 0x294F, 2}, /* UP BARB RIGHT DOWN BARB RIGHT HARPOON */
	{0x2950, 0x2950, 2}, /* LEFT BARB DOWN RIGHT BARB DOWN HARPOON */
	{0x2951, 0x2951, 2}, /* UP BARB LEFT DOWN BARB LEFT HARPOON */
	{0x2952, 0x2952, 2}, /* LEFTWARDS HARPOON WITH BARB UP TO BAR */
	{0x2953, 0x2953, 2}, /* RIGHTWARDS HARPOON WITH BARB UP TO BAR */
	{0x2954, 0x2954, 2}, /* UPWARDS HARPOON WITH BARB RIGHT TO BAR */
	{0x2955, 0x2955, 2}, /* DOWNWARDS HARPOON WITH BARB RIGHT TO BAR */
	{0x2956, 0x2956, 2}, /* LEFTWARDS HARPOON WITH BARB DOWN TO BAR */
	{0x2957, 0x2957, 2}, /* RIGHTWARDS HARPOON WITH BARB DOWN TO BAR */
	{0x2958, 0x2958, 2}, /* UPWARDS HARPOON WITH BARB LEFT TO BAR */
	{0x2959, 0x2959, 2}, /* DOWNWARDS HARPOON WITH BARB LEFT TO BAR */
	{0x295A, 0x295A, 2}, /* LEFTWARDS HARPOON WITH BARB UP FROM BAR */
	{0x295B, 0x295B, 2}, /* RIGHTWARDS HARPOON WITH BARB UP FROM BAR */
	{0x295C, 0x295C, 2}, /* UPWARDS HARPOON WITH BARB RIGHT FROM BAR */
	{0x295D, 0x295D, 2}, /* DOWNWARDS HARPOON WITH BARB RIGHT FROM BAR */
	{0x295E, 0x295E, 2}, /* LEFTWARDS HARPOON WITH BARB DOWN FROM BAR */
	{0x295F, 0x295F, 2}, /* RIGHTWARDS HARPOON WITH BARB DOWN FROM BAR */
	{0x2960, 0x2960, 2}, /* UPWARDS HARPOON WITH BARB LEFT FROM BAR */
	{0x2961, 0x2961, 2}, /* DOWNWARDS HARPOON WITH BARB LEFT FROM BAR */
	{0x2962, 0x2962, 2}, /* LEFTWARDS HARPOON WITH BARB UP ABOVE LEFTWARDS HARPOON WITH BARB DOWN */
	{0x2963, 0x2963, 2}, /* UPWARDS HARPOON WITH BARB LEFT BESIDE UPWARDS HARPOON WITH BARB RIGHT */
	{0x2964, 0x2964, 2}, /* RIGHTWARDS HARPOON WITH BARB UP ABOVE RIGHTWARDS HARPOON WITH BARB DOWN */
	{0x2965, 0x2965, 2}, /* DOWNWARDS HARPOON WITH BARB LEFT BESIDE DOWNWARDS HARPOON WITH BARB RIGHT */
	{0x2966, 0x2966, 2}, /* LEFTWARDS HARPOON WITH BARB UP ABOVE RIGHTWARDS HARPOON WITH BARB UP */
	{0x2967, 0x2967, 2}, /* LEFTWARDS HARPOON WITH BARB DOWN ABOVE RIGHTWARDS HARPOON WITH BARB DOWN */
	{0x2968, 0x2968, 2}, /* RIGHTWARDS HARPOON WITH BARB UP ABOVE LEFTWARDS HARPOON WITH BARB UP */
	{0x2969, 0x2969, 2}, /* RIGHTWARDS HARPOON WITH BARB DOWN ABOVE LEFTWARDS HARPOON WITH BARB DOWN */
	{0x296A, 0x296A, 2}, /* LEFTWARDS HARPOON WITH BARB UP ABOVE LONG DASH */
	{0x296B, 0x296B, 2}, /* LEFTWARDS HARPOON WITH BARB DOWN BELOW LONG DASH */
	{0x296C, 0x296C, 2}, /* RIGHTWARDS HARPOON WITH BARB UP ABOVE LONG DASH */
	{0x296D, 0x296D, 2}, /* RIGHTWARDS HARPOON WITH BARB DOWN BELOW LONG DASH */
	{0x296E, 0x296E, 2}, /* UPWARDS HARPOON WITH BARB LEFT BESIDE DOWNWARDS HARPOON WITH BARB RIGHT */
	{0x296F, 0x296F, 2}, /* DOWNWARDS HARPOON WITH BARB LEFT BESIDE UPWARDS HARPOON WITH BARB RIGHT */
	{0x2970, 0x2970, 2}, /* RIGHT DOUBLE ARROW WITH ROUNDED HEAD */
	{0x2971, 0x2971, 2}, /* EQUALS SIGN ABOVE RIGHTWARDS ARROW */
	{0x2972, 0x2972, 2}, /* TILDE OPERATOR ABOVE RIGHTWARDS ARROW */
	{0x2973, 0x2973, 2}, /* LEFTWARDS ARROW ABOVE TILDE OPERATOR */
	{0x2974, 0x2974, 2}, /* RIGHTWARDS ARROW ABOVE TILDE OPERATOR */
	{0x2975, 0x2975, 2}, /* RIGHTWARDS ARROW ABOVE ALMOST EQUAL TO */
	{0x2976, 0x2976, 2}, /* LESS-THAN ABOVE LEFTWARDS ARROW */
	{0x2978, 0x2978, 2}, /* GREATER-THAN ABOVE RIGHTWARDS ARROW */
	{0x2979, 0x2979, 2}, /* SUBSET ABOVE RIGHTWARDS ARROW */
	{0x297B, 0x297B, 2}, /* SUPERSET ABOVE LEFTWARDS ARROW */
	{0x297C, 0x297C, 2}, /* LEFT FISH TAIL */
	{0x297D, 0x297D, 2}, /* RIGHT FISH TAIL */
	{0x297E, 0x297E, 2}, /* UP FISH TAIL */
	{0x297F, 0x297F, 2}, /* DOWN FISH TAIL */
	{0x2985, 0x2985, 2}, /* LEFT WHITE PARENTHESIS */
	{0x2986, 0x2986, 2}, /* RIGHT WHITE PARENTHESIS */
	{0x298B, 0x298B, 2}, /* LEFT SQUARE BRACKET WITH UNDERBAR */
	{0x298C, 0x298C, 2}, /* RIGHT SQUARE BRACKET WITH UNDERBAR */
	{0x298D, 0x298D, 2}, /* LEFT SQUARE BRACKET WITH TICK IN TOP CORNER */
	{0x298E, 0x298E, 2}, /* RIGHT SQUARE BRACKET WITH TICK IN BOTTOM CORNER */
	{0x298F, 0x298F, 2}, /* LEFT SQUARE BRACKET WITH TICK IN BOTTOM CORNER */
	{0x2990, 0x2990, 2}, /* RIGHT SQUARE BRACKET WITH TICK IN TOP CORNER */
	{0x2991, 0x2991, 2}, /* LEFT ANGLE BRACKET WITH DOT */
	{0x2992, 0x2992, 2}, /* RIGHT ANGLE BRACKET WITH DOT */
	{0x2993, 0x2993, 2}, /* LEFT ARC LESS-THAN BRACKET */
	{0x2994, 0x2994, 2}, /* RIGHT ARC GREATER-THAN BRACKET */
	{0x2995, 0x2995, 2}, /* DOUBLE LEFT ARC GREATER-THAN BRACKET */
	{0x2996, 0x2996, 2}, /* DOUBLE RIGHT ARC LESS-THAN BRACKET */
	{0x299A, 0x299A, 2}, /* VERTICAL ZIGZAG LINE */
	{0x299C, 0x299C, 2}, /* RIGHT ANGLE VARIANT WITH SQUARE */
	{0x299D, 0x299D, 2}, /* MEASURED RIGHT ANGLE WITH DOT */
	{0x29A4, 0x29A4, 2}, /* ANGLE WITH UNDERBAR */
	{0x29A5, 0x29A5, 2}, /* REVERSED ANGLE WITH UNDERBAR */
	{0x29A6, 0x29A6, 2}, /* OBLIQUE ANGLE OPENING UP */
	{0x29A7, 0x29A7, 2}, /* OBLIQUE ANGLE OPENING DOWN */
	{0x29A8, 0x29A8, 2}, /* MEASURED ANGLE WITH OPEN ARM ENDING IN ARROW POINTING UP AND RIGHT */
	{0x29A9, 0x29A9, 2}, /* MEASURED ANGLE WITH OPEN ARM ENDING IN ARROW POINTING UP AND LEFT */
	{0x29AA, 0x29AA, 2}, /* MEASURED ANGLE WITH OPEN ARM ENDING IN ARROW POINTING DOWN AND RIGHT */
	{0x29AB, 0x29AB, 2}, /* MEASURED ANGLE WITH OPEN ARM ENDING IN ARROW POINTING DOWN AND LEFT */
	{0x29AC, 0x29AC, 2}, /* MEASURED ANGLE WITH OPEN ARM ENDING IN ARROW POINTING RIGHT AND UP */
	{0x29AD, 0x29AD, 2}, /* MEASURED ANGLE WITH OPEN ARM ENDING IN ARROW POINTING LEFT AND UP */
	{0x29AE, 0x29AE, 2}, /* MEASURED ANGLE WITH OPEN ARM ENDING IN ARROW POINTING RIGHT AND DOWN */
	{0x29AF, 0x29AF, 2}, /* MEASURED ANGLE WITH OPEN ARM ENDING IN ARROW POINTING LEFT AND DOWN */
	{0x29B0, 0x29B0, 2}, /* REVERSED EMPTY SET */
	{0x29B1, 0x29B1, 2}, /* EMPTY SET WITH OVERBAR */
	{0x29B2, 0x29B2, 2}, /* EMPTY SET WITH SMALL CIRCLE ABOVE */
	{0x29B3, 0x29B3, 2}, /* EMPTY SET WITH RIGHT ARROW ABOVE */
	{0x29B4, 0x29B4, 2}, /* EMPTY SET WITH LEFT ARROW ABOVE */
	{0x29B5, 0x29B5, 2}, /* CIRCLE WITH HORIZONTAL BAR */
	{0x29B6, 0x29B6, 2}, /* CIRCLED VERTICAL BAR */
	{0x29B7, 0x29B7, 2}, /* CIRCLED PARALLEL */
	{0x29B9, 0x29B9, 2}, /* CIRCLED PERPENDICULAR */
	{0x29BB, 0x29BB, 2}, /* CIRCLE WITH SUPERIMPOSED X */
	{0x29BC, 0x29BC, 2}, /* CIRCLED ANTICLOCKWISE-ROTATED DIVISION SIGN */
	{0x29BE, 0x29BE, 2}, /* CIRCLED WHITE BULLET */
	{0x29BF, 0x29BF, 2}, /* CIRCLED BULLET */
	{0x29C0, 0x29C0, 2}, /* CIRCLED LESS-THAN */
	{0x29C1, 0x29C1, 2}, /* CIRCLED GREATER-THAN */
	{0x29C2, 0x29C2, 2}, /* CIRCLE WITH SMALL CIRCLE TO THE RIGHT */
	{0x29C3, 0x29C3, 2}, /* CIRCLE WITH TWO HORIZONTAL STROKES TO THE RIGHT */
	{0x29C4, 0x29C4, 2}, /* SQUARED RISING DIAGONAL SLASH */
	{0x29C5, 0x29C5, 2}, /* SQUARED FALLING DIAGONAL SLASH */
	{0x29C9, 0x29C9, 2}, /* TWO JOINED SQUARES */
	{0x29CD, 0x29CD, 2}, /* TRIANGLE WITH SERIFS AT BOTTOM */
	{0x29CE, 0x29CE, 2}, /* RIGHT TRIANGLE ABOVE LEFT TRIANGLE */
	{0x29CF, 0x29CF, 2}, /* LEFT TRIANGLE BESIDE VERTICAL BAR */
	{0x29D0, 0x29D0, 2}, /* VERTICAL BAR BESIDE RIGHT TRIANGLE */
	{0x29DA, 0x29DA, 2}, /* LEFT DOUBLE WIGGLY FENCE */
	{0x29DC, 0x29DC, 2}, /* INCOMPLETE INFINITY */
	{0x29DD, 0x29DD, 2}, /* TIE OVER INFINITY */
	{0x29DE, 0x29DE, 2}, /* INFINITY NEGATED WITH VERTICAL BAR */
	{0x29E3, 0x29E3, 2}, /* EQUALS SIGN AND SLANTED PARALLEL */
	{0x29E4, 0x29E4, 2}, /* EQUALS SIGN AND SLANTED PARALLEL WITH TILDE ABOVE */
	{0x29E5, 0x29E5, 2}, /* IDENTICAL TO AND SLANTED PARALLEL */
	{0x29EB, 0x29EB, 2}, /* BLACK LOZENGE */
	{0x29F4, 0x29F4, 2}, /* RULE-DELAYED */
	{0x29F6, 0x29F6, 2}, /* SOLIDUS WITH OVERBAR */
	{0x2A00, 0x2A00, 2}, /* N-ARY CIRCLED DOT OPERATOR */
	{0x2A01, 0x2A01, 2}, /* N-ARY CIRCLED PLUS OPERATOR */
	{0x2A02, 0x2A02, 2}, /* N-ARY CIRCLED TIMES OPERATOR */
	{0x2A04, 0x2A04, 2}, /* N-ARY UNION OPERATOR WITH PLUS */
	{0x2A06, 0x2A06, 2}, /* N-ARY SQUARE UNION OPERATOR */
	{0x2A0C, 0x2A0C, 2}, /* QUADRUPLE INTEGRAL OPERATOR */
	{0x2A0D, 0x2A0D, 2}, /* FINITE PART INTEGRAL */
	{0x2A10, 0x2A10, 2}, /* CIRCULATION FUNCTION */
	{0x2A11, 0x2A11, 2}, /* ANTICLOCKWISE INTEGRATION */
	{0x2A12, 0x2A12, 2}, /* LINE INTEGRATION WITH RECTANGULAR PATH AROUND POLE */
	{0x2A13, 0x2A13, 2}, /* LINE INTEGRATION WITH SEMICIRCULAR PATH AROUND POLE */
	{0x2A14, 0x2A14, 2}, /* LINE INTEGRATION NOT INCLUDING THE POLE */
	{0x2A15, 0x2A15, 2}, /* INTEGRAL AROUND A POINT OPERATOR */
	{0x2A16, 0x2A16, 2}, /* QUATERNION INTEGRAL OPERATOR */
	{0x2A17, 0x2A17, 2}, /* INTEGRAL WITH LEFTWARDS ARROW WITH HOOK */
	{0x2A22, 0x2A22, 2}, /* PLUS SIGN WITH SMALL CIRCLE ABOVE */
	{0x2A23, 0x2A23, 2}, /* PLUS SIGN WITH CIRCUMFLEX ACCENT ABOVE */
	{0x2A24, 0x2A24, 2}, /* PLUS SIGN WITH TILDE ABOVE */
	{0x2A25, 0x2A25, 2}, /* PLUS SIGN WITH DOT BELOW */
	{0x2A26, 0x2A26, 2}, /* PLUS SIGN WITH TILDE BELOW */
	{0x2A27, 0x2A27, 2}, /* PLUS SIGN WITH SUBSCRIPT TWO */
	{0x2A29, 0x2A29, 2}, /* MINUS SIGN WITH COMMA ABOVE */
	{0x2A2A, 0x2A2A, 2}, /* MINUS SIGN WITH DOT BELOW */
	{0x2A2D, 0x2A2D, 2}, /* PLUS SIGN IN LEFT HALF CIRCLE */
	{0x2A2E, 0x2A2E, 2}, /* PLUS SIGN IN RIGHT HALF CIRCLE */
	{0x2A2F, 0x2A2F, 2}, /* VECTOR OR CROSS PRODUCT */
	{0x2A30, 0x2A30, 2}, /* MULTIPLICATION SIGN WITH DOT ABOVE */
	{0x2A31, 0x2A31, 2}, /* MULTIPLICATION SIGN WITH UNDERBAR */
	{0x2A33, 0x2A33, 2}, /* SMASH PRODUCT */
	{0x2A34, 0x2A34, 2}, /* MULTIPLICATION SIGN IN LEFT HALF CIRCLE */
	{0x2A35, 0x2A35, 2}, /* MULTIPLICATION SIGN IN RIGHT HALF CIRCLE */
	{0x2A36, 0x2A36, 2}, /* CIRCLED MULTIPLICATION SIGN WITH CIRCUMFLEX ACCENT */
	{0x2A37, 0x2A37, 2}, /* MULTIPLICATION SIGN IN DOUBLE CIRCLE */
	{0x2A38, 0x2A38, 2}, /* CIRCLED DIVISION SIGN */
	{0x2A39, 0x2A39, 2}, /* PLUS SIGN IN TRIANGLE */
	{0x2A3A, 0x2A3A, 2}, /* MINUS SIGN IN TRIANGLE */
	{0x2A3B, 0x2A3B, 2}, /* MULTIPLICATION SIGN IN TRIANGLE */
	{0x2A3C, 0x2A3C, 2}, /* INTERIOR PRODUCT */
	{0x2A3F, 0x2A3F, 2}, /* AMALGAMATION OR COPRODUCT */
	{0x2A40, 0x2A40, 2}, /* INTERSECTION WITH DOT */
	{0x2A42, 0x2A42, 2}, /* UNION WITH OVERBAR */
	{0x2A43, 0x2A43, 2}, /* INTERSECTION WITH OVERBAR */
	{0x2A44, 0x2A44, 2}, /* INTERSECTION WITH LOGICAL AND */
	{0x2A45, 0x2A45, 2}, /* UNION WITH LOGICAL OR */
	{0x2A46, 0x2A46, 2}, /* UNION ABOVE INTERSECTION */
	{0x2A47, 0x2A47, 2}, /* INTERSECTION ABOVE UNION */
	{0x2A48, 0x2A48, 2}, /* UNION ABOVE BAR ABOVE INTERSECTION */
	{0x2A49, 0x2A49, 2}, /* INTERSECTION ABOVE BAR ABOVE UNION */
	{0x2A4A, 0x2A4A, 2}, /* UNION BESIDE AND JOINED WITH UNION */
	{0x2A4B, 0x2A4B, 2}, /* INTERSECTION BESIDE AND JOINED WITH INTERSECTION */
	{0x2A4C, 0x2A4C, 2}, /* CLOSED UNION WITH SERIFS */
	{0x2A4D, 0x2A4D, 2}, /* CLOSED INTERSECTION WITH SERIFS */
	{0x2A50, 0x2A50, 2}, /* CLOSED UNION WITH SERIFS AND SMASH PRODUCT */
	{0x2A53, 0x2A53, 2}, /* DOUBLE LOGICAL AND */
	{0x2A54, 0x2A54, 2}, /* DOUBLE LOGICAL OR */
	{0x2A55, 0x2A55, 2}, /* TWO INTERSECTING LOGICAL AND */
	{0x2A56, 0x2A56, 2}, /* TWO INTERSECTING LOGICAL OR */
	{0x2A57, 0x2A57, 2}, /* SLOPING LARGE OR */
	{0x2A58, 0x2A58, 2}, /* SLOPING LARGE AND */
	{0x2A5A, 0x2A5A, 2}, /* LOGICAL AND WITH MIDDLE STEM */
	{0x2A5B, 0x2A5B, 2}, /* LOGICAL OR WITH MIDDLE STEM */
	{0x2A5C, 0x2A5C, 2}, /* LOGICAL AND WITH HORIZONTAL DASH */
	{0x2A5D, 0x2A5D, 2}, /* LOGICAL OR WITH HORIZONTAL DASH */
	{0x2A5F, 0x2A5F, 2}, /* LOGICAL AND WITH UNDERBAR */
	{0x2A66, 0x2A66, 2}, /* EQUALS SIGN WITH DOT BELOW */
	{0x2A6A, 0x2A6A, 2}, /* TILDE OPERATOR WITH DOT ABOVE */
	{0x2A6D, 0x2A6D, 2}, /* CONGRUENT WITH DOT ABOVE */
	{0x2A6E, 0x2A6E, 2}, /* EQUALS WITH ASTERISK */
	{0x2A6F, 0x2A6F, 2}, /* ALMOST EQUAL TO WITH CIRCUMFLEX ACCENT */
	{0x2A70, 0x2A70, 2}, /* APPROXIMATELY EQUAL OR EQUAL TO */
	{0x2A71, 0x2A71, 2}, /* EQUALS SIGN ABOVE PLUS SIGN */
	{0x2A72, 0x2A72, 2}, /* PLUS SIGN ABOVE EQUALS SIGN */
	{0x2A73, 0x2A73, 2}, /* EQUALS SIGN ABOVE TILDE OPERATOR */
	{0x2A74, 0x2A74, 2}, /* DOUBLE COLON EQUAL */
	{0x2A75, 0x2A75, 2}, /* TWO CONSECUTIVE EQUALS SIGNS */
	{0x2A77, 0x2A77, 2}, /* EQUALS SIGN WITH TWO DOTS ABOVE AND TWO DOTS BELOW */
	{0x2A78, 0x2A78, 2}, /* EQUIVALENT WITH FOUR DOTS ABOVE */
	{0x2A79, 0x2A79, 2}, /* LESS-THAN WITH CIRCLE INSIDE */
	{0x2A7A, 0x2A7A, 2}, /* GREATER-THAN WITH CIRCLE INSIDE */
	{0x2A7B, 0x2A7B, 2}, /* LESS-THAN WITH QUESTION MARK ABOVE */
	{0x2A7C, 0x2A7C, 2}, /* GREATER-THAN WITH QUESTION MARK ABOVE */
	{0x2A7D, 0x2A7D, 2}, /* LESS-THAN OR SLANTED EQUAL TO */
	{0x2A7E, 0x2A7E, 2}, /* GREATER-THAN OR SLANTED EQUAL TO */
	{0x2A7F, 0x2A7F, 2}, /* LESS-THAN OR SLANTED EQUAL TO WITH DOT INSIDE */
	{0x2A80, 0x2A80, 2}, /* GREATER-THAN OR SLANTED EQUAL TO WITH DOT INSIDE */
	{0x2A81, 0x2A81, 2}, /* LESS-THAN OR SLANTED EQUAL TO WITH DOT ABOVE */
	{0x2A82, 0x2A82, 2}, /* GREATER-THAN OR SLANTED EQUAL TO WITH DOT ABOVE */
	{0x2A83, 0x2A83, 2}, /* LESS-THAN OR SLANTED EQUAL TO WITH DOT ABOVE RIGHT */
	{0x2A84, 0x2A84, 2}, /* GREATER-THAN OR SLANTED EQUAL TO WITH DOT ABOVE LEFT */
	{0x2A85, 0x2A85, 2}, /* LESS-THAN OR APPROXIMATE */
	{0x2A86, 0x2A86, 2}, /* GREATER-THAN OR APPROXIMATE */
	{0x2A87, 0x2A87, 2}, /* LESS-THAN AND SINGLE-LINE NOT EQUAL TO */
	{0x2A88, 0x2A88, 2}, /* GREATER-THAN AND SINGLE-LINE NOT EQUAL TO */
	{0x2A89, 0x2A89, 2}, /* LESS-THAN AND NOT APPROXIMATE */
	{0x2A8A, 0x2A8A, 2}, /* GREATER-THAN AND NOT APPROXIMATE */
	{0x2A8B, 0x2A8B, 2}, /* LESS-THAN ABOVE DOUBLE-LINE EQUAL ABOVE GREATER-THAN */
	{0x2A8C, 0x2A8C, 2}, /* GREATER-THAN ABOVE DOUBLE-LINE EQUAL ABOVE LESS-THAN */
	{0x2A8D, 0x2A8D, 2}, /* LESS-THAN ABOVE SIMILAR OR EQUAL */
	{0x2A8E, 0x2A8E, 2}, /* GREATER-THAN ABOVE SIMILAR OR EQUAL */
	{0x2A8F, 0x2A8F, 2}, /* LESS-THAN ABOVE SIMILAR ABOVE GREATER-THAN */
	{0x2A90, 0x2A90, 2}, /* GREATER-THAN ABOVE SIMILAR ABOVE LESS-THAN */
	{0x2A91, 0x2A91, 2}, /* LESS-THAN ABOVE GREATER-THAN ABOVE DOUBLE-LINE EQUAL */
	{0x2A92, 0x2A92, 2}, /* GREATER-THAN ABOVE LESS-THAN ABOVE DOUBLE-LINE EQUAL */
	{0x2A93, 0x2A93, 2}, /* LESS-THAN ABOVE SLANTED EQUAL ABOVE GREATER-THAN ABOVE SLANTED EQUAL */
	{0x2A94, 0x2A94, 2}, /* GREATER-THAN ABOVE SLANTED EQUAL ABOVE LESS-THAN ABOVE SLANTED EQUAL */
	{0x2A95, 0x2A95, 2}, /* SLANTED EQUAL TO OR LESS-THAN */
	{0x2A96, 0x2A96, 2}, /* SLANTED EQUAL TO OR GREATER-THAN */
	{0x2A97, 0x2A97, 2}, /* SLANTED EQUAL TO OR LESS-THAN WITH DOT INSIDE */
	{0x2A98, 0x2A98, 2}, /* SLANTED EQUAL TO OR GREATER-THAN WITH DOT INSIDE */
	{0x2A99, 0x2A99, 2}, /* DOUBLE-LINE EQUAL TO OR LESS-THAN */
	{0x2A9A, 0x2A9A, 2}, /* DOUBLE-LINE EQUAL TO OR GREATER-THAN */
	{0x2A9D, 0x2A9D, 2}, /* SIMILAR OR LESS-THAN */
	{0x2A9E, 0x2A9E, 2}, /* SIMILAR OR GREATER-THAN */
	{0x2A9F, 0x2A9F, 2}, /* SIMILAR ABOVE LESS-THAN ABOVE EQUALS SIGN */
	{0x2AA0, 0x2AA0, 2}, /* SIMILAR ABOVE GREATER-THAN ABOVE EQUALS SIGN */
	{0x2AA1, 0x2AA1, 2}, /* DOUBLE NESTED LESS-THAN */
	{0x2AA2, 0x2AA2, 2}, /* DOUBLE NESTED GREATER-THAN */
	{0x2AA4, 0x2AA4, 2}, /* GREATER-THAN OVERLAPPING LESS-THAN */
	{0x2AA5, 0x2AA5, 2}, /* GREATER-THAN BESIDE LESS-THAN */
	{0x2AA6, 0x2AA6, 2}, /* LESS-THAN CLOSED BY CURVE */
	{0x2AA7, 0x2AA7, 2}, /* GREATER-THAN CLOSED BY CURVE */
	{0x2AA8, 0x2AA8, 2}, /* LESS-THAN CLOSED BY CURVE ABOVE SLANTED EQUAL */
	{0x2AA9, 0x2AA9, 2}, /* GREATER-THAN CLOSED BY CURVE ABOVE SLANTED EQUAL */
	{0x2AAA, 0x2AAA, 2}, /* SMALLER THAN */
	{0x2AAB, 0x2AAB, 2}, /* LARGER THAN */
	{0x2AAC, 0x2AAC, 2}, /* SMALLER THAN OR EQUAL TO */
	{0x2AAD, 0x2AAD, 2}, /* LARGER THAN OR EQUAL TO */
	{0x2AAE, 0x2AAE, 2}, /* EQUALS SIGN WITH BUMPY ABOVE */
	{0x2AAF, 0x2AAF, 2}, /* PRECEDES ABOVE SINGLE-LINE EQUALS SIGN */
	{0x2AB0, 0x2AB0, 2}, /* SUCCEEDS ABOVE SINGLE-LINE EQUALS SIGN */
	{0x2AB3, 0x2AB3, 2}, /* PRECEDES ABOVE EQUALS SIGN */
	{0x2AB4, 0x2AB4, 2}, /* SUCCEEDS ABOVE EQUALS SIGN */
	{0x2AB5, 0x2AB5, 2}, /* PRECEDES ABOVE NOT EQUAL TO */
	{0x2AB6, 0x2AB6, 2}, /* SUCCEEDS ABOVE NOT EQUAL TO */
	{0x2AB7, 0x2AB7, 2}, /* PRECEDES ABOVE ALMOST EQUAL TO */
	{0x2AB8, 0x2AB8, 2}, /* SUCCEEDS ABOVE ALMOST EQUAL TO */
	{0x2AB9, 0x2AB9, 2}, /* PRECEDES ABOVE NOT ALMOST EQUAL TO */
	{0x2ABA, 0x2ABA, 2}, /* SUCCEEDS ABOVE NOT ALMOST EQUAL TO */
	{0x2ABB, 0x2ABB, 2}, /* DOUBLE PRECEDES */
	{0x2ABC, 0x2ABC, 2}, /* DOUBLE SUCCEEDS */
	{0x2ABD, 0x2ABD, 2}, /* SUBSET WITH DOT */
	{0x2ABE, 0x2ABE, 2}, /* SUPERSET WITH DOT */
	{0x2ABF, 0x2ABF, 2}, /* SUBSET WITH PLUS SIGN BELOW */
	{0x2AC0, 0x2AC0, 2}, /* SUPERSET WITH PLUS SIGN BELOW */
	{0x2AC1, 0x2AC1, 2}, /* SUBSET WITH MULTIPLICATION SIGN BELOW */
	{0x2AC2, 0x2AC2, 2}, /* SUPERSET WITH MULTIPLICATION SIGN BELOW */
	{0x2AC3, 0x2AC3, 2}, /* SUBSET OF OR EQUAL TO WITH DOT ABOVE */
	{0x2AC4, 0x2AC4, 2}, /* SUPERSET OF OR EQUAL TO WITH DOT ABOVE */
	{0x2AC5, 0x2AC5, 2}, /* SUBSET OF ABOVE EQUALS SIGN */
	{0x2AC6, 0x2AC6, 2}, /* SUPERSET OF ABOVE EQUALS SIGN */
	{0x2AC7, 0x2AC7, 2}, /* SUBSET OF ABOVE TILDE OPERATOR */
	{0x2AC8, 0x2AC8, 2}, /* SUPERSET OF ABOVE TILDE OPERATOR */
	{0x2ACB, 0x2ACB, 2}, /* SUBSET OF ABOVE NOT EQUAL TO */
	{0x2ACC, 0x2ACC, 2}, /* SUPERSET OF ABOVE NOT EQUAL TO */
	{0x2ACF, 0x2ACF, 2}, /* CLOSED SUBSET */
	{0x2AD0, 0x2AD0, 2}, /* CLOSED SUPERSET */
	{0x2AD1, 0x2AD1, 2}, /* CLOSED SUBSET OR EQUAL TO */
	{0x2AD2, 0x2AD2, 2}, /* CLOSED SUPERSET OR EQUAL TO */
	{0x2AD3, 0x2AD3, 2}, /* SUBSET ABOVE SUPERSET */
	{0x2AD4, 0x2AD4, 2}, /* SUPERSET ABOVE SUBSET */
	{0x2AD5, 0x2AD5, 2}, /* SUBSET ABOVE SUBSET */
	{0x2AD6, 0x2AD6, 2}, /* SUPERSET ABOVE SUPERSET */
	{0x2AD7, 0x2AD7, 2}, /* SUPERSET BESIDE SUBSET */
	{0x2AD8, 0x2AD8, 2}, /* SUPERSET BESIDE AND JOINED BY DASH WITH SUBSET */
	{0x2AD9, 0x2AD9, 2}, /* ELEMENT OF OPENING DOWNWARDS */
	{0x2ADA, 0x2ADA, 2}, /* PITCHFORK WITH TEE TOP */
	{0x2ADB, 0x2ADB, 2}, /* TRANSVERSAL INTERSECTION */
	{0x2AE4, 0x2AE4, 2}, /* VERTICAL BAR DOUBLE LEFT TURNSTILE */
	{0x2AE6, 0x2AE6, 2}, /* LONG DASH FROM LEFT MEMBER OF DOUBLE VERTICAL */
	{0x2AE7, 0x2AE7, 2}, /* SHORT DOWN TACK WITH OVERBAR */
	{0x2AE8, 0x2AE8, 2}, /* SHORT UP TACK WITH UNDERBAR */
	{0x2AE9, 0x2AE9, 2}, /* SHORT UP TACK ABOVE SHORT DOWN TACK */
	{0x2AEB, 0x2AEB, 2}, /* DOUBLE UP TACK */
	{0x2AEC, 0x2AEC, 2}, /* DOUBLE STROKE NOT SIGN */
	{0x2AED, 0x2AED, 2}, /* REVERSED DOUBLE STROKE NOT SIGN */
	{0x2AEE, 0x2AEE, 2}, /* DOES NOT DIVIDE WITH REVERSED NEGATION SLASH */
	{0x2AEF, 0x2AEF, 2}, /* VERTICAL LINE WITH CIRCLE ABOVE */
	{0x2AF0, 0x2AF0, 2}, /* VERTICAL LINE WITH CIRCLE BELOW */
	{0x2AF1, 0x2AF1, 2}, /* DOWN TACK WITH CIRCLE BELOW */
	{0x2AF2, 0x2AF2, 2}, /* PARALLEL WITH HORIZONTAL STROKE */
	{0x2AF3, 0x2AF3, 2}, /* PARALLEL WITH TILDE OPERATOR */
	{0x2AFD, 0x2AFD, 2}, /* DOUBLE SOLIDUS OPERATOR */
	{0x300A, 0x300A, 2}, /* LEFT DOUBLE ANGLE BRACKET */
	{0x300B, 0x300B, 2}, /* RIGHT DOUBLE ANGLE BRACKET */
	{0x3014, 0x3014, 2}, /* LEFT TORTOISE SHELL BRACKET */
	{0x3015, 0x3015, 2}, /* RIGHT TORTOISE SHELL BRACKET */
	{0x3018, 0x3018, 2}, /* LEFT WHITE TORTOISE SHELL BRACKET */
	{0x3019, 0x3019, 2}, /* RIGHT WHITE TORTOISE SHELL BRACKET */
	{0x301A, 0x301A, 2}, /* LEFT WHITE SQUARE BRACKET */
	{0x301B, 0x301B, 2}, /* RIGHT WHITE SQUARE BRACKET */
	{0xFB00, 0xFB00, 2}, /* LATIN SMALL LIGATURE FF */
	{0xFB01, 0xFB01, 2}, /* LATIN SMALL LIGATURE FI */
	{0xFB02, 0xFB02, 2}, /* LATIN SMALL LIGATURE FL */
	{0xFB03, 0xFB03, 2}, /* LATIN SMALL LIGATURE FFI */
	{0xFB04, 0xFB04, 2}, /* LATIN SMALL LIGATURE FFL */
	{0xFE35, 0xFE35, 2}, /* PRESENTATION FORM FOR VERTICAL LEFT PARENTHESIS */
	{0xFE36, 0xFE36, 2}, /* PRESENTATION FORM FOR VERTICAL RIGHT PARENTHESIS */
	{0xFE37, 0xFE37, 2}, /* PRESENTATION FORM FOR VERTICAL LEFT CURLY BRACKET */
	{0xFE38, 0xFE38, 2}, /* PRESENTATION FORM FOR VERTICAL RIGHT CURLY BRACKET */
	{0xFFFD, 0xFFFD, 2}, /* REPLACEMENT CHARACTER */
	{0xFFFF, 0xFFFF, 0}
	};


/**
 *  Various characters not supported in the 7bit ASCII text (either default
 *  GSM, shifted GSM, downgraded by the internal converter or downgraded by
 *  the PREQ2090 converter).
 */
const TMappedChar  gUnconvertible7bitGSMShiftChars[] =
	{
	{0x0000, 0x003F, 1}, /* NUL */
	{0x0001, 0x003F, 1}, /* SOH */
	{0x0002, 0x003F, 1}, /* STX */
	{0x0003, 0x003F, 1}, /* ETX */
	{0x0004, 0x003F, 1}, /* EOT */
	{0x0005, 0x003F, 1}, /* ENQ */
	{0x0006, 0x003F, 1}, /* ACK */
	{0x0007, 0x003F, 1}, /* BEL */
	{0x0008, 0x003F, 1}, /* BS */
	{0x0009, 0x003F, 1}, /* CHARACTER TABULATION */
	{0x000B, 0x003F, 1}, /* VT */
	{0x000E, 0x003F, 1}, /* SO */
	{0x000F, 0x003F, 1}, /* SI */
	{0x0010, 0x003F, 1}, /* DLE */
	{0x0011, 0x003F, 1}, /* DC1 */
	{0x0012, 0x003F, 1}, /* DC2 */
	{0x0013, 0x003F, 1}, /* DC3 */
	{0x0014, 0x003F, 1}, /* DC4 */
	{0x0015, 0x003F, 1}, /* NAK */
	{0x0016, 0x003F, 1}, /* SYN */
	{0x0017, 0x003F, 1}, /* ETB */
	{0x0018, 0x003F, 1}, /* CAN */
	{0x0019, 0x003F, 1}, /* EM */
	{0x001A, 0x003F, 1}, /* SUB */
	// Escape code
	{0x001C, 0x003F, 1}, /* FS */
	{0x001D, 0x003F, 1}, /* GS */
	{0x001E, 0x003F, 1}, /* RS */
	{0x001F, 0x003F, 1}, /* US */
	{0x007F, 0x003F, 1}, /* DEL */
	//{0x00A8, 0x003F, 1}, /* DIAERESIS */ DEF142811 adds (0x00a8 -> 0x0022) to symbian extension sms 7 bit converter
	{0x00AF, 0x003F, 1}, /* MACRON */
	{0x00B1, 0x003F, 1}, /* PLUS-MINUS SIGN */
	{0x00B6, 0x003F, 1}, /* PILCROW SIGN */
	{0x0132, 0x003F, 1}, /* LATIN CAPITAL LIGATURE IJ */
	{0x0133, 0x003F, 1}, /* LATIN SMALL LIGATURE IJ */
	{0x0192, 0x003F, 1}, /* LATIN SMALL LETTER F WITH HOOK */
	{0x01B5, 0x003F, 1}, /* LATIN CAPITAL LETTER Z WITH STROKE */
	{0x01F5, 0x003F, 1}, /* LATIN SMALL LETTER G WITH ACUTE */
	{0x02C6, 0x003F, 1}, /* MODIFIER LETTER CIRCUMFLEX ACCENT */
	{0x02C7, 0x003F, 1}, /* CARON */
	{0x02D8, 0x003F, 1}, /* BREVE */
	{0x02D9, 0x003F, 1}, /* DOT ABOVE */
	{0x02DA, 0x003F, 1}, /* RING ABOVE */
	{0x02DB, 0x003F, 1}, /* OGONEK */
	{0x02DC, 0x003F, 1}, /* SMALL TILDE */
	{0x02DD, 0x003F, 1}, /* DOUBLE ACUTE ACCENT */
	{0x0311, 0x003F, 1}, /* COMBINING INVERTED BREVE */
	{0x0332, 0x003F, 1}, /* COMBINING LOW LINE */
	{0x03D6, 0x003F, 1}, /* GREEK PI SYMBOL */
	{0x03DC, 0x003F, 1}, /* GREEK LETTER DIGAMMA */
	{0x03DD, 0x003F, 1}, /* GREEK SMALL LETTER DIGAMMA */
	{0x03F0, 0x003F, 1}, /* GREEK KAPPA SYMBOL */
	{0x03F1, 0x003F, 1}, /* GREEK RHO SYMBOL */
	{0x03F5, 0x003F, 1}, /* GREEK LUNATE EPSILON SYMBOL */
	{0x03F6, 0x003F, 1}, /* GREEK REVERSED LUNATE EPSILON SYMBOL */
	{0x0401, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER IO */
	{0x0402, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER DJE */
	{0x0403, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER GJE */
	{0x0404, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER UKRAINIAN IE */
	{0x0405, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER DZE */
	{0x0406, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I */
	{0x0407, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER YI */
	{0x0408, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER JE */
	{0x0409, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER LJE */
	{0x040A, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER NJE */
	{0x040B, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER TSHE */
	{0x040C, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER KJE */
	{0x040E, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER SHORT U */
	{0x040F, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER DZHE */
	{0x0410, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER A */
	{0x0411, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER BE */
	{0x0412, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER VE */
	{0x0413, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER GHE */
	{0x0414, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER DE */
	{0x0415, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER IE */
	{0x0416, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER ZHE */
	{0x0417, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER ZE */
	{0x0418, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER I */
	{0x0419, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER SHORT I */
	{0x041A, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER KA */
	{0x041B, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER EL */
	{0x041C, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER EM */
	{0x041D, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER EN */
	{0x041E, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER O */
	{0x041F, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER PE */
	{0x0420, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER ER */
	{0x0421, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER ES */
	{0x0422, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER TE */
	{0x0423, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER U */
	{0x0424, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER EF */
	{0x0425, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER HA */
	{0x0426, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER TSE */
	{0x0427, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER CHE */
	{0x0428, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER SHA */
	{0x0429, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER SHCHA */
	{0x042A, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER HARD SIGN */
	{0x042B, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER YERU */
	{0x042C, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER SOFT SIGN */
	{0x042D, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER E */
	{0x042E, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER YU */
	{0x042F, 0x003F, 1}, /* CYRILLIC CAPITAL LETTER YA */
	{0x0430, 0x003F, 1}, /* CYRILLIC SMALL LETTER A */
	{0x0431, 0x003F, 1}, /* CYRILLIC SMALL LETTER BE */
	{0x0432, 0x003F, 1}, /* CYRILLIC SMALL LETTER VE */
	{0x0433, 0x003F, 1}, /* CYRILLIC SMALL LETTER GHE */
	{0x0434, 0x003F, 1}, /* CYRILLIC SMALL LETTER DE */
	{0x0435, 0x003F, 1}, /* CYRILLIC SMALL LETTER IE */
	{0x0436, 0x003F, 1}, /* CYRILLIC SMALL LETTER ZHE */
	{0x0437, 0x003F, 1}, /* CYRILLIC SMALL LETTER ZE */
	{0x0438, 0x003F, 1}, /* CYRILLIC SMALL LETTER I */
	{0x0439, 0x003F, 1}, /* CYRILLIC SMALL LETTER SHORT I */
	{0x043A, 0x003F, 1}, /* CYRILLIC SMALL LETTER KA */
	{0x043B, 0x003F, 1}, /* CYRILLIC SMALL LETTER EL */
	{0x043C, 0x003F, 1}, /* CYRILLIC SMALL LETTER EM */
	{0x043D, 0x003F, 1}, /* CYRILLIC SMALL LETTER EN */
	{0x043E, 0x003F, 1}, /* CYRILLIC SMALL LETTER O */
	{0x043F, 0x003F, 1}, /* CYRILLIC SMALL LETTER PE */
	{0x0440, 0x003F, 1}, /* CYRILLIC SMALL LETTER ER */
	{0x0441, 0x003F, 1}, /* CYRILLIC SMALL LETTER ES */
	{0x0442, 0x003F, 1}, /* CYRILLIC SMALL LETTER TE */
	{0x0443, 0x003F, 1}, /* CYRILLIC SMALL LETTER U */
	{0x0444, 0x003F, 1}, /* CYRILLIC SMALL LETTER EF */
	{0x0445, 0x003F, 1}, /* CYRILLIC SMALL LETTER HA */
	{0x0446, 0x003F, 1}, /* CYRILLIC SMALL LETTER TSE */
	{0x0447, 0x003F, 1}, /* CYRILLIC SMALL LETTER CHE */
	{0x0448, 0x003F, 1}, /* CYRILLIC SMALL LETTER SHA */
	{0x0449, 0x003F, 1}, /* CYRILLIC SMALL LETTER SHCHA */
	{0x044A, 0x003F, 1}, /* CYRILLIC SMALL LETTER HARD SIGN */
	{0x044B, 0x003F, 1}, /* CYRILLIC SMALL LETTER YERU */
	{0x044C, 0x003F, 1}, /* CYRILLIC SMALL LETTER SOFT SIGN */
	{0x044D, 0x003F, 1}, /* CYRILLIC SMALL LETTER E */
	{0x044E, 0x003F, 1}, /* CYRILLIC SMALL LETTER YU */
	{0x044F, 0x003F, 1}, /* CYRILLIC SMALL LETTER YA */
	{0x0451, 0x003F, 1}, /* CYRILLIC SMALL LETTER IO */
	{0x0452, 0x003F, 1}, /* CYRILLIC SMALL LETTER DJE */
	{0x0453, 0x003F, 1}, /* CYRILLIC SMALL LETTER GJE */
	{0x0454, 0x003F, 1}, /* CYRILLIC SMALL LETTER UKRAINIAN IE */
	{0x0455, 0x003F, 1}, /* CYRILLIC SMALL LETTER DZE */
	{0x0456, 0x003F, 1}, /* CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I */
	{0x0457, 0x003F, 1}, /* CYRILLIC SMALL LETTER YI */
	{0x0458, 0x003F, 1}, /* CYRILLIC SMALL LETTER JE */
	{0x0459, 0x003F, 1}, /* CYRILLIC SMALL LETTER LJE */
	{0x045A, 0x003F, 1}, /* CYRILLIC SMALL LETTER NJE */
	{0x045B, 0x003F, 1}, /* CYRILLIC SMALL LETTER TSHE */
	{0x045C, 0x003F, 1}, /* CYRILLIC SMALL LETTER KJE */
	{0x045E, 0x003F, 1}, /* CYRILLIC SMALL LETTER SHORT U */
	{0x045F, 0x003F, 1}, /* CYRILLIC SMALL LETTER DZHE */
	{0xFFFD, 0x003F, 1}, /* REPLACEMENT CHARACTER */
	{0xFFFF, 0xFFFF, 0}
	};


/**
 *  7bit ASCII text that is not in the GSM default table and would be
 *  downgraded by the current internal converter.
 */
const TMappedChar  gDowngrading7bitGSMChars[] =
	{
	{0x00C0, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH GRAVE */
	{0x00C1, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH ACUTE */
	{0x00C2, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
	{0x00C3, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH TILDE */
	{0x00C7, 0x00E7, 1}, /* LATIN CAPITAL LETTER C WITH CEDILLA */
	{0x00C8, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH GRAVE */
	{0x00CA, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
	{0x00CB, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH DIAERESIS */
	{0x00CC, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH GRAVE */
	{0x00CD, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH ACUTE */
	{0x00CE, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
	{0x00CF, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH DIAERESIS */
	{0x00D2, 0x004f, 1}, /* LATIN CAPITAL LETTER O WITH GRAVE */
	{0x00D3, 0x004f, 1}, /* LATIN CAPITAL LETTER O WITH ACUTE */
	{0x00D4, 0x004f, 1}, /* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
	{0x00D5, 0x004f, 1}, /* LATIN CAPITAL LETTER O WITH TILDE */
	{0x00D9, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH GRAVE */
	{0x00DA, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH ACUTE */
	{0x00DB, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
	{0x00DD, 0x0059, 1}, /* LATIN CAPITAL LETTER Y WITH ACUTE */
	{0x00E1, 0x0061, 1}, /* LATIN SMALL LETTER A WITH ACUTE */
	{0x00E2, 0x0061, 1}, /* LATIN SMALL LETTER A WITH CIRCUMFLEX */
	{0x00E3, 0x0061, 1}, /* LATIN SMALL LETTER A WITH TILDE */
	{0x00EA, 0x0065, 1}, /* LATIN SMALL LETTER E WITH CIRCUMFLEX */
	{0x00EB, 0x0065, 1}, /* LATIN SMALL LETTER E WITH DIAERESIS */
	{0x00ED, 0x0069, 1}, /* LATIN SMALL LETTER I WITH ACUTE */
	{0x00EE, 0x0069, 1}, /* LATIN SMALL LETTER I WITH CIRCUMFLEX */
	{0x00EF, 0x0069, 1}, /* LATIN SMALL LETTER I WITH DIAERESIS */
	{0x00F3, 0x006f, 1}, /* LATIN SMALL LETTER O WITH ACUTE */
	{0x00F4, 0x006f, 1}, /* LATIN SMALL LETTER O WITH CIRCUMFLEX */
	{0x00F5, 0x006f, 1}, /* LATIN SMALL LETTER O WITH TILDE */
	{0x00FA, 0x0075, 1}, /* LATIN SMALL LETTER U WITH ACUTE */
	{0x00FB, 0x0075, 1}, /* LATIN SMALL LETTER U WITH CIRCUMFLEX */
	{0x00FD, 0x0079, 1}, /* LATIN SMALL LETTER Y WITH ACUTE */
	{0x00FF, 0x0079, 1}, /* LATIN SMALL LETTER Y WITH DIAERESIS */
	{0x0386, 0x0041, 1}, /* GREEK CAPITAL LETTER ALPHA WITH TONOS */
	{0x0388, 0x0045, 1}, /* GREEK CAPITAL LETTER EPSILON WITH TONOS */
	{0x0389, 0x0048, 1}, /* GREEK CAPITAL LETTER ETA WITH TONOS */
	{0x038a, 0x0049, 1}, /* GREEK CAPITAL LETTER IOTA WITH TONOS */
	{0x038c, 0x004f, 1}, /* GREEK CAPITAL LETTER OMICRON WITH TONOS */
	{0x038e, 0x0059, 1}, /* GREEK CAPITAL LETTER UPSILON WITH TONOS */
	{0x038f, 0x03a9, 1}, /* GREEK CAPITAL LETTER OMEGA WITH TONOS */
	{0x0390, 0x0049, 1}, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS */
	{0x0391, 0x0041, 1}, /* GREEK CAPITAL LETTER ALPHA */
	{0x0392, 0x0042, 1}, /* GREEK CAPITAL LETTER BETA */
	{0x0395, 0x0045, 1}, /* GREEK CAPITAL LETTER EPSILON */
	{0x0396, 0x005a, 1}, /* GREEK CAPITAL LETTER ZETA */
	{0x0397, 0x0048, 1}, /* GREEK CAPITAL LETTER ETA */
	{0x0399, 0x0049, 1}, /* GREEK CAPITAL LETTER IOTA */
	{0x039a, 0x004b, 1}, /* GREEK CAPITAL LETTER KAPPA */
	{0x039c, 0x004d, 1}, /* GREEK CAPITAL LETTER MU */
	{0x039d, 0x004e, 1}, /* GREEK CAPITAL LETTER NU */
	{0x039f, 0x004f, 1}, /* GREEK CAPITAL LETTER OMICRON */
	{0x03a1, 0x0050, 1}, /* GREEK CAPITAL LETTER RHO */
	{0x03a4, 0x0054, 1}, /* GREEK CAPITAL LETTER TAU */
	{0x03A5, 0x0059, 1}, /* GREEK CAPITAL LETTER UPSILON */
	{0x03a7, 0x0058, 1}, /* GREEK CAPITAL LETTER CHI */
	{0x03aa, 0x0049, 1}, /* GREEK CAPITAL LETTER IOTA WITH DIALYTIKA */
	{0x03ab, 0x0059, 1}, /* GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA */
	{0x03ac, 0x0041, 1}, /* GREEK SMALL LETTER ALPHA WITH TONOS */
	{0x03ad, 0x0045, 1}, /* GREEK SMALL LETTER EPSILON WITH TONOS */
	{0x03ae, 0x0048, 1}, /* GREEK SMALL LETTER ETA WITH TONOS */
	{0x03af, 0x0049, 1}, /* GREEK SMALL LETTER IOTA WITH TONOS */
	{0x03b0, 0x0059, 1}, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS */
	{0x03B1, 0x0041, 1}, /* GREEK SMALL LETTER ALPHA */
	{0x03B2, 0x0042, 1}, /* GREEK SMALL LETTER BETA */
	{0x03B3, 0x0393, 1}, /* GREEK SMALL LETTER GAMMA */
	{0x03B4, 0x0394, 1}, /* GREEK SMALL LETTER DELTA */
	{0x03B5, 0x0045, 1}, /* GREEK SMALL LETTER EPSILON */
	{0x03B6, 0x005a, 1}, /* GREEK SMALL LETTER ZETA */
	{0x03B7, 0x0048, 1}, /* GREEK SMALL LETTER ETA */
	{0x03B8, 0x0398, 1}, /* GREEK SMALL LETTER THETA */
	{0x03B9, 0x0049, 1}, /* GREEK SMALL LETTER IOTA */
	{0x03BA, 0x004b, 1}, /* GREEK SMALL LETTER KAPPA */
	{0x03BB, 0x039b, 1}, /* GREEK SMALL LETTER LAMDA */
	{0x03BC, 0x004d, 1}, /* GREEK SMALL LETTER MU */
	{0x03BD, 0x004e, 1}, /* GREEK SMALL LETTER NU */
	{0x03BE, 0x039e, 1}, /* GREEK SMALL LETTER XI */
	{0x03bf, 0x004f, 1}, /* GREEK SMALL LETTER OMICRON */
	{0x03C0, 0x03a0, 1}, /* GREEK SMALL LETTER PI */
	{0x03C1, 0x0050, 1}, /* GREEK SMALL LETTER RHO */
	{0x03C2, 0x03a3, 1}, /* GREEK SMALL LETTER FINAL SIGMA */
	{0x03C3, 0x03a3, 1}, /* GREEK SMALL LETTER SIGMA */
	{0x03C4, 0x0054, 1}, /* GREEK SMALL LETTER TAU */
	{0x03C5, 0x0059, 1}, /* GREEK SMALL LETTER UPSILON */
	{0x03C6, 0x03a6, 1}, /* GREEK SMALL LETTER PHI */
	{0x03C7, 0x0058, 1}, /* GREEK SMALL LETTER CHI */
	{0x03C8, 0x03a8, 1}, /* GREEK SMALL LETTER PSI */
	{0x03C9, 0x03a9, 1}, /* GREEK SMALL LETTER OMEGA */
	{0x03ca, 0x0049, 1}, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA */
	{0x03cb, 0x0059, 1}, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA */
	{0x03cc, 0x004f, 1}, /* GREEK SMALL LETTER OMICRON WITH TONOS */
	{0x03cd, 0x0059, 1}, /* GREEK SMALL LETTER UPSILON WITH TONOS */
	{0x03ce, 0x03a9, 1}, /* GREEK SMALL LETTER OMEGA WITH TONOS */
	//{0x03d0, 0x0042, 1}, /* GREEK BETA SYMBOL */
	//{0x03D1, 0x0398, 1}, /* GREEK THETA SYMBOL */
	//{0x03D2, 0x0059, 1}, /* GREEK UPSILON WITH HOOK SYMBOL */
	//{0x03d3, 0x0059, 1}, /* GREEK UPSILON WITH ACUTE AND HOOK SYMBOL */
	//{0x03d4, 0x0059, 1}, /* GREEK UPSILON WITH DIAERESIS AND HOOK SYMBOL */
	//{0x03D5, 0x03a6, 1}, /* GREEK PHI SYMBOL */
	//{0x2126, 0x03a9, 1}, /* OHM SIGN */
	//{0x220F, 0x03a0, 1}, /* N-ARY PRODUCT */
	//{0x2211, 0x03a3, 1}, /* N-ARY SUMMATION */
	{0xFFFF, 0xFFFF, 0}
	};


/**
 *  7bit ASCII text that is not in the GSM default table and would be
 *  downgraded by the new Eastern European converter (PREQ2090).
 */
const TMappedChar  gDowngrading7bitGSMCharsExtended[] =
	{
	{0x0060, 0x0027, 1}, /* GRAVE ACCENT */
	{0x00A0, 0x0020, 1}, /* NO-BREAK SPACE */
	{0x00A2, 0x0063, 1}, /* CENT SIGN */
	{0x00A8, 0x0022, 1}, /* DIAERESIS */ // DEF142811 adds (0x00a8 -> 0x0022) to symbian extension sms 7 bit converter
	{0x00A9, 0x0063, 1}, /* COPYRIGHT SIGN */
	{0x00AA, 0x0061, 1}, /* FEMININE ORDINAL INDICATOR */
	{0x00AB, 0x0022, 1}, /* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{0x00AD, 0x002D, 1}, /* SOFT HYPHEN */
	{0x00AE, 0x0072, 1}, /* REGISTERED SIGN */
	{0x00B0, 0x006F, 1}, /* DEGREE SIGN */
	{0x00B2, 0x0032, 1}, /* SUPERSCRIPT TWO */
	{0x00B3, 0x0033, 1}, /* SUPERSCRIPT THREE */
	{0x00B4, 0x0027, 1}, /* ACUTE ACCENT */
	{0x00B5, 0x0075, 1}, /* MICRO SIGN */
	{0x00B7, 0x002E, 1}, /* MIDDLE DOT */
	{0x00B8, 0x002C, 1}, /* CEDILLA */
	{0x00B9, 0x0031, 1}, /* SUPERSCRIPT ONE */
	{0x00BA, 0x006F, 1}, /* MASCULINE ORDINAL INDICATOR */
	{0x00BB, 0x0022, 1}, /* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{0x00D0, 0x0044, 1}, /* LATIN CAPITAL LETTER ETH */
	{0x00D7, 0x002A, 1}, /* MULTIPLICATION SIGN */
	{0x00DE, 0x0054, 1}, /* LATIN CAPITAL LETTER THORN */
	{0x00F0, 0x0064, 1}, /* LATIN SMALL LETTER ETH */
	{0x00F7, 0x002F, 1}, /* DIVISION SIGN */
	{0x00FE, 0x0074, 1}, /* LATIN SMALL LETTER THORN */
	{0x0100, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH MACRON */
	{0x0101, 0x0061, 1}, /* LATIN SMALL LETTER A WITH MACRON */
	{0x0102, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH BREVE */
	{0x0103, 0x0061, 1}, /* LATIN SMALL LETTER A WITH BREVE */
	{0x0104, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH OGONEK */
	{0x0105, 0x0061, 1}, /* LATIN SMALL LETTER A WITH OGONEK */
	{0x0106, 0x0043, 1}, /* LATIN CAPITAL LETTER C WITH ACUTE */
	{0x0107, 0x0063, 1}, /* LATIN SMALL LETTER C WITH ACUTE */
	{0x0108, 0x0043, 1}, /* LATIN CAPITAL LETTER C WITH CIRCUMFLEX */
	{0x0109, 0x0063, 1}, /* LATIN SMALL LETTER C WITH CIRCUMFLEX */
	{0x010A, 0x0043, 1}, /* LATIN CAPITAL LETTER C WITH DOT ABOVE */
	{0x010B, 0x0063, 1}, /* LATIN SMALL LETTER C WITH DOT ABOVE */
	{0x010C, 0x0043, 1}, /* LATIN CAPITAL LETTER C WITH CARON */
	{0x010D, 0x0063, 1}, /* LATIN SMALL LETTER C WITH CARON */
	{0x010E, 0x0044, 1}, /* LATIN CAPITAL LETTER D WITH CARON */
	{0x010F, 0x0064, 1}, /* LATIN SMALL LETTER D WITH CARON */
	{0x0110, 0x0044, 1}, /* LATIN CAPITAL LETTER D WITH STROKE */
	{0x0111, 0x0064, 1}, /* LATIN SMALL LETTER D WITH STROKE */
	{0x0112, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH MACRON */
	{0x0113, 0x0065, 1}, /* LATIN SMALL LETTER E WITH MACRON */
	{0x0114, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH BREVE */
	{0x0115, 0x0065, 1}, /* LATIN SMALL LETTER E WITH BREVE */
	{0x0116, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH DOT ABOVE */
	{0x0117, 0x0065, 1}, /* LATIN SMALL LETTER E WITH DOT ABOVE */
	{0x0118, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH OGONEK */
	{0x0119, 0x0065, 1}, /* LATIN SMALL LETTER E WITH OGONEK */
	{0x011A, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH CARON */
	{0x011B, 0x0065, 1}, /* LATIN SMALL LETTER E WITH CARON */
	{0x011C, 0x0047, 1}, /* LATIN CAPITAL LETTER G WITH CIRCUMFLEX */
	{0x011D, 0x0067, 1}, /* LATIN SMALL LETTER G WITH CIRCUMFLEX */
	{0x011E, 0x0047, 1}, /* LATIN CAPITAL LETTER G WITH BREVE */
	{0x011F, 0x0067, 1}, /* LATIN SMALL LETTER G WITH BREVE */
	{0x0120, 0x0047, 1}, /* LATIN CAPITAL LETTER G WITH DOT ABOVE */
	{0x0121, 0x0067, 1}, /* LATIN SMALL LETTER G WITH DOT ABOVE */
	{0x0122, 0x0047, 1}, /* LATIN CAPITAL LETTER G WITH CEDILLA */
	{0x0123, 0x0067, 1}, /* LATIN SMALL LETTER G WITH CEDILLA */
	{0x0124, 0x0048, 1}, /* LATIN CAPITAL LETTER H WITH CIRCUMFLEX */
	{0x0125, 0x0068, 1}, /* LATIN SMALL LETTER H WITH CIRCUMFLEX */
	{0x0126, 0x0048, 1}, /* LATIN CAPITAL LETTER H WITH STROKE */
	{0x0127, 0x0068, 1}, /* LATIN SMALL LETTER H WITH STROKE */
	{0x0128, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH TILDE */
	{0x0129, 0x0069, 1}, /* LATIN SMALL LETTER I WITH TILDE */
	{0x012A, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH MACRON */
	{0x012B, 0x0069, 1}, /* LATIN SMALL LETTER I WITH MACRON */
	{0x012C, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH BREVE */
	{0x012D, 0x0069, 1}, /* LATIN SMALL LETTER I WITH BREVE */
	{0x012E, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH OGONEK */
	{0x012F, 0x0069, 1}, /* LATIN SMALL LETTER I WITH OGONEK */
	{0x0130, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH DOT ABOVE */
	{0x0131, 0x0069, 1}, /* LATIN SMALL LETTER DOTLESS I */
	{0x0134, 0x004A, 1}, /* LATIN CAPITAL LETTER J WITH CIRCUMFLEX */
	{0x0135, 0x006A, 1}, /* LATIN SMALL LETTER J WITH CIRCUMFLEX */
	{0x0136, 0x004B, 1}, /* LATIN CAPITAL LETTER K WITH CEDILLA */
	{0x0137, 0x006B, 1}, /* LATIN SMALL LETTER K WITH CEDILLA */
	{0x0138, 0x006B, 1}, /* LATIN SMALL LETTER KRA */
	{0x0139, 0x004C, 1}, /* LATIN CAPITAL LETTER L WITH ACUTE */
	{0x013A, 0x006C, 1}, /* LATIN SMALL LETTER L WITH ACUTE */
	{0x013B, 0x004C, 1}, /* LATIN CAPITAL LETTER L WITH CEDILLA */
	{0x013C, 0x006C, 1}, /* LATIN SMALL LETTER L WITH CEDILLA */
	{0x013D, 0x004C, 1}, /* LATIN CAPITAL LETTER L WITH CARON */
	{0x013E, 0x006C, 1}, /* LATIN SMALL LETTER L WITH CARON */
	{0x013F, 0x004C, 1}, /* LATIN CAPITAL LETTER L WITH MIDDLE DOT */
	{0x0140, 0x006C, 1}, /* LATIN SMALL LETTER L WITH MIDDLE DOT */
	{0x0141, 0x004C, 1}, /* LATIN CAPITAL LETTER L WITH STROKE */
	{0x0142, 0x006C, 1}, /* LATIN SMALL LETTER L WITH STROKE */
	{0x0143, 0x004E, 1}, /* LATIN CAPITAL LETTER N WITH ACUTE */
	{0x0144, 0x006E, 1}, /* LATIN SMALL LETTER N WITH ACUTE */
	{0x0145, 0x004E, 1}, /* LATIN CAPITAL LETTER N WITH CEDILLA */
	{0x0146, 0x006E, 1}, /* LATIN SMALL LETTER N WITH CEDILLA */
	{0x0147, 0x004E, 1}, /* LATIN CAPITAL LETTER N WITH CARON */
	{0x0148, 0x006E, 1}, /* LATIN SMALL LETTER N WITH CARON */
	{0x0149, 0x006E, 1}, /* LATIN SMALL LETTER N PRECEDED BY APOSTROPHE */
	{0x014A, 0x004E, 1}, /* LATIN CAPITAL LETTER ENG */
	{0x014B, 0x006E, 1}, /* LATIN SMALL LETTER ENG */
	{0x014C, 0x004F, 1}, /* LATIN CAPITAL LETTER O WITH MACRON */
	{0x014D, 0x006F, 1}, /* LATIN SMALL LETTER O WITH MACRON */
	{0x014E, 0x004F, 1}, /* LATIN CAPITAL LETTER O WITH BREVE */
	{0x014F, 0x006F, 1}, /* LATIN SMALL LETTER O WITH BREVE */
	{0x0150, 0x00D6, 1}, /* LATIN CAPITAL LETTER O WITH DOUBLE ACUTE */
	{0x0151, 0x00F6, 1}, /* LATIN SMALL LETTER O WITH DOUBLE ACUTE */
	{0x0152, 0x004F, 1}, /* LATIN CAPITAL LIGATURE OE */
	{0x0153, 0x006F, 1}, /* LATIN SMALL LIGATURE OE */
	{0x0154, 0x0052, 1}, /* LATIN CAPITAL LETTER R WITH ACUTE */
	{0x0155, 0x0072, 1}, /* LATIN SMALL LETTER R WITH ACUTE */
	{0x0156, 0x0052, 1}, /* LATIN CAPITAL LETTER R WITH CEDILLA */
	{0x0157, 0x0072, 1}, /* LATIN SMALL LETTER R WITH CEDILLA */
	{0x0158, 0x0052, 1}, /* LATIN CAPITAL LETTER R WITH CARON */
	{0x0159, 0x0072, 1}, /* LATIN SMALL LETTER R WITH CARON */
	{0x015A, 0x0053, 1}, /* LATIN CAPITAL LETTER S WITH ACUTE */
	{0x015B, 0x0073, 1}, /* LATIN SMALL LETTER S WITH ACUTE */
	{0x015C, 0x0053, 1}, /* LATIN CAPITAL LETTER S WITH CIRCUMFLEX */
	{0x015D, 0x0073, 1}, /* LATIN SMALL LETTER S WITH CIRCUMFLEX */
	{0x015E, 0x0053, 1}, /* LATIN CAPITAL LETTER S WITH CEDILLA */
	{0x015F, 0x0073, 1}, /* LATIN SMALL LETTER S WITH CEDILLA */
	{0x0160, 0x0053, 1}, /* LATIN CAPITAL LETTER S WITH CARON */
	{0x0161, 0x0073, 1}, /* LATIN SMALL LETTER S WITH CARON */
	{0x0162, 0x0054, 1}, /* LATIN CAPITAL LETTER T WITH CEDILLA */
	{0x0163, 0x0074, 1}, /* LATIN SMALL LETTER T WITH CEDILLA */
	{0x0164, 0x0054, 1}, /* LATIN CAPITAL LETTER T WITH CARON */
	{0x0165, 0x0074, 1}, /* LATIN SMALL LETTER T WITH CARON */
	{0x0166, 0x0054, 1}, /* LATIN CAPITAL LETTER T WITH STROKE */
	{0x0167, 0x0074, 1}, /* LATIN SMALL LETTER T WITH STROKE */
	{0x0168, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH TILDE */
	{0x0169, 0x0075, 1}, /* LATIN SMALL LETTER U WITH TILDE */
	{0x016A, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH MACRON */
	{0x016B, 0x0075, 1}, /* LATIN SMALL LETTER U WITH MACRON */
	{0x016C, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH BREVE */
	{0x016D, 0x0075, 1}, /* LATIN SMALL LETTER U WITH BREVE */
	{0x016E, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH RING ABOVE */
	{0x016F, 0x0075, 1}, /* LATIN SMALL LETTER U WITH RING ABOVE */
	{0x0170, 0x00DC, 1}, /* LATIN CAPITAL LETTER U WITH DOUBLE ACUTE */
	{0x0171, 0x00FC, 1}, /* LATIN SMALL LETTER U WITH DOUBLE ACUTE */
	{0x0172, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH OGONEK */
	{0x0173, 0x0075, 1}, /* LATIN SMALL LETTER U WITH OGONEK */
	{0x0174, 0x0057, 1}, /* LATIN CAPITAL LETTER W WITH CIRCUMFLEX */
	{0x0175, 0x0077, 1}, /* LATIN SMALL LETTER W WITH CIRCUMFLEX */
	{0x0176, 0x0059, 1}, /* LATIN CAPITAL LETTER Y WITH CIRCUMFLEX */
	{0x0177, 0x0079, 1}, /* LATIN SMALL LETTER Y WITH CIRCUMFLEX */
	{0x0178, 0x0059, 1}, /* LATIN CAPITAL LETTER Y WITH DIAERESIS */
	{0x0179, 0x005A, 1}, /* LATIN CAPITAL LETTER Z WITH ACUTE */
	{0x017A, 0x007A, 1}, /* LATIN SMALL LETTER Z WITH ACUTE */
	{0x017B, 0x005A, 1}, /* LATIN CAPITAL LETTER Z WITH DOT ABOVE */
	{0x017C, 0x007A, 1}, /* LATIN SMALL LETTER Z WITH DOT ABOVE */
	{0x017D, 0x005A, 1}, /* LATIN CAPITAL LETTER Z WITH CARON */
	{0x017E, 0x007A, 1}, /* LATIN SMALL LETTER Z WITH CARON */
	{0x037E, 0x003B, 1}, /* GREEK QUESTION MARK */
	{0x0384, 0x0027, 1}, /* GREEK TONOS */
	{0x0385, 0x0027, 1}, /* GREEK DIALYTIKA TONOS */
	//{0x00A6, 0x007C, 1}, /* BROKEN BAR */
	//{0x00AC, 0x002D, 1}, /* NOT SIGN */
	//{0x017F, 0x0073, 1}, /* LATIN SMALL LETTER LONG S */
	//{0x0387, 0x002E, 1}, /* GREEK ANO TELEIA */
	{0xFFFF, 0xFFFF, 0}
	};

/**
 *  Turkish 7bit ASCII text that is in the GSM Turkish table but
 *  is downgraded (using PREQ2090).
 */
const TMappedChar  gDowngradingTurkish7bitGSM[] =
	{
	{0x0131, 0x0069, 1}, /* LATIN SMALL LETTER DOTLESS I */
	{0x011E, 0x0047, 1}, /* LATIN CAPITAL LETTER G WITH BREVE */
	{0x011F, 0x0067, 1}, /* LATIN SMALL LETTER G WITH BREVE */
	{0x015E, 0x0053, 1}, /* LATIN CAPITAL LETTER S WITH CEDILLA */
	{0x015F, 0x0073, 1}, /* LATIN SMALL LETTER S WITH CEDILLA */
	{0x0130, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH DOT ABOVE */
	{0xFFFF, 0xFFFF, 0}
	};


/**
 *  Turkish 7bit ASCII text that is in the GSM Turkish table (and
 *  not the default table).
 */
const TMappedChar  gTurkish7bitGSMLockingChars[] =
	{
	{0x0131, 0x0131, 1}, /* LATIN SMALL LETTER DOTLESS I */
	{0x011E, 0x011E, 1}, /* LATIN CAPITAL LETTER G WITH BREVE */
	{0x011F, 0x011F, 1}, /* LATIN SMALL LETTER G WITH BREVE */
	{0x015E, 0x015E, 1}, /* LATIN CAPITAL LETTER S WITH CEDILLA */
	{0x015F, 0x015F, 1}, /* LATIN SMALL LETTER S WITH CEDILLA */
	{0x0130, 0x0130, 1}, /* LATIN CAPITAL LETTER I WITH DOT ABOVE */
	{0x00E7, 0x00E7, 1}, /* LATIN SMALL LETTER C WITH CEDILLA */
	{0x20AC, 0x20AC, 1}, /* EURO SIGN */
	{0xFFFF, 0xFFFF, 0}
	};


/**
 *  7bit ASCII text that is not in the GSM default table, not in the
 *  Turkish tables and would be downgraded by the current internal converter.
 */
const TMappedChar  gNonTurkishDowngrading7bitGSMChars[] =
	{
	{0x00C0, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH GRAVE */
	{0x00C1, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH ACUTE */
	{0x00C2, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
	{0x00C3, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH TILDE */
	{0x00C8, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH GRAVE */
	{0x00CA, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
	{0x00CB, 0x0045, 1}, /* LATIN CAPITAL LETTER E WITH DIAERESIS */
	{0x00CC, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH GRAVE */
	{0x00CD, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH ACUTE */
	{0x00CE, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
	{0x00CF, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH DIAERESIS */
	{0x00D2, 0x004f, 1}, /* LATIN CAPITAL LETTER O WITH GRAVE */
	{0x00D3, 0x004f, 1}, /* LATIN CAPITAL LETTER O WITH ACUTE */
	{0x00D4, 0x004f, 1}, /* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
	{0x00D5, 0x004f, 1}, /* LATIN CAPITAL LETTER O WITH TILDE */
	{0x00D9, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH GRAVE */
	{0x00DA, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH ACUTE */
	{0x00DB, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
	{0x00DD, 0x0059, 1}, /* LATIN CAPITAL LETTER Y WITH ACUTE */
	{0x00E1, 0x0061, 1}, /* LATIN SMALL LETTER A WITH ACUTE */
	{0x00E2, 0x0061, 1}, /* LATIN SMALL LETTER A WITH CIRCUMFLEX */
	{0x00E3, 0x0061, 1}, /* LATIN SMALL LETTER A WITH TILDE */
	{0x00EA, 0x0065, 1}, /* LATIN SMALL LETTER E WITH CIRCUMFLEX */
	{0x00EB, 0x0065, 1}, /* LATIN SMALL LETTER E WITH DIAERESIS */
	{0x00ED, 0x0069, 1}, /* LATIN SMALL LETTER I WITH ACUTE */
	{0x00EE, 0x0069, 1}, /* LATIN SMALL LETTER I WITH CIRCUMFLEX */
	{0x00EF, 0x0069, 1}, /* LATIN SMALL LETTER I WITH DIAERESIS */
	{0x00F3, 0x006f, 1}, /* LATIN SMALL LETTER O WITH ACUTE */
	{0x00F4, 0x006f, 1}, /* LATIN SMALL LETTER O WITH CIRCUMFLEX */
	{0x00F5, 0x006f, 1}, /* LATIN SMALL LETTER O WITH TILDE */
	{0x00FA, 0x0075, 1}, /* LATIN SMALL LETTER U WITH ACUTE */
	{0x00FB, 0x0075, 1}, /* LATIN SMALL LETTER U WITH CIRCUMFLEX */
	{0x00FD, 0x0079, 1}, /* LATIN SMALL LETTER Y WITH ACUTE */
	{0x00FF, 0x0079, 1}, /* LATIN SMALL LETTER Y WITH DIAERESIS */
	{0x0386, 0x0041, 1}, /* GREEK CAPITAL LETTER ALPHA WITH TONOS */
	{0x0388, 0x0045, 1}, /* GREEK CAPITAL LETTER EPSILON WITH TONOS */
	{0x0389, 0x0048, 1}, /* GREEK CAPITAL LETTER ETA WITH TONOS */
	{0x038a, 0x0049, 1}, /* GREEK CAPITAL LETTER IOTA WITH TONOS */
	{0x038c, 0x004f, 1}, /* GREEK CAPITAL LETTER OMICRON WITH TONOS */
	{0x038e, 0x0059, 1}, /* GREEK CAPITAL LETTER UPSILON WITH TONOS */
	{0x038f, 0x03a9, 1}, /* GREEK CAPITAL LETTER OMEGA WITH TONOS */
	{0x0390, 0x0049, 1}, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS */
	{0x0391, 0x0041, 1}, /* GREEK CAPITAL LETTER ALPHA */
	{0x0392, 0x0042, 1}, /* GREEK CAPITAL LETTER BETA */
	{0x0395, 0x0045, 1}, /* GREEK CAPITAL LETTER EPSILON */
	{0x0396, 0x005a, 1}, /* GREEK CAPITAL LETTER ZETA */
	{0x0397, 0x0048, 1}, /* GREEK CAPITAL LETTER ETA */
	{0x0399, 0x0049, 1}, /* GREEK CAPITAL LETTER IOTA */
	{0x039a, 0x004b, 1}, /* GREEK CAPITAL LETTER KAPPA */
	{0x039c, 0x004d, 1}, /* GREEK CAPITAL LETTER MU */
	{0x039d, 0x004e, 1}, /* GREEK CAPITAL LETTER NU */
	{0x039f, 0x004f, 1}, /* GREEK CAPITAL LETTER OMICRON */
	{0x03a1, 0x0050, 1}, /* GREEK CAPITAL LETTER RHO */
	{0x03a4, 0x0054, 1}, /* GREEK CAPITAL LETTER TAU */
	{0x03A5, 0x0059, 1}, /* GREEK CAPITAL LETTER UPSILON */
	{0x03a7, 0x0058, 1}, /* GREEK CAPITAL LETTER CHI */
	{0x03aa, 0x0049, 1}, /* GREEK CAPITAL LETTER IOTA WITH DIALYTIKA */
	{0x03ab, 0x0059, 1}, /* GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA */
	{0x03ac, 0x0041, 1}, /* GREEK SMALL LETTER ALPHA WITH TONOS */
	{0x03ad, 0x0045, 1}, /* GREEK SMALL LETTER EPSILON WITH TONOS */
	{0x03ae, 0x0048, 1}, /* GREEK SMALL LETTER ETA WITH TONOS */
	{0x03af, 0x0049, 1}, /* GREEK SMALL LETTER IOTA WITH TONOS */
	{0x03b0, 0x0059, 1}, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS */
	{0x03B1, 0x0041, 1}, /* GREEK SMALL LETTER ALPHA */
	{0x03B2, 0x0042, 1}, /* GREEK SMALL LETTER BETA */
	{0x03B3, 0x0393, 1}, /* GREEK SMALL LETTER GAMMA */
	{0x03B4, 0x0394, 1}, /* GREEK SMALL LETTER DELTA */
	{0x03B5, 0x0045, 1}, /* GREEK SMALL LETTER EPSILON */
	{0x03B6, 0x005a, 1}, /* GREEK SMALL LETTER ZETA */
	{0x03B7, 0x0048, 1}, /* GREEK SMALL LETTER ETA */
	{0x03B8, 0x0398, 1}, /* GREEK SMALL LETTER THETA */
	{0x03B9, 0x0049, 1}, /* GREEK SMALL LETTER IOTA */
	{0x03BA, 0x004b, 1}, /* GREEK SMALL LETTER KAPPA */
	{0x03BB, 0x039b, 1}, /* GREEK SMALL LETTER LAMDA */
	{0x03BC, 0x004d, 1}, /* GREEK SMALL LETTER MU */
	{0x03BD, 0x004e, 1}, /* GREEK SMALL LETTER NU */
	{0x03BE, 0x039e, 1}, /* GREEK SMALL LETTER XI */
	{0x03bf, 0x004f, 1}, /* GREEK SMALL LETTER OMICRON */
	{0x03C0, 0x03a0, 1}, /* GREEK SMALL LETTER PI */
	{0x03C1, 0x0050, 1}, /* GREEK SMALL LETTER RHO */
	{0x03C2, 0x03a3, 1}, /* GREEK SMALL LETTER FINAL SIGMA */
	{0x03C3, 0x03a3, 1}, /* GREEK SMALL LETTER SIGMA */
	{0x03C4, 0x0054, 1}, /* GREEK SMALL LETTER TAU */
	{0x03C5, 0x0059, 1}, /* GREEK SMALL LETTER UPSILON */
	{0x03C6, 0x03a6, 1}, /* GREEK SMALL LETTER PHI */
	{0x03C7, 0x0058, 1}, /* GREEK SMALL LETTER CHI */
	{0x03C8, 0x03a8, 1}, /* GREEK SMALL LETTER PSI */
	{0x03C9, 0x03a9, 1}, /* GREEK SMALL LETTER OMEGA */
	{0x03ca, 0x0049, 1}, /* GREEK SMALL LETTER IOTA WITH DIALYTIKA */
	{0x03cb, 0x0059, 1}, /* GREEK SMALL LETTER UPSILON WITH DIALYTIKA */
	{0x03cc, 0x004f, 1}, /* GREEK SMALL LETTER OMICRON WITH TONOS */
	{0x03cd, 0x0059, 1}, /* GREEK SMALL LETTER UPSILON WITH TONOS */
	{0x03ce, 0x03a9, 1}, /* GREEK SMALL LETTER OMEGA WITH TONOS */
	//{0x03d0, 0x0042, 1}, /* GREEK BETA SYMBOL */
	//{0x03D1, 0x0398, 1}, /* GREEK THETA SYMBOL */
	//{0x03D2, 0x0059, 1}, /* GREEK UPSILON WITH HOOK SYMBOL */
	//{0x03d3, 0x0059, 1}, /* GREEK UPSILON WITH ACUTE AND HOOK SYMBOL */
	//{0x03d4, 0x0059, 1}, /* GREEK UPSILON WITH DIAERESIS AND HOOK SYMBOL */
	//{0x03D5, 0x03a6, 1}, /* GREEK PHI SYMBOL */
	//{0x2126, 0x03a9, 1}, /* OHM SIGN */
	//{0x220F, 0x03a0, 1}, /* N-ARY PRODUCT */
	//{0x2211, 0x03a3, 1}, /* N-ARY SUMMATION */
	{0xFFFF, 0xFFFF, 0}
	};


/**
 *  Turkish 7bit ASCII text that is in the shift table (and
 *  not the default shift table)..
 */
const TMappedChar  gTurkish7bitGSMShiftChars[] =
	{
	{0x011E, 0x011E, 2}, /* LATIN CAPITAL LETTER G WITH BREVE */
	{0x0130, 0x0130, 2}, /* LATIN CAPITAL LETTER I WITH DOT ABOVE */
	{0x015E, 0x015E, 2}, /* LATIN CAPITAL LETTER S WITH CEDILLA */
	{0x00E7, 0x00E7, 2}, /* LATIN SMALL LETTER C WITH CEDILLA */
	{0x011F, 0x011F, 2}, /* LATIN SMALL LETTER G WITH BREVE */
	{0x0131, 0x0131, 2}, /* LATIN SMALL LETTER DOTLESS I */
	{0x015F, 0x015F, 2}, /* LATIN SMALL LETTER S WITH CEDILLA */
	{0xFFFF, 0xFFFF, 0}
	};

/**
 *  Spanish 7bit ASCII text that is in the GSM Spanish table but
 *  is downgraded (using PREQ2090).
 */
const TMappedChar  gDowngradingSpanish7bitGSM[] = 
    {
    {0x00C1, 0x0041, 1}, /* LATIN CAPITAL LETTER A WITH ACUTE */
    {0x00CD, 0x0049, 1}, /* LATIN CAPITAL LETTER I WITH ACUTE */
    {0x00D3, 0x004F, 1}, /* LATIN CAPITAL LETTER O WITH ACUTE */
    {0x00DA, 0x0055, 1}, /* LATIN CAPITAL LETTER U WITH ACUTE */
    {0x00E1, 0x0061, 1}, /* LATIN SMALL LETTER A WITH ACUTE */
    {0x00ED, 0x0069, 1}, /* LATIN SMALL LETTER I WITH ACUTE */
    {0x00F3, 0x006F, 1}, /* LATIN SMALL LETTER O WITH ACUTE */
    {0x00FA, 0x0075, 1}, /* LATIN SMALL LETTER U WITH ACUTE */
    {0xFFFF, 0xFFFF, 0}
    };

/**
 *  Spanish 7bit ASCII text that is in the shift table (and
 *  not the default shift table and not in the Turkish shift table)..
 */
const TMappedChar  gSpanish7bitGSMShiftChars[] = 
    {
    {0x00C1, 0x00C1, 2}, /* LATIN CAPITAL LETTER A WITH ACUTE */
    {0x00CD, 0x00CD, 2}, /* LATIN CAPITAL LETTER I WITH ACUTE */
    {0x00D3, 0x00D3, 2}, /* LATIN CAPITAL LETTER O WITH ACUTE */
    {0x00DA, 0x00DA, 2}, /* LATIN CAPITAL LETTER U WITH ACUTE */
    {0x00E1, 0x00E1, 2}, /* LATIN SMALL LETTER A WITH ACUTE */
    {0x00ED, 0x00ED, 2}, /* LATIN SMALL LETTER I WITH ACUTE */
    {0x00F3, 0x00F3, 2}, /* LATIN SMALL LETTER O WITH ACUTE */
    {0x00FA, 0x00FA, 2}, /* LATIN SMALL LETTER U WITH ACUTE */
    {0xFFFF, 0xFFFF, 0}
    };

/**
 *  Portuguese 7bit ASCII text that is in the GSM Portuguese table but
 *  is downgraded (using PREQ2090).
 */
const TMappedChar  gDowngradingPortuguese7bitGSM[] = 
    {
    {0x00C0, 0x0041, 1}, /*   �   LATIN CAPITAL LETTER A WITH GRAVE */
    {0x00C2, 0x0041, 1}, /*   �   LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
    {0x00EA, 0x0065, 1}, /*   �   LATIN SMALL LETTER E WITH CIRCUMFLEX */
    {0x00D4, 0x004F, 1}, /*   �   LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
    {0x00C3, 0x0041, 1}, /*   �   LATIN CAPITAL LETTER A WITH TILDE */
    {0x00E3, 0x0061, 1}, /*   �   LATIN SMALL LETTER A WITH TILDE */
    {0x00F4, 0x006F, 1}, /*   �   LATIN SMALL LETTER O WITH CIRCUMFLEX */
    {0x00D5, 0x004F, 1}, /*   �   LATIN CAPITAL LETTER O WITH TILDE */
    {0x00F5, 0x006F, 1}, /*   �   LATIN SMALL LETTER O WITH TILDE */
    {0x00CA, 0x0045, 1}, /*   �   LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
    {0x00E2, 0x0061, 1}, /*   �   LATIN SMALL LETTER A WITH CIRCUMFLEX */
    {0xFFFF, 0xFFFF, 0}
    };


/**
 *  Selection of Portuguese 7bit ASCII text that are in the GSM Portuguese table (and
 *  not the default table).
 */
const TMappedChar  gPortuguese7bitGSMLockingChars[] = 
    {
    {0x0060, 0x0060, 1}, /* GRAVE ACCENT */
    {0x007C, 0x007C, 1}, /* VERTICAL LINE */
    {0x00C0, 0x00C0, 1}, /*   �   LATIN CAPITAL LETTER A WITH GRAVE */
    {0x00C2, 0x00C2, 1}, /*   �   LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
    {0x00EA, 0x00EA, 1}, /*   �   LATIN SMALL LETTER E WITH CIRCUMFLEX */
    {0x00E7, 0x00E7, 1}, /*   �   LATIN SMALL LETTER C WITH CEDILLA */
    {0x00D4, 0x00D4, 1}, /*   �   LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
    {0x00C3, 0x00C3, 1}, /*   �   LATIN CAPITAL LETTER A WITH TILDE */
    {0x00E3, 0x00E3, 1}, /*   �   LATIN SMALL LETTER A WITH TILDE */
    {0x00F4, 0x00F4, 1}, /*   �   LATIN SMALL LETTER O WITH CIRCUMFLEX */
    {0x00D5, 0x00D5, 1}, /*   �   LATIN CAPITAL LETTER O WITH TILDE */
    {0x00F5, 0x00F5, 1}, /*   �   LATIN SMALL LETTER O WITH TILDE */
    {0x00CA, 0x00CA, 1}, /*   �   LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
    {0x00E2, 0x00E2, 1}, /*   �   LATIN SMALL LETTER A WITH CIRCUMFLEX */
    {0x00AA, 0x00AA, 1}, /* FEMININE ORDINAL INDICATOR */
    {0x00BA, 0x00BA, 1}, /* MASCULINE ORDINAL INDICATOR */
    {0x221E, 0x221E, 1}, /* INFINITY */
    {0xFFFF, 0xFFFF, 0}
    };

/**
 *  Portuguese 7bit ASCII text that is in the shift table (and
 *  not the default shift table and not in the Turkish or Spanish shift table)..
 */
const TMappedChar  gPortuguese7bitGSMShiftChars[] = 
    {
    {0x00C0, 0x00C0, 2}, /*   �   LATIN CAPITAL LETTER A WITH GRAVE */
    {0x00C2, 0x00C2, 2}, /*   �   LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
    {0x00EA, 0x00EA, 2}, /*   �   LATIN SMALL LETTER E WITH CIRCUMFLEX */
    {0x00E7, 0x00E7, 2}, /*   �   LATIN SMALL LETTER C WITH CEDILLA */
    {0x00D4, 0x00D4, 2}, /*   �   LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
    {0x00C3, 0x00C3, 2}, /*   �   LATIN CAPITAL LETTER A WITH TILDE */
    {0x00E3, 0x00E3, 2}, /*   �   LATIN SMALL LETTER A WITH TILDE */
    {0x00F4, 0x00F4, 2}, /*   �   LATIN SMALL LETTER O WITH CIRCUMFLEX */
    {0x00D5, 0x00D5, 2}, /*   �   LATIN CAPITAL LETTER O WITH TILDE */
    {0x00F5, 0x00F5, 2}, /*   �   LATIN SMALL LETTER O WITH TILDE */
    {0x00CA, 0x00CA, 2}, /*   �   LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
    {0x00E2, 0x00E2, 2}, /*   �   LATIN SMALL LETTER A WITH CIRCUMFLEX */
    {0xFFFF, 0xFFFF, 0}
    };

/**
 *  Portuguese 7bit ASCII not in the locking shift table 
 */
const TMappedChar  gPortuguese7bitGSMExt[] = 
    {
    {0x007B, 0x007B, 2}, /* { LEFT CURLY BRACKET */
    {0x007D, 0x007D, 2}, /* } RIGHT CURLY BRACKET */
    {0xFFFF, 0xFFFF, 0}
    };

/**
 *  7bit ASCII in standard table but not in the Portuguese locking shift table 
 *  and the Portuguese shift table. 
 */
const TMappedChar  gPortuguese7bitGSMShiftAndStd1[] = 
    {
    {0x03A6, 0x03A6, 1}, /* GREEK CAPITAL LETTER PHI */
    {0x0393, 0x0393, 1}, /* GREEK CAPITAL LETTER GAMMA */
    {0xFFFF, 0xFFFF, 0}
    };
const TMappedChar  gPortuguese7bitGSMShiftAndStd2[] = 
    {
    {0x039B, 0x039B, 1}, /* GREEK CAPITAL LETTER LAMDA */
    {0x03A9, 0x03A9, 1}, /* GREEK CAPITAL LETTER OMEGA */
    {0xFFFF, 0xFFFF, 0}
    };

/**
 *  Standard static constructor.
 */
CSmsPrtTestAlphabet* CSmsPrtTestAlphabet::NewL()
	{
	CSmsPrtTestAlphabet*  self = new(ELeave) CSmsPrtTestAlphabet();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	} // CSmsPrtTestAlphabet::NewL


/**
 *  Standard constructor.
 */
CSmsPrtTestAlphabet::CSmsPrtTestAlphabet()
	{
	//NOP
	} // CSmsPrtTestAlphabet::CSmsPrtTestAlphabet


/**
 *  Standard destructor.
 */
CSmsPrtTestAlphabet::~CSmsPrtTestAlphabet()
	{
	iCharSetNames.Reset();
	iCharSetArrays.Reset();
	iCharSetSizes.Reset();
	iCharPositions.Reset();
	} // CSmsPrtTestAlphabet::~CSmsPrtTestAlphabet


/**
 *  Standard second phase constructor.
 */
void CSmsPrtTestAlphabet::ConstructL()
	{
	//
	// Create the list of character set names...
	//
	iCharSetNames.Reset();
	iCharSetArrays.Reset();

	iCharSetNames.AppendL(TPtrC(KCharacterSetStandard7bitGSM));
	iCharSetArrays.AppendL(gStandard7bitGSMChars);

	iCharSetNames.AppendL(TPtrC(KCharacterSetStandard7bitGSMShift));
	iCharSetArrays.AppendL(gStandard7bitGSMShiftChars);

	iCharSetNames.AppendL(TPtrC(KCharacterSetStandard8bitGSM));
	iCharSetArrays.AppendL(gStandard8bitGSMChars);

	iCharSetNames.AppendL(TPtrC(KCharacterSetVariousUCS2));
	iCharSetArrays.AppendL(gVariousUCS2Chars);

	iCharSetNames.AppendL(TPtrC(KCharacterSetUnconvertible7bitGSM));
	iCharSetArrays.AppendL(gUnconvertible7bitGSMShiftChars);
	
	iCharSetNames.AppendL(TPtrC(KCharacterSetDowngrading7bitGSM));
	iCharSetArrays.AppendL(gDowngrading7bitGSMChars);

	iCharSetNames.AppendL(TPtrC(KCharacterSetDowngrading7bitGSMExtended));
	iCharSetArrays.AppendL(gDowngrading7bitGSMCharsExtended);

	iCharSetNames.AppendL(TPtrC(KCharacterSetDowngradingTurkish7bitGSM));
	iCharSetArrays.AppendL(gDowngradingTurkish7bitGSM);

	iCharSetNames.AppendL(TPtrC(KCharacterSetTurkish7bitGSMLocking));
	iCharSetArrays.AppendL(gTurkish7bitGSMLockingChars);

	iCharSetNames.AppendL(TPtrC(KCharacterSetTurkish7bitGSMShift));
	iCharSetArrays.AppendL(gTurkish7bitGSMShiftChars);

	iCharSetNames.AppendL(TPtrC(KCharacterSetNonTurkishDowngrading7bitGSMChars));
	iCharSetArrays.AppendL(gNonTurkishDowngrading7bitGSMChars);

    iCharSetNames.AppendL(TPtrC(KCharacterSetDowngradingSpanish7bitGSM));
    iCharSetArrays.AppendL(gDowngradingSpanish7bitGSM);

    iCharSetNames.AppendL(TPtrC(KCharacterSetSpanish7bitGSMShift));
    iCharSetArrays.AppendL(gSpanish7bitGSMShiftChars);

    iCharSetNames.AppendL(TPtrC(KCharacterSetDowngradingPortuguese7bitGSM));
    iCharSetArrays.AppendL(gDowngradingPortuguese7bitGSM);

    iCharSetNames.AppendL(TPtrC(KCharacterSetPortuguese7bitGSMLocking));
    iCharSetArrays.AppendL(gPortuguese7bitGSMLockingChars);

    iCharSetNames.AppendL(TPtrC(KCharacterSetPortuguese7bitGSMShift));
    iCharSetArrays.AppendL(gPortuguese7bitGSMShiftChars);
    
    iCharSetNames.AppendL(TPtrC(KCharacterSetPortuguese7bitGSMExt));
    iCharSetArrays.AppendL(gPortuguese7bitGSMExt);    
    
    iCharSetNames.AppendL(TPtrC(KCharacterSetPortuguese7bitGSMShiftAndStd1));
    iCharSetArrays.AppendL(gPortuguese7bitGSMShiftAndStd1);    
    
    iCharSetNames.AppendL(TPtrC(KCharacterSetPortuguese7bitGSMShiftAndStd2));
    iCharSetArrays.AppendL(gPortuguese7bitGSMShiftAndStd2);    
    
	//
	// Setup the current character set sizes and positions...
	//
	TInt  index;
	
	iCharSetSizes.Reset();
	iCharPositions.Reset();

	for (index = 0;  index < iCharSetNames.Count();  index++)
		{
		const TMappedChar*  charSet = iCharSetArrays[index];
		TInt  charSetSize = 0;
		
		while (charSet[charSetSize].iOriginal != 0xFFFF  &&
		       charSet[charSetSize].iExpected != 0xFFFF)
			{
			charSetSize++;
			}
		
		if (charSetSize == 0)
			{
			User::Leave(KErrNotFound);
			}
		
		iCharSetSizes.AppendL(charSetSize);
		iCharPositions.AppendL(0);
		}
	} // CSmsPrtTestAlphabet::ConstructL


/**
 *  Finds the index of the character set in the mapping table.
 * 
 *  @param aCharSet  Character set to look for.
 */
TInt CSmsPrtTestAlphabet::GetCharacterSetIndexL(const TDesC& aCharSet)
	{
	//
	// Search for the character set...
	//
	TInt  index;
	TInt count = iCharSetNames.Count();
	
	for (index = 0;  index < count;  ++index)
		{
		if (iCharSetNames[index] == aCharSet)
			{
			break;
			}
		}
	
	if (index >= iCharSetNames.Count())
		{
		User::Leave(KErrNotFound);
		}
	
	return index;
	} // CSmsPrtTestAlphabet::GetCharacterSetIndexL


/**
 *  Gets the number of characters in the character set.
 * 
 *  @param aCharSet  Character set to count.
 */
TInt CSmsPrtTestAlphabet::GetCharacterSetSizeL(const TDesC& aCharSet)
	{
	TInt  index = GetCharacterSetIndexL(aCharSet);
	
	return iCharSetSizes[index];
	} // CSmsPrtTestAlphabet::GetCharacterSetSizeL


/**
 *  Gets the current position in character set. This is the index of the next
 *  character to append.
 * 
 *  @param aCharSet  Character set to get.
 */
TInt CSmsPrtTestAlphabet::GetCharacterSetPositionL(const TDesC& aCharSet)
	{
	TInt  index = GetCharacterSetIndexL(aCharSet);
	
	return iCharPositions[index];
	} // CSmsPrtTestAlphabet::GetCharacterSetPositionL


/**
 *  Sets the current position in character set. This is the index of the next
 *  character to append.
 * 
 *  @param aCharSet   Character set to set position of.
 *  @param aPosition  Position value.
 */
void CSmsPrtTestAlphabet::SetCharacterSetPositionL(const TDesC& aCharSet, TInt aPosition)
	{
	TInt  index = GetCharacterSetIndexL(aCharSet);
	
	iCharPositions[index] = (aPosition) % iCharSetSizes[index];
	} // CSmsPrtTestAlphabet::SetCharacterSetPositionL


/**
 *  Resets the current position in all character set to the start. This is the
 *  index of the next character to append and therefore all state information
 *  is reset.
 */
void CSmsPrtTestAlphabet::ResetAllCharacterSetPositionsL()
	{
	//
	// For each character set, reset the position...
	//
	TInt index;
	TInt count = iCharSetNames.Count();
	for (index = 0;  index < count;  ++index)
		{
		iCharPositions[index] = 0;
		}
	} // CSmsPrtTestAlphabet::ResetAllCharacterSetPositionsL


TBool CSmsPrtTestAlphabet::IsCharListedInCharacterSetL(const TDesC& aCharSet, TChar aChar)
	{
	TInt  index = GetCharacterSetIndexL(aCharSet);
	const TMappedChar*  charSet = iCharSetArrays[index];

	//
	// Search the character set...
	//
	TInt position = 0;
	
	while (charSet[position].iOriginal != 0xFFFF  &&
	       charSet[position].iExpected != 0xFFFF)
		{
		if (charSet[position].iOriginal == aChar)
			{
			return ETrue;
			}
		
		position++;
		}
	
	return EFalse;
	} // CSmsPrtTestAlphabet::IsCharListedInCharacterSetL


TBool CSmsPrtTestAlphabet::IsCharDowngradedByCharacterSetL(const TDesC& aCharSet, TChar aChar)
	{
	TInt  index = GetCharacterSetIndexL(aCharSet);
	const TMappedChar*  charSet = iCharSetArrays[index];

	//
	// Search the character set...
	//
	TInt position = 0;
	
	while (charSet[position].iOriginal != 0xFFFF  &&
	       charSet[position].iExpected != 0xFFFF)
		{
		if (charSet[position].iOriginal == aChar  &&
				charSet[position].iOriginal != charSet[position].iExpected)
			{
			return ETrue;
			}
		
		position++;
		}
	
	return EFalse;
	} // CSmsPrtTestAlphabet::IsCharDowngradedByCharacterSetL


/**
 *  Appends a number of characters from the character set given to two
 *  descriptors. The original descriptor will contain the original text
 *  and the expected descriptor will contain text that is downgraded in
 *  the table.
 * 
 *  @param aCharSet             Character set to use.
 *  @param aCount               Number of characters to add.
 *  @param aOriginal            Descriptor to append original text to.
 *  @param aExpected            Descriptor to append expected text to.
 *  @param aOctetsCount         Incremented by the number of octets added.
 *  @param aDowngradesCount     Incremented by the number of downgraded
 *                              chars added.
 *  @param aUnconvertibleCount  Incremented by the number of unconvertible
 *                              chars added.
 */
void CSmsPrtTestAlphabet::AppendCharactersL(const TDesC& aCharSet, TInt aCount,
		                                    TDes& aOriginal, TDes& aExpected,
		                                    TInt& aOctetsCount, TInt& aDowngradesCount,
		                                    TInt& aUnconvertibleCount)
	{
	TInt  index = GetCharacterSetIndexL(aCharSet);
	const TMappedChar*  charSet = iCharSetArrays[index];

	//
	// Add the number of characters required...
	//
	while (aCount > 0)
		{
		TInt position = iCharPositions[index];
		aOriginal.Append(charSet[position].iOriginal);
		aExpected.Append(charSet[position].iExpected);
		
		aOctetsCount += charSet[position].iOctets;
		
		if (charSet[position].iOriginal !=
						charSet[position].iExpected)
			{
			if (charSet[position].iExpected != '?')
				{
				aDowngradesCount++;
				}
			else
				{
				aUnconvertibleCount++;
				}
			}
		
		TInt sizeCharSet = iCharSetSizes[index];
		TInt newPosition = (position + 1) % sizeCharSet;
		iCharPositions[index] = newPosition;
		
		--aCount;
		}
	} // CSmsPrtTestAlphabet::AppendCharactersL

