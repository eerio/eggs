target remote localhost:3333
file dest/main.elf
monitor reset halt
monitor flash write_image erase dest/main.hex
monitor reset halt

