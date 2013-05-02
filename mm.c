/*
 * @author Vinit Pandey(201101090)
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    "Vinit Pandey",
	"201101090",
    "201101090@daiict.ac.in"
    
};
#define CHUNKSIZE (1<<12)
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0xF)    // size+7 > next nearest multiple... so anding with 1111111111000 gives the next nearest



#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))                //==16

#define FTRP(bp) ()

#define GET(bp) (*(size_t *)bp)

#define PUT(bp,val) (*(size_t *)bp =val)

#define HDRP(bp) ()

#define PACK(size,alloc) (size|alloc)

#define SIZEOF(bp) (*(size_t *)(bp) & ~0xF)

#define ALLOCOF(bp) (*(size_t *)(bp) & 0x1)

void *heap_p;
size_t *freelist_p = NULL;

void putp(char *bp){
	
	if(freelist_p == NULL)
	{
		(bp) = (char *)NULL;
		(char **)bp+8 = NULL;
		freelist_p = (size_t *)(bp + 8);
	}
	else{
		char *temp;
		temp =(char *)freelist_p;
		*(size_t *)(temp-8) = bp;
		*(size_t *)bp = (size_t)NULL;
		*(size_t *)(bp+8) = temp;

		freelist_p = (size_t *)(bp+8);
	
	
	}
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	heap_p = mem_sbrk(16);
	if(heap_p == (void *)NULL)
		return -1;
	
	PUT(heap_p,0);
	PUT(heap_p + 4, PACK(16,1));
	PUT(heap_p +12, PACK(16,1));
	PUT(heap_p +20, PACK(0,1));    //epilogue
	heap_p+=12;         // points after the prologue header

	if(extend_heap(CHUNKSIZE) == NULL)
		return -1;

    return 0;
}// init complete



/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
		size_t *temp = **freelist_p;
		while(*temp != NULL)
		{
			if(SIZEOF((char *)(temp)-16) >= newsize)
			{
				oldsize = SIZEOF((char *)(temp)-16);
				PUT((char *)(temp)-16, PACK(newsize,1));
				PUT((char *)(temp) + 8 + size, PACK(newsize , 1));         //footer
				*(size_t *)((char *)(temp)-8)+1 = *temp;           //ERROR PRONE
				*temp-1 = *(temp-1); // ERROR PRONE
				(size_t *)((char *)(temp)-8) = temp = NULL;// assign the pointers null
				PUT((char *)(temp + 16 + size),PACK(oldsize-newsize,0);   // make the new free pointer with new value
				PUT((char*)(temp + oldsize - 16 -8), PACK(oldsize - newsize,0);
				return (void *)(temp+8);

			}else{
				*temp = **temp; //error prone
			}
		}
		
		if(extend_heap(CHUNKSIZE) ==NULL)
			return -1;
		else{
			size_t oldsize = SIZEOF((char *)(temp)-16);
				PUT((char *)(temp)-16, PACK(newsize,1));
				PUT((char *)(temp) + 8 + size, PACK(newsize , 1));         //footer
				*(size_t *)((char *)(temp)-8)+1 = *temp;           //ERROR PRONE
				*temp-1 = *(temp-1); // ERROR PRONE
				(size_t *)((char *)(temp)-8) = temp = NULL;// assign the pointers null
				PUT((char *)(temp + 16 + size),PACK(oldsize-newsize,0);   // make the new free pointer with new value
				PUT((char*)(temp + oldsize - 16 -8), PAck(oldsize - newsize,0);
				return (void *)(temp+8);	
		
		
		}
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
	
	PUT((char *)ptr -24,PACK(SIZEOF((char *)ptr-24),0);
	putp((char *)ptr-16);
	coalesc((void *)((char *)ptr-24));
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
   if(ptr ==(void *)ptr)
   {
		newptr = mm_malloc(size);
    	if (newptr == NULL)
      		return NULL;
	}
	else if(size==0)
	{
		mm_free(ptr);
	}
	else{

    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
	newptr = mm_malloc(size);
	if(newptr == NULL)
		return NULL:
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
	}
}

static void *extend_heap(size_t size){
		
	size_t nsize= ALIGN(size);
	void *x = mem_sbrk(size);
	if(x == (void *)NULL)
		return NULL;
	PUT((char *)x-8, PACK(nsize,0)); // puts the free space in place of epilogue
	putp((char *)x);
	PUT((char *)x + nsize - 16, PACK(nsize,0));
	PUT((char *)x + nsize -8, PACK(0,1));
	
	return coalesc((void *)((char *)x-8));
	
	
	}

static void *coalesc(void *hp)
{
	size_t prev_alloc = (ALLOCOF((char *)(hp)-8));
	size_t next_alloc = ALLOCOF((char *)(hp)+SIZEOF((char *)(hp)));
	size_t size = SIZEOF((char *)hp);
	if(prev_alloc && next_alloc)
		return hp;
	else if(prev_alloc && !next_alloc) //previous is allocated ... do process on next block
	{
		size_t size_next = SIZEOF((char *)hp+ size);
		PUT((char *)hp,PACK(size_next+size,0);
		PUT((char *)hp+size + size_next-8,PACK(size_next+size,0);
		size_t *temp=(size_t *)hp;
		(temp+2) = *((size_t *)((char *)hp +size)+2); // pointer manipulation
		*((size_t *)((char *)hp +size)+2)-1 = temp-1;
		
		return hp;
		

	}else if(next_alloc && !prev_alloc)
	{
		size_t size_prev = SIZEOF((char *)hp-8);
		PUT((char *)hp- size_prev,PACK(size_prev + size,0);
		PUT((char *)hp+ + size-8, PACK(size_prev + size,0);

		size_t *temp = (size_t *)((char *)hp-size_prev +16);
		temp = *((size_t *)((char *)hp + 16));
		*(((size_t *)((char *)hp +16))-1) = temp -1;

		hp = (void *)((char *)hp - size_prev);
		return hp;

	}
	else if(!next_alloc && !prev_alloc)
	{
		size_t size_next = SIZEOF((char *)hp+ size);
		size_t size_prev = SIZEOF((char *)hp-8);
		PUT((char *)hp- size_prev,PACK(size_prev + size_next + size,0);
		PUT((char *)hp+size + size_next-8,PACK(size_next+size + size_prev,0);

		//TODO very imp

		hp = (void *)((char*)hp - size_prev;
		return hp;


	}


}













