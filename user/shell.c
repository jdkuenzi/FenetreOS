#include "ulibc.h"

static void help() {
	puts("PROG      : execute program PROG\n");
	puts("cat FILE  : dump contents of FILE\n");
	puts("sleep N   : sleep for N milliseconds\n");
	puts("exit      : exit the shell\n");
}

int main(char *argv[], int argc) {
	char cmd[SM_BUFFER];
	char sn_buf[SM_BUFFER];
	char *local_argv[ARGV_BUFFER];
	int local_argc;
	int return_code;
	cat("shell_logo");
	printf("\nargc = %d\n", sn_buf, argc);
	for (int i = 0; i < argc; i++)
	{
		sn_printf(sn_buf, "\n%s\n", argv[i]);
		puts(sn_buf);
	}

	while (1)
	{
		puts(">");
		read_string(cmd, SM_BUFFER);
		char *line = to_lower(trim(cmd)); // remove heading and trailing spaces and convert to lower case
		local_argc = 0;
		while (line)
		{
			local_argv[local_argc] = strsep(&line, " ");
			local_argc++;
		}

		// printf("\nlocal_argc = %d\n", sn_buf, local_argc);
		// for (int i = 0; i < local_argc; i++)
		// {
		// 	printf("\nlocal_argv[%d]=%s len=%d\n", sn_buf, i, local_argv[i], strlen(local_argv[i]));
		// }
		puts("\n");
		if (cmd[0] == 0)
		{
		}
		else if (strcmp("help", cmd) == 0) {
			help();
		}
		else if (strcmp("cat", cmd) == 0 && local_argc > 1)
		{
			cat(local_argv[1]);
		}
		else if (strcmp("sleep", cmd) == 0 && local_argc > 1)
		{
			uint_t ms = atoi(local_argv[1]);
			printf("Sleeping for %dms...\n", sn_buf, ms);
			sleep(ms);
		}
		else if (strcmp("exit", cmd) == 0) {
			puts("\nBye.\n");
			break;
			// exit();
		}
		// Attempt to run the specified file
		else {
			return_code = exec(cmd, local_argv, local_argc);
			printf("%s exit with code %d\n", sn_buf, cmd, return_code);
		}
	}

	return 1;
}