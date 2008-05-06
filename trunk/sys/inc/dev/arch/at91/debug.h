#ifndef AT91_DEBUG_H
#define AT91_DEBUG_H

void dbgu_print_ascii(const char *buffer);
void dbgu_print_hex8(unsigned long value);
void configure_dbgu (void);

#endif /*AT91_DEBUG_H*/
