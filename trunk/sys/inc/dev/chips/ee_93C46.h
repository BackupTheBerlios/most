/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/*
 ** low level support for 93c46b 1 kbit serial eeprom
 */

#ifndef EE_93C46_H
#define EE_93C46_H

#include <ace/err.h>
#include <dev/spi.h>

/** @addtogroup dev
 *
 * @{
 */

/** @addtogroup chips
 *
 * @{
 */

/** @defgroup ee_93C46 ee_93C46.h
 *
 * 93c46b 1 kbit serial eeprom
 *
 * @{
 */


struct EE_93C46
{
    DEV_spi_dev_t *spi;
};

typedef struct EE_93C46 EE_93C46_t;


/**
 * Initialize.
 */
extern ACE_err_t EE_93C46_init (EE_93C46_t *ee, DEV_spi_dev_t *spi);

/**
 * Erase eeprom.
 */
extern void EE_93C46_eraseall (EE_93C46_t *ee);

/**
 * Write to eeprom.
 *
 * @param addr
 * @param len
 * @param data
 */
extern void EE_93C46_write (EE_93C46_t *ee, unsigned int addr, unsigned int len, unsigned short *data);

/**
 * Read from eeprom.
 *
 * @param addr
 * @param len
 * @param data
 */
extern unsigned int EE_93C46_read (EE_93C46_t *ee, unsigned int addr, unsigned int len, unsigned short *data);

/** @}
 */

/** @}
 */

/** @}
 */

#endif
