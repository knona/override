#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void log_wrapper(FILE *backup,	 // rbp-0x118
				 char *line,	 // rbp-0x120
				 char *file_path // rbp-0x128
)
{
	char buffer[254]; // rbp-0x110

	strcpy(buffer, line);
	snprintf(buffer + strlen(buffer), 254 - strlen(buffer), file_path);

	buffer[strcspn(buffer, "\n")] = 0;

	fprintf(backup, "LOG: %s\n", buffer);
}

int main(int argc,		   // rbp-0x94
		 const char **argv // rbp-0xa0
)
{
	FILE *backup;  // rbp-0x88
	FILE *file;	   // rbp-0x80
	int fd_log;	   // rbp-0x78
	char str[100]; // rbp-0x70
	char c;		   // rbp-0x71

	if (argc != 2)
		printf("Usage: %s filename\n", argv[0]);

	backup = fopen("./backups/.log", "w");
	if (!backup)
	{
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(1);
	}

	log_wrapper(backup, "Starting back up: ", argv[1]);

	file = fopen(argv[1], "r");
	if (!file)
	{
		printf("ERROR: Failed to open %s\n", argv[1]);
		exit(1);
	}

	memcpy(str, "./backups/", 10);
	strncat(str, argv[1], 99 - strlen(str));
	fd_log = open(str, O_WRONLY | O_CREAT | O_EXCL);
	if (fd_log < 0)
	{
		printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
		exit(1);
	}

	c = fgetc(file);
	while (c != 0xff)
	{
		write(fd_log, &c, 1);
		c = fgetc(file);
	}

	log_wrapper(backup, "Finished back up ", argv[1]);

	fclose(file);
	close(fd_log);
	return 0;
}
