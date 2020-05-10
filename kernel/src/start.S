.global _start
.global main
.section .text.prologue

_start: 
	# Setup the stack
	la sp, __stack_top
	la gp, __global_pointer	

	li t0, (0b1 << 11) | (0b1 << 5)
	csrw mstatus, t0

	la t0, main
	csrw mepc, t0

	li t0, 0xffff

	csrrw x0, medeleg, t0
	csrrw x0, mideleg, t0

	li t0, 0xffffffff

	csrrw x0, mie, t0

	mret
