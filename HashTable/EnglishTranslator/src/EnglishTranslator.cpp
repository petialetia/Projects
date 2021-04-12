#include <ctype.h>

#include "../../include/HashFunctionsCollection.hpp"
#include "../../include/ProcessDictionary.hpp"
#include "../../include/HashTable.hpp"

const char* const STANDART_TEXT_FILE = "Test.txt";
const char* const STANDART_HTML_FILE = "Translated.html";

const char* const ERROR_MSG = "ERROR";

const size_t MAX_LINE_LENGTH = 100;
const size_t MAX_WORD_LENGHT = 50;

struct translation_info
{
    size_t index = 0;
    size_t old_index = 0;

    size_t line_length = 0;
    size_t length_of_word = 0;
    bool is_translation_ended = false;
};

size_t CountNumOfWords (text* text);
char** FillWords (text* text, size_t num_of_words);

void TranslateText (int argC, char** argV, hash_table* hash_table, text* text, const char* output_file_name = STANDART_HTML_FILE);
void ProcessTranslation (text* text, hash_table* hash_table, FILE* html_file);
void PrintSpaces (translation_info* translation_info, text* text, FILE* html_file);
void CheckLineOverflow (translation_info* translation_info, FILE* html_file);
size_t GetNextWord (char* orig_word, translation_info* translation_info, text* text);
void strlwr (char* string);
void PrintAtypicalWord (translation_info* translation_info, text* text, hash_table* hash_table, FILE* html_file, char* word_combination);
void PrintSingleWord (translation_info* translation_info, text* text, FILE* html_file, const char* translated, size_t* offset);
void PrintWord (text* text, FILE* html_file, size_t length, size_t* offset);

int main (int argC, char** argV)
{
    text           dictionary     = {};
    for_hash_table for_hash_table = {}; 

    ProcessDictionary (argC, argV, &for_hash_table, &dictionary);

    hash_table hash_table = {};

    FillHashTable (&hash_table, &for_hash_table, CountPolynomialHash);

    text text = {};

    ProcessInput (argC, argV, &text, STANDART_TEXT_FILE);

    TranslateText (argC, argV, &hash_table, &text);

    DestroyText (&text);
    DestroyHashTable (&hash_table);
    free (for_hash_table.translation_pairs);
    DestroyText (&dictionary);
}

size_t CountNumOfWords (text* text)
{
    assert (text != nullptr);

    size_t num_of_words = 0;

    for (size_t i = 0; i < text->file_length; i++)
    {
        if (isalpha (text->pointer_on_buffer[i]))
        {
            num_of_words++;
            i++;
            while (isalpha (text->pointer_on_buffer[i])) i++;
        }
    }

    return num_of_words;
}

void TranslateText (int argC, char** argV, hash_table* hash_table, text* text, const char* output_file_name)
{
    assert (argV       != nullptr);
    assert (hash_table != nullptr);
    assert (text       != nullptr);

    FILE* html_file = OpenFile (argC, argV, "out", "w", output_file_name);
    assert (html_file != nullptr);

    fprintf (html_file, "<meta charset=\"UTF-8\">\n<html>\n<body>\n<pre>\n");

    ProcessTranslation (text, hash_table, html_file);

    fprintf (html_file, "\n</pre>\n</body>\n</html>");
}

void ProcessTranslation (text* text, hash_table* hash_table, FILE* html_file)
{
    assert (text       != nullptr);
    assert (hash_table != nullptr);
    assert (html_file  != nullptr);

    char word_combination[2*MAX_WORD_LENGHT + 2] = {}; //+2 for ' ' and '/0'

    translation_info translation_info = {};

    while (!translation_info.is_translation_ended)
    {
        PrintSpaces (&translation_info, text, html_file);

        if (translation_info.is_translation_ended) break;

        CheckLineOverflow (&translation_info, html_file);
        translation_info.length_of_word = GetNextWord (word_combination, &translation_info, text);

        if (translation_info.index + translation_info.length_of_word == text->file_length) translation_info.is_translation_ended = true;
        strlwr (word_combination);

        const char* translated = FindHashTable (hash_table, word_combination);

        if (translated == nullptr) PrintAtypicalWord (&translation_info, text, hash_table, html_file, word_combination);
        else                       PrintSingleWord   (&translation_info, text, html_file, translated, &translation_info.index); 
    }
}

