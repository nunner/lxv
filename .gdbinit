define print_array
    print *($arg0)@(sizeof($arg0)/ sizeof($arg0[0]))
end

define deref 
	print *(($arg0 *) $arg1)
end

define dump_page
	x/4096 $arg0
end

layout asm
layout reg
layout split 
target remote localhost:26000

file img.elf
b main
b handle_supervisor_trap
b scan_virtio
b setup_network
