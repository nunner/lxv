.global _start
.global main 
.global setup_cpu

.section .text.prologue

# typedef struct {
# 	uint64_t x[32];
# 	uint64_t f[32];
# 	uint64_t satp;
# 	uint64_t stack;
# 	uint64_t pc;
# } frame_t;


_start: 
	# Setup the stack
	la sp, __stack_top
	la gp, __global_pointer	

	li t0, (0b1 << 11) | (0b1 << 5)
	csrw mstatus, t0

	la t0, main 
	csrw mepc, t0

	call setup_cpu

	mret
