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

    bucket->data = (bucket_elem*) calloc (sizeof (bucket_elem), start_capacity);

    //bucket->data = (bucket_elem*) aligned_alloc (64, start_capacity*sizeof(bucket_elem));

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
                                int (*Comparator) (hash_table_cmp_type left_value, hash_table_cmp_type right_value))
{
    assert (bucket     != nullptr);
    assert (Comparator != nullptr);

    __m256i searchable_key_vector = CastStringToVector (key);

    for (size_t i = 0; i < bucket->size; i++)
    {
        //printf ("<%s>\n", bucket->data[i].key);
        //printf ("<%s>\n", key);
        if (!(~Comparator (CastStringToVector (bucket->data[i].key), searchable_key_vector /*key*/))) return bucket->data[i].val;   
        //printf ("%d\n", ~Comparator (CastStringToVector (bucket->data[i].key), CastStringToVector (key))); 
        //printf ("HEY\n");         
    }

    //printf ("HOY\n");

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
