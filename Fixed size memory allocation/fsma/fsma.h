#ifndef __H_FSMA__
#define __H_FSMA__
#include <stdlib.h> /* size_t */
/***********************************************************************
 * This file contains the API of Allocator
 *
 *                                                				
 * Written by: 		OL62    								
 * Last updated : 	17.1.2019 
 ***********************************************************************/

typedef struct allocator_s allocator_t;

allocator_t *FSMAInit(void *memory_ptr,
				size_t capacity, size_t block_size);
/********************************************************************
 * FSMACreate - Creates an allocator.
 *
 * memory_ptr - a pointer to allocated memory
 * by the user according to FSMASizeToAlloc().
 * 
 * capacity - the numbers of memory blocks in the pool.
 * 
 * block_size - the size of one memory block in the pool, in bytes.
 * 
 * Return - a pointer to allocator_t if successful, NULL otherwise.
 ********************************************************************/

size_t FSMAGetSizeToAlloc(size_t capacity, size_t block_size);
/********************************************************************
 * FSMASizeToAlloc - Calculates the amount of memory, in bytes,
 * needed to allocate in order to create the allocator.
 * Function needs to be used prior to FSMACreate.
 * 
 * capacity - the number of memory blocks in the pool.
 *
 * block_size - the size of one memory block in the pool, in bytes.
 *
 * Return - the number of bytes to allocate in order to
 * create an allocator via FSMACreate().
 ********************************************************************/

void *FSMAGetBlockFromPool(allocator_t *allocator);
/********************************************************************
 * FSMAGetBlock - returns a pointer to a block not in use.
 *
 * allocator - a pointer to an allocator. NOT NULL.
 *
 * Return - a pointer to the free block.
 ********************************************************************/

void FSMAReturnBlockToPool(allocator_t *allocator, void *block_ptr);
/********************************************************************
 * FSMAReturnBlock - receives a block to be used again by the
 * allocator.
 *
 * allocator - a pointer to an allocator. NOT NULL.
 *
 * block_ptr - a pointer to the block to be returned to the pool.
 ********************************************************************/

size_t FSMAGetPoolSize(allocator_t *allocator);
/********************************************************************
 * FSMAGetPoolSize - returns the number of available blocks that
 * can be used in the allocator.
 *
 * allocator - a pointer to an allocator. NOT NULL.
 ********************************************************************/

size_t FSMAGetCapacity(allocator_t *allocator);
/********************************************************************
 * FSMAGetCapacity - returns the capacity that can be
 * used in the allocator - number of blocks.
 *
 * allocator - a pointer to an allocator. NOT NULL.
 *
 * Return - the number of blocks in allocator.
 ********************************************************************/

#endif