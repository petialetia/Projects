#pragma once

#include <math.h>
#include <immintrin.h>

typedef size_t hash;
typedef const char* hash_table_key_type;
typedef const char* hash_table_val_type;

typedef __m256 hash_table_cmp_type;

struct hash_table_node
{
    hash_table_key_type key = nullptr;
    //__m256 key;
    hash_table_val_type val = nullptr;
};

int StandartComparator (hash_table_cmp_type left_key, hash_table_cmp_type right_key, const int imm8 = _CMP_EQ_OQ);