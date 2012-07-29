/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_CPU_H
#define MDC_CPU_H

#include <ace/stddef.h>
#include <dev/arch/h8/H83003_REG.h>
#include <dev/arch/h8/H83003.h>
#include <dev/arch/h8/bitops.h>

#include "arch/pins.h"

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

/**
 * Triggers the external watchdog which should happen at least once every 1.6 sec.
 */
extern void
MDC_watchdog_trigger (void)
ACE_SECTION_ (".ramcode");

/** @}
 */

/** @}
 */

#endif
