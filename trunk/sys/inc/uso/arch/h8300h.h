/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_H8300H_H
#define USO_H8300H_H

#include <ace/stddef.h>

/** @addtogroup uso
 * @{
 */

/** @defgroup uso_h8300h arch/h8300h.h
 *
 * H8300H architecture interface.
 * @{
 */

#define USO_STACK_HI2LOW
#undef USO_STACK_LOW2HI

typedef volatile unsigned char USO_cpu_status_t;
typedef unsigned long USO_stack_t;
typedef volatile unsigned long USO_cpu_register_t;

#define USO_CPU_PS_INIT ((USO_cpu_status_t)0x00)

struct USO_cpu
{
    USO_cpu_register_t er2;
    USO_cpu_register_t er3;
    USO_cpu_register_t er4;
    USO_cpu_register_t er5;
    USO_cpu_register_t er6;
    USO_cpu_register_t sp;
    USO_cpu_register_t pc;
    USO_cpu_status_t ps;
};

/** @}
 */

/** @}
 */


#endif
