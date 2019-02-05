.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

.global PendSV_Handler

.type PendSV_Handler, %function
PendSV_Handler:
	/* Disable interrupts - it's a critical code part */
	cpsid i

	/* Save current task's SP: */
	ldr	r2, =current_tcb
	ldr	r1, [r2]
	mrs	r0, psp
	str	r0, [r1]

	/* Load next task's SP: */
	ldr	r1, =next_tcb
	ldr	r1, [r2]
	ldr	r0, [r1]
	msr	psp, r0
	
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
	cpsie i
	bx	lr

