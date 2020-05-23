#pragma once

#define __CREATE_SYSCALL_DECL(name) void name();
#define __CREATE_SYSCALL(name, num) 			\
	inline void name() {						\
		__asm__("sd a0, -8(sp)");				\
		__asm__("li a0, %0" :: "I" (num) ); 	\
		__asm__("ecall");						\
		__asm__("ld a0, -8(sp)");				\
	}


__CREATE_SYSCALL_DECL(exit)
__CREATE_SYSCALL_DECL(yield)
