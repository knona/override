#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

int main(int argc, const char **argv)
{
	FILE *file;		   // rbp-0x8
	int tmp;		   // rbp-0xc
	char *buffer[96];  // rbp-0x70
	char *buffer2[40]; // rbp-0xa0
	char *buffer3[96]; // rbp-0x110

	int i = 0;
	while (i < 12)
	{
		int j = i * 8;
		buffer[j] = 0;
		buffer[j + 1] = 0;
		buffer[j + 2] = 0;
		buffer[j + 3] = 0;
		buffer[j + 4] = 0;
		buffer[j + 5] = 0;
		buffer[j + 6] = 0;
		buffer[j + 7] = 0;
		i++;
	}
	// same as bzero(buffer, 96);

	buffer[i * 8] = 0;
	buffer + 4;

	int i = 0;
	while (i < 5)
	{
		int j = i * 8;
		buffer2[j] = 0;
		buffer2[j + 1] = 0;
		buffer2[j + 2] = 0;
		buffer2[j + 3] = 0;
		buffer2[j + 4] = 0;
		buffer2[j + 5] = 0;
		buffer2[j + 6] = 0;
		buffer2[j + 7] = 0;
		i++;
	}
	// same as bzero(buffer2, 40);

	buffer2[i * 8] = 0;
	buffer2 + 1;

	int i = 0;
	while (i < 12)
	{
		int j = i * 8;
		buffer3[j] = 0;
		buffer3[j + 1] = 0;
		buffer3[j + 2] = 0;
		buffer3[j + 3] = 0;
		buffer3[j + 4] = 0;
		buffer3[j + 5] = 0;
		buffer3[j + 6] = 0;
		buffer3[j + 7] = 0;
		i++;
	}
	// same as bzero(buffer3, 96);

	buffer3[i * 8] = 0;
	buffer3 + 4;

	file = NULL;
	tmp = 0;
	file = fopen("/home/users/level03/.pass", "r");
	if (!file)
	{
		fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
		exit(1);
	}
	tmp = fread(buffer2, 1, 0x29, file);

	int res = strcspn(buffer2, "\n");
	buffer2[res] = '\0';

	if (tmp != 41)
	{
		fwrite("ERROR: failed to read password file\n", 1, 36, stdout);
		exit(1);
	}
	fclose(file);
	puts("===== [ Secure Access System v1.0 ] =====");
	puta("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\**************************************/");
	printf("--[ Username: ");
	fgets(buffer, 100, stdin);

	int res = strcspn(buffer, "\n");
	buffer[res] = '\0';

	printf("--[ Password: ");

	fgets(buffer3, 100, stdin);
	int res = strcspn(buffer3, "\n");
	buffer3[res] = '\0';

	puts("*****************************************");

	if (!strncmp(buffer2, buffer3, 41))
	{
		printf(buffer);
		puts(" does not have access!");
		exit(1);
	}

	printf("Greetings, %s!\n", buffer);
	system("/bin/sh");
	return 0;
}
