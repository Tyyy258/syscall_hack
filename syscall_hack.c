#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/time.h>
//#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>

unsigned long *sys_call_table;

unsigned int clear_and_return_cr0(void);
void setback_cr0(unsigned int val);
static int sys_hackcall(void);

unsigned long *sys_call_table = 0;

static int (*orig_syscall_saved)(void);

unsigned int clear_and_return_cr0(void)
{
    unsigned int cr0 = 0;
    unsigned int ret;

    
    asm volatile ("movq %%cr0, %%rax" : "=a"(cr0));

    ret = cr0;
    cr0 &= 0xfffeffff;  

   
    asm volatile ("movq %%rax, %%cr0" :: "a"(cr0));

    return ret;
}

void setback_cr0(unsigned int val)
{
    asm volatile ("movq %%rax, %%cr0" :: "a"(val));
}

static int sys_hackcall(void)
{
    printk("Hack syscall is successful!!!\n");
    return 0;
}

static int __init init_hack_module(void)
{
    int orig_cr0;

    printk("Hack syscall is starting...\n");

   
    sys_call_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");

   
    orig_syscall_saved = (int(*)(void))(sys_call_table[__NR_perf_event_open]);  //此处可填298

    orig_cr0 = clear_and_return_cr0(); 
    sys_call_table[__NR_perf_event_open] = (unsigned long)&sys_hackcall; 
    setback_cr0(orig_cr0);

    return 0;
}

static void __exit exit_hack_module(void)
{
    int orig_cr0;

    orig_cr0 = clear_and_return_cr0();
    sys_call_table[__NR_perf_event_open] = (unsigned long)orig_syscall_saved; 
    setback_cr0(orig_cr0);

    printk("Hack syscall is exited....\n");
}

module_init(init_hack_module);
module_exit(exit_hack_module);
MODULE_LICENSE("GPL");
