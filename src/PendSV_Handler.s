.syntax unified
.cpu cortex-m0

.text

.globl PendSV_Handler
.thumb_func
PendSV_Handler:
	cpsid i
	cpsie i

