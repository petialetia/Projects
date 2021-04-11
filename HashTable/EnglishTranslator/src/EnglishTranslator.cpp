#include <ctype.h>

#include "../../include/HashFunctionsCollection.hpp"
#include "../../include/ProcessDictionary.hpp"
#include "../../include/HashTable.hpp"

const char* const STANDART_TEXT_FILE = "Text.txt";
const char* const STANDART_HTML_FILE = "Translated.html";

size_t CountNumOfWords (text* text);
char** FillWords (text* text, size_t num_of_words);
void TranslateText (int argC, char** argV, hash_table* hash_table, text* text, const char* output_file_name = STANDART_HTML_FILE);
void strlwr (char* string);

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
    assert (text != nullptr);

    FILE* html_file = OpenFile (argC, argV, "out", "w", output_file_name);
    assert (html_file != nullptr);

    fprintf (html_file, "<meta charset=\"UTF-8\">\n<html>\n<body>\n<pre>\n");

    char word[100] = {};
    char orig_word[100] = {};

    //GetNextWord (&orig_word, text, html_file);

    for (size_t i = 0, length_of_line = 0; i < text->file_length; i++)
    {
        if (isalpha (text->pointer_on_buffer[i]))
        {
            if (length_of_line > 100) 
            {
                length_of_line = 0;
                fprintf (html_file, "\n");
            }

            fprintf (html_file, "<span title=\"");
            size_t num_of_letters = 1;
            while (isalpha (text->pointer_on_buffer[i + num_of_letters]))
            {
                num_of_letters++;
            }  

            strncpy (word, &text->pointer_on_buffer[i], num_of_letters);
            word[num_of_letters] = '\0';
            strlwr (word);

            strncpy (orig_word, &text->pointer_on_buffer[i], num_of_letters);
            orig_word[num_of_letters] = '\0';

            const char* translated = FindHashTable (hash_table, word);

            if (translated == nullptr)
            {
                if (num_of_letters > 1)
                {
                    word[num_of_letters-1] = '\0';
                    translated = FindHashTable (hash_table, word);
                    if (translated == nullptr)
                    {
                        if (num_of_letters > 2)
                        {
                            word[num_of_letters-2] = '\0';
                            translated = FindHashTable (hash_table, word);
                            if (translated == nullptr) translated = "ERROR";
                        }
                        else translated = "ERROR";
                    }
                }
                else translated = "ERROR";
            }

            fprintf (html_file, "%s\">%s</span>", translated, orig_word);

            i += num_of_letters - 1;
            length_of_line += num_of_letters;
        }
        else 
        {
            fprintf (html_file, "%c", text->pointer_on_buffer[i]);
            
            if (text->pointer_on_buffer[i] == '\n') length_of_line = 0;
            else                                    length_of_line++;
        }
    }

    fprintf (html_file, "\n</pre>\n</body>\n</html>");

}

/*void GetNextWord (char* orig_word, text* text, FILE* html)
{
    static size_t i = 0;
    static size_t length_of_line = 0;

    while (!isalpha (text->pointer_on_buffer[i]) 
    {

    }
}*/

void strlwr (char* string)
{
    assert (string != nullptr);

    for (; *string != '\0'; string++)
    {
        if (isupper (*string)) *string += 32; 
    }
}