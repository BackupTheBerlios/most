/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DPY_GU256X64SER_IMAGE_H
#define DPY_GU256X64SER_IMAGE_H

/** @addtogroup dpy
 *
 * @{
 */

/** @addtogroup gu256x64ser
 *
 * @{
 */

/** @defgroup image image.h
 *
 * Bit image display group.
 *
 * @{
 */

/**
 * 
 */
extern void DPY_gu256x64ser_RAMImageDefinition (unsigned long address,
                                                unsigned long length, unsigned char *data);

/**
 * 
 */
extern void DPY_gu256x64ser_DownloadImageDisplay (unsigned char displayMemory,
                                                  unsigned long imageAddress,
                                                  unsigned short image,
                                                  unsigned short x, unsigned short y);

/**
 * 
 */
extern void DPY_gu256x64ser_RealTimeImageDisplay (unsigned short x,
                                                  unsigned short y, unsigned char *data);

/** @}
 */

/** @}
 */

/** @}
 */


#endif
