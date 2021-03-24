#include "exec.h"
#include "task.h"
#include "../mem/gdt.h"
#include "../../common/lib/string.h"

// Return true if the exec was successful or false otherwise.
bool exec(char *filename, multiboot_info_t* info) {
    bool success_flag = false;
    bool available_flag = false;
    task_t *task_ptr; 

    for (int i = 0; i < TASKS_SIZE; i++)
    {
        task_ptr = &tasks[i];
        if (task_ptr->is_available) {
            available_flag = true;
            break;
        }
    }

    if (available_flag) {
        task_ptr->is_available = false;
        multiboot_module_t *mods_addr = (multiboot_module_t*)info->mods_addr;
        for (int i = 0; i < info->mods_count; i++, mods_addr++)
        {
            if (strcmp((char*)mods_addr->cmdline, filename) == 0) {
                multiboot_uint32_t size = mods_addr->mod_end - mods_addr->mod_start + 1;
                memcpy(task_ptr->task_addr_space, (void*)mods_addr->mod_start, size);
                task_switch(task_ptr->gdt_tss_sel);
                task_ptr->is_available = true;
                success_flag = true;
                break;
            }
        }
    }
    
    return success_flag;
}
