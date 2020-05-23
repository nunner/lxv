.global aquire
.global release

.global lock
.global locked
.global unlock

aquire:
	li t0, 1
	loop:
		lw t1, (a0)
		bnez t1, loop
		amoswap.w.aq t1, t0, (a0)
		bnez t1, loop 
	ret

release:
	amoswap.w.rl x0, x0, (a0)
	j yield
	ret
