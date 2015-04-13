#include <stdio.h>
#include <tty.h>
#include <vga.h>

void putchar(char c)
{	
	write_char(c);
}
