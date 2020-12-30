#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

typedef unsigned int uint;

void secret_backdoor()
{
	char *buffer;

	fgets(buffer, 0x80, 0);
	system(buffer);
}

void set_message(
	char *data // rbp-0x408
)
{
	char buffer[1024]; // rbp-0x400

	bzero(buffer, 1024);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(buffer, 1024, 0);
	strncpy(data, buffer, data[180]);
}

void set_username(
	char *data // rbp-0x98
)
{
	char buffer[128]; // rbp-0x90
	uint i;			  // rbp-0x4

	bzero(buffer, 128);
	puts(">: Enter your data");
	printf(">>: ");
	fgets(buffer, 128, 0);
	i = 0;
	while (i <= 40 && buffer[i])
	{
		(data + 140)[i] = buffer[i];
		i++;
	}
	printf(">: Welcome, %s", data + 140);
}

void handle_msg(void)
{
	char *data; // rbp-0xc0

	bzero(data + 140, 40);
	data[180] = 140;
	set_username(data);
	set_message(data);
	puts(">: Msg sent!");
}

int main(int argc, const char **argv)
{
	puts("--------------------------------------------");
	puts("|   ~Welcome to l33t-m$n ~    v1337        |");
	puts("--------------------------------------------");
	handle_msg();
}
