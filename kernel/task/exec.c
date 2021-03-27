#include "exec.h"
#include "task.h"
#include "../mem/gdt.h"
#include "../vid/stdio.h"
#include "../file_system/file_system.h"
#include "../../common/lib/string.h"

extern  multiboot_info_t* info;

// Return true if the exec was successful or false otherwise.
bool exec(char *filename) {
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
        multiboot_module_t addr;
        if (find_file(filename, &addr)) {
            file_read(&addr, (void*)task_ptr->task_addr_space);
            task_switch(task_ptr->gdt_tss_sel);
            memset(task_ptr->task_addr_space, 0, sizeof(task_ptr->task_addr_space));
            success_flag = true;
        } else {
            char buffer[XL_BUFFER];
            eprintf("Exception : Exec, file \"%s\" not found !", COLOR_LIGHT_RED, buffer, filename);
        }
        task_ptr->is_available = true;
    } else {
        puts_error("Exception : Exec, no task available !", COLOR_LIGHT_RED);
    }
    
    return success_flag;
}
