#include "exec.h"

extern multiboot_info_t *info;

// Return true if the exec was successful or false otherwise.
int exec(char *filename, char *argv[], int argc)
{
    char buffer[SM_BUFFER];
    int success_flag = -1;
    bool available_flag = false;
    task_t *task_ptr;

    for (int i = 0; i < TASKS_SIZE; i++)
    {
        task_ptr = &tasks[i];
        if (task_ptr->is_available)
        {
            available_flag = true;
            break;
        }
    }

    if (available_flag)
    {
        multiboot_module_t addr;
        if (find_file(filename, &addr))
        {
            if (argc > ARGV_BUFFER)
            {
                puts_error("Exception : Exec, too many args !", COLOR_LIGHT_RED);
            }
            else
            {
                multiboot_uint32_t size = addr.mod_end - addr.mod_start;
                task_ptr->is_available = false;
                file_read(&addr, (void *)task_ptr->task_addr_space);

                char **task_argv = (char **)task_ptr->task_addr_space + size;
                char *argv_ptr = (char *)task_argv + ARGV_BUFFER * sizeof(uint32_t);
                for (int i = 0; i < argc; i++)
                {
                    uint_t argv_len = strlen(argv[i]) + 1;
                    strncpy(argv_ptr, argv[i], argv_len);
                    task_argv[i] = (char *)(argv_ptr - (char *)task_ptr->task_addr_space);
                    argv_ptr += argv_len;
                }

                uint32_t *stack = (uint32_t *)&task_ptr->task_addr_space[TASK_ADDR_SPACE];
                *--stack = (uint32_t)argc;
                *--stack = (uint32_t)task_argv - (uint32_t)task_ptr->task_addr_space;
                task_ptr->task_tss.esp -= 8;
                task_ptr->task_tss.esp0 -= 8;

                task_switch(task_ptr->gdt_tss_sel);

                success_flag = task_ptr->task_tss.eax;
                clean_task(task_ptr);
            }
        }
        else
        {
            eprintf("Exception : Exec, file %s not found !", COLOR_LIGHT_RED, buffer, filename);
        }
    }
    else
    {
        puts_error("Exception : Exec, no task available !", COLOR_LIGHT_RED);
    }

    return success_flag;
}
