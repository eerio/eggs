target remote localhost:3333
file dest/main_big.elf
monitor reset halt
monitor flash write_image erase dest/main.hex
monitor reset halt

