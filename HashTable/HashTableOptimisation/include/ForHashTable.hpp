#pragma once

#include <math.h>

typedef size_t hash;
typedef const char* hash_table_key_type;
typedef const char* hash_table_val_type;

struct hash_table_node
{
    hash_table_key_type key = nullptr;
    hash_table_val_type val = nullptr;
};