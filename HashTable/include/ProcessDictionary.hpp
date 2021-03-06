#define NDEBUG

#include "TextFunctions.hpp"
#include "ForHashTable.hpp"
#include "HashTable.hpp"

const char* const STANDART_DICTIONARY_FILE = "../Dictionary.txt";

const size_t STANDART_APPROX_VECTOR_SIZE = 15;

struct translation_pair
{
    const char* eng_word = nullptr;
    const char* rus_word = nullptr;
};

struct for_hash_table
{
    size_t length_of_table = 0;

    size_t num_of_translation_pairs = 0;  
    translation_pair* translation_pairs = nullptr; 
};

void ProcessDictionary (int argC, char** argV, for_hash_table* for_hash_table, text* dictionary, 
                        const char* dictionary_file_name = STANDART_DICTIONARY_FILE, size_t approx_vector_size = STANDART_APPROX_VECTOR_SIZE);

translation_pair* FillTranslationPairs (text* dictionary, size_t num_of_translation_pairs);

void MoveToTheNextLine (text* dictionary, size_t* i);

size_t FindMaxPrimeNumberInRange (size_t max_number);

void LoadDictionaryInHashTable (hash_table* hash_table, for_hash_table* for_hash_table, hash (*CountHash) (hash_table_val_type elem), 
                                int (*Comparator) (hash_table_val_type left_value, hash_table_val_type right_value) = strcmp);

void FillHashTable (hash_table* hash_table, for_hash_table* for_hash_table);