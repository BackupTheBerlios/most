/*
 * reset.S
 *
 *  Created on: 26.05.2013
 *      Author: maik
 */



.section multiboot

#define MB_MAGIC 0x1badb002
#define MB_FLAGS 0x0
#define MB_CHECKSUM -(MB_MAGIC + MB_FLAGS)
#define STACK_INIT_VALUE 0xA5A5A5A5 /* must be the same as in uso/stack.h */

// Der Multiboot-Header
.align 4
.int    MB_MAGIC
.int    MB_FLAGS
.int    MB_CHECKSUM

.section .text

// Init ist eine Funktion aus init.c
.extern PC_init_kernel

// reset_kernel muss global sein, damit der Linker es findet und als Einsprungspunkt
// benutzen kann (alle Labels, die nicht global sind, sind nur in dieser Datei
// sichtbar)
.global reset_kernel
reset_kernel:

    // Stack mit A5 Werten initialisieren
	mov init_stack_value, %eax
	mov $init_stack_start, %ecx
	mov $init_stack_end, %edx
write:
	mov %eax, (%ecx)
	add  $4, %ecx
	cmp %ecx, %edx
	jnz write

    // Stack Pointer initialisieren
	mov %edx, %esp

    // C-Code aufrufen und Multiboot-Infostruktur als Parameter uebergeben
    push %ebx
    call PC_init_kernel

    // Falls wir jemals aus init zurueckkommen sollten, gehen wir in eine
    // Endlosschleife
    jmp .

// 8 kB Stack fuer den Kernel. Das Label steht hinter dem freien Speicher,
// weil der Stack nach unten waechst

.section .data
init_stack_value:
.long STACK_INIT_VALUE

.section .bss
.global init_stack_start
.global init_stack_end


init_stack_start:
.space 8192
init_stack_end:
