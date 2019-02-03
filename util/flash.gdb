target remote localhost:3333
file dest/main.dbg
monitor reset halt
monitor flash write_image erase dest/main.elf
monitor reset halt

