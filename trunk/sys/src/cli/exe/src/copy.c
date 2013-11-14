/*
 * copy.c
 *
 *  Created on: 28.12.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/exec.h>
#include <uso/log.h>


extern ACE_err_t
CLI_exe_copy (char *arg)
{
    unsigned long cnt = 0;
    int c;
    for (;;)
    {
        c = ACE_getc();
        if (c == ACE_EOF) break;
        cnt++;
        ACE_putc(c);
    }
    USO_log_printf(USO_LL_INFO, "%lu bytes copied\n", cnt); /* can not use ACE_printf */
    return ACE_OK;
}

