#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

typedef unsigned int uint;

int read_number(char *data)
{
}

int store_number(char *data)
{
}

int main(int argc, const char **argv, char **envp)
{
	uint x;			  // esp+0x1b4
	char command[20]; // esp+0x1b8 - esp+0x1c8
	uint a;			  // exp+0x1cc
	char *data[400];  // esp+0x24
	char **av;		  // esp+0x1c
	char **env;		  // esp+0x18

	av = argv;
	env = envp;
	a = 20;
	x = 0;
	bzero(command, 20);
	bzero(data, 400);

	while (av[0] != NULL)
	{
		memset(av[0], 0, strlen(av[0]));
		av += 4;
	}

	while (env[0] != NULL)
	{
		memset(env[0], 0, strlen(env[0]));
		env += 4;
	}
	puts("----------------------------------------------------");
	puts("   Welcome to wil's crappy number storage service!  ");
	puts("----------------------------------------------------");
	puts("Commands:");
	puts("    store - store a number into the data storage");
	puts("    read  - read a number from the data storage");
	puts("    quit  - exit the program");
	puts("----------------------------------------------------");
	puts("   wil has reserved some storage :");
	puts("----------------------------------------------------");

	while (1)
	{
		printf("Input command: ");
		x = 1;
		fgets(command[1], 20, stdin);

		command[strlen(command) - 1] = '\0';

		if (!strncmp(command, "store", 5))
			x = store_number(data);
		else if (!strncmp(command, "read", 4))
			x = read_number(data);
		else if (!strncmp(command, "quit", 4))
			return 0;

		if (x)
			printf(" Failed to do %s command\n", command);
		else
			printf(" Completed %s command successfully\n", command);

		bzero(command, 20);
	}
}
