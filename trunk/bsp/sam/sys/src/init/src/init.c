/*
 * This file is part of MOST.
 *
 * MOST is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MOST is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with MOST.  If not, see <http://www.gnu.org/licenses/>.
 */	
    
#include <ace/arch/cpu.h>
#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/scheduler.h>
#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/log.h>
#include <dev/arch/at91/debug.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/arch/cpu.h>
#include <cli/interpreter.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>

#include "arch/pins.h"
#include "arch/exceptions.h"
#include "arch/uart.h"
#include "arch/eth.h"
#include "arch/ticks.h"
#include "arch/digio.h"
#include "init/main.h"
#include "init/init.h"
#include "init/start.h"

#define START_STACK_SIZE       0x400

FILE *ser0 = NULL;
static FILE *ser1 = NULL;

extern char data_start, data_end, text_end;   /* Defined in *.ld! */
extern char bss_start, bss_end;               /* Defined in *.ld! */
extern char stack_start, stack_end;           /* Defined in *.ld! */
extern char heap_start, heap_end;             /* Defined in *.ld! */

static USO_heap_t heap;

static void
init (void)
{
    USO_thread_t *start_thread;

    DEV_timers_init ();
    DEV_clock_init ();

    SAM_ticks_init ();
	SAM_eth_init ();
    SAM_sys_interrupt_init();
 
    SAM_uart_init_0 ();
    ser0 = MFS_get_stream (MFS_open(MFS_sysfs_serial(), "0"));
    if (ser0 == NULL){
    	DEV_digout_set (&SAM_red_led);
		DEV_at91_dbgu_print_ascii("open ser0 failed\n");
    }

    SAM_uart_init_1 ();
    ser1 = MFS_get_stream (MFS_open(MFS_sysfs_serial(), "1"));
    if (ser0 == NULL){
    	DEV_digout_set (&SAM_red_led);
		DEV_at91_dbgu_print_ascii("open ser1 failed\n");
    }

    USO_log_init (ser0, USO_LL_DEBUG);
    USO_enable ();
    USO_kputs (USO_LL_INFO, "Debug on ser0.\n");

    unsigned long loop_count = DEV_get_ticks();
    DEV_cpudelay(ACE_USEC_2_LOOPS(50000));
    USO_kprintf (USO_LL_INFO, "Loop calib 50ms: %lu.\n", DEV_get_ticks_diff(loop_count));
    
    /* The start thread has its own stack
       and its stacksize can be changed here! */
    start_thread = USO_thread_new (SAM_start_run, START_STACK_SIZE, USO_USER,
                                  USO_ROUND_ROBIN, "Start");
	if (start_thread != NULL)
	{
		USO_thread_flags_set(start_thread, (1 << USO_FLAG_DETACH));
    	USO_start (start_thread);
	}
    USO_kputs (USO_LL_DEBUG, "Idle.\n");
}

extern void SAM_init(void)
{
    memcpy(&data_start, &text_end, &data_end - &data_start);
    memset(&bss_start, 0, &bss_end - &bss_start); 

    SAM_pins_init();

    DEV_at91_configure_dbgu();
    DEV_at91_dbgu_print_ascii("AT91SAM7X chip ID : ");
    DEV_at91_dbgu_print_hex8(AT91C_BASE_DBGU->DBGU_CIDR);
    DEV_at91_dbgu_print_ascii("\n");

    SAM_digio_init();	    
    
    if (USO_heap_init (&heap, &heap_start, &heap_end) == FALSE){
    	DEV_digout_set (&SAM_red_led);
        DEV_at91_dbgu_print_ascii("heap_init failed\n");
    }
    ACE_stdlib_init(&heap);
    
    if (MFS_sysfs_init() == FALSE){
    	DEV_digout_set (&SAM_red_led);
        DEV_at91_dbgu_print_ascii("sysfs_init failed\n");
    }
    USO_heap_install(&heap, "0");
   
    USO_transform (init, (USO_stack_t*)&stack_start, 2048 / sizeof(USO_stack_t) );
}
