#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <sys/wait.h>

typedef unsigned int uint;

int main(int argc, const char **argv)
{
	uint a;			  // esp+0x1c
	char buffer[128]; // esp+0x20
	uint b;			  // esp+0xa0
	uint c;			  // esp+0xa4
	uint d;			  // esp+0xa8
	pid_t child;	  // esp+0xac

	child = fork();
	bzero(buffer, 128);
	d = 0;
	a = 0;

	if (!child)
	{
		prctl(PR_SET_PDEATHSIG, 1);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(buffer);
		return (0);
	}
	while (d != 11)
	{

		wait(&a);
		b = a;
		if (b & 0x7f != 0)
		{
			c = a;
			int tmp = (c & 0x7f + 1) >> 1;
			char last_byte = ((char *)&a)[3];
			last_byte >>= 1;
			if (last_byte > 0)
			{
				puts("child is exiting...");
				return 0;
			}
			d = ptrace(PTRACE_PEEKUSER, child, 2, 0);
		}
	}
	puts("no exec() for you");
	kill(child, SIGKILL);
}
