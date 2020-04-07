BUILD_DIR=build
KERNEL=img.elf

QEMU=qemu-system-riscv64 \
	 -machine virt \
	 -serial stdio \
	 -nographic \
	 -monitor tcp::1234,server,nowait \
	 -d cpu_reset \
	 -bios none \
	 -kernel $(KERNEL)

all: clean kernel

.PHONY: bootdisk bootloader kernel

kernel:
	make -C kernel

qemu: all
	$(QEMU)

qemu-debug: all
	$(QEMU) -S -gdb tcp::26000

clean:
	rm -f *.elf
