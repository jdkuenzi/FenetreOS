global entrypoint  ; the entry point symbol defined in kernel.ld

extern entry

; Values for the multiboot header
MULTIBOOT_MAGIC        equ 0x1BADB002  ; multiboot magic value
MULTIBOOT_ALIGN        equ 1   ; load kernel and modules on page boundary
MULTIBOOT_MEMINFO      equ 2   ; provide kernel with memory info

MULTIBOOT_FLAGS        equ (MULTIBOOT_ALIGN|MULTIBOOT_MEMINFO)

; Magic + checksum + flags must equal 0!
MULTIBOOT_CHECKSUM     equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

;---------------------------------------------------------------------------------------------------
; .multiboot section
; This section must be located at the very beginning of the kernel image.

section .multiboot

; Mandatory part of the multiboot header
; see http://git.savannah.gnu.org/cgit/grub.git/tree/doc/multiboot.h?h=multiboot
dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

entrypoint:

		; Bootloader code starts executing here
		cli  ; disable hardware interruptions

		; TODO:
		; - Initialize the stack pointer and EBP (both to the same value)
		mov esp, stack_space
		mov ebp, stack_space
		; - Pass the multiboot info to the kernel entry point function

		; - Call the kernel entry point function (C code)
		call entry


; should never return from kernel main fonction
.forever:              
		hlt
		jmp .forever

;-----------------------------------------------------------------------------------------------------------------------
; Stack section
; nobits indicates value in the section aren't initialized (as with .bss)
section .stack nobits

; TODO:
; - Reserve a stack area (1MB minimum) for the kernel
stack_space:
	resb 1048576