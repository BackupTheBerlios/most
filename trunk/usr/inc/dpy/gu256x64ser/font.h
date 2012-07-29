/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DPY_GU256X64SER_FONT_H
#define DPY_GU256X64SER_FONT_H

/** @addtogroup dpy
 *
 * @{
 */

/** @addtogroup gu256x64ser
 *
 * @{
 */

/** @defgroup gu256x64ser_font font.h
 *
 * Font command group. 
 *
 * @{
 */

/**
 * .
 */
extern void DPY_gu256x64ser_FontSizeSelect (unsigned char fontSize);

/**
 * .
 */
extern void DPY_gu256x64ser_16x16CharDefinition (unsigned short charCode, unsigned char *data);

/**
 * .
 */
extern void DPY_gu256x64ser_16x16dotMode (unsigned char specify);

/**
 * .
 */
extern void DPY_gu256x64ser_Cancel16x16DownloadedChar (unsigned short charCode);

/**
 * .
 */
extern void DPY_gu256x64ser_FontMagnifiedDisplay (unsigned char x, unsigned char y);

/**
 * .
 */
extern void DPY_gu256x64ser_CharBoldDisplay (unsigned char bold);

/** @}
 */

/** @}
 */

/** @}
 */


#endif
