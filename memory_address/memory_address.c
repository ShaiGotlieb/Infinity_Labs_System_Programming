#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int bss_address;
extern int etext, edata, end;

int main()
{
  char command_stack[100];
  char command_heap[100];

  sprintf(command_stack, "sudo cat /proc/%d/maps | grep stack | awk '{print $1}'", getpid());
  sprintf(command_heap, "sudo cat /proc/%d/maps | grep heap | awk '{print $1}'", getpid());

  printf("Code segment address:\n%p-%p \n", &main ,&etext);
  printf("Data segment address:\n%p-%p \n", &edata ,(&end-1));
  printf("BSS segment address:\n%p-%p \n", &bss_address, &end);

  printf("Stack segment address: \n");
  system(command_stack);
  printf("Heap segment address: \n");
  system(command_heap);
  
  return (EXIT_SUCCESS);
}

