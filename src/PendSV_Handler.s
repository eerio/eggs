.syntax unified
.code 16
.globl led_on_delay
.globl led_off_delay
.globl PendSV_Handler
PendSV_Handler:
	push {r4, lr}
	bl led_on_delay
	bl led_off_delay
	pop {r4, pc}

