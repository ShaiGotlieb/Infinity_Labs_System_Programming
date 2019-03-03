#include <stdio.h>
#include "sh1.h"
#include "libst.h"

void bar1()
{
	printf("in bar1()\n");
	foo1();
}