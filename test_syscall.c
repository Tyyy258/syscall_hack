#include <syscall.h>
#include <stdio.h>
#include <unistd.h>
 
int main(void)
{
    unsigned long ret = syscall(__NR_perf_event_open, NULL, 0, 0, 0, 0);
    printf("%d\n", (int)ret);
    return 0;  //0: hack successful;  1: hack failed
}
