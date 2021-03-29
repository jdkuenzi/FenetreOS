QEMU=qemu-system-i386 -enable-kvm -m 20M -monitor stdio
BUILD_FOLDER=build/
BUILD_STRUCTURE=$(BUILD_FOLDER)boot/grub $(BUILD_FOLDER)dat
MODULE_SRC=res/.
MODULE_DST=build/dat/
GRUB_CFG_SRC=grub/grub.cfg
GRUB_CFG_DST=build/boot/grub/
ISO_NAME=fenetre.iso
ISO_FOLDER=ISO/
ISO_PATH=$(ISO_FOLDER)$(ISO_NAME)

$(ISO_PATH): common user kernel
	mkdir -p $(ISO_FOLDER)
	grub-mkrescue /usr/lib/grub/i386-pc $(BUILD_FOLDER) -o $@

run: $(ISO_PATH)
	$(QEMU) -cdrom $(ISO_PATH)

common:
	$(MAKE) -C common

kernel:
	mkdir -p $(BUILD_STRUCTURE)
	cp $(GRUB_CFG_SRC) $(GRUB_CFG_DST)
	cp -r $(MODULE_SRC) $(MODULE_DST)
	$(MAKE) -C kernel

user:
	$(MAKE) -C user

.PHONY: kernel common user $(ISO_PATH)

clean:
	$(MAKE) clean -C common
	$(MAKE) clean -C kernel
	$(MAKE) clean -C user
	rm -rf $(BUILD_FOLDER) $(ISO_FOLDER)