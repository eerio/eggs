.text
.globl TIM2_IRQHandler

TIM2_IRQHandler:
	mov r0, GPIOA
	ldr r1, [r0]
	bx lr

