Date: Jan 2019
License: GNU GPL v3

How to use it?
Take any file from example/, rename it to main.c and put into src/
Compile by `make`, then run OpenOCD server by `make con` and burn
the program into your uC's memory by `make flash` - it will open a
GDB console. Enter there a `continue` command and it will run

What advantages does it have?
It does not use ST's HAL, it's all written in CMSIS bare bones

Features of the 'os':
-simple kernel threads abstraction
-(SysTick IRQ)-based round-robin scheduler
-O(1) context switch in the PendSV IRQ handler
-SPI abstraction layer
-SDHC card driver
-FAT32 FS support by porting ChaN's Petit FATFS module
-Cool semihosting features

Development goals:
-rtos scheduler
-hold tasks' statuses in a bitfield
-(CLZ instruction)-based scheduler
-syscall support with semihosting coverage
-ror+clz -based queue implementation
-hd44780 i2c driver
-esp32 interface for networing services
-smartcard driver

