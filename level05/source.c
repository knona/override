#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

typedef unsigned int uint;

int main(int argc, const char **argv)
{
	char buffer[100]; // esp+0x28
	int i;			  // esp+0x8c

	i = 0;
	fgets(buffer, 100, stdin);

	int i = 0;
	while (i < strlen(buffer)) // not really a call to strlen function
	{
		if (buffer[i] >= 'A' && buffer[i] <= 'Z')
			buffer[i] = buffer[i] - 'A' + 'a'; // buffer[i] ^ 0x20
		i++;
	}
	printf(buffer);
	exit(0);
}
