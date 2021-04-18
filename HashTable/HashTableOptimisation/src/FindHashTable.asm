global FindHashTable
section .text

extern FindBucket

FindHashTable:

    push r12
    push r13
    mov r12, rdi          ;r12 = hash_table pointer  
    mov r13, rsi          ;r13 = key

    mov rdi, rsi          ;rdi = key
    mov rsi, [r12 + 2 * 8];rsi = CountHash

    call rsi

    xor rdx, rdx          ;needed for division
    mov rsi, [r12 + 8]    ;rsi = length_of_hash_table

    div rsi

    mov rdi, [r12]        ;rdi = columns pointer

    mov rax, rdx          ;rdx = 3 * rdx
    add rdx, rdx          
    add rdx, rax

    shl rdx, 3            ;rdx = 8 * rdx
    add rdi, rdx          ;rdi = needed column pointer

    mov rsi, r13          ;rsi = key

    mov rdx, [r12 + 3 * 8];rdx = Comparator

    call FindBucket

    pop r13
    pop r12
    ret


