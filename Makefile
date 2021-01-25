QEMU=qemu-system-i386 -enable-kvm -m 64 -monitor stdio
ISO_NAME=fenetre.iso
ISO_FOLDER=ISO/
ISO_PATH=$(ISO_FOLDER)$(ISO_NAME)

run: $(ISO_PATH) kernel
	$(QEMU) -cdrom $(ISO_PATH)

run_shell: kernel
	$(QEMU) -kernel ./kernel/kernelExec

$(ISO_PATH):
	mkdir ISO
	grub-mkrescue /usr/lib/grub/i386-pc -o $@ $(KERNEL_FOLDER)

kernel:
	$(MAKE) -C kernel

.PHONY: kernel clean

clean:
	$(MAKE) clean -C kernel
	rm -rf $(ISO_FOLDER)