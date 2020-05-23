GCC=riscv64-unknown-elf-gcc
IMG=../img.elf

ASM_SRCS=$(shell find . -name "*.S")
C_SRCS := $(shell find . -name "*.c")
LINK=link.ld

INCLUDE=include
LIBC=$(INCLUDE)/libc

# https://forums.sifive.com/t/relocation-truncated-to-fit-r-riscv-hi20/2430
CFLAGS+= -I $(INCLUDE) \
		 -I $(LIBC) \
		 -std=gnu99 \
		 -ffreestanding \
		 -Wall \
		 -Wextra \
		 -g \
		 -nostartfiles \
		 -nostdlib \
		 -nodefaultlibs \
		 -mcmodel=medany \
 		 -falign-functions=16

all: build

build:
	$(GCC) $(CFLAGS) $(ASM_SRCS) $(C_SRCS) -T $(LINK) -o $(IMG)

clean:
	rm -f $(OS_OBS)
	rm -f $(ASM_OBS)
