#include <stdio.h>
#include <stdint.h>

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

static inline unsigned int get_cyclecount (void)  __attribute__((always_inline));
static inline unsigned int get_cyclecount (void)
{
 unsigned int value;
 // Read CCNT Register
 asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));  
 return value;
}

static inline void init_perfcounters (int32_t do_reset, int32_t enable_divider)
{
 // in general enable all counters (including cycle counter)
 int32_t value = 1;

 // peform reset:  
 if (do_reset)
 {
   value |= 2;     // reset all counters to zero.
   value |= 4;     // reset cycle counter to zero.
 } 

 if (enable_divider)
   value |= 8;     // enable "by 64" divider for CCNT.

 value |= 16;

 // program the performance-counter control-register:
 asm volatile ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(value));  

 // enable all counters:  
 asm volatile ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000000f));  

 // clear overflows:
 asm volatile ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000000f));
}

int main()
{
  int i;
 // init counters:
 init_perfcounters (1, 0); 
 int cycles_array[100];
 int results_array[100];
 // measure the counting overhead:
 unsigned int overhead = get_cyclecount();
 overhead = get_cyclecount() - overhead;    

 unsigned int t = get_cyclecount();

 // do some stuff here..
 for (i = 0; i<100; i++)
 {
    results_array[i] =  syscall(SYS_getppid);
  
 t = get_cyclecount() - t;
     cycles_array[i]= t;
 }

  for (i = 0; i<100; i++)
 {
   printf (" try %d  for getpid() = %d  took exactly %d cycles \n", i, results_array[i], cycles_array[i] );
 }

 printf ("function took exactly %d cycles (%d overhead)\n", t, overhead);
 return 0;
}
