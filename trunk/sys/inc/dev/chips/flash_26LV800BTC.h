/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef FLASH_26LV800BTC_H
#define FLASH_26LV800BTC_H

#include <ace/stddef.h>

/** @addtogroup dev
 *
 * @{
 */

/** @addtogroup chips
 *
 * @{
 */

/** @defgroup 29F040 29F040.h
 *
 * Flash.
 *
 * @{
 */

enum FLASH_26LV800BTC_err_code
{
    FLASH_26LV800BTC_ok,
    FLASH_26LV800BTC_timeout,
    FLASH_26LV800BTC_addr,
    FLASH_26LV800BTC_len
};

/**
 * During flashing maybe your watchdog has to be triggered.
 * your trigger function must be located in ACE_SECTION_ (.ramcode)
 * better disable watchdog before flashing and enable after.
 * @param watchdog_trigger : your watchdog trigger function.
 */
extern void FLASH_26LV800BTC_set_wtd_trigger (void (*watchdog_trigger) (void));

/**
 * @return size of a flash segment.
 */
extern unsigned long FLASH_26LV800BTC_segment_size (void);

/**
 * @param sector_addr
 * @return errorcode
 */
extern enum FLASH_26LV800BTC_err_code FLASH_26LV800BTC_sector_erase_is (volatile unsigned short *sector_addr);

/**
 * @param addr
 * @param data
 * @param len
 * @return errorcode
 */
extern enum FLASH_26LV800BTC_err_code FLASH_26LV800BTC_programm_is (volatile unsigned short *addr,
                const char *data,
                ACE_size_t len);

extern enum FLASH_26LV800BTC_err_code
FLASH_26LV800BTC_get_id_is (unsigned short *mf, unsigned short *device);

/** @}
 */

/** @}
 */

/** @}
 */


#endif
