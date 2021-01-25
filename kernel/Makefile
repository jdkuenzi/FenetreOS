include ../common.mk

LD_FLAGS=-T kernel.ld -lgcc
OUT=kernelExec
# CC_FLAGS=

C_SRC:=$(wildcard *.c)
C_HEADERS:=$(wildcard *.h)
C_OBJ:=$(C_SRC:.c=.o)
C_DEPS:=$(C_OBJ:.o=.d)

ASM_SRC:=$(shell find . -name "*.s")
ASM_OBJ:=$(ASM_SRC:.s=.o)

$(OUT): $(C_OBJ) $(ASM_OBJ)
	$(CC) $(LD_FLAGS) $^ -o $@

%.o: %.c
	$(CC) $< -c

%.o: %.s
	nasm -f elf32 $< -o $@

# .PHONY: clean

clean:
	rm -f $(OUT) $(C_OBJ) $(ASM_OBJ) $(C_DEPS)

-include $(C_DEPS)
