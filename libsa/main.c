#include "stdint.h"
#include "stdio.h"

const char* manufacturer = "DELL";
const char* build_ver = "1124";
const char* os_name = "Via Operating System";

void kmain() 
{
    terminal_initialize();

    printf("%s\n", os_name);
    printf("Beta build %s\n", build_ver);
    printf("OEM Manufacturer: %s", manufacturer);
}