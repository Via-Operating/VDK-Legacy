// made by kap and theo

#include "stdint.h"
#include "stdio.h"
#include "../../../libkern/keyboard.h"

void VDKChangeDir(char* x)
{
	terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
	printf("\n[ERROR] ");
	terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
	printf("Unsupported\n");

	printf("> ");
}