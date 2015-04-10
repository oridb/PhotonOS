#include <stdio.h>
#include <tty.h>
#include <vga.h>

void putchar(char c)
{	
	tty_putchar(c);
}
