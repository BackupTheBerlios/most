#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <ace/ctype.h>
#include <ace/string.h>
#include <ace/limits.h>


#define A_GETC(fn)  (++consumed,(fn)->getc((fn)->data) )
#define A_PUTC(fn, c)    (--consumed,(fn)->putc((fn)->data, (c)) )

int __ACE_vscanf__(struct __ACE_arg_scanf__ *fn, const char *format, ACE_va_list_t  args)
{
    unsigned int ch; /* format act. char */
    int n = 0;

    /* args tmps */
    long *pl;
    short *ph;
    int *pi;
    char *s;

    unsigned int consumed = 0;

    /* get one char */
    int tpch = A_GETC(fn);

    //while ((tpch!=-1)&&(*format))
    while (*format)
    {
        ch = *format++;
        switch (ch)
        {
            /* end of format string ?!? */
            case 0:
                return 0;

                /* skip spaces ... */
            case ' ':
            case '\f':
            case '\t':
            case '\v':
            case '\n':
            case '\r':
                while ((*format) && (ACE_isspace(*format)))
                    ++format;
                while (ACE_isspace(tpch))
                    tpch = A_GETC(fn);
                break;

                /* format string ... */
            case '%':
            {
                unsigned int _div = 0;
                int width = -1;
                char flag_width = 0;
                char flag_discard = 0;
                char flag_half = 0;
                char flag_long = 0;

in_scan:
                ch = *format++;
                if (ch != 'n' && tpch == -1)
                    goto err_out;
                switch (ch)
                {
                    /* end of format string ?!? */
                    case 0:
                        return 0;

                        /* check for % */
                    case '%':
                        if ((unsigned char) tpch != ch)
                            goto err_out;
                        tpch = A_GETC(fn);
                        break;

                        /* FLAGS */
                    case '*':
                        flag_discard = 1;
                        goto in_scan;
                    case 'h':
                        flag_half = 1;
                        goto in_scan;
                    case 'l':
                        flag_long = 1;
                        goto in_scan;

                        /* WIDTH */
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        width = ACE_strtol(format - 1, &s, 10);
                        format = s;
                        flag_width = 1;
                        goto in_scan;

                        /* scan for integer / strtol reimplementation ... */
                    case 'p':
                    case 'X':
                    case 'x':
                        _div += 6;
                    case 'd':
                        _div += 2;
                    case 'o':
                        _div += 8;
                    case 'u':
                    case 'i':
                    {
                        unsigned long v = 0;
                        unsigned int consumedsofar;
                        int neg = 0;
                        while (ACE_isspace(tpch))
                            tpch = A_GETC(fn);
                        if (tpch == '-')
                        {
                            tpch = A_GETC(fn);
                            neg = 1;
                        }

                        if (tpch == '+')
                            tpch = A_GETC(fn);

                        if (tpch == -1)
                            return n;
                        consumedsofar = consumed;

                        if (!flag_width)
                        {
                            if ((_div == 16) && (tpch == '0'))
                                goto scan_hex;
                            if (!_div)
                            {
                                _div = 10;
                                if (tpch == '0')
                                {
                                    _div = 8;
scan_hex:
                                    tpch = A_GETC(fn);
                                    if ((tpch | 32) == 'x')
                                    {
                                        tpch = A_GETC(fn);
                                        _div = 16;
                                    }
                                }
                            }
                        }
                        while ((width) && (tpch != -1))
                        {
                            register unsigned long c = tpch & 0xff;
                            register unsigned long d = c | 0x20;
                            c = (d >= 'a' ? d - 'a' + 10 : c <= '9' ? c - '0' : 0xff);
                            if (c >= _div)
                                break;
                            d = v * _div;
                            v = (d < v) ? ACE_ULONG_MAX : d + c;
                            --width;
                            tpch = A_GETC(fn);
                        }

                        if (consumedsofar == consumed)
                            return n;

                        if ((ch | 0x20) < 'p')
                        {
                            register long l = v;
                            if (v >= -((unsigned long) ACE_LONG_MIN))
                            {
                                l = (neg) ? ACE_LONG_MIN : ACE_LONG_MAX;
                            }
                            else
                            {
                                if (neg)
                                    v *= -1;
                            }
                        }
                        if (!flag_discard)
                        {
                            if (flag_long)
                            {
                                pl = (long *) ACE_va_arg(args, long*);
                                *pl = v;
                            }
                            else if (flag_half)
                            {
                                ph = (short*) ACE_va_arg(args, short*);
                                *ph = v;
                            }
                            else
                            {
                                pi = (int *) ACE_va_arg(args, int*);
                                *pi = v;
                            }
                            if (consumedsofar < consumed)
                                ++n;
                        }
                    }
                        break;

                        /* char-sequences */
                    case 'c':
                        if (!flag_discard)
                        {
                            s = (char *) ACE_va_arg(args, char*);
                            ++n;
                        }
                        if (!flag_width)
                            width = 1;
                        while (width && (tpch != -1))
                        {
                            if (!flag_discard)
                                *(s++) = tpch;
                            --width;
                            tpch = A_GETC(fn);
                        }
                        break;

                        /* string */
                    case 's':
                        if (!flag_discard)
                            s = (char *) ACE_va_arg(args, char*);
                        while (ACE_isspace(tpch))
                            tpch = A_GETC(fn);
                        if (tpch == -1)
                            break; /* end of scan -> error */
                        while (width && (tpch != -1) && (!ACE_isspace(tpch)))
                        {
                            if (!flag_discard)
                                *s = tpch;
                            if (tpch)
                                ++s;
                            else
                                break;
                            --width;
                            tpch = A_GETC(fn);
                        }
                        if (!flag_discard)
                        {
                            *s = 0;
                            ++n;
                        }
                        break;

                        /* consumed-count */
                    case 'n':
                        if (!flag_discard)
                        {
                            pi = (int *) ACE_va_arg(args, int *);
                            /* in accordance to ANSI C we don't count this conversion */
                            *pi = consumed - 1;
                        }
                        break;

                    default:
                        goto err_out;
                }
            }
                break;

                /* check if equal format string... */
            default:
                if ((unsigned char) tpch != ch)
                    goto err_out;
                tpch = A_GETC(fn);
                break;
        }
    }

    /* maybe a "%n" follows */
    if (*format)
    {
        while (ACE_isspace(*format))
            format++;
        if (format[0] == '%' && format[1] == 'n')
        {
            pi = (int *) ACE_va_arg(args, int *);
            *pi = consumed - 1;
        }
    }

err_out:
    if (tpch < 0 && n == 0)
        return ACE_EOF;
    A_PUTC(fn, tpch);
    return n;
}

