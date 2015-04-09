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
#include <stddef.h>
#include <task.h>
#include <multiboot.h>


#define FREQ 100
uint8_t inbuffer[STDIN_SIZE];

void init_stdio() {
	stdin = (uint8_t*) inbuffer;
	for (int i = 0; i <= STDIN_SIZE; i++) {
		((uint8_t*)stdin)[i] = 0;
	}
} 

void init(multiboot *mboot_ptr, uint32_t init_stack) {
	init_esp = init_stack;

	tty_init();
	printk("Initialized tty.\n");

	init_gdt();
	printk("Initialized GDT.\n");
	
	init_idt();
	printk("Initialized IDT and interrupts.\n");
	
	init_timer(FREQ);
	printk("Install timer and clock.\n");
	
	init_stdio();
	printk("Initialized stdio.\n");
	
	install_keyboard();
	printk("Install keyboard support.\n");
	
	init_paging(mboot_ptr);
	printk("Kernel starts at: %x\n", (size_t)&kernel_start);
	printk("Kernel ends at: %x\n", (size_t)&kernel_end);
	printk("RAM: %d MB\n", mem_size_mb);
	printk("Paging initialized.\n");
	
	/* tasking is useless, because switcher crashes every time
	init_tasking();
	
	uint32_t cr3, eflags;
	asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=r"(cr3)::"%eax");
	asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=r"(eflags)::"%eax");
	
	task_t *clock_task = create_task(clock_task, &update_time, eflags, cr3);
	*/

	printk("Done!");
	sti();
	
	getch();
}

void welcome() {
	tty_clear();	
	
	wstr_color("      |\\     /|(  ____ \\( \\      (  ____ \\(  ___  )(       )(  ____ \\\n", COLOR_RED);
	wstr_color("      | )   ( || (    \\/| (      | (    \\/| (   ) || () () || (    \\/\n", COLOR_RED);
	wstr_color("      | | _ | || (__    | |      | |      | |   | || || || || (\\\n", COLOR_RED);
	wstr_color("      | |( )| ||  __)   | |      | |      | |   | || |(_)| ||  __)\\\n", COLOR_RED);   
	wstr_color("      | || || || (      | |      | |      | |   | || |   | || (\\\n", COLOR_RED);      
	wstr_color("      | () () || (____/\\| (____/\\| (____/\\| (___) || )   ( || (____/\\\n", COLOR_RED);
	wstr_color("      (_______)(_______/(_______/(_______/(_______)|/     \\|(_______/\n\n", COLOR_RED);
	
	wstr_color("                           \\__   __/(  ___  )\n", COLOR_YELLOW);
	wstr_color("                              ) (   | (   ) |\n", COLOR_YELLOW);
	wstr_color("                              | |   | |   | |\n", COLOR_YELLOW);
	wstr_color("                              | |   | |   | |\n", COLOR_YELLOW);
	wstr_color("                              | |   | |   | |\n", COLOR_YELLOW);
	wstr_color("                              | |   | (___) |\n", COLOR_YELLOW);
	wstr_color("                              )_(   (_______)\n\n", COLOR_YELLOW);
	
	wstr_color(" _______           _______ _________ _______  _        _______  _______ \n", COLOR_BLUE);
	wstr_color("(  ____ )|\\     /|(  ___  )\\__   __/(  ___  )( (    /|(  ___  )(  ____ \\\\\n", COLOR_BLUE);
	wstr_color("| (    )|| )   ( || (   ) |   ) (   | (   ) ||  \\  ( || (   ) || (    \\/\n", COLOR_BLUE);
	wstr_color("| (____)|| (___) || |   | |   | |   | |   | ||   \\ | || |   | || (_____ \n", COLOR_BLUE);
	wstr_color("|  _____)|  ___  || |   | |   | |   | |   | || (\\ \\) || |   | |(_____  )\n", COLOR_BLUE);
	wstr_color("| (      | (   ) || |   | |   | |   | |   | || | \\   || |   | |      ) |\n", COLOR_BLUE);
	wstr_color("| )      | )   ( || (___) |   | |   | (___) || )  \\  || (___) |/\\____) |\n", COLOR_BLUE);
	wstr_color("|/       |/     \\|(_______)   )_(   (_______)|/    )_)(_______)\\_______)\n", COLOR_BLUE);
	
	printk("                                                        by Feraru Mihail");
	
	getch();
	tty_clear();
}

void login() {
	wstr_color(" _______           _______ _________ _______  _        _______  _______ \n", COLOR_BLUE);
	wstr_color("(  ____ )|\\     /|(  ___  )\\__   __/(  ___  )( (    /|(  ___  )(  ____ \\\\\n", COLOR_BLUE);
	wstr_color("| (    )|| )   ( || (   ) |   ) (   | (   ) ||  \\  ( || (   ) || (    \\/\n", COLOR_BLUE);
	wstr_color("| (____)|| (___) || |   | |   | |   | |   | ||   \\ | || |   | || (_____ \n", COLOR_BLUE);
	wstr_color("|  _____)|  ___  || |   | |   | |   | |   | || (\\ \\) || |   | |(_____  )\n", COLOR_BLUE);
	wstr_color("| (      | (   ) || |   | |   | |   | |   | || | \\   || |   | |      ) |\n", COLOR_BLUE);
	wstr_color("| )      | )   ( || (___) |   | |   | (___) || )  \\  || (___) |/\\____) |\n", COLOR_BLUE);
	wstr_color("|/       |/     \\|(_______)   )_(   (_______)|/    )_)(_______)\\_______)\n", COLOR_BLUE);
	
	printk("Log in please.\n");
	printk("Username: ");
	gets(user);
	printk("Machine: ");
	gets(machine);
	printk("Loged in!\n");
}
