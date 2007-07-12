/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#ifndef DEV_CPU_H
#define DEV_CPU_H

#include <ace/arch/cpu.h>

/** @defgroup cpu cpu.h
 *
 * Cpu architecture low level.
 *
 * @{
 */

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


#endif
