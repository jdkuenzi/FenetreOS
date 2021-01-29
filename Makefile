QEMU=qemu-system-i386 -enable-kvm -m 64 -monitor stdio
BUILD_STRUCTURE=build/boot/grub build/dat
KERNEL_FOLDER=build/boot/kernel.elf
GRUB_CFG_SRC=grub/grub.cfg
GRUB_CFG_DST=build/boot/grub/
ISO_NAME=fenetre.iso
ISO_FOLDER=ISO/
ISO_PATH=$(ISO_FOLDER)$(ISO_NAME)

run: kernel $(ISO_PATH)
	$(QEMU) -cdrom $(ISO_PATH)

run_shell: kernel
	$(QEMU) -kernel ./kernel/kernelExec

$(ISO_PATH):
	mkdir $(ISO_FOLDER)
	grub-mkrescue /usr/lib/grub/i386-pc $(KERNEL_FOLDER) -o $@

kernel:
	mkdir -p $(BUILD_STRUCTURE)
	cp $(GRUB_CFG_SRC) $(GRUB_CFG_DST)
	$(MAKE) -C kernel

.PHONY: kernel clean

clean:
	$(MAKE) clean -C kernel
	rm -rf build $(ISO_FOLDER)