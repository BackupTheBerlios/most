/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/ctype.h>
#include <uso/list.h>
#include <uso/sleep.h>
#include <mfs/descriptor.h>
#include <mfs/sysfs.h>

#include "cli/interpreter.h"
#include "cli/commands.h"
#include "cli/cmd_desc.h"


#define CLI_RX_POLLING_TIME ACE_MSEC_2_TICKS(250)

static const char *hostname;

extern void
CLI_setup (const char *name)
{
    hostname = name;
    CLI_commands_init ();
} 

extern void
CLI_interpreter_init (CLI_interpreter_t *cli)
{
    cli->desc = MFS_sysfs_root();
}

static void
promt (CLI_interpreter_t *cli)
{
    printf ("%s:%s> ", hostname, cli->desc->name);
}

static CLI_command_t *
find(char* name)
{
	CLI_command_t *command = NULL;
	while ( (command = (CLI_command_t*)USO_next_element(&CLI_commands, (USO_node_t*)command))
			!= NULL){
	    if (strcmp ( command->name, name) == 0 ){
   			break;	   	
    	}		   		
	}    
	return command;
}


static int
parse (CLI_interpreter_t *cli, char* buf)
{
    char *token = buf;
    ++cli->argc;
    for (;;) {
        int c = getc ();
        if (c == EOF) {
            USO_sleep (CLI_RX_POLLING_TIME);
        } else {
            if (isprint (c)) {
                putc ((unsigned char)c);
			}
			if (c == '\n') {
                *token = '\0';
                return (token - buf);
			} else if (c == '\b') {
				if (token > buf) {
					putc ((unsigned char)c);
					putc ((unsigned char)' ');
					putc ((unsigned char)c);	
					token--;
				} else if (cli->argc > 1) {
					cli->argc--;
					return (-1);
				}
			} else if (isgraph (c)) {
                if (token < buf + CLI_TOKEN_SIZE - 1) {
                    *token++ = c;
                }
            } else if (c == ' ') {
                *token = '\0';
		        if (cli->argc < CLI_TOKEN_COUNTER) { 
		        	if (parse (cli, cli->token_buffer[cli->argc]) < 0)
		        		putc ((unsigned char)'\b');
		        	else
		        		return (token - buf);		
		        } else
                	return (token - buf);
            }
        }
    }
}

static int
inc_argv(CLI_interpreter_t * cli, int idx)
{
   	if (cli->argc > 0) { --cli->argc; }
   	++idx;
    for (int i = 0; i < cli->argc; ++i){ 
  		cli->argv[i] = cli->token_buffer[idx + i];
   	}
   	return idx;
}

extern void
CLI_interpreter_run (void *param)
{
	CLI_interpreter_t *cli = (CLI_interpreter_t *)param; 
    CLI_command_t *command;
    int idx;
    puts ("\nCLI:\n");
    for (;;)
    {
        for (int i = 0; i < CLI_TOKEN_COUNTER; ++i){ 
        	cli->token_buffer[i][0] = '\0';
	    	cli->argv[i] = cli->token_buffer[i];
        }
        cli->argc = 0;
		idx = 0;

        promt (cli);
        (void) parse (cli, cli->token_buffer[cli->argc]);
        putc ('\n');

		if (cli->argv[0][0] == '/'){
   	    	cli->argv[0] = &cli->token_buffer[0][1];
       	    if (CLI_cmd_open(cli) == FALSE){
            	puts ("error\n");
            	continue;
           	}
           	idx = inc_argv(cli, idx);
		}

        command = find (cli->argv[0]);
        if (command != NULL) {
	       	idx = inc_argv(cli, idx);
            if (command->f (cli) == FALSE){
	            puts ("error\n");
            }
        } else {
            printf ("? %s\n", cli->argv[0]);
        }

		if (idx >= 2){
			cli->argc = 0;
       	    CLI_cmd_close(cli);
		}
        
    }
}
