#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "ForHashTable.hpp"

typedef hash_table_node bucket_elem;

struct bucket_info 
{
    bucket_elem* data = nullptr;
    
    size_t size = 0;
    size_t capacity = 0;    
};

const size_t GROWTH_COEF = 2;
const size_t START_BUCKET_CAPACITY = 15;

const hash_table_val_type POISON = nullptr;

__m256i CastStringToVector (const char* string);

void BuildBucket (bucket_info* bucket, size_t start_capacity = START_BUCKET_CAPACITY);

void PushBackBucket (bucket_info* bucket, bucket_elem elem);

hash_table_val_type FindBucket (bucket_info* bucket, hash_table_key_type key, 
                                int (*Comparator) (hash_table_cmp_type left_value, hash_table_cmp_type right_value));

void DestroyBucket (bucket_info* bucket);

void DumpBucket (bucket_info* bucket, const char* const log_file_name = "BucketLog.txt");