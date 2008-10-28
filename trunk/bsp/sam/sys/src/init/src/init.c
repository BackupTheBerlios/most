
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
#include <dev/arch/at91/debug.h>
#include <mfs/sysfs.h>
#include "mfs/descriptor.h"
#include "mfs/stream.h"
#include "mfs/sysfs.h"

#include "arch/OLIMEX_SAM7_EX256.h"
#include "arch/lowlevel.h"
#include "init/main.h"
#include "init/init.h"
#include "init/start.h"


#define START_STACK_SIZE       0x400

extern char heap_start, heap_end;             /* Defined in *.ld! */

static USO_heap_t heap;

static void
init (void)
{
    USO_thread_t *start_thread;

    DEV_timers_init ();
    DEV_clock_init ();

	SAM_sys_interrupt_init();

    USO_enable ();

#if 0
    MDC_sci_init_0 ();
    ser0 = MFS_get_stream (MFS_open(MFS_sysfs_serial(), "0"));
    if (ser0 == NULL){
         DEV_digout_set (&MDC_ctrl_led_1);
    }

   	USO_log_init (ser1, USO_LL_INFO);
    USO_kputs (USO_LL_INFO, "Debug on ser0.\n");
#endif


    unsigned long loop_count = DEV_get_ticks();
    DEV_cpudelay(ACE_USEC_2_LOOPS(50000));
	dbgu_print_ascii("Loop count: ");
	dbgu_print_hex8(loop_count);
	dbgu_print_ascii("\n");

    //USO_kprintf (USO_LL_INFO, "Loop calib 50ms: %lu.\n", DEV_get_ticks_diff(loop_count));
    
    /* Why an init thread, the init thread has its own stack
       and its stacksize can be changed here! */
    start_thread = USO_thread_new (SAM_start_run, START_STACK_SIZE, USO_USER,
                                  USO_ROUND_ROBIN, "start", TRUE);
    USO_start (start_thread);

    //USO_kputs (USO_LL_INFO, "Turn to idle.\n");
}

extern void SAM_init(void)
{
	configure_dbgu();
	SAM_io_init();

	dbgu_print_ascii("AT91SAM7X chip ID : ");
	dbgu_print_hex8(AT91C_BASE_DBGU->DBGU_CIDR);
	dbgu_print_ascii("\n");

    if (USO_heap_init (&heap, &heap_start, &heap_end) == FALSE){
        dbgu_print_ascii("heap_init failed\n");
    }
    ACE_stdlib_init(&heap);
    
    if (MFS_sysfs_init() == FALSE){
        dbgu_print_ascii("sysfs_init failed\n");
    }
    USO_heap_install(&heap, "0");
   
    USO_transform (init);
}
