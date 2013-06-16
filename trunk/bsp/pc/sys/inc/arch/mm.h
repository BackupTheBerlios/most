#ifndef MM_H
#define MM_H

#include <arch/multiboot.h>

void PC_pmm_init(struct multiboot_info* mb_info);
void* PC_pmm_alloc(void);
void PC_pmm_free(void* page);

#define PTE_PRESENT 0x1
#define PTE_WRITE   0x2
#define PTE_USER    0x4

void PC_vmm_init(void);

#endif
