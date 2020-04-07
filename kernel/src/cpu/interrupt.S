.section .text
.global enable_interrupts 
.global enable_machine_interrupt

enable_interrupts:
	# Enable machine interrupts
	li t0, (1<<3)
	csrrs x0, mstatus, t0
	# Load the trap vector
	la t0, handler
	csrrw x0, mtvec, t0

	ret

enable_machine_interrupt:
	li t0, 1
	sll t0, t0, a0
	csrrs x0, mie, t0
	ret

handler:
	csrrs a0, mcause, x0
	j default_handler
	mret
