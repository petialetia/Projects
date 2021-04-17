#include "../include/HashFunctionsCollection.hpp"

#define ROR(elem) ((elem << (sizeof(elem) * BYTE_SIZE - 1)) | (elem >> 1))
#define ROL(elem) ((elem >> (sizeof(elem) * BYTE_SIZE - 1)) | (elem << 1))

size_t CountUnaryHash (hash_table_val_type elem)
{
    assert (elem != nullptr);
    return 1;
}

size_t CountLengthHash (hash_table_val_type elem)
{
    assert (elem != nullptr);
    return strlen (elem);
}

size_t CountFirstAsciiHash (hash_table_val_type elem)
{
    assert (elem != nullptr);
    return elem[0];
}

size_t CountSumAsciiHash (hash_table_val_type elem)
{
    assert (elem != nullptr);
    hash sum = 0;

    for (; *elem != '\0' ; elem++)
    {
        sum += *elem;
    }

    return sum;
}

size_t CountRorHash (hash_table_val_type elem)
{
    assert (elem != nullptr);
    hash hash = 0;

    for (; *elem != '\0'; elem++)
    {
        hash = ROR (hash) ^ *elem;
    }

    return hash;
}

size_t CountRolHash (hash_table_val_type elem)
{
    assert (elem != nullptr);
    hash hash = 0;

    for (; *elem != '\0'; elem++)
    {
        hash = ROL (hash) ^ *elem;
    }

    return hash;
}

size_t CountPolynomialHash (hash_table_val_type elem)
{
    assert (elem != nullptr);

    __asm__(
        ".intel_syntax noprefix\n"

            "xor rax, rax\n"
            "mov cl,  8\n"
            "mov rsi, 999983\n"

        "COUNT_HASH_LOOP:\n"

            "cmp byte ptr [rdi], 0\n"
            "je END_OF_COUNT_HASH\n"

            "shl rax, cl\n"
            "adc al, [rdi]\n"

            "xor rdx, rdx\n"

            "div rsi\n"

            "mov rax, rdx\n"
            "inc rdi\n"

            "jmp COUNT_HASH_LOOP\n"

        "END_OF_COUNT_HASH:\n"

            "mov rsp, rbp\n"
            "pop rbp\n"
            "ret\n"
        
        ".att_syntax\n"
    );

    /*assert (elem != nullptr);

    hash hash = 0;

    for (; *elem != '\0'; elem++)
    {
        hash = (hash * ((1 << BYTE_SIZE) - 1) + *elem) % BIG_PRIME_NUMBER;
    }

    return hash;*/
}

#undef ROR
#undef ROL