#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "../boot/multiboot.h"
#include "../../common/types.h"
#include "../../common/lib/string.h"
#include "../vid/stdio.h"

bool find_file(char *filename, multiboot_module_t *addr);
void file_read(multiboot_module_t *addr, void *buf);
void file_stat(multiboot_module_t *addr, stat_t *stat);
void ls();

#endif