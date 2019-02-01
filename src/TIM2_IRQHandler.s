.syntax unified
.arch armv6-m

.globl thread_1
.globl thread_2
.globl TIM2_IRQHandler

.data
gpioa: .word 0x48000000
current: .word 0

.bss
.align 4
tcb_a: .space 1
.align 4
tcb_b: .space 1

.text
.code 16
TIM2_IRQHandler:
	/* store context to memory */
	push {r0-r7, lr}
	mov sp, r0
	bne thr_b
	ldr r1, =tcb_a_addr
thr_b:
	ldr r1, =tcb_b_addr
	str r0, [r1]

	/* change executed thread */
	ldr r0, =current_addr
	ldr r1, [r0]
	movs r2, #1
	eors r1, r1, r2
	str r1, [r0]

	/* load context */
	bne thr_b_s
	ldr r0, =tcb_a_addr
thr_b_s:
	ldr r0, =tcb_b_addr
	mov r0, sp
	pop {r0-r7, pc}

current_addr: .word current
tcb_a_addr: .word tcb_a
tcb_b_addr: .word tcb_b

