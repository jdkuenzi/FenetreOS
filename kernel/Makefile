include ../common.mk

LD_FLAGS=-T kernel.ld -lgcc -static
OUT=../build/boot/kernel.elf

C_SRC:=$(shell find . -name "*.c")
C_HEADERS:=$(shell find . -name "*.h")
C_OBJ:=$(C_SRC:.c=.o)
C_DEPS:=$(C_OBJ:.o=.d)

ASM_SRC:=$(shell find . -name "*.s")
ASM_OBJ:=$(ASM_SRC:.s=.o)

$(OUT): $(C_OBJ) $(ASM_OBJ)
	$(CC) $(LD_FLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@

%.o: %.s
	nasm -f elf32 $< -o $@

.PHONY: vid pmio boot lib

clean:
	rm -rf $(OUT) $(C_OBJ) $(ASM_OBJ) $(C_DEPS)

-include $(C_DEPS)
