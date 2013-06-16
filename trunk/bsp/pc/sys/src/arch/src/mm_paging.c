#include <dev/arch/ibmpc/types.h>
#include <arch/mm.h>

struct vmm_context {
    ACE_u32_t* pagedir;
};

/*
 * Dieser Speicherkontext wird nur waehrend der Initialisierung verwendet.
 * Spaeter laeuft der Kernel immer im Kontext des aktuellen Prozesses.
 */
static struct vmm_context* kernel_context;


struct vmm_context* vmm_create_context(void)
{
    struct vmm_context* context = PC_pmm_alloc();
    int i;

    /* Page Directory anlegen und mit Nullen initialisieren */
    context->pagedir = PC_pmm_alloc();
    for (i = 0; i < 1024; i++) {
        context->pagedir[i] = 0;
    }

    return context;
}

int vmm_map_page(struct vmm_context* context, uintptr_t virt, uintptr_t phys)
{
    ACE_u32_t page_index = virt / 0x1000;
    ACE_u32_t pd_index = page_index / 1024;
    ACE_u32_t pt_index = page_index % 1024;

    ACE_u32_t* page_table;
    int i;

    /* Wir brauchen 4k-Alignment */
    if ((virt & 0xFFF) || (phys & 0xFFF)) {
        return -1;
    }

    /* Page Table heraussuchen bzw. anlegen */
    if (context->pagedir[pd_index] & PTE_PRESENT) {
        /* Page Table ist schon vorhanden */
        page_table = (ACE_u32_t*) (context->pagedir[pd_index] & ~0xFFF);
    } else {
        /* Neue Page Table muss angelegt werden */
        page_table = PC_pmm_alloc();
        for (i = 0; i < 1024; i++) {
            page_table[i] = 0;
        }
        context->pagedir[pd_index] =
            (ACE_u32_t) page_table | PTE_PRESENT | PTE_WRITE;
    }

    /* Neues Mapping in the Page Table eintragen */
    page_table[pt_index] = phys | PTE_PRESENT | PTE_WRITE;
    __asm__ volatile("invlpg %0" : : "m" (*(char*)virt));

    return 0;
}

void vmm_activate_context(struct vmm_context* context)
{
    __asm__ volatile("mov %0, %%cr3" : : "r" (context->pagedir));
}

void PC_vmm_init(void)
{
    ACE_u32_t cr0;
    int i;

    /* Speicherkontext anlegen */
    kernel_context = vmm_create_context();

    /* Die ersten 4 MB an dieselbe physische wie virtuelle Adresse mappen */
    for (i = 0; i < 4096 * 1024; i += 0x1000) {
        vmm_map_page(kernel_context, i, i);
    }

    vmm_activate_context(kernel_context);

    __asm__ volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= (1 << 31);
    __asm__ volatile("mov %0, %%cr0" : : "r" (cr0));
}
