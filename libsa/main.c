#include "stdint.h"
#include "stdio.h"
#include "../libkern/gdt.h"
#include "../libkern/idt.h"
#include "../libkern/isr.h"
#include "../libkern/keyboard.h"

const char* oem = "BOOT 4.0";
const char* build_ver = "1124";
const char* os_name = " Via Operating System";

#define BUFFER_SIZE 100

void kmain() 
{
    gdt_init();
    idt_init();
    keyboard_init();
    terminal_initialize();

    printf("%s\n", os_name);
    printf("Beta build %s\n", build_ver);
    printf("OEM Manufacturer: %s\n", oem);

    printf("> ");

    char buffer[BUFFER_SIZE];
    int buffer_index = 0;

    while(1)
    {
        char c = kb_getchar();
        if (c == '\n')
        {
            buffer[buffer_index] = '\0'; // Null-terminate the string

            if (strcmp(buffer, "hello") == 0) // Compare buffer
            {
                printf("\nHi there!\n");
            }
            else
            {
                printf("\nUnrecognized command : %s\n", buffer);
            }
            // Reset buffer for next input
            buffer_index = 0;
            printf("> ");
        }
        else
        {
            if (buffer_index < BUFFER_SIZE - 1) // Ensure there is space in the buffer
            {
                buffer[buffer_index++] = c; // Add character to buffer
                printf("%c", c); // Echo character to the terminal
            }
            else
            {
                printf("\nBuffer overflow. Command too long.\n");
            }
        }
    }

    // asm volatile("\txorl %edx, %edx");
    // asm volatile("\tmovl $0x7b, %eax");
    // asm volatile("\tmovl $0, %ecx");
    // asm volatile("\tidivl %ecx");
}