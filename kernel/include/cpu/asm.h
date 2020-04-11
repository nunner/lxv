/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

/*
 * The 2-Clause BSD License
 * SPDX short identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Western Digital Corporation or its affiliates and other
 * contributors.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifdef __ASSEMBLY__
#define __ASM_STR(x)	x
#else
#define __ASM_STR(x)	#x
#endif

#define csr_swap(csr, val)                                              \
	({                                                              \
	 unsigned long __v = (unsigned long)(val);               \
	 __asm__ __volatile__("csrrw %0, " __ASM_STR(csr) ", %1" \
			 : "=r"(__v)                        \
			 : "rK"(__v)                        \
			 : "memory");                       \
			 __v;                                                    \
			 })

#define csr_read(csr)                                           \
	({                                                      \
	 register unsigned long __v;                     \
	 __asm__ __volatile__("csrr %0, " __ASM_STR(csr) \
			 : "=r"(__v)                \
			 :                          \
			 : "memory");               \
			 __v;                                            \
			 })

#define csr_write(csr, val)                                        \
	({                                                         \
	 unsigned long __v = (unsigned long)(val);          \
	 __asm__ __volatile__("csrw " __ASM_STR(csr) ", %0" \
			 :                             \
			 : "rK"(__v)                   \
			 : "memory");                  \
			 })

#define csr_read_set(csr, val)                                          \
	({                                                              \
	 unsigned long __v = (unsigned long)(val);               \
	 __asm__ __volatile__("csrrs %0, " __ASM_STR(csr) ", %1" \
			 : "=r"(__v)                        \
			 : "rK"(__v)                        \
			 : "memory");                       \
			 __v;                                                    \
			 })

#define csr_set(csr, val)                                          \
	({                                                         \
	 unsigned long __v = (unsigned long)(val);          \
	 __asm__ __volatile__("csrs " __ASM_STR(csr) ", %0" \
			 :                             \
			 : "rK"(__v)                   \
			 : "memory");                  \
			 })

#define csr_read_clear(csr, val)                                        \
	({                                                              \
	 unsigned long __v = (unsigned long)(val);               \
	 __asm__ __volatile__("csrrc %0, " __ASM_STR(csr) ", %1" \
			 : "=r"(__v)                        \
			 : "rK"(__v)                        \
			 : "memory");                       \
			 __v;                                                    \
			 })

#define csr_clear(csr, val)                                        \
	({                                                         \
	 unsigned long __v = (unsigned long)(val);          \
	 __asm__ __volatile__("csrc " __ASM_STR(csr) ", %0" \
			 :                             \
			 : "rK"(__v)                   \
			 : "memory");                  \
			 })
