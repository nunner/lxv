# LeOS
This is a simple kernel I'm writing while following several tutorials for teaching myself low-level concepts.

## Build
First of all, build a gcc riscv64-unknown toolchain. I'm not going to explain that here, there are enough resources on the internet. 

Then, to run it through qemu, just run 
```
make qemu
```

# TODO
- Implement page alignment in the heap allocator
- Move end header when resizing heap
