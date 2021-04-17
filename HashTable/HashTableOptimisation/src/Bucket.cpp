#include "../include/Bucket.hpp"

void BuildBucket (bucket_info* bucket, size_t start_capacity)
{
    assert (bucket != nullptr);
    assert (start_capacity != 0);

    bucket->data = (bucket_elem*) calloc (sizeof (bucket_elem), start_capacity);
    bucket->size = 0;
    bucket->capacity = start_capacity;
}

void PushBackBucket (bucket_info* bucket, bucket_elem elem)
{
    assert (bucket != nullptr);
    assert (bucket->size <= bucket->capacity);

    if (bucket->capacity == bucket->size)
    {
        bucket->data = (bucket_elem*) realloc (bucket->data, bucket->size * sizeof (bucket_elem) * GROWTH_COEF);
        bucket->capacity = bucket->capacity * GROWTH_COEF;
    }  

    bucket->data[bucket->size] = elem;
    bucket->size++;   
}

hash_table_val_type FindBucket (bucket_info* bucket, hash_table_key_type key, 
                                int (*Comparator) (hash_table_key_type left_value, hash_table_key_type right_value))
{
    assert (bucket     != nullptr);
    assert (Comparator != nullptr);

    for (size_t i = 0; i < bucket->size; i++)
    {
        if (!Comparator (bucket->data[i].key, key)) return bucket->data[i].val;            
    }

    return POISON;    
}

void DestroyBucket (bucket_info* bucket)
{
    assert (bucket != nullptr);

    free (bucket->data);
}

void Dump (bucket_info* bucket, const char* const log_file_name)
{
    assert (bucket != nullptr);
    assert (log_file_name != nullptr);

    static FILE* log_file = nullptr;

    if (log_file == nullptr) 
    {
        log_file = fopen (log_file_name, "w");
        assert (log_file != nullptr);
    }

    fprintf (log_file, "data = %p\nsize = %zu\ncapacity = %zu\n\n", (void*)bucket->data, bucket->size, bucket->capacity);
}
