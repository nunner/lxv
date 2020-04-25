.global _start
.global main
.section .text.prologue

_start: 
	# Setup the stack
	la sp, __stack_top
	la gp, __global_pointer	

	call setup_machine_interrupts
	call init_paging
		
	li t0, (0b1 << 11) | (0b1 << 5)
	csrw mstatus, t0

	la t0, main
	csrw mepc, t0

	mret
