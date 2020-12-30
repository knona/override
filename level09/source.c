#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

typedef unsigned int uint;

void set_message(
	char *message // rbp-0x408
)
{
	char buffer[1024]; // rbp-0x400

	bzero(buffer, 1024);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(buffer, 0x400, 0);
	strncpy(message, buffer, message[0xb4]);
}

void set_username(
	char *username // rbp-0x98
)
{
	char buffer[128]; // rbp-0x90
	uint i;			  // rbp-0x4

	bzero(buffer, 128);
	puts(">: Enter your username");
	printf(">>: ");
	fgets(buffer, 128, 0);
	i = 0;
	while (i <= 40 && buffer[i])
	{
		(username + 0x8c)[i] = buffer[i];
		i++;
	}
	printf(">: Welcome, %s", username + 0x8c);
}

void handle_msg(void)
{
	char *username; // rbp-0xc0

	bzero(username + 0x8c, 40);
	*(username + 0xb4) = 0x8c;
	set_username(username);
	set_message(username);
	puts(">: Msg sent!");
}

int main(int argc, const char **argv)
{
	puts("--------------------------------------------");
	puts("|   ~Welcome to l33t-m$n ~    v1337        |");
	puts("--------------------------------------------");
	handle_msg();
}
