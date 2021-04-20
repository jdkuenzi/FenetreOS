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
		puts("\n");
		if (line[0] == 0) {
			
		}
		else if (strcmp("help", line) == 0) {
			help();
		}
		else if (start_with("cat ", line)) {
			cat(trim(line + strlen("cat ")));
		}
		else if (start_with("sleep ", line)) {
			uint_t ms = atoi(trim(line + strlen("sleep ")));
			printf("Sleeping for %dms...\n", sn_buf, ms);
			sleep(ms);
		}
		else if (strcmp("exit", line) == 0) {
			puts("\nBye.\n");
			break;
			// exit();
		}
		// Attempt to run the specified file
		else {
			return_code = exec(line, NULL, 0);
			printf("%s exit with code %d\n", sn_buf, line, return_code);
		}
	}

	return 1;
}