.altmacro
.global machine_trap_entry

.macro save_reg i, basereg=t6
	sd x\i, ((\i)*8)(basereg)
.endm

.macro load_reg i, basereg=t6
	ld x\i, ((\i)*8)(basereg)
.endm

machine_trap_entry:
# Switch t6 with the pointer to the trap frame struct
	csrrw t6, mscratch, t6

	.set i, 1
	.rept 30
		save_reg %i
		.set i, i+1
	.endr

	mv t5, t6
	sd t6, (31*8)(t5)
	csrw mscratch, t5

	mv a0, sp
	
	call handle_machine_trap

	.set i, 1
	.rept 31
		load_reg %i
		.set i, i+1
	.endr

	mret
