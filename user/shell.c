#include "ulibc.h"

static void help()
{
	puts("PROG arg1 arg2 ... : execute program PROG with arguments if needed\n");
	puts("cat FILE           : dump contents of FILE\n");
	puts("sleep N[ms]        : sleep for N milliseconds\n");
	puts("argv               : display the arguments\n");
	puts("ls                 : display loaded modules\n");
	puts("cls                : clean terminal\n");
	puts("exit               : exit the shell\n");
}

static void display_argv(char *argv[], int argc)
{
	char sn_buf[SM_BUFFER];
	printf("+-- argc = %d --+\n", sn_buf, argc);
	for (int i = 0; i < argc; i++)
	{
		printf("\targv[%d] = %s\n", sn_buf, i, argv[i]);
	}
}

int main(char *argv[], int argc)
{
	char cmd[SM_BUFFER];
	char sn_buf[SM_BUFFER];
	char *local_argv[ARGV_BUFFER];
	int local_argc;
	int return_code;
	// cat("shell_logo");
	puts("Hi welcome to shell.bin, enter command 'help' to have more information :)\n");
	while (1)
	{
		puts(">");
		read_string(cmd, SM_BUFFER);
		char *line = trim(cmd); // remove heading and trailing spaces
		local_argc = 0;
		while (line)
		{
			local_argv[local_argc] = strsep(&line, " ");
			local_argc++;
		}
		to_lower(cmd); // convert to lower case

		puts("\n");
		if (cmd[0] == '\0')
		{
		}
		else if (strcmp("help", cmd) == 0)
		{
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
		else if (strcmp("exit", cmd) == 0)
		{
			puts("\nBye.\n");
			// exit();
			break;
		}
		else if (strcmp("argv", cmd) == 0)
		{
			display_argv(argv, argc);
		}
		else if (strcmp("ls", cmd) == 0)
		{
			ls();
		}
		else if (strcmp("cls", cmd) == 0)
		{
			clean_vid_x_y(0, 0);
			setcursor(0, 0);
		}
		// Attempt to run the specified file
		else
		{
			// printf("executing : %s\n", sn_buf, cmd);
			return_code = exec(cmd, local_argv, local_argc);
			printf("%s exit with code %d\n", sn_buf, cmd, return_code);
		}
	}

	return 1;
}