.syntax unified
.cpu cortex-m0

.text
.globl PendSV_Handler

.thumb_func
PendSV_Handler:
	/* Disable interrupts - it's a critical code part */
	cpsid	i

	/*
	subs	r0, #16
	stmia	r0!,{r4-r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, r10
	mov	r7, r11
	subs	r0, #32
	stmia	r0!,{r4-r7}
	subs	r0, #16
	*/

	/* Save current task's SP: */
	ldr	r2, =current_tcb
	ldr	r1, [r2]
	mrs	r0, psp
	str	r0, [r1]

	/* Load next task's SP: */
	ldr	r2, =next_tcb
	ldr	r1, [r2]
	ldr	r0, [r1]
	msr	psp, r0

	/*
	ldmia	r0!,{r4-r7}
	mov	r8, r4
	mov	r9, r5
	mov	r10, r6
	mov	r11, r7
	ldmia	r0!,{r4-r7}
	*/

	/* Refer to ProgMan, p. 27 for documentation of branching
	 * to a magic number like this.
	 * 
	 * bx 0xFFFFFFFD:
	 *   - Return to Thread mode
	 *   - Set PSP as active SP
	 *   - Pop the new context from PSP
	 *
	 * Just before branching, enable interrupts again. This instruction
	 * has to be executed as late as possible, to minimize the possibility
	 * of another interrupt overtaking the control during context switching
	 */
	ldr r0, =0xFFFFFFFD
	cpsie	i	
	bx	r0

