.syntax unified
.cpu arm7tdmi
.arm
.section .text

.global stringLength
.type stringLength, %function
stringLength:
    mov r2, #0               @ Initialize length counter
1:
    ldrb r3, [r0], #1        @ Load byte, increment pointer
    cmp r3, #0               @ Compare with null terminator
    beq 2f                   @ Break if null terminator
    add r2, r2, #1           @ Increment length
    b 1b                     @ Repeat loop
2:
    mov r0, r2               @ Return length in r0
    bx lr                    @ Return to caller

.global IntToASCII
.type IntToASCII, %function
IntToASCII:
    push {r4, lr}            @ Save registers
    ldr r4, =buffer          @ Load buffer address
    add r4, r4, #11          @ Point to end of buffer
    mov r3, #0               @ Initialize buffer with null terminator
    strb r3, [r4, #-1]!      @ Add null terminator at the end
    cmp r0, #0               @ Check if value is zero
    bne 1f
    ldr r0, =zeroString      @ Return "0" if value is 0
    b 2f
1:
    mov r3, #10              @ Base 10
    mov r1, r0               @ Copy value to r1
    mov r0, #0               @ Initialize digit counter
2:
    cmp r1, #0               @ Check if remaining value is zero
    beq 3f                   @ Break if no value remains
    bl divideAndRemainder    @ Divide r1 by 10, result in r1 (quotient), r2 (remainder)
    add r2, r2, #48          @ Convert remainder to ASCII
    strb r2, [r4, #-1]!      @ Store digit
    b 2b                     @ Repeat for next digit
3:
    mov r0, r4               @ Return pointer to start of number
    pop {r4, pc}             @ Restore registers and return

.global divideAndRemainder
.type divideAndRemainder, %function
divideAndRemainder:
    mov r2, #0               @ Initialize remainder
1:
    cmp r0, #10              @ Compare value with base
    blt 2f                   @ If less, finish division
    sub r0, r0, #10          @ Subtract base
    add r2, r2, #1           @ Increment quotient
    b 1b                     @ Repeat
2:
    bx lr                    @ Return to caller

.global HexString
.type HexString, %function
HexString:
    push {r4, lr}            @ Save registers
    ldr r3, =hexBuf          @ Load buffer address
    ldr r2, =hexChars        @ Load hex character table
    add r3, r3, #4           @ Start at end of buffer
    mov r1, #0               @ Initialize
    strb r1, [r3, #-1]!      @ Null terminate
    mov r4, #4               @ 4 hex digits
1:
    lsr r1, r0, #4           @ Shift value to isolate nibble
    and r1, r1, #0xF         @ Mask nibble
    add r1, r1, r2           @ Add to lookup base
    ldrb r1, [r1]            @ Get hex character
    strb r1, [r3, #-1]!      @ Write to buffer
    sub r4, r4, #1           @ Decrement digit counter
    bne 1b                   @ Repeat for all digits
    mov r0, r3               @ Return pointer to buffer
    pop {r4, pc}             @ Restore registers and return

.section .rodata
hexChars:
    .asciz "0123456789ABCDEF"
buffer:
    .space 12                @ Buffer for IntToASCII
zeroString:
    .asciz "0"
hexBuf:
    .space 5                 @ Buffer for HexString
