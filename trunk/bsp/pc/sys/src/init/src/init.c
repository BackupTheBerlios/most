/*
 * init.c
 *
 *  Created on: 26.05.2013
 *      Author: maik
 */

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
#include <cli/switch.h>
#include <mfs/sysfs.h>
#include <mfs/descriptor.h>
#include <mfs/stream.h>
#include <mfs/sysfs.h>

#include <arch/debug.h>
#include <arch/cpu.h>
#include <arch/pic.h>
#include <arch/exceptions.h>
#include <arch/mm.h>
#include <arch/ticks.h>
#include <arch/kbd.h>
#include <init/init.h>
#include <init/start.h>


//extern void bss_start;
//extern void bss_end;
extern USO_stack_t init_stack_start;
//extern USO_stack_t init_stack_end;


/* take care! must match stack size in reset.S */
#define PC_IDLE_STACK_SIZE    (0x2000/sizeof(USO_stack_t))
#define HEAP_SIZE     0x100000
#define PC_LOOPS_PER_MSEC 45000L;

/* !!!! you must have more or same amount of ios buffers as CLIs otherwise the system deadlocks !!!!!
 * want to get rid of ios bufs (sprintf like sscanf)
 */
#define IOS_BUF_COUNT 8
#define IOS_BUF_SIZE 0x100

static USO_heap_t heap;
static char heap_buffer[HEAP_SIZE];

static USO_buf_pool_t ios_buf_pool;
static char ios_bufs[IOS_BUF_COUNT][IOS_BUF_SIZE];

static MFS_descriptor_t *con0 = NULL;
static MFS_descriptor_t *tty0 = NULL;

static IBMPC_console_t con_0;

static CLI_tty_t tty_0;
static CLI_tty_t tty_1;
static CLI_tty_t tty_2;
static CLI_tty_t tty_3;


static void
abort_handler (char *msg, char *file, int line)
{
    PC_dbg_puts("!!! Abort !!!\n");
    PC_dbg_puts("\n");
    PC_dbg_puts(msg);
    PC_dbg_puts("\n");
    PC_dbg_puts(file);
    for (;;);
}

static void
panic_handler (char *msg, char *file, int line)
{
    PC_dbg_puts("!!!Don't PANIC: !!!\n");
    PC_dbg_puts("\n");
    PC_dbg_puts(msg);
    PC_dbg_puts("\n");
    PC_dbg_puts(file);
    for (;;);
}

static void
idle_run (void)
{
    /* System initialization without kernel logging!
     * The scheduler is initialized and we are running on the idle
     * thread so now we are allowed to start threads!
     * The idle thread is not allowed to block!
     */

    /* System initialization without kernel logging */

    PC_dbg_puts("idle\n");

    /* try to remove ios_bufs (do sprintf like sscanf) to avoid deadlocks */
    USO_buf_pool_init (&ios_buf_pool, ios_bufs, IOS_BUF_COUNT, IOS_BUF_SIZE);
    ACE_stdio_init (&ios_buf_pool);
    USO_sleep_init ();
    DEV_timers_init ();
    PC_ticks_init();
    //PC_rtc_init ();

    PC_dbg_puts("init con\n");

    IBMPC_console_install (&con_0, "con0");
    PC_kbd_install(&con_0);

    con0 = MFS_resolve("/sys/dev/serial/con0");
    if (con0 == NULL)
    {
        ACE_PANIC ("Open con0 fail");
    }

    PC_dbg_puts("init ttys\n");

    CLI_tty_init (&tty_0,
                  con0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty0",
                  TRUE);

    tty0 = MFS_resolve("/sys/cli/tty0");
    if (tty0 == NULL)
    {
        ACE_PANIC ("Open tty0 fail");
    }

    CLI_tty_init (&tty_1,
                  con0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty1",
                  FALSE);

    CLI_tty_init (&tty_2,
                  con0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty2",
                  FALSE);

    CLI_tty_init (&tty_3,
                  con0,
                  CLI_TTY_INTRANSL_CR_2_NL,
                  CLI_TTY_OUTTRANSL_ADD_CR,
                  "tty3",
                  FALSE);

    CLI_switch_init();
    CLI_switch_set(0,tty0);
    CLI_tty_select(tty0);

    PC_dbg_puts("init log\n");
    /* The idle thread is not allowed to block
     * so it is not allowed to do any printf or log output!!!!
     */
    USO_log_init (tty0, USO_LL_INFO);

    PC_dbg_puts("enable int.\n");
    /* Interrupts must be enabled before starting the first thread! */
    USO_enable ();

    PC_dbg_puts("start\n");
    /* ttyS0 is stdio for start thread,
     * all other threads started(derived) from start thread
     * will also use ttyS0 as stdio as long they don't change it.
     */
    PC_start (tty0);
    /* idle */
}

void PC_init(struct multiboot_info *mb_info)
{
    /* C setup: not initialized variables */
    //memset (&bss_start, 0, &bss_end - &bss_start);

    //USO_disable (); /* normaly interrupts should be disabled but to be safe */

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

    PC_dbg_puts("transform\n");

    /* Go multithreading */
    USO_transform (idle_run, &init_stack_start, PC_IDLE_STACK_SIZE);

}
