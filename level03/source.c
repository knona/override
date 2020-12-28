#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

void decrypt(int random)
{
}

void test(int a, int b)
{
	int diff = b - a; // ebp - 0xc

	if (diff == 0x15) // unsigned comparison
	{
		decrypt(rand());
		return;
	}
	decrypt(diff);
	// jmp to : diff << 2 + 0x80489f0;
}

int main(int argc, const char **argv)
{
	int a; // esp+0x1c
	srand(time(NULL));

	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &a);
	test(a, 0x1337d00d);
	return (0);
}
