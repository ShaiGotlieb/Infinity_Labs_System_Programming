#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */

#include "fsma.h"

#define OFFSET (sizeof(size_t))

struct allocator_s
{
	size_t next_free;
	size_t used;
	size_t unused;
	size_t block_size;
	char *base;
};

allocator_t *FSMAInit(void *memory_ptr, size_t capacity, size_t block_size)
{
	size_t i;
	allocator_t *allocator = (allocator_t *)memory_ptr;

	assert (NULL != memory_ptr);

	allocator->next_free = 0;
	allocator->used = 0;
	allocator->unused = capacity;
	allocator->block_size = block_size;
	allocator->base = (char *)memory_ptr + sizeof(allocator_t);

	for (i = 1; i < capacity; ++i) 
	{
		*(allocator->base + i * ((block_size) + OFFSET) - OFFSET) = i;
	}

	return (allocator);
}

size_t FSMAGetSizeToAlloc(size_t capacity, size_t block_size)
{
	return ((capacity * (block_size + OFFSET)) + sizeof(allocator_t));
}

void SwapIndexs(allocator_t *allocator, void *block)
{
	size_t next_free_tmp = allocator->next_free;
	allocator->next_free = *((char*)block + allocator->block_size);
	*((char*)block + allocator->block_size) = next_free_tmp;
}

void *FSMAGetBlockFromPool(allocator_t *allocator)
{
	void *next_free_memory;
	assert(NULL != allocator);

	next_free_memory = (allocator->base + allocator->next_free * (allocator->block_size + OFFSET));

	SwapIndexs(allocator, next_free_memory);
	allocator->used++;
	allocator->unused--;

	return (next_free_memory);
}

void FSMAReturnBlockToPool(allocator_t *allocator, void *block_ptr)
{
	SwapIndexs(allocator, block_ptr);
	allocator->used--;
	allocator->unused++;
}

size_t FSMAGetPoolSize(allocator_t *allocator)
{
	return (allocator->unused);
}

size_t FSMAGetCapacity(allocator_t *allocator)
{
	return (allocator->used + allocator->unused);
}


