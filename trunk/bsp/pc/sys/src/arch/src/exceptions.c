#include <uso/io_ports.h>
#include <arch/exceptions.h>
#include <arch/ticks.h>
#include <arch/kbd.h>
#include <uso/log.h>


struct cpu_state* syscall(struct cpu_state* cpu)
{
    /*
     * Der Aufrufer uebergibt in eax die Nummer des Syscalls. In den weiteren
     * Registern werden die Parameter uebergeben.
     */
    switch (cpu->eax) {
        case 0: /* putc */
            USO_log_printf(USO_LL_INFO, "%c", cpu->ebx);
            break;
    }

    return cpu;
}

struct cpu_state* PC_handle_vector(struct cpu_state* cpu)
{
    struct cpu_state* new_cpu = cpu;

    if (cpu->vector <= 0x1f) {
        USO_log_printf(USO_LL_INFO, "Exception %d, Kernel angehalten!\n", cpu->vector);

        // TODO Hier den CPU-Zustand ausgeben

        while(1) {
            // Prozessor anhalten
            __asm__ volatile("cli; hlt");
        }
    } else if (cpu->vector >= 0x20 && cpu->vector <= 0x2f) {

        if (cpu->vector == 0x20) {
        	PC_ticks_interrupt();  //new_cpu = schedule(cpu);
            //tss[1] = (uint32_t) (new_cpu + 1);
        }

        if (cpu->vector == 0x21) {
        	PC_kbd_interrupt();
        }

        if (cpu->vector >= 0x28) {
            // EOI an Slave-PIC
            USO_out_b(0xa0, 0x20);
        }
        // EOI an Master-PIC
        USO_out_b(0x20, 0x20);
    } else if (cpu->vector == 0x30) {
        new_cpu = syscall(cpu);
    } else {
        USO_log_puts(USO_LL_PANIC, "Unbekannter Interrupt\n");
        while(1) {
            // Prozessor anhalten
            __asm__ volatile("cli; hlt");
        }
    }

    return new_cpu;
}
