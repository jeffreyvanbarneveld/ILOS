#ifndef __MULTIBOOT_H
#define __MULTIBOOT_H

/* The multiboot header that is passed to us has this structure */ 
typedef struct 
{ 
    uint32_t flags;                	/* Flags */
    uint32_t mem_low;              	/* The amount of lower memory */
    uint32_t mem_hi;               	/* The amount of higher memory */
    uint32_t boot_device;          	/* Device that booted us */
    uint32_t cmd_line;             	/* Kernel command line */
    uint32_t mods_count;           	/* How many modules were loaded */
    uint32_t mods_addr;            	/* Module info addresses */
    uint32_t num;                 	/* ELF: Number of symbols */
    uint32_t size;                 	/* ELF: size */
    uint32_t addr;                 	/* ELF: address */
    uint32_t shndx;                	/* ELF: index of string table */
    uint32_t mmap_len;             	/* Length of the memory map */
    uint32_t mmap_addr;            	/* Address of the memory map */
    uint32_t drives_len;           	/* Drive info buffer length */
    uint32_t driver_addr;          	/* Address of the drives info */
    uint32_t config_table;         	/* ROM configuration table */
    uint32_t bootloader_name;      	/* The name of the bootloader */
    uint32_t apm_table;            	/* APM table */
    uint32_t vbe_ctrl_info;        	/* VBE control info */
    uint32_t vbe_mode_info;        	/* Information about the current VBE mode */
    uint32_t vbe_mode;             	/* Current VBE mode number */
    uint32_t vbe_interface_seg;    	/* Used for protected mode interface */
    uint32_t vbe_interface_off;    	/* Used for protected mode interface */
    uint32_t vbe_interface_len;    	/* Used for protected mode interface */
} __attribute__((packed)) multiboot_t;

/* Structure of a module */ 
typedef struct 
{ 
    uintptr_t mod_start;            /* The start address of this module */
    uintptr_t mod_end;              /* The end address of this module */
    uintptr_t cmd_line;             /* The command line of this module */
    uint32_t  padding;              /* Padding to take this structure to 16 bytes, these bytes are all zeroes */
} __attribute__((packed)) multiboot_module_t;

#endif