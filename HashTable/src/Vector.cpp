#include "../include/Vector.hpp"

void BuildVector (vector_info* vector, size_t start_capacity)
{
    assert (vector != nullptr);
    assert (start_capacity != 0);

    vector->data = (vector_elem*) calloc (sizeof (vector_elem), start_capacity);
    vector->size = 0;
    vector->capacity = start_capacity;
}

void PushBackVector (vector_info* vector, vector_elem elem)
{
    assert (vector != nullptr);
    assert (vector->size <= vector->capacity);

    if (vector->capacity == vector->size)
    {
        vector->data = (vector_elem*) realloc (vector->data, vector->size * sizeof (vector_elem) * GROWTH_COEF);
        vector->capacity = vector->capacity * GROWTH_COEF;
    }  

    vector->data[vector->size] = elem;
    vector->size++;   
}

hash_table_val_type FindVector (vector_info* vector, hash_table_key_type key, 
                                int (*Comparator) (hash_table_key_type left_value, hash_table_key_type right_value))
{
    assert (vector     != nullptr);
    assert (Comparator != nullptr);

    for (size_t i = 0; i < vector->size; i++)
    {
        if (!Comparator (vector->data[i].key, key)) return vector->data[i].val;            
    }

    return POISON;    
}

void DestroyVector (vector_info* vector)
{
    assert (vector != nullptr);

    free (vector->data);
}

void VectorDump (vector_info* vector, const char* const log_file_name)
{
    assert (vector != nullptr);
    assert (log_file_name != nullptr);

    static FILE* log_file = nullptr;

    if (log_file == nullptr) 
    {
        log_file = fopen (log_file_name, "w");
        assert (log_file != nullptr);
    }

    fprintf (log_file, "data = %p\nsize = %zu\ncapacity = %zu\n\n", (void*)vector->data, vector->size, vector->capacity);
}
