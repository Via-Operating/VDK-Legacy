#include "stdint.h"
#include "stdio.h"
#include "../libkern/gdt.h"
#include "../libkern/idt.h"
#include "../libkern/isr.h"
#include "../libkern/keyboard.h"
#include "../libkern/mouse.h"
#include "string.h"
#include "../libkern/ide.h"
#include "../osfs/fs.h"

const char* oem = "BOOT 4.0";
const char* build_ver = "1224";
const char* os_name = " Via Operating System";

#define BUFFER_SIZE 100

bool isArg = false;
bool argPassed = false;
bool CMDecho = false;
bool CMDread = false;
char CMDarg1[BUFFER_SIZE];
char CMDarg2[BUFFER_SIZE];

// Trigger Makeflow

void kmain() 
{
    gdt_init();
    idt_init();
    keyboard_init();
    mouse_init();
    terminal_initialize();

    ata_init();

    printf("%s\n", os_name);
    printf("Beta build %s\n", build_ver);
    printf("OEM Manufacturer: %s\n", oem);

    char buffer[BUFFER_SIZE];
    int buffer_index = 0;

    // BSOD Test
    // asm volatile("\txorl %edx, %edx");
    // asm volatile("\tmovl $0x7b, %eax");
    // asm volatile("\tmovl $0, %ecx");
    // asm volatile("\tidivl %ecx");

    const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
    const uint32_t LBA = 0;
    const uint8_t NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    char bofre[ATA_SECTOR_SIZE] = {0};
    char bofreA[ATA_SECTOR_SIZE] = {0};

    VD_Entry testE;

    // Setup TEXT.TXT
    strcpy(testE.Name, "TEST.TXT");
    testE.FirstSector = 1;
    testE.Attributes = VD_ATTRIBUTE_NONE;
    testE.IsDirectory = false;
    testE.Position = 0;

    // Write to FS
    VD_Write(testE);

    // Setup LOL.TXT
    strcpy(testE.Name, "LOL.TXT");
    testE.FirstSector = 3;
    testE.Position = 1;

    // Write to FS
    VD_Write(testE); 

    // Open TEST.TXT
    testE = VD_Open("TEST.TXT");
    printf("\nFile found: %s\n", testE.Name);

    // Write and read data to TEST.TXT
    strcpy(bofre, "Hello world from a file!!!!!");
    VD_WriteData(bofre, testE);
    VD_ReadData("TEST.TXT", bofreA);
    printf("\nContents: %s\n", bofreA);

    // Open LOL.TXT (with checks for invalid file)
    testE = VD_Open("LOL.TXT");
    if(strcmp("NULL", testE.Name) == 0 && testE.Position == -1)
    {
        printf("\nFile not found: %s\n", "LOL.TXT");   
    }
    else
    {
        printf("\nFile found: %s\n", testE.Name);
    }

    // Write and read data to LOL.TXT
    strcpy(bofre, "Another text file!!!!!!!!!!!!");
    VD_WriteData(bofre, testE);
    VD_ReadData("LOL.TXT", bofreA);
    printf("\nContents: %s\n", bofreA);

    // >
    printf("> ");

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

            if(CMDread)
            {
                VD_ReadData(CMDarg1, bofreA);
                printf("\nContents: %s\n", bofreA);
                CMDread = false;
            }

            if (strcmp(buffer, "hello") == 0) // Compare buffer
            {
                printf("\nHi there!\n");
            }
            else if (strcmp(buffer, "help") == 0) // Compare buffer
            {
                printf("\nList of commands:\n");
                printf("\nhello - Simply prints Hi There!.\n");
                printf("echo X - Repeats X.\n");
                printf("read X - Reads file X and prints it's contents.\n");
            }
            else if (strcmp(buffer, "echo") == 0) // Compare buffer
            {
                printf("\nFirst argument: ");
                isArg = true;
                CMDecho = true;
            }
            else if (strcmp(buffer, "read") == 0) // Compare buffer
            {
                printf("\nFirst argument: ");
                isArg = true;
                CMDread = true;
            }
            else
            {
                if(!CMDecho || !CMDread)
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
