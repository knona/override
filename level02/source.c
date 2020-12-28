#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

// r < <(sleep .3 ; cat test test)

// b *0x4008ad  set $rax = 0x00007ffff7dd4340

// PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv

// b *0x400981 set $rax = 0x00007ffff7dd4180

// %llx %llx %llx %llx %llx %llx %llx %llx %llx %llx %llx

// b *0x400a24

int main(int argc, const char **argv)
{
	char *password[96];		 // rbp-0x110
	char *password_file[40]; // rbp-0xa0 0x 7f ff ff ff e5 40
	char *username[96];		 // rbp-0x70
	int tmp;				 // rbp-0xc
	FILE *file;				 // rbp-0x8

	bzero(username, 96);	  // not really a call to bzero function
	bzero(password_file, 40); // not really a call to bzero function
	bzero(password, 96);	  // not really a call to bzero function

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
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
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
