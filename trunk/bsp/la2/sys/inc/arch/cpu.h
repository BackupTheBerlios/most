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
#define LA2_SYSTEM_CLOCK     30000000  /* 30 MHz */
#define LA2_EXT_CLOCK        32768     /* 32KHZ */

/**
 *
 */
extern void LA2_cpu_init (void);


/** @} */

/** @} */

#endif
