/* A simple test harness for memory alloction. */

#include "mm_alloc.h"
#include <stdio.h>
int main(int argc, char **argv)
{
    int *data;

    data = (int*) mm_malloc(4);
    data[0] = 1;
    mm_free(data);
    printf("malloc sanity test successful!\n");
    return 0;
}
