.macro vector nr
.global vector_\nr
vector_\nr:
    pushl $0
    pushl $\nr
    jmp common_vector_handler
.endm

.macro vector_error_code nr
.global vector_\nr
vector_\nr:
    pushl $\nr
    jmp common_vector_handler
.endm

// Exceptions
vector 0
vector 1
vector 2
vector 3
vector 4
vector 5
vector 6
vector 7
vector_error_code 8
vector 9
vector_error_code 10
vector_error_code 11
vector_error_code 12
vector_error_code 13
vector_error_code 14
vector 15
vector 16
vector_error_code 17
vector 18

// IRQs
vector 32
vector 33
// usw. bis 47

// Syscall
vector 48

.extern PC_handle_vector
common_vector_handler:
    // CPU-Zustand sichern
    push %ebp
    push %edi
    push %esi
    push %edx
    push %ecx
    push %ebx
    push %eax

    // Kernel-Datensegmente laden
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es

    // Handler aufrufen
    // Der Rueckgabewert ist der Prozessorzustand des moeglicherweise
    // gewechselten Tasks. Wir muessen also den Stack dorthin wechseln
    // um die Register wiederherzustellen.
    push %esp
    call PC_handle_vector
    mov %eax, %esp

    // User-Datensegmente laden
    mov $0x23, %ax
    mov %ax, %ds
    mov %ax, %es

    // CPU-Zustand wiederherstellen
    pop %eax
    pop %ebx
    pop %ecx
    pop %edx
    pop %esi
    pop %edi
    pop %ebp

    // Fehlercode und Interruptnummer vom Stack nehmen
    add $8, %esp

    iret