void PrintSpaces (translation_info* translation_info, text* text, FILE* html_file)
{
    assert (translation_info != nullptr);
    assert (text             != nullptr);
    assert (html_file        != nullptr);

    while (true)
    {
        if (translation_info->index == text->file_length) 
        {
            translation_info->is_translation_ended = true;
            break;
        }

        if (!isalpha (text->pointer_on_buffer[translation_info->index]))
        {
            fprintf (html_file, "%c", text->pointer_on_buffer[translation_info->index]);

            if (text->pointer_on_buffer[translation_info->index] == '\n') translation_info->line_length = 0;
            else                                                          translation_info->line_length++;

            translation_info->index++;
        }
        else break;
    }
}

void CheckLineOverflow (translation_info* translation_info, FILE* html_file)
{
    assert (translation_info != nullptr);
    assert (html_file        != nullptr);

    if (translation_info->line_length > MAX_LINE_LENGTH)
    {   
        fprintf (html_file, "\n");
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

void PrintAtypicalWord (translation_info* translation_info, text* text, hash_table* hash_table, FILE* html_file, char* word_combination)
{
    assert (translation_info != nullptr);
    assert (text             != nullptr);
    assert (hash_table       != nullptr);
    assert (html_file        != nullptr);
    assert (word_combination != nullptr);

    bool is_different_lines = false;
    size_t i = 0;
    translation_info->old_index = translation_info->index; 
    translation_info->index += translation_info->length_of_word;
    do
    {
        if (text->pointer_on_buffer[translation_info->index + i] == '\n') 
        {
            word_combination[translation_info->length_of_word + i] = ' ';
            is_different_lines = true;
        }
        else word_combination[translation_info->length_of_word + i] = text->pointer_on_buffer[translation_info->index + i];
        i++; 
    }
    while (!isalpha (text->pointer_on_buffer[translation_info->index + i]));
    word_combination[translation_info->length_of_word + i] = '\0';
    translation_info->index += i;
    size_t length_of_second_word = GetNextWord (&word_combination[translation_info->length_of_word + i], translation_info, text);
    strlwr (&word_combination[translation_info->length_of_word + i]);
    const char* translated = FindHashTable (hash_table, word_combination);
    if (translated != nullptr) 
    {
        translation_info->line_length += translation_info->length_of_word + i; 
        fprintf (html_file, "<span title=\"%s\">", translated);
        if (is_different_lines)
        {
            translation_info->index--;
            while (translation_info->old_index < translation_info->index)
            {
                fprintf (html_file, "%c", text->pointer_on_buffer[translation_info->old_index]);
                translation_info->old_index++;
            } 
            fprintf (html_file, "\n");
            translation_info->line_length = 0;
            translation_info->index++;
        }
        else
        {
            while (translation_info->old_index < translation_info->index)
            {
                fprintf (html_file, "%c", text->pointer_on_buffer[translation_info->old_index]);
                translation_info->old_index++;
            }
            CheckLineOverflow (translation_info, html_file);
        }
        for (size_t i = 0; i < length_of_second_word; i++)
        {
            fprintf (html_file, "%c", text->pointer_on_buffer[translation_info->index]);
            translation_info->index++;   
        } 
        fprintf (html_file, "</span>");
        translation_info->line_length += length_of_second_word;
    }
    else
    {
        translated = ERROR_MSG;
        fprintf (html_file, "<span title=\"%s\">", translated);
        for (size_t i = 0; i < translation_info->length_of_word; i++)
        {
            fprintf (html_file, "%c", text->pointer_on_buffer[translation_info->old_index]);
            translation_info->old_index++;
        }
        fprintf (html_file, "</span>");
        translation_info->line_length += translation_info->length_of_word;
        translation_info->index -= i;
    }
}

void PrintSingleWord (translation_info* translation_info, text* text, FILE* html_file, const char* translated, size_t* offset)
{
    fprintf (html_file, "<span title=\"%s\">", translated);

    PrintWord (text, html_file, translation_info->length_of_word, offset);

    fprintf (html_file, "</span>"); 

    translation_info->line_length += translation_info->length_of_word;
}

void PrintWord (text* text, FILE* html_file, size_t length, size_t* offset)
{
    for (size_t i = 0; i < length; i++)
    {
        fprintf (html_file, "%c", text->pointer_on_buffer[*offset]);
        (*offset)++;
    }
}