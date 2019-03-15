# eggs

eggs is an Operating System created for STM32 development boards with Cortex-M0 processors.  
Status: under development since 17 I 2019  
----

# Features:
  - Round-robin task-scheduler based on SysTick interrupt  
  - O(1) context switching in the PendSV interrupt  
  - SPI interface and initialization procedures  
  - SDHC card driver
  - FAT32 File System support, thanks to [ChaN's Petit FATFS Module](http://elm-chan.org/fsw/ff/00index_p.html)
  - Semihosting features
----
# Development goals:  
  - Thread-based SPI module with smart DMA configuration allowing to implement non-blocking I/O over SPI
  - Syscalls-based threading model
  - Processes support
  - HD44780 LED display driver via I2C I/O expander  
  - ESP32 interface for the access to networking services  

----
### Tech
The eggs project uses a few open source technologies to work properly:  
GNU ARM EABI toolchain  
GDB debugger  
OpenOCD  
CMSIS libraries  
[dillinger.io](https://dillinger.io/) to develop this stunning README file  
The eggs project does **not** use neither ST's nor ARM's HAL libraries!  

----
### License
GNU GPL v3.0  
