.syntax unified

.data
gpioa: .word 0x48000000

.text
.thumb_func
.globl blink
.globl TIM2_IRQHandler
TIM2_IRQHandler:
	push {r0, r1, r2, lr}
	ldr r0, =gpioa
	ldr r0, [r0]
	ldr r1, [r0, #0x18]
	movs r2, #32
	orrs r1, r2
	str r1, [r0]
	pop {r0, r1, r2, pc}

