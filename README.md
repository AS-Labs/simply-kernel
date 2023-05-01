# simply-kernel
Following Kernel 101 https://arjunsreedharan.org/post/82710718100/kernels-101-lets-write-a-kernel


## Hello world
A simple kernel which could be loaded with GRUB bootloader on x86 systems


## How does an x86 machine boot

Most registers of the x86 CPU have well defined values after power-on.
### The Instruction Pointer (EIP)
The EIP is a register in the x86 CPU architecture that holds the memory address of the next instruction to be executed.
It is also known as the Program Counter in other CPU architectures.

The EIP register is a 32-bit x86 CPU and a 64 bit register on 64-bit CPUs.
When the CPU fetches an instruction to execute, it reads the value of the EIP to determine the memory address of the instruction to fetch.
Once fetched, the EIP register is updated to point to the next instruction to be executed.


