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
Opens file for reading, by default it is "hamlet.txt", returns pointer on it
\param[in] filename    Name of file program needs to read
*/

FILE* in_fopen  (char* filename = "hamlet.txt");

/*!
Opens file for writing, by default it is "Sorted Hamlet.txt", returns pointer on it
\param[in] filename    Name of file program needs to write in
*/

FILE* out_fopen (char* filename = "Sorted Hamlet.txt");

/*!
Measures the size of the origin, returns it
\param[in] input    Pointer to origin
*/

size_t measure_file (FILE* input);

/*!
Copies the origin into buffer
\param[in]      input      Pointer to origin
\param[in]      pointer    Pointer to buffer
\param[in, out] amount     Size of origin
*/

void copy_file (FILE* input, char* pointer, size_t* amount);

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
\param[in]     struct_array    Pointer to beginning of buffer of structs
\param[in]     pointer         Pointer to buffer with origin
\param[in]     amount          Size of origin
\param[in,out] lines           Number of lines
*/

void fill_struct_str_array (str* struct_array, char* pointer, size_t amount, size_t* lines);

/*!
Creates same buffer with structs for revers sorting, returns pointer on it
\param[in] lines            Number of lines
\param[in] struct_array     Pointer on beginning of buffer of structs
\param[in] reverse_array    Pointer on beginning of buffer for reverse sorting
*/

void copy_struct_array (size_t lines, str* struct_array, str* reverse_array);

/*!
Quick sorts array of structs
\param[in] s         Pointer on the beginning of buffer with structs
\param[in] last      Number of elements in the buffer
\param[in] cmp       Comparator
\param[in] swaper    Function to swap structs
*/

void quick_sort (str* s, int last, int (*cmp)(str str1, str str2), void (*swaper)(str* s1, str* s2));

/*!
Comparator for front sorting
\param[in] str1    Pointer to first struct
\param[in] str2    Pointer to second struct
*/

int front_compare (str str1, str str2);

/*!
Comparator for reverse sorting
\param[in] str1    Pointer on first struct
\param[in] str2    Pointer on second struct
*/

int reverse_compare (str str1, str str2);

/*!
Function to swap structs
\param[in] s1    Pointer to first struct
\param[in] s2    Pointer to second struct
*/

void swaper (str* s1, str* s2);

/*!
Writes results of sorting
\param[in] pointer          Pointer on buffer with origin
\param[in] amount           Size of origin
\param[in] struct_array     Pointer on the beginning of buffer with structs
\param[in] reverse_array    Pointer on the buffer with structs for revers sorting
\param[in] lines            Number of lines
\param[in] output           Pointer on output file
*/

void write_result (char* ponter, size_t amount, str* struct_array, str* reverse_array, size_t lines, FILE* output);

void ftest (bool test);
