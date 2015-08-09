#ifndef __MULTIBOOT_H
#define __MULTIBOOT_H

/* MULTIBOOT HEADER */
typedef struct multiboot {
	uint32_t flags;
	uint32_t lower_memory;
	uint32_t upper_memory;
	uint32_t boot_device;
	uint32_t command_line;
	uint32_t mods_count;
	uint32_t mods_address;
	uint32_t num;
	uint32_t size;
	uint32_t address;
	uint32_t shndx;
	uint32_t mmap_length;
	uint32_t mmap_address;
	uint32_t drives_length;
	uint32_t drives_address;
	uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;
} __attribute__((packed)) multiboot_t;

#endif
