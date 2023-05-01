# simply-kernel
Following Kernel 101 https://arjunsreedharan.org/post/82710718100/kernels-101-lets-write-a-kernel


## Hello world
A simple kernel, which could be loaded with GRUB bootloader on x86 systems.


## How does an x86 machine boot

Most registers of the x86 CPU have well defined values after power-on.

### Boot process
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

The chipset's memory map makes sure that the reset vector is mapped to a certain part of the BIOS not the RAM, meanwhile the BIOS copies itself to the RAM for faster access (shadowing)
The address 0xFFFFFFF0 will contain a jump instruction to the address in memory where the BIOS has copied itself.


Now the BIOS code starts executing, first searches for a bootable device in the configured boot device order, it checks for a cetain magic number to determine if the device is bootable or not (whether bytes 511 and 512 of first sector are 0xAA55 (43785)?

Once it has found a bootable device, it copies the contents of the device's first sector into RAM starting from physical address 0x7c00 (31744)?
then jumps into the address and executes the code just loaded, this code is the bootloader.

The bootloader then loads the kernel at the physical address 0x100000. The address is used as the start-address for all big kernels on x86 machines.

All x86 processors begin in a simplistic 16-bit mode called real mode, the GRUB bootloader makes the switch to 32-bit protected mode by setting the lowest bit of CR0 register to 1.
#### real mode
* Makes the CPU behave like an older intel 8086 processor, it is a 16-bit operating mode.
* In real mode the CPU can only address 1MB of memory and can only execute 16-bit instructions. it provides direct access to the hardware, as well as access to BIOS. making it well-suited for bootstrapping the OS.
* The CPU uses segmentation to address memory, each segment being 64KB in size, (CS,DS,SS and ES)
* Provides access to the interrupt vector table (IVT) which contains the addresses of the interrupt handlers used bu the BIOS.
* Provides support for real-time clock, keyboard and mouse inputs

Note that GRUB detects linux boot protocol and loads linux kernel in real mode, Linux kernel itself makes the switch to protected mode.



## Prerequisies 
* x86 PC
* Linux
* NASM assembler
* gcc
* ld (GNU Linker)
* grub

original author's source code ("https://github.com/arjun024/mkernel") , Thank you!




## Entry point using assembly

We cannot avoid a little bit of assembly, the assemply will invoke an external function in C then halt the program flow.

### How we make sure that assembly code will serve as the starting point of the kernel?
We will use a linker script that links the object files to produce the final kernel executable.
In the linker script, we will explicitly specify that we want our binary to be loaded at the address 0x100000.
This address is where the kernel is expected to be.


```nasm
;;kernel.asm
```



