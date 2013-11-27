/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_CPU_H
#define USO_CPU_H

/** @addtogroup uso
 * @{
 */

/** @defgroup uso_cpu cpu.h
 *
 * Cpu architecture interface.
 * @{
 */

/*------------- Representation ------------------------------------------*/

#if defined MOST_CPU_I386
#include <uso/arch/i386.h>
#elif defined MOST_CPU_ARM
#include <uso/arch/arm.h>
#elif defined MOST_CPU_H8300
#include <uso/arch/h8300h.h>
#elif defined MOST_CPU_M68K
#error "CPU not supported!"
#else
#error "CPU not defined!"
#endif

/*------------------------------------------------------------------------*/

/**
 * CPU type.
 */
typedef struct USO_cpu USO_cpu_t;


/*-------------- Interface -----------------------------------------------*/

/**
 * Enable all interrupts.
 */
extern void USO_enable (void);

/**
 * Disable all interrupts and return the CCR.
 */
extern USO_cpu_status_t USO_disable (void);

/**
 * Restore the CCR.
 *
 * @param status : Saved status which has to be written to ccr.  
 */
extern void USO_restore (USO_cpu_status_t status);

/**
 * Do a context switch.
 *
 * @param old : from old
 * @param new : to new
 */
extern void USO_context_switch (USO_cpu_t * old, USO_cpu_t * new);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */


#endif
