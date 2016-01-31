#include <linux/module.h>
#include <linux/kernel.h>
 
/*
 * works for ARM1176JZ-F
 */
 
int init_module(void)
{
    /* enable user-mode access to the performance counter */
    asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));
    /* enable all counters */
    asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));
  printk (KERN_INFO "User-level access to CCR has been turned on.\n");
  return 0;
}
 
void cleanup_module(void)
{
}
