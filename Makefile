BUILD_DIR=build
KERNEL=img.elf

QEMU=qemu-system-riscv64 \
	 -machine virt \
	 -serial stdio \
 	 -nographic \
	 -machine virt \
 	 -monitor tcp::1234,server,nowait \
	 -device virtio-net-device \
	 -bios none \
	 -kernel $(KERNEL)

# -d cpu_reset \

all: clean kernel

.PHONY: kernel

kernel:
	make -C kernel

run: all
	$(QEMU)

debug: all
	$(QEMU) -S -gdb tcp::26000

clean:
	rm -f *.elf

todo: 
	rg "TODO" > TODO
