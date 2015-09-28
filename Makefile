# Sources
SOURCE_DIR	:= source/
VM_DIR		:= source/vm/
BUILD_DIR	:= build/

# find source files
ASM_FILES	:= $(wildcard $(SOURCE_DIR)*.asm)
C_FILES 	:= $(wildcard $(SOURCE_DIR)*.c)
C_FILES		+= $(wildcard $(VM_DIR)*.c)
C_FILES		+= $(wildcard $(VM_DIR)runtime/*.c)

# make object files from found files
OBJ_FILES 		:= $(patsubst %.asm,%.o, $(ASM_FILES))
OBJ_FILES 		+= $(patsubst %.c,%.o, $(C_FILES))
OBJ_FILES_BUILD := $(patsubst source/%.o,$(BUILD_DIR)%.o, $(OBJ_FILES))

# compile flags
C_FLAGS		:= -I./include -I./include/vm -std=gnu99 -masm=intel -m32 -Wall -O -pedantic -nostdlib
L_FLAGS		:= -m elf_i386 -s
A_FLAGS		:= -f elf32

ifeq ($(OS), Windows_NT)
   RM = rm -rf
   DellAll = $(RM) build\*.o
   Nasm = "tools/Nasm/nasm.exe"
   Linker = "tools/gcc/bin/i586-elf-ld.exe"
   GCC = "tools/gcc/bin/i586-elf-gcc.exe"
   CP = cp
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
	  DellAll = $(RM) build/*.o
	  Nasm = nasm
	  Linker = ld
	  GCC = gcc
	  CP = cp
   endif
endif

# find libgcc
LIBGCC := $(shell $(GCC) -print-libgcc-file-name)

all: startMsg kernel.elf clean
	@echo Finished compiling kernel

startMsg:
	@echo Begin compiling kernel

sync:
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/src/stack.c -O source/vm/stack.c -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/src/vm.c -O source/vm/vm.c -q --no-check-certificate

	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/src/runtime/system_globalization.c -O source/vm/runtime/system_globalization.c -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/src/runtime/system_number.c -O source/vm/runtime/system_number.c -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/src/runtime/system_object.c -O source/vm/runtime/system_object.c -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/src/runtime/system_string.c -O source/vm/runtime/system_string.c -q --no-check-certificate

	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/ops.h -O include/vm/ops.h -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/vm.h -O include/vm/vm.h -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/runtime.h -O include/vm/runtime.h -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/stack.h -O include/vm/stack.h -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/binarystructs.h -O include/vm/binarystructs.h -q --no-check-certificate
	
kernel.elf: $(OBJ_FILES) 
	@echo Linking kernel...
	@$(Linker) -T link.ld $(L_FLAGS) -o build/kernel.bin $(OBJ_FILES_BUILD) $(LIBGCC)
	@$(CP) build/kernel.bin V:\

clean:
	@echo cleaning up...
	@$(DellAll)

%.o: %.asm Makefile
	@echo Assembling $<...
	@$(Nasm) $(A_FLAGS)  $< -o $(patsubst source/%.o,$(BUILD_DIR)%.o, $@)

%.o: %.c Makefile
	@echo Compiling $<..
	@$(GCC) $(C_FLAGS) -c $< -o $(patsubst source/%.o,$(BUILD_DIR)%.o, $@)
