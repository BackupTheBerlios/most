#include <ace/ctype.h>
#include <ace/err.h>
#include <ace/limits.h>
#include <ace/stdlib.h>
#include <uso/scheduler.h>

extern long
ACE_strtol(const char *nptr, char **endptr, int base)
{
    int neg = 0;
    unsigned long int v;
    const char*orig = nptr;

    while(ACE_isspace(*nptr)) nptr++;

    if (*nptr == '-' && ACE_isalnum(nptr[1])) { neg = -1; ++nptr; }
    v = ACE_strtoul(nptr, endptr, base);
    if (endptr && *endptr == nptr) *endptr = (char *)orig;
    if (v >= ACE_LONG_MIN) {
        if (v == ACE_LONG_MIN && neg) {
            USO_current()->error = ACE_OK;
            return v;
        }
        USO_current()->error = DEF_ERR_RANGE;
        return (neg ? ACE_LONG_MIN : ACE_LONG_MAX);
    }
    return (neg ? -v : v);
}
