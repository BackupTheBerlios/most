#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <dev/arch/ibmpc/types.h>

struct multiboot_info {
    ACE_u32_t    mbs_flags;
    ACE_u32_t    mbs_mem_lower;
    ACE_u32_t    mbs_mem_upper;
    ACE_u32_t    mbs_bootdevice;
    ACE_u32_t    mbs_cmdline;
    ACE_u32_t    mbs_mods_count;
    void*       mbs_mods_addr;
    ACE_u32_t    mbs_syms[4];
    ACE_u32_t    mbs_mmap_length;
    void*       mbs_mmap_addr;
} __attribute__((packed));

struct multiboot_mmap {
    ACE_u32_t    entry_size;
    ACE_u64_t    base;
    ACE_u64_t    length;
    ACE_u32_t    type;
} __attribute__((packed));

struct multiboot_module {
    ACE_u32_t    mod_start;
    ACE_u32_t    mod_end;
    char*       cmdline;
    ACE_u32_t    reserved;
} __attribute__((packed));

#endif
