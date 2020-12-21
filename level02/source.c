#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

int main(int argc, const char **argv)
{
	char *password[96];		 // rbp-0x110
	char *password_file[40]; // rbp-0xa0
	char *username[96];		 // rbp-0x70
	int tmp;				 // rbp-0xc
	FILE *file;				 // rbp-0x8

	int i = 0;
	while (i < 12)
	{
		int j = i * 8;
		username[j] = 0;
		username[j + 1] = 0;
		username[j + 2] = 0;
		username[j + 3] = 0;
		username[j + 4] = 0;
		username[j + 5] = 0;
		username[j + 6] = 0;
		username[j + 7] = 0;
		i++;
	}
	// same as bzero(username, 96);

	username[i * 8] = 0;
	username + 4;

	int i = 0;
	while (i < 5)
	{
		int j = i * 8;
		password_file[j] = 0;
		password_file[j + 1] = 0;
		password_file[j + 2] = 0;
		password_file[j + 3] = 0;
		password_file[j + 4] = 0;
		password_file[j + 5] = 0;
		password_file[j + 6] = 0;
		password_file[j + 7] = 0;
		i++;
	}
	// same as bzero(password_file, 40);

	password_file[i * 8] = 0;
	password_file + 1;

	int i = 0;
	while (i < 12)
	{
		int j = i * 8;
		password[j] = 0;
		password[j + 1] = 0;
		password[j + 2] = 0;
		password[j + 3] = 0;
		password[j + 4] = 0;
		password[j + 5] = 0;
		password[j + 6] = 0;
		password[j + 7] = 0;
		i++;
	}
	// same as bzero(password, 96);

	password[i * 8] = 0;
	password + 4;

	file = NULL;
	tmp = 0;
	file = fopen("/home/users/level03/.pass", "r");
	if (!file)
	{
		fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
		exit(1);
	}
	tmp = fread(password_file, 1, 41, file);

	int res = strcspn(password_file, "\n");
	password_file[res] = '\0';

	if (tmp != 41)
	{
		fwrite("ERROR: failed to read password file\n", 1, 36, stdout);
		exit(1);
	}
	fclose(file);
	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\**************************************/");
	printf("--[ Username: ");
	fgets(username, 100, stdin);

	int res = strcspn(username, "\n");
	username[res] = '\0';

	printf("--[ Password: ");

	fgets(password, 100, stdin);
	int res = strcspn(password, "\n");
	password[res] = '\0';

	puts("*****************************************");

	if (strncmp(password_file, password, 41))
	{
		printf(username);
		puts(" does not have access!");
		exit(1);
	}

	printf("Greetings, %s!\n", username);
	system("/bin/sh");
	return 0;
}
