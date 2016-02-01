#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

typedef unsigned int uint;

static inline unsigned int rdtsc() __attribute__((always_inline));
static inline unsigned int rdtsc()
{
	unsigned int value;
	// Read CCNT Register
	asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));  
	return value;
}

static inline void init_perfcounters (int32_t do_reset, int32_t enable_divider) __attribute__((always_inline));
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

static void measureOverhead()
{
	uint start = 0;
        uint end   = 0;
	uint overhead = 0;
	uint overheads[20];

        start = rdtsc();
	end = rdtsc();
	overhead = end - start;
	printf("overhead (single): %u\n", overhead);
	
	int i = 0;
	int j;
	int k;
	for(; i < 20; ++i)
	{
		overheads[i] = 1000000;
		k = 0;
		for(; k < 100; ++k)
		{
			j = 0;
			start = rdtsc();
			for(; j < i; ++j)
			{
			}
			end = rdtsc();
			overheads[i] = (overheads[i] < end - start ? overheads[i] : end - start);	
		}
	}
	i = 0;
	for(; i < 20; ++i)
	{
		printf("overhead (loop): %u\n", overheads[i]);
	}
}

static inline void measureClockRate() __attribute__((always_inline));

static inline void measureClockRate()
{
	uint start = 0;
        uint end   = 0;
	uint overhead = 0;

        start = rdtsc();
	sleep(10);
	end = rdtsc();
	overhead = end - start;
	float clockRate = overhead / (10.0 * 1000000);
	printf("clock rate: %f Mhz\n", clockRate);
        printf("cycles = %d \n", overhead);
}

