.syntax unified
.arch armv6-m

.text

.extern current_tcb, next_tcb
.globl PendSV_Handler

.thumb_func
PendSV_Handler:
	/* Disable IRQs */
	cpsid i
	mrs r0, psp
	stmia r0!, {r4-r7}
	push {r4-r7}
	pop {r4-r7}
	ldmia r0!, {r4-r7}

	/* SP changed when processor entered Handler mode, so get PSP */
	/* We can't use high registers in stm, so store low registers first.
	 * We will store r4-r11, so 8 registers = 32 bytes
	 * Sub 32 from PSP and store it already
	 */
	
	/* sub 32 from psp to make space for 8 regisetrs we wanna store */
	mrs r0, psp
	mov r1, r0
	subs r1, r1, #32
	/* save psp to tcb */
	ldr r3, =current_tcb
	ldr r2, [r3]
	str r1, [r2]
	/* now store r4-r7 */
	mov sp, r0
	push {r4-r7}
	/* move r8-r11 to r4-r7 */
	mov r4, r8
	mov r5, r9
	mov r6, r10
	mov r7, r11
	/* store r8-r11 */
	push {r4-r7}

	/* load new sp */	
	ldr r2, =next_tcb
	ldr r1, [r2]
	ldr r0, [r1]
	msr psp, r0
	/* load r8-r11 */
	pop {r4-r7}
	mov r8, r4
	mov r9, r5
	mov r10, r6
	mov r11, r7
	/* load r4-r7 */
	pop {r4-r7}

	/* return to Thread mode, take state from PSP, then use PSP as stack */
	ldr r0, =0xFFFFFFFD
	cpsie i
	bx r0

.size PendSV_Handler, .-PendSV_Handler

