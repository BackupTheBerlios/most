#ifndef SAM_LOWLEVEL_H
#define SAM_LOWLEVEL_H

void SAM_prefetch_abort(void);
void SAM_data_abort(void);
void SAM_sys_interrupt_init(void);

void SAM_cpu_init(void);
void SAM_io_init (void);

#endif /* SAM_LOWLEVEL_H*/
