#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//My Heroes:
//Elya
//Vasya
//Nikita
//Sergey
//Adam
//Ilya

struct str
    {
    char*  beginning = nullptr;
    size_t length = 0;
    };

struct for_text
    {
    FILE* input             = nullptr;
    FILE* output            = nullptr;
    size_t file_length      = 0;
    char* pointer_on_buffer = nullptr;
    size_t lines            = 0;
    str* struct_array       = nullptr;
    };

struct keys
    {
    size_t help    = 0;
    size_t in      = 0;
    size_t out     = 0;
    size_t reverse = 0;
    size_t front   = 0;
    size_t origin  = 0;
    size_t test    = 0;
    };

/*!
Function for searching a key in command line, returns number of next string after key in command line or 0
\param[in] argC      Number of strings in command line
\param[in] argV[]    Strings from command line
\param[in] key       Key function needs to find
*/

size_t get_arg_val (int argC, char* argV[], char* key);

/*!
Writes information about program for user
*/

void help_printf();

/*!
Function for reading names of files from command line
\param[in]      argC           Number of strings in command line
\param[in]      argV[]         Strings from command line
\param[in, out] hamlet         Pointer on struct with variables for work with text
\param[in, out] hamlet_keys    Pointer on struct with variables for work with command line
*/

void read_name_of_files (int argC, char* argV[], for_text* hamlet, keys* hamlet_keys);

/*!
Function to process information from input file, sort strings and write results of sorting
\param[in]      argC           Number of strings in command line
\param[in]      argV[]         Strings from command line
\param[in, out] hamlet         Pointer on struct with variables for work with text
\param[in, out] hamlet_keys    Pointer on struct with variables for work with command line
*/

void process_and_sort (int argC, char* argV[], for_text* hamlet, keys* hamlet_keys);

/*!
Measures the size of the origin, returns it
\param[in] input    Pointer on origin
*/

size_t find_length_of_file (FILE* input);

/*!
Copies the origin into buffer
\param[in]      input                Pointer to origin
\param[in]      pointer_on_buffer    Pointer to buffer
\param[in, out] file_length          Size of origin
*/

void copy_file (FILE* input, char* pointer_on_buffer, size_t* file_length);

/*!
Counts number of lines, returns number of lines
\param[in] pointer    Pointer to buffer with origin
\param[in] amount     Size of origin
*/

size_t count_lines (char* pointer, size_t amount);

/*!
Replaces c_old with c_new in buffer
\param[in] pointer    Pointer to buffer with origin
\param[in] amount     Size of origin
\param[in] c_old      Char, needed to be replaced
\param[in] c_new      Char, which will replace c_old
\param[in] lines      Number of lines
*/

void replace_char_from_buffer (char* pointer, size_t amount, char c_old, char c_new, size_t lines = 0);

/*!
Fills buffer of structs with pointers to beginnings of the lines and sizes of lines
\param[in]     struct_array              Pointer on beginning of buffer of structs
\param[in]     pointer_on_buffer         Pointer on buffer with origin
\param[in]     file_length               Size of origin
\param[in,out] lines                     Number of lines
*/

void fill_str_array (str* struct_array, char* pointer_on_buffer, size_t file_length, size_t* lines);

/*!
Quick sort for array of structs
\param[in] s_array    Pointer on the beginning of buffer with structs
\param[in] length     Number of elements in the buffer
\param[in] cmp        Comparator
*/

void sort_and_write (int argC, char* argV[], for_text* hamlet, keys* hamlet_keys);

/*!
Comparator for front sorting
\param[in] str1    First struct
\param[in] str2    Second struct
*/

void quick_sort (str* s_array, int length, int (*cmp)(const void* str1, const void* str2));

/*!
Function for sorting and writing results
\param[in]      argC           Number of strings in command line
\param[in]      argV[]         Strings from command line
\param[in, out] hamlet         Pointer on struct with variables for work with text
\param[in, out] hamlet_keys    Pointer on struct with variables for work with command line
*/

int front_compare (const void* str1, const void* str2);

/*!
Comparator for reverse sorting
\param[in] str1    First struct
\param[in] str2    Second struct
*/

int reverse_compare (const void* str1, const void* str2);

/*!
Universal function to swap things
\param[in] a1      Pointer on first thing
\param[in] a2      Pointer on second thing
\param[in] size    Size of the thing
*/

void swaper (void* a1, void* a2, size_t size);

/*!
Writes results of sorting
\param[in] str       Pointer on the beginning of buffer with structs
\param[in] lines     Number of lines
\param[in] output    Pointer on output file
*/

void write_result_sorted (str* str, size_t lines, FILE* output);

/*!
Writes origin
\param[in] pointer    Pointer on buffer with origin
\param[in] amount     Size of origin
\param[in] output     Pointer on output file
*/

void write_origin (char* pointer, size_t amount, FILE* output);

void test_me ();

void ftest (bool test);
