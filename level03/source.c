#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

void decrypt(int nb)
{
	[...]
	if ([...])
		system("/bin/sh");
	else
		puts("\nInvalid Password");
}

void test(int a, int b)
{
	int diff = b - a; // ebp - 0xc

	if (diff > 21)
	{
		decrypt(rand());
		return;
	}
	else // jump to adress pointed by : diff << 2 + 0x80489f0
	{
		if (diff == 0 || (diff >= 10 && diff <= 15))
			decrypt(rand());
		else
			decrypt(diff);
	}
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
	test(a, 322424845);
	return (0);
}
