#ifndef _EXEC_H_
#define _EXEC_H_

#include "../../common/types.h"
#include "../boot/multiboot.h"

extern bool exec(char *filename, multiboot_info_t* info);

#endif
