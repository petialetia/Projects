#pragma once

#include <immintrin.h>
#include <string.h>

#include "../include/Bucket.hpp"

struct hash_table 
{
    bucket_info* columns = nullptr;
    size_t length_of_table = 0;

    hash (*CountHash)  (hash_table_key_type elem) = nullptr;
    int  (*Comparator) (hash_table_cmp_type left_value, hash_table_cmp_type right_value, const int imm8) = nullptr;
};

void BuildHashTable (hash_table* hash_table, size_t length_of_table, 
                     hash        (*CountHash)  (hash_table_key_type elem), 
                     int         (*Comparator) (hash_table_cmp_type left_value, hash_table_cmp_type right_value, const int imm8));

__m256 CastStringToVector (const char* string);

void InsertHashTable (hash_table* hash_table, hash_table_key_type key, hash_table_val_type val);

hash_table_val_type FindHashTable (hash_table* hash_table, hash_table_key_type key);

void DestroyHashTable (hash_table* hash_table);