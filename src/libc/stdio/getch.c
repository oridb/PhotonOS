#include <stdio.h>
#include <pit.h>
#include <vga.h>

int getch()
{
	flush_video();
	for (;;) {
		if (((uint8_t*)stdin)[in_size] != 0) {
			in_size++;
			break;
		}
	}
	int c = ((uint8_t*)stdin)[in_size - 1];
	flush_video();
	
	return c;
}
