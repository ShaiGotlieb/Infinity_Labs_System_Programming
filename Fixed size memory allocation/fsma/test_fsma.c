#include <stdlib.h> /* size_t */
#include "fsma.h"
#include "test.h"

#define EMPTY (0)

TestResult TestFSMAInit();
TestResult TestFSMAGetSizeToAlloc();
TestResult TestFSMAGetBlockFromPool();
TestResult TestFSMAReturnBlockToPool();
TestResult TestFSMAGetPoolSize();
TestResult TestFSMAGetCapacity();

struct allocator_s
{
	size_t next_free;
	size_t used;
	size_t unused;
	char *base;
};

int main()
{
	RUN_TEST(TestFSMAInit);
	RUN_TEST(TestFSMAGetSizeToAlloc);
	RUN_TEST(TestFSMAGetBlockFromPool);
	RUN_TEST(TestFSMAGetPoolSize);
	RUN_TEST(TestFSMAReturnBlockToPool);
	RUN_TEST(TestFSMAGetCapacity);

	return (EXIT_SUCCESS);
}

TestResult TestFSMAInit()
{
	void *memory = malloc(FSMAGetSizeToAlloc(5,8));
	memory = FSMAInit(memory, 5, 8);

	REQUIRE(NULL != memory);

	free(memory);
	memory = NULL;

	return (TEST_PASS);
}

TestResult TestFSMAGetSizeToAlloc()
{
	size_t allocation_size = FSMAGetSizeToAlloc(5, 8);
	size_t size_needed = 5 * 16 + 32;

	REQUIRE(size_needed != allocation_size);

	return (TEST_PASS);
}

TestResult TestFSMAGetBlockFromPool()
{
	void *memory = malloc(FSMAGetSizeToAlloc(5,8));
	memory = FSMAInit(memory, 5, 8);
	
	memory = FSMAInit(memory, 5, 8);
	REQUIRE(FSMAGetPoolSize(memory) == 5);
	FSMAGetBlockFromPool(memory);
	REQUIRE(FSMAGetPoolSize(memory) == 4);
	FSMAGetBlockFromPool(memory);
	FSMAGetBlockFromPool(memory);
	FSMAGetBlockFromPool(memory);
	FSMAGetBlockFromPool(memory);
	REQUIRE(FSMAGetPoolSize(memory) == EMPTY); /* do loop */


	free(memory);
	memory = NULL;

	return (TEST_PASS);
}

TestResult TestFSMAReturnBlockToPool()
{
	void *block;
	void *memory = malloc(FSMAGetSizeToAlloc(5,8));
	memory = FSMAInit(memory, 5, 8);
	
	memory = FSMAInit(memory, 5, 8);
	REQUIRE(FSMAGetPoolSize(memory) == 5);

	block = FSMAGetBlockFromPool(memory);
	FSMAReturnBlockToPool(memory, block);

	REQUIRE(FSMAGetCapacity(memory) == 5);

	free(memory);
	memory = NULL;

	return (TEST_PASS);
}

TestResult TestFSMAGetPoolSize()
{
	size_t pool_size;
	void *memory1 = malloc(FSMAGetSizeToAlloc(5,8));
	void *memory2 = malloc(FSMAGetSizeToAlloc(5,8));

	memory1 = FSMAInit(memory1, 5, 8);
	memory2 = FSMAInit(memory2, 0, 8);

	pool_size = FSMAGetPoolSize(memory1);
	REQUIRE(pool_size == 5);
	
	pool_size = FSMAGetPoolSize(memory2);
	REQUIRE(pool_size == EMPTY);

	free(memory1);
	free(memory2);
	memory1 = NULL;
	memory2 = NULL;

	return (TEST_PASS);
}

TestResult TestFSMAGetCapacity()
{
	size_t capacity_size;
	void *memory1 = malloc(FSMAGetSizeToAlloc(5,8));
	void *memory2 = malloc(FSMAGetSizeToAlloc(5,8));

	memory1 = FSMAInit(memory1, 5, 8);
	memory2 = FSMAInit(memory2, 0, 8);

	capacity_size = FSMAGetCapacity(memory1);
	REQUIRE(capacity_size == 5);

	capacity_size = FSMAGetCapacity(memory2);
	REQUIRE(capacity_size == EMPTY);

	free(memory1);
	free(memory2);
	memory1 = NULL;
	memory2 = NULL;

	return (TEST_PASS);
}


