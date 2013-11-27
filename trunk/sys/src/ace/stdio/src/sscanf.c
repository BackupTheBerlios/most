#include <ace/stdio.h>


struct str_data {
    unsigned char *str;
};

static int sgetc(struct str_data* sd) {
    unsigned char c = *(sd->str++);
    return (c) ? (int)c : ACE_EOF;
}

static int sputc(struct str_data* sd, int c) {
    return (*(--sd->str) == c) ? c : ACE_EOF;
}

extern int
ACE_vsscanf(const char *str, const char *format, ACE_va_list_t args)
{
    struct str_data  fdat = { (unsigned char*)str };
    struct __ACE_arg_scanf__ farg = { (void*)&fdat, (int(*)(void*))sgetc, (int(*)(void *, int))sputc };
    return __ACE_vscanf__(&farg, format, args);
}

extern int
ACE_sscanf(const char *str, const char *format, ...)
{
    int n;
    ACE_va_list_t args;

    ACE_va_start(args, format);
    n = ACE_vsscanf(str, format, args);
    ACE_va_end (args);

    return n;
}
