/*
 * init.c
 *
 *  Created on: 26.05.2013
 *      Author: maik
 */

#include <ace/cpu.h>
#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/buf_pool.h>
#include <uso/scheduler.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <dev/timer.h>
#include <dev/clock.h>
#include <dev/cpu.h>
#include <cli/tty.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>
#include <dev/arch/ibmpc/console.h>

#include <arch/debug.h>
#include <arch/cpu.h>
#include <arch/pic.h>
#include <arch/exceptions.h>
#include <arch/mm.h>
#include <arch/ticks.h>
#include <arch/kbd.h>
#include <init/init.h>
#include <init/start.h>

#define PC_LOOPS_PER_MSEC 45000L;

extern void bss_start;
extern void bss_end;

extern USO_stack_t init_stack_start;
extern USO_stack_t init_stack_end;

#define INIT_STACK_SIZE    (0x2000/sizeof(USO_stack_t))
#define HEAP_SIZE     0x100000

static char heap_buffer[HEAP_SIZE];

static MFS_descriptor_t *con0 = NULL;
static MFS_descriptor_t *kbd0 = NULL;
static MFS_descriptor_t *tty0 = NULL;

static CLI_tty_t tty_0;
static USO_heap_t heap;


static void
abort_handler (char *msg, char *file, int line)
{
    PC_dbg_puts("!!! Abort !!!\n");
    PC_dbg_puts(msg);
    PC_dbg_puts(file);
    for (;;);
}

static void
panic_handler (char *msg, char *file, int line)
{
    PC_dbg_puts("!!!Don't PANIC: !!!\n");
    PC_dbg_puts(msg);
    for (;;);
}



#define IOS_BUF_COUNT 3
#define IOS_BUF_SIZE 0x100
static USO_buf_pool_t ios_buf_pool;
static char ios_bufs[IOS_BUF_COUNT][IOS_BUF_SIZE];


static void
init (void)
{
    /* System initialization without kernel logging */

    PC_dbg_puts("init2\n");

    USO_buf_pool_init (&ios_buf_pool, ios_bufs, IOS_BUF_COUNT, IOS_BUF_SIZE);
    ACE_stdio_init (&ios_buf_pool);
    USO_sleep_init ();
    DEV_timers_init ();
    IBMPC_console_install (NULL, "con0");
    PC_kbd_install();
    PC_ticks_init();
    //PC_rtc_init ();

    PC_dbg_puts("init con\n");

    con0 = MFS_resolve("/sys/dev/serial/con0");
    if (con0 == NULL)
    {
        ACE_PANIC ("Open con0 fail");
    }

    PC_dbg_puts("init kbd\n");

    kbd0 = MFS_resolve("/sys/dev/serial/kbd0");
    if (kbd0 == NULL)
    {
        ACE_PANIC ("Open kbd0 fail");
    }

    PC_dbg_puts("init tty\n");

    CLI_tty_init (&tty_0,
                  kbd0, con0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty0");

    tty0 = MFS_resolve("/sys/cli/tty0");
    if (tty0 == NULL)
    {
        ACE_PANIC ("Open tty0 fail");
    }

    PC_dbg_puts("init log\n");

    USO_log_init (tty0, USO_LL_DEBUG);
    USO_enable ();
    USO_log_puts (USO_LL_INFO, "\nInit: Kernel log on con0.\n");

    /* Kernel logging on */


    unsigned long ticks_count = DEV_get_ticks ();
    DEV_cpudelay (DEV_USEC_2_LOOPS (100000L));
    USO_log_printf (USO_LL_INFO, "Loop calib 100ms: %lu.\n", USO_TICKS_2_MSEC(DEV_get_ticks_diff (ticks_count)) );

    PC_keyboard_start ();

    PC_dbg_puts("start\n");

    PC_start (tty0, tty0);

    USO_log_puts (USO_LL_INFO, "Idle.\n");

    for (;;)
    {
        DEV_cpudelay(DEV_USEC_2_LOOPS(1000000));
        USO_log_puts (USO_LL_PROTOCOL, "I");
    }

}

void PC_init(struct multiboot_info *mb_info)
{
    /* C setup: not initialized variables */
    //memset (&bss_start, 0, &bss_end - &bss_start); do not do this , it will overwrite the init_stack

    PC_dbg_clr();
    PC_dbg_puts("init\n");

    PC_pmm_init(mb_info);
    PC_vmm_init();

    PC_cpu_init();
    PC_init_vectors();

    DEV_loops_per_msec = PC_LOOPS_PER_MSEC;

    PC_dbg_puts("init stdlib\n");

    /* Abort handler initialization */
    ACE_stdlib_init (&heap, abort_handler, panic_handler);

    PC_dbg_puts("init heap\n");

    /* Heap initialization */
    if (USO_heap_init (&heap, &heap_buffer[0], &heap_buffer[HEAP_SIZE]) == FALSE)
    {
        ACE_PANIC ("Heap init failed!");
    }

    PC_dbg_puts("init sysfs\n");

    /* SYS FS needs heap */
    if (MFS_sysfs_init () == FALSE)
    {
        ACE_PANIC ("Sysfs init failed!");
    }

    PC_dbg_puts("install heap\n");

    USO_heap_install (&heap, "heap0");


    //USO_stack_init (&init_stack_start, INIT_STACK_SIZE);

    PC_dbg_puts("transform\n");

    /* Go multithreading */
    USO_transform (init, &init_stack_start, INIT_STACK_SIZE);

}
