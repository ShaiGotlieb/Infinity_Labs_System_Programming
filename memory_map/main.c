#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "libst.h"
#include "sh1.h"
#include "sh2.h"

int main()
{
	void *handle;
    void (*bar1)(void);
    char *error;

	foo();
	bar2();
	handle = dlopen("libsh1.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

	dlerror();  

	*(void **) (&bar1) = dlsym(handle, "bar1");

	if (bar1)
	{
		bar1();
	}
	
    dlclose(handle);

	return 0;
}