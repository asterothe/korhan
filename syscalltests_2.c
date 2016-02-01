nline void measureSysCall() __attribute__((always_inline));
static inline void measureSysCall()
{
        uint start = 0;
        uint end   = 0;
        int times[1000];
        int i = 0;
        uint minTime = 1000000;

        for(; i < 1000; ++i)
        {
                start = rdtsc();
                getppid();
                syscall(SYS_getppid);
                end = rdtsc();
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
