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

global _exception_0
_exception_0:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0    ; exception number
    jmp     exception_wrapper

global _exception_1
_exception_1:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    1    ; exception number
    jmp     exception_wrapper

global _exception_2
_exception_2:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    2    ; exception number
    jmp     exception_wrapper

; example of low-level exception (without code) handler
global _exception_3
_exception_3:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    3    ; exception number
    jmp     exception_wrapper

global _exception_4
_exception_4:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    4    ; exception number
    jmp     exception_wrapper

global _exception_5
_exception_5:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    5    ; exception number
    jmp     exception_wrapper

global _exception_6
_exception_6:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    6    ; exception number
    jmp     exception_wrapper

global _exception_7
_exception_7:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    7    ; exception number
    jmp     exception_wrapper

; example of low-level exception (with code) handler
global _exception_8
_exception_8:
    cli          ; disable interrupts
    push    0    ; error code
    push    8    ; exception number
    jmp     exception_wrapper

global _exception_9
_exception_9:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    9    ; exception number
    jmp     exception_wrapper

global _exception_10
_exception_10:
    cli          ; disable interrupts
    push    10   ; exception number
    jmp     exception_wrapper

global _exception_11
_exception_11:
    cli          ; disable interrupts
    push    11   ; exception number
    jmp     exception_wrapper

global _exception_12
_exception_12:
    cli          ; disable interrupts
    push    12   ; exception number
    jmp     exception_wrapper

global _exception_13
_exception_13:
    cli          ; disable interrupts
    push    13   ; exception number
    jmp     exception_wrapper

global _exception_14
_exception_14:
    cli          ; disable interrupts
    push    14   ; exception number
    jmp     exception_wrapper

global _exception_15
_exception_15:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    15   ; exception number
    jmp     exception_wrapper

global _exception_16
_exception_16:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    16   ; exception number
    jmp     exception_wrapper

global _exception_17
_exception_17:
    cli          ; disable interrupts
    push    17   ; exception number
    jmp     exception_wrapper

global _exception_18
_exception_18:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    18   ; exception number
    jmp     exception_wrapper

global _exception_19
_exception_19:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    19   ; exception number
    jmp     exception_wrapper

global _exception_20
_exception_20:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    20   ; exception number
    jmp     exception_wrapper

;------------------------------------------------
; IRQ
global _irq_0
_irq_0:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x0  ; irq number
    jmp     irq_wrapper

global _irq_1
_irq_1:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x1  ; irq number
    jmp     irq_wrapper

global _irq_2
_irq_2:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x2  ; irq number
    jmp     irq_wrapper

global _irq_3
_irq_3:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x3  ; irq number
    jmp     irq_wrapper

global _irq_4
_irq_4:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x4  ; irq number
    jmp     irq_wrapper

global _irq_5
_irq_5:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x5  ; irq number
    jmp     irq_wrapper

global _irq_6
_irq_6:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x6  ; irq number
    jmp     irq_wrapper

global _irq_7
_irq_7:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x7  ; irq number
    jmp     irq_wrapper

global _irq_8
_irq_8:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x8 ; irq number
    jmp     irq_wrapper

global _irq_9
_irq_9:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0x9 ; irq number
    jmp     irq_wrapper

global _irq_10
_irq_10:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0xA ; irq number
    jmp     irq_wrapper

global _irq_11
_irq_11:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0xB ; irq number
    jmp     irq_wrapper

global _irq_12
_irq_12:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0xC  ; irq number
    jmp     irq_wrapper

global _irq_13
_irq_13:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0xD ; irq number
    jmp     irq_wrapper

global _irq_14
_irq_14:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0xE ; irq number
    jmp     irq_wrapper

global _irq_15
_irq_15:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0xF ; irq number
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
