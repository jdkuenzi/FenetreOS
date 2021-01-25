QEMU=qemu-system-i386 -enable-kvm -m 64 -monitor stdio

run: $(ISO_NAME)
	$(QEMU) ...

kernel:
    $(MAKE) -C kernel

clean:
