/*
 * esc.c
 *
 *  Created on: 25.11.2013
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <ace/esc.h>
#include <ace/ascii.h>
#include <ace/ascii.h>
#include <ace/err.h>
#include <ace/ctype.h>

#define ACE_ESC_RX_BUF_SIZE  16

static int
esc_read_number(int *n, ACE_bool_t *valid)
{
    long l;
    int c, i;
    char *end;
    char string[ACE_ESC_RX_BUF_SIZE];
    for( i = 0; i < (ACE_ESC_RX_BUF_SIZE - 1); ++i){
        if ( ACE_isdigit(c = ACE_getc ()) ) {
            string[i] = c;
        } else {
            break;
        }
    }
    string[i] = 0;
    l = ACE_strtol(string, &end, 10);
    if (end >= string) {
        *n = (int)l;
        *valid = TRUE;
    }
    return c;
}
extern int
ACE_esc_request_cursor(int *y, int *x, ACE_bool_t *valid)
{
    int c;
    *valid = FALSE;
    ACE_puts (ACE_ESC_REQUEST_CURSOR);
    if ((c = ACE_getc ()) != ACE_ASCII_ESC) return c;
    if ((c = ACE_getc ()) != '[') return c;
    c = esc_read_number(y, valid);
    if (*valid == FALSE) return c;
    *valid = FALSE;
    if (c != ';') return c;
    c = esc_read_number(x, valid);
    if (*valid == FALSE) return c;
    *valid = FALSE;
    if (c != 'R') return c;
    *valid = TRUE;
    return ACE_EOF;
}

extern void
ACE_esc_set_cursor(int y, int x)
{
    ACE_printf(ACE_CSI"%i;%iH", y, x);
}

extern void
ACE_esc_cursor_up(int n)
{
    ACE_printf(ACE_CSI"%iA", n);
}

extern void
ACE_esc_cursor_down(int n)
{
    ACE_printf(ACE_CSI"%iB", n);
}

extern void
ACE_esc_cursor_right(int n)
{
    ACE_printf(ACE_CSI"%iC", n);
}

extern void
ACE_esc_cursor_left(int n)
{
    ACE_printf(ACE_CSI"%iD", n);
}

