/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/stdio.h"
#include "ace/ctype.h"
#include "ace/string.h"

#define add2str(pps,c) {**(pps)=c;(*(pps))++;}

static int
skip_atoi (const char **s)
{
    int i = 0;

    while (ACE_isdigit (**s))
        i = i * 10 + *((*s)++) - '0';
    return (i);
}

#define ZEROPAD	1               /* pad with zero */
#define SIGN	2               /* unsigned/signed long */
#define PLUS	4               /* show plus */
#define SPACE	8               /* space if plus */
#define LEFT	16              /* left justified */
#define SPECIAL	32              /* 0x */
#define LARGE	64              /* use 'ABCDEF' instead of 'abcdef' */


static char *
number (char *str, long num, int base, int size, int precision, int type)
{
    char c, sign, tmp[66];
    const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i;

    if (type & LARGE)
        digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (type & LEFT)
        type &= ~ZEROPAD;
    if (base < 2 || base > 36)
        return (0);
    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;
    if (type & SIGN)
    {
        if (num < 0)
        {
            sign = '-';
            num = -num;
            size--;
        }
        else if (type & PLUS)
        {
            sign = '+';
            size--;
        }
        else if (type & SPACE)
        {
            sign = ' ';
            size--;
        }
    }
    if (type & SPECIAL)
    {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    else
        while (num != 0)
        {
            tmp[i++] = digits[(int)((unsigned long)num % (unsigned)base)];
            num = (unsigned long)num / (unsigned)base;
        }
    if (i > precision)
        precision = i;
    size -= precision;
    if (!(type & (ZEROPAD + LEFT)))
        while (size-- > 0)
            add2str (&str, ' ');        /* *str++ = ' '; */
    if (sign)
        add2str (&str, sign);   /* *str++ = sign; */
    if (type & SPECIAL)
    {
        if (base == 8)
        {
            add2str (&str, '0');        /* *str++ = '0'; */
        }
        else
        {
            if (base == 16)
            {
                add2str (&str, '0');    /* *str++ = '0'; */
                add2str (&str, digits[33]);     /* *str++ = digits[33]; */
            }
        }
    }
    if (!(type & LEFT))
        while (size-- > 0)
            add2str (&str, c);  /* *str++ = c; */
    while (i < precision--)
        add2str (&str, '0');    /* *str++ = '0'; */
    while (i-- > 0)
        add2str (&str, tmp[i]); /* *str++ = tmp[i]; */
    while (size-- > 0)
        add2str (&str, ' ');    /* *str++ = ' '; */
    return (str);
}

extern int
ACE_vsprintf (char *buf, const char *fmt, ACE_va_list_t args)
{
    int len;
    unsigned long num;
    int i, base;
    char *str;
    const char *s;

    int flags;                  /* flags to number() */

    int field_width;            /* width of output field */
    int precision;              /* min. # of digits for integers; max
                                 * number of chars for from string */
    int qualifier;              /* 'h', 'l', or 'L' for integer fields */

    for (str = buf; *fmt; ++fmt)
    {
        if (*fmt != '%')
        {
            add2str (&str, *fmt);       /* *str++ = *fmt; */
            continue;
        }

        /*
         * process flags 
         */
        flags = 0;
      repeat:
        ++fmt;                  /* this also skips first '%' */
        switch (*fmt)
        {
        case '-':
            flags |= LEFT;
            goto repeat;
        case '+':
            flags |= PLUS;
            goto repeat;
        case ' ':
            flags |= SPACE;
            goto repeat;
        case '#':
            flags |= SPECIAL;
            goto repeat;
        case '0':
            flags |= ZEROPAD;
            goto repeat;
        }

        /*
         * get field width 
         */
        field_width = -1;
        if (ACE_isdigit (*fmt))
            field_width = skip_atoi (&fmt);
        else if (*fmt == '*')
        {
            ++fmt;
            /*
             * it's the next argument 
             */
            field_width = ACE_va_arg (args, int);
            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /*
         * get the precision 
         */
        precision = -1;
        if (*fmt == '.')
        {
            ++fmt;
            if (ACE_isdigit (*fmt))
                precision = skip_atoi (&fmt);
            else if (*fmt == '*')
            {
                ++fmt;
                /*
                 * it's the next argument 
                 */
                precision = ACE_va_arg (args, int);
            }
            if (precision < 0)
                precision = 0;
        }

        /*
         * get the conversion qualifier 
         */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
        {
            qualifier = *fmt;
            ++fmt;
        }

        /*
         * default base 
         */
        base = 10;

        switch (*fmt)
        {
        case 'c':
            if (!(flags & LEFT))
                while (--field_width > 0)
                    add2str (&str, ' ');        /* *str++ = ' '; */
            add2str (&str, (unsigned char)ACE_va_arg (args, int));      /* *str++
                                                                         * =
                                                                         * (unsigned 
                                                                         * char)
                                                                         * va_arg(args, 
                                                                         * int); */
            while (--field_width > 0)
                add2str (&str, ' ');    /* *str++ = ' '; */
            continue;

        case 's':
            s = ACE_va_arg (args, char *);
            if (!s)
                s = "<NULL>";

            len = ACE_strnlen (s, precision);

            if (!(flags & LEFT))
                while (len < field_width--)
                    add2str (&str, ' ');        /* *str++ = ' '; */
            for (i = 0; i < len; ++i)
                add2str (&str, *s++);   /* *str++ = *s++; */
            while (len < field_width--)
                add2str (&str, ' ');    /* *str++ = ' '; */
            continue;

        case 'p':
            if (field_width == -1)
            {
                field_width = 2 * sizeof (void *);
                flags |= ZEROPAD;
            }
            str = number (str, (unsigned long)ACE_va_arg (args, void *),
                          16, field_width, precision, flags);
            continue;


        case 'n':
            if (qualifier == 'l')
            {
                long *ip = ACE_va_arg (args, long *);
                *ip = (str - buf);
            }
            else
            {
                int *ip = ACE_va_arg (args, int *);
                *ip = (str - buf);
            }
            continue;

            /*
             * integer number formats - set up the flags and "break" 
             */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
            /* no break */
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
            /* no break */
        case 'u':
            break;

        default:
            if (*fmt != '%')
                add2str (&str, '%');    /* *str++ = '%'; */
            if (*fmt)
            {
                add2str (&str, *fmt);   /* *str++ = *fmt; */
            }
            else
                --fmt;
            continue;
        }
        if (qualifier == 'l')
        {
            num = ACE_va_arg (args, unsigned long);
        }
        else if (qualifier == 'h')
        {
            if (flags & SIGN)
            {
                num = ACE_va_arg (args, int);
            }
            else
            {
                num = ACE_va_arg (args, unsigned int);
            }
        }
        else if (flags & SIGN)
        {
            num = ACE_va_arg (args, int);
        }
        else
        {
            num = ACE_va_arg (args, unsigned int);
        }
        str = number (str, num, base, field_width, precision, flags);
    }
    *str = '\0';
    return (str - buf);
}
