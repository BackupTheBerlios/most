#ifndef ELF_H
#define ELF_H

#include <dev/arch/ibmpc/types.h>

#define ELF_MAGIC 0x464C457F

struct elf_header {
    ACE_u32_t    magic;
    ACE_u32_t    version;
    ACE_u64_t    reserved;
    ACE_u64_t    version2;
    ACE_u32_t    entry;
    ACE_u32_t    ph_offset;
    ACE_u32_t    sh_offset;
    ACE_u32_t    flags;
    ACE_u16_t    header_size;
    ACE_u16_t    ph_entry_size;
    ACE_u16_t    ph_entry_count;
    ACE_u16_t    sh_entry_size;
    ACE_u16_t    sh_entry_count;
    ACE_u16_t    sh_str_table_index;
} __attribute__((packed));

struct elf_program_header {
    ACE_u32_t    type;
    ACE_u32_t    offset;
    ACE_u32_t    virt_addr;
    ACE_u32_t    phys_addr;
    ACE_u32_t    file_size;
    ACE_u32_t    mem_size;
    ACE_u32_t    flags;
    ACE_u32_t    alignment;
} __attribute__((packed));

#endif
