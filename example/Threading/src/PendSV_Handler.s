/* Context switch mechanism implementation for the Task Scheduler
 *
 * author: Pawel Balawender
 * https://github.com/eerio/eggs.git
 */
.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

/* Both current_tcb and next_tcb are defined in os.c file */
.extern current_tcb, next_tcb
.global PendSV_Handler

.type PendSV_Handler, %function
/* Notes:
 * Cortex-M0 in Thumb mode doesn't support:
 *  - STMDB instruction
 *  - STM without changing the value of base register: [!] option gotta be set
 *  - LDR/STR high registers (r8-r15) directly
 * Technical:
 *  - We have to store low registers first to enable us to transport hi regs
 *  - We have to load low at the end not to block them for transporting hi's
 *  Thus, store low => store high => load high => load low
 */
PendSV_Handler:
	/* Disable interrupts - it's a critical code part */
	cpsid i

	/* Save low registers higher on the stack */
	mrs	r0, psp

	/* Push r4-r7 */
	subs r0, #16
	stmia r0!, {r4-r7}

	/* Push r8-r11 */
	mov r4, r8
	mov r5, r9
	mov r6, r10
	mov r7, r11
	subs r0, #32
	stmia r0!, {r4-r7}
	
	/* Return to the original sp */
	adds r0, #16

	/* Save Process Stack Pointer to the current TCB */
	ldr	r2, =current_tcb
	ldr	r1, [r2]
	str	r0, [r1]

	/* Load PSP of the next task */
	ldr	r2, =next_tcb
	ldr	r1, [r2]
	ldr	r0, [r1]

	/* Load r8-r11 */
	subs r0, #32
	ldmia r0!, {r4-r7}
	mov r8, r4
	mov r9, r5
	mov r10, r6
	mov r11, r7	
	
	/* Load r4-r7 */
	ldmia r0!, {r4-r7}

	/* Set the correct PSP for NVIC to load the correct context */
	msr psp, r0
	
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
	cpsie i
	bx	r0

