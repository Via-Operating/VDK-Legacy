/*
	Stage 2 VDK Loader
	Written by fellow VDK Developer(s):
	Kap Petrov

	Credits:
	- nanobyte
*/

#include "stdint.h"
#include "stdio.h"

int ageOfUser = 13;
int build_Ver = 90124;

void _cdecl cstart_(uint16_t bootDrive)
{
	const char far_str = "far string";

    puts("Please wait while we are loading your operating system...\r\n");
    printf("Beta build %d\r\n", build_Ver);

   	printf("You are %d years old\r\n", ageOfUser);

    for (;;);
}
