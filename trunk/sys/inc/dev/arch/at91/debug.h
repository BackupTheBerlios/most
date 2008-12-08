#ifndef DEV_AT91_DEBUG_H
#define DEV_AT91_DEBUG_H

extern void DEV_at91_dbgu_print_ascii(const char *buffer);
extern void DEV_at91_dbgu_print_hex8(unsigned long value);
extern void DEV_at91_configure_dbgu (void);

#endif
