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

The first instruction bits 32 is not an x86 assembly instruction, it's a directive to the NASM assembler that specifies it should generate code to run on a processor in 32-bit mode.
As good practice it is better to be explicit.

The text section is where we have our code.

global is another NASM directive to set symbols from source code as global.
so the linker knows where the symbol start is; which happens to be our entry point.

kmain is our function that will be defined in our kernel.c file. extern declares that the function is declared elsewhere.


Then, we have the start function that which calls the kmain function and halts, interrupts can awake the CPU from an hlt instruction.
so we have disabled interrupts beforehand using cli instruction, which is short for clear-interrupts.

Ideally we should set aside some memory for the stack and point the stack pointer esp to it.
We use the resb instruction, which reserves memory given in bytes, after which a label is left which will point to the edge of the reserved piece of memory.
Just before the kmain is called, the stack pointer (esp) is made to point to this space using the mov instruction.


#### ESP register
* The ESP register stores the memory address of the current top of the stack, and automatically updated by the CPU when data is pushed onto or popped off the stack.
* Used for temporary storage of data during program execution, The stack is a last-in-first-out data structure.
* Sees GRUB does this for us, and the stack pointer is already set at this point

Just to be sure, we allocate some space in the BSS section and point the stack pointer to the beginning of the allocated memory.
#### BSS section
* Block Started by Symbol
* Section of memory in an executable that contains uninitialized data.
* Used typically to store global and static variables that are initialized to zero or null.
* When the OS allocates memory for the BSS section it sets all the bytes to zero








##### LIFO data structure example (ESP register)
```c
// LIFO data structure example in C
#include <stdio.h>
#include <stdlib.h>

// define a struct for the linked list node
typedef struct Node {
        int data;
        struct Node* next;
} Node;

// define a struct for the LIFO stack
typedef struct Stack {
    Node* top;
} Stack;

// initialize an empty stack
void init(Stack* s) {
    s->top = NULL;
}

// check if the stack is empty
int isEmpty(Stack* s) {
    return s->top == NULL;
}

// push an element onto the stack
void push(Stack* s, int data) {
    // allocate a new node
    Node* newNode = (Node*) malloc(sizeof(Node));
    if (newNode == NULL) {
            fprintf(stderr, "Error: memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        // set the node data and next pointer
        newNode->data = data;
        newNode->next = s->top;
        // set the new node as the top of the stack
        s->top = newNode;
}

// pop an element off the stack
int pop(Stack* s) {
    if (isEmpty(s)) {
            fprintf(stderr, "Error: stack is empty\n");
            exit(EXIT_FAILURE);
        }
        // save the top node and data
        Node* topNode = s->top;
        int data = topNode->data;
        // set the next node as the new top of the stack
        s->top = topNode->next;
        // free the top node
        free(topNode);
        // return the popped data
        return data;
}


// print the stack contents
void printStack(Stack* s) {
    printf("Stack: ");
    Node* node = s->top;
    while (node != NULL) {
            printf("%d ", node->data);
            node = node->next;
        }
        printf("\n");

}
```


### The kernel in C
In kernel.asm, we called the function kmain(), so the C code will start executing at kmain()

```c
// kernel.c
```
All the kernel will do is clear the screen and write the string we specified.

First, we made a pointer vidptr to point to the address 0xb8000.
This address is the start of video memory in protected mode, The screen's text memory is simply a chunk of memory in our address space.
The memory mapped input/output for the screen starts at 0xb8000 and supports 25 lines, each line contain 80 ascii characters.

Each character element in this text memory is represented by 16 bits (2 bytes), the first byte should have the representation of the character as in ASCII.
The second byte is the attribute-byte; this describes the formatting of the character including attributes such as color.

Printing a character 's' in green color on a black background, we will store the character 's' in the ifrst byte of the video memory address and the value 0x02 in the second byte.
Where 0 represents black background and 2 represents green foreground.

In our kernel, wel will use light grey character on a black background. value = 0x07

In the first while loop, the program writes the blank character with 0x07 attribute all over the 80 columns of the 25 lines. clearing the screen.
In the second while loop, characters of the null terminated string "my first kernel, thank you arjunsreedharan" are written to the chunk of video memory with each character holding the attribute-byte of 0x07.

#### The linking part
We will assemble ```kernel.asm``` with NASM to an object file; then using GCC we will compile ```kernel.c``` to another object file.
Now we need to get those objects linked to an executable bootable kernel.

Using a explicit linker script, it can be passed as an agrument to ```ld```

```ld
 link.ld
```
First we set the output format of our output executable to be 32-bit ELF [Executable and Linkable Format].
ELF is the standard binary file format for the Unix-like systems on x86 arch.

ENTRY takes on argument, it specifies the symbol name that should be the entry point of our executable.
SECTIONS is the most important part, defining the layout of our executable. We could specify how the different sections are to be merged and at what location each of these is to be placed.

Within the braces that follow the SECTIONS statement, the period character (.) represents the location counter.
The location counter is always initialized at 0x0 at the beginning of the SECTIONS block.

Remember, earlier I told you that kernel's code should start at the address 0x1000000.

