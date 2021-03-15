#include "ulibc.h"

static void help() {
	putc('\n');
	puts("PROG      : execute program PROG\n");
	puts("cat FILE  : dump contents of FILE\n");
	puts("sleep N   : sleep for N milliseconds\n");
	puts("exit      : exit the shell\n");
}

static void run() {
	puts("True or false? This is easily the most awesome lab I've ever worked on :-)\n");

	char buf[512];

	while (1) {
		puts(">");
		read_string(buf);
		char *line = tolower(trim(buf));  // remove heading and trailing spaces and convert to lower case
		if (line[0] == 0) {
			putc('\n');
			continue;
		}
		else if (strcmp("help", line) == 0) {
			help();
		}
		else if (starts_with("cat ", line)) {
			putc('\n');
			cat(trim(line + strlen("cat ")));
		}
		else if (starts_with("sleep ", line)) {
			uint_t ms = atoi(trim(line + strlen("sleep ")));
			putc('\n');
			printf("Sleeping for %dms...\n", ms);
			sleep(ms);
		}
		else if (strcmp("exit", line) == 0) {
			puts("\nBye.\n");
			exit();
		}
		// Attempt to run the specified file
		else {
			putc('\n');
			if (exec(line) == -1) {
				printf("Failed executing \"%s\"\n", line);
			}
		}
	}
}

void main() {
	run();
}
