include ../common.mk

LD_FLAGS=-g -Tkernel.ld $(COMMON_FLAGS) -lgcc
CC_FLAGS=

DEP=...


%.o: %.c
	$(CC) $(CC_FLAGS) $< -c



clean:



-include $(DEP)
