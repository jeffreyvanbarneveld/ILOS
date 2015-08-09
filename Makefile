# ARM binary
SOURCE_DIR	:= source/
BUILD_DIR	:= build/

# find source files
ASM_FILES	:= $(wildcard $(SOURCE_DIR)*.asm)
C_FILES 	:= $(wildcard $(SOURCE_DIR)*.c)

# make object files from found files
OBJ_FILES 	:= $(patsubst %.asm,%.o, $(ASM_FILES))
OBJ_FILES 	+= $(patsubst %.c,%.o, $(C_FILES))
OBJ_FILES_BUILD := $(patsubst source/%.o,$(BUILD_DIR)%.o, $(OBJ_FILES))

# compile flags
C_FLAGS		:=  -I./include -std=gnu99 -nostdlib -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin
L_FLAGS		:= -m elf_i386
A_FLAGS		:= -f elf32

ifeq ($(OS),Windows_NT)
   FixPath = $(subst /,\,$1)
   RM = rm -f
   DellAll = $(RM) $(OBJ_FILES_BUILD) kernel.elf
   Nasm = "tools/Nasm/nasm.exe"
   Linker := "tools/gcc/bin/i586-elf-ld.exe"
   gcc_cmd := "tools/gcc/bin/i586-elf-gcc.exe"
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
	  DellAll = $(RM) $(OBJ_FILES_BUILD) kernel.elf
      FixPath = $1
	  Nasm = nasm
	  Linker = ld
	  GCC = gcc
   endif
endif

all: startMsg kernel.img	
	@echo Finished compiling kernel.img

startMsg:
	@echo Begin compiling kernel.img

kernel.elf: $(OBJ_FILES) 
	@echo Linking kernel...
	@$(Linker) -T link.ld $(L_FLAGS) -o build/kernel.bin $(OBJ_FILES_BUILD)
	cp build/kernel.bin E:\

kernel.img: kernel.elf
	@echo cleaning up...
	@$(DellAll)

clean:
	@echo cleaning up...
	@$(RM) $(OBJ_FILES_BUILD) kernel.elf

%.o: %.asm Makefile
	@echo Assembling $<...
	@$(Nasm) $(A_FLAGS)  $< -o $(patsubst source/%.o,$(BUILD_DIR)%.o, $@)


%.o: %.c Makefile
	@echo Compiling $<..
	@$(gcc_cmd) $(C_FLAGS) -c $< -o $(patsubst source/%.o,$(BUILD_DIR)%.o, $@)
