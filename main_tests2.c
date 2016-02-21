#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

#define ARRAY_SIZE 1024  * 32 

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

static inline void measureClockRate2() __attribute__((always_inline));

static inline void measureClockRate2()
{
        uint start = 0;
        uint end   = 0;
        uint overhead = 0;
struct timeval tvalBefore, tvalAfter;  
     int i,j,k;
     int sum;
    for (k=0; k < 10; ++k)
      {
    gettimeofday (&tvalBefore, NULL);
uint useccount =0;


        start = rdtsc();
        //sleep(10);

          for (j=0;j <15000; j++)
            {
               for (i = 0;i< 5000; ++i)
                sum = i + sum;
              for (i = 0;i< 5000; ++i)
               sum = i-  sum;
    
              }

        end = rdtsc();


    gettimeofday (&tvalAfter, NULL);
     useccount = ((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000000L
           +tvalAfter.tv_usec) - tvalBefore.tv_usec;

 printf(" time ellapsed in us = %u \n",useccount);
        overhead = end - start;
        float clockRate = overhead / useccount;
        printf("cycles = %u \n", overhead);
         
        printf("clock rate: %f Mhz\n", clockRate);
     }
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

static inline void measureFork() __attribute__((always_inline));
static inline void measureFork()
{
        uint start = 0;
        uint end   = 0;
        int times[100];
        int result[100];
        int i = 0;
        uint minTime = 1000000;
        int mypid  =getpid();

        for(; i < 10; ++i)
        {
                start = rdtsc();
//              getppid();
                result[i]= fork();
                end = rdtsc();
                times[i] = end - start;
                minTime = (minTime < end - start ? minTime : end - start);
        }
        i = 0;
        for(; i < 10; ++i)
        {
                if(mypid == getpid())
                printf("fork  time: %u\n", times[i]);
                   else return;
        }
        printf(" min fork time: %u\n", minTime);
}
/*
void* doSomeThing(void *arg)
{
   int a  = 19;
    a++;
   return NULL;
}
static inline void measurepThread() __attribute__((always_inline));
static inline void measurepThread()
{
        uint start = 0;
        uint end   = 0;
        int times[100];
        int result[100];
        int i = 0;
        uint minTime = 1000000;
        pthread_t tid[10];
        for(; i < 10; ++i)
        {
                start = rdtsc();
                result[i] = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
                end = rdtsc();
                times[i] = end - start;
                minTime = (minTime < end - start ? minTime : end - start);
        }
        i = 0;
        for(; i < 10; ++i)
        {
                printf("pthread_create  time: %u\n", times[i]);
        }
        printf(" min pthreadr_create time: %u\n", minTime);
}

void* doSomeThingContextSwitch(void *arg)
{ 
      int which = PRIO_PROCESS;
      int pid;
      int priority = -20;
      int ret;
      uint start = 0;
      uint end   = 0;
      int i =0, j=0,k=0;
      int sum = 0;
      int times[100]= {0};
           char *message;
     message = (char *) arg;
     //printf("%s \n", message);
      pid = getpid();
      ret = setpriority(which, pid, priority);
     


      printf("pthread_created for context switch test  pid = %d ret =%d thread = %s :\n ", pid , ret, message);
      for (j=0;j < 5; j++) 
       {
          for (i = 0;i< 5000; ++i)
          {
           sum = i + sum; 
          start = rdtsc();
          pthread_yield();
          end = rdtsc();
          if (k < 100)
          times[k++]= end  - start;
          }
         for (i = 0;i< 5000; ++i)
         {
          sum = i-  sum;
                   start = rdtsc();
          pthread_yield();
          end = rdtsc();
          if (k < 100)
          times[k++]= end  - start;

         }
       } 
      printf("pthread_created for context switch test  pid = %d ret =%d :\n ", pid , ret);
              for(k =0 ; k < 100; ++k)
        {
                printf("pthread_context switch  time: %u for thread = %s \n", times[k], message);
        }

      return NULL;
}


static inline void measurepeThreadContextSwitch() __attribute__((always_inline));
static inline void measurepThreadContextSwitch()
{
        uint start = 0;
        uint end   = 0;
        int times[100];
        int result[100];
        int i = 0;
        uint minTime = 1000000;
        pthread_t tid[2];
        
  const char *message1 = "Thread 1";

     const char *message2 = "Thread 2";
 
         
        //for(; i < 2; ++i)
       // {
 //               start = rdtsc();
           //     result[i] = pthread_create(&(tid[i]), NULL, &doSomeThingContextSwitch, NULL);
  //              end = rdtsc();
    //            times[i] = end - start;
   //             minTime = (minTime < end - start ? minTime : end - start);
       // }

        result[0] = pthread_create(&(tid[0]), NULL, &doSomeThingContextSwitch, (void *)message1);
       result[1] = pthread_create(&(tid[1]), NULL, &doSomeThingContextSwitch, (void *)message2);
       i =0;
       for(; i < 2; ++i)

       pthread_join( tid[i], NULL);

}

*/
static inline void measureForkContextSwitch() __attribute__((always_inline));
static inline void measureForkContextSwitch()
{
        uint start = 0;
        uint end   = 0;
        int times[100];
        int result[100];
        int i = 0, j= 0, k =0;
        uint minTime = 1000000;
        int mypid  =getpid();
        int childpid =-2;
        int ret;
        int status = 0;
        int sum = 0;

int which = PRIO_PROCESS;
      int priority = -20;


        for(; i < 2; ++i)
        {      
                if (childpid != 0)
                childpid= fork();
        }
        if (mypid != getpid())
          {
        printf(" I'm child  my pid = %d \n", getpid());
      ret = setpriority(which, getpid(), priority);


             for (j=0;j < 5; j++)
       {
          for (i = 0;i< 5000; ++i)
          {
           sum = i + sum;
          start = rdtsc();
          //pthread_yield();
          sched_yield();
          end = rdtsc();
          if (k < 100)
          times[k++]= end  - start;
          }
         for (i = 0;i< 5000; ++i)
         {
          sum = i-  sum;
                   start = rdtsc();
          //pthread_yield();
//           printf("ping = %d\n", getpid());
          sched_yield();
          end = rdtsc();
          if (k < 100)
          times[k++]= end  - start;

         }
       }
              for(k =0 ; k < 100; ++k)
        {
                printf("process_context switch  time: %u  \n", times[k] );
        }


        exit(0);
      } 
       else  if(mypid == getpid()) 
       {
        printf(" I'm parent \n");
        //wait(NULL);
        //waitpid(childpid, &ret, 0);
        while(wait(&status) >0)
                 ;
        exit (0);
        }
}

static inline void linked_list(uint array_size) __attribute__((always_inline));
static inline void linked_list(uint array_size)
//void linked_list()
{

struct list_el {
  // int val;
   //int a[15];
  struct list_el * next;
};

typedef struct list_el item;
           unsigned long start = 0;
        unsigned long  end   = 0;

   item * curr, * head , *delptr;
   int i;
   int k =0;
//int times[ARRAY_SIZE] = {0};
//int values[ARRAY_SIZE]={0};
   head = NULL;
   int max = 0;
   //uint array_size = ARRAY_SIZE; 

   for(i=0;i<array_size;i++) {
      curr = (item *)malloc(sizeof(item));
    //  curr->a[7]= i;
      curr->next  = head;
      head = curr;
   }

   curr = head;
    start = rdtsc();
   while(curr) {
     // printf("%d\n", curr->val);
    //start = rdtsc();
//     values[k]= curr->val;
      curr = curr->next ;
    // end = rdtsc();
     //times[k++] = end -start;

    }
    end = rdtsc();

   

   curr = head;
   
   while(curr) {
      delptr = curr;

      curr = curr->next ;
      free (delptr);
    }


//   printf("  average = %f \n" , (float)((unsigned long)(end - start)/ array_size));

 //  printf("end = %lu , start = %lu, Array Size = %u \n", end, start, array_size);

  // printf("size of = %d \n", sizeof(item));
//  printf("  array size = %u , size of each item = %d,   average = %f \n" ,array_size , sizeof(item),  (float)((unsigned long)(end - start)/ array_size));


printf("  array size  ( = %u) *  size of each item(= %d) = %u,   average = %f \n" , array_size , sizeof(item) , array_size * sizeof(item) ,  (float)((unsigned long)(end - start)/ array_size));


  //  for (i = 0; i<ARRAY_SIZE; i++)
   // {
   //     if( times[i] > max )
    //         max = times[i] ;
//          if(times[i] > 1000)
//         printf(" latency = %u cycles for i = %d \n", times[i], i);
//           printf(" val = %d \n", values[i]);
   // }
   // printf("max = %d \n", max);

}


int main()
{
        int i =0;
        unsigned int j;
	init_perfcounters (1, 0); 
	
//	sleep(1);

     //   measureClockRate2();
//        measureFork();

         for(j =512; j <= 33554432 ; j*=2 )
           {
         for  (i = 0 ; i < 10 ; i++)
{
         
         linked_list(j);
         init_perfcounters(1,0);
}
           }
   //      measurepThread();
  // for phread you need to compile with pthread flag 
//measureForkContextSwitch();
// measurepThreadContextSwitch();	
	//measureClockRate();
	//measureClockRate();

//	measureOverhead();
	//measureOverhead();
	
	//measureProcedure();
	
//	measureSysCall();

	return 0;
}
