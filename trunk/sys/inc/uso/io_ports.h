/*
 * io_ports.h
 *
 *  Created on: 01.12.2013
 *      Author: maik
 */

#ifndef USO_IO_PORTS_H_
#define USO_IO_PORTS_H_

#if defined MOST_CPU_I386
#include <uso/arch/i386/io_ports.h>
#elif defined MOST_CPU_ARM
#include <uso/arch/arm/io_ports.h>
#elif defined MOST_CPU_H8300
#include <uso/arch/h8300/io_ports.h>
#elif defined MOST_CPU_M68K
#error "CPU not supported!"
#else
#error "CPU not defined!"
#endif


#endif /* IO_PORTS_H_ */
