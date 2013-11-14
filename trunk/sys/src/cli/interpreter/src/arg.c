/*
 * arg.c
 *
 *  Created on: 13.11.2013
 *      Author: maik
 */

#include <ace/string.h>
#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <ace/assert.h>
#include <cli/arg.h>

extern int
CLI_arg_parse(char *arg, char *buf[CLI_MAX_ARG])
{
    USO_lock (&ACE_lock);

    int i = 0;
    
    for (buf[i] = ACE_strtok(arg, " "); buf[i] != NULL; buf[i] = ACE_strtok(NULL, " ")){
        ++i;
        if (i >= CLI_MAX_ARG) break;
    }

    USO_unlock (&ACE_lock);

    return i;
}
