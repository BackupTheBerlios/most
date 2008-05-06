/*
 *  Copyright (C) 2008  Michael Feiner
 *
 */

#include <uso/thread.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <cli/interpreter.h>

#include "arch/OLIMEX_SAM7_EX256.h"
#include "init/init.h"
#include "init/start.h"
#include "init/main.h"

#define RUNLED_STACK_SIZE     0x400
//#define CLI_STACK_SIZE        0x400

static USO_thread_t run_led_thread;
static USO_stack_t run_led_stack[RUNLED_STACK_SIZE];

//static USO_thread_t cli0_thread;
//static USO_stack_t cli0_stack[CLI_STACK_SIZE];
//static CLI_interpreter_t cli0;

#if 1
static void
run_led_run (void *nix)
{
    for (;;)
    {
		//MDC_watchdog_trigger ();
        //DEV_digout_toggle (&MDC_run_led);
        //USO_sleep (ACE_MSEC_2_TICKS(200));
        AT91C_BASE_PIOB->PIO_SODR = AT91B_LCD_BL;
        USO_sleep(1000);
        AT91C_BASE_PIOB->PIO_CODR = AT91B_LCD_BL;
        USO_sleep(1000);
    }
}
#endif

extern void
SAM_start_run (void *nix)
{
    //USO_kputs (USO_LL_INFO, "Init run.\n");

#if 1
    USO_thread_init (&run_led_thread,
                     run_led_run,
                     run_led_stack, ARRAYSIZE (run_led_stack),
                     USO_USER, USO_FIFO, "RunLed");
    USO_start (&run_led_thread);
    //USO_kputs (USO_LL_INFO, "RunLed started.\n");
	USO_yield();
#endif


#if 0
    CLI_setup (MDC_get_hostname ());
    CLI_interpreter_init (&cli0);
    USO_thread_init (&cli0_thread,
                     CLI_interpreter_run,
                     cli0_stack, ARRAYSIZE (cli0_stack),
                      USO_USER, USO_ROUND_ROBIN, "Cli0");
    USO_thread_arg_init (&cli0_thread, &cli0, FALSE);
    USO_thread_ios_init (&cli0_thread, ser0, ser0);
    USO_start (&cli0_thread);
    USO_kputs (USO_LL_INFO, "Cli0 on ser0.\n");
#endif

    //USO_kputs (USO_LL_INFO, "Main.\n");

    SAM_main();
}
