.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

.global start_os
.type start_os, %function
start_os:
/* Load &TCB to r2 */
	ldr r2, =current_tcb
/* Load &TCB->SP to r1*/
	ldr r1, [r2]
/* Load TCB->SP to r0 and set as PSP to get the new context from */
	ldr r0, [r1]
	msr psp, r0
/* Return to Thread mode, get context from PSP, use PSP as SP */
	ldr r0, =0xFFFFFFFD
	bx r0

