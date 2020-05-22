define print_array
    print *($arg0)@(sizeof($arg0)/ sizeof($arg0[0]))
end

define dump_page
	x/4096 $arg0
end

layout asm
layout reg
layout split 
target remote localhost:26000

file img.elf
b _start
b main:38
b handle_supervisor_trap
b scan_virtio
