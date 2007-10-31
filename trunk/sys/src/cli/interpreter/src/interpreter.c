/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/ctype.h>
#include <uso/sleep.h>
#include <mfs/descriptor.h>
#include <mfs/sysfs.h>

#include "cli/interpreter.h"
#include "cli/commands.h"
#include "cli/cmd_desc.h"
#include "mfs/directory.h"
#include "mfs/sysfs.h"


#define CLI_RX_POLLING_TIME ACE_MSEC_2_TICKS(250)
#define	CLI_HISTORY_SIZE	10

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
promt_desc (MFS_descriptor_t *desc)
{
	if (NULL != desc->parent)
	{
		promt_desc (desc->parent);
		putc ('/');
	}
	puts (desc->name);
}

static void
promt (CLI_interpreter_t *cli)
{
    printf ("%s:", hostname);
    promt_desc (cli->desc);
    puts ("> ");
}

static CLI_command_t *
find(char* name)
{
	MFS_descriptor_t *desc = MFS_lookup(MFS_sysfs_cli(), name);
	if (NULL != desc)
		return (CLI_command_t*)desc->entry;
	return NULL;
}
static void
token_clear (CLI_interpreter_t *cli)
{
	int i = cli->token.argc;
    do {
		char *buf = cli->token.buffer[cli->token.argc];
    	char *token = buf + strlen (buf);
    	
    	while (token-- > buf)
    		puts ("\b \b");
    		
    	if (cli->token.argc-- > 0)
    		puts ("\b \b");
	} while (i--);
}

static void
token_print (CLI_interpreter_t *cli)
{
	for (int i = 0; i < cli->token.argc; i++) {
		if (i > 0) putc (' ');
		printf ("%s", cli->token.buffer[i]);
	}
}

static void
parse_cntrl (CLI_interpreter_t *cli)
{
    for (;;) {
        int c = getc ();
        if (c == EOF) {
            USO_sleep (CLI_RX_POLLING_TIME);
        } else {
			if (c == 'A') {
				token_clear (cli);
				CLI_history_next (&cli->history, &cli->token);
				token_print (cli);
				cli->token.argc--;
                break;
			} else if (c == 'B') {
				token_clear (cli);
				CLI_history_prev (&cli->history, &cli->token);
				token_print (cli);
				cli->token.argc--;
				break;
			} else {
				break;
			}
        }
    }
}

static void
parse (CLI_interpreter_t *cli)
{
	char *buf = cli->token.buffer[0];
    char *token = buf;

    for (;;) {
        int c = getc ();
        if (c == EOF) {
            USO_sleep (CLI_RX_POLLING_TIME);
        } else {
			if (c == '\n') {
                *token = '\0';
                if (token > buf) cli->token.argc++;
                break;
			} else if (c == 0x5B) {
				*token = '\0';
				parse_cntrl (cli);
				buf = cli->token.buffer[cli->token.argc];
				token = buf + strlen(buf);
			} else if (c == '\b') {
				if (token > buf) {
					puts ("\b \b");
					token--;
				} else if (cli->token.argc > 0) {
					puts ("\b \b");
					cli->token.argc--;
					buf = cli->token.buffer[cli->token.argc];
					token = buf + strlen(buf);
				}
			} else if (isgraph (c)) {
                if (token < buf + CLI_TOKEN_SIZE - 1) {
                    *token++ = c;
                    putc ((unsigned char)c);
                }
            } else if (c == ' ') {
            	if (token == buf)
            		continue;
                *token = '\0';
		        if (cli->token.argc < CLI_TOKEN_COUNTER - 1) {
		        	cli->token.argc++;
		        	buf = token = cli->token.buffer[cli->token.argc];
		        	putc ((unsigned char)c);
		        }
            }
        }
    }
}

static int
inc_argv(CLI_interpreter_t * cli, int idx)
{
   	if (cli->token.argc > 0) { --cli->token.argc; }
   	++idx;
    for (int i = 0; i < cli->token.argc; ++i){ 
  		cli->argv[i] = cli->token.buffer[idx + i];
   	}
   	return idx;
}

extern void
CLI_interpreter_run (void *param)
{
	CLI_interpreter_t *cli = (CLI_interpreter_t *)param; 
    CLI_command_t *command;
    int idx;
    bool_t abbr;

    CLI_history_init (&cli->history, CLI_HISTORY_SIZE);

    puts ("\nCLI:\n");
    for (;;)
    {
        for (int i = 0; i < CLI_TOKEN_COUNTER; ++i) { 
        	cli->token.buffer[i][0] = '\0';
	    	cli->argv[i] = cli->token.buffer[i];
        }
        cli->token.argc = 0;
		idx = 0;
		abbr = FALSE;
		
        promt (cli);
		parse (cli);
        putc ('\n');

		if (cli->argv[0][0] == '/' || cli->argv[0][0] == '&' || cli->argv[0][0] == '|') {
   	    	cli->argv[0] = &cli->token.buffer[0][1];
       	    if (CLI_cmd_open(cli) == FALSE){
            	puts ("error\n");
            	continue;
           	}
           	abbr = TRUE;
           	CLI_history_add (&cli->history, &cli->token);
           	if (cli->token.buffer[0][0] == '/') {
           		idx = inc_argv(cli, idx);
           	} else if (cli->token.buffer[0][0] == '|') {
           		strcpy (cli->argv[0], "exec");
           	} else {
           		strcpy (cli->argv[0], "run");
           	}
		}
		
        command = find (cli->argv[0]);
        if (command != NULL) {
        	if (FALSE == abbr && command->f != CLI_cmd_history)
        		CLI_history_add (&cli->history, &cli->token);
	       	idx = inc_argv(cli, idx);       	
            if (command->f (cli) == FALSE){
	            puts ("error\n");
            }
        } else if (strlen(cli->argv[0])) {
            printf ("? %s\n", cli->argv[0]);
        }

		if (abbr) {
			cli->token.argc = 0;
       	    CLI_cmd_close(cli);
		}
        
    }
}
