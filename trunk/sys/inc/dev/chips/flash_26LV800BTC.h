/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef FLASH_26LV800BTC_H
#define FLASH_26LV800BTC_H

#include <ace/stddef.h>
#include <dev/err.h>

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
extern ACE_err_t FLASH_26LV800BTC_sector_erase_is (volatile unsigned short *sector_addr);

/**
 * @param addr
 * @param data
 * @param len
 * @return errorcode
 */
extern ACE_err_t FLASH_26LV800BTC_programm_is (volatile unsigned short *addr,
                const char *data,
                ACE_size_t len);

extern ACE_err_t
FLASH_26LV800BTC_get_id_is (volatile unsigned short *mf, volatile unsigned short *device);

/** @}
 */

/** @}
 */

/** @}
 */


#endif
