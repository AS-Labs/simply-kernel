# simply-kernel
Following Kernel 101 https://arjunsreedharan.org/post/82710718100/kernels-101-lets-write-a-kernel


## Hello world
A simple kernel, which could be loaded with GRUB bootloader on x86 systems.


## How does an x86 machine boot

Most registers of the x86 CPU have well defined values after power-on.

### The Instruction Pointer (EIP)
The EIP is a register in the x86 CPU architecture that holds the memory address of the next instruction to be executed.
It is also known as the Program Counter in other CPU architectures.

The EIP register is a 32-bit x86 CPU and a 64 bit register on 64-bit CPUs.
When the CPU fetches an instruction to execute, it reads the value of the EIP to determine the memory address of the instruction to fetch.
Once fetched, the EIP register is updated to point to the next instruction to be executed.

The EIP is hardcoded to the value 0xFFFFFFF0.. (16777208)?

Therefore the CPU is hardwired to begin execution at the physical address of 0xFFFFFFF0.
The last 16 bytes of the 32-bit address space, this memory address is called reset vector.

Reset vector:
Points to the location in memory where the CPU begins executing code when the system is powered on or reset.
Also referred to as the boot vector, or the reset address.
It is always the first code to run on the system and is responsible for initializing the system and loading the OS or firmware,



