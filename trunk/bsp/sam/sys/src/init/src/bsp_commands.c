/*
 * bsp_commands.c
 *
 *  Created on: 11.09.2010
 *      Author: maik
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/stack.h>
#include <cli/commands.h>

#include "init/bsp_commands.h"
#include "arch/cpu.h"

extern char stack_start, stack_end;           /* Defined in *.ld! */

struct stack_data{
	int size;
	char *name;
};

/* must match reset.S and *.ld*/
static struct stack_data stack[] = {
		{SAM_IDLE_STACK_SIZE,"Idle"},
		{SAM_UND_STACK_SIZE,"Undef"},
		{SAM_ABT_STACK_SIZE,"Abort"},
		{SAM_IRQ_STACK_SIZE,"IRQ"},
		{0, NULL}
};

static CLI_exec_t exception_stack_check;

static void exception_stack_check_exec(char *nix)
{
	USO_stack_t *sp = (USO_stack_t*)&stack_start;
	ACE_printf ("S.name\tS.size\tS.free\tS.top\tS.bot\n");
	for (int i = 0; sp <= (USO_stack_t*)&stack_end; ++i){
		if (stack[i].size == 0) break;
		ACE_printf ("%s\t%i\t%i\t%p %p\n",
				stack[i].name,
				stack[i].size,
				USO_stack_get_free(sp, (stack[i].size - 1)/sizeof(USO_stack_t)) * sizeof(USO_stack_t),
				sp,
				sp + ( (stack[i].size - 1)/sizeof(USO_stack_t)));
		sp += (stack[i].size/sizeof(USO_stack_t));
	}
}

extern void
bsp_commands_install(MFS_descriptor_t *bsp)
{
    CLI_exec_init (bsp, &exception_stack_check, "excep_S", "Exception stack check", exception_stack_check_exec);
}
