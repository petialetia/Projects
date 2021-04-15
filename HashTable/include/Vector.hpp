#include <stdio.h>
#include <assert.h>

#include "ForHashTable.hpp"

typedef hash_table_node vector_elem;

struct vector_info 
{
    vector_elem* data = nullptr;
    
    size_t size = 0;
    size_t capacity = 0;    
};

const size_t GROWTH_COEF = 2;
const size_t START_VECTOR_CAPACITY = 15;

const hash_table_val_type POISON = nullptr;

void BuildVector (vector_info* vector, size_t start_capacity = START_VECTOR_CAPACITY);

void PushBackVector (vector_info* vector, vector_elem elem);

hash_table_val_type FindVector (vector_info* vector, hash_table_key_type key, 
                                int (*Comparator) (hash_table_key_type left_value, hash_table_key_type right_value));

void DestroyVector (vector_info* vector);

void VectorDump (vector_info* vector, const char* const log_file_name = "VectorLog.txt");