#include <stdio.h>
#include <math.h>  
#include <cstdint>
#include "../../include/ProcessDictionary.hpp"
#include "../../include/TextFunctions.hpp"
#include "../../include/HashTable.hpp"
#include "../../include/HashFunctionsCollection.hpp"


const char* const STANDART_CSV_FILE = "ResultTable.csv";

void TestHashFunctions (for_hash_table* for_hash_table, FILE* csv_file);

void TestHashFunction (for_hash_table* for_hash_table, FILE* csv_file, hash (*CountHash) (hash_table_val_type elem), 
                       int (*Comparator) (hash_table_val_type left_value, hash_table_val_type right_value) = strcmp);

int main (int argC, char** argV)
{
    text           dictionary     = {};
    for_hash_table for_hash_table = {}; 

    ProcessDictionary (argC, argV, &for_hash_table, &dictionary);

    FILE* csv_file = OpenFile (argC, argV, "out", "w", STANDART_CSV_FILE);
    assert (csv_file != nullptr);

    for (size_t i = 0; i < for_hash_table.length_of_table; i++)
    {
        fprintf (csv_file, "%zu,", i);
    }

    fprintf (csv_file, "\n");

    TestHashFunctions (&for_hash_table, csv_file);

    fclose (csv_file);
    free (for_hash_table.translation_pairs);
    DestroyText (&dictionary);
}

void TestHashFunctions (for_hash_table* for_hash_table, FILE* csv_file)
{
    assert (for_hash_table != nullptr);
    assert (csv_file       != nullptr);

#define TEST_HASH_FUNCTION(HashFunction) TestHashFunction (for_hash_table, csv_file, HashFunction);

    TEST_HASH_FUNCTION (CountUnaryHash)
    TEST_HASH_FUNCTION (CountLengthHash)
    TEST_HASH_FUNCTION (CountFirstAsciiHash)
    TEST_HASH_FUNCTION (CountSumAsciiHash)
    TEST_HASH_FUNCTION (CountRorHash)
    TEST_HASH_FUNCTION (CountRolHash)
    TEST_HASH_FUNCTION (CountPolynomialHash)

#undef TEST_HASH_FUNCTION

}

void TestHashFunction (for_hash_table* for_hash_table, FILE* csv_file, hash (*CountHash) (hash_table_val_type elem), 
                       int (*Comparator) (hash_table_val_type left_value, hash_table_val_type right_value))
{
    assert (for_hash_table != nullptr);
    assert (csv_file       != nullptr);
    assert (CountHash      != nullptr);
    assert (Comparator     != nullptr);

    hash_table hash_table = {};

    FillHashTable (&hash_table, for_hash_table, CountHash, Comparator);

    for (size_t i = 0; i < for_hash_table->length_of_table; i++)
    {
        fprintf (csv_file, "%zu,", hash_table.columns[i].size);
    }

    fprintf (csv_file, "\n");

    DestroyHashTable (&hash_table);
}