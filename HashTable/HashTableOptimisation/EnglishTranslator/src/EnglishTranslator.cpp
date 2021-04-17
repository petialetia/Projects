#include "EnglishTranslator.hpp"

int main (int argC, char** argV)
{
    text           dictionary     = {};
    for_hash_table for_hash_table = {}; 

    ProcessDictionary (argC, argV, &for_hash_table, &dictionary);

    hash_table hash_table = {};

    LoadDictionaryInHashTable (&hash_table, &for_hash_table, CountPolynomialHash);

    text text = {};

    ProcessInput (argC, argV, &text, STANDART_TEXT_FILE);

    TranslateText (argC, argV, &hash_table, &text);

    DestroyText (&text);
    DestroyHashTable (&hash_table);
    free (for_hash_table.translation_pairs);
    DestroyText (&dictionary);

    /*hash_table hash_table = {};

    BuildHashTable (&hash_table, 2, CountPolynomialHash, strcmp);

    InsertHashTable (&hash_table, "test", "тест");

    DestroyHashTable (&hash_table);*/

    //printf ("%zu\n", CountPolynomialHash ("test"));
}

void TranslateText (int argC, char** argV, hash_table* hash_table, text* text, const char* output_file_name)
{
    assert (argV       != nullptr);
    assert (hash_table != nullptr);
    assert (text       != nullptr);

    translation_info translation_info = {};

    translation_info.html_file = OpenFile (argC, argV, "out", "w", output_file_name);
    assert (translation_info.html_file != nullptr);

    fprintf (translation_info.html_file, "<meta charset=\"UTF-8\">\n<html>\n<body>\n<pre>\n");

    ProcessTranslation (&translation_info, text, hash_table);

    fprintf (translation_info.html_file, "\n</pre>\n</body>\n</html>");
}

void ProcessTranslation (translation_info* translation_info, text* text, hash_table* hash_table)
{
    assert (translation_info            != nullptr);
    assert (text                        != nullptr);
    assert (hash_table                  != nullptr);
    assert (translation_info->html_file != nullptr);

    while (!translation_info->is_translation_ended)
    {
        PrintSpaces (translation_info, text);

        if (translation_info->is_translation_ended) break;

        CheckLineOverflow (translation_info);
        translation_info->length_of_word = GetNextWord (translation_info->word_combination, translation_info, text);

        if (translation_info->index + translation_info->length_of_word == text->file_length) translation_info->is_translation_ended = true;
        strlwr (translation_info->word_combination);

        const char* translated = FindHashTable (hash_table, translation_info->word_combination);

        if (translated == nullptr) ProcessAtypicalWord (translation_info, text, hash_table);
        else                       PrintSingleWord     (translation_info, text, translated); 
    }
}

void PrintSpaces (translation_info* translation_info, text* text)
{
    assert (translation_info            != nullptr);
    assert (text                        != nullptr);
    assert (translation_info->html_file != nullptr);

    while (true)
    {
        if (translation_info->index == text->file_length) 
        {
            translation_info->is_translation_ended = true;
            break;
        }

        if (!isalpha (text->pointer_on_buffer[translation_info->index]))
        {
            fprintf (translation_info->html_file, "%c", text->pointer_on_buffer[translation_info->index]);

            if (text->pointer_on_buffer[translation_info->index] == '\n') translation_info->line_length = 0;
            else                                                          translation_info->line_length++;

            translation_info->index++;
        }
        else break;
    }
}

void CheckLineOverflow (translation_info* translation_info)
{
    assert (translation_info            != nullptr);
    assert (translation_info->html_file != nullptr);

    if (translation_info->line_length > MAX_LINE_LENGTH)
    {   
        fprintf (translation_info->html_file, "\n");
        translation_info->line_length = 0;   
    }
}

size_t GetNextWord (char* word, translation_info* translation_info, text* text)
{
    assert (word             != nullptr);
    assert (translation_info != nullptr);
    assert (text             != nullptr);
    assert (isalpha (text->pointer_on_buffer[translation_info->index]));

    size_t length_of_word = 1;

    while (isalpha (text->pointer_on_buffer[translation_info->index + length_of_word])) length_of_word++; 

    strncpy (word, &text->pointer_on_buffer[translation_info->index], length_of_word);
    word[length_of_word] = '\0';

    return length_of_word;
}

void strlwr (char* string)
{
    assert (string != nullptr);

    for (; *string != '\0'; string++)
    {
        if (isupper (*string)) *string += 32; 
    }
}

