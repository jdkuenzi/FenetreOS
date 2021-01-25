QEMU=qemu-system-i386 -enable-kvm -m 64 -monitor stdio
ISO_NAME=fenetre.iso
ISO_FOLDER=ISO/
ISO_PATH=$(ISO_FOLDER)$(ISO_NAME)

run: $(ISO_PATH)
	$(QEMU) -cdrom $(ISO_PATH)

run_shell:
	$(QEMU) -kernel ./kernel/kernelExec

$(ISO_PATH):
	grub-mkrescue /usr/lib/grub/i386-pc -o $@ $(KERNEL_FOLDER)

kernel:
	$(MAKE) -C kernel

# .PHONY: kernel

clean:
	$(MAKE) clean -C kernel
	rm -f $(ISO_PATH)