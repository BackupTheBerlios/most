/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DPY_GU256X64SER_DISPLAY_H
#define DPY_GU256X64SER_DISPLAY_H

/** @addtogroup gu256x64ser
 *
 * Display.
 *
 * @{
 */

/** @defgroup display display.h
 *
 * Display.
 *
 * @{
 */

#include <ace/stdlib.h>

/**
 * .
 */
extern void DPY_gu256x64ser_WriteChar2Display (const unsigned char data);

/**
 * .
 */
extern void DPY_gu256x64ser_WriteArray2Display (const unsigned char *data, size_t len);

/**
 * .
 */
extern int DPY_gu256x64ser_CharCommand (const unsigned char command);

/**
 * .
 */
extern void DPY_gu256x64ser_CursorSet (unsigned short x, unsigned short y);

/**
 * .
 */
extern void DPY_gu256x64ser_InitializeDisplay (void);

/**
 * .
 */
extern void DPY_gu256x64ser_DownloadRegister (unsigned char enable);

/**
 * .
 */
extern void DPY_gu256x64ser_DownloadCharDefinition (unsigned char type,
                                       unsigned char start,
                                       unsigned char end, unsigned char *data);
/**
 * .
 */
extern void DPY_gu256x64ser_DeleteDownloadedChar (unsigned char select, unsigned char code);

/**
 * .
 */
extern void DPY_gu256x64ser_InternalFontSet (unsigned char fontSet);

/**
 * .
 */
extern void DPY_gu256x64ser_CharCodeType (unsigned char codeType);

/**
 * .
 */
extern void DPY_gu256x64ser_OverwriteMode (void);

/**
 * .
 */
extern void DPY_gu256x64ser_VerticalScrollMode (void);

/**
 * .
 */
extern void DPY_gu256x64ser_HorizontalScrollMode (void);

/**
 * .
 */
extern void DPY_gu256x64ser_HorizontalScrollSpeed (unsigned char speed);

/**
 * .
 */
extern void DPY_gu256x64ser_ReverseDisplay (unsigned char mode);

/**
 * .
 */
extern void DPY_gu256x64ser_MixtureDisplayMode (unsigned char mode);

/**
 * .
 */
extern void DPY_gu256x64ser_BrightnessLevel (unsigned char level);

/**
 * .
 */
extern void DPY_gu256x64ser_RAMMacroDefinition (unsigned short length, unsigned char *data);

/**
 * .
 */
extern void DPY_gu256x64ser_MacroExecution (unsigned char definitionNr,
                               unsigned char displayTime,
                               unsigned char idleTime);

/** @}
 */

/** @}
 */

#endif
