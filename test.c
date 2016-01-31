#include <unistd.h>

void foo()
{
 asm volatile("" ::: "memory");
 sleep(10);
}
main(){
foo();
return 0;
}
