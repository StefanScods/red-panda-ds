.arm

.global main
.type main, %function

.equ DMA0SAD,  0x040000B0
.equ DMA0DAD,  0x040000B4
.equ DMA0CNT,  0x040000B8

.equ SOURCE,   0x02001000
.equ DEST,     0x02001010

main:
    mov sp, #0x02300000
    @ Write test data to source
    ldr r0, =SOURCE
    ldr r1, =0x12345678
    str r1, [r0]
    ldr r1, =0xAABBCCDD
    str r1, [r0, #4]
    mov r1, #0x11
    str r1, [r0, #8]
    mov r1, #0x22
    str r1, [r0, #12]

    @ Configure DMA source
    ldr r0, =DMA0SAD
    ldr r1, =SOURCE
    str r1, [r0]

    @ Configure DMA destination
    ldr r0, =DMA0DAD
    ldr r1, =DEST
    str r1, [r0]

    @ Configure DMA:
    @ - Enable (bit 31)
    @ - Start immediately (bits 27-28 = 00)
    @ - 32-bit transfer (bit 26)
    @ - Transfer 4 words
    ldr r0, =DMA0CNT
    ldr r1, =0b10000100000000000000000000000100
    str r1, [r0]

    @ End the test.
    mov r0, #1
    b test_endTest

.include "common.s"
