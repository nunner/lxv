OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS {
	. = 0x80000000;
	__memory_start = .;
	PROVIDE(__load_addr = .);

	.text : {
		*(.text.prologue);
		_text = .;
	} 

	__plic = 0x0c000000;
	__uart = 0x10000000;

	.rodata : {
		_rodata = .;
		__rodata = .;
	}
	.data : {
		_data = .;
		/* https://groups.google.com/a/groups.riscv.org/d/msg/sw-dev/60IdaZj27dY/s1eJMlrUAQAJ */
		PROVIDE(__global_pointer = . + 0x800);
		__data = .;
	}

	.bss : {
		_bss = .;
		. += 64K;
		PROVIDE(__stack_top = .);
		__bss = .;
	}

	PROVIDE(__heap_start = ALIGN(4K));
	PROVIDE(__heap_end = __memory_start + 128M);
}
