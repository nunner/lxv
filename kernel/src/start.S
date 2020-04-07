.global _start
.global main
.section .text.prologue

_start: 
	# Setup the stack
	la sp, __stack_top
	la gp, __global_pointer
	
	j main
