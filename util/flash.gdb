target remote localhost:3333
file dest/main.dbg
monitor arm semihosting enable
monitor reset halt
monitor flash write_image erase dest/main.hex
monitor reset halt

