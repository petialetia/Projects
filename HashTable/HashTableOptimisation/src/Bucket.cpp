#include "../include/Bucket.hpp"

__m256i CastStringToVector (const char* string)
{
    assert (string != nullptr);

    char buf[sizeof (__m256i)] = {};
    strcpy (buf, string);
    return _mm256_loadu_si256((const __m256i*)buf);
}

void BuildBucket (bucket_info* bucket, size_t start_capacity)
{
    assert (bucket != nullptr);
    assert (start_capacity != 0);

    //bucket->data = (bucket_elem*) calloc (sizeof (bucket_elem), start_capacity);

    bucket->data = (bucket_elem*) aligned_alloc (32, start_capacity*sizeof(bucket_elem));

    bucket->size = 0;
    bucket->capacity = start_capacity;
}

void PushBackBucket (bucket_info* bucket, bucket_elem elem)
{
    //printf ("HI\n");

    assert (bucket != nullptr);
    assert (bucket->size <= bucket->capacity);

    if (bucket->capacity == bucket->size)
    {
        bucket->capacity = bucket->capacity * GROWTH_COEF;

        //bucket->data = (bucket_elem*) realloc (bucket->data, bucket->size * sizeof (bucket_elem) * GROWTH_COEF);

        bucket_elem* new_data = (bucket_elem*) aligned_alloc (32, bucket->capacity * sizeof(bucket_elem));
        memcpy (new_data, bucket->data, bucket->size * sizeof (bucket_elem));
        free (bucket->data);
        bucket->data = new_data;
    }  

    bucket->data[bucket->size] = elem;

    /*u_int64_t string[8] = {};

    string[0] = _mm256_extract_epi64 (bucket->data[bucket->size].key, 0);
    string[1] = _mm256_extract_epi64 (bucket->data[bucket->size].key, 1);

    printf ("<%s>!!\n", (char*) string);*/

    bucket->size++;   
}

hash_table_val_type FindBucket (bucket_info* bucket, hash_table_key_type key, 
                                int (*Comparator) (hash_table_cmp_type left_value, hash_table_cmp_type right_value))
{
    assert (bucket     != nullptr);
    assert (Comparator != nullptr);

    __m256i searchable_key_vector = CastStringToVector (key);

    for (size_t i = 0; i < bucket->size; i++)
    {
        if (!(~Comparator (bucket->data[i].key, searchable_key_vector /*key*/))) return bucket->data[i].val;        
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
