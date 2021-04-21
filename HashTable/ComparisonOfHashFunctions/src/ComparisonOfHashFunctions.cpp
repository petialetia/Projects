#include <stdio.h>
#include <math.h>
#include <time.h>
#include <cstdint>
#include "../../include/ProcessDictionary.hpp"
#include "../../include/TextFunctions.hpp"
#include "../../include/HashTable.hpp"
#include "../../include/HashFunctionsCollection.hpp"

const char* const STANDART_CSV_FILE = "ResultTable15.csv";

const size_t APPROX_VECTOR_SIZE = 15;

const size_t STEP_OF_DISTRIBUTION = 5;

void TestHashFunctions (for_hash_table* for_hash_table, FILE* csv_file);

float TestHashFunction (for_hash_table* for_hash_table, FILE* csv_file, hash (*CountHash) (hash_table_key_type elem), 
                        int (*Comparator) (hash_table_key_type left_value, hash_table_key_type right_value) = strcmp);

int main (int argC, char** argV)
{
    text           dictionary     = {};
    for_hash_table for_hash_table = {}; 

    ProcessDictionary (argC, argV, &for_hash_table, &dictionary, STANDART_DICTIONARY_FILE, APPROX_VECTOR_SIZE);

    FILE* csv_file = OpenFile (argC, argV, "out", "w", STANDART_CSV_FILE);
    assert (csv_file != nullptr);

    fprintf (csv_file, "Hash function name,Test runtime,Bucket's nums,");

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

#define TEST_HASH_FUNCTION(HashFunction)                      \
                                                              \
    fprintf (csv_file, "%s,", #HashFunction);                 \
    TestHashFunction (for_hash_table, csv_file, HashFunction);\

    TEST_HASH_FUNCTION (CountUnaryHash)
    TEST_HASH_FUNCTION (CountLengthHash)
    TEST_HASH_FUNCTION (CountFirstAsciiHash)
    TEST_HASH_FUNCTION (CountSumAsciiHash)
    TEST_HASH_FUNCTION (CountRorHash)
    TEST_HASH_FUNCTION (CountRolHash)
    TEST_HASH_FUNCTION (CountPolynomialHash)

#undef TEST_HASH_FUNCTION

}

float TestHashFunction (for_hash_table* for_hash_table, FILE* csv_file, hash (*CountHash) (hash_table_key_type elem), 
                        int (*Comparator) (hash_table_key_type left_value, hash_table_key_type right_value))
{
    assert (for_hash_table != nullptr);
    assert (csv_file       != nullptr);
    assert (CountHash      != nullptr);
    assert (Comparator     != nullptr);

    hash_table hash_table = {};

    BuildHashTable (&hash_table, for_hash_table->length_of_table, CountHash, Comparator);

    clock_t start_time = clock ();
    FillHashTable (&hash_table, for_hash_table);
    clock_t end_time = clock ();

    fprintf (csv_file, "%f,Bucket sizes,", (float)(end_time - start_time)/CLOCKS_PER_SEC);

    size_t* distribution = (size_t*) calloc (for_hash_table->num_of_translation_pairs / STEP_OF_DISTRIBUTION + 1, sizeof (size_t));

    for (size_t i = 0; i < for_hash_table->length_of_table; i++)
    {
        fprintf (csv_file, "%zu,", hash_table.columns[i].size);
        //printf ("%zu\n", hash_table.columns[i].size / STEP_OF_DISTRIBUTION);

        //size_t test = hash_table.columns[i].size / STEP_OF_DISTRIBUTION;

        //distribution[test]++;

        distribution[hash_table.columns[i].size / STEP_OF_DISTRIBUTION]++; 
    }

    fprintf (csv_file, "\n");

    for (size_t i = 0; i <= for_hash_table->num_of_translation_pairs / STEP_OF_DISTRIBUTION; i++)
    {
        fprintf (csv_file, "%zu - %zu,", i * STEP_OF_DISTRIBUTION, i * STEP_OF_DISTRIBUTION + STEP_OF_DISTRIBUTION - 1);
    }

    fprintf (csv_file, "\n");

    //printf ("HEY\n");

    for (size_t i = 0; i <= for_hash_table->num_of_translation_pairs / STEP_OF_DISTRIBUTION; i++)
    {
        fprintf (csv_file, "%zu,", distribution[i]);
    }

    fprintf (csv_file, "\n\n");

    free (distribution);

    DestroyHashTable (&hash_table);

    return (float)(end_time - start_time)/CLOCKS_PER_SEC;
}