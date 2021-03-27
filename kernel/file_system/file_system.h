#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "../../common/types.h"
#include "../boot/multiboot.h"

bool find_file(char *filename, multiboot_module_t *addr);
void file_read(multiboot_module_t *addr, void *buf);
void file_stat(multiboot_module_t *addr, stat_t *stat);

#endif