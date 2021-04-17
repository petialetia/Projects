section. text

global InsertHashTable
extern PushBackBucket

InsertHashTable:

    ;rdi - struct pointer
    ;rsi - key pointer
    ;rdx - val pointer

    ;push rbp
    ;mov rbp, rsp
    push r12, r13, r14
    mov r12, rdi
    mov r13, rsi
    mov r14, rdx

    mov rdi, rsi
    mov rdx, [r12 + 16]
    call rdx

    xor rdx, rdx
    mov rcx, [r12+8]
    div rcx

    shl rax, 3

    mov rdi, [r12]
    add rdi, rax

    mov rsi, r13
    mov rdx, r14

    call PushBackBucket

    pop r15, r14, r12
    ret