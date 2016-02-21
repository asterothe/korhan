#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

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
   
        // do some heavy work
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

static inline void linked_list(uint array_size) __attribute__((always_inline));
static inline void linked_list(uint array_size)
{
   // in here 4 bytes is base size for the struct due to the pointer
   // the rest is up to us. we can put only one int and make it 8 bytes or put an   
   // array of ints and make the stride 4 + size of array x 4
   struct item_ {
      int a[15];
      struct item_ * next;
   } ;
  
   typedef struct item_ item;
   unsigned long start = 0;
   unsigned long  end   = 0;
   item * curr, * head , *delptr;
   int i;
   head = NULL;
 
   // allocate as many as array_size items
   // no need to fill the array, we can turn it on if we want
   for(i=0;i<array_size;i++) 
   {
      curr = (item *)malloc(sizeof(item));
      //curr->a[7]= i;
      curr->next  = head;
      head = curr;
   }

   //  walk through the array.
   //  we can measure each item before and after access or
   //  walk through all and get the average
   //  right now latter is active
   curr = head;
   start = rdtsc();
   while(curr) 
   {    
     //start = rdtsc();   
      curr = curr->next ;
     //end = rdtsc();
    }
    end = rdtsc();


   // free the allocated memory
    
   curr = head;
   while(curr) {
      delptr = curr;
      curr = curr->next ;
      free (delptr);
    }

    printf("  array size  ( = %u) *  size of each item(= %d) = %u,   average = %f \n" , array_size , sizeof(item) , array_size * sizeof(item) ,  (float)((unsigned long)(end - start)/ array_size));



}


int main()
{
        int          i =0;
        unsigned int j;
        init_perfcounters (1, 0);

         //measureClockRate2();

        // outer loop is for array size, 
         for(j = 512; j <= 33554432 ; j*=2 )
         { 
           // inner loop is for the number of trials for each array size 
           for  (i = 0 ; i < 10 ; i++)
           { 
              linked_list(j);
              // need to reset counters 
              init_perfcounters(1,0);
           }
         }
         
         
        return 0;
}

