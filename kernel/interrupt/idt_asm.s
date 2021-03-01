%include "const.inc"

section .text                      ; start of the text (code) section
align 4                            ; the code must be 4 byte aligned

;------------------------------------------------
; Load the IDT
global idt_load
idt_load:
	; TODO...
    mov  eax,[esp+4]  ; Get the pointer to the IDT, passed as a parameter.
    lidt [eax] ; where eax points to a 48 bits structure
    ret

;------------------------------------------------
; CPU exceptions

; example of low-level exception (without code) handler
global _exception_3
_exception_3:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    3    ; exception number
    jmp     exception_wrapper

; example of low-level exception (with code) handler
global _exception_8
_exception_8:
    cli          ; disable interrupts
    push    8    ; exception number
    jmp     exception_wrapper

;------------------------------------------------
; IRQ

; example of low-level IRQ handler
global _irq_5
_irq_5:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    5    ; irq number
    jmp     irq_wrapper

;------------------------------------------------
; Wrapper for exceptions

extern exception_handler
exception_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax
    
    ; Save where we're coming from (either from a task or the kernel)
    str		eax
	push	eax
    
    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax    
    call    exception_handler  ; implemented in idt.c
    pop     eax  ; only here to balance the two "push eax" done before the call
    pop		eax

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax
    
	; Fix the stack pointer due to the 2 push done before the call
	; to exception_wrapper/irq_wrapper : error code and exception/irq number
    add     esp,8
    iret

;------------------------------------------------
; Wrapper for exceptions

extern irq_handler
irq_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax
    
    ; Save where we're coming from (either from a task or the kernel)
    str		eax
	push	eax
    
    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax    
    call    irq_handler  ; implemented in idt.c
    pop     eax  ; only here to balance the two "push eax" done before the call
    pop		eax

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax
    
	; Fix the stack pointer due to the 2 push done before the call
	; to exception_wrapper/irq_wrapper : error code and exception/irq number
    add     esp,8
    iret
