#include "../include/HashTable.hpp"

#ifndef StandartComparatorOptimised

int StandartComparator (hash_table_cmp_type left_key, hash_table_cmp_type right_key)
{
    #ifdef AVX_INSTRUCTIONS

    return _mm256_movemask_epi8 (_mm256_cmpeq_epi8 (left_key, right_key));

    #else

    return strcmp (left_key, right_key);

    #endif
}

#endif

extern "C" int StandartComparator (hash_table_cmp_type left_key, hash_table_cmp_type right_key)
{
    return _mm256_movemask_epi8 (_mm256_cmpeq_epi8 (left_key, right_key));   
}

void BuildHashTable (hash_table* hash_table, size_t length_of_table, 
                     hash (*CountHash) (hash_table_val_type elem), 
                     int  (*Comparator) (hash_table_cmp_type left_value, hash_table_cmp_type right_value))
{
    assert (hash_table      != nullptr);
    assert (length_of_table != 0);
    assert (CountHash       != nullptr);
    assert (Comparator      != nullptr);

    hash_table->length_of_table = length_of_table;
    
    hash_table->columns = (bucket_info*) calloc (sizeof (bucket_info), hash_table->length_of_table);

    for (size_t i = 0; i < hash_table->length_of_table; i++)
    {
        BuildBucket (&hash_table->columns[i]);
    }

    hash_table->CountHash  = CountHash;
    hash_table->Comparator = Comparator;
}

void InsertHashTable (hash_table* hash_table, hash_table_key_type key, hash_table_val_type val)
{
    assert (hash_table != nullptr);

    hash hash = hash_table->CountHash (key);

#ifdef AVX_INSTRUCTIONS

    PushBackBucket (&hash_table->columns[hash%hash_table->length_of_table], {CastStringToVector(key), val});

#else

    PushBackBucket (&hash_table->columns[hash%hash_table->length_of_table], {key, val});

#endif
}

#ifndef FindHashTableOptimised

hash_table_val_type FindHashTable (hash_table* hash_table, hash_table_key_type key)
{
    assert (hash_table != nullptr);

    hash_table_val_type translated = nullptr;

    for (size_t i = 0; i < num_of_repeat; i++)
    {
        hash hash = hash_table->CountHash (key) % hash_table->length_of_table;

        translated = FindBucket (&hash_table->columns[hash], key, hash_table->Comparator);
    }

    return translated;
}

#endif

void DestroyHashTable (hash_table* hash_table)
{
    assert (hash_table != nullptr);

    for (size_t i = 0; i < hash_table->length_of_table; i++)
    {
        DestroyBucket (&hash_table->columns[i]);
    }

    free (hash_table->columns);
}