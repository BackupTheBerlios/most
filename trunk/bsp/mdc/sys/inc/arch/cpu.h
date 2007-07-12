/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_CPU_H
#define MDC_CPU_H

#include <ace/stddef.h>
#include <dev/arch/h8300/io_ports.h>
#include <dev/arch/h8300/io_def.h>
#include <dev/arch/h8300/bitops.h>

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

/** Set WDT and refresh controller requests to high priority */

#define MDC_SET_PRIORITY_WDT_RFSH() { h8_IPRA |=  H8_IPRA_IPRA3; }
/**
 * Initializes the hardware and is called once in the startup.
 */
void MDC_cpu_init (void);

/**
 * Triggers the external watchdog which should happen at least once every 1.6 sec.
 */
_INLINE_ static void
MDC_watchdog_trigger (void)
{
    H8_BITNOT (H8_P45_WDI, h8_P4DR);
}


/**
 * Switch the SCI1 Rx and Tx lines of the CPU to the standard SS1 connection. 
 * 
 */
_INLINE_ static void
MDC_sci1_2_ss1 (void)
{
    H8_BITCLEAR (H8_P46_SEL0, h8_P4DR);
    H8_BITCLEAR (H8_P47_SEL1, h8_P4DR);
}


/** @}
 */

/** @}
 */

#endif
