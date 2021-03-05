include ../common.mk


C_SRC:=$(shell find . -name "*.c")
C_OBJ:=$(C_SRC:.c=.o)
C_DEPS:=$(C_OBJ:.o=.d)

ASM_SRC:=$(shell find . -name "*.s")
ASM_OBJ:=$(ASM_SRC:.s=.o)

all: $(C_OBJ) $(ASM_OBJ)

%.o: %.c
	$(CC) -c $< -o $@

%.o: %.s
	nasm -f elf32 $< -o $@

.PHONY: vid pmio lib

clean:
	rm -rf $(OUT) $(C_OBJ) $(ASM_OBJ) $(C_DEPS)
