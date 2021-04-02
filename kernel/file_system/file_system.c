/**
 * @file file_system.c
 * @brief simulate a filesystem using grub modules 
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 22, 2021
 * @version 0.1
 */

#include "file_system.h"
#include "../../common/lib/string.h"

extern multiboot_info_t *info;

/**
 * Finds a file in GRUB modules
 * @param filename name of the file
 * @param addr address to retrieve file information
 * 
 * @return true if find else false
 */
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

/**
 * Copy a file to a buffer
 * @param addr information of the file
 * @param buf write buffer 
 */
void file_read(multiboot_module_t *addr, void *buf) {
    multiboot_uint32_t size = addr->mod_end - addr->mod_start;
    memcpy(buf, (void*)addr->mod_start, size);
}

/**
 * Get the name and size of a file
 * @param addr information of the file
 * @param stat structure where we will copy the information
 */
void file_stat(multiboot_module_t *addr, stat_t *stat) {
    multiboot_uint32_t size = addr->mod_end - addr->mod_start;
    char *filename = (char*)addr->cmdline;
    stat->size = size;
    memcpy(stat->filename, filename, strlen(filename) + 1);
}