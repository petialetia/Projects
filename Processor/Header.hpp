#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include "Safe stack.cpp"

const size_t size_of_header = 7;

const size_t max_num_of_labels = 10;

const size_t max_length_of_label = 10;




enum Commands
{

    #define DEF_CMD(name, num, asm_cmd, disasm_cmd, cpu_cmd)\
    CMD_##name = num,


    #include "Commands.hpp"

    #undef DEF_CMD
};



struct str
{
    char*  beginning = nullptr;
    size_t length = 0;
};

struct text
{
    size_t file_length      = 0;
    char* pointer_on_buffer = nullptr;
    size_t lines            = 0;
    str* struct_array       = nullptr;
};

struct keys
{
    size_t in  = 0;
    size_t out = 0;
};

struct CPU_struct
{
    Stack_t stack;
    Stack_t stack_of_calls;
    char* code = nullptr;
    size_t ofs = 0;
    double reg[4];
};

struct label
{
    char addr = -1;
    char* name = nullptr;
};

text* construct_text (                 size_t file_length = 0, char* pointer_on_buffer = nullptr, size_t lines = 0, str* struct_array = nullptr);
text* construct_text (text* text_file, size_t file_length = 0, char* pointer_on_buffer = nullptr, size_t lines = 0, str* struct_array = nullptr);
void process_files (int argC, char* argV[], text* text_file, keys* hamlet_keys);

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

void read_file (FILE* input, char* pointer_on_buffer, size_t* file_length);

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
\param[in] c_new      Char, which will replace c_old
\param[in] c_old1     First char, needed to be replaced
\param[in] c_old2     Second char, needed to be replaced
*/

size_t replace_char_from_buffer (char* pointer, size_t amount,  size_t lines, char c_new, char c_old1, char c_old2);

/*!
Fills buffer of structs with pointers to beginnings of the lines and sizes of lines
\param[in]     struct_array              Pointer on beginning of buffer of structs
\param[in]     pointer_on_buffer         Pointer on buffer with origin
\param[in]     file_length               Size of origin
\param[in,out] lines                     Number of lines
*/

void fill_str_array (str* struct_array, char* pointer_on_buffer, size_t file_length, size_t* lines);

void destroy_text (text* text_file);

void destroy_keys (keys* keys);

text* construct_text (size_t file_length, char* pointer_on_buffer, size_t lines, str* struct_array)
{
    text* text_file = (text*) calloc (1, sizeof(text));
    assert (text_file != nullptr);


    text_file->file_length       = file_length;
    text_file->pointer_on_buffer = pointer_on_buffer;
    text_file->lines             = lines;
    text_file->struct_array      = struct_array;


    return text_file;
}

text* construct_text (text* text_file, size_t file_length, char* pointer_on_buffer, size_t lines, str* struct_array)
{

    text_file->file_length       = file_length;
    text_file->pointer_on_buffer = pointer_on_buffer;
    text_file->lines             = lines;
    text_file->struct_array      = struct_array;

    return text_file;
}

size_t get_arg_val (int argC, char* argV[], char* key)
{
    assert (argC != 0);
    assert (key != nullptr);


    for (size_t cnt = 1; cnt < argC; cnt++)
    {
        if (strcmp (argV[cnt], key) == 0)
        {
            return cnt + 1;
        }
    }

    return 0;
}

size_t find_length_of_file (FILE* input)
{
    assert (input != nullptr);

    fseek (input, 0, SEEK_END);
    int length = ftell (input);
    assert (length != 0);

    fseek (input, 0, SEEK_SET);
    return length;
}

void read_file (FILE* input, char* pointer_on_buffer, size_t* file_length)
{
    assert (input != nullptr);
    assert (pointer_on_buffer != nullptr);
    assert (file_length != 0);

    size_t size = fread (pointer_on_buffer, sizeof(char), *file_length, input);
    assert (size != 0);

    *file_length = size+1;
}

size_t replace_char_from_buffer (char* pointer, size_t amount,  size_t lines, char c_new, char c_old1, char c_old2)
{
    assert (pointer != nullptr);
    assert (amount != 0);

    size_t i = 0;

    for (;amount > 0; amount--, pointer++)
    {
        if ((*pointer == c_old1) || (*pointer == c_old2))
        {
            *pointer = c_new;
            i++;
        }
    }

    return i;
}

size_t count_lines (char* pointer, size_t amount)
{
    assert (pointer != nullptr);
    assert (amount != 0);

    size_t lines = 0;

    for (;amount > 0; amount--, pointer++)
        {
        if (*pointer == '\0')
            {
            lines++;
            }
        }

    assert ((lines + 1) != 0);
    return lines + 1;
}

void fill_str_array (str* struct_array, char* pointer_on_buffer, size_t file_length, size_t* lines)
{
    assert (struct_array != nullptr);
    assert (pointer_on_buffer != nullptr);
    assert (file_length != 0);
    assert (lines != 0);

    size_t cnt = 0;

    for (size_t size_of_line = 0; file_length > 0; file_length--, pointer_on_buffer++)
    {
        if (*pointer_on_buffer != '\0')
        {
            struct_array[cnt].beginning = pointer_on_buffer;
        }
        for (; *pointer_on_buffer != '\0'; pointer_on_buffer++, size_of_line++, file_length--)
        {
        }
        if (struct_array[cnt].beginning != nullptr)
        {
            struct_array[cnt].length = size_of_line;
            size_of_line = 0;
            cnt++;
        }
    }

    assert (cnt != 0);

    *lines = cnt;
}

void destroy_text (text* text_file)
{
    free (text_file->pointer_on_buffer);
    free (text_file->struct_array);
    free (text_file);
}

void destroy_keys (keys* keys)
{
    free (keys);
}
