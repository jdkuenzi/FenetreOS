#ifndef _SYSCALL_NB_COMMON_H_
#define _SYSCALL_NB_COMMON_H_

typedef enum { 
    SYSCALL_GETC = 0,
    SYSCALL_EXEC,
    ...
    
    __SYSCALL_END__
} syscall_t;

#endif
