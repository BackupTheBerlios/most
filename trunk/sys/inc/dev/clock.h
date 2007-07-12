#ifndef DEV_CLOCK_H
#define DEV_CLOCK_H

extern void DEV_clock_init (void);
extern void DEV_clock_tick (void);
extern unsigned long DEV_get_ticks (void);
extern unsigned long DEV_get_ticks_diff (unsigned long old_ticks);


#endif /* DEV_CLOCK_H */
