#include <semaphore.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include<sys/time.h>
#include<time.h>


inline uint32_t
rdtsc32(void)
{
//if defined(__GNUC__) && defined(__ARM_ARCH_7A__)
        uint32_t r = 0;
        asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
        return r;
//else
//error Unsupported architecture/compiler!
//endif
}


static inline void measureSysCall() __attribute__((always_inline));
static inline void measureSysCall()
{
        uint start = 0;
        uint end   = 0;
        int times[1000];
        int i = 0;
        uint minTime = 1000000;

        for(; i < 1000; ++i)
        {
                start = rdtsc32();
                getppid();
                //syscall(SYS_getppid);
                end = rdtsc32();
                times[i] = end - start;
                minTime = (minTime < end - start ? minTime : end - start);
        }
        i = 0;
        for(; i < 1000; ++i)
        {
                printf("getppid time: %u\n", times[i]);
        }
        printf("getppid time: %u\n", minTime);
}

void main(){

  measureSysCall();

}
