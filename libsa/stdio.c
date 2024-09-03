#include "stdio.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void terminal_initialize(void) 
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void bsod_initialize(void) 
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) 
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) 
{
    if(c == '\n')
    {
        terminal_row++;
        terminal_column = 0;
    }
    else
    {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    }

    if (++terminal_column == VGA_WIDTH) 
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        terminal_row = 0;
    }
}

void terminal_write(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
    terminal_write(data, strlen(data));
}

#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_LENGTH_SHORT 2
#define PRINTF_STATE_LENGTH_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_DEFAULT 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4

// Function to convert an integer to a string
void intToStr(int num, char *str) {
    int i = 0, j, temp;
    
    if (num == 0) {
        str[i++] = '0';
    } else {
        while (num > 0) {
            str[i++] = (num % 10) + '0'; // Convert digit to character
            num /= 10;
        }
        // Reverse the string
        str[i] = '\0';
        for (j = 0; j < i / 2; j++) {
            temp = str[j];
            str[j] = str[i - j - 1];
            str[i - j - 1] = temp;
        }
    }
}
int* printf_number(int* argp, int length, bool sign, int radix);

void printf(const char* fmt, ...)
{
	int* argp = (int)&fmt;
	int state = PRINTF_STATE_NORMAL;
	int length = PRINTF_LENGTH_DEFAULT;
	int radix = 10;
	bool sign = false;

	argp++;

	while(*fmt)
	{
		switch(state)
		{
			case PRINTF_STATE_NORMAL:
				switch(*fmt)
				{
					case '%': state = PRINTF_STATE_LENGTH;
							  break;
					default:  terminal_putchar(*fmt);
							  break;
				}
				break;

			case PRINTF_STATE_LENGTH:
				switch(*fmt)
				{
					case 'h':	length = PRINTF_LENGTH_SHORT;
								state = PRINTF_STATE_LENGTH_SHORT;
								break;
								
					case 'l':	length = PRINTF_LENGTH_LONG;
								state = PRINTF_STATE_LENGTH_LONG;
								break;
					default:	goto PRINTF_STATE_SPEC_;
				}
				break;

			case PRINTF_LENGTH_SHORT:
				if (*fmt == 'h')
				{
					length = PRINTF_LENGTH_SHORT_SHORT;
					state = PRINTF_STATE_SPEC;
				}
				else goto PRINTF_STATE_SPEC_;
				break;

			case PRINTF_LENGTH_LONG:
					if (*fmt == 'l')
					{
						length = PRINTF_LENGTH_LONG_LONG;
						state = PRINTF_STATE_SPEC;
					}
					else goto PRINTF_STATE_SPEC_;
					break;
			case PRINTF_STATE_SPEC:
			PRINTF_STATE_SPEC_:
				switch (*fmt)
				{
					case 'c':	terminal_putchar((char)*argp);
								argp++;
								break;

					case 's':	terminal_writestring(*(char**)argp);
								argp++;
								break;

					case '%':	terminal_putchar('%');
								break;

					case 'd':
					case 'i':	radix = 10; sign = true;
								argp = printf_number(argp, length, sign, radix);
								break;

					case 'u':	radix = 10; sign = false;
								argp = printf_number(argp, length, sign, radix);
								break;

					case 'X':
                    case 'x':
                    case 'p':   radix = 16; sign = false;
                                argp = printf_number(argp, length, sign, radix);
                                break;

                    case 'o':   radix = 8; sign = false;
                                argp = printf_number(argp, length, sign, radix);
                                break;					

					default:	break;
				}

				state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_DEFAULT;
                radix = 10;
                sign = false;
				break;
		}
		fmt++;
	}
}

const char g_HexChars[] = "0123456789abcdef";

int* printf_number(int* argp, int length, bool sign, int radix)
{
	char buffer[12]; // Enough to hold int value and null terminator

    intToStr((int)argp, buffer);
    
    terminal_writestring(buffer);
    return argp;
}