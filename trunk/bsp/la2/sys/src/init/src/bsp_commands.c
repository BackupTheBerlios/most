/*
 * bsp_commands.c
 *
 *  Created on: 11.09.2010
 *      Author: maik
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/cpu.h>
#include <uso/stack.h>
#include <uso/heap.h>
#include <uso/sleep.h>
#include <cli/exec.h>
#include <init/bsp_commands.h>
#include <arch/cpu.h>
#include <arch/reset.h>
#include <arch/ticks.h>

extern char stack_start, stack_end;     /* Defined in *.ld! */

struct stack_data
{
    int size;
    char *name;
};

/* must match reset.S and *.ld*/
static struct stack_data stack[] = {
    {LA2_IDLE_STACK_SIZE, "Idle"},
    {LA2_UND_STACK_SIZE, "Undef"},
    {LA2_ABT_STACK_SIZE, "Abort"},
    {LA2_IRQ_STACK_SIZE, "IRQ"},
    {0, NULL}
};

static CLI_exec_t exception_stack_check;

static ACE_err_t
exception_stack_check_exec (char *nix)
{
    USO_stack_t *sp = (USO_stack_t *) & stack_start;
    ACE_printf ("Name\tS.size\tS.free\tS.top\tS.bot\n");
    for (int i = 0; sp <= (USO_stack_t *) & stack_end; ++i)
    {
        if (stack[i].size == 0)
            break;
        ACE_printf ("%s\t%i\t%i\t%p %p\n",
                    stack[i].name,
                    stack[i].size,
                    USO_stack_get_free (sp,
                                        (stack[i].size -
                                         1) / sizeof (USO_stack_t)) * sizeof (USO_stack_t), sp,
                    sp + ((stack[i].size - 1) / sizeof (USO_stack_t)));
        sp += (stack[i].size / sizeof (USO_stack_t));
    }
    return ACE_OK;
}



static CLI_exec_t ticks_debug;

static ACE_err_t
ticks_debug_exec (char *nix)
{
    LA2_ticks_debug ();
    return ACE_OK;
}

static CLI_exec_t start_boot;

extern void
LA2_start_boot (void)
{
    USO_sleep (USO_MSEC_2_TICKS (100));
    USO_disable ();
    if (!LA2_WDT_DISABLE)
    {
        LA2_wdt_trigger ();
    }
    LA2_jump_boot ();
}

static ACE_err_t
start_boot_exec (char *nix)
{
    LA2_start_boot ();
    return CLI_ERR_NOT_REACHED;
}

static CLI_exec_t start_app;

extern void
LA2_start_app (void)
{
    USO_sleep (USO_MSEC_2_TICKS (100));
    USO_disable ();
    if (!LA2_WDT_DISABLE)
    {
        LA2_wdt_trigger ();
    }
    LA2_jump_app ();
}

static ACE_err_t
start_app_exec (char *nix)
{
    LA2_start_app ();
    return CLI_ERR_NOT_REACHED;
}

extern void
LA2_bsp_commands_install (MFS_descriptor_t * bsp)
{
    CLI_exec_init (bsp, &exception_stack_check, "excep_S", "Exception stack check",
                   exception_stack_check_exec);
    CLI_exec_init (bsp, &ticks_debug, "tick_D", "Debug ticks interrupt", ticks_debug_exec);
    CLI_exec_init (bsp, &start_boot, "start_B", "Start boot", start_boot_exec);
    CLI_exec_init (bsp, &start_app, "start_A", "Start app", start_app_exec);
}
