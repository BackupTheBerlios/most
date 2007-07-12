/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/*
 ** low level support for 93c46b 1 kbit serial eeprom
 */

#ifndef EE_93C46_H
#define EE_93C46_H

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup 93C46 93C46.h
 *
 * 93c46b 1 kbit serial eeprom
 *
 * @{
 */


/**
 * Initialize.
 */
extern void EE_93C46_init (void);

/**
 * Erase eeprom.
 */
extern void EE_93C46_eraseall (void);

/**
 * Write to eeprom.
 *
 * @param addr
 * @param len
 * @param data
 */
extern void EE_93C46_write (unsigned int addr, unsigned int len, void *data);

/**
 * Read from eeprom.
 *
 * @param addr
 * @param len
 * @param data
 */
extern unsigned int EE_93C46_read (unsigned int addr,
                                   unsigned int len, void *data);

/** @}
 */

/** @}
 */

#endif
