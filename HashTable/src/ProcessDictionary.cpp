#include "../include/ProcessDictionary.hpp"

void ProcessDictionary (int argC, char** argV, for_hash_table* for_hash_table, text* dictionary)
{
    assert (argV           != nullptr);
    assert (for_hash_table != nullptr);
    assert (dictionary     != nullptr);

    ProcessInput (argC, argV, dictionary, STANDART_DICTIONARY_FILE);

    size_t num_of_lines = CountNumOfLines (dictionary);
    assert (num_of_lines != 0);

    for_hash_table->num_of_translation_pairs = num_of_lines/2;
    for_hash_table->translation_pairs = FillTranslationPairs (dictionary, for_hash_table->num_of_translation_pairs);

    for_hash_table->length_of_table = FindMaxPrimeNumberInRange (for_hash_table->num_of_translation_pairs/APPROX_VECTOR_SIZE);
    assert (for_hash_table->length_of_table != 0); 
}

translation_pair* FillTranslationPairs (text* dictionary, size_t num_of_translation_pairs)
{
    assert (dictionary != nullptr);
    assert (num_of_translation_pairs != 0);

    translation_pair* translation_pairs = (translation_pair*) calloc (sizeof (translation_pair), num_of_translation_pairs);

    size_t i = 0;
    size_t num_of_pair = 0;

    do 
    {
        translation_pairs[num_of_pair].eng_word = &dictionary->pointer_on_buffer[i];

        while (dictionary->pointer_on_buffer[i] != '\r')
        {
            i++; 
            assert (i <= dictionary->file_length);   
        }
        MoveToTheNextLine (dictionary, &i);
        translation_pairs[num_of_pair].rus_word = &dictionary->pointer_on_buffer[i];

        while (dictionary->pointer_on_buffer[i] != '\r' && dictionary->pointer_on_buffer[i] != '\0')
        {
            assert (i <= dictionary->file_length);
            i++;
        }
        MoveToTheNextLine (dictionary, &i);
        num_of_pair++;
    }
    while ((i <= dictionary->file_length) && (dictionary->pointer_on_buffer[i] != '\0') && (num_of_pair < num_of_translation_pairs));

    return translation_pairs;
}

void MoveToTheNextLine (text* dictionary, size_t* i)
{
    assert (dictionary != nullptr);
    assert (i          != nullptr);

    dictionary->pointer_on_buffer[*i] = '\0'; //removing '\r'
    (*i)++;
    dictionary->pointer_on_buffer[*i] = '\0'; //removing '\n'
    (*i)++;
}

size_t FindMaxPrimeNumberInRange (size_t max_number)
{
    assert (max_number > 1);

    bool* is_composite = (bool*) calloc (sizeof (bool), max_number - 1);
    assert (is_composite != nullptr);

    for (size_t i = 2; i <= (size_t)sqrt ((double)max_number) + 1; i++)
    {
        if (!is_composite[i-2]) 
        {
            for (size_t j = 1; i*j <= max_number; j++)
            {
                is_composite[i*j-2] = true;
            }      
        }
    }

    for (size_t i = max_number; i >= 2; i--)
    {
        if (!is_composite[i-2]) 
        {
            free (is_composite);
            return i;
        }
    }

    free (is_composite);
    return 0;
}