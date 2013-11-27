/*
 * read_line.c
 *
 *  Created on: 25.11.2013
 *      Author: maik
 */

#include <uso/sleep.h>
#include <uso/scheduler.h>
#include <cli/read_line.h>

extern char*
CLI_fread_line (ACE_FILE * in, char *string, ACE_size_t len)
{
    char *ret = string;
    if (len == 0 || string == NULL) return NULL;
    for (int n = 0; n < len - 1; ++n)
    {
        char c;
        if (ACE_fread (in, &c, sizeof (c)) >= sizeof (c)){
            if (c == '\b') {
                if (n >= 1)
                {
                    ACE_putc (c);
                    ACE_putc ((unsigned char)' ');
                    ACE_putc (c);
                    --n;
                    --string;
                }
                --n;
                continue;
            }
            *string++ = c;
            if (c == '\0') return ret;
            if (c == '\n') break;
            ACE_putc ((unsigned char)c);
        } else {
            USO_sleep (CLI_RX_POLLING_TIME);
        }
    }
    *string = '\0';
    return ret;
}

extern char*
CLI_read_line (char *string, ACE_size_t len)
{
    return CLI_fread_line (USO_current()->in, string, len);
}

