#include <task.h>
#include <gdt.h>
#include <kheap.h>
#include <time.h>
#include <system.h>

void *syscalls[] = {
	&write
};
int num_syscalls = 1;

void init_usermode() {
	set_kernel_stack((uint32_t) NULL);
    asm volatile("  \
      cli; \
      mov $0x23, %ax; \
      mov %ax, %ds; \
      mov %ax, %es; \
      mov %ax, %fs; \
      mov %ax, %gs; \
                    \
       \
      mov %esp, %eax; \
      pushl $0x23; \
      pushl %esp; \
      pushf; \
      pushl $0x1B; \
      push $1f; \
      iret; \
    1: \
      ");
}

void syscall_handler(registers_t *regs)
{
	if (regs->eax > num_syscalls)
		return;

	void *location = syscalls[regs->eax - 1];

	int ret;
	asm volatile (" \
		push %1; \
		push %2; \
		push %3; \
		push %4; \
		push %5; \
		call *%6; \
		pop %%ebx; \
		pop %%ebx; \
		pop %%ebx; \
		pop %%ebx; \
		pop %%ebx; \
		" : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
		regs->eax = ret;
}

void init_tasking()
{
	pid = 1;
	uint32_t cr3, eflags;
	asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=r"(cr3)::"%eax");
	asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=r"(eflags)::"%eax");
	
	start_task = create_task(start_task, &kmain, eflags, cr3);
	
	current_task = start_task;
}

task_t *create_task(task_t *new_task, void (*main)(), int32_t flags, uint32_t pagedir)
{
	new_task = (task_t*) kmalloc(sizeof(task_t));
	new_task->pid = pid;
	new_task->regs.eax = 0;
	new_task->regs.ebx = 0;
	new_task->regs.ecx = 0;
	new_task->regs.edx = 0;
	new_task->regs.esi = 0;
	new_task->regs.edi = 0;
	new_task->regs.eflags = flags;
	new_task->regs.eip = (uint32_t) main;
	new_task->regs.cr3 = (uint32_t) pagedir;
	new_task->regs.esp = (uint32_t) kmalloc(2000);
	new_task->next = 0;
	
	if (pid != (uint32_t) 1) {
		task_t *tmp_task = start_task;
		while (tmp_task->next)
			tmp_task = tmp_task->next;
		tmp_task->next = new_task;
	}
	pid++;
	return new_task;
}

void preempt()
{
	cli();
	printk("Current task: %x\n", current_task);
	printk("Start task: %x\n", start_task);
	printk("Main address: %x\n", current_task->regs.eip);
	switch_task();
	sti();
}

void switch_task()
{  
    uint32_t esp, ebp, eip;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();
    if (eip == 0x12345)
        return;

    current_task->regs.eip = eip;
    current_task->regs.esp = esp;
    current_task->regs.ebp = ebp;
    
    current_task = current_task->next;
    if (!current_task) {
    	current_task = start_task;
    	return;	
    }
    printk("Current task: %x\n", current_task);
    printk("Current task regs: %x\n", &current_task->regs);

    eip = current_task->regs.eip;
    esp = current_task->regs.esp;
    ebp = current_task->regs.ebp;
    
    printk("%x\n", &update_time);
    printk("%x\n", eip);
    printk("%x\n", esp);
    printk("%x\n", ebp);
    printk("%x\n", current_task->regs.cr3);
    
    asm volatile("         \
      mov %0, %%ecx;       \
      mov %1, %%esp;       \
      mov %2, %%ebp;       \
      mov %3, %%cr3;       \
      mov $0x12345, %%eax; \
      jmp *%%ecx           "
                 : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_task->regs.cr3));
                     keep_running();
}
