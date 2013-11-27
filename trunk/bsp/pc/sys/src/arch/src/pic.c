/*
 * pic.c
 *
 *  Created on: 30.05.2013
 *      Author: maik
 */

#include <uso/log.h>
#include <dev/arch/ibmpc/ports.h>
#include <ace/stddef.h>

#include <arch/pic.h>


extern void vector_0(void);
extern void vector_1(void);
extern void vector_2(void);
extern void vector_3(void);
extern void vector_4(void);
extern void vector_5(void);
extern void vector_6(void);
extern void vector_7(void);
extern void vector_8(void);
extern void vector_9(void);
extern void vector_10(void);
extern void vector_11(void);
extern void vector_12(void);
extern void vector_13(void);
extern void vector_14(void);
extern void vector_15(void);
extern void vector_16(void);
extern void vector_17(void);
extern void vector_18(void);

extern void vector_32(void);
extern void vector_33(void);

extern void vector_48(void);

#define IDT_ENTRIES 256
static long long unsigned int idt[IDT_ENTRIES];

#define IDT_FLAG_INTERRUPT_GATE 0xe
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0 0x00
#define IDT_FLAG_RING3 0x60

static void idt_set_entry(int i, void (*fn)(), unsigned int selector,
    int flags)
{
    unsigned long int handler = (unsigned long int) fn;
    idt[i] = handler & 0xffffLL;
    idt[i] |= (selector & 0xffffLL) << 16;
    idt[i] |= (flags & 0xffLL) << 40;
    idt[i] |= ((handler>> 16) & 0xffffLL) << 48;
}


static void init_pic(void)
{
    // Master-PIC initialisieren
    outb(0x20, 0x11); // Initialisierungsbefehl fuer den PIC
    outb(0x21, 0x20); // Interruptnummer fuer IRQ 0
    outb(0x21, 0x04); // An IRQ 2 haengt der Slave
    outb(0x21, 0x01); // ICW 4

    // Slave-PIC initialisieren
    outb(0xa0, 0x11); // Initialisierungsbefehl fuer den PIC
    outb(0xa1, 0x28); // Interruptnummer fuer IRQ 8
    outb(0xa1, 0x02); // An IRQ 2 haengt der Slave
    outb(0xa1, 0x01); // ICW 4

    // Alle IRQs aktivieren (demaskieren)
    outb(0x20, 0x0);
    outb(0xa0, 0x0);
}


extern void PC_init_vectors(void)
{
    struct {
        unsigned short int limit;
        void* pointer;
    } __attribute__((packed)) idtp = {
        .limit = IDT_ENTRIES * 8 - 1,
        .pointer = idt,
    };

    // Nummern der Vektoren umbiegen
    init_pic();

    // Excpetion-Handler
    idt_set_entry(0, vector_0, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(1, vector_1, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(2, vector_2, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(3, vector_3, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(4, vector_4, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(5, vector_5, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(6, vector_6, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(7, vector_7, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(8, vector_8, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(9, vector_9, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(10, vector_10, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(11, vector_11, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(12, vector_12, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(13, vector_13, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(14, vector_14, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(15, vector_15, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(16, vector_16, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(17, vector_17, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(18, vector_18, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);

    // IRQ-Handler
    idt_set_entry(32, vector_32, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(33, vector_33, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);

    // Syscall
    idt_set_entry(48, vector_48, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING3 | IDT_FLAG_PRESENT);

    __asm__ volatile("lidt %0" : : "m" (idtp));

}
