/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
/* Your final implementation should comment out this macro. */
#define MM_USE_STUBS

s_block_ptr first = {0,NULL,NULL,1,NULL};
s_block_ptr last = {0,NULL,NULL,1,NULL};

s_block_ptr extend_heap (s_block_ptr last,size_t s){
void *brk_ptr = sbrk(s+sizeof(s_block))
	if(brk_ptr == (void*)-1){
		perror("extend_heap sbrk: ");
		return NULL;
	}
	if(last->ptr == NULL){
		set_block_content(last,s,NULL,NULL,0,brk_ptr);
	}
	else{
		set_block_content(last,s,NULL,(s_block_ptr)last->ptr,0,brk_ptr);
	}
	if(first->ptr == NULL){
		set_block_content(first,s,NULL,NULL,0,brk_ptr);
	}
	else if(first->next == NULL){
		set_block_content(first,s,(s_block_ptr)brk_ptr,NULL,0,first->ptr);
	}
	return (brk_ptr;
}

void* mm_malloc(size_t size)
{
#ifdef MM_USE_STUBS
	struct rlimit rlim;
	getrlimit(RLIMIT_DATA,&rlim);
	printf("max: %u\n",rlim.rlim_max);
	printf("cur: %u\n",rlim.rlim_cur);
  return calloc(1, size);
#else
#endif
}

void* mm_realloc(void* ptr, size_t size)
{
#ifdef MM_USE_STUBS
    return realloc(ptr, size);
#else
#error Not implemented.
#endif
}

void mm_free(void* ptr)
{
#ifdef MM_USE_STUBS
    free(ptr);
#else
#error Not implemented.
#endif
}
