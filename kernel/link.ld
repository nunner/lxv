OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS {
	. = 0x80000000;
	__memory_start = .;
	__kernel_start = .;
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
		PROVIDE(__kernel_proc = .);
		. += 0x1000;
		/* https://groups.google.com/a/groups.riscv.org/d/msg/sw-dev/60IdaZj27dY/s1eJMlrUAQAJ */
		PROVIDE(__global_pointer = . + 0x800);
		__data = .;
	}

	.bss : {
		_bss = .;
		. += 0x1000;
		PROVIDE(__kernel_stack = .);
		. += 0x1000K;
		PROVIDE(__machine_stack = .);
		__bss = .;
	}
	__kernel_end = .;


	PROVIDE(__placement_addr = ALIGN(4K));
	. = 0x90000000;
	PROVIDE(__heap_start = ALIGN(4K));
}
