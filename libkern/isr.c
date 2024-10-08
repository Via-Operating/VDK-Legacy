#include "isr.h"
#include "idt.h"
#include "8259_pic.h"

// For both exceptions and irq interrupt
ISR g_interrupt_handlers[NO_INTERRUPT_HANDLERS];

// for more details, see Intel manual -> Interrupt & Exception Handling
char *exception_messages[32] = 
{
    "DIV_BY_ZERO",
    "DEBUG",
    "NON_MASKABLE_INT",
    "BREAKPOINT",
    "OVERFLOW",
    "BOUND_RANGE_EXCEED",
    "UNKNOWN_OPCODE",
    "DEVICE_NOT_AVAILABLE",
    "DOUBLE_FAULT",
    "COPROCESSOR_SEG_OVERRUN",
    "INVALID_TSS",
    "SEG_NOT_PRESENT",
    "STACK_SEG_FAULT",
    "GENERAL_PROTECTION",
    "PAGE_FAULT",
    "UNKNOWN_INTERRUPT (INTEL_RESERVED)",
    "FPU_ERROR",
    "ALIGNMENT_CHECK",
    "MACHINE_CHECK",
    "SIMD_FP_EXCEPTION",
    "VIRT_EXCEPTION",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED"
};

/**
 * register given handler to interrupt handlers at given num
 */
void isr_register_interrupt_handler(int num, ISR handler) 
{
    //printf("IRQ %d registered\n", num);
    if (num < NO_INTERRUPT_HANDLERS)
        g_interrupt_handlers[num] = handler;
}

/*
 * turn off current interrupt
*/
void isr_end_interrupt(int num) 
{
    pic8259_eoi(num);
}

/**
 * invoke isr routine and send eoi to pic,
 * being called in irq.asm
 */
void isr_irq_handler(REGISTERS *reg) 
{
    if (g_interrupt_handlers[reg->int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg->int_no];
        handler(reg);
    }
    pic8259_eoi(reg->int_no);
}

static void print_registers(REGISTERS *reg) 
{
    // printf("REGISTERS:\n");
    // printf("err_code=%d\n", reg->err_code);
    // printf("eax=0x%x, ebx=0x%x, ecx=0x%x, edx=0x%x\n", reg->eax, reg->ebx, reg->ecx, reg->edx);
    // printf("edi=0x%x, esi=0x%x, ebp=0x%x, esp=0x%x\n", reg->edi, reg->esi, reg->ebp, reg->esp);
    // printf("eip=0x%x, cs=0x%x, ss=0x%x, eflags=0x%x, useresp=0x%x\n", reg->eip, reg->ss, reg->eflags, reg->useresp);
}

/**
 * invoke exception routine,
 * being called in exception.asm
 */
void isr_exception_handler(REGISTERS reg) 
{
    if (reg.int_no < 32) {
        bsod_initialize();
        printf("YOUR SYSTEM HAS CRASHED BECAUSE OF A INVALID INSTRUCTION OR EXCEPTION. PLEASE RESTART YOUR SYSTEM. FOR FURTHER INFORMATION, CONTACT VIA OPERATING AND PROVIDE THEM THE EXCEPTION OR ERROR CODE.\n\n");
        printf("EXCEPTION: %s\n", exception_messages[reg.int_no]);
        printf("\nSYSTEM IS BUSY...\n");
        for (;;)
            ;
    }
    if (g_interrupt_handlers[reg.int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg.int_no];
        handler(&reg);
    }
}
