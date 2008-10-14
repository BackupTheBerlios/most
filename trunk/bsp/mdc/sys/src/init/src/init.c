/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/arch/cpu.h>
#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/scheduler.h>
#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <uso/heap.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/arch/cpu.h>
#include <mfs/sysfs.h>

#include "arch/cpu.h"
#include "arch/93C46.h"
#include "arch/sci.h"
#include "arch/digio.h"
#include "arch/ticks.h"
#include "arch/eth.h"
#include "init/init.h"
#include "init/start.h"
#include "init/config.h"
#include "mfs/descriptor.h"
#include "mfs/stream.h"
#include "mfs/sysfs.h"
#include "arch/digio.h"


#define START_STACK_SIZE       0x400

extern char data_start, data_end, code_end;   /* Defined in *.ld! */
extern char bss_start, bss_end;               /* Defined in *.ld! */
extern char heap_start, heap_end;             /* Defined in *.ld! */

FILE *ser0 = NULL;
static FILE *ser1 = NULL;

static USO_heap_t heap;

static void
init (void)
{
    USO_thread_t *start_thread;

    DEV_timers_init ();
    DEV_clock_init ();
    MDC_ticks_init ();

    MDC_sci_init_0 ();
    ser0 = MFS_get_stream (MFS_open(MFS_sysfs_serial(), "0"));
    if (ser0 == NULL){
         DEV_digout_set (&MDC_red_led);
    }

    MDC_sci_init_1 ();
    ser1 = MFS_get_stream (MFS_open(MFS_sysfs_serial(), "1"));
    if (ser1 == NULL){
         DEV_digout_set (&MDC_red_led);
    }

    USO_log_init (ser0, USO_LL_INFO);
    USO_kputs (USO_LL_INFO, "Debug on ser0.\n");

    EE_93C46_init ();
    MDC_ee_config_read ();
    MDC_eth_init ();

    USO_enable ();

	unsigned long loop_count = DEV_get_ticks();
	DEV_cpudelay(ACE_USEC_2_LOOPS(50000));
    USO_kprintf (USO_LL_INFO, "Loop calib 50ms: %lu.\n", DEV_get_ticks_diff(loop_count));
	
    /* Why an start thread? the start thread has its own stack
       and its stacksize can be changed here! */
       
    start_thread = USO_thread_new (MDC_start_run, START_STACK_SIZE, USO_USER,
    							  USO_ROUND_ROBIN, "start", TRUE);
    USO_start (start_thread);

    USO_kputs (USO_LL_INFO, "Turn to idle.\n");
}

extern void
MDC_init (void)
{
	MDC_cpu_init ();

	memcpy(&data_start, &code_end, &data_end - &data_start);
    memset(&bss_start, 0, &bss_end - &bss_start); 

    MDC_digio_init ();
    
    if (USO_heap_init (&heap, &heap_start, &heap_end) == FALSE){
         DEV_digout_set (&MDC_red_led);
    }
    ACE_stdlib_init(&heap);
    
 	if (MFS_sysfs_init() == FALSE){
         DEV_digout_set (&MDC_red_led);
    }
 	USO_heap_install(&heap, "0");
 		
    USO_transform (init);
}
