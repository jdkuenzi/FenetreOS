#ifndef _UTILS_H
#define _UTILS_H

/**
 * Program's entry point. 
 * @param num number of command line arguments.
 * @param base array of strings storing the command line arguments.
 * @return program exit code.
 */
char *convert(unsigned int num, int base, char *buffer, unsigned int size);

#endif