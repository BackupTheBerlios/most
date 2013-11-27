#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <arch/multiboot.h>

struct cpu_state {
    // Von Hand gesicherte Register
    ACE_u32_t   eax;
    ACE_u32_t   ebx;
    ACE_u32_t   ecx;
    ACE_u32_t   edx;
    ACE_u32_t   esi;
    ACE_u32_t   edi;
    ACE_u32_t   ebp;

    ACE_u32_t   vector;
    ACE_u32_t   error;

    // Von der CPU gesichert
    ACE_u32_t   eip;
    ACE_u32_t   cs;
    ACE_u32_t   eflags;
    ACE_u32_t   esp;
    ACE_u32_t   ss;
};

struct cpu_state* PC_handle_vector(struct cpu_state* cpu);

#endif
