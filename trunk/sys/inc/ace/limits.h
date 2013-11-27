#ifndef ACE_LIMITS_H
#define ACE_LIMITS_H

#define ACE_CHAR_MAX     (0x7f)
#define ACE_SHORT_MAX    (0x7fff)
#define __ACE_INT_MAX__  (0x7fffffff)
#define ACE_LONG_MAX     (0x7fffffffl)

#if defined MOST_CPU_H8300
#define ACE_INT_MAX      ACE_SHORT_MAX
#elif defined MOST_CPU_I386
#define ACE_INT_MAX      __ACE_INT_MAX__
#elif defined MOST_CPU_ARM
#define ACE_INT_MAX      __ACE_INT_MAX__
#else
#error "CPU not defined!"
#endif



#define ACE_CHAR_MIN     (-1 - ACE_CHAR_MAX)
#define ACE_SHORT_MIN    (-1 - ACE_SHORT_MAX)
#define ACE_INT_MIN      (-1 - ACE_INT_MAX)
#define ACE_LONG_MIN     (-1l - ACE_LONG_MAX)


#define ACE_UCHAR_MAX    (ACE_CHAR_MAX * 2 + 1)
#define ACE_USHORT_MAX   (ACE_SHORT_MAX * 2 + 1)
#define ACE_UINT_MAX     (ACE_INT_MAX * 2u + 1)
#define ACE_ULONG_MAX    (ACE_LONG_MAX * 2ul + 1)


#define ACE_SIZE_MAX     ACE_ULONG_MAX
#define ACE_SSIZE_MIN    ACE_LONG_MIN
#define ACE_SSIZE_MAX    ACE_LONG_MAX


#endif
