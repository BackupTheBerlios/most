#ifndef DEV_AT91_DEBUG_H
#define DEV_AT91_DEBUG_H

extern void DEV_at91_DBGU_print_ascii (const char *buffer);
extern void DEV_at91_DBGU_print_hex8 (unsigned long value);
extern void DEV_at91_DBGU_configure (void);

#endif
