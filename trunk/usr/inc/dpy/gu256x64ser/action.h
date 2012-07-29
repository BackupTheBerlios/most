/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DPY_GU256X64SER_ACTION_H
#define DPY_GU256X64SER_ACTION_H

/** @addtogroup dpy
 *
 * @{
 */

/** @addtogroup gu256x64ser
 *
 * @{
 */

/** @defgroup action action.h
 *
 * Action commands
 *
 * @{
 */

/**
 * .
 */
extern void DPY_gu256x64ser_Wait (unsigned char time);

/**
 * .
 */
extern void DPY_gu256x64ser_Scroll (unsigned short screenShift,
                                    unsigned short reptition, unsigned char speed);

/**
 * .
 */
extern void DPY_gu256x64ser_Blink (unsigned char pattern,
                                   unsigned char normalTime,
                                   unsigned char brankTime, unsigned char reptition);

/**
 * .
 */
extern void DPY_gu256x64ser_Curtain (unsigned char direction,
                                     unsigned char speed, unsigned char pattern);

/**
 * .
 */
extern void DPY_gu256x64ser_Spring (unsigned char direction,
                                    unsigned char speed, unsigned short patternAddress);

/**
 * .
 */
extern void DPY_gu256x64ser_Random (unsigned char speed, unsigned short patternAddress);

/** @}
 */

/** @}
 */

/** @}
 */

#endif
