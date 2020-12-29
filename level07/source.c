#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

typedef unsigned int uint;

void clear_stdin(void)
{
	int c; // ebp-0x9

	c = 0;
	while (c != 0xff)
	{
		c = getchar();
		if (c == '\n')
			return;
	}
}

int get_unum(void)
{
	uint nb; // ebp-0xc

	nb = 0;
	fflush(stdout);
	scanf("%u", &nb);
	clear_stdin();
	return nb;
}

int read_number(char *data)
{
	uint index; // ebp-0xc

	printf(" Index: ");
	index = get_unum();
	printf(" Number at data[%u] is %u\n", index, data[index << 2]); // index << 2 ?
	return 0;
}

int store_number(char *data)
{
	uint nb;	// ebp-0x10
	uint index; // ebp-0xc

	nb = 0;
	index = 0;
	printf(" Number: ");
	nb = get_unum();
	printf(" Index: ");
	index = get_unum();

	int tmp = index * 0xaaaaaaab;
	tmp <<= 1;
	tmp *= 3;

	if (index - tmp == 0 || nb >> 24 != 183)
	{
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return 1;
	}
	data[index << 2] = nb;
	return 0;
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
