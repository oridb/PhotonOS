#include <vga.h>
#include <tty.h>
#include <stdio.h>
#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

void flush_video()
{
	clear_video();
	char c = current_tty->buffer.text[0];
	int vga_row = 0;
	int vga_column = 0;
	uint8_t color;
	
	for (int i = 0; i < STDIO_SIZE && c != 0; i++) {
	c = current_tty->buffer.text[i];
	color = make_color(current_tty->buffer.fg[i], current_tty->bg);

	switch (c) {
		case '\n':
			++vga_row;
			vga_column = -1;
			break;

		case '\t':
			for (int i = 0; i < current_tty->tabstop; ++i) {
				tty_putentryat(' ', color, vga_column, vga_row);
				vga_column++;
				if (vga_column == 80) { 
					vga_row++;
					vga_column = 0; 
				}
			}
			break;

		case '\b':
			--vga_column;
			tty_putentryat(' ', color, vga_column, vga_row);
			--vga_column;
			break;

		case '\a':
			// no sound support yet
			break;

		default:
			tty_putentryat(c, color, vga_column, vga_row);
			break;
	}
	vga_column++;
	if (vga_column == 80) { 
		vga_row++;
		vga_column = 0; 
	}
	tty_scroll(vga_row);
	}
	tty_move_cursor(vga_row, vga_column);
}

void init_video()
{
	video_memory = (uint16_t*) 0xB8000;
	vga_change = 0;
	main_tty = tty_init(main_tty);
	clear_video();
}

void clear_video()
{
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			((uint16_t*) video_memory)[index] = make_vgaentry(' ', make_color(current_tty->buffer.fg[current_tty->buffer.index], current_tty->bg));
		}
	}
}
