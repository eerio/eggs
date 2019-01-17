#!/usr/bin/env bash

cd ..
arm-none-eabi-gcc -std=c99 -mcpu=cortex-m0 -mthumb -mlittle-endian \
    -DSTM32F091xC -ggdb -O0 -Wall -Wextra -Iinc -Iinc/CMSIS \
    -c util/empty_main.c -o dest/empty_main.o
arm-none-eabi-gcc -std=c99 -mcpu=cortex-m0 -mthumb -mlittle-endian \
    -DSTM32F091xC -Tutil/STM32F091RCTx_FLASH.ld -Wl,--gc-sections \
    --specs=nosys.specs dest/empty_main.o -o dest/empty_main.elf

