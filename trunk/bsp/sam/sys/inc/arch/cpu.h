#ifndef SAM_CPU_H
#define SAM_CPU_H

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup cpu cpu.h
 *
 * CPU.
 *
 * @{
 */

#define SAM_MAIN_OSC             18432000       /**< Main Oscillator MAINCK */
#define SAM_MCK          ((SAM_MAIN_OSC*73/14)/2)       /**< Output PLL Clock (48 MHz) Master Clock */
#define SAM_MCK_IN_MHZ    48

#define SAM_WDT_DISABLE 0       /**< Set to 1 for disabling watchdog */
#define SAM_WDV_2SEC    (0x00000200)   /**< Watchdog value for 2 seconds */
#define SAM_WDV_10SEC   (0x00000A00)   /**< Watchdog value for 10 seconds */

/**
 *
 */
extern void SAM_cpu_init (void);

/**
 *
 */
extern void SAM_wdt_disable (void);

/**
 *
 */
extern void SAM_wdt_setup (unsigned long wdv);

/**
 *
 */
extern void SAM_wdt_trigger (void);

/**
 * Jump to Application
 */
extern void SAM_jump_app (void);

/**
 * Jump to Bootloader
 */
extern void SAM_jump_boot (void);

/** @} */

/** @} */

#endif
