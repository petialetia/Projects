#define NDEBUG

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const char* const KEYS_IN = "in";

struct text
{
    size_t file_length      = 0;
    char* pointer_on_buffer = nullptr;
};

struct keys
{
    size_t in  = 0;
};

void ProcessInput (int argC, char* argV[], text* input_text, const char* name_of_input_file);

size_t GetArgVal (int argC, char* argV[], const char* key);

size_t FindLengthOfFile (FILE* input);

void CopyFileInBuffer (FILE* input, char* pointer_on_buffer, size_t* file_length);

size_t CountNumOfLines (text* text);

void DestroyText (text* text);

FILE* OpenFile (int argC, char* argV[], const char* console_flag, const char* open_flag, const char* name_of_file = nullptr);
