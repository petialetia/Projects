global FindBucket
section .text

extern CastStringToVector

FindBucket:

    push r12
    push r13
    push r14

    ;push ymm2

    mov r12, rdi ;r12 = bucket pointer
    mov r14, rdx ;r14 = Comparator

    mov rdi, rsi
    call CastStringToVector

    vmovdqa ymm2, ymm0

    mov r13, [r12 + 8] ;r13 = size of bucket

    mov r12, [r12]     ;pointer to current node

FindBucketLoop:

    test r13, r13

    jz NothingFound

    vmovdqa ymm0, [r12]
    vmovdqa ymm1, ymm2

    call r14

    cmp eax, -1
    je WordFound

    dec r13
    add r12, 64 ; 64 is size of node

    jmp FindBucketLoop

WordFound: 

    add r12, 32
    mov rax, [r12]

    ;lea rax, [r12 + 32]

    jmp FindBucketExit

NothingFound:

    xor rax, rax

FindBucketExit:

    ;pop ymm2

    pop r14
    pop r13
    pop r12

    ret
