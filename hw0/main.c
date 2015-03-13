#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    struct rlimit lim;
	getrlimit(RLIMIT_STACK,&lim);
    printf("stack size: %d\n", (int) lim.rlim_cur);
	getrlimit(RLIMIT_NPROC,&lim);
    printf("process limit: %d\n",(int)lim.rlim_cur);
	getrlimit(RLIMIT_NOFILE,&lim);
    printf("max file descriptors: %d\n", (int)lim.rlim_cur);
}
