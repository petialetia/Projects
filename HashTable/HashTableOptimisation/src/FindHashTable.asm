global FindHashTable
section .text

extern FindBucket

FindHashTable:

    push r12
    push r13
    push r14
    mov r12, rdi          ;r12 = hash_table pointer  
    mov r13, rsi          ;r13 = key

    mov r14, 100          ;r14 = num_of_repeat
    
FindHashTableLoop:

    test r14, r14
    jz FindHashTableExit
    
    mov rdi, r13          ;rdi = key
    mov rsi, [r12 + 2 * 8];rsi = CountHash

    call rsi

    xor rdx, rdx          ;needed for division
    mov rsi, [r12 + 8]    ;rsi = length_of_hash_table

    div rsi

    mov rdi, [r12]        ;rdi = columns pointer

    mov rax, rdx          ;rdx = 3 * rdx (3 == num of 8-byte fields in node)
    add rdx, rdx          
    add rdx, rax

    shl rdx, 3            ;rdx = 8 * rdx
    add rdi, rdx          ;rdi = needed column pointer

    mov rsi, r13          ;rsi = key

    call FindBucket
    
    dec r14
    jmp FindHashTableLoop

FindHashTableExit:
    
    pop r14
    pop r13
    pop r12
    ret


