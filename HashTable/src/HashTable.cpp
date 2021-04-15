#include "../include/HashTable.hpp"

void BuildHashTable (hash_table* hash_table,                       size_t length_of_table, 
                     hash (*CountHash) (hash_table_val_type elem), int (*Comparator) (hash_table_key_type left_value, hash_table_key_type right_value))
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

    PushBackBucket (&hash_table->columns[hash%hash_table->length_of_table], {key, val});
}

hash_table_val_type FindHashTable (hash_table* hash_table, hash_table_key_type key)
{
    assert (hash_table != nullptr);

    hash hash = hash_table->CountHash (key) % hash_table->length_of_table;

    return FindBucket (&hash_table->columns[hash], key, hash_table->Comparator);
}

void DestroyHashTable (hash_table* hash_table)
{
    assert (hash_table != nullptr);

    for (size_t i = 0; i < hash_table->length_of_table; i++)
    {
        DestroyBucket (&hash_table->columns[i]);
    }

    free (hash_table->columns);
}