#define NDEBUG

#include <ctype.h>
#include <time.h>

#include "../../include/HashFunctionsCollection.hpp"
#include "../../include/ProcessDictionary.hpp"
#include "../../include/HashTable.hpp"

const char* const STANDART_TEXT_FILE = "Text.txt";
const char* const STANDART_HTML_FILE = "Translated.html";

const char* const ERROR_MSG = "ERROR";

const size_t MAX_LINE_LENGTH = 100;
const size_t MAX_WORD_LENGHT = 50;

struct translation_info
{
    char word_combination[2*MAX_WORD_LENGHT + 2] = {}; //+2 for ' ' and '/0'

    size_t index = 0;
    size_t old_index = 0;

    size_t line_length = 0;
    size_t length_of_word = 0;

    bool is_translation_ended = false;

    FILE* html_file = nullptr;
};

struct for_combination_word
{
    bool is_different_lines = false;

    size_t num_of_spaces = 0;
    size_t length_of_second_word = 0;

    const char* translated = nullptr;
};

void TranslateText (int argC, char** argV, hash_table* hash_table, text* text, const char* output_file_name = STANDART_HTML_FILE);
void ProcessTranslation (translation_info* translation_info, text* text, hash_table* hash_table);
void PrintSpaces (translation_info* translation_info, text* text);
void CheckLineOverflow (translation_info* translation_info);
size_t GetNextWord (char* orig_word, translation_info* translation_info, text* text);
void strlwr (char* string);
void ProcessAtypicalWord (translation_info* translation_info, text* text, hash_table* hash_table);
void SkipSpaces (translation_info* translation_info, text* text, for_combination_word* comb_word_info);
void ProcessShortening (translation_info* translation_info, text* text, hash_table* hash_table);
void PrintSingleWord (translation_info* translation_info, text* text, const char* translated);
void PrintTwoWords (translation_info* translation_info, text* text, for_combination_word* comb_word_info);
void PrintWord (text* text, FILE* html_file, size_t length, size_t* offset);
