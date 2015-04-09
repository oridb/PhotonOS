#ifndef _system_h
#define _system_h

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <multiboot.h>

#define OS_Name "PhotonOS"
#define Version "alpha-3"
#define Relase_Date "1 April 2015"
#define Author "Feraru Mihail"

#define PIC1		0x20		
#define PIC2		0xA0		
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define sti() asm volatile("sti")
#define cli() asm volatile("cli")
#define hlt() asm volatile("hlt")
#define keep_running() while(true) { hlt(); }

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

typedef struct registers
{
    uint32_t ds;                  // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t; // registers for interrupts

typedef struct g_regs
{
	uint32_t edi;
	uint32_t esi;
	uint32_t edx;
	uint32_t ecx;
	uint32_t ebx;
	uint32_t eax;
} g_regs_t; // general registers for system calls

typedef struct t_regs {
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} t_regs_t; // task registers

typedef void (*isr_t)(registers_t*);
void register_interrupt_handler(uint8_t n, isr_t handler);

typedef void* type_t;
extern size_t kernel_end;
extern size_t kernel_start;
uint32_t init_esp;

char user[20];
char machine[30];

void kmain(multiboot *mboot_ptr, uint32_t init_stack);
void panic(char *msg, int line, char *file);
void reboot();
void shell(char *str);
void prompt();

#endif
