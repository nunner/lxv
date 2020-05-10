define print_array
    print *($arg0)@(sizeof($arg0)/ sizeof($arg0[0]))
end

layout asm
layout reg
layout split 
target remote localhost:26000

file img.elf
b init_paging
b handle_supervisor_trap
