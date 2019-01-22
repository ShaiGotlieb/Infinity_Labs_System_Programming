#ifndef __H_VSMA__
#define __H_VSMA__

/***********************************************************************
 * This file contains the API of Variable Size Memory Allocator (VSMA)
 *
 *                                                				
 * Written by: 		OL62    								
 * Last updated : 	20.1.2019 
 ***********************************************************************/

typedef struct variable_allocator_s variable_allocator_t;

variable_allocator_t *VSMAInit(void *memory_ptr, size_t size);
/********************************************************************
 * VSMAInit - Creates a Variable Size Memory Allocator. 
 * If the size is smaller than the overhead bytes used by the allocateor, 
 * NULL will be returned.
 *
 * memory_ptr - a pointer to the memory allocated by the user. not NULL.
  * 
 * size - the size, in bytes, of the allocated memory provided
 * by the user.
 * 
 * Return - a pointer to variable_allocator_t.
 ********************************************************************/

void *VSMAAlloc(variable_allocator_t *allocator, size_t size);
/********************************************************************
 * VSMAGetBlock - returns a pointer to a free block with a size of 'size' bytes.
 *
 * allocator - a pointer to an allocator. NOT NULL.
 *
 * Return - a pointer to the free block if available. 
 * Otherwise, NULL.
 ********************************************************************/

void VSMAFree(void *block_ptr);
/********************************************************************
 * VSMAFree - receives a pointer to a memory block to be used again by the allocator.
 *
 * block_ptr - a pointer to the block to be returned to the allocator.
 ********************************************************************/

size_t VSMAGetMaxFreeBlockSize(variable_allocator_t *allocator);
/********************************************************************
 * VSMAGetMaxFreeBlockSize - returns the largest free block that can be returned 
 * by the allocator, when using VSMAAlloc().
 *
 * allocator - a pointer to an allocator. NOT NULL.
 ********************************************************************/

#endif