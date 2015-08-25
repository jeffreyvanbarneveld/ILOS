# X86 binary
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
C_FLAGS		:=  -I./include -std=gnu99 -masm=intel -m32 -Wall -O -fno-stack-protector -finline-functions -fno-builtin
L_FLAGS		:= -m elf_i386
A_FLAGS		:= -f elf32

ifeq ($(OS),Windows_NT)
   FixPath = $(subst /,\,$1)
   RM = rm
   DellAll = $(RM) build/*.o
   Nasm = "tools/Nasm/nasm.exe"
   Linker = "tools/gcc/bin/i586-elf-ld.exe"
   GCC = "tools/gcc/bin/i586-elf-gcc.exe"
   CP = cp
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
	  DellAll = $(RM) build/*.o
      FixPath = $1
	  Nasm = nasm
	  Linker = ld
	  GCC = gcc
	  CP = cp
   endif
endif

all: startMsg kernel.elf clean	
	@echo Finished compiling kernel

startMsg:
	@echo Begin compiling kernel

sync:
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/stack.c -O source/stack.c -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/runtime.c -O source/runtime.c -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/ops.h -O include/ops.h -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/runtime.h -O include/runtime.h -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/stack.h -O include/stack.h -q --no-check-certificate
	@wget https://raw.githubusercontent.com/NielsDev/IL2VM/master/C/include/binarystructs.h -O include/binarystructs.h -q --no-check-certificate
	
kernel.elf: $(OBJ_FILES) 
	@echo Linking kernel...
	@$(Linker) -T link.ld $(L_FLAGS) -o build/kernel.bin $(OBJ_FILES_BUILD)
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
