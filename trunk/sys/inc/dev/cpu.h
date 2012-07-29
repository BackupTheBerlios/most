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
 * Global variable defining loop circles needed for one msec.
 * The correct value for this determined in the BSP.
 */
extern unsigned long DEV_loops_per_msec;

/**
 * Convert usec to loop circles.
 */
#define DEV_USEC_2_LOOPS(DEV_usec) ( (DEV_loops_per_msec * (DEV_usec)) / 1000L  )

/*-------------- Interface -----------------------------------------------*/

/**
 * Spin the CPU.
 *
 * this is not finished, there should be a parameter for the speed the cpu is running.
 * and loop should be a fixed timing.
 * this function is used for initializing hardware were you have to wait short time.
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
