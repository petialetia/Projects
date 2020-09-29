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

struct str
    {
    char*  beginning = nullptr;
    size_t length = 0;
    };

/*!
Writes information about program for user
*/

void help_printf();

/*!
Reads names of files program needs to read and write in
\param[in] argC             Number of strings in command line
\param[in] argV[]           Strings from command line
\param[in] in_fopen         Function to open file for reading or writing in
\param[in] str              String, program need to find
\param[in] standart_name    Name by default for file
*/

FILE* read_name_of_file (int argC, char* argV[], FILE* (*u_fopen)(char* filename), char* str, char* standart_name);

/*!
Opens file for reading, by default it is "hamlet.txt", returns pointer on it
\param[in] filename    Name of file program needs to read
*/

FILE* in_fopen  (char* filename = "Hamlet.txt");

/*!
Opens file for writing, by default it is "Sorted Hamlet.txt", returns pointer on it
\param[in] filename    Name of file program needs to write in
*/

FILE* out_fopen (char* filename = "Sorted Hamlet.txt");

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
Replaces '\n' with '\0' in buffer, returns number of lines
\param[in] pointer    Pointer to buffer with origin
\param[in] amount     Size of origin
*/

size_t replace_enter (char* pointer, size_t amount);

/*!
Fills buffer of structs with pointers to beginnings of the lines and sizes of lines
\param[in]     struct_array              Pointer on beginning of buffer of structs
\param[in]     pointer_on_buffer         Pointer on buffer with origin
\param[in]     file_length               Size of origin
\param[in,out] lines                     Number of lines
*/

void fill_struct_str_array (str* struct_array, char* pointer_on_buffer, size_t file_length, size_t* lines);

/*!
Function fo searching a specific string in command line, returns true, if finds, else returns false
\param[in] argC    Number of strings in command line
\param[in] argV    Strings from command line
\param[in] str     String program needs to find
*/

bool find_string (int argC, char* argV[], char* str);

/*!
Quick sorts array of structs
\param[in] s_array    Pointer on the beginning of buffer with structs
\param[in] length     Number of elements in the buffer
\param[in] cmp        Comparator
\param[in] swaper     Function to swap structs
*/

void quick_sort (str* s_array, int length, int (*cmp)(str str1, str str2), void (*swaper)(str* s1, str* s2));

/*!
Comparator for front sorting
\param[in] str1    First struct
\param[in] str2    Second struct
*/

int front_compare (str str1, str str2);

/*!
Comparator for reverse sorting
\param[in] str1    First struct
\param[in] str2    Second struct
*/

int reverse_compare (str str1, str str2);

/*!
Function to swap structs
\param[in] s1    Pointer on first struct
\param[in] s2    Pointer on second struct
*/

void swaper (str* s1, str* s2);

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

void ftest (bool test);
