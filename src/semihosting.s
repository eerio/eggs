.cpu cortex-m0
.fpu softvfp
.thumb

.global send_command

.type send_command, %function
send_command:
    bkpt 0xAB

