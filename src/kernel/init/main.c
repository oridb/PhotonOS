#include <tty.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <vga.h>
#include <isr.h>
#include <io.h>
#include <system.h>
#include <keyboard.h>
#include <time.h>
#include <kheap.h>
#include <paging.h>
#include <init.h>
#include <multiboot.h>
#include <task.h>

void kmain(multiboot *mboot_ptr, uint32_t init_stack) {
	init(mboot_ptr, init_stack);
	//welcome();
	login();
	prompt();
	panic("Shell exited!", __LINE__, __FILE__);
}
