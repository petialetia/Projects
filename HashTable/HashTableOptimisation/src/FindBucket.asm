global FindBucket
section .text

extern CastStringToVector

FindBucket:

    push r12
    push r13
    push r14

    mov r12, rdi       ; r12 = bucket pointer
    mov r14, rdx       ; r14 = Comparator

    mov rdi, rsi
    call CastStringToVector

    vmovdqa ymm2, ymm0 ; saving vector

    mov r13, [r12 + 8] ; r13 = size of bucket

    mov r12, [r12]     ; pointer to current node

FindBucketLoop:

    test r13, r13      ; check if list is ended

    jz NothingFound

    vmovdqa ymm0, [r12]
    vmovdqa ymm1, ymm2

    call r14           ; call Comparator

    cmp eax, -1
    je WordFound

    dec r13            ; reducing num of nodes remain in bucket
    add r12, 64        ; 64 is size of node in bucket

    jmp FindBucketLoop

WordFound: 

    add r12, 32
    mov rax, [r12]     ; rax = rus translation

    jmp FindBucketExit

NothingFound:

    xor rax, rax       ; rax = nullptr

FindBucketExit:

    pop r14
    pop r13
    pop r12

    ret
