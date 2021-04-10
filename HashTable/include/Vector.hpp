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

void BuildVector (vector_info* vector, size_t start_capacity = START_VECTOR_CAPACITY);

void PushBackVector (vector_info* vector, vector_elem elem);

void DestroyVector (vector_info* vector);

void VectorDump (vector_info* vector, const char* const log_file_name = "VectorLog.txt");