Lab 1

[Link to Lab 1](https://pdos.csail.mit.edu/6.828/2014/labs/lab1/)

## Exercise 3:

- At what point does the processor start executing 32-bit code? What exactly causes the switch from 16- to 32-bit mode?
  line 55 of boot.S, where `ljmp $PROT_MOD_CSEG, $protcseg`
- What is the last instruction of the boot loader executed, and what is the first instruction of the kernel it just loaded?
  call ....
  Last instruction of the boot loader executed:
  `0x7d6b:      call   DWORD PTR ds:0x10018`
  First instruction of the kernel it just loaded:
  `0x7d6b:      call   DWORD PTR ds:0x10018`

- Where is the first instruction of the kernel?
  entry.S：44
- How does the boot loader decide how many sectors it must read in order to fetch the entire kernel from disk? Where does it find this information?
  The `fileoffset` and `size` offset in elf header for seach segment.

## Exercise 4:
