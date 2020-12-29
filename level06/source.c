#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/ptrace.h>

typedef unsigned int uint;

int auth(char *login, uint serial)
{
	uint len; // ebp-0xc
	uint res; // ebp-0x10
	uint i;	  // ebp-0x14

	login[strcspn(login, "\n")] = 0;

	len = strnlen(login, 0x20);

	if (len <= 5)
		return 1;

	if (ptrace(PTRACE_TRACEME, 0, 1, 0) == 0xffffffff)
	{
		puts("\033[32m.---------------------------.");
		puts("\033[31m| !! TAMPERING DETECTED !!  |");
		puts("\033[32m'---------------------------'");
		return 1;
	}

	res = (login[3] ^ 0x1337) + 0x5eeded;
	i = 0;
	while (i < len)
	{
		if (login[i] <= 0x1f)
			return 1;

		int c = login[i] ^ res;
		int nb = 0x88233b2b * c;
		int nb2 = c - nb;

		nb2 >> 1;
		nb2 += nb;
		nb2 >> 10;
		nb2 *= 0x539;
		res = res + c - nb2;
		i++;
	}

	if (res == serial)
		return 0;
	return 1;
}

int main(int argc, const char **argv)
{
	char *login; // esp+0x2c
	uint serial; // esp+0x28

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	printf("-> Enter Login: ");
	fgets(login, 0x20, stdin);

	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");
	printf("-> Enter Serial: ");

	scanf("%u", &serial);

	if (!auth(login, serial))
	{
		puts("Authenticated!");
		system("/bin/sh");
		return 0;
	}
	return 1;
}
