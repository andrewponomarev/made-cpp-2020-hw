.text
.global count_det

count_det:
    push {r4, r5, r6, r7}
    mov r1, #1
    mov r2, #1
    mov r3, #1
    mov r4, #1
    mov r5, #1
    mov r6, #1

    ldrsb r7, [r0], #4
    mul r1, r7
    mul r6, r7
    ldrsb r7, [r0], #4
    mul r2, r7
    mul r5, r7
    ldrsb r7, [r0], #4
    mul r3, r7
    mul r4, r7

    ldrsb r7, [r0], #4
    mul r3, r7
    mul r5, r7
    ldrsb r7, [r0], #4
    mul r1, r7
    mul r4, r7
    ldrsb r7, [r0], #4
    mul r2, r7
    mul r6, r7

    ldrsb r7, [r0], #4
    mul r2, r7
    mul r4, r7
    ldrsb r7, [r0], #4
    mul r3, r7
    mul r6, r7
    ldrsb r7, [r0], #4
    mul r1, r7
    mul r5, r7

    mov r0, r1
    add r0, r2
    add r0, r3
    sub r0, r4
    sub r0, r5
    sub r0, r6

    pop {r4, r5, r6, r7}

    bx lr

