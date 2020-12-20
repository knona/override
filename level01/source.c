#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

char *a_user_name; // 0x804a040

int verify_user_name(void)
{
	puts("verifying username....\n");
	return strncmp("dat_wil", a_user_name, 7); // not really a call to strncmp function
}

int verify_user_pass(char *pass)
{
	return strncmp("admin", pass, 5); // not really a call to strncmp function
}

int main(int argc, const char **argv)
{
	char buffer[64]; // esp+0x1c
	int a;			 // esp+0x5c

	int i = 0;
	while (i < 16)
	{
		int j = i * 4;
		buffer[j] = 0;
		buffer[j + 1] = 0;
		buffer[j + 2] = 0;
		buffer[j + 3] = 0;
		i++;
	}
	// same as bzero(buffer, 64);

	a = 0;

	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(a_user_name, 256, stdin);
	if (verify_user_name())
	{
		puts("nope, incorrect username...\n");
		return 1;
	}
	puts("Enter Password: ");
	a = verify_user_pass(fgets(buffer, 100, stdin));
	if (a == 0)
	{
		puts("nope, incorrect password...\n");
		return 1;
	}
	else
	{
		if (a == 0)
			return 0;
		else
		{
			puts("nope, incorrect password...\n");
			return 1;
		}
	}
}
