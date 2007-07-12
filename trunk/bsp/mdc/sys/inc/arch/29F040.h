/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef FLASH_29F040_H
#define FLASH_29F040_H

#include <ace/stddef.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup 29F040 29F040.h
 *
 * Flash.
 *
 * @{
 */

enum FLASH_29F040_err_code
{
    FLASH_29F040_ok,
    FLASH_29F040_timeout,
    FLASH_29F040_addr
};

/**
 * @return size of a flash segment.
 */
extern unsigned long FLASH_29F040_segment_size (void);

/**
 * @param sector_addr
 * @return errorcode
 */
extern enum FLASH_29F040_err_code FLASH_29F040_sector_erase_s (volatile unsigned char
							     *sector_addr);

/**
 * @param sector_addr
 * @return errorcode
 */
extern enum FLASH_29F040_err_code FLASH_29F040_sector_erase_ns (volatile unsigned char
							     *sector_addr)
      _SECTION_ (".ramcode");

/**
 * @param addr
 * @param data
 * @param len
 * @return errorcode
 */
extern enum FLASH_29F040_err_code FLASH_29F040_programm_s (volatile unsigned char *addr,
							 unsigned char *data,
							 unsigned long len);

/**
 * @param addr
 * @param data
 * @param len
 * @return errorcode
 */
extern enum FLASH_29F040_err_code FLASH_29F040_programm_ns (volatile unsigned char *addr,
							 unsigned char *data,
							 unsigned long len)
      _SECTION_ (".ramcode");

/** @}
 */

/** @}
 */

#endif
