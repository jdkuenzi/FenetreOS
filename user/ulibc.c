#include "ulibc.h"

extern int syscall(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

int setcursor(uint32_t x, uint32_t y) {
    return syscall(SYSCALL_SET_CUSRSOR, x, y, 0, 0);
}

int clean_vid_x_y(uint32_t x, uint32_t y) {
	return syscall(SYSCALL_CLEAN_X_Y, x, y, 0, 0);
}

int puts_x_y(char *s, uint32_t x, uint32_t y) {
	return syscall(SYSCALL_PUTS_X_Y, (uint32_t)s, x, y, 0);
}

int puts(char *s) {
    return syscall(SYSCALL_PUTS, (uint32_t)s, 0, 0, 0);
}

int exec(char *filename, char** argv, int argc) {
	return syscall(SYSCALL_EXEC, (uint32_t)filename, argv, argc, 0);
}

char getc() {
	return syscall(SYSCALL_GETC, 0, 0, 0, 0);
}

int read_string(char *buf, uint16_t size) {
	return syscall(SYSCALL_READ_STRING, (uint32_t)buf, size, 0, 0);
}

int filesread(char *filename, void *buf) {
	return syscall(SYSCALL_FILE_READ, (uint32_t)filename, (uint32_t)buf, 0, 0);
}

int filestat(char *filename, stat_t *stat) {
	return syscall(SYSCALL_FILE_STAT, (uint32_t)filename, (uint32_t)stat, 0, 0);
}

int cat(char *filename) {
	stat_t st;
	if (filestat(filename, &st) == 0) {
		char buf[st.size];
		filesread(filename, (void*)buf);
		return puts(buf);
	} else {
		return -1;
	}
}

uint_t random(uint_t max) {
	return syscall(SYSCALL_RANDOM, (uint32_t)max, 0, 0, 0);
}

int sleep(uint_t ms) {
	return syscall(SYSCALL_SLEEP, (uint32_t)ms, 0, 0, 0);
}

void exit() {
	asm("iret");
}