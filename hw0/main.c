#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    struct rlimit lim;
    printf("stack size: %d\n",  0);
    printf("process limit: %d\n", 0);
    printf("max file descriptors: %d\n", 0);
}
