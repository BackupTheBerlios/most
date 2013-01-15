/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_CPU_H
#define MDC_CPU_H

#include <dev/arch/h8/H83003_REG.h>
#include <dev/arch/h8/H83003.h>


/** @addtogroup arch
 *
 * @{
 */

/** @defgroup cpu cpu.h
 *
 * CPU
 *
 * @{
 */



/**
 * Initializes the hardware and is called once in the startup.
 */
void MDC_cpu_init (void);


/** @}
 */

/** @}
 */

#endif
