/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DPY_GU256X64SER_USER_H
#define DPY_GU256X64SER_USER_H

/** @addtogroup gu256x64ser
 *
 * @{
 */

/** @defgroup user user.h
 *
 * User set up mode commands.
 *
 * @{
 */

/**
 * .
 */
extern void DPY_gu256x64ser_UserSetUpModeStart (void);

/**
 * .
 */
extern void DPY_gu256x64ser_UserSetUpModeEnd (void);

/**
 * .
 */
extern void DPY_gu256x64ser_MemorySwSetting (unsigned char memorySw, unsigned char data);

/**
 * .
 */
extern void DPY_gu256x64ser_MemorySwDataSend (unsigned char data);

/**
 * .
 */
extern void DPY_gu256x64ser_FROMImageDefinition (unsigned long address,
                                    unsigned long length, unsigned char *data);

/** 
 * .
 */
extern void DPY_gu256x64ser_SaveDownloadedChar (unsigned char fontType);

/**
 * .
 */
extern void DPY_gu256x64ser_FROMMacroDefinition (unsigned char registration,
                                    unsigned short length,
                                    unsigned char displayTime,
                                    unsigned char idleTime,
                                    unsigned char *data);

/**
 * .
 */
extern void DPY_gu256x64ser_DownloadCharTransfer (unsigned char fontType);

/**
 * .
 */
extern void DPY_gu256x64ser_DisplayStatusSendData (unsigned char status,
                                      unsigned char address,
                                      unsigned char length);

/** @}
 */

/** @}
 */

#endif
