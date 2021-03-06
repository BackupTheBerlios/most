/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#ifndef DEV_CPU_H
#define DEV_CPU_H

/** @addtogroup dev
 * @{
 */


/** @defgroup dev_cpu cpu.h
 *
 * Cpu architecture low level.
 *
 * @{
 */

/**
 * Global variable defining loop circles needed for one millisec.
 * The correct value for this determined in the BSP.
 * This must not be long long because than calculation takes too long,
 * for example la2: with long long the lowest cpudelay would be 100 usec, and
 * with long the lowest cpudelay would be 20 usec!
 * 
 */
extern unsigned long DEV_loops_per_msec;

/**
 * Convert usec to loop circles.
 */
#define DEV_USEC_2_LOOPS(DEV_usec) ( ( DEV_loops_per_msec * (DEV_usec)) / 1000L  )

/*-------------- Interface -----------------------------------------------*/

/**
 * Spin the CPU.
 *
 *
 * @param loop : Spin the cpu n loops.  
 */
extern void DEV_cpudelay (unsigned long loop);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */


#endif
