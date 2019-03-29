# eggs  
eggs is an Operating System created for STM32 development boards with Cortex-M0 processors.  
Status: under development since **17 I 2019**  

## How to use:
Take a file from **example** directory, rename it to main.c and place it in the **src** directory.
Compile using *make*, then run the OpenOCD server by *make con* and program the board by *make flash*. In the new GDB console, start execution by sending to it *continue* command.

## Features:
  - Round-robin task-scheduler based on SysTick interrupt  
  - O(1) context switching in the PendSV interrupt handler  
  - SPI interface and initialization procedures  
  - SDHC card driver
  - FAT32 File System support, thanks to [ChaN's Petit FATFS Module](http://elm-chan.org/fsw/ff/00index_p.html)
  - Semihosting features

## Development goals:  
  - Keeping it RTOS by not blocking interrupts - beyond kernel threads, software will be able to use interrupts to obtain deterministic execution time of the IRQ handler, which is guaranteed by Cortex-M0 architecture
  - Support for 128 avaliable kernel threads on 4 priority levels stored as 4 words, so the whole array can be loaded to the scheduler without popping it from the stack
  - O(1) Task Scheduler taking advantage of CLZ instruction (not avaliable on ARMv6-M processors - there the scheduler will be O(n))
  - Thread-based SPI module with smart DMA configuration allowing to implement SPI I/O with no wasted cycles while waiting for the DMA / second device to finish / respond; It will change DMA channels CMAR and CNDTR registers and then **yield** the control to OS, so only a few processor cycles will be required to send even a large chunk of data
  - Syscalls support: full semihosting coverage, User LED (LD2) and User Button (B1) interfaces, system settings managers - clocks & flash latency etc.
  - Queue implementation without branching nor modulo operation, thanks to ROR and CLZ instructions, and using LDMDB instruction to quickly load the wanted values from the queue
  - Processes
  - time.h methods w/ synchronization with the host possibility by the semihosting interface 
  - HD44780 LED display driver via I2C I/O expander  
  - ESP32 interface for the access to networking services  
  - Bootloader extension to load code from a SD card 
  - Smartcard controler
  - thread-safe malloc


## Tech:
GNU ARM EABI toolchain  
OpenOCD  
CMSIS libraries  
[dillinger.io](https://dillinger.io/) to develop this README file  
The eggs project does **not** use neither ST's nor ARM's HAL libraries!  


## License:
GNU GPL v3.0  
