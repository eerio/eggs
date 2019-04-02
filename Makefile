# Compiler and debugger commands
CC = arm-none-eabi-gcc
GDB = arm-none-eabi-gdb

# Used directories' names
INC = inc
INC_CMSIS = $(INC)/CMSIS
SRC = src
DEST = dest
UTIL = util

# OpenOCD scripts' location
OOCD_SCR = /usr/share/openocd/scripts

# Path to the linker script
LINKER = util/STM32F091RCTx_FLASH.ld

# Build options
INCLUDES = -I$(INC) -I$(INC_CMSIS)
COMMON_FLAGS = -mcpu=cortex-m0 -mthumb -mlittle-endian -DSTM32F091xC
COMP_FLAGS = $(COMMON_FLAGS) -std=c99 -g3 -O0 -Wall -Wextra $(INCLUDES) -c
LINK_FLAGS = $(COMMON_FLAGS) -T$(LINKER) -Wl,--gc-sections --specs=nosys.specs

# Collecting source code files
_srcs_c = $(wildcard $(SRC)/*.c)
_srcs_s = $(wildcard $(SRC)/*.s)
OBJS = $(patsubst $(SRC)/%.c, $(DEST)/%.o, $(_srcs_c)) $(patsubst $(SRC)/%.s, $(DEST)/%.o, $(_srcs_s))
DEPS = $(INC)/*.h $(INC)/CMSIS/*.h

# Compile the whole project as the default "make" target
# Separate debugging symbols' table from the executable itself
$(DEST)/main.hex: $(DEST)/main.elf
	arm-none-eabi-objcopy -Oihex $< $@
	arm-none-eabi-strip --strip-debug --strip-unneeded $@ -o $@
$(DEST)/main.dbg: $(DEST)/main.elf
	arm-none-eabi-objcopy --only-keep-debug $< $@

# Link .o files
$(DEST)/main.elf: $(OBJS)
	$(CC) $(LINK_FLAGS) $^ -o $@

# Compile every source file
$(DEST)/%.o: $(SRC)/%.c $(INC)/%.h
	$(CC) $(COMP_FLAGS) $< -o $@
$(DEST)/%.o: $(SRC)/%.s $(INC)/%.h
	$(CC) $(COMP_FLAGS) $< -o $@

# These 2 files are kidna exceptional - we don't need headers for them
$(DEST)/main.o: $(SRC)/main.c
	$(CC) $(COMP_FLAGS) $< -o $@
$(DEST)/startup_stm32f091xc.o: $(SRC)/startup_stm32f091xc.s
	$(CC) $(COMP_FLAGS) $< -o $@

.PHONY: con debug flash clean
con:
	openocd
debug: $(DEST)/main.dbg
	$(GDB) -x $(UTIL)/debug.gdb
flash: $(DEST)/main.hex $(DEST)/main.dbg
	$(GDB) -x $(UTIL)/flash.gdb
clean:
	rm -f $(DEST)/*

