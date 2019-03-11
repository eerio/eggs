.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

.extern SystemCoreClock
.global delay

/* In r0 there is an argument, i.e. unsigned int n
 * T(delay, n) = 12 + 4n cycles, including [BL delay] instr.
 * interrupts are disabled during execution of delay, so
 * it's totally reliable and precise
 * ref: TechMan, p. 37
 */
.type delay, %function
delay:
    cpsid i
    adds r0, #1
loop:
    subs r0, #1
    bne loop
end:
    cpsie i
    bx lr

