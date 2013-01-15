/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/cpu.h>

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

/** Stack grows from hi addresses to lo addresses */
#define USO_HI2LOW 21

/** Stack grows from low addresses to hi addresses */
#define USO_LOW2HI 12


#ifdef ACE_CPU
#if ACE_CPU==ACE_CPU_H8300
#include <uso/arch/h8300h.h>
#elif ACE_CPU==ACE_CPU_I386
#include <uso/arch/i386.h>
#elif ACE_CPU==ACE_CPU_ARM
#include <uso/arch/arm.h>
#else
#error "ACE CPU not supported!"
#endif
#else
#error "ACE CPU not defined!"
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
