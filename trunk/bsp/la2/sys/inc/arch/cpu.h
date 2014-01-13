#ifndef LA2_CPU_H
#define LA2_CPU_H

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


/*--------------*/
/* Master Clock */
/*--------------*/

#define LA2_MAIN_CLOCK       6000000   /* Exetrnal ocilator MAINCLK */
#define LA2_SYSTEM_CLOCK     30000000L  /* 30 MHz */
#define LA2_EXT_CLOCK        32768     /* 32KHZ */

#define LA2_WDT_DISABLE      1       /**< Set to 1 for disabling watchdog */

/**
 *
 */
extern void LA2_cpu_init (void);

/**
 *
 */
extern void LA2_wdt_trigger (void);

/** Following functions are defined in reset.S */

/**
 * Jump to Application
 */
extern void LA2_jump_app (void);

/**
 * Jump to Bootloader
 */
extern void LA2_jump_boot (void);

/** @} */

/** @} */

#endif
