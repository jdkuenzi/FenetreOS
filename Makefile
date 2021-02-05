QEMU=qemu-system-i386 -enable-kvm -m 10M -monitor stdio
BUILD_FOLDER=build/
BUILD_STRUCTURE=$(BUILD_FOLDER)boot/grub $(BUILD_FOLDER)dat
MODULE_SRC=res/.
MODULE_DST=build/dat/
GRUB_CFG_SRC=grub/grub.cfg
GRUB_CFG_DST=build/boot/grub/
ISO_NAME=fenetre.iso
ISO_FOLDER=ISO/
ISO_PATH=$(ISO_FOLDER)$(ISO_NAME)

run: kernel $(ISO_PATH)
	$(QEMU) -cdrom $(ISO_PATH)

run_shell: kernel
	$(QEMU) -kernel ./build/boot/kernel.elf

$(ISO_PATH):
	mkdir -p $(ISO_FOLDER)
	grub-mkrescue /usr/lib/grub/i386-pc $(BUILD_FOLDER) -o $@

kernel:
	mkdir -p $(BUILD_STRUCTURE)
	cp $(GRUB_CFG_SRC) $(GRUB_CFG_DST)
	cp -r $(MODULE_SRC) $(MODULE_DST)
	$(MAKE) -C kernel

.PHONY: kernel $(ISO_PATH)

clean:
	$(MAKE) clean -C kernel
	rm -rf $(BUILD_FOLDER) $(ISO_FOLDER)