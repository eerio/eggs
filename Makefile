CC = arm-none-eabi-gcc
GDB = arm-none-eabi-gdb
INC = inc
SRC = src
DEST = dest
LINKER = util/STM32F091RCTx_FLASH.ld
OOCD_SCR = /usr/share/openocd/scripts

INCLUDES = -I$(INC) -I$(INC)/CMSIS
COMMON_FLAGS = -std=c99 -mcpu=cortex-m0 -mlittle-endian -mthumb -DSTM32F091xC
COMP_FLAGS = $(COMMON_FLAGS) -g3 -O0 -Wall -Wextra $(INCLUDES) -c
LINK_FLAGS = $(COMMON_FLAGS) -T$(LINKER) -Wl,--gc-sections --specs=nosys.specs

C_SRCS = $(wildcard $(SRC)/*.c)
C_OBJS = $(patsubst $(SRC)/%.c, $(DEST)/%.o, $(SRCS))
ASM_SRCS = $(wildcard $(SRC)/*.s)
ASM_OBJS = $(patsubst $(SRC)/%.s, $(DEST)/%.o, $(SRCS))
DEPS = $(INC)/*.h $(INC)/CMSIS/*.h

$(C_OBJS): $(C_SRCS):
	$(CC) $(COMP_FLAGS) $< -o $@

$(ASM_OBJS): $(ASM_SRCS):
	$(CC) $(COMP_FLAGS) $< -o $@

$(DEST)/main.elf: $(C_OBJS) $(ASM_OBJS)
	$(CC) $(LINK_FLAGS) $^ -o $@

$(DEST)/main.hex: $(DEST)/main.elf
	objcopy -Oihex $(DEST)/main.elf $(DEST)/main.hex

.PHONY: con debug flash clean

con:
	openocd -s $(OOCD_SCR) -f $(OOCD_SCR)/board/st_nucleo_f0.cfg

debug:
	$(GDB) -x util/debug.gdb

flash:
	$(GDB) -x util/flash.gdb

clean:
	rm -rf $(DEST)

