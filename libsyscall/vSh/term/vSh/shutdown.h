#include "stdint.h"
#include "stdio.h"
#include "../../../libkern/keyboard.h"

void shutdown()
{
	// Currently in this system, there are none processes.
	terminal_initialize();
	printf("\nSEE YOU SOON!\n");
	printf("HASTA LA VISTA!\n");
	printf("A BIENTOT!\n");
	printf("BIS BALD!\n");
	printf("ARRIVEDERCI!\n");
}