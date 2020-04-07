layout asm
layout reg
layout split 
target remote localhost:26000

file img.elf
b main
