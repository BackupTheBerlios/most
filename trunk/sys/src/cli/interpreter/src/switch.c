/*
 * switch.c
 *
 *  Created on: 22.11.2013
 *      Author: maik
 */

#include <cli/switch.h>
#include <cli/tty.h>

#define MAX_TTY  4

static MFS_descriptor_t *stream[MAX_TTY];

extern void
CLI_switch_init(void)
{
    for (int i = 0; i < MAX_TTY; ++i){
        stream[i] = NULL;
    }
}

extern void
CLI_switch_set(int i, MFS_descriptor_t *tty)
{
    if ( (i >= 0) && (i < MAX_TTY)){
        stream[i] = tty;
    }
}

extern ACE_bool_t
CLI_switch_select(int i)
{
    if ( (i >= 0) && (i < MAX_TTY) ){
        if (stream[i] != NULL){
            CLI_tty_select(stream[i]);
            return TRUE;
        }
    }
    return FALSE;
}
