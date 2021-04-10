#include "../include/HashTable.hpp"

void BuildHashTable (hash_table* hash_table,                       size_t length_of_table, 
                     hash (*CountHash) (hash_table_val_type elem), int (*Comparator) (hash_table_val_type left_value, hash_table_val_type right_value))
{
    assert (hash_table      != nullptr);
    assert (length_of_table != 0);
    assert (CountHash       != nullptr);
    assert (Comparator      != nullptr);

    hash_table->length_of_table = length_of_table;
    
    hash_table->columns = (vector_info*) calloc (sizeof (vector_info), hash_table->length_of_table);

    for (size_t i = 0; i < hash_table->length_of_table; i++)
    {
        BuildVector (&hash_table->columns[i]);
    }

    hash_table->CountHash  = CountHash;
    hash_table->Comparator = Comparator;
}

void InsertHashTable (hash_table* hash_table, hash_table_key_type key, hash_table_val_type val)
{
    assert (hash_table != nullptr);

    hash hash = hash_table->CountHash (key);

    PushBackVector (&hash_table->columns[hash%hash_table->length_of_table], {key, val});
}

hash_table_val_type FindHashTable (hash_table* hash_table, hash_table_key_type key)
{
    assert (hash_table != nullptr);

    hash hash = hash_table->CountHash (key) % hash_table->length_of_table;

    for (size_t i = 0; i < hash_table->columns[hash].size; i++)
    {
        if (!hash_table->Comparator (hash_table->columns[hash].data[i].key, key)) return hash_table->columns[hash].data[i].val;            
    }

    return POISON; 
}

void DestroyHashTable (hash_table* hash_table)
{
    assert (hash_table != nullptr);

    for (size_t i = 0; i < hash_table->length_of_table; i++)
    {
        DestroyVector (&hash_table->columns[i]);
    }

    free (hash_table->columns);
}