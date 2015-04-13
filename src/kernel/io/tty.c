#include <tty.h>
#include <vga.h>
#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <kheap.h>

tty_t *tty_init(tty_t *new)
{
	// basic variables of tty
	default_fg = COLOR_LIGHT_GREY;
	new = (tty_t*) kmalloc(sizeof(tty_t));
	new->row = 0;
	new->column = 0;
	new->bg = COLOR_RED;
	new->tabstop = 4;
	new->buffer.index = 0;
	
	for (int i = 0; i < STDIO_SIZE; i++) {
		new->buffer.text[i] = 0;
	}
	
	current_tty = new;
	
	return new;
}

void tty_clear()
{
	for (int index = 0; index < 25; index++) {
		tty_putchar('\n');
	}
}

void tty_scroll(int row, int column)
{
	uint16_t blank = make_vgaentry(' ', make_color(default_fg, current_tty->bg));
	
    if (row >= 25)
    {
        int temp = row - 25 + 1;
        memcpy ((uint16_t*) video_memory, (uint16_t*) video_memory + temp * 80, (25 - temp) * 80 * 2);
        
        memsetw ((uint16_t*) video_memory + (25 - temp) * 80, blank, 80);
        row = 25 - 1;
    }
}

void tty_setcolor(enum vga_color fg, enum vga_color bg)
{
	current_tty->bg = bg;
	default_fg = fg;
}

void tty_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	((uint16_t*) video_memory)[index] = make_vgaentry(c, color);
}

void tty_putchar(char c)
{	
	current_tty->buffer.text[current_tty->buffer.index] = c;
	current_tty->buffer.fg[current_tty->buffer.index] = default_fg;
	current_tty->buffer.index++;

	if (c == '\n') {
		flush_video();
	}
}

void tty_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ )
		tty_putchar(data[i]);
}

void tty_set_tab(int size)
{
	current_tty->tabstop = size;
}

void tty_move_cursor(int row, int column)
{
	uint16_t cursor_pos = row * 80 + column - 1;
	outb(0x3D4, 14);
	outb(0x3D5, cursor_pos >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, cursor_pos);
}

void clear_line(int line)
{
	/*current_tty->row = line;
	current_tty->column = 0;
	for (int i = 0; i <= 80; i++) {
		tty_putchar(' ');
	}
	current_tty->column = 0;
	current_tty->row = line;
	tty_move_cursor();*/
}

void tty_putchar_color(char c, enum vga_color fg)
{
	uint8_t temp_color = default_fg;
	default_fg = fg;
	tty_putchar(c);
	default_fg = temp_color;
}

void wstr_color(const char* data, enum vga_color fg)
{
	for ( size_t i = 0, n = strlen(data); i < n; i++ )
		tty_putchar_color((int) ((const unsigned char*) data)[i], fg);
}
