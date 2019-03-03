#include <ctype.h> /* isspace */
#include <errno.h>  /* exit s*/
#include <stdio.h> /* printf, gets */
#include <stdlib.h> /* system */ 
#include <string.h> /* strcmp */
#include <sys/types.h> /* pid_t */
#include <sys/utsname.h> /* uname */
#include <sys/wait.h> /* wait*/
#include <unistd.h>  /* fork, execvp */

#include "shell.h"

#define MAX_ARGV_SIZE (64)
#define MAX_INPUT_SIZE (1024)
#define GREEN  ("\x1B[32m")
#define RED  ("\x1B[31m")
#define WHITE  ("\x1B[37m")

static void ParseCommand(char *command, char **argv);
static void ExecuteCommand(char **argv);

void ShellRun()
{
	char *argv[MAX_ARGV_SIZE];
	char input[MAX_INPUT_SIZE];
	struct utsname unameData;

	uname(&unameData);

   	while (1)
   	{
	   	printf(GREEN);
		printf("%s@%s:\n", unameData.nodename, unameData.sysname);
	   	
	   	printf(WHITE);
	   	fscanf(stdin, "%[^\n]%*c", input);
	   	
	   	ParseCommand(input, argv);

	   	if (0 == strcmp(argv[0], "exit"))
	   	{
	   		exit(0);            
	   	}

	   	ExecuteCommand(argv);
   	}

   	return;
}

static void ParseCommand(char *command, char **argv)
{
	while ('\0' != *command) 
	{      
		while (isspace(*command))
		{
			*command++ = '\0';    
		}

		*argv++ = command;         
		while ('\0' != *command && !isspace(*command)) 
		{
			command++;            
		}
	}

	*argv = '\0';
}

static void ExecuteCommand(char **argv)
{
	int status;
	pid_t pid = fork();

	if (0 == pid)
	{
		if (execvp(*argv, argv) < 0)
		{
			printf(RED);
			printf("\n%s: command not found\n",argv[0]);

			exit(0);	
		} 
	}
	else if (pid > 0)
	{
		wait(&status);
	}
	else
	{
		printf(RED);
		printf("\nERROR: process failure!\n");

		exit(0);
	}
}