#define NDEBUG

#define PRINTF_RUNTIME

#define AVX_INSTRUCTIONS
#define FindBucketOptimised
#define StandartComparatorOptimised
#define CountPolynomialHashOptimised
#define FindHashTableOptimised

#pragma once

#include <math.h>
#include <immintrin.h>

const size_t num_of_repeat = 100;

typedef size_t hash;
typedef const char* hash_table_key_type;
typedef const char* hash_table_val_type;

#ifdef AVX_INSTRUCTIONS

typedef __m256i hash_table_cmp_type;

#else

typedef const char* hash_table_cmp_type;

#endif

struct hash_table_node
{

#ifdef AVX_INSTRUCTIONS

    __m256i key;

#else

    hash_table_key_type key = nullptr;

#endif



    hash_table_val_type val = nullptr;



#ifdef AVX_INSTRUCTIONS

    void*               placeholder1 = nullptr;
    void*               placeholder2 = nullptr;
    void*               placeholder3 = nullptr; //placeholders are needed for hash_table_node to be 64 byte size

#endif

};

#ifdef StandartComparatorOptimised

extern "C" int StandartComparator (hash_table_cmp_type left_key, hash_table_cmp_type right_key);

#else

int StandartComparator (hash_table_cmp_type left_key, hash_table_cmp_type right_key);

#endif