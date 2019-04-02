target remote localhost:3333
monitor arm semihosting enable
file dest/main.dbg
monitor reset halt
