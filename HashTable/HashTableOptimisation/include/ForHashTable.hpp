#define NDEBUG

#pragma once

#include <math.h>
#include <immintrin.h>

typedef size_t hash;
typedef const char* hash_table_key_type;
typedef const char* hash_table_val_type;

typedef __m256i hash_table_cmp_type;
//typedef const char* hash_table_cmp_type;

struct hash_table_node
{
    //hash_table_key_type key = nullptr;
    __m256i key;
    hash_table_val_type val = nullptr;
    void*               placeholder1 = nullptr;
    void*               placeholder2 = nullptr;
    void*               placeholder3 = nullptr; //placeholders are needed for hash_table_node to be 64 byte size
};

extern "C" int StandartComparator (hash_table_cmp_type left_key, hash_table_cmp_type right_key);