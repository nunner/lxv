.global _start
.global main
.global setup_machine_interrupts
.section .text.prologue

_start: 
	# Setup the stack
	la sp, __stack_top
	la gp, __global_pointer	

	la t0, setup_machine_interrupts
	jalr ra, t0

	la t0, init_paging
	jalr ra, t0

	la t0, _enter_supervisor
	jalr ra, t0

	j main

_enter_supervisor:
	
	ret
