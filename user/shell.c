#include "ulibc.h"

static void help() {
	puts("PROG      : execute program PROG\n");
	puts("cat FILE  : dump contents of FILE\n");
	puts("sleep N   : sleep for N milliseconds\n");
	puts("exit      : exit the shell\n");
}

void main() {
	// puts("True or false? This is easily the most awesome lab I've ever worked on :-)\n");
	char cmd[15];
	char sn_buf[20];
	while (1) {
		puts(">");
		read_string(cmd, 15);
		char *line = cmd;  // remove heading and trailing spaces and convert to lower case
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
			exit();
		}
		// Attempt to run the specified file
		else {
			// printf("\nline = %s\n", sn_buf, line);
			// printf("Je vais executer %s\n", sn_buf, line);
			exec(line);
		}
	}
}
