LD=gcc
COMMON_FLAGS=-m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -fno-pie -static
CC=gcc -g -std=gnu11 $(COMMON_FLAGS) -Wall -Wextra -MMD
