#include "file_system.h"
#include "../../common/lib/string.h"

extern multiboot_info_t *info;

bool find_file(char *filename, multiboot_module_t *addr) {
    bool file_found_flag = false;
    multiboot_module_t *mods_addr = (multiboot_module_t*)info->mods_addr;
    for (multiboot_uint32_t i = 0; i < info->mods_count; i++, mods_addr++)
    {
        if (strcmp((char*)mods_addr->cmdline, filename) == 0) {
            file_found_flag = true;
            *addr = *mods_addr;
            break;
        }
    }

    return file_found_flag;
}

void file_read(multiboot_module_t *addr, void *buf) {
    multiboot_uint32_t size = addr->mod_end - addr->mod_start;
    memcpy(buf, (void*)addr->mod_start, size);
}

void file_stat(multiboot_module_t *addr, stat_t *stat) {
    multiboot_uint32_t size = addr->mod_end - addr->mod_start;
    char *filename = (char*)addr->cmdline;
    stat->size = size;
    memcpy(stat->filename, filename, strlen(filename) + 1);
}