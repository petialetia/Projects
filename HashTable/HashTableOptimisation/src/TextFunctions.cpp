#include "../include/TextFunctions.hpp"

void ProcessInput (int argC, char* argV[], text* input_text, const char* name_of_input_file)
{
    assert (argC               != 0);
    assert (argV               != nullptr);
    assert (input_text         != nullptr);
    assert (name_of_input_file != nullptr);

    keys input_keys = {};

    input_keys.in = GetArgVal (argC, argV, KEYS_IN);

    FILE* input = nullptr;

    if (name_of_input_file != nullptr) input = fopen (name_of_input_file, "r");
    else
    {
        if (input_keys.in != 0) input = fopen (argV[input_keys.in], "r");
        else
        {
            printf ("No input file\n");
            return;
        }
    } 

    input_text->file_length = FindLengthOfFile (input);

    input_text->pointer_on_buffer = (char*) calloc (input_text->file_length + 1, sizeof(char));

    CopyFileInBuffer (input, input_text->pointer_on_buffer, &(input_text->file_length));

    fclose (input);
}

size_t GetArgVal (int argC, char* argV[], const char* key)
{
    assert (argC != 0);
    assert (argV != nullptr);
    assert (key  != nullptr);


    for (int cnt = 1; cnt < argC; cnt++)
    {
        if (strcmp (argV[cnt], key) == 0)
        {
            return cnt + 1;
        }
    }

    return 0;
}

size_t FindLengthOfFile (FILE* input)
{
    assert (input != nullptr);

    fseek (input, 0, SEEK_END);
    int size = ftell (input);
    assert (size != 0);

    fseek (input, 0, SEEK_SET);
    return size;
}

void CopyFileInBuffer (FILE* input, char* pointer_on_buffer, size_t* file_length)
{
    assert (input != nullptr);
    assert (pointer_on_buffer != nullptr);
    assert (file_length != 0);

    size_t size = fread (pointer_on_buffer, sizeof(char), *file_length, input);
    assert (size != 0);

    *file_length = size+1;
}

size_t CountNumOfLines (text* text)
{
    assert (text != nullptr);

    size_t num_of_lines = 1;

    for (size_t i = 0; i < text->file_length; i++)
    {
        if (text->pointer_on_buffer[i] == '\n') num_of_lines++;
    }

    return num_of_lines;
}

void DestroyText (text* text)
{
    free (text->pointer_on_buffer);
}

FILE* OpenFile (int argC, char* argV[], const char* console_flag, const char* open_flag, const char* name_of_file)
{
    assert (argV != nullptr);
    assert (open_flag != nullptr);

    FILE* file = nullptr;

    size_t key = 0;

    if (console_flag != nullptr) key = GetArgVal (argC, argV, console_flag);

    if (key != 0) file = fopen (argV[key], open_flag);
    else
    {
        if (name_of_file != nullptr) file = fopen (name_of_file, open_flag);
        else                         printf ("There is no file to open\n");
    }

    return file;
}