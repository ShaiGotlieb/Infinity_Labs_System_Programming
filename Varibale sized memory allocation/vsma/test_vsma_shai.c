#include <stdlib.h> /* size_t */

#include "vsma.h"
#include "test.h"

#define EMPTY (0)
#define MAX_BLOCK_SIZE (2000)
#define RANDOM (rand() % 25)

TestResult TestVSMAInit();
TestResult TestVSMAAlloc();
TestResult TestVSMAFree();
TestResult TestVSMAGetMaxFreeBlockSize();

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

	free(allocator);
	allocator = NULL;

	memory = malloc(EMPTY);
	allocator = VSMAInit(memory, EMPTY);
	REQUIRE(NULL == allocator);

	free(allocator);
	allocator = NULL;

	memory = malloc(random_size);
	allocator = VSMAInit(memory, random_size);
	REQUIRE(NULL == allocator);

	free(allocator);

	return (TEST_PASS);
}

TestResult TestVSMAAlloc()
{
	void *memory_block1 = NULL;
	void *memory_block2 = NULL;
	void *memory_block3 = NULL;
	void *memory = malloc(MAX_BLOCK_SIZE);
	variable_allocator_t *allocator = memory;
	
	allocator = VSMAInit(memory, MAX_BLOCK_SIZE);
	
	memory_block1 = VSMAAlloc(allocator, 4);
	*(int *)memory_block1 = 10;
	REQUIRE(*(int *)memory_block1 == 10);
	
	memory_block2 = VSMAAlloc(allocator, 4);
	*(int *)memory_block2 = 30;
	REQUIRE(*(int *)memory_block2 == 30);
	
	memory_block3 = VSMAAlloc(allocator, MAX_BLOCK_SIZE);
	REQUIRE(memory_block3 == NULL);
	
	free(allocator);

	return (TEST_PASS);

}

TestResult TestVSMAFree()
{
	void *memory_block1 = NULL;
	void *memory_block2 = NULL;
	void *memory_block3 = NULL;
	void *memory = malloc(MAX_BLOCK_SIZE);
	variable_allocator_t *allocator = memory;
	
	allocator = VSMAInit(memory, MAX_BLOCK_SIZE);
	
	memory_block1 = VSMAAlloc(allocator, MAX_BLOCK_SIZE - 100);
	memory_block2 = VSMAAlloc(allocator, 100);
	REQUIRE(NULL == memory_block2);

	VSMAFree(memory_block1);
	memory_block2 = VSMAAlloc(allocator, 100);
	REQUIRE(NULL != memory_block2);

	VSMAFree(memory_block2);
	memory_block3 = VSMAAlloc(allocator, 200);
	REQUIRE(memory_block3 != NULL);
	
	free(allocator);

	return (TEST_PASS);
}

TestResult TestVSMAGetMaxFreeBlockSize()
{
	void *memory_block1 = NULL;
	void *memory_block2 = NULL;
	void *memory_block3 = NULL;
	size_t max_before_changes = 0;
	size_t max_after_changes = 0;
	void *memory = malloc(MAX_BLOCK_SIZE);
	variable_allocator_t *allocator = memory;
	
	allocator = VSMAInit(memory, MAX_BLOCK_SIZE);

	max_before_changes = VSMAGetMaxFreeBlockSize(allocator);
	
	memory_block1 = VSMAAlloc(allocator, 100);
	memory_block2 = VSMAAlloc(allocator, 100);
	memory_block3 = VSMAAlloc(allocator, 100);

	max_after_changes = VSMAGetMaxFreeBlockSize(allocator);

	REQUIRE(max_before_changes > max_after_changes);

	VSMAFree(memory_block1);
	VSMAFree(memory_block2);
	VSMAFree(memory_block3);

	max_after_changes = VSMAGetMaxFreeBlockSize(allocator);

	REQUIRE(max_before_changes == max_after_changes);
	
	free(allocator);

	return (TEST_PASS);
}