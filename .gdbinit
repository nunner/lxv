layout asm
layout reg
layout split 
target remote localhost:26000

file img.elf
b _start
b main
b handle_supervisor_trap
