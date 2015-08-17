BITS 32
ALIGN 4

global start
extern kmain

SECTION .multiboot
mboot:
    MULTIBOOT_PAGE_ALIGN    equ 1 << 0
    MULTIBOOT_MEMORY_INFO   equ 1 << 1
    MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
    MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0

SECTION .text
start:
    ; Set stack
    mov esp, _sys_stack

    ; Pass magic and header to kmain
    push eax
    push ebx
    call kmain

    ; Gets here if unexpected end
    cli
    hlt

SECTION .bss
    resb 8192

_sys_stack: