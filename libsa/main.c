#include "stdint.h"
#include "stdio.h"
#include "../libkern/gdt.h"
#include "../libkern/idt.h"
#include "../libkern/isr.h"
#include "../libkern/keyboard.h"
#include "../libkern/mouse.h"
#include "string.h"

const char* oem = "BOOT 4.0";
const char* build_ver = "1224";
const char* os_name = " Via Operating System";

#define BUFFER_SIZE 100

bool isArg = false;
bool argPassed = false;
bool CMDecho = false;
char CMDarg1[BUFFER_SIZE];
char CMDarg2[BUFFER_SIZE];

void kmain() 
{
    gdt_init();
    idt_init();
    keyboard_init();
    mouse_init();
    terminal_initialize();

    printf("%s\n", os_name);
    printf("Beta build %s\n", build_ver);
    printf("OEM Manufacturer: %s\n", oem);

    printf("> ");

    char buffer[BUFFER_SIZE];
    int buffer_index = 0;

    // BSOD Test
    // asm volatile("\txorl %edx, %edx");
    // asm volatile("\tmovl $0x7b, %eax");
    // asm volatile("\tmovl $0, %ecx");
    // asm volatile("\tidivl %ecx");

    while(1)
    {
        char c = kb_getchar();
        if (c == '\n')
        {
            buffer[buffer_index] = '\0'; // Null-terminate the string

            if(isArg)
            {
                //CMDarg1 = buffer;
                strcpy(CMDarg1, buffer);
                isArg = false;
                argPassed = true;
            }

            if(CMDecho)
            {
                printf("\n%s\n", CMDarg1);
                CMDecho = false;
            }

            if (strcmp(buffer, "hello") == 0) // Compare buffer
            {
                printf("\nHi there!\n");
            }
            else if (strcmp(buffer, "help") == 0) // Compare buffer
            {
                printf("\nList of commands:\n");
                printf("\nhello - Simply prints Hi There!\n");
                printf("echo X - prints X back\n");
            }
            else if (strcmp(buffer, "echo") == 0) // Compare buffer
            {
                printf("\nFirst argument: ");
                isArg = true;
                CMDecho = true;
            }
            else
            {
                if(!CMDecho)
                    printf("\nUnrecognized command: %s\n", buffer);
            }
            // Reset buffer for next input
            buffer_index = 0;
            if(!isArg)
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
}