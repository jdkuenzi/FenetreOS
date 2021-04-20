#include "exec.h"
#include "task.h"
#include "../mem/gdt.h"
#include "../vid/stdio.h"
#include "../file_system/file_system.h"
#include "../../common/lib/string.h"

extern  multiboot_info_t* info;

// Return true if the exec was successful or false otherwise.
int exec(char *filename, char *argv[], int argc) {

    int success_flag = -1;
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
        multiboot_module_t addr;
        if (find_file(filename, &addr)) {
            multiboot_uint32_t size = addr.mod_end - addr.mod_start;
            task_ptr->is_available = false;
            file_read(&addr, (void*)task_ptr->task_addr_space);
            int *stack = (int*)task_ptr->task_addr_space + size;
            stack = task_ptr->argv;
            for (int i = 0; i < argc; i++)
            {
                task_ptr->argv[i] = argv[i];
            }
            stack = (int*)&task_ptr->task_addr_space[1049000];
            *stack-- = task_ptr->argv;
            *stack = argc;
            task_ptr->task_tss.esp -= 8;
            task_ptr->task_tss.esp0 -= 8;
            task_switch(task_ptr->gdt_tss_sel);
            success_flag = task_ptr->task_tss.eax;
            clean_task(task_ptr);
        } else {
            char buffer[XL_BUFFER];
            eprintf("Exception : Exec, file \"%s\" not found !", COLOR_LIGHT_RED, buffer, filename);
        }
    } else {
        puts_error("Exception : Exec, no task available !", COLOR_LIGHT_RED);
    }

    return success_flag;
}
