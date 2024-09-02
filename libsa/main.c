#include "stdint.h"
#include "stdio.h"
#include "../libkern/gdt.h"
#include "../libkern/idt.h"

const char* manufacturer = "TEST";
const char* build_ver = "1124";
const char* os_name = "Via Operating System";

void kmain() 
{
    gdt_init();
    idt_init();
    terminal_initialize();

    printf("%s\n", os_name);
    printf("Beta build %s\n", build_ver);
    printf("OEM Manufacturer: %s\n", manufacturer);

    asm volatile("\txorl %edx, %edx");
    asm volatile("\tmovl $0x7b, %eax");
    asm volatile("\tmovl $0, %ecx");
    asm volatile("\tidivl %ecx");
}