uint inside = 0;
void procedure0() { inside = rdtsc(); }
void procedure00() { }
void procedure1(int arg1) { inside = rdtsc(); }
void procedure01(int arg1) { }
void procedure2(int arg1, int arg2) { inside = rdtsc(); }
void procedure02(int arg1, int arg2) { }
void procedure3(int arg1, int arg2, int arg3) { inside = rdtsc(); }
void procedure03(int arg1, int arg2, int arg3) { }
void procedure4(int arg1, int arg2, int arg3, int arg4) { inside = rdtsc(); }
void procedure04(int arg1, int arg2, int arg3, int arg4) { }
void procedure5(int arg1, int arg2, int arg3, int arg4, int arg5) { inside = rdtsc(); }
void procedure05(int arg1, int arg2, int arg3, int arg4, int arg5) { }
void procedure6(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) { inside = rdtsc(); }
void procedure06(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) { }
void procedure7(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7) { inside = rdtsc(); }
void procedure07(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7) { }
static inline void measureProcedure() __attribute__((always_inline));
static inline void measureProcedure()
{
	//measure(procedure0); ...
	uint start = 0;
        uint end   = 0;
	uint time = 0;
	
	int arg1, arg2, arg3, arg4, arg5, arg6, arg7;
	arg1 = 0;
	arg2 = 0;
	arg3 = 0;
	arg4 = 0;
	arg5 = 0;
	arg6 = 0;
	arg7 = 0;

	int i;
	uint minProcTime;
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure00();
		end = rdtsc();
		minProcTime = (minProcTime < end - start ? minProcTime : end - start);	
	}
	printf("procedure 0 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure0();
		minProcTime = (minProcTime < inside - start ? minProcTime : inside - start);	
	}
	printf("call 0 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		procedure0();
		end = rdtsc();
		minProcTime = (minProcTime < end - inside ? minProcTime : end - inside);	
	}
	printf("return 0 time: %u\n", minProcTime);	

	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure01(arg1);
		end = rdtsc();
		minProcTime = (minProcTime < end - start ? minProcTime : end - start);	
	}
	printf("procedure 1 time: %u\n", minProcTime);	
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure1(arg1);
		minProcTime = (minProcTime < inside - start ? minProcTime : inside - start);	
	}
	printf("call 1 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		procedure1(arg1);
		end = rdtsc();
		minProcTime = (minProcTime < end - inside ? minProcTime : end - inside);	
	}
	printf("return 1 time: %u\n", minProcTime);

	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure02(arg1, arg2);
		end = rdtsc();
		minProcTime = (minProcTime < end - start ? minProcTime : end - start);	
	}
	printf("procedure 2 time: %u\n", minProcTime);	
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure2(arg1, arg2);
		minProcTime = (minProcTime < inside - start ? minProcTime : inside - start);	
	}
	printf("call 2 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		procedure2(arg1, arg2);
		end = rdtsc();
		minProcTime = (minProcTime < end - inside ? minProcTime : end - inside);	
	}
	printf("return 2 time: %u\n", minProcTime);

	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure03(arg1, arg2, arg3);
		end = rdtsc();
		minProcTime = (minProcTime < end - start ? minProcTime : end - start);	
	}
	printf("procedure 3 time: %u\n", minProcTime);	
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure3(arg1, arg2, arg3);
		minProcTime = (minProcTime < inside - start ? minProcTime : inside - start);	
	}
	printf("call 3 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		procedure3(arg1, arg2, arg3);
		end = rdtsc();
		minProcTime = (minProcTime < end - inside ? minProcTime : end - inside);	
	}
	printf("return 3 time: %u\n", minProcTime);

	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure04(arg1, arg2, arg3, arg4);
		end = rdtsc();
		minProcTime = (minProcTime < end - start ? minProcTime : end - start);	
	}
	printf("procedure 4 time: %u\n", minProcTime);	
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure4(arg1, arg2, arg3, arg4);
		minProcTime = (minProcTime < inside - start ? minProcTime : inside - start);	
	}
	printf("call 4 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		procedure4(arg1, arg2, arg3, arg4);
		end = rdtsc();
		minProcTime = (minProcTime < end - inside ? minProcTime : end - inside);	
	}
	printf("return 4 time: %u\n", minProcTime);	

	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure05(arg1, arg2, arg3, arg4, arg5);
		end = rdtsc();
		minProcTime = (minProcTime < end - start ? minProcTime : end - start);	
	}
	printf("procedure 5 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure5(arg1, arg2, arg3, arg4, arg5);
		minProcTime = (minProcTime < inside - start ? minProcTime : inside - start);	
	}
	printf("call 5 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		procedure5(arg1, arg2, arg3, arg4, arg5);
		end = rdtsc();
		minProcTime = (minProcTime < end - inside ? minProcTime : end - inside);	
	}
	printf("return 5 time: %u\n", minProcTime);

	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure06(arg1, arg2, arg3, arg4, arg5, arg6);
		end = rdtsc();
		minProcTime = (minProcTime < end - start ? minProcTime : end - start);	
	}
	printf("procedure 6 time: %u\n", minProcTime);	
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure6(arg1, arg2, arg3, arg4, arg5, arg6);
		minProcTime = (minProcTime < inside - start ? minProcTime : inside - start);	
	}
	printf("call 6 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		procedure6(arg1, arg2, arg3, arg4, arg5, arg6);
		end = rdtsc();
		minProcTime = (minProcTime < end - inside ? minProcTime : end - inside);	
	}
	printf("return 6 time: %u\n", minProcTime);

	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure07(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		end = rdtsc();
		minProcTime = (minProcTime < end - start ? minProcTime : end - start);	
	}
	printf("procedure 7 time: %u\n", minProcTime);	
			i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		start = rdtsc();
		procedure7(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		minProcTime = (minProcTime < inside - start ? minProcTime : inside - start);	
	}
	printf("call 7 time: %u\n", minProcTime);
	i = 0;
	minProcTime = 1000000;
	for(; i < 1000; ++i)
	{
		procedure7(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		end = rdtsc();
		minProcTime = (minProcTime < end - inside ? minProcTime : end - inside);	
	}
	printf("return 7 time: %u\n", minProcTime);	
}

static inline void measureSysCall() __attribute__((always_inline));
static inline void measureSysCall()
{
	uint start = 0;
        uint end   = 0;
	int times[100];
	int i = 0;
	uint minTime = 1000000;
	
	for(; i < 100; ++i)
	{
		start = rdtsc();
//		getppid();
		syscall(SYS_getppid);
		end = rdtsc();
		times[i] = end - start;
		minTime = (minTime < end - start ? minTime : end - start);	
	}
	i = 0;
	for(; i < 100; ++i)
	{
		printf("getppid time: %u\n", times[i]);
	}
	printf("getppid time: %u\n", minTime);
}

int main()
{
	init_perfcounters (1, 0); 
	
//	sleep(1);

        measureClockRate();
	
	//measureClockRate();
	//measureClockRate();

//	measureOverhead();
	//measureOverhead();
	
	//measureProcedure();
	//measureProcedure();
	
//	measureSysCall();

	return 0;
}
