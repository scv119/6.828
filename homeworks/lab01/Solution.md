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

## Exercise 7:

Before setting up the page, 0x00100000 is the kernel code where as 0xf0100000 is empty. After executing the code and the paging is exampled, 
both 0x00100000 and 0xf0100000 are mapped to kernel execution code.  Since the relocate is a mapping to 0xf010002f, it failed to load if paging is not enabled.

## Exercise 8:

1. What funciton does console.c export? How is the function used by printf.c?

cputchar is exported, and it's called in printf as the function to write output.

2. Explain the follwing code from console.c:
...
It's write a new line and move the crt_pos to end of the truncated output if the output is longer than CRT_SIZE.

3. Trace the execution of following code step-by-step:
```
int x = 1, y = 3, z = 4;
cprintf("x %d, y %x, z %d\n", x, y, z);
```
In the call to cprintf(), to what does `fmt` point? To what does `ap` point?

fmt point to the address of ebp + 0x8 and ap point to address of ebp + 0xc.

The stack looks like

```
.
.
.
|z
|y
|x
|fmt pointer
|return adress （pushed when `call` assembly is called)
|ebp
v 
where stack grows.
```
List the function calls to cons_putc, va_arg and vcprintf. For cons_putc, list its arguments as well, For va_arg, list what ap points to before and after the call
Most importantly, the va_arg increase by 4 each time after the function call, that's how va_arg work to retrieve the list of argument in the stack.

4. TBD

5. In the following code, what is going to be printed after 'y='? (note: the answer is not a specific value.) Why does this happen?
```
cprintf("x=%d y=%d", 3);
```

It will tries to print the value in the stack before 3, which could be the ebp address of preivous function call or local variables defined in the stack.

6. The argements might need to point to the first argument of argument list instead of last one, and then va_start and va_end should decrease instead of increase.

## Excersize 9:
It's defined in following line in entry.S
```
 movl  $0x0,%ebp     # nuke frame pointer

 # Set the stack pointer
 movl  $(bootstacktop),%esp

```
It's located at .data section, whose virtual address should start with f0108000, the size is 4 * PGSIZE and the esp is point to the end (top) of the memory.

## Excersize 10

8 32-bit words does each recusive nesting level of `test_backtrace` push on the stack.
```
> gdb$ x/24x $esp
0xf010ff3c:     0xf0100068   # return address of test_backtrace
                0x00000000   # argument for test_backtrace     
                0x00000001   # following are the local variables used for cprintf and compare      
                0xf010ff78
0xf010ff4c:     0x00000000      
                0xf01008b1      
                0x00000002      
                0xf010ff78
...
```