void ProcessAtypicalWord (translation_info* translation_info, text* text, hash_table* hash_table)
{
    assert (translation_info                   != nullptr);
    assert (text                               != nullptr);
    assert (hash_table                         != nullptr);
    assert (translation_info->html_file        != nullptr);
    assert (translation_info->word_combination != nullptr);

    for_combination_word comb_word_info = {};

    translation_info->old_index = translation_info->index; 
    translation_info->index += translation_info->length_of_word;

    SkipSpaces (translation_info, text, &comb_word_info);

    translation_info->word_combination[translation_info->length_of_word + comb_word_info.num_of_spaces] = '\0';
    translation_info->index += comb_word_info.num_of_spaces;
    comb_word_info.length_of_second_word = GetNextWord (&translation_info->word_combination[translation_info->length_of_word + comb_word_info.num_of_spaces], 
                                                        translation_info, text);

    strlwr (&translation_info->word_combination[translation_info->length_of_word + comb_word_info.num_of_spaces]);
    comb_word_info.translated = FindHashTable (hash_table, translation_info->word_combination);

    if (comb_word_info.translated != nullptr) PrintTwoWords (translation_info, text, &comb_word_info);
    else                                      ProcessShortening (translation_info, text, hash_table);
}

void SkipSpaces (translation_info* translation_info, text* text, for_combination_word* comb_word_info)
{
    assert (translation_info != nullptr);
    assert (text             != nullptr);
    assert (comb_word_info   != nullptr);

    do
    {
        if (text->pointer_on_buffer[translation_info->index + comb_word_info->num_of_spaces] == '\n') 
        {
            translation_info->word_combination[translation_info->length_of_word + comb_word_info->num_of_spaces] = ' ';
            comb_word_info->is_different_lines = true;
        }
        else translation_info->word_combination[translation_info->length_of_word + comb_word_info->num_of_spaces] = 
                                                                                 text->pointer_on_buffer[translation_info->index + comb_word_info->num_of_spaces];
        comb_word_info->num_of_spaces++; 
    }
    while (!isalpha (text->pointer_on_buffer[translation_info->index + comb_word_info->num_of_spaces]));
}

void ProcessShortening (translation_info* translation_info, text* text, hash_table* hash_table)
{
    assert (translation_info != nullptr);
    assert (text             != nullptr);
    assert (hash_table       != nullptr);

    translation_info->word_combination[translation_info->length_of_word - 1] = '\0';
    translation_info->index = translation_info->old_index;

    const char* translated = FindHashTable (hash_table, translation_info->word_combination); 

    if (translated != nullptr) PrintSingleWord (translation_info, text, translated);
    else
    {
        translation_info->word_combination[translation_info->length_of_word - 2] = '\0';
        translated = FindHashTable (hash_table, translation_info->word_combination);

        if (translated == nullptr) translated = ERROR_MSG;

        PrintSingleWord (translation_info, text, translated);    
    }
}

void PrintSingleWord (translation_info* translation_info, text* text, const char* translated)
{
    assert (translation_info            != nullptr);
    assert (text                        != nullptr);
    assert (translation_info->html_file != nullptr);
    assert (translated                  != nullptr);

    fprintf (translation_info->html_file, "<span title=\"%s\">", translated);

    PrintWord (text, translation_info->html_file, translation_info->length_of_word, &translation_info->index);

    fprintf (translation_info->html_file, "</span>"); 

    translation_info->line_length += translation_info->length_of_word;
}

void PrintTwoWords (translation_info* translation_info, text* text, for_combination_word* comb_word_info)
{
    assert (translation_info != nullptr);
    assert (text             != nullptr);
    assert (comb_word_info   != nullptr);

    translation_info->line_length += translation_info->length_of_word + comb_word_info->num_of_spaces; 
    fprintf (translation_info->html_file, "<span title=\"%s\">", comb_word_info->translated);

    if (comb_word_info->is_different_lines)
    {
        translation_info->index--;

        PrintWord (text, translation_info->html_file, translation_info->index - translation_info->old_index, &translation_info->old_index);
        fprintf (translation_info->html_file, "\n");

        translation_info->line_length = 0;
        translation_info->index++;
    }
    else
    {
        PrintWord (text, translation_info->html_file, translation_info->index - translation_info->old_index, &translation_info->old_index);
        CheckLineOverflow (translation_info);
    }

    PrintWord (text, translation_info->html_file, comb_word_info->length_of_second_word, &translation_info->index);

    fprintf (translation_info->html_file, "</span>");
    translation_info->line_length += comb_word_info->length_of_second_word;
}

void PrintWord (text* text, FILE* html_file, size_t length, size_t* offset)
{
    assert (text      != nullptr);
    assert (html_file != nullptr);
    assert (offset    != nullptr);

    assert (length > 0);

    for (size_t i = 0; i < length; i++)
    {
        fprintf (html_file, "%c", text->pointer_on_buffer[*offset]);
        (*offset)++;
    }
}