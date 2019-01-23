#include <stdlib.h> /* size_t */

#include "vsma.h"
#include "test.h"

#define EMPTY (0)
#define MAX_BLOCK_SIZE (2000)
#define RANDOM (rand() % 25)
#define INT_SIZE (sizeof(int))
#define DOUBLE_SIZE (sizeof(double))
#define STRUCT_SIZE (INT_SIZE * 2 + DOUBLE_SIZE * 2)

TestResult TestVSMAInit();
TestResult TestVSMAAlloc();
TestResult TestVSMAFree();
TestResult TestVSMAGetMaxFreeBlockSize();

void *memory_blocks[10];

int main()
{
	RUN_TEST(TestVSMAInit);
	RUN_TEST(TestVSMAAlloc);
	RUN_TEST(TestVSMAFree);
	RUN_TEST(TestVSMAGetMaxFreeBlockSize);

	return (EXIT_SUCCESS);
}

TestResult TestVSMAInit()
{
	void *memory = malloc(MAX_BLOCK_SIZE);
	variable_allocator_t *allocator = VSMAInit(memory, MAX_BLOCK_SIZE);
	int random_size = RANDOM;

	REQUIRE(NULL != allocator);

	free(memory);
	allocator = NULL;

	memory = malloc(EMPTY);
	allocator = VSMAInit(memory, EMPTY);
	REQUIRE(NULL == allocator);

	free(memory);
	allocator = NULL;

	memory = malloc(random_size);
	allocator = VSMAInit(memory, random_size);
	REQUIRE(NULL == allocator);

	free(memory);
	allocator = NULL;

	return (TEST_PASS);
}

TestResult TestVSMAAlloc()
{
	
	void *memory = malloc(MAX_BLOCK_SIZE);
	variable_allocator_t *allocator = memory;
	int block_value = 10;
	
	allocator = VSMAInit(memory, MAX_BLOCK_SIZE);
	
	memory_blocks[0] = VSMAAlloc(allocator, INT_SIZE);
	*(int *)memory_blocks[0] = block_value;
	REQUIRE(NULL != memory_blocks[0]);
	REQUIRE(*(int *)memory_blocks[0] == block_value);

	memory_blocks[1] = VSMAAlloc(allocator, DOUBLE_SIZE);
	*(double *)memory_blocks[1] = block_value;
	REQUIRE(NULL != memory_blocks[1]);
	REQUIRE(*(double *)memory_blocks[1] == block_value);
	
	block_value = RANDOM;
	memory_blocks[2] = VSMAAlloc(allocator, block_value);
	*(int *)memory_blocks[2] = block_value;
	REQUIRE(NULL != memory_blocks[2]);
	REQUIRE(*(int *)memory_blocks[2] == block_value);
	
	memory_blocks[3] = VSMAAlloc(allocator, MAX_BLOCK_SIZE);
	REQUIRE(memory_blocks[3] == NULL);

	free(memory);
	allocator = NULL;

	memory = malloc(MAX_BLOCK_SIZE);
	block_value = MAX_BLOCK_SIZE / 4;
	allocator = VSMAInit(memory, MAX_BLOCK_SIZE);

	memory_blocks[4] = VSMAAlloc(allocator, block_value);
	REQUIRE(NULL != memory_blocks[4]);

	memory_blocks[5 ]= VSMAAlloc(allocator, block_value);
	REQUIRE(NULL != memory_blocks[5]);
	
	memory_blocks[6] = VSMAAlloc(allocator, block_value);
	REQUIRE(NULL != memory_blocks[6]);

	memory_blocks[7] = VSMAAlloc(allocator, block_value);
	REQUIRE(NULL == memory_blocks[7]);
	
	free(memory);
	allocator = NULL;

	return (TEST_PASS);
}

TestResult TestVSMAFree()
{
	void *memory = malloc(MAX_BLOCK_SIZE);
	variable_allocator_t *allocator = memory;
	int complement_size = 100;
	
	allocator = VSMAInit(memory, MAX_BLOCK_SIZE);
	
	memory_blocks[0] = VSMAAlloc(allocator, MAX_BLOCK_SIZE - complement_size);
	memory_blocks[1] = VSMAAlloc(allocator, complement_size);
	
	REQUIRE(NULL == memory_blocks[1]);
	VSMAFree(memory_blocks[0]);
	memory_blocks[1] = VSMAAlloc(allocator, complement_size);
	
	REQUIRE(NULL != memory_blocks[1]);
	VSMAFree(memory_blocks[1]);
	memory_blocks[2] = VSMAAlloc(allocator, RANDOM);
	
	REQUIRE(memory_blocks[2] != NULL);
	VSMAFree(memory_blocks[2]);

	memory_blocks[0] = VSMAAlloc(allocator, complement_size);
	memory_blocks[1] = VSMAAlloc(allocator, complement_size);
	memory_blocks[2] = VSMAAlloc(allocator, complement_size);

	VSMAFree(memory_blocks[1]);
	memory_blocks[1] = VSMAAlloc(allocator, complement_size * 2);
	REQUIRE(memory_blocks[1] != NULL);
	memory_blocks[3] = VSMAAlloc(allocator, complement_size);
	REQUIRE(memory_blocks[3] != NULL);
	
	free(allocator);
	allocator = NULL;

	return (TEST_PASS);
}

TestResult TestVSMAGetMaxFreeBlockSize()
{
	size_t max_before_changes = 0;
	size_t max_after_changes = 0;
	void *memory = malloc(MAX_BLOCK_SIZE);
	variable_allocator_t *allocator = memory;
	
	allocator = VSMAInit(memory, MAX_BLOCK_SIZE);

	max_before_changes = VSMAGetMaxFreeBlockSize(allocator);
	
	memory_blocks[0] = VSMAAlloc(allocator, RANDOM);
	memory_blocks[1] = VSMAAlloc(allocator, RANDOM);
	memory_blocks[2] = VSMAAlloc(allocator, RANDOM);

	max_after_changes = VSMAGetMaxFreeBlockSize(allocator);

	REQUIRE(max_before_changes > max_after_changes);

	VSMAFree(memory_blocks[0]);
	VSMAFree(memory_blocks[1]);
	VSMAFree(memory_blocks[2]);

	max_after_changes = VSMAGetMaxFreeBlockSize(allocator);

	REQUIRE(max_before_changes == max_after_changes);
	
	free(allocator);
	allocator = NULL;

	return (TEST_PASS);
}