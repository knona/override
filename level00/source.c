#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, const char **argv)
{
	int a; // esp + 0x1c

	puts("***********************************");
	puts("* \t     -Level00 -\t\t  *");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &a);
	if (a != 5276)
	{
		puts("\nInvalid Password!");
		return 1;
	}
	puts("\nAuthenticated!");
	system("/bin/sh");
	return 0;
}
