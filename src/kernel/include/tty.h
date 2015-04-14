#ifndef _tty_h
#define _tty_h

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include <vga.h>
#include <system.h>
#include <stdio.h>

enum vga_color default_fg;

typedef struct video_buff {
	char text[STDIO_SIZE];
	enum vga_color fg[STDIO_SIZE];
	int index;
} video_buff_t;

typedef struct tty {
	size_t row;
	size_t column;
	enum vga_color bg;
	int tabstop;
	video_buff_t buffer;
} tty_t;

tty_t *main_tty;
tty_t *current_tty;
tty_t **ttys;
int tty_index;

/**
  * Initialize the terminal. Set the video buffer, color the screen
  * and set text font.
  */
tty_t *tty_init(tty_t *new);

/**
  * Switches current tty to the specified tty. (tty_t *other)
  */
void switch_tty(tty_t *other);

/**
  * Clear the screen.
  */
void tty_clear();

/**
  * Scroll the terminal.
  */
int tty_scroll(int row);

/**
  * Sets the foreground and background color of tty.
  */
void tty_setcolor(enum vga_color fg, enum vga_color bg);

/**
  * Create a char entry at 'x', 'y' coordonates with specified color.
  */
void tty_putentryat(char c, uint8_t color, size_t x, size_t y);

/**
  * Write a char in frame buffer.
  */
void tty_putchar(char c);

/**
  * Write a string on the screen.
  */
void tty_writestring(const char* data);

/**
  * Change size of a '\t' (tab) char.
  */
void tty_set_tab(int size);

/**
  * Move system cursor.
  */
void tty_move_cursor(int row, int column);

/**
  * Basic function for write colored char.
  */
void putchar_color(char c, enum vga_color fg);

/**
  * Basic function for write colored str.
  */
void wstr_color(const char* data, enum vga_color fg);

#endif
