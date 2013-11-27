#include <ace/ctype.h>
#include <ace/stdlib.h>
#include <ace/err.h>
#include <ace/limits.h>
#include <uso/scheduler.h>

extern unsigned long
ACE_strtoul(const char *ptr, char **endptr, int base)
{
    int neg = 0, overflow = 0;
    unsigned long int v=0;
    const char *orig;
    const char *nptr=ptr;

    while(ACE_isspace(*nptr)) { ++nptr; }

    if (*nptr == '-') { neg = 1; nptr++; }
    else if (*nptr == '+') { ++nptr; }
    orig = nptr;
    if (base == 16 && nptr[0] == '0') { goto skip0x; }
    if (base) {
        register unsigned int b = base-2;
        if (b > 34) {
            USO_current()->error = DEF_ERR_ARG;
            return 0;
        }
    } else {
        if (*nptr == '0') {
            base=8;
            skip0x:
            if ((nptr[1] == 'x' || nptr[1] == 'X') && ACE_isxdigit(nptr[2])) {
                nptr += 2;
                base=16;
            }
        } else { base=10; }
    }
    while(*nptr) {
        register unsigned char c=*nptr;
        c = ( c >= 'a' ? c-'a'+10 : c >= 'A' ? c-'A'+10 : c <= '9' ? c-'0' : 0xff);
        if (c >= base) break;   /* out of base */
        {
            register unsigned long x = (v & 0xff) * base + c;
            register unsigned long w = (v >> 8) * base + (x >> 8);
            if (w > (ACE_ULONG_MAX >> 8)) { overflow=1; }
            v = (w << 8)+(x & 0xff);
        }
        ++nptr;
    }
    if (nptr==orig) { /* no conversion done */
        nptr=ptr;
        USO_current()->error = DEF_ERR_ARG;
        v = 0;
    }
    if (endptr) *endptr=(char *)nptr;
    if (overflow) {
        USO_current()->error = DEF_ERR_RANGE;
        return ACE_ULONG_MAX;
    }
    return ( neg ? -v : v);
}
