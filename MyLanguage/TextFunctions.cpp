#include "TextFunctions.hpp"

void ProcessInput (int argC, char* argV[], text* input_text, const char* name_of_input_file)
{
    assert (argC               != 0);
    assert (argV               != nullptr);
    assert (input_text         != nullptr);
    assert (name_of_input_file != nullptr);

    keys input_keys = {};

    input_keys.in = GetArgVal (argC, argV, KEYS_IN);

    FILE* input = (input_keys.in != 0) ? (fopen (argV[input_keys.in], "r")) : (fopen (name_of_input_file, "r"));

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
