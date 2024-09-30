#ifndef _EXEC_H_
#define _EXEC_H_

#include "task.h"
#include "../mem/gdt.h"
#include "../vid/stdio.h"
#include "../file_system/file_system.h"
#include "../../common/types.h"
#include "../../common/lib/string.h"

extern int exec(char *filename, char *argv[], int argc);

#endif