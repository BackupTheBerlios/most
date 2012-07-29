/*
 * bsp_commands.c
 *
 *  Created on: 11.09.2010
 *      Author: maik
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/arch/cpu.h>
#include <uso/stack.h>
#include <uso/heap.h>
#include <uso/sleep.h>
#include <cli/commands.h>
#include <nap/bootp.h>

#include "init/bsp_commands.h"
#include "init/config.h"
#include "arch/cpu.h"
#include "arch/reset.h"
#include "arch/eth.h"
#include "arch/ticks.h"

extern char stack_start, stack_end;     /* Defined in *.ld! */

struct stack_data
{
    int size;
    char *name;
};

/* must match reset.S and *.ld*/
static struct stack_data stack[] = {
    {SAM_IDLE_STACK_SIZE, "Idle"},
    {SAM_UND_STACK_SIZE, "Undef"},
    {SAM_ABT_STACK_SIZE, "Abort"},
    {SAM_IRQ_STACK_SIZE, "IRQ"},
    {0, NULL}
};

static CLI_exec_t exception_stack_check;

static void
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
}

static CLI_exec_t bootp;

extern void
SAM_bootp (void)
{
    if (NAP_bootp (&SAM_config.eth_addr) == 0)
    {
        NET_netif_set_ipaddr (&SAM_eth0, &NAP_bootp_data.ip_addr);
        NET_netif_set_gateway (&SAM_eth0, &NAP_bootp_data.gateway);
        SAM_config.ip_addr = NAP_bootp_data.ip_addr;
        SAM_config.gateway = NAP_bootp_data.gateway;
        SAM_config.server = NAP_bootp_data.server;
    }
}

static void
bootp_exec (char *nix)
{
    SAM_bootp ();
}

static CLI_exec_t start_boot;

extern void
SAM_start_boot (void)
{
    USO_sleep (USO_MSEC_2_TICKS (100));
    USO_disable ();
    if (!SAM_WDT_DISABLE)
    {
        SAM_wdt_trigger ();
    }
    SAM_jump_boot ();
}

static void
start_boot_exec (char *nix)
{
    SAM_start_boot ();
}

static CLI_exec_t start_app;

extern void
SAM_start_app (void)
{
    USO_sleep (USO_MSEC_2_TICKS (100));
    USO_disable ();
    if (!SAM_WDT_DISABLE)
    {
        SAM_wdt_trigger ();
    }
    SAM_jump_app ();
}

static void
start_app_exec (char *nix)
{
    SAM_start_app ();
}

static CLI_exec_t heap_debug;

static void
heap_debug_exec (char *heap)
{
    MFS_descriptor_t *desc = MFS_open (MFS_sysfs_get_dir (MFS_SYSFS_DIR_HEAPS), heap);
    if (desc != NULL)
        USO_debug_heap_list (desc->entry);
}

static CLI_exec_t ticks_debug;

static void
ticks_debug_exec (char *nix)
{
    SAM_ticks_debug ();
}

extern void
SAM_bsp_commands_install (MFS_descriptor_t * bsp)
{
    CLI_exec_init (bsp, &exception_stack_check, "excep_S", "Exception stack check",
                   exception_stack_check_exec);
    CLI_exec_init (bsp, &bootp, "bootp", "Bootp request", bootp_exec);
    CLI_exec_init (bsp, &start_boot, "startb", "Start boot", start_boot_exec);
    CLI_exec_init (bsp, &start_app, "starta", "Start app", start_app_exec);
    CLI_exec_init (bsp, &heap_debug, "heap_D", "Debug heap list", heap_debug_exec);
    CLI_exec_init (bsp, &ticks_debug, "tick_D", "Debug ticks interrupt", ticks_debug_exec);
}
