# LeOS-V 
## Memory management
Parts:
- Frame allocator
- Page allocator
### Where does the heap live?
End of kernel stack/sections until the end of RAM (probably done through linker script)
### Linker (cont)
- Mark end of RAM
- Place marker at the end of all other sections
- Align everything on a 4K basis
