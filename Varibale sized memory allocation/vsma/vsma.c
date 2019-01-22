#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */

#include "vsma.h"

#define MAGIC_NUMBER (0xdeadbeef)
#define ALLOCATOR_SIZE (sizeof(variable_allocator_t))
#define HEADER_SIZE (sizeof(block_header_t))
#define START_HEADER (block_header_t *)((char *)(allocator) + ALLOCATOR_SIZE)
#define TRUE (1)
#define FALSE (0)

struct variable_allocator_s
{
	size_t size;
};

typedef struct block_header 
{
	long signed int block_size;
	#ifndef NDEBUG
		long magic_number;
	#endif

}block_header_t;


static block_header_t *CreateNewBlockHeader(block_header_t *current_header, size_t size, size_t original_block_size)
{
	block_header_t *new_block_header = (block_header_t *)((char*)(current_header + 1) + size);
	

	new_block_header->block_size = original_block_size - (HEADER_SIZE + size);

	#ifndef NDBUG
		new_block_header->magic_number = MAGIC_NUMBER;
	#endif

	return (new_block_header);
}

static int IsLastHeader(variable_allocator_t *allocator, block_header_t *header)
{
	return (((char*)allocator + allocator->size) == ((char *)(header + 1) + abs(header->block_size)));
}

static block_header_t *GetNextHeader(block_header_t *header)
{
	return ((block_header_t *)((char *)(header + 1) + abs(header->block_size)));
}

static block_header_t *FindAvaiableBlock(variable_allocator_t *allocator, block_header_t *header)
{
	while (0 > header->block_size && FALSE == IsLastHeader(allocator, header)) 
	{
		header = GetNextHeader(header);
	}

	return (header);
}

static void UpdateBlockHeader(block_header_t *block_header, size_t size)
{
	block_header->block_size = -size;
	
	#ifndef NDBUG
		block_header->magic_number = MAGIC_NUMBER;
	#endif	
}

variable_allocator_t *VSMAInit(void *memory_ptr, size_t size)
{
	variable_allocator_t *allocator;
	block_header_t *block_header;

	assert(NULL != memory_ptr);

	if (size < (ALLOCATOR_SIZE + HEADER_SIZE))
	{
		return (NULL);
	}

	allocator = memory_ptr;
	allocator->size = size;

	block_header = (block_header_t *)((char*)memory_ptr + ALLOCATOR_SIZE);
	block_header->block_size = size - (ALLOCATOR_SIZE + HEADER_SIZE);

	#ifndef NDBUG
		block_header->magic_number = MAGIC_NUMBER;
	#endif

	return (allocator);
}

void *Defragmentation(variable_allocator_t *allocator, block_header_t *current_header, size_t size)
{
	block_header_t *next_header = NULL;

	for (next_header = current_header; current_header->block_size < (long int)size && FALSE == IsLastHeader(allocator, current_header); current_header = next_header)
	{
		current_header = FindAvaiableBlock(allocator, current_header);

		if ((long int)size <= current_header->block_size)
		{
			return (current_header);
		}

		if (TRUE == IsLastHeader(allocator, current_header))
		{
			return (NULL);
		}

		next_header = GetNextHeader(current_header);

		while (0 < next_header->block_size)
		{
			current_header->block_size += (next_header->block_size + HEADER_SIZE); 
			
			if ((long int)size <= current_header->block_size)
			{
				return (current_header);
			}
			else if (TRUE == IsLastHeader(allocator, current_header))
			{
				break; 
			}

			next_header = GetNextHeader(current_header);
		}
	}

	return (NULL);
}

size_t VSMAGetMaxFreeBlockSize(variable_allocator_t *allocator)
{
	long int max = 0; 
	block_header_t *block_header = (block_header_t *)(allocator + 1); 

	assert(NULL != allocator);

	Defragmentation(allocator, block_header, allocator->size); 
	
	while (FALSE == IsLastHeader(allocator, block_header))
	{
		if (block_header->block_size > max)
		{
			max = block_header->block_size;
		}

		block_header = GetNextHeader(block_header);
	}

	if (block_header->block_size > max)
	{
		max = block_header->block_size;
	}

	return (max);
}

void VSMAFree(void *block_ptr)
{
	block_header_t* block_to_free = (block_header_t*)block_ptr - 1;

	assert(block_to_free->magic_number == MAGIC_NUMBER);

	UpdateBlockHeader(block_to_free, block_to_free->block_size);
}

void *VSMAAlloc(variable_allocator_t *allocator, size_t size)
{
	block_header_t *block_header = NULL;

	size_t original_block_size = 0;

	assert(NULL != allocator);

	block_header = START_HEADER;

	if (0 == size)
	{
		return (NULL);
	}

	block_header = FindAvaiableBlock(allocator, block_header);

	if ((long int)size > block_header->block_size)
	{
		block_header = Defragmentation(allocator, block_header, size);
		if (NULL == block_header)
		{
			return (NULL);
		}
	}

	original_block_size = block_header->block_size;

	if (original_block_size - size > HEADER_SIZE)
	{
		CreateNewBlockHeader(block_header, size, original_block_size);
	}
	else
	{
		size = original_block_size;
	}

	UpdateBlockHeader(block_header, size);

	return (block_header + 1);
